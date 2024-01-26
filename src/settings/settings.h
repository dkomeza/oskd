#pragma once

#include <Arduino.h>
#include <vector>

class Setting
{
public:
    Setting(String name, String description);
};

class Category
{
public:
    Category(String name)
    {
        this->name = name;
    };

    void addSetting(Setting setting)
    {
        this->settings.push_back(setting);
    };

    void renderHeader();
    void render();

private:
    String name;
    std::vector<Setting> settings;
};

class Settings
{
public:
    Settings(){};

    void setup();

private:
};

extern Settings settings;