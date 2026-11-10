#include <datagen/io/file.hpp>
#include <fstream>
#include <iterator>

namespace datagen::io {
std::expected<std::string, FileError> read_text(const fs::path& path) {
    std::ifstream file{path, std::ios::binary};

    if (!file.is_open()) {
        return std::unexpected(FileError{.path = path, .message = "Unable to open file"});
    }

    std::string content{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};
    if (file.bad()) {
        return std::unexpected(FileError{.path = path, .message = "Failed while reading file"});
    }

    return content;
}

std::expected<Json, FileError> read_json(const fs::path& path) {
    auto content = read_text(path);

    if (!content) {
        return std::unexpected(std::move(content.error()));
    }

    try {
        return Json::parse(*content);
    } catch (const Json::parse_error& error) {
        return std::unexpected(FileError{.path = path, .message = error.what()});
    }
}

} // namespace datagen::io
