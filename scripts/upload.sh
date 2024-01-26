#!/bin/bash

cd ..
echo "Uploading..."

# Test for connection to OSKD.local
# If no connection, exit script
if ! ping -c 1 OSKD.local &> /dev/null
then
    echo "OSKD.local not found"
    exit
fi

pio run -e esp32 --target upload

echo "Upload complete!"

cd scripts