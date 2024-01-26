#include "screen.h"
#include "views/dashboard.h"
#include "views/settings.h"
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
    digitalWrite(BACKLIGHT_PIN, HIGH);

    switch (screen::view)
    {
    case View::Dashboard:
        dashboard::draw();
        break;

    default:
        break;
    }
}

void screen::shutdown()
{
    gpio_hold_en((gpio_num_t)BACKLIGHT_PIN);
    digitalWrite(BACKLIGHT_PIN, LOW);
}

void screen::draw()
{
    switch (screen::view)
    {
    case View::Dashboard:
        dashboard::draw();
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
