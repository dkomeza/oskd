#pragma once

#include <TFT_eSPI.h>
#include <Arduino.h>

#include "fonts/FONT_S.h"
#include "fonts/FONT_SM.h"
#include "fonts/FONT_M.h"
#include "fonts/FONT_L.h"
#include "fonts/FONT_XL.h"

typedef uint16_t Color;

#define FONT_S Lato12
#define FONT_SM Lato20
#define FONT_M Lato26
#define FONT_L Lato60
#define FONT_XL Lato82

#define COLOR_BACKGROUND 0x0841

extern TFT_eSPI tft;
extern TFT_eSprite spr;