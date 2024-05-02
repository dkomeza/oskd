#include <Arduino.h>
#include <vector>
#include "settings/settings.h"

enum class BUTTON
{
    Power,
    Up,
    Down
};

class SettingsView;
class Category;
class Setting;

class Setting
{
public:
    Setting(String name, int addr)
    {
        this->name = name;
        this->addr = addr;
        this->value = &settings.settingsArray[addr];
        this->min = minValues[addr];
        this->max = maxValues[addr];
        this->fullName = name;
    };
    Setting(String name, int addr, bool isBool)
    {
        this->name = name;
        this->addr = addr;
        this->value = &settings.settingsArray[addr];
        this->min = minValues[addr];
        this->max = maxValues[addr];
        this->isBool = isBool;
        this->fullName = name;
    };

    void setDesc(String desc) { this->desc = desc; }
    void setFullName(String fullName) { this->fullName = fullName; }

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

    void setScroll(int offset)
    {
        this->offset = offset;
    }

    void setHovered(bool isHovered);

    void setParent(Category *parent) { this->parent = parent; }

    void draw();
    void drawHeader();

    void handleClick(BUTTON button);

    int x, y, width, height;
    int offset = 0;

private:
    String name;
    String fullName;

    int addr;

    int *value;
    int min;
    int max;

    String desc;

    bool isHovered = false;
    Category *parent;

    bool isBool = false;
    bool selected = false;
    bool backHovered = false;

    void drawBackButton();
    void drawSetting();
};

class Category
{
public:
    Category(String name, SettingsView *parent) : name(name), isHovered(false), parent(parent)
    {
        this->offset = 0;
    }

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

    void setScroll(int offset)
    {
        this->offset = offset;
    }

    void addSetting(Setting *setting)
    {
        setting->setParent(this);
        settings.push_back(setting);
    }
    void draw();
    void drawHeader();
    void setHovered(bool isHovered);

    void handleClick(BUTTON button);
    void deselectSetting();

    int x, y, width, height, offset;
    String name;

private:
    std::vector<Setting *> settings;

    bool isHovered = false;

    int hovered = 0;
    int selected = -1;

    int selfOffset = 0;
    int offsetDiff = 0;

    SettingsView *parent;

    void drawBackButton();
    void handleScroll();
    void drawScrollBar();
};

class SettingsView
{
public:
    SettingsView();

    void draw();
    void handleClick(BUTTON button);

    void deselectCategory();

private:
    std::vector<Category *> categories;

    int offset = 0;
    int offsetDiff = 0;

    int hovered = 0;
    int selected = -1;

    void handleScroll();
    void drawScrollBar();
};

extern SettingsView settingsView;