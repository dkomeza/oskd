#include "screen.h"
#include "dashboard.h"
#include "settings.h"
#include "tft.h"

View currentView = View::Dashboard;

static const int BACKLIGHT_PIN = 26;

void screen::setup()
{
    tft.init();
    tft.setRotation(4);
    tft.fillScreen(TFT_BLACK);

    delay(50);

    pinMode(BACKLIGHT_PIN, OUTPUT);
    digitalWrite(BACKLIGHT_PIN, HIGH);

    switch (currentView)
    {
    case View::Dashboard:
        dashboard::draw();
        break;
    
    default:
        break;
    }
}

void screen::loop()
{
}

void screen::setView(View view)
{
    if (currentView == view)
        return;

    currentView = view;
}