#pragma once
#ifndef __RGB565_COLORS_H__
#define __RGB565_COLORS_H__

#include <stdint.h>

// RGB565 Color Definitions
// Format: 5 bits Red, 6 bits Green, 5 bits Blue

// Basic Colors
#define RGB565_BLACK       0x0000  //   0,   0,   0
#define RGB565_WHITE       0xFFFF  // 255, 255, 255
#define RGB565_RED         0xF800  // 255,   0,   0
#define RGB565_GREEN       0x07E0  //   0, 255,   0
#define RGB565_BLUE        0x001F  //   0,   0, 255
#define RGB565_CYAN        0x07FF  //   0, 255, 255
#define RGB565_MAGENTA     0xF81F  // 255,   0, 255
#define RGB565_YELLOW      0xFFE0  // 255, 255,   0

// Grays
#define RGB565_DARKGRAY    0x39E7  //  64,  64,  64
#define RGB565_GRAY        0x7BEF  // 128, 128, 128
#define RGB565_LIGHTGRAY   0xC618  // 192, 192, 192

// Additional Colors
#define RGB565_ORANGE      0xFD20  // 255, 165,   0
#define RGB565_PURPLE      0x780F  // 128,   0, 128
#define RGB565_BROWN       0xA145  // 165,  42,  42
#define RGB565_PINK        0xFE19  // 255, 192, 203
#define RGB565_NAVY        0x000F  //   0,   0, 128
#define RGB565_MAROON      0x7800  // 128,   0,   0
#define RGB565_OLIVE       0x7BE0  // 128, 128,   0

// Utility macro to convert RGB888 to RGB565
#define RGB888_TO_RGB565(r, g, b) \
    ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | (((b) & 0xF8) >> 3))

#endif // __RGB565_COLORS_H__