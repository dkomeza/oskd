#include "settings.h"
#include "screen/tft.h"
#include "screen/screen.h"
#include <WiFi.h>
#include <QRCodeGen.h>
#include <EEPROM.h>

Settings settings;

Settings::Settings()
{
}

void Settings::setup()
{
  loadSettings();
}

void Settings::loadSettings()
{
  for (int i = 0; i < SETTINGS_ARRAY_SIZE; i++)
  {
    settingsArray[i] = EEPROM.read(SETTINGS_ADDRESS + i);
  }

  if (!validateSettings())
  {
    loadDefaults();
    saveSettings();
  }
}

bool Settings::validateSettings()
{
  for (int i = 0; i < SETTINGS_ARRAY_SIZE; i++)
  {
    if (settingsArray[i] < minValues[i] || settingsArray[i] > maxValues[i])
    {
      return false;
    }
  }

  return true;
}

void Settings::loadDefaults()
{
  for (int i = 0; i < SETTINGS_ARRAY_SIZE; i++)
  {
    settingsArray[i] = defaultValues[i];
  }
}

void Settings::saveSettings()
{
  for (int i = 0; i < SETTINGS_ARRAY_SIZE; i++)
  {
    EEPROM.write(SETTINGS_ADDRESS + i, settingsArray[i]);
  }

  EEPROM.commit();
}

void Settings::setSetting(int index, int value)
{
  if (value < minValues[index] || value > maxValues[index])
  {
    return;
  }

  int oldValue = settingsArray[index];

  settingsArray[index] = value;

  EEPROM.write(SETTINGS_ADDRESS + index, value);
  EEPROM.commit();

  if (index == DISPLAY_BRIGHTNESS_ADDRESS)
  {
    screen::changeBrightness(oldValue, value);
  }
}

void Settings::setWheelSize(int direction)
{
  int current = -1;

  for (int i = 0; i < WHEEL_SIZE_COUNT; i++)
  {
    if (settingsArray[WHEEL_SIZE_ADDRESS] == wheelSizes[i].value)
    {
      current = i;
      break;
    }
  }

  if (current == -1)
  {
    settingsArray[WHEEL_SIZE_ADDRESS] = wheelSizes[0].value;
    EEPROM.write(SETTINGS_ADDRESS + WHEEL_SIZE_ADDRESS, wheelSizes[0].value);
    EEPROM.commit();
    return;
  }

  current += direction;

  if (current < 0)
  {
    current = 0;
  }
  else if (current >= WHEEL_SIZE_COUNT)
  {
    current = WHEEL_SIZE_COUNT - 1;
  }

  int value = wheelSizes[current].value;

  settingsArray[WHEEL_SIZE_ADDRESS] = value;
  EEPROM.write(SETTINGS_ADDRESS + WHEEL_SIZE_ADDRESS, value);
  EEPROM.commit();
}

void Settings::update()
{
  delay(10);
}