#include "screen.h"
#include "views/dashboard.h"
#include "tft.h"
#include "settings/settings.h"

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
        settings.draw();
        break;

    default:
        break;
    }
}

void screen::lightUp()
{

    int max_brightness = 155 + (settings.brightness * 10) + 1;

    for (int i = 0; i < max_brightness; i++)
    {
        analogWrite(BACKLIGHT_PIN, i);
        delay(5);
    }

    analogWrite(BACKLIGHT_PIN, max_brightness);
}

void screen::shutdown()
{
    for (int i = 255; i >= 0; i--)
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
        settings.draw();
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

    case View::Settings:
        settings.update();
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
