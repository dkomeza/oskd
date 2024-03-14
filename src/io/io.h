#pragma once

#include <Arduino.h>

class IO
{
public:
  void setup();
  void update();

private:
  static const int BATTERY_VOLTAGE_PIN = 35;
  static const int BATTERY_VOLTAGE_OFFSET_PIN = 34;

  int voltageOffset;

  static const int UPDATE_INTERVAL = 100; // ms
  long lastUpdate = 0;

  int getVoltageOffset();
  int getVoltage();
};

extern IO io;