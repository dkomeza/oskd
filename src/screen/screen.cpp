#include "screen.h"
#include "views/dashboard.h"
#include "views/settings.h"
#include "settings/settings.h"
#include "tft.h"

static const int BACKLIGHT_PIN = 26;
static const int minBrightness = 55;
static const int brightnessStep = 20;

void screen::setup()
{
    tft.init();
    tft.setRotation(4);
    tft.fillScreen(TFT_BLACK);

    delay(50);

    gpio_hold_dis((gpio_num_t)BACKLIGHT_PIN);
    pinMode(BACKLIGHT_PIN, OUTPUT);
}

void screen::lightUp()
{

    int max_brightness = minBrightness + (*settings.brightness * brightnessStep) + 1;

    for (int i = 0; i < max_brightness; i++)
    {
        analogWrite(BACKLIGHT_PIN, i);
        delay(5);
    }

    analogWrite(BACKLIGHT_PIN, max_brightness);
}

void screen::shutdown()
{
    int max_brightness = minBrightness + (*settings.brightness * brightnessStep);

    for (int i = max_brightness; i >= 0; i--)
    {
        analogWrite(BACKLIGHT_PIN, i);
        delay(2);
    }

    analogWrite(BACKLIGHT_PIN, 0);

    delay(50);
    gpio_hold_en((gpio_num_t)BACKLIGHT_PIN);
}

void screen::changeBrightness(int oldValue, int newValue)
{
    int old_brightness = minBrightness + (oldValue * brightnessStep);
    int new_brightness = minBrightness + (newValue * brightnessStep);

    int step = new_brightness > old_brightness ? 1 : -1;

    for (int i = old_brightness; i != new_brightness; i += step)
    {
        analogWrite(BACKLIGHT_PIN, i);
        delay(2);
    }
}