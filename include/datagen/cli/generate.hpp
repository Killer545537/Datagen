#pragma once

#include <CLI/CLI.hpp>
#include <cstddef>
#include <filesystem>

namespace datagen::cli {

namespace fs = std::filesystem;

class GenerateCommand final {
public:
    explicit GenerateCommand(CLI::App& app);

private:
    void execute() const;

    fs::path schema_;
    fs::path instructions_;
    fs::path output_;

    std::size_t count_{1};
};
} // namespace datagen::cli
