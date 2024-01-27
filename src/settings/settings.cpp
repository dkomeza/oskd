#include "settings.h"
#include "screen/tft.h"

Settings settings;

Settings::Settings()
{
  Category general("General", true);
  this->categories.push_back(general);

  Category motor("Motor");
  this->categories.push_back(motor);

  Category pas("PAS");
  this->categories.push_back(pas);

  Category battery("Battery");
  this->categories.push_back(battery);

  Category throttle("Throttle");
  this->categories.push_back(throttle);

  Category display("Display");
  Setting brightness("Brightness", "Brightness of the display", &this->brightness, 0, 5);
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
    Category *category = &this->categories[i];

    int x = paddingX;
    int y = offsetY + i * (drawerHeight + paddingY);

    category->setPosition(x, y, drawerWidth, drawerHeight);
    category->drawHeader();
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

void Settings::handleUpButton()
{
  if (selectedCategory != -1)
  {
    Category category = this->categories[selectedCategory];
    // category.handleUpButton();
  }
  else
  {
    if (hoveredCategory > 0)
    {
      this->categories[hoveredCategory].setHovered(false);
      this->hoveredCategory--;
      this->categories[hoveredCategory].setHovered(true);
    }
  }
}

void Settings::handleDownButton()
{
  if (selectedCategory != -1)
  {
    Category category = this->categories[selectedCategory];
    // category.handleDownButton();
  }
  else
  {
    if (hoveredCategory < this->categories.size() - 1)
    {
      this->categories[hoveredCategory].setHovered(false);
      this->hoveredCategory++;
      this->categories[hoveredCategory].setHovered(true);
    }
  }
}

void Settings::handlePowerButton()
{
  if (selectedCategory != -1)
  {
    Category category = this->categories[selectedCategory];
    // category.handlePowerButton();
  }
  else
  {
    this->selectedCategory = this->hoveredCategory;
  }
}
