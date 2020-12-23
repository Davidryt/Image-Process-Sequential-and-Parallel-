cmake_minimum_required(VERSION 3.17)
project(Arqui2020)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Arqui2020
        bmp.h
        main.cpp)
