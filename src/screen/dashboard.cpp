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
void drawMain();
void drawPowerArc();
void updatePower();

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
    drawMain();
}

void dashboard::update()
{
    if (data::voltage != voltage)
    {
        voltage = data::voltage;
        updateVoltage();
        updateBattery();
    }

    if (data::power != power)
    {
        power = data::power;
        updatePower();
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

void drawMain()
{
    // Draw the speed background
    int mainBackgroundRadius = 110;
    int mainBackgroundX = 120;
    int mainBackgroundY = 140;

    tft.fillSmoothCircle(mainBackgroundX, mainBackgroundY, mainBackgroundRadius, TFT_BLACK, COLOR_BACKGROUND);

    // Draw the power arc
    drawPowerArc();

    // Draw the speed arc
    int speedArcRadius = 96;
    int speedArcAngle = 210;

    int speedArcStartAngle = 180 - speedArcAngle / 2;
    int speedArcEndAngle = 180 + speedArcAngle / 2;

    // tft.drawSmoothArc(speedBackgroundX, speedBackgroundY, speedArcRadius, speedArcRadius - 1, speedArcStartAngle, speedArcEndAngle, TFT_WHITE, TFT_BLACK);

    updatePower();
}

void drawPowerArc()
{
    int maxPower = 1000;

    int mainBackgroundX = 120;
    int mainBackgroundY = 140;

    // Draw the power arc
    int powerArcRadius = 100;
    int powerArcAngle = 90;

    int powerArcStartAngle = 360 - powerArcAngle / 2;
    int powerArcEndAngle = powerArcAngle / 2;

    tft.drawSmoothArc(mainBackgroundX, mainBackgroundY, powerArcRadius, powerArcRadius - 1, powerArcStartAngle, powerArcEndAngle, TFT_WHITE, TFT_BLACK);

    int powerBars = 6;
    int powerBarHeight = 6;
    for (int i = 0; i < powerBars; i++)
    {
        int powerBarAngle = powerArcAngle / (powerBars - 1) * i + powerArcAngle / 2;

        int powerBarX = mainBackgroundX + cos(powerBarAngle * PI / 180.0) * powerArcRadius;
        int powerBarY = mainBackgroundY + sin(powerBarAngle * PI / 180.0) * powerArcRadius;

        int powerBarX2 = mainBackgroundX + cos(powerBarAngle * PI / 180.0) * (powerArcRadius - powerBarHeight);
        int powerBarY2 = mainBackgroundY + sin(powerBarAngle * PI / 180.0) * (powerArcRadius - powerBarHeight);

        tft.drawLine(powerBarX, powerBarY, powerBarX2, powerBarY2, TFT_WHITE);
    }

    int smallPowerBarHeight = 3;
    for (int i = 0; i < powerBars - 1; i++)
    {
        int powerBarAngle = powerArcAngle / (powerBars - 1) * i + powerArcAngle / 2 + powerArcAngle / (powerBars - 1) / 2;

        int powerBarX = mainBackgroundX + cos(powerBarAngle * PI / 180.0) * powerArcRadius;
        int powerBarY = mainBackgroundY + sin(powerBarAngle * PI / 180.0) * powerArcRadius;

        int powerBarX2 = mainBackgroundX + cos(powerBarAngle * PI / 180.0) * (powerArcRadius - smallPowerBarHeight);
        int powerBarY2 = mainBackgroundY + sin(powerBarAngle * PI / 180.0) * (powerArcRadius - smallPowerBarHeight);

        tft.drawLine(powerBarX, powerBarY, powerBarX2, powerBarY2, TFT_WHITE);
    }

    double divider = maxPower < 1000 ? 100.0 : 100.0;
    tft.setTextFont(2);
    for (int i = 0; i < powerBars; i++)
    {
        int powerBarAngle = powerArcAngle / (powerBars - 1) * i + powerArcAngle / 2;

        int powerBarX = mainBackgroundX + cos(powerBarAngle * PI / 180.0) * (powerArcRadius - 12);
        int powerBarY = mainBackgroundY + sin(powerBarAngle * PI / 180.0) * (powerArcRadius - 12);

        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.setTextDatum(CC_DATUM);

        double power = (double)maxPower / (double)(powerBars - 1) * (powerBars - 1 - i) / divider;
        int powerInt = (int)power;

        if (power - (double)powerInt == 0)
        {
            tft.drawString(String(powerInt), powerBarX, powerBarY, 1);
        }
        else
        {
            tft.drawString(String(power, 1), powerBarX, powerBarY, 1);
        }
    }
}

void updatePower()
{
    int powerArcRadius = 106;
    int powerArcInnerRadius = 102;

    int powerArcMaxAngle = 90;

    int maxPower = 1000;

    int pow = data::power > maxPower ? maxPower : data::power;

    if (pow < 0)
        pow = 0;

    int endAngle = 45;
    int absEndAngle = 360 - endAngle;
    int startAngle = endAngle - (double)pow / (double)maxPower * powerArcMaxAngle;

    if (startAngle < 0)
        startAngle = 360 + startAngle;

    if (startAngle != endAngle)
        tft.drawSmoothArc(120, 140, powerArcRadius, powerArcInnerRadius, startAngle, endAngle, TFT_GREEN, TFT_BLACK);

    if (pow == maxPower)
        return;
    tft.drawSmoothArc(120, 140, powerArcRadius, powerArcInnerRadius, absEndAngle, startAngle, TFT_BLACK, TFT_BLACK);
}
