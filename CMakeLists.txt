cmake_minimum_required(VERSION 3.14)

project(echo-server)

add_subdirectory("src")

# Generate compile_commands.json and specify path to it
# for clangd LSP server (for Neovim in my case)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
configure_file(
    ${CMAKE_SOURCE_DIR}/.clangd.in
    ${CMAKE_SOURCE_DIR}/.clangd
)

