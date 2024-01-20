#include <Arduino.h>
#include <TFT_eSPI.h>

#include "connection/OTA.h"
#include "screen/screen.h"
#include "data/data.h"
#include "screen/dashboard.h"

#include "io/button.h"

Button button(27);
Button button2(12);

bool rising = true;

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting...");

    // connection::setupOTA();
    screen::setup();

    button.setLongPressTime(600);

    button.onClick([]()
                   { data::voltage--; });

    button.onLongPress([]()
                       { data::voltage -= 1; },
                       1);

    button2.onClick([]()
                    { data::voltage++; });

    button2.onLongPress([]()
                        { data::voltage += 1; },
                        1);

    Serial.println("Started!");
}

void loop()
{
    connection::loop();

    button.update();
    button2.update();

    dashboard::update();
}