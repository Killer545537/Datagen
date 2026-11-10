#include <datagen/cli/generate.hpp>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    CLI::App app{"Generate structured data using Apple's Foundation Models"};

    app.require_subcommand(1);

    datagen::cli::GenerateCommand generate_command{app};

    CLI11_PARSE(app, argc, argv);

    return EXIT_SUCCESS;
}
