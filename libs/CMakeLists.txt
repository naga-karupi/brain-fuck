cmake_minimum_required(VERSION 3.22)
project(compiling CXX)

add_library(file_exists STATIC file_exists_check.cpp)
target_compile_features(file_exists PUBLIC cxx_std_20)
target_compile_options(file_exists PUBLIC -O2 -Wall -Wextra -Wpedantic)
