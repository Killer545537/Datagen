# datagen

A command-line tool that generates structured JSON data. You give it a JSON schema and a short text file of instructions, and it asks Apple's on-device model (through the `fm` CLI) to produce records that match the schema.

## Usage

```sh
./build/datagen generate \
    -s schemas/person.json \
    -i instructions/person.txt \
    -n 5 \
    -o person.json
```

| Option | Description |
| --- | --- |
| `-s, --schema` | Path to the JSON schema (required) |
| `-i, --instructions` | Path to a text file describing what to generate (required) |
| `-n, --count` | Number of records to generate (default: 1) |
| `-o, --output` | Output file (default: stdout) |

The output is a JSON array containing one object per record:

```json
[
  { "age": 28, "name": "Elena Martinez" },
  { "age": 27, "name": "Lila Nguyen" }
]
```

## Requirements

- macOS on Apple silicon, with the `fm` command available on your `PATH`
- CMake 4.0 or newer
- A C++26 compiler (the project is built with Homebrew LLVM `clang++`)
- Ninja (recommended)
- Internet access on the first configure, to download dependencies

## Build

```sh
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++
cmake --build build
```

The binary is written to `build/datagen`.

Warnings are treated as errors (`-Wall -Wextra -Wpedantic -Werror`).

## Tech stack

- C++26
- CMake, with dependencies fetched through `FetchContent`
- [CLI11](https://github.com/CLIUtils/CLI11) v2.5.0 for command-line parsing
- [nlohmann/json](https://github.com/nlohmann/json) v3.12.0 for JSON handling
- `fm` CLI for text generation
