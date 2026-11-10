#include <array>
#include <cerrno>
#include <crt_externs.h>
#include <cstddef>
#include <cstring>
#include <datagen/fm/client.hpp>
#include <format>
#include <spawn.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

namespace datagen::fm {
namespace {
constexpr std::size_t read_buffer_size = 4096;
} // namespace

std::expected<std::string, Error> Client::generate(const Request& request) {
    std::array<int, 2> pipe_fd{};

    if (pipe(pipe_fd.data()) == -1) {
        return std::unexpected{Error{
            .message = std::format("Failed to create pipe: {}", std::string{std::strerror(errno)}),
        }};
    }

    posix_spawn_file_actions_t actions{};
    auto result = posix_spawn_file_actions_init(&actions);

    if (result != 0) {
        close(pipe_fd.at(0));
        close(pipe_fd.at(1));

        return std::unexpected{Error{
            .message =
                std::format("Failed to spawn actions: {}", std::string{std::strerror(result)}),
        }};
    }

    posix_spawn_file_actions_adddup2(&actions, pipe_fd.at(1), STDOUT_FILENO);
    posix_spawn_file_actions_addclose(&actions, pipe_fd.at(0));
    posix_spawn_file_actions_addclose(&actions, pipe_fd.at(1));

    const auto schema = request.schema.string();

    std::vector<std::string> arguments{
        "fm",   "respond",        "--no-stream",        "--schema",
        schema, "--instructions", request.instructions, request.prompt,
    };

    std::vector<char*> argv;
    argv.reserve(arguments.size() + 1);

    for (auto& argument : arguments) {
        argv.push_back(argument.data());
    }

    argv.push_back(nullptr);

    pid_t pid{};
    char** const environment = *_NSGetEnviron();
    result = posix_spawnp(&pid, "fm", &actions, nullptr, argv.data(), environment);

    posix_spawn_file_actions_destroy(&actions);
    close(pipe_fd.at(1));

    if (result != 0) {
        close(pipe_fd.at(0));

        return std::unexpected{Error{
            .message = std::format("Failed to launch fm: {}", std::string{std::strerror(result)}),
        }};
    }

    std::string output;
    std::array<char, read_buffer_size> buffer{};

    while (true) {
        const auto bytes_read = read(pipe_fd.at(0), buffer.data(), buffer.size());
        if (bytes_read > 0) {
            output.append(buffer.data(), bytes_read);
        } else if (bytes_read == 0) {
            break;
        } else if (errno != EINTR) {
            const int read_errno = errno;
            close(pipe_fd.at(0));

            int status{};
            while (waitpid(pid, &status, 0) == -1 && errno == EINTR) {
            }

            return std::unexpected{Error{
                .message = std::format("Failed to read fm output: {}",
                                       std::string{std::strerror(read_errno)}),
            }};
        }
    }

    close(pipe_fd.at(0));

    int status{};
    auto wait_result = waitpid(pid, &status, 0);

    while (wait_result == -1 && errno == EINTR) {
        wait_result = waitpid(pid, &status, 0);
    }

    if (wait_result == -1) {
        return std::unexpected{Error{
            .message = std::format("Failed to wait for fm: {}", std::string{std::strerror(errno)}),
        }};
    }

    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        return std::unexpected{Error{
            .message = "fm process failed",
            .exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1,
        }};
    }

    return output;
}
} // namespace datagen::fm
