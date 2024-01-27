#include "setting.h"
#include "screen/tft.h"

Setting::Setting(String name, String description, int *value, int min, int max)
{
  this->name = name;
  this->description = description;
  this->value = value;
  this->min = min;
  this->max = max;
}