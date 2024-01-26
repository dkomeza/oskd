#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>
#include <EEPROM.h>

class Controller
{
public:
    Controller(){};

    void setup();
    void shutdown();

    void update();

    void setLegalMode(bool legalMode);

    int getMaxSpeed();
    int getMaxPower();

    void handleButtonUp();
    void handleButtonUpLongPress();
    void handleButtonDown();
    void handleButtonDownLongPress();
    void handleButtonDownLongPressStop();

    int gear = 0; // 0 - 5 (0 - 2 in legal mode) W(6) as the walk mode

private:
    HardwareSerial *serial;

    static const int CONTROLLER_BAUD_RATE = 9600;
    static const int CONTROLLER_RX_PIN = 16;
    static const int CONTROLLER_TX_PIN = 17;

    static const int PACKET_UP_SIZE = 13;
    static const int PACKET_DOWN_SIZE = 12;

    static const int POWER_PIN = 19;

    static const int GEAR_ADDRESS = 0x01; // EEPROM address

    const double RPM_CONSTANT = 0.1885;

    static const int UPDATE_INTERVAL = 100; // ms
    long lastUpdate = 0;

    bool legalMode = true;
    int maxSpeed = 25;
    int maxPower = 250;
    int _maxPower = 1500; // read from the settings

    int _gear = 0; // save the current gear before walk mode is activated

    void sendPacket();
    void receivePacket();

    void parsePacket(byte *packet);

    bool isPacketValid(byte *packet);
};

extern Controller controller;
