cmake_minimum_required(VERSION 3.25)
project(Szeregowanie LANGUAGES CXX)

# --- Standard C++ dla całego projektu ---------------------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# compile_commands.json -> clangd wie, jak kompilowany jest każdy plik
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Wspólne flagi ostrzeżeń jako "target" ----------------------------------
add_library(project_warnings INTERFACE)
target_compile_options(project_warnings INTERFACE
    -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion
)

# --- Podprojekty -------------------------------------------------------------
add_subdirectory(common)
add_subdirectory(zad1)
