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

    connection::setupOTA();
    screen::setup();

    button.setLongPressTime(600);

    button.onClick([]()
                   { Serial.println("Click"); });
    button.onLongPress([]()
                       { Serial.println("Long press"); },
                       100);
    button.onLongPressRelease([]()
                              { Serial.println("Long press release"); });
}

void loop()
{
    connection::loop();

    button.update();

    dashboard::update();
}