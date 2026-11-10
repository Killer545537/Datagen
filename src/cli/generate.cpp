#include <datagen/cli/generate.hpp>
#include <datagen/fm/client.hpp>
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

    auto results = io::Json::array();

    for (std::size_t i = 0; i < count_; ++i) {
        std::println(std::cerr, "Generating {}/{}", i + 1, count_);

        auto response = datagen::fm::Client::generate({
            .schema = fs::absolute(schema_),
            .instructions = *instructions,
            .prompt = "Generate one new, distinct fictional record",
        });

        if (!response) {
            std::println(stderr, "Error: {}", response.error().message);
            return;
        }

        try {
            results.push_back(io::Json::parse(*response));
        } catch (const io::Json::parse_error& error) {
            std::println(stderr, "Error parsing response: {}", error.what());
            return;
        }
    }

    const auto serialized = results.dump(2) + '\n';

    if (output_.empty()) {
        std::print(std::cout, "{}", serialized);
        return;
    }

    std::ofstream file{output_, std::ios::binary};
    file << serialized;
    file.flush();

    if (!file) {
        std::println(stderr, "Failed to write output: {}", output_.string());
        return;
    }

    std::println(std::cerr, "Wrote {} records to {}", results.size(), output_.string());
}

} // namespace datagen::cli
