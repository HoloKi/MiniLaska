cmake_minimum_required(VERSION 3.17)
project(laskaprivate C)

set(CMAKE_C_STANDARD 99)

add_executable(laskaprivate main.c laska.c laska.h)