#include <datagen/cli/generate.hpp>
#include <datagen/io/file.hpp>
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
    auto instructions = io::read_text(instructions_);

    if (!instructions) {
        std::println(stderr, "Error reading instructions: {} : {}",
                     instructions.error().path.string(), instructions.error().message);
        return;
    }

    auto schema = io::read_json(schema_);

    if (!schema) {
        std::println(stderr, "Error reading schema: {} : {}", schema.error().path.string(),
                     schema.error().message);
        return;
    }

    if (!schema->is_object()) {
        std::println(stderr, "Error: JSON schema must be an object");
        return;
    }

    std::println("Instructions loaded: {} bytes", instructions->size());
    std::println("Schema loaded successfully");
    std::println("Records requested: {}", count_);
}

} // namespace datagen::cli
