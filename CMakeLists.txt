cmake_minimum_required(VERSION 3.14)
project(bm25)

set(CMAKE_CXX_STANDARD 11)

add_executable(bm25 main.cpp Parser.h Parser.cpp)