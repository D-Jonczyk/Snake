cmake_minimum_required(VERSION 3.10)

project(Snake C)

set(CMAKE_C_STANDARD 99)

file(GLOB SOURCES "*.c")

add_executable(Snake ${SOURCES})
