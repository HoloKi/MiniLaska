cmake_minimum_required(VERSION 3.17)
project(minilaska C)

set(CMAKE_C_STANDARD 99)

add_executable(minilaska main.c laska.h laska.c)