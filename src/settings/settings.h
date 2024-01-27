#pragma once

#include <Arduino.h>
#include <vector>
#include "components/category.h"

class Settings
{
public:
    Settings();

    void setup();
    void draw();
    void update();

    void handleUpButton();
    void handleDownButton();
    void handlePowerButton();

    int brightness = 5;

private:
    std::vector<Category> categories;
    int hoveredCategory = 0;

    int selectedCategory = -1;
};

extern Settings settings;