cmake_minimum_required(VERSION 4.0)

project(datagen
    VERSION 1.0
    DESCRIPTION "Data generation tool"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(datagen
    src/main.cpp
)

target_compile_options(datagen PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)
