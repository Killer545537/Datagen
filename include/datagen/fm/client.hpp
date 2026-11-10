#pragma once

#include <expected>
#include <filesystem>
#include <string>

namespace datagen::fm {
namespace fs = std::filesystem;
struct Request {
    fs::path schema;
    std::string instructions;
    std::string prompt;
};

struct Error {
    std::string message;
    int exit_code{-1};
};

class Client {
public:
    [[nodiscard]] static std::expected<std::string, Error> generate(const Request& request);
};
} // namespace datagen::fm
