#include <Arduino.h>
#include <EEPROM.h>
#include <TFT_eSPI.h>

#include "connection/OTA.h"

#include "screen/screen.h"
#include "screen/views/dashboard.h"

#include "data/data.h"
#include "settings/settings.h"
#include "controller/controller.h"
#include "settings/settings.h"

#include "io/button.h"
#include "io/io.h"

const int BUTTON_DOWN_PIN = 14;
const int BUTTON_POWER_PIN = 27;
const int BUTTON_UP_PIN = 12;

const int EEPROM_SIZE = 64;

Button pButton = Button(BUTTON_POWER_PIN);
Button uButton = Button(BUTTON_UP_PIN);
Button dButton = Button(BUTTON_DOWN_PIN);

enum class BUTTON
{
    Power,
    Up,
    Down
};

void startup();
void setupButtons();

void setup()
{
    pinMode(BUTTON_POWER_PIN, INPUT_PULLUP);
    esp_sleep_enable_ext0_wakeup((gpio_num_t)BUTTON_POWER_PIN, 0);

    EEPROM.begin(EEPROM_SIZE);

    controller.setup();
    io.setup();
    connection::setup();
    screen::setup();

    startup();
    setupButtons();
}

void loop()
{
    controller.update(); // Controller
    connection::loop();  // OTA

    uButton.update();
    dButton.update();
    pButton.update();
}

void startup()
{
    bool legalMode = false;
    long start = millis();

    // startup routine
    while (start + 1000 > millis())
    {
        if (!pButton.isPressed())
            esp_deep_sleep_start();

        loop();
        delay(10);
    }

    // check for settings
    if (pButton.isPressed() && dButton.isPressed())
    {
        return;
    }

    screen::lightUp();

    start = millis();

    // legal mode routine
    while (start + 3000 > millis())
    {
        if (!pButton.isPressed() || !uButton.isPressed())
        {
            legalMode = true;
            break;
        }

        delay(10);
    }

    controller.setLegalMode(legalMode);
}

void shutdown()
{
    controller.shutdown();

    delay(100);
    esp_deep_sleep_start();
}

void setupButtons()
{
    uButton.onClick([]()
                    { onClick(BUTTON::Up); });
    dButton.onClick([]()
                    { onClick(BUTTON::Down); });
    pButton.onClick([]()
                    { onClick(BUTTON::Power); });

    uButton.onLongPress([]()
                        { onLongPress(BUTTON::Up); });
    dButton.onLongPress([]()
                        { onLongPress(BUTTON::Down); });
    pButton.onLongPress([]()
                        { onLongPress(BUTTON::Power); });

    dButton.onLongPressRelease([]()
                               { onLongPressRelease(BUTTON::Down); });
}

void onClick(BUTTON button)
{
    switch (button)
    {
    case BUTTON::Power:
        break;
    case BUTTON::Up:
        controller.setGear(controller.gear + 1);
        break;
    case BUTTON::Down:
        controller.setGear(controller.gear - 1);
        break;
    }
}

void onLongPress(BUTTON button)
{
    switch (button)
    {
    case BUTTON::Power:
        shutdown();
        break;
    case BUTTON::Up:
        controller.setLegalMode(true);
        break;
    case BUTTON::Down:
        controller.setWalkMode(true);
        break;
    }
}

void onLongPressRelease(BUTTON button)
{
    switch (button)
    {
    case BUTTON::Power:
        break;
    case BUTTON::Up:
        break;
    case BUTTON::Down:
        controller.setWalkMode(false);
        break;
    }
}