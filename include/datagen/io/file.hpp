#pragma once

#include <expected>
#include <filesystem>
#include <string>

#include <nlohmann/json.hpp>

namespace datagen::io {

namespace fs = std::filesystem;

using Json = nlohmann::json;

struct FileError {
    fs::path path;
    std::string message;
};

[[nodiscard]] std::expected<std::string, FileError> read_text(const fs::path& path);

[[nodiscard]] std::expected<Json, FileError> read_json(const fs::path& path);

} // namespace datagen::io
