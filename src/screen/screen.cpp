#include "screen.h"
#include "views/dashboard.h"
#include "views/settings.h"
#include "settings/settings.h"
#include "tft.h"

View screen::view = View::Dashboard;

static const int BACKLIGHT_PIN = 26;

void screen::setup()
{
    tft.init();
    tft.setRotation(4);
    tft.fillScreen(TFT_BLACK);

    delay(50);

    gpio_hold_dis((gpio_num_t)BACKLIGHT_PIN);
    pinMode(BACKLIGHT_PIN, OUTPUT);

    switch (screen::view)
    {
    case View::Dashboard:
        dashboard::draw();
        break;

    case View::Settings:
        settingsView.draw();
        break;

    default:
        break;
    }
}

void screen::lightUp()
{

    int max_brightness = 155 + (*settings.brightness * 10) + 1;

    for (int i = 0; i < max_brightness; i++)
    {
        analogWrite(BACKLIGHT_PIN, i);
        delay(5);
    }

    analogWrite(BACKLIGHT_PIN, max_brightness);
}

void screen::shutdown()
{
    int max_brightness = 155 + (*settings.brightness * 10) + 1;

    for (int i = max_brightness; i >= 0; i--)
    {
        analogWrite(BACKLIGHT_PIN, i);
        delay(2);
    }

    analogWrite(BACKLIGHT_PIN, 0);

    delay(50);
    gpio_hold_en((gpio_num_t)BACKLIGHT_PIN);
}

void screen::draw()
{
    switch (screen::view)
    {
    case View::Dashboard:
        dashboard::draw();
        break;

    case View::Settings:
        settingsView.draw();
        break;

    default:
        break;
    }
}

void screen::loop(bool force)
{
    switch (screen::view)
    {
    case View::Dashboard:
        dashboard::update(force);
        break;

    default:
        break;
    }
}

void screen::setView(View view)
{
    if (screen::view == view)
        return;

    screen::view = view;
}
