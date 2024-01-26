#pragma once

#include <Arduino.h>
#include "views/dashboard.h"

enum class View
{
    Dashboard,
    Settings,
};

namespace screen
{
    extern View view;

    void setup();
    void lightUp();
    void shutdown();

    void draw();
    void loop(bool force = false);

    void setView(View view);
}