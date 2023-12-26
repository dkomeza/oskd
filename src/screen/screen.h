#pragma once

#include <Arduino.h>

enum class View {
    Dashboard,
    Settings,
};

namespace screen
{
    void setup();
    void loop();

    void setView(View view);
}