#pragma once

#include <Arduino.h>

namespace screen
{
    void setup();
    void lightUp();
    void shutdown();
    void changeBrightness(int oldValue, int newValue);
}