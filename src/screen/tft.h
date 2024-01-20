#pragma once

#include <TFT_eSPI.h>
#include <Arduino.h>
#include <OpenFontRender.h>

#include "fonts/Font18.h"
#include "fonts/Font28.h"
#include "fonts/Font96.h"
#include "fonts/Font164.h"
#include "fonts/binaryttf.h"

typedef uint16_t Color;

#define FONT_MEDIUM Baloo18
#define FONT_LARGE Inconsolata28
#define FONT_HUGE Inconsolata96
#define FONT_GIANT Inconsolata164
#define FONT_BINARY binaryttf

#define COLOR_BACKGROUND 0x0841

extern TFT_eSPI tft;
extern TFT_eSprite spr;
extern OpenFontRender ofr;