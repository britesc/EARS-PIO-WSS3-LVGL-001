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
// Format: 5 bits Red, 6 bits Green, 5 bits Blue

// Basic Colors
#define EARS_RGB888_BLACK       #000000  //   0,   0,   0
#define EARS_RGB888_WHITE       #FFFFFF  // 255, 255, 255
#define EARS_RGB888_RED         #FF0000  // 255,   0,   0
#define EARS_RGB888_GREEN       #00FF00  //   0, 255,   0
#define EARS_RGB888_BLUE        #0000FF  //   0,   0, 255
#define EARS_RGB888_CYAN        #00FFFF  //   0, 255, 255
#define EARS_RGB888_MAGENTA     #FF00FF  // 255,   0, 255
#define EARS_RGB888_YELLOW      #FFFF00  // 255, 255,   0

// Grays
#define EARS_RGB888_DARKGRAY    #A9A9A9  // 169, 169, 169
#define EARS_RGB888_GRAY        #808080  // 128, 128, 128
#define EARS_RGB888_LIGHTGRAY   #D3D3D3  // 211, 211, 211

// Additional Colors
#define EARS_RGB888_ORANGE      #FFA500  // 255, 165,   0
#define EARS_RGB888_PURPLE      #800080  // 128,   0, 128
#define EARS_RGB888_BROWN       #A52A2A  // 165,  42,  42
#define EARS_RGB888_PINK        #FEC0CB  // 255, 192, 203
#define EARS_RGB888_NAVY        #000080  //   0,   0, 128
#define EARS_RGB888_MAROON      #800000  // 128,   0,   0
#define EARS_RGB888_OLIVE       #808000  // 128, 128,   0

// Camouflage Swatch

#define EARS_RGB888_AS_BROWN1	#604439  //  96,  68,  57
#define EARS_RGB888_AS_TAN		#9E9A75  // 158, 154, 117
#define EARS_RGB888_AS_BLACK	#1C222E  //  28,  34,  46
#define EARS_RGB888_AS_GREEN	#41533B  //  65,  83,  59
#define EARS_RGB888_AS_BROWN2	#554840  //  85,  72,  64

#endif // __RGB888_COLOURS_H_