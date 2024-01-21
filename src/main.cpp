#include <Arduino.h>
#include <TFT_eSPI.h>

#include "connection/OTA.h"
#include "screen/screen.h"
#include "data/data.h"
#include "screen/dashboard.h"
#include "settings/settings.h"

#include "io/button.h"

const int BUTTON_DOWN_PIN = 14;
const int BUTTON_POWER_PIN = 27;
const int BUTTON_UP_PIN = 12;

Button pButton = Button(BUTTON_POWER_PIN);
Button uButton = Button(BUTTON_UP_PIN);
Button dButton = Button(BUTTON_DOWN_PIN);

void startup();

void setup()
{
    pinMode(BUTTON_POWER_PIN, INPUT_PULLUP);
    esp_sleep_enable_ext0_wakeup((gpio_num_t)BUTTON_POWER_PIN, 0);

    Serial.begin(115200);

    startup();
}

void loop()
{
}

void startup()
{
    bool legalMode = false;
    long start = millis();

    while (start + 1000 > millis())
    {
        if (!pButton.isPressed())
            esp_deep_sleep_start();

        delay(10);
    }

    if (pButton.isPressed() && dButton.isPressed())
    {
        Serial.println("Settings");
    }

    screen::setup();
    screen::loop();

    start = millis();

    while (start + 3000 > millis())
    {
        if (!pButton.isPressed() || !uButton.isPressed())
        {
            legalMode = true;
            break;
        }

        delay(10);
    }

    settings::legal = legalMode;

    if (legalMode)
        return;

    screen::draw();
    screen::loop(true);
}