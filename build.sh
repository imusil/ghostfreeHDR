#!/bin/sh
rm CMakeCache.txt
rm -r CMakeFiles
cmake CMakeLists.txt -DCMAKE_BUILD_TYPE=Release
make -j4
mkdir results
