#include "dashboard.h"
#include "tft.h"
#include "data/data.h"

static const int BATTERY_MAX_VOLTAGE = 540; // 54.0V
static const int BATTERY_MIN_VOLTAGE = 420; // 42.0V

void drawBattery();
void updateBattery();

void dashboard::draw()
{
    // Draw the background
    tft.fillScreen(TFT_BLACK);

    // Crate the main box
    Color mainBoxColor = tft.color565(8, 8, 8);
    int mainBoxRadius = 24;
    tft.fillSmoothRoundRect(0, -mainBoxRadius, 240, 260 + mainBoxRadius, mainBoxRadius, mainBoxColor);

    // Draw the components
    drawBattery();
}

void drawBattery()
{
    int batteryWidth = 56;
    int batteryHeight = 20;

    int batteryX = 240 - batteryWidth - 12;
    int batteryY = 8;

    int batteryRadius = 4;

    Color mainBoxColor = tft.color565(8, 8, 8);

    tft.fillSmoothRoundRect(batteryX, batteryY, batteryWidth, batteryHeight, batteryRadius, TFT_WHITE);
    tft.fillSmoothRoundRect(batteryX + batteryWidth + 1, batteryY + batteryHeight / 2 - 4, 4, 8, 1, TFT_WHITE);
    tft.fillSmoothRoundRect(batteryX + 2, batteryY + 2, batteryWidth - 4, batteryHeight - 4, 2, mainBoxColor);

    updateBattery();
}

void updateBattery()
{
    int batteryWidth = 56;
    int batteryHeight = 20;

    int batteryX = 240 - batteryWidth - 12;
    int batteryY = 8;

    batteryWidth -= 4;
    batteryWidth -= 2;

    batteryHeight -= 4;
    batteryHeight -= 2;

    int batteryPercent = (double)(data::voltage - BATTERY_MIN_VOLTAGE) / (double)(BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE) * 100.0;

    if (batteryPercent < 0)
        batteryPercent = 0;
    else if (batteryPercent > 100)
        batteryPercent = 100;

    batteryWidth = batteryWidth * batteryPercent / 100;

    tft.fillSmoothRoundRect(batteryX + 3, batteryY + 3, batteryWidth, batteryHeight, 2, TFT_GREEN);
}