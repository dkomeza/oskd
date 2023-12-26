#include "controller.h"

static const int MAX_GEAR = 5;
static const int MIN_GEAR = 0;

void loadSettings();
void loadGear();

void settings::setup()
{
    loadSettings();
    loadGear();
}

void loadSettings()
{
}