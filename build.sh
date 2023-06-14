#!/bin/bash
#
# Function to display help information
show_help() {
    echo "Usage: build.sh [-r]"
    echo "Options:"
    echo "  --help, -h       Display this help message"
    echo "  --rebuild, -r    Delete build and build from zero"
}

# Check all script arguments for help flag
for arg in "$@"; do
    if [[ "$arg" == "--help" || "$arg" == "-h" ]]; then
        show_help
        exit 0
    fi
done

if [ $# -gt 1 ] || [ $1 != "-r" ] ; then
    echo "Usage: ./build.sh [-r]"
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
