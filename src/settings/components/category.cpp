#include "category.h"
#include "screen/tft.h"

Category::Category(String name)
{
  this->name = name;
}

Category::Category(String name, bool hovered)
{
  this->name = name;
  this->hovered = hovered;
}

void Category::setPosition(int x, int y, int width, int height)
{
  this->position.x = x;
  this->position.y = y;
  this->position.width = width;
  this->position.height = height;
}

void Category::drawHeader()
{
  int width = position.width;
  int height = position.height;

  int x = position.x;
  int y = position.y;

  int colorBg = this->hovered ? TFT_LIGHTGREY : COLOR_BACKGROUND;
  int colorText = this->hovered ? TFT_BLACK : TFT_WHITE;

  tft.fillSmoothRoundRect(x, y, width, height, 8, colorBg);
  tft.setTextColor(colorText, colorBg);
  tft.setTextDatum(ML_DATUM);

  tft.loadFont(FONT_M);
  tft.drawString(this->name, x + 8, y + height / 2);

  tft.unloadFont();
}

void Category::setHovered(bool hovered)
{
  this->hovered = hovered;

  this->drawHeader();
}
