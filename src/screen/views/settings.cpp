#include "settings.h"
#include "screen/tft.h"

SettingsView settingsView;

SettingsView::SettingsView()
{
    Category general("General");
}

void SettingsView::draw()
{
    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.loadFont(FONT_M);
    tft.setTextDatum(TC_DATUM);
    tft.drawString("Settings", TFT_WIDTH / 2, 8);
    tft.unloadFont();

    int y = 40;
    int padding = 8;

    int drawerWidth = TFT_WIDTH - 2 * padding;
    int drawerHeight = 24;

    for (int i = 0; i < categories.size(); i++)
    {
        categories[i].setPos(padding, y);
        categories[i].setSizes(drawerWidth, drawerHeight);
        categories[i].drawHeader();
        y += drawerHeight;
        y += padding;
    }
}

void Category::drawHeader()
{
    tft.drawRect(x, y, width, height, isHovered ? TFT_WHITE : TFT_DARKGREY);
    tft.drawString(name, x + width / 2, y + height / 2);
}
