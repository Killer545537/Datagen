cmake_minimum_required(VERSION 4.0)

project(datagen
    VERSION 1.0
    DESCRIPTION "Data generation tool"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11
    GIT_TAG v2.5.0
    GIT_SHALLOW True
)

FetchContent_MakeAvailable(CLI11)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
    GIT_SHALLOW True
)

FetchContent_MakeAvailable(nlohmann_json)

add_executable(datagen
    src/main.cpp
    src/cli/generate.cpp
    src/io/file.cpp
)

target_include_directories(datagen PRIVATE
    include
)

target_link_libraries(datagen PRIVATE
    CLI11::CLI11
    nlohmann_json::nlohmann_json
)

target_compile_options(datagen PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)
