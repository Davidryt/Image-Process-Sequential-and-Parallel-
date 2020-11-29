cmake_minimum_required(VERSION 3.17)
project(Arqui2020)

set(CMAKE_CXX_STANDARD 17)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17 -Wall -Wextra -Wno-deprecated -Werror -pedantic -pedantic-errors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(Arqui2020
        main.cpp bmp.h)
