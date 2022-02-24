#!/bin/sh

mkdir -p build
cd build
cmake ../test
make -j4
ctest
