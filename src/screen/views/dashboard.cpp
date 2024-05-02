#include "dashboard.h"
#include "screen/tft.h"
#include "controller/controller.h"
#include "io/io.h"

static const int BATTERY_MAX_VOLTAGE = 520; // 52.0V
static const int BATTERY_MIN_VOLTAGE = 420; // 42.0V

#define UPDATE_INTERVAL 100

int speed = 0;
int power = 0;
int gear = 0;
int battery = 0;
int temperature = 0;
bool brake = false;

Dashboard dashboard;

void Dashboard::setup()
{
    tft.fillScreen(TFT_BLACK);

    int mainBoxRadius = 24;
    tft.fillSmoothRoundRect(0, -mainBoxRadius, 240, 260 + mainBoxRadius, mainBoxRadius, COLOR_BACKGROUND);

    drawBattery();
    drawMain();

    updateBattery();
    updateSpeed();
    updatePower();
    updateGear();
}

void Dashboard::update()
{
    if (bounds.power != lastBounds.power || bounds.speed != lastBounds.speed)
    {
        drawMain();

        updateBattery();
        updateSpeed();
        updatePower();
        updateGear();

        return;
    }

    long now = millis();

    if (now < lastUpdate)
        lastUpdate = now;

    if (now - lastUpdate < UPDATE_INTERVAL)
        return;

    lastUpdate = now;

    if (io.battery != battery)
        updateBattery();

    if (controller.speed != speed)
        updateSpeed();

    if (controller.power != power)
        updatePower();

    if (controller.gear != gear)
        updateGear();

    if (controller.brake != brake)
        updateGear();
}

/**
 * @brief Set the new bounds for the dashboard
 * @param bounds The new bounds
 */
void Dashboard::setBounds(DashboardBounds bounds)
{
    this->lastBounds = this->bounds;
    this->bounds = bounds;
}

/**
 * @brief Draw the battery outline
 */
void Dashboard::drawBattery()
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

/**
 * @brief Draw the main dashboard components
 */
void Dashboard::drawMain()
{
    lastBounds = bounds;

    int size = 220;
    spr.createSprite(size, size);
    spr.fillSprite(TFT_TRANSPARENT);

    // Draw the main background
    int mainBackgroundRadius = 110;
    int mainBackgroundX = 120;
    int mainBackgroundY = 140;
    spr.fillSmoothCircle(mainBackgroundRadius, mainBackgroundRadius, mainBackgroundRadius, TFT_BLACK, COLOR_BACKGROUND);

    // Draw the red wedge
    int wedgeX = 75;
    int wedgeWidth = 30;
    int wedgeY = 122;
    Color color = tft.color565(192, 0, 0);
    spr.drawWedgeLine(wedgeX, wedgeY, wedgeX + wedgeWidth, wedgeY, 0.5, 1.5, color, TFT_BLACK);
    spr.drawWedgeLine(size - wedgeX, wedgeY, size - wedgeX - wedgeWidth, wedgeY, 0.5, 1.5, color, TFT_BLACK);
    spr.drawWideLine(wedgeX + wedgeWidth + 2.5, wedgeY, size - wedgeX - wedgeWidth - 2.5, wedgeY, 3, color, color);

    // Draw the speed arc
    drawSpeedArc();

    // Draw the power arc
    drawPowerArc();

    spr.pushSprite(mainBackgroundX - mainBackgroundRadius, mainBackgroundY - mainBackgroundRadius, TFT_TRANSPARENT);
    spr.deleteSprite();
}

/**
 * @brief Draw the speed arc
 */
void Dashboard::drawSpeedArc()
{
    int size = 220;

    int speedArcRadius = 100;
    int speedArcAngle = 220;
    int speedArcStartAngle = 180 - speedArcAngle / 2;
    int speedArcEndAngle = 180 + speedArcAngle / 2;

    int speedBars = bounds.speed == 50 ? 10 + 1 : 5 + 1;
    int smallSpeedBars = bounds.speed == 50 ? 50 + 1 : 25 + 1;

    int speedBarHeight = 8;
    int smallSpeedBarHeight = 4;

    int divider = 5;

    if (bounds.speed == 50)
    {
        int startAngle = speedArcStartAngle + (double)speedArcAngle / 2 + 22; // 22 is the offset (one bar)
        int previousColor = TFT_BLACK;                                        // Used to draw the gradient with smooth arc

        for (int i = 8; i > 0; i--)
        {
            int color = tft.color565(255 - i * 16, 0, 0); // From dark red to red

            spr.drawSmoothArc(size / 2, size / 2, speedArcRadius - i + 1, speedArcRadius - i, startAngle, speedArcEndAngle, color, previousColor);

            previousColor = color;
        }
    }

    for (int i = 0; i < smallSpeedBars; i++)
    {
        int startAngle = speedArcStartAngle + (double)speedArcAngle / (smallSpeedBars - 1) * i;
        int endAngle = startAngle + 1;

        spr.drawSmoothArc(size / 2, size / 2, speedArcRadius, speedArcRadius - smallSpeedBarHeight, startAngle, endAngle, TFT_WHITE, TFT_BLACK);
    }

    for (int i = 0; i < speedBars; i++)
    {
        int startAngle = speedArcStartAngle + (double)speedArcAngle / (speedBars - 1) * i;
        int endAngle = startAngle + 1;

        spr.drawSmoothArc(size / 2, size / 2, speedArcRadius, speedArcRadius - speedBarHeight, startAngle, endAngle, TFT_WHITE, TFT_BLACK);
    }

    // Draw the arc
    spr.drawSmoothArc(size / 2, size / 2, speedArcRadius, speedArcRadius - 1, speedArcStartAngle, speedArcEndAngle, TFT_WHITE, TFT_BLACK);

    // Draw the speed text
    spr.loadFont(FONT_S);
    for (int i = 0; i < speedBars; i++)
    {
        int startAngle = 160 + (double)speedArcAngle / (speedBars - 1) * i + 1;

        spr.setTextColor(TFT_WHITE, TFT_BLACK, true);
        spr.setTextDatum(CC_DATUM);

        int speed = divider * i;

        int speedBarX = size / 2 + cos(startAngle * PI / 180.0) * (speedArcRadius - 18);
        int speedBarY = size / 2 + sin(startAngle * PI / 180.0) * (speedArcRadius - 18);

        spr.drawString(String(speed), speedBarX, speedBarY, 1);
    }
    spr.unloadFont();
}

/**
 * @brief Draw the power arc
 */
void Dashboard::drawPowerArc()
{
    int size = 220;

    int powerArcRadius = 100;
    int powerArcAngle = 90;

    int powerArcStartAngle = 360 - powerArcAngle / 2;
    int powerArcEndAngle = powerArcAngle / 2;

    int maxPower = bounds.power;
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
            spr.drawRect(size / 2, size / 2 + powerArcRadius - smallPowerBarHeight, 2, smallPowerBarHeight, TFT_WHITE);
            continue;
        }

        if (i == smallPowerBars - 1) // Last bar (so that it doesnt extend past the arc)
            startAngle--;

        int endAngle = startAngle + 1;

        spr.drawSmoothArc(size / 2, size / 2, powerArcRadius, powerArcRadius - smallPowerBarHeight, startAngle, endAngle, TFT_WHITE, TFT_BLACK);
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

        spr.drawSmoothArc(size / 2, size / 2, powerArcRadius, powerArcRadius - powerBarHeight, startAngle, endAngle, TFT_WHITE, TFT_BLACK);
    }

    // Draw the power arc
    spr.drawSmoothArc(size / 2, size / 2, powerArcRadius, powerArcRadius - 1, powerArcStartAngle, powerArcEndAngle, TFT_WHITE, TFT_BLACK);

    // Draw the power text
    spr.loadFont(FONT_S);
    for (int i = 1; i < powerBars; i++)
    {
        int startAngle = 360 - powerArcAngle / (powerBars - 1) * i + powerArcAngle * 1.5;

        spr.setTextColor(TFT_WHITE, TFT_BLACK, true);
        spr.setTextDatum(CC_DATUM);

        int power = (double)maxPower / (double)(powerBars - 1) * i / divider;

        int powerBarX = size / 2 + cos(startAngle * PI / 180.0) * (powerArcRadius - 14);
        int powerBarY = size / 2 + sin(startAngle * PI / 180.0) * (powerArcRadius - 14);

        spr.drawString(String(power), powerBarX, powerBarY, 1);
    }

    spr.drawString("x" + String(divider) + "W", 51, 170, 1);
    spr.unloadFont();
}

/**
 * @brief Update the battery value
 */
void Dashboard::updateBattery()
{
    battery = io.battery;

    int padding = 1;

    int innerWidth = 52;
    int innerHeight = 16;

    int batteryX = 240 - 56 - 12 + 2;
    int batteryY = 8 + 2;

    spr.createSprite(innerWidth, innerHeight);
    spr.fillSprite(COLOR_BACKGROUND);

    int batteryPercent = (double)(battery - BATTERY_MIN_VOLTAGE) / (double)(BATTERY_MAX_VOLTAGE - BATTERY_MIN_VOLTAGE) * 100.0;

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

    int voltageX = 8;
    int voltageY = 8;

    String voltageString = String(battery / 10.0, 1) + "V";

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

void Dashboard::updateSpeed()
{
    speed = controller.speed;

    int s = speed;

    int speedArcRadius = 106;
    int speedArcInnerRadius = 102;

    int speedArcMaxAngle = 220;

    int speedArcStartAngle = 180 - speedArcMaxAngle / 2;
    int speedArcEndAngle = 180 + speedArcMaxAngle / 2;

    // Either 25 or 50 (km/h)
    int maxSpeed = bounds.speed;

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
    spr.drawString(String(s), 110, 50);
    spr.unloadFont();

    if (s > maxSpeed)
        s = maxSpeed;

    int endAngle = speedArcStartAngle + (double)speedArcMaxAngle / (double)maxSpeed * s;

    spr.drawSmoothArc(110, 110, speedArcRadius, speedArcInnerRadius, speedArcStartAngle, speedArcEndAngle, TFT_BLACK, TFT_BLACK);
    if (s > 0)
        spr.drawSmoothArc(110, 110, speedArcRadius, speedArcInnerRadius, speedArcStartAngle, endAngle, TFT_GREEN, TFT_BLACK);

    spr.pushSprite(120 - 110, 140 - 110, TFT_TRANSPARENT);
    spr.deleteSprite();
}

void Dashboard::updatePower()
{
    power = controller.power;

    int powerArcRadius = 106;
    int powerArcInnerRadius = 102;

    int p = power;

    int powerArcAngle = 90;

    int maxPower = bounds.power;

    if (p > 9999)
        p = 9999;

    else if (p < 0)
        p = 0;

    spr.createSprite(220, 220);
    spr.fillSprite(TFT_TRANSPARENT);

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
}

void Dashboard::updateGear()
{
    gear = controller.gear;
    brake = controller.brake;

    int gearX = 120;
    int gearY = 164;

    spr.createSprite(64, 56);

    spr.fillSprite(TFT_BLACK);
    spr.setTextDatum(TC_DATUM);

    if (brake)
        spr.setTextColor(TFT_RED, TFT_BLACK);
    else
        spr.setTextColor(TFT_WHITE, TFT_BLACK);

    spr.loadFont(FONT_L);
    if (gear == 6)
        spr.drawString("W", 32, 0);
    else
        spr.drawString(String(gear), 32, 0);
    spr.unloadFont();

    spr.pushSprite(gearX - 32, gearY);
    spr.deleteSprite();
}
