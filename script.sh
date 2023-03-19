#!/bin/bash
echo   "Build project"
mkdir -p build
cd build
cmake ..
make 
exec ./SIGNAL
