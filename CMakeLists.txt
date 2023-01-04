cmake_minimum_required(VERSION 3.22)
project(compiling CXX)

add_executable(compiler compiler.cpp)
target_compile_features(compiler PUBLIC cxx_std_20)
target_compile_options(compiler PUBLIC -O2 -Wall -Wextra -Wpedantic)
add_subdirectory(libs)