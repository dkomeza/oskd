#include "dashboard.h"
#include "tft.h"
#include "data/data.h"
#include "settings/settings.h"

static const int BATTERY_MAX_VOLTAGE = 520; // 52.0V
static const int BATTERY_MIN_VOLTAGE = 420; // 42.0V

int speed = -1;
int gear = -1;
int power = -1;
int voltage = -1;

void drawBattery();
void updateBattery();
void updateVoltage();
void drawMain();
void drawPowerArc();
void updatePower();
void drawSpeedArc();
void updateSpeed();
void updateGear();

void dashboard::draw()
{
    // Draw the background
    tft.fillScreen(TFT_BLACK);

    // Crate the main box
    int mainBoxRadius = 24;
    tft.fillSmoothRoundRect(0, -mainBoxRadius, 240, 260 + mainBoxRadius, mainBoxRadius, COLOR_BACKGROUND);

    // Draw the components
    drawBattery();
    drawMain();
}

void dashboard::update(bool force)
{
    if (data::voltage != voltage || force)
    {
        voltage = data::voltage;
        updateVoltage();
        updateBattery();
    }

    if (data::speed != speed || force)
    {
        speed = data::speed;
        updateSpeed();
    }

    if (data::power != power || force)
    {
        power = data::power;
        updatePower();
    }

    if (data::gear != gear || force)
    {
        gear = data::gear;
        updateGear();
    }
}

void drawBattery()
{
    int batteryWidth = 56;
    int batteryHeight = 20;
    int padding = 2;

    int batteryX = 240 - batteryWidth - 12;
    int batteryY = 8;

    int batteryRadius = 4;
    int batteryTabRadius = 2;
    int batteryTabWidth = 4 + batteryTabRadius;
    int batteryTabHeight = 8;
    int batteryTabX = batteryX + batteryWidth - batteryTabRadius;
    int batteryTabY = batteryY + batteryHeight / 2 - batteryTabHeight / 2;

    tft.fillSmoothRoundRect(batteryX, batteryY, batteryWidth, batteryHeight, batteryRadius, TFT_WHITE);
    tft.fillRoundRect(batteryTabX, batteryTabY, batteryTabWidth, batteryTabHeight, batteryTabRadius, TFT_WHITE);
}
void updateBattery()
{
    int padding = 1;

    int innerWidth = 52;
    int innerHeight = 16;

    int batteryX = 240 - 56 - 12 + 2;
    int batteryY = 8 + 2;

    spr.createSprite(innerWidth, innerHeight);
    spr.fillSprite(COLOR_BACKGROUND);

    int batteryPercent = (double)(data::voltage - BATTERY_MIN_VOLTAGE) / (double)(BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE) * 100.0;

    if (batteryPercent < 0)
        batteryPercent = 0;
    else if (batteryPercent > 100)
        batteryPercent = 100;

    innerWidth -= padding * 2;
    innerHeight -= padding * 2;

    innerWidth = innerWidth * batteryPercent / 100;

    spr.fillSmoothRoundRect(padding, padding, innerWidth, innerHeight, padding, TFT_GREEN, COLOR_BACKGROUND);

    spr.pushSprite(batteryX, batteryY);
    spr.deleteSprite();
}
void updateVoltage()
{
    int voltageX = 8;
    int voltageY = 8;

    String voltageString = String(data::voltage / 10.0, 1) + "V";

    spr.createSprite(70, 20);

    spr.fillSprite(COLOR_BACKGROUND);
    spr.loadFont(FONT_M);
    spr.setTextColor(TFT_WHITE, COLOR_BACKGROUND);
    spr.setTextDatum(TL_DATUM);
    spr.drawString(voltageString, 0, 0);

    spr.unloadFont();

    spr.pushSprite(voltageX, voltageY);
    spr.deleteSprite();
}

void drawMain()
{
    // Draw the speed background
    int mainBackgroundRadius = 110;
    int mainBackgroundX = 120;
    int mainBackgroundY = 140;

    tft.fillSmoothCircle(mainBackgroundX, mainBackgroundY, mainBackgroundRadius, TFT_BLACK, COLOR_BACKGROUND);

    int wedgeX = 60;
    int wedgeX2 = wedgeX + 30;
    int wedgeY = 148;

    uint16_t color = tft.color565(192, 0, 0);

    tft.drawWedgeLine(60, wedgeY, 90, wedgeY, 0.1, 1.5, color, TFT_BLACK);
    tft.drawWedgeLine(240 - wedgeX, wedgeY, 240 - wedgeX2, wedgeY, 0.1, 1.5, color, TFT_BLACK);
    tft.drawWideLine(wedgeX2, wedgeY, 240 - wedgeX2, wedgeY, 3, color, TFT_RED);

    // Draw the speed arc
    drawSpeedArc();

    // Draw the power arc
    drawPowerArc();
}
void drawSpeedArc()
{
    int speedArcRadius = 100;
    int speedArcAngle = 220;

    int speedArcStartAngle = 180 - speedArcAngle / 2;
    int speedArcEndAngle = 180 + speedArcAngle / 2;

    // Either 25 or 50 (km/h)
    int maxSpeed = settings::legal ? 25 : 50;

    int speedBars = maxSpeed == 50 ? 10 + 1 : 5 + 1;
    int smallSpeedBars = maxSpeed == 50 ? 50 + 1 : 25 + 1;

    int speedBarHeight = 8;
    int smallSpeedBarHeight = 4;

    int divider = 5;

    // Draw the red arc if over 30 km/h
    if (maxSpeed == 50)
    {
        int startAngle = speedArcStartAngle + (double)speedArcAngle / 2 + 22; // 22 is the offset (one bar)
        int previousColor = TFT_BLACK;                                        // Used to draw the gradient with smooth arc

        for (int i = 8; i > 0; i--)
        {
            int color = tft.color565(255 - i * 16, 0, 0); // From dark red to red

            tft.drawSmoothArc(120, 140, speedArcRadius - i + 1, speedArcRadius - i, startAngle, speedArcEndAngle, color, previousColor);

            previousColor = color;
        }
    }

    // Draw small speed bars
    for (int i = 0; i < smallSpeedBars; i++)
    {
        int startAngle = speedArcStartAngle + (double)speedArcAngle / (smallSpeedBars - 1) * i;
        int endAngle = startAngle + 1;

        tft.drawSmoothArc(120, 140, speedArcRadius, speedArcRadius - smallSpeedBarHeight, startAngle, endAngle, TFT_WHITE, TFT_BLACK);
    }

    // Draw speed bars
    for (int i = 0; i < speedBars; i++)
    {
        int startAngle = speedArcStartAngle + (double)speedArcAngle / (speedBars - 1) * i;
        int endAngle = startAngle + 1;

        tft.drawSmoothArc(120, 140, speedArcRadius, speedArcRadius - speedBarHeight, startAngle, endAngle, TFT_WHITE, TFT_BLACK);
    }

    // Draw the arc
    tft.drawSmoothArc(120, 140, speedArcRadius, speedArcRadius - 1, speedArcStartAngle, speedArcEndAngle, TFT_WHITE, TFT_BLACK);

    // Draw the speed text
    tft.loadFont(FONT_S);
    for (int i = 0; i < speedBars; i++)
    {
        int startAngle = 160 + (double)speedArcAngle / (speedBars - 1) * i + 1;

        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.setTextDatum(CC_DATUM);

        int speed = divider * i;

        int speedBarX = 120 + cos(startAngle * PI / 180.0) * (speedArcRadius - 18);
        int speedBarY = 140 + sin(startAngle * PI / 180.0) * (speedArcRadius - 18);

        tft.drawString(String(speed), speedBarX, speedBarY, 1);
    }
    tft.unloadFont();
}
void drawPowerArc()
{
    int powerArcRadius = 100;
    int powerArcAngle = 90;

    int powerArcStartAngle = 360 - powerArcAngle / 2;
    int powerArcEndAngle = powerArcAngle / 2;

    int maxPower = settings::legal ? 250 : settings::maxPower; // Divisible by 500
    int divider = maxPower == 250 ? 10 : 100;

    int powerBars = 5 + 1;
    int smallPowerBars = 20 + 1;

    int powerBarHeight = 6;
    int smallPowerBarHeight = 4;

    // Draw small power bars
    for (int i = 0; i < smallPowerBars; i++)
    {
        int startAngle = powerArcStartAngle + (double)powerArcAngle / (smallPowerBars - 1) * i;
        if (startAngle > 360)
            startAngle -= 360;

        if (startAngle == 0 || startAngle == 360)
        {
            tft.drawRect(120, 140 + powerArcRadius - smallPowerBarHeight, 2, smallPowerBarHeight, TFT_WHITE);
            continue;
        }

        if (i == smallPowerBars - 1) // Last bar (so that it doesnt extend past the arc)
            startAngle--;

        int endAngle = startAngle + 1;

        tft.drawSmoothArc(120, 140, powerArcRadius, powerArcRadius - smallPowerBarHeight, startAngle, endAngle, TFT_WHITE, TFT_BLACK);
    }

    // Draw the power bars
    for (int i = 0; i < powerBars; i++)
    {
        int startAngle = powerArcStartAngle + (double)powerArcAngle / (powerBars - 1) * i;
        if (startAngle > 360)
            startAngle -= 360;

        if (i == powerBars - 1) // Last bar (so that it doesnt extend past the arc)
            startAngle--;

        int endAngle = startAngle + 1;

        tft.drawSmoothArc(120, 140, powerArcRadius, powerArcRadius - powerBarHeight, startAngle, endAngle, TFT_WHITE, TFT_BLACK);
    }

    // Draw the power arc
    tft.drawSmoothArc(120, 140, powerArcRadius, powerArcRadius - 1, powerArcStartAngle, powerArcEndAngle, TFT_WHITE, TFT_BLACK);

    // Draw the power text
    tft.loadFont(FONT_S);
    for (int i = 1; i < powerBars; i++)
    {
        int startAngle = 360 - powerArcAngle / (powerBars - 1) * i + powerArcAngle * 1.5;

        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.setTextDatum(CC_DATUM);

        int power = (double)maxPower / (double)(powerBars - 1) * i / divider;

        int powerBarX = 120 + cos(startAngle * PI / 180.0) * (powerArcRadius - 14);
        int powerBarY = 140 + sin(startAngle * PI / 180.0) * (powerArcRadius - 14);

        tft.drawString(String(power), powerBarX, powerBarY, 1);
    }

    tft.drawString("x" + String(divider) + "W", 56, 200, 1);
    tft.unloadFont();
}

void updateSpeed()
{
    int s = speed;

    int speedArcRadius = 106;
    int speedArcInnerRadius = 102;

    int speedArcMaxAngle = 220;

    int speedArcStartAngle = 180 - speedArcMaxAngle / 2;
    int speedArcEndAngle = 180 + speedArcMaxAngle / 2;

    // Either 25 or 50 (km/h)
    int maxSpeed = settings::legal ? 25 : 50;

    if (s > 99)
        s = 99;

    else if (s < 0)
        s = 0;

    spr.createSprite(220, 220);
    spr.fillSprite(TFT_TRANSPARENT);

    // Draw the speed text
    spr.loadFont(FONT_XL);
    spr.setTextColor(TFT_WHITE, TFT_BLACK);
    spr.setTextDatum(TC_DATUM);

    spr.fillRect(110 - 48, 50, 96, 61, TFT_BLACK);
    spr.drawString(String(speed), 110, 50);
    spr.unloadFont();

    if (s > maxSpeed)
        s = maxSpeed;

    int endAngle = speedArcStartAngle + (double)speedArcMaxAngle / (double)maxSpeed * speed;

    spr.drawSmoothArc(110, 110, speedArcRadius, speedArcInnerRadius, speedArcStartAngle, speedArcEndAngle, TFT_BLACK, TFT_BLACK);
    if (s > 0)
        spr.drawSmoothArc(110, 110, speedArcRadius, speedArcInnerRadius, speedArcStartAngle, endAngle, TFT_GREEN, TFT_BLACK);

    spr.pushSprite(120 - 110, 140 - 110, TFT_TRANSPARENT);
    spr.deleteSprite();
}
void updatePower()
{
    int powerArcRadius = 106;
    int powerArcInnerRadius = 102;

    int p = power;

    int powerArcAngle = 90;

    int maxPower = settings::legal ? 250 : settings::maxPower;

    if (power > 9999)
        p = 9999;

    else if (power < 0)
        p = 0;

    spr.createSprite(220, 220);
    spr.fillSprite(TFT_TRANSPARENT);

    // spr.fillRect(110 - 40, 130, 80, 14, TFT_BLACK);
    // spr.loadFont(FONT_SM);
    // spr.setTextColor(TFT_WHITE, TFT_BLACK);
    // spr.setTextDatum(TC_DATUM);

    // String powerString = String(power);

    // while (powerString.length() < 4)
    //     powerString = "0" + powerString;

    // spr.drawString(powerString + "W", 110, 130);

    if (p > maxPower)
        p = maxPower;

    int powerArcStartAngle = powerArcAngle / 2;
    int powerArcEndAngle = 360 - powerArcAngle / 2;

    int endAngle = powerArcStartAngle - (double)powerArcAngle / (double)maxPower * p;

    if (endAngle < 0)
        endAngle += 360;

    spr.drawSmoothArc(110, 110, powerArcRadius, powerArcInnerRadius, powerArcEndAngle, powerArcStartAngle, TFT_BLACK, TFT_BLACK);

    if (p > 0)
        spr.drawSmoothArc(110, 110, powerArcRadius, powerArcInnerRadius, endAngle, powerArcStartAngle, TFT_YELLOW, TFT_BLACK);

    spr.pushSprite(120 - 110, 140 - 110, TFT_TRANSPARENT);
    spr.deleteSprite();

    // Draw the power text
}
void updateGear()
{
    int gearX = 120;
    int gearY = 158;

    spr.createSprite(64, 56);

    spr.fillSprite(TFT_BLACK);
    spr.setTextDatum(TC_DATUM);
    spr.setTextColor(TFT_WHITE, TFT_BLACK);

    spr.loadFont(FONT_L);
    spr.drawString(String(gear), 32, 0);
    spr.unloadFont();

    spr.pushSprite(gearX - 32, gearY);
    spr.deleteSprite();
}
