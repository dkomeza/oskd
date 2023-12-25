#!/bin/bash

cd ..
echo "Building..."

pio run -e esp32

echo "Build complete!"

cd scripts