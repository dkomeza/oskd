#pragma once

#include <Arduino.h>

enum class View
{
    Dashboard,
    Settings,
};

namespace screen
{
    extern View view;

    void setup();
    void draw();
    void loop(bool force = false);

    void setView(View view);
    void updateLegalMode();
}