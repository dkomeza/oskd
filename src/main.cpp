#include <Arduino.h>
#include <TFT_eSPI.h>

#include "connection/OTA.h"
#include "screen/screen.h"

void setup()
{
    connection::setupOTA();
    screen::setup();
}

void loop()
{
    connection::loop();
}