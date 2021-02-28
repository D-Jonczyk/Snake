cmake_minimum_required(VERSION 3.10)

project(Snake C)

set(CMAKE_C_STANDARD 99)

if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -w")
endif()
if ( MSVC )
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} /W0")
endif()

add_executable(Snake collisions.c dialogs.c engine.c food_point.c init_functions.c main.c)
