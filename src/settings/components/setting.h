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
