/**
 * @file EARS_RGB565_COLORS.h
 * @author Julian (51fiftyone51fiftyone@gmail.com)
 * @brief Define the Colours for EARS_RGB565 format
 * @version 0.1
 * @date 20251229
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#ifndef __RGB555_COLOURS_H_
#define __RGB555_COLOURS_H_

#include <stdint.h>

// EARS_RGB565 Color Definitions
// Format: 5 bits Red, 6 bits Green, 5 bits Blue

// Basic Colors
#define EARS_RGB565_BLACK       0x0000  //   0,   0,   0
#define EARS_RGB565_WHITE       0xFFFF  // 255, 255, 255
#define EARS_RGB565_RED         0xF800  // 255,   0,   0
#define EARS_RGB565_GREEN       0x07E0  //   0, 255,   0
#define EARS_RGB565_BLUE        0x001F  //   0,   0, 255
#define EARS_RGB565_CYAN        0x07FF  //   0, 255, 255
#define EARS_RGB565_MAGENTA     0xF81F  // 255,   0, 255
#define EARS_RGB565_YELLOW      0xFFE0  // 255, 255,   0

// Grays
#define EARS_RGB565_DARKGRAY    0x39E7  //  64,  64,  64
#define EARS_RGB565_GRAY        0x7BEF  // 128, 128, 128
#define EARS_RGB565_LIGHTGRAY   0xC618  // 192, 192, 192

// Additional Colors
#define EARS_RGB565_ORANGE      0xFD20  // 255, 165,   0
#define EARS_RGB565_PURPLE      0x780F  // 128,   0, 128
#define EARS_RGB565_BROWN       0xA145  // 165,  42,  42
#define EARS_RGB565_PINK        0xFE19  // 255, 192, 203
#define EARS_RGB565_NAVY        0x000F  //   0,   0, 128
#define EARS_RGB565_MAROON      0x7800  // 128,   0,   0
#define EARS_RGB565_OLIVE       0x7BE0  // 128, 128,   0

// Camouflage Swatch

#define EARS_RGB565_AS_BROWN1	0x6227  //  96,  68,  57
#define EARS_RGB565_AS_TAN		0x9CCE  // 158, 154, 117
#define EARS_RGB565_AS_BLACK	0x1906  //  28,  34,  46
#define EARS_RGB565_AS_GREEN	0x42A7  //  65,  83,  59
#define EARS_RGB565_AS_BROWN2	0x5248  //  85,  72,  64

// Utility macro to convert RGB888 to EARS_RGB565
#define RGB888_TO_EARS_RGB565(r, g, b) \
    ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | (((b) & 0xF8) >> 3))

#endif // __RGB555_COLOURS_H_