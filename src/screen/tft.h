#pragma once

#include <TFT_eSPI.h>
#include <Arduino.h>
#include <OpenFontRender.h>

#include "fonts/Font12.h"
#include "fonts/Font20.h"
#include "fonts/Font26.h"
#include "fonts/FONT_L.h"
#include "fonts/Font96.h"
#include "fonts/binaryttf.h"

typedef uint16_t Color;

#define FONT_BINARY binaryttf
#define FONT_S Lato12
#define FONT_SM Lato20
#define FONT_M Lato26
#define FONT_L Lato60
#define FONT_XL Lato82

#define COLOR_BACKGROUND 0x0841

extern TFT_eSPI tft;
extern TFT_eSprite spr;
extern OpenFontRender ofr;