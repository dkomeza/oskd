#include <Arduino.h>
#include <TFT_eSPI.h>

#include "connection/OTA.h"
#include "screen/screen.h"
#include "data/data.h"
#include "screen/dashboard.h"

bool rising = true;

void setup()
{
    Serial.begin(115200);

    connection::setupOTA();
    screen::setup();
}

void loop()
{
    data::voltage = rising ? data::voltage + 1 : data::voltage - 1;

    if (data::voltage == 546)
        rising = false;
    else if (data::voltage == 400)
        rising = true;

    connection::loop();

    dashboard::update();

    delay(50);
}