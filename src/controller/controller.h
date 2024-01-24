#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>

class Controller
{
public:
    Controller(){};

    void setup();
    void update();

    void setLegalMode(bool legalMode);

    int getMaxSpeed();
    int getMaxPower();

private:
    HardwareSerial *serial;

    static const int CONTROLLER_BAUD_RATE = 9600;
    static const int CONTROLLER_RX_PIN = 16;
    static const int CONTROLLER_TX_PIN = 17;

    static const int PACKET_UP_SIZE = 13;
    static const int PACKET_DOWN_SIZE = 12;

    const double RPM_CONSTANT = 0.1885;

    static const int UPDATE_INTERVAL = 100; // ms
    long lastUpdate = 0;

    bool legalMode = true;
    int maxSpeed = 25;
    int maxPower = 250;
    int _maxPower = 1500; // read from the settings

    void sendPacket();
    void receivePacket();

    void parsePacket(byte *packet);

    bool isPacketValid(byte *packet);
};

extern Controller controller;
