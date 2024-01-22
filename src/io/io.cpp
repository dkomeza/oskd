#include "io.h"
#include "data/data.h"

IO io;

void IO::setup()
{
  pinMode(BATTERY_VOLTAGE_PIN, INPUT);
  pinMode(BATTERY_VOLTAGE_OFFSET_PIN, INPUT);

  voltageOffset = getVoltageOffset();
}

void IO::update()
{
  long now = millis();

  if (now < lastUpdate)
    lastUpdate = now;

  if (now - lastUpdate < UPDATE_INTERVAL)
    return;

  lastUpdate = now;

  data::voltage = getVoltage();
}

int IO::getVoltageOffset()
{
  int reference = map(1800, 0, 3300, 0, 4095); // 1800mV is the reference voltage
  int sum = 0;

  for (int i = 0; i < 20; i++) // take 20 samples
  {
    sum += analogRead(BATTERY_VOLTAGE_OFFSET_PIN);
    delay(1);
  }

  return reference - (sum / 20);
}

int IO::getVoltage()
{
  int sum = 0;

  for (int i = 0; i < 10; i++) // take 20 samples
  {
    sum += analogRead(BATTERY_VOLTAGE_PIN);
    delay(1);
  }

  sum /= 10;

  sum += voltageOffset;

  int voltage = map(sum, 0, 4095, 0, 3300);

  int batteryVoltage = voltage * (1000 + 56) / 56; // 56kOhm and 1MOhm voltage divider

  return batteryVoltage / 100; // 10x battery voltage
}
