#pragma once

#include <Arduino.h>

namespace dashboard
{
    void draw();

    void update(bool force = false);
    void updateLegalMode();
}