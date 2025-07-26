#!/bin/bash

echo "Building TalorikAgent..."

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the project
make -j$(nproc)

echo "Build completed!" 