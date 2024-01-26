#pragma once

#include <Arduino.h>
#include <vector>

typedef void (*CustomRender)(void);

class Setting
{
public:
    Setting(String name, String description, int *value, int min, int max);

    void renderHeader();
    void draw();

    void setCustomRender(CustomRender customRender);

protected:
    String name;
    String description;
    int *value;
    int min;
    int max;
    CustomRender customRender;
};
;

class SettingToggle : public Setting
{
public:
    SettingToggle(String name, String description, bool *value);
};

class SettingSelect : public Setting
{
public:
    SettingSelect(String name, String description, int *value, std::vector<std::pair<String, int>> options);
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

    void drawHeader();
    void draw();

    String name;

private:
    std::vector<Setting> settings;
};

class Settings
{
public:
    Settings();

    void setup();
    void draw();
    void update();

    int brightness = 255;

private:
    std::vector<Category> categories;

    void createGeneralCategory();
    void createMotorCategory();
    void createPasCategory();
    void createBatteryCategory();
    void createThrottleCategory();
    void createDisplayCategory();
};

extern Settings settings;