#include <Arduino.h>
#include <TFT_eSPI.h>

#include "connection/OTA.h"
#include "screen/screen.h"
#include "screen/views/dashboard.h"

#include "data/data.h"
#include "settings/settings.h"
#include "controller/controller.h"

#include "io/button.h"
#include "io/io.h"

const int BUTTON_DOWN_PIN = 14;
const int BUTTON_POWER_PIN = 27;
const int BUTTON_UP_PIN = 12;

const int EEPROM_SIZE = 32;

Button pButton = Button(BUTTON_POWER_PIN);
Button uButton = Button(BUTTON_UP_PIN);
Button dButton = Button(BUTTON_DOWN_PIN);

void startup();
void setupButtons();

void setup()
{
    pinMode(BUTTON_POWER_PIN, INPUT_PULLUP);
    esp_sleep_enable_ext0_wakeup((gpio_num_t)BUTTON_POWER_PIN, 0);

    Serial.begin(115200);
    EEPROM.begin(EEPROM_SIZE);

    startup();
    setupButtons();

    // screen::setup();
    IPAddress ip = connection::setup();
}

void loop()
{
    controller.update();
    screen::loop();
    connection::loop();
    io.update();

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

        delay(10);
    }

    // check for settings
    if (pButton.isPressed() && dButton.isPressed())
    {
        screen::setView(View::Settings);
        screen::setup();
        screen::loop();
        return;
    }

    // Setup the controller and IO
    controller.setup();
    io.setup();

    // Get initial data from the controller and update the IO
    controller.update();
    io.update();

    // fill the screen
    screen::setup();
    screen::loop();

    // Light up the screen
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
    controller.loadGear();
    screen::loop(true);
}

void setupButtons()
{
    uButton.onClick([]()
                    {
        switch (screen::view)
        {
            case View::Dashboard:
                controller.handleButtonUp();
                break;
        } });

    dButton.onClick([]()
                    {
        switch (screen::view)
        {
            case View::Dashboard:
                controller.handleButtonDown();
                break;
        } });

    dButton.onLongPress([]()
                        {
        switch (screen::view)
        {
            case View::Dashboard:
                controller.handleButtonDownLongPress();
                break;
        } });

    dButton.onLongPressRelease([]()
                               {
        switch (screen::view)
        {
            case View::Dashboard:
                controller.handleButtonDownLongPressStop();
                break;
        } });

    uButton.onLongPress([]()
                        {
        switch (screen::view)
        {
            case View::Dashboard:
                controller.handleButtonUpLongPress();
                break;
        } });

    pButton.onLongPress([]()
                        {
        screen::shutdown();
        controller.shutdown(); 
        
        delay(200);
        esp_deep_sleep_start(); });
}