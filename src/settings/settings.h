#pragma once

#include <Arduino.h>
#include <vector>

static const int SETTINGS_ARRAY_SIZE = 18;

#define SPEED_LIMIT_ADDRESS 0
#define WHEEL_SIZE_ADDRESS 1
#define MOTOR_POLE_PAIRS_ADDRESS 2
#define WHEEL_SPEED_PULSES_ADDRESS 3
#define POWER_ASSIST_CONTROL_MODE_ADDRESS 4
#define THROTTLE_STARTUP_ADDRESS 5
#define POWER_MONITORING_ADDRESS 6
#define PAS_PARAMETER_SELECT_ADDRESS 7
#define MOTOR_PHASE_CLASSIFICATION_ADDRESS 8
#define THROTTLE_FUNCTION_ADDRESS 9
#define THROTTLE_SPEED_LIMIT_ADDRESS 10
#define CONTROLLER_MAXIMUM_CURRENT_ADDRESS 11
#define CONTROLLER_MINIMUM_VOLTAGE_ADDRESS 12
#define CONTROLLER_REGEN_BRAKE_ADDRESS 13
#define POWER_ASSIST_TUNING_ADDRESS 14
#define DISPLAY_BRIGHTNESS_ADDRESS 15
#define DISPLAY_AUTO_SLEEP_ADDRESS 16
#define DISPLAY_TEMPERATURE_DISPLAY_ADDRESS 17

class Settings
{
public:
    Settings();

    void setup();
    void update();

    void setSetting(int index, int value);

    int settingsArray[SETTINGS_ARRAY_SIZE];
    int *speedLimit = &settingsArray[SPEED_LIMIT_ADDRESS];

    int *brightness = &settingsArray[DISPLAY_BRIGHTNESS_ADDRESS];
    bool *displayTemperature = (bool *)&settingsArray[DISPLAY_TEMPERATURE_DISPLAY_ADDRESS];

private:
    const int SETTINGS_ADDRESS = 8;

    void loadSettings();
    bool validateSettings();
    void loadDefaults();
    void saveSettings();
};

extern Settings settings;