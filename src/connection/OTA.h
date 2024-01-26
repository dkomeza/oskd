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

        WiFi.waitForConnectResult(500);

        ArduinoOTA.begin();

        return WiFi.localIP();
    }

    void loop()
    {
        ArduinoOTA.handle();
    }
}