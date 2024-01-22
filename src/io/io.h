#pragma once

#include <Arduino.h>

class IO
{
public:
  IO(){};

  void setup();
  void update();

private:
  static const int BATTERY_VOLTAGE_PIN = 35;
  static const int BATTERY_VOLTAGE_OFFSET_PIN = 34;

  int voltageOffset;

  int getVoltageOffset();
  int getVoltage();
};

extern IO io;