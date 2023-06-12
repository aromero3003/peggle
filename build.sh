#!/bin/bash

if [ $# -gt 1 ] || [ $1 != "-r" ] ; then
    echo "error: usage: ./build.sh [-r]"
    exit 1
fi
if [ $# -eq 1 ] && [ $1 = "-r" ]; then
    if [ -d "build" ]; then
        rm -r build
        mkdir build
    fi

fi
cmake -S . -B build
cmake --build build
