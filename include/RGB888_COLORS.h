/**
 * @file EARS_RGB888_COLORS.h
 * @author Julian (51fiftyone51fiftyone@gmail.com)
 * @brief Define the Colours for EARS_RGB888 format
 * @version 0.1
 * @date 20251229
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#ifndef __RGB888_COLOURS_H_
#define __RGB888_COLOURS_H_

#include <stdint.h>

// EARS_RGB888 Color Definitions
// Format: 6 bits Red, 6 bits Green, 6 bits Blue

// Basic Colors
#define EARS_RGB888_BLACK       0x000000  //   0,   0,   0
#define EARS_RGB888_WHITE       0xFFFFFF  // 255, 255, 255
#define EARS_RGB888_RED         0xFF0000  // 255,   0,   0
#define EARS_RGB888_GREEN       0x00FF00  //   0, 255,   0
#define EARS_RGB888_BLUE        0x0000FF  //   0,   0, 255
#define EARS_RGB888_CYAN        0x00FFFF  //   0, 255, 255
#define EARS_RGB888_MAGENTA     0xFF00FF  // 255,   0, 255
#define EARS_RGB888_YELLOW      0xFFFF00  // 255, 255,   0

// Grays
#define EARS_RGB888_DARKGRAY    0xA9A9A9  // 169, 169, 169
#define EARS_RGB888_GRAY        0x808080  // 128, 128, 128
#define EARS_RGB888_LIGHTGRAY   0xD3D3D3  // 211, 211, 211

// Additional Colors
#define EARS_RGB888_ORANGE      0xFFA500  // 255, 165,   0
#define EARS_RGB888_PURPLE      0x800080  // 128,   0, 128
#define EARS_RGB888_BROWN       0xA52A2A  // 165,  42,  42
#define EARS_RGB888_PINK        0xFEC0CB  // 255, 192, 203
#define EARS_RGB888_NAVY        0x000080  //   0,   0, 128
#define EARS_RGB888_MAROON      0x800000  // 128,   0,   0
#define EARS_RGB888_OLIVE       0x808000  // 128, 128,   0

// Camouflage Swatch

#define EARS_RGB888_AS_BROWN1	0x604439  //  96,  68,  57
#define EARS_RGB888_AS_TAN		0x9E9A75  // 158, 154, 117
#define EARS_RGB888_AS_BLACK	0x1C222E  //  28,  34,  46
#define EARS_RGB888_AS_GREEN	0x41533B  //  65,  83,  59
#define EARS_RGB888_AS_BROWN2	0x554840  //  85,  72,  64

#endif // __RGB888_COLOURS_H_