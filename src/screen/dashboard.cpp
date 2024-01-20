#include "dashboard.h"
#include "tft.h"
#include "data/data.h"

static const int BATTERY_MAX_VOLTAGE = 520; // 52.0V
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

void dashboard::update()
{
    if (data::voltage != voltage)
    {
        voltage = data::voltage;
        updateVoltage();
        updateBattery();
    }

    // if (data::power != power)
    // {
    //     power = data::power;
    //     updatePower();
    // }

    // if (data::speed != speed)
    // {
    //     speed = data::speed;
    //     updateSpeed();
    // }

    // if (data::gear != gear)
    // {
    //     gear = data::gear;
    //     updateGear();
    // }
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
    int maxSpeed = 50;

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

    int maxPower = 1000; // Divisible by 500
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
    for (int i = 0; i < powerBars; i++)
    {
        int startAngle = 360 - powerArcAngle / (powerBars - 1) * i + powerArcAngle * 1.5;

        tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
        tft.setTextDatum(CC_DATUM);

        int power = (double)maxPower / (double)(powerBars - 1) * i / divider;

        int powerBarX = 120 + cos(startAngle * PI / 180.0) * (powerArcRadius - 14);
        int powerBarY = 140 + sin(startAngle * PI / 180.0) * (powerArcRadius - 14);

        tft.drawString(String(power), powerBarX, powerBarY, 1);
    }
}

// void updatePower()
// {
//     int powerArcRadius = 106;
//     int powerArcInnerRadius = 102;

//     int powerArcMaxAngle = 90;

//     int maxPower = 1500;

//     int pow = data::power > maxPower ? maxPower : data::power;

//     if (pow < 0)
//         pow = 0;

//     int endAngle = 45;
//     int absEndAngle = 360 - endAngle;
//     int startAngle = endAngle - (double)pow / (double)maxPower * powerArcMaxAngle;

//     if (startAngle < 0)
//         startAngle = 360 + startAngle;

//     if (startAngle != endAngle)
//         tft.drawSmoothArc(120, 140, powerArcRadius, powerArcInnerRadius, startAngle, endAngle, TFT_GREEN, TFT_BLACK);

//     if (pow == maxPower)
//         return;
//     tft.drawSmoothArc(120, 140, powerArcRadius, powerArcInnerRadius, absEndAngle, startAngle, TFT_BLACK, TFT_BLACK);
// }

// void updateSpeed()
// {
//     String speedString = String(speed).c_str();

//     tft.loadFont(FONT_HUGE);

//     tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
//     tft.setTextDatum(CC_DATUM);

//     int padding = tft.textWidth("00");
//     tft.setTextPadding(padding);

//     tft.drawString(speedString, 120, 125);

//     tft.unloadFont();

//     int speedArcRadius = 106;
//     int speedArcInnerRadius = 102;

//     int speedArcMaxAngle = 220;

//     int maxSpeed = 50;

//     int speed = data::speed > maxSpeed ? maxSpeed : data::speed;

//     if (speed < 0)
//         speed = 0;

//     int startAngle = 180 - speedArcMaxAngle / 2;
//     int endAngle = startAngle + (double)speed / (double)maxSpeed * speedArcMaxAngle;

//     if (startAngle != endAngle)
//         tft.drawSmoothArc(120, 140, speedArcRadius, speedArcInnerRadius, startAngle, endAngle, TFT_GREEN, TFT_BLACK);

//     if (speed == maxSpeed)
//         return;

//     int absEndAngle = startAngle + speedArcMaxAngle;
//     tft.drawSmoothArc(120, 140, speedArcRadius, speedArcInnerRadius, endAngle, absEndAngle, TFT_BLACK, TFT_BLACK);
// }

// void updateGear()
// {
//     int gearX = 120;
//     int gearY = 150;

//     spr.createSprite(64, 64);
//     spr.fillSprite(TFT_BLACK);

//     ofr.setDrawer(spr);

//     ofr.setFontColor(TFT_WHITE, TFT_BLACK);

//     if (ofr.loadFont(FONT_BINARY, sizeof(FONT_BINARY)))
//     {
//         Serial.println("dupa");
//     }
//     ofr.setCursor(32, 0);

//     ofr.setAlignment(Align::TopCenter);
//     ofr.setFontSize(64);
//     ofr.cprintf("%d", gear);

//     ofr.unloadFont();

//     spr.pushSprite(gearX - 32, gearY);
//     spr.deleteSprite();
// }
