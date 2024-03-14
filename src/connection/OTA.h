#pragma once

#include <Arduino.h>
#include <ArduinoOTA.h>

#include "credentials.h"

namespace connection
{
    long lastUpdate = 0;
    int updateInterval = 200;

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
        long now = millis();

        if (now < lastUpdate)
            lastUpdate = now;

        if (now - lastUpdate < updateInterval)
            return;

        ArduinoOTA.handle();
        lastUpdate = now;
    }
}