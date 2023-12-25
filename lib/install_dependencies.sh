#!/bin/bash

# Install dependencies
echo "Installing dependencies..."
git clone https://github.com/Bodmer/TFT_eSPI.git
echo "Dependencies installed!"

# Copy the user_setup.h file to the TFT_eSPI library folder
echo "Copying User_Setup.h file to TFT_eSPI library folder..."
cp User_Setup.h TFT_eSPI/User_Setup.h
echo "User_Setup.h file copied!"