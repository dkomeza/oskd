#include <Arduino.h>
#include <TFT_eSPI.h>

#include "connection/OTA.h"
#include "screen/screen.h"

void setup()
{
    Serial.begin(115200);

    connection::setupOTA();
    screen::setup();
}

void loop()
{
    connection::loop();
}