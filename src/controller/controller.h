#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>

class Controller
{
public:
    void setup();
    void shutdown();

    void update();

    void setGear(int gear);
    void setWalkMode(bool walkMode);
    void setLegalMode(bool legalMode);

    int gear = 0; // 0 - 5 (0 - 2 in legal mode) W(6) as the walk mode

private:
    HardwareSerial *serial;

    bool legalMode = true;
    int maxSpeed = 25;
    int maxPower = 250;

    int _gear = 0; // save the current gear before walk mode is activated

    long lastUpdate = 0;

    void loadGear();

    void calculatePacket(byte *packet);
    void parsePacket(byte *packet);
};

extern Controller controller;