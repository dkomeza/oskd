#pragma once

#include <Arduino.h>

struct DashboardBounds
{
    int power;
    int speed;
};

class Dashboard
{
public:
    void setup();
    void update();

    void setBounds(DashboardBounds bounds);

private:
    DashboardBounds bounds = {.power = 250, .speed = 25};

    DashboardBounds lastBounds;

    long lastUpdate = 0;

    void drawBattery();
    void drawMain();
    void drawSpeedArc();
    void drawPowerArc();

    void updateBattery();
    void updateSpeed();
    void updatePower();
    void updateGear();
};

extern Dashboard dashboard;