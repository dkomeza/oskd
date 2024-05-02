#include "settings.h"
#include "screen/tft.h"
#include "settings/settings.h"

SettingsView settingsView;

SettingsView::SettingsView()
{
    Category *general = new Category("General", this);
    Setting *speedLimit = new Setting("Speed limit", SPEED_LIMIT_ADDRESS);
    general->addSetting(speedLimit);
    Setting *wheelSize = new Setting("Wheel size", WHEEL_SIZE_ADDRESS);
    general->addSetting(wheelSize);
    categories.push_back(general);

    Category *motor = new Category("Motor", this);
    Setting *polePairs = new Setting("Pole pairs", MOTOR_POLE_PAIRS_ADDRESS);
    motor->addSetting(polePairs);
    Setting *wheelSpeedPulses = new Setting("Wheel speed pulses", WHEEL_SPEED_PULSES_ADDRESS);
    motor->addSetting(wheelSpeedPulses);
    Setting *motorPhaseClassification = new Setting("Phase classification", MOTOR_PHASE_CLASSIFICATION_ADDRESS);
    motor->addSetting(motorPhaseClassification);
    categories.push_back(motor);

    Category *powerAssist = new Category("Power assist", this);
    Setting *controlMode = new Setting("Control mode", POWER_ASSIST_CONTROL_MODE_ADDRESS);
    powerAssist->addSetting(controlMode);
    Setting *pasParameterSelect = new Setting("PAS parameter select", PAS_PARAMETER_SELECT_ADDRESS);
    powerAssist->addSetting(pasParameterSelect);
    Setting *PASTuning = new Setting("PAS tuning", POWER_ASSIST_TUNING_ADDRESS);
    powerAssist->addSetting(PASTuning);
    categories.push_back(powerAssist);

    Category *throttle = new Category("Throttle", this);
    Setting *throttleStartup = new Setting("Startup", THROTTLE_STARTUP_ADDRESS);
    throttle->addSetting(throttleStartup);
    Setting *throttleFunction = new Setting("Function", THROTTLE_FUNCTION_ADDRESS);
    throttle->addSetting(throttleFunction);
    Setting *throttleSpeedLimit = new Setting("Speed limit", THROTTLE_SPEED_LIMIT_ADDRESS);
    throttle->addSetting(throttleSpeedLimit);
    categories.push_back(throttle);

    Category *controller = new Category("Controller", this);
    Setting *powerMonitoring = new Setting("Power monitoring", POWER_MONITORING_ADDRESS);
    controller->addSetting(powerMonitoring);
    Setting *controllerMaximumCurrent = new Setting("Maximum current", CONTROLLER_MAXIMUM_CURRENT_ADDRESS);
    controller->addSetting(controllerMaximumCurrent);
    Setting *controllerMinimumVoltage = new Setting("Minimum voltage", CONTROLLER_MINIMUM_VOLTAGE_ADDRESS);
    controller->addSetting(controllerMinimumVoltage);
    Setting *controllerRegenBrake = new Setting("Regen brake", CONTROLLER_REGEN_BRAKE_ADDRESS);
    controller->addSetting(controllerRegenBrake);
    categories.push_back(controller);

    Category *display = new Category("Display", this);
    Setting *brightness = new Setting("Brightness", DISPLAY_BRIGHTNESS_ADDRESS);
    display->addSetting(brightness);
    Setting *displayTemperature = new Setting("Temperature", DISPLAY_TEMPERATURE_DISPLAY_ADDRESS, true);
    display->addSetting(displayTemperature);
    Setting *autoSleep = new Setting("Auto sleep", DISPLAY_AUTO_SLEEP_ADDRESS);
    display->addSetting(autoSleep);
    categories.push_back(display);
}

void SettingsView::draw()
{
    tft.fillScreen(TFT_BLACK);

    int y = 40;
    int padding = 8;

    int drawerWidth = TFT_WIDTH - 2 * padding;
    int drawerHeight = 36;

    offsetDiff = drawerHeight + padding;

    int containerHeight = categories.size() * drawerHeight + (categories.size() - 1) * padding;
    int containerSpace = TFT_HEIGHT - 40;

    if (containerHeight > containerSpace)
        drawerWidth -= 16;

    for (int i = 0; i < categories.size(); i++)
    {
        categories[i]->setPos(padding, y);
        categories[i]->setSizes(drawerWidth, drawerHeight);

        categories[i]->drawHeader();
        y += drawerHeight;
        y += padding;
    }

    if (hovered != -1)
    {
        categories[hovered]->setHovered(true);
    }

    tft.fillRect(0, 0, TFT_WIDTH, 40, TFT_BLACK);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.loadFont(FONT_M);
    tft.setTextDatum(TC_DATUM);
    tft.drawString("Settings", TFT_WIDTH / 2, 8);
    tft.unloadFont();

    drawScrollBar();
}

void SettingsView::drawScrollBar()
{
    int containerHeight = categories.size() * 36 + (categories.size() - 1) * 8;
    int containerSpace = TFT_HEIGHT - 40;

    if (containerHeight <= containerSpace)
    {
        return;
    }

    int scrollBarHeight = containerSpace * containerSpace / containerHeight;
    int scrollBarY = offset * containerSpace / containerHeight;

    spr.createSprite(4, containerSpace);
    spr.fillSprite(TFT_BLACK);
    spr.fillSmoothRoundRect(0, scrollBarY, 4, scrollBarHeight, 2, TFT_DARKGREY);
    spr.pushSprite(TFT_WIDTH - 12, 40);
    spr.deleteSprite();
}

void SettingsView::handleClick(BUTTON button)
{
    if (selected != -1)
    {
        categories[selected]->handleClick(button);
        return;
    }

    if (button == BUTTON::Power)
    {
        selected = hovered;
        categories[selected]->draw();
        return;
    }

    int direction = button == BUTTON::Up ? 1 : -1;

    int newHovered = hovered + direction;

    if (newHovered < 0 || newHovered >= categories.size())
    {
        return;
    }

    categories[hovered]->setHovered(false);
    hovered = newHovered;
    categories[hovered]->setHovered(true);

    handleScroll();
}

void SettingsView::handleScroll()
{
    int categoryBottom = categories[hovered]->y - offset + categories[hovered]->height;

    if (categoryBottom > TFT_HEIGHT)
        offset += offsetDiff;
    else if (categories[hovered]->y - offset < 40)
        offset -= offsetDiff;
    else
        return;

    for (int i = 0; i < categories.size(); i++)
    {
        categories[i]->setScroll(offset);

        categories[i]->drawHeader();
    }

    drawScrollBar();
}

void SettingsView::deselectCategory()
{
    selected = -1;
    draw();
}

void Category::drawHeader()
{
    int y = this->y - offset;

    if (y < 40 || y + height > TFT_HEIGHT)
    {
        return;
    }

    tft.fillSmoothRoundRect(x, y, width, height, 4, isHovered ? TFT_DARKGREY : COLOR_BACKGROUND);

    tft.setTextColor(TFT_WHITE, isHovered ? TFT_DARKGREY : COLOR_BACKGROUND);
    tft.loadFont(FONT_M);
    tft.setTextDatum(ML_DATUM);
    tft.drawString(name, x + 4, y + height / 2);
    tft.unloadFont();
}

void Category::setHovered(bool isHovered)
{
    this->isHovered = isHovered;
    this->drawHeader();
}

void Category::handleClick(BUTTON button)
{
    if (selected != -1)
    {
        settings[selected]->handleClick(button);
        return;
    }

    if (button == BUTTON::Power)
    {
        if (hovered == -1)
        {
            parent->deselectCategory();
            selected = -1;
            hovered = 0;
            return;
        }
        selected = hovered;
        settings[selected]->draw();
        return;
    }

    int direction = button == BUTTON::Up ? 1 : -1;

    int newHovered = hovered + direction;

    if (newHovered == -1)
    {
        settings[hovered]->setHovered(false);
        hovered = -1;
        this->drawBackButton();
        return;
    }

    if (newHovered < -1 || newHovered >= settings.size())
    {
        return;
    }

    settings[hovered]->setHovered(false);
    hovered = newHovered;

    settings[hovered]->setHovered(true);
    this->drawBackButton();

    handleScroll();
}

void Category::draw()
{
    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.loadFont(FONT_M);
    tft.setTextDatum(TC_DATUM);
    tft.drawString(name, TFT_WIDTH / 2, 8);
    tft.unloadFont();

    drawBackButton();

    if (settings.size() == 0)
    {
        return;
    }

    int y = 40;
    int padding = 8;

    int drawerWidth = TFT_WIDTH - 2 * padding;
    int drawerHeight = 36;

    offsetDiff = drawerHeight + padding;

    int containerHeight = settings.size() * drawerHeight + (settings.size() - 1) * padding;
    int containerSpace = TFT_HEIGHT - 40;

    if (containerHeight > containerSpace)
        drawerWidth -= 16;

    for (int i = 0; i < settings.size(); i++)
    {
        settings[i]->setPos(padding, y);
        settings[i]->setSizes(drawerWidth, drawerHeight);

        settings[i]->drawHeader();
        y += drawerHeight;
        y += padding;
    }

    if (hovered != -1)
    {
        settings[hovered]->setHovered(true);
    }

    drawScrollBar();
}

void Category::handleScroll()
{
    int categoryBottom = settings[hovered]->y - selfOffset + settings[hovered]->height;

    if (categoryBottom > TFT_HEIGHT)
        selfOffset += offsetDiff;
    else if (settings[hovered]->y - selfOffset < 40)
        selfOffset -= offsetDiff;
    else
        return;

    for (int i = 0; i < settings.size(); i++)
    {
        settings[i]->setScroll(selfOffset);

        settings[i]->draw();
    }

    drawScrollBar();
}

void Category::drawScrollBar()
{
    int containerHeight = settings.size() * 36 + (settings.size() - 1) * 8;
    int containerSpace = TFT_HEIGHT - 40;

    if (containerHeight <= containerSpace)
    {
        return;
    }

    int scrollBarHeight = containerSpace * containerSpace / containerHeight;
    int scrollBarY = selfOffset * containerSpace / containerHeight;

    spr.createSprite(4, containerSpace);
    spr.fillSprite(TFT_BLACK);
    spr.fillSmoothRoundRect(0, scrollBarY, 4, scrollBarHeight, 2, TFT_DARKGREY);
    spr.pushSprite(TFT_WIDTH - 12, 40);
    spr.deleteSprite();
}

void Category::drawBackButton()
{
    bool isHovered = hovered == -1;

    Color background = isHovered ? TFT_DARKGREY : COLOR_BACKGROUND;
    tft.fillSmoothCircle(20, 20, 12, background);
    tft.fillTriangle(16, 20, 24, 16, 24, 24, TFT_WHITE);
}

void Category::deselectSetting()
{
    selected = -1;
    draw();
}

void Setting::drawHeader()
{
    int y = this->y - offset;

    if (y < 40 || y + height > TFT_HEIGHT)
    {
        return;
    }

    tft.fillSmoothRoundRect(x, y, width, height, 4, isHovered ? TFT_DARKGREY : COLOR_BACKGROUND);

    tft.setTextColor(TFT_WHITE, isHovered ? TFT_DARKGREY : COLOR_BACKGROUND);
    tft.loadFont(FONT_M);
    tft.setTextDatum(ML_DATUM);
    tft.drawString(name, x + 4, y + height / 2);
    tft.unloadFont();
}

void Setting::setHovered(bool isHovered)
{
    this->isHovered = isHovered;
    this->drawHeader();
}

void Setting::handleClick(BUTTON button)
{
    if (button == BUTTON::Power)
    {
        if (backHovered)
        {
            backHovered = false;
            selected = false;
            parent->deselectSetting();
            return;
        }
        else
        {
            selected = !selected;
            drawSetting();
            return;
        }
    }

    int direction = button == BUTTON::Up ? 1 : -1;

    if (selected)
    {
        if (isBool)
        {
            settings.setSetting(addr, !*value);
            drawSetting();
        }
        else if (addr == WHEEL_SIZE_ADDRESS)
        {
            settings.setWheelSize(direction);
            drawSetting();
        }
        else
        {
            settings.setSetting(addr, *value + direction);
            drawSetting();
        }
    }
    else
    {
        if (backHovered && direction == 1)
        {
            backHovered = false;
            drawBackButton();
            drawSetting();
        }
        else if (!backHovered && direction == -1)
        {
            backHovered = true;
            drawBackButton();
            drawSetting();
        }
    }
}

void Setting::draw()
{
    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.loadFont(FONT_M);
    tft.setTextDatum(TC_DATUM);
    tft.drawString(name, TFT_WIDTH / 2, 8);
    tft.unloadFont();

    drawBackButton();

    drawSetting();
}

void Setting::drawSetting()
{
    Color background = backHovered ? COLOR_BACKGROUND : TFT_DARKGREY;
    Color textColor = selected ? TFT_YELLOW : TFT_WHITE;

    spr.createSprite(TFT_WIDTH, 40);
    spr.fillSprite(TFT_BLACK);
    spr.fillSmoothRoundRect(8, 0, TFT_WIDTH - 16, 40, 8, background);

    spr.setTextColor(TFT_WHITE, background);
    spr.setTextDatum(ML_DATUM);
    spr.loadFont(FONT_S);
    spr.drawString(name, 16, 20);

    spr.setTextColor(textColor, background);
    spr.setTextDatum(MR_DATUM);
    if (isBool)
    {
        spr.drawString(*value ? "On" : "Off", TFT_WIDTH - 16, 20);
    }
    else if (addr == WHEEL_SIZE_ADDRESS)
    {
        const wheelSize *size = nullptr;

        for (int i = 0; i < WHEEL_SIZE_COUNT; i++)
        {
            if (wheelSizes[i].value == *value)
            {
                size = &wheelSizes[i];
                break;
            }
        }

        if (size == nullptr)
            size = &wheelSizes[0];

        spr.drawString(size->name, TFT_WIDTH - 16, 20);
    }
    else
    {
        spr.drawString(String(*value), TFT_WIDTH - 16, 20);
    }

    spr.unloadFont();
    spr.pushSprite(0, 70);
    spr.deleteSprite();
}

void Setting::drawBackButton()
{
    Color background = backHovered ? TFT_DARKGREY : COLOR_BACKGROUND;
    tft.fillSmoothCircle(20, 20, 12, background);
    tft.fillTriangle(16, 20, 24, 16, 24, 24, TFT_WHITE);
}
