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
    
}

void loop()
{
    
}