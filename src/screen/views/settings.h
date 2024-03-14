#include <Arduino.h>
#include <vector>

class Setting
{
};

class Category
{
public:
    Category(String name) : name(name), isHovered(false) {}
    Category(String name, bool isHovered) : name(name), isHovered(hovered) {}

    void setPos(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    void setSizes(int width, int height)
    {
        this->width = width;
        this->height = height;
    }

    void addSetting(Setting setting) { settings.push_back(setting); }
    void draw();
    void drawHeader();

private:
    String name;

    std::vector<Setting> settings;

    bool isHovered = false;

    int hovered = 0;
    int selected = -1;

    int x, y, width, height;
};

class SettingsView
{
public:
    SettingsView();

    void draw();

private:
    std::vector<Category> categories;

    int hovered = 0;
    int selected = -1;
};

extern SettingsView settingsView;