cmake_minimum_required(VERSION 3.15)
project(pascal2c C)

set(CMAKE_C_STANDARD 11)
include_directories(3rd_parties)
add_executable(pascal2c 3rd_parties/sds.c src/main.c src/ast.c src/ast.h)