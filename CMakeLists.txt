cmake_minimum_required(VERSION 3.17)
project(OSTEP C)

set(CMAKE_C_STANDARD 99)


add_subdirectory(chapter6)

add_executable(OSTEP main.c)