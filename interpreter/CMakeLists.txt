cmake_minimum_required(VERSION 3.22)
project(bf_interpreter CXX)

add_executable(interpreter interpreter.cpp)
target_compile_features(interpreter PUBLIC cxx_std_20)
target_compile_options(interpreter PUBLIC -O2 -Wall -Wextra -Wpedantic)

target_include_directories(interpreter PUBLIC ../libs)
target_link_directories(interpreter PUBLIC ../libs/build)
target_link_libraries(interpreter file_exists)

if(DEBUG)
    target_compile_options(interpreter PUBLIC -g)
endif(DEBUG)
