#pragma once

#include <TFT_eSPI.h>
#include <Arduino.h>
#include <OpenFontRender.h>

#include "fonts/Font26.h"
#include "fonts/binaryttf.h"

typedef uint16_t Color;

#define FONT_BINARY binaryttf
#define FONT_M Lato26

#define COLOR_BACKGROUND 0x0841

extern TFT_eSPI tft;
extern TFT_eSprite spr;
extern OpenFontRender ofr;