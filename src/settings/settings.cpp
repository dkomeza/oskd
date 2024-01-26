#include "settings.h"
#include "screen/tft.h"

Settings settings;

Settings::Settings()
{
  this->createGeneralCategory();
  this->createMotorCategory();
  this->createPasCategory();
  this->createBatteryCategory();
  this->createThrottleCategory();
  this->createDisplayCategory();
}

void Settings::createGeneralCategory()
{
  Category general("General");

  this->categories.push_back(general);
}
void Settings::createMotorCategory()
{
  Category motor("Motor");

  this->categories.push_back(motor);
}
void Settings::createPasCategory()
{
  Category pas("PAS");

  this->categories.push_back(pas);
}
void Settings::createBatteryCategory()
{
  Category battery("Battery");

  this->categories.push_back(battery);
}
void Settings::createThrottleCategory()
{
  Category throttle("Throttle");

  this->categories.push_back(throttle);
}
void Settings::createDisplayCategory()
{
  Category display("Display");

  // Setting brightness("Brightness", "Adjust the brightness of the display", &this->brightness, 0, 255);

  // display.addSetting(brightness);
  this->categories.push_back(display);
}

void Settings::draw()
{
  tft.fillScreen(TFT_BLACK);

  int offsetY = 8;
  int paddingX = 8;
  int paddingY = 8;

  int drawerWidth = TFT_WIDTH - paddingX * 2;
  int drawerHeight = 42;

  for (int i = 0; i < this->categories.size(); i++)
  {
    Category category = this->categories[i];

    spr.createSprite(drawerWidth, drawerHeight);
    spr.fillSprite(TFT_BLACK);

    spr.fillSmoothRoundRect(0, 0, drawerWidth, drawerHeight, 8, COLOR_BACKGROUND);

    spr.loadFont(FONT_M);
    spr.setTextColor(TFT_WHITE);
    spr.setTextDatum(ML_DATUM);

    spr.drawString(category.name, 8, 8);

    spr.unloadFont();

    int y = offsetY + i * (drawerHeight + paddingY);
    spr.pushSprite(paddingX, y);
    spr.deleteSprite();
  }
}

void Settings::setup()
{
  delay(10);
}

void Settings::update()
{
  delay(10);
}
