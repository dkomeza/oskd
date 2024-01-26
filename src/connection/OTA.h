#pragma once

#include <Arduino.h>
#include <ArduinoOTA.h>

#include "credentials.h"

namespace connection
{
    IPAddress setup()
    {
        ArduinoOTA.setHostname("OSKD");

        WiFi.mode(WIFI_STA);
        WiFi.begin(credentials::ssid, credentials::password);

        long start = millis();

        while (millis() - start < 1000 && WiFi.waitForConnectResult() != WL_CONNECTED)
        {
            delay(100);
        }

        ArduinoOTA.begin();

        return WiFi.localIP();
    }

    void loop()
    {
        ArduinoOTA.handle();
    }
}