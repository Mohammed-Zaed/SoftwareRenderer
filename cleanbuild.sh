#!/bin/sh
rm -r build
mkdir build && cd build
cmake -G "Unix Makefiles" .. && cmake --build . && ./renderer && cd ..
