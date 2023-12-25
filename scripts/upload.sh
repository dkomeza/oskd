#!/bin/bash

cd ..
echo "Uploading..."

pio run -e esp32 --target upload

echo "Upload complete!"

cd scripts