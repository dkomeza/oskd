#include "settings.h"
#include "screen/tft.h"
#include <WiFi.h>
#include <QRCodeGen.h>
#include <EEPROM.h>

static const int minValues[SETTINGS_ARRAY_SIZE] = {10, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
static const int maxValues[SETTINGS_ARRAY_SIZE] = {72, 30, 255, 6, 1, 1, 40, 7, 1, 5, 100, 10, 7, 5, 3, 5, 15, 1};
static const int defaultValues[SETTINGS_ARRAY_SIZE] = {25, 20, 86, 1, 1, 0, 13, 5, 0, 0, 50, 10, 4, 0, 2, 5, 5, 1};

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

  settingsArray[index] = value;

  EEPROM.write(SETTINGS_ADDRESS + index, value);
  EEPROM.commit();
}

void Settings::update()
{
  delay(10);
}