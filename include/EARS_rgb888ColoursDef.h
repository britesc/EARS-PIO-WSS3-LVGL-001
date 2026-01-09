/**
 * @file EARS_rgb888ColoursDef.h
 * @author Julian (51fiftyone51fiftyone@gmail.com)
 * @brief Define the Colours for EARS_RGB888 format
 * @version 0.5.0
 * @date 20251229
 * @updated 20260109
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#ifndef __EARS_RGB888_COLOURS_DEF_H__
#define __EARS_RGB888_COLOURS_DEF_H__

#include <stdint.h>

// EARS_RGB888 Color Definitions
// Format: 6 bits Red, 6 bits Green, 6 bits Blue

// Basic Colors
#define EARS_RGB888_BLACK           0x000000  //   0,   0,   0  #000000
#define EARS_RGB888_WHITE           0xFFFFFF  // 255, 255, 255  #FFFFFF
#define EARS_RGB888_RED             0xFF0000  // 255,   0,   0  #FF0000
#define EARS_RGB888_GREEN           0x00FF00  //   0, 255,   0  #00FF00
#define EARS_RGB888_BLUE            0x0000FF  //   0,   0, 255  #0000FF
#define EARS_RGB888_CYAN            0x00FFFF  //   0, 255, 255  #00FFFF
#define EARS_RGB888_MAGENTA         0xFF00FF  // 255,   0, 255  #FF00FF
#define EARS_RGB888_YELLOW          0xFFFF00  // 255, 255,   0  #FFFF00

// Grays
#define EARS_RGB888_DARKGRAY        0xA9A9A9  // 169, 169, 169  #A9A9A9
#define EARS_RGB888_GRAY            0x808080  // 128, 128, 128  #808080
#define EARS_RGB888_LIGHTGRAY       0xD3D3D3  // 211, 211, 211  #D3D3D3

// Additional Colors
#define EARS_RGB888_ORANGE          0xFFA500  // 255, 165,   0  #FFA500
#define EARS_RGB888_PURPLE          0x800080  // 128,   0, 128  #800080
#define EARS_RGB888_BROWN           0xA52A2A  // 165,  42,  42  #A52A2A
#define EARS_RGB888_PINK            0xFEC0CB  // 255, 192, 203  #FEC0CB
#define EARS_RGB888_NAVY            0x000080  //   0,   0, 128  #000080
#define EARS_RGB888_MAROON          0x800000  // 128,   0,   0  #800000
#define EARS_RGB888_OLIVE           0x808000  // 128, 128,   0  #808000

// Define True Black
#define EARS_RGB888_TRUE_BLACK      0x0A0B0B  //  10,  11,  11  #0A0B0B

// Camouflage Swatch
#define EARS_RGB888_CS_BROWN1	    0x604439  //  96,  68,  57  #604439
#define EARS_RGB888_CS_TAN		    0x9E9A75  // 158, 154, 117  #9E9A75
#define EARS_RGB888_CS_BLACK	    EARS_RGB888_TRUE_BLACK
#define EARS_RGB888_CS_GREEN	    0x41533B  //  65,  83,  59  #41533B
#define EARS_RGB888_CS_BROWN2	    0x554840  //  85,  72,  64  #554840
#define EARS_RGB888_CS_PRESSED      EARS_RGB888_CS_BROWN2
#define EARS_RGB888_CS_TEXT         EARS_RGB888_CS_TAN
#define EARS_RGB888_CS_PRIMARY      EARS_RGB888_CS_GREEN
#define EARS_RGB888_CS_SECONDARY    EARS_RGB888_CS_BROWN1


// Red Swatch
#define EARS_RGB888_RS_DARKRED	    0x7A1E1E  // 122,  30,  30   #7A1E1E 
#define EARS_RGB888_RS_PINKRED	    0xFF6B6B  // 255, 107, 107   #FF6B6B
#define EARS_RGB888_RS_BLACK	    EARS_RGB888_TRUE_BLACK
#define EARS_RGB888_RS_LIGHTRED     0xE64545  // 230,  69,  69   #E64545
#define EARS_RGB888_RS_MIDRED       0xB83232  // 184,  50,  50   #B83232
#define EARS_RGB888_RS_PRESSED      EARS_RGB888_RS_DARKRED
#define EARS_RGB888_RS_TEXT         EARS_RGB888_RS_PINKRED
#define EARS_RGB888_RS_PRIMARY      EARS_RGB888_RS_LIGHTRED
#define EARS_RGB888_RS_SECONDARY    EARS_RGB888_RS_MIDRED

#endif // __EARS_RGB888_COLOURS_DEF_H__