cmake_minimum_required(VERSION 3.22)
project(bf_interpreter CXX)

add_executable(interpreter interpreter.cpp)
target_compile_features(interpreter PUBLIC cxx_std_20)
target_compile_options(interpreter PUBLIC -O2 -Wall -Wextra -Wpedantic)
add_subdirectory(libs)

if(DEBUG)
    target_compile_options(interpreter PUBLIC -g)
endif(DEBUG)
