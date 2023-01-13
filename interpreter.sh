#!/usr/bin/bash

cd libs
cmake -S . -B build
cd build 
make
cd ../../

cd interpreter
cmake -S . -B build
cd build 
make
