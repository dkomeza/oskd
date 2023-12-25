#include <Arduino.h>
#include <TFT_eSPI.h>

#include "connection/OTA.h"

void setup()
{
    connection::setupOTA();

    TFT_eSPI tft = TFT_eSPI();
    tft.init();
    tft.setRotation(4);
    tft.fillScreen(TFT_YELLOW);

    pinMode(26, OUTPUT);
    digitalWrite(26, HIGH);
}

void loop()
{
    connection::loop();
}