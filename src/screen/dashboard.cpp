#include "dashboard.h"
#include "tft.h"
#include "data/data.h"

static const int BATTERY_MAX_VOLTAGE = 540; // 54.0V
static const int BATTERY_MIN_VOLTAGE = 420; // 42.0V

int speed = 0;
int gear = 0;
int power = 0;
int voltage = 0;

void drawBattery();
void updateBattery();
void updateVoltage();

void dashboard::draw()
{
    // Draw the background
    tft.fillScreen(TFT_BLACK);

    // Crate the main box
    int mainBoxRadius = 24;
    tft.fillSmoothRoundRect(0, -mainBoxRadius, 240, 260 + mainBoxRadius, mainBoxRadius, COLOR_BACKGROUND);

    // Draw the components
    drawBattery();
    updateVoltage();
}

void dashboard::update()
{
    if (data::voltage != voltage)
    {
        voltage = data::voltage;
        updateVoltage();
        updateBattery();
    }
}

void drawBattery()
{
    int batteryWidth = 56;
    int batteryHeight = 20;

    int batteryX = 240 - batteryWidth - 12;
    int batteryY = 8;

    int batteryRadius = 4;

    tft.fillSmoothRoundRect(batteryX, batteryY, batteryWidth, batteryHeight, batteryRadius, TFT_WHITE);
    tft.fillSmoothRoundRect(batteryX + batteryWidth + 1, batteryY + batteryHeight / 2 - 4, 4, 8, 1, TFT_WHITE);
    tft.fillSmoothRoundRect(batteryX + 2, batteryY + 2, batteryWidth - 4, batteryHeight - 4, 2, COLOR_BACKGROUND);

    updateBattery();
}

void updateBattery()
{
    int batteryWidth = 56;
    int batteryHeight = 20;

    int batteryX = 240 - batteryWidth - 12;
    int batteryY = 8;

    int innerWidth = batteryWidth - 6;
    int innerHeight = batteryHeight - 6;

    int batteryPercent = (double)(data::voltage - BATTERY_MIN_VOLTAGE) / (double)(BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE) * 100.0;

    if (batteryPercent < 0)
        batteryPercent = 0;
    else if (batteryPercent > 100)
        batteryPercent = 100;

    innerWidth = innerWidth * batteryPercent / 100;

    // background
    int backgroundX = batteryX + 3 + innerWidth - 2;
    int backgroundY = batteryY + 3;
    int backgroundWidth = batteryWidth - 4 - innerWidth + 2;
    int backgroundHeight = innerHeight;

    tft.fillRect(backgroundX, backgroundY, backgroundWidth, backgroundHeight, COLOR_BACKGROUND);

    // tft.fillSmoothRoundRect(batteryX + 2, batteryY + 2, batteryWidth - 4, batteryHeight - 4, 2, mainBoxColor);
    tft.fillSmoothRoundRect(batteryX + 3, batteryY + 3, innerWidth, innerHeight, 2, TFT_GREEN);
}

void updateVoltage()
{
    int voltageX = 8;
    int voltageY = 8;

    tft.loadFont(FONT_LARGE);

    tft.setTextColor(TFT_WHITE, COLOR_BACKGROUND, true);
    tft.setTextDatum(TL_DATUM);

    tft.drawString(String(data::voltage / 10.0, 1) + "V", voltageX, voltageY, 1);

    tft.unloadFont();
}
