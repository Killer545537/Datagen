#include <datagen/cli/generate.hpp>
#include <print>

namespace datagen::cli {
GenerateCommand::GenerateCommand(CLI::App& app) {
    auto* command = app.add_subcommand("generate", "Generate structured data");
    command->add_option("-s,--schema", schema_, "Path to JSON schema")
        ->required()
        ->check(CLI::ExistingFile);

    command->add_option("-i,--instructions", instructions_, "Path to generation instructions")
        ->required()
        ->check(CLI::ExistingFile);

    command->add_option("-n,--count", count_, "Number of records")->check(CLI::PositiveNumber);

    command->add_option("-o,--output", output_, "Output file (default: stdout)");

    command->callback([this] {
        this->execute();
    });
}

void GenerateCommand::execute() const {
    std::println("Schema: {}", schema_.string());
    std::println("Instructions: {}", instructions_.string());
    std::println("Count: {}", count_);
    std::println("Output: {}", output_.string());
}

} // namespace datagen::cli
