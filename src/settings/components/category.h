#pragma once

#include <Arduino.h>
#include <vector>

#include "setting.h"

struct Position
{
  int x = 0;
  int y = 0;
  int width = 0;
  int height = 0;
};

class Category
{
public:
  Category(String name);
  Category(String name, bool hovered);

  void setPosition(int x, int y, int width, int height);

  void addSetting(Setting setting)
  {
    this->settings.push_back(setting);
  };

  void drawHeader();
  void draw();

  void setHovered(bool hovered);

private:
  String name;
  bool hovered = false;
  Position position;

  std::vector<Setting> settings;
};
