cmake_minimum_required(VERSION 4.2)
project(Allocating_resources C)

set(CMAKE_C_STANDARD 11)

add_executable(Allocating_resources main.c
        functions.c
        functions.h)
