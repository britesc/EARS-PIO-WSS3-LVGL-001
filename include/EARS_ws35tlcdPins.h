/**
 * @file EARS_ws35tlcdPins.h
 * @author Julian (51fiftyone51fiftyone@gmail.com)
 * @brief Defines pin assignments for the Waveshare 3.5" ESP32-S3 LCD display.
 * @details This file is for the Arduino GFX Library.
 * @version 0.1
 * @date 20251229
 */
#pragma once
#ifndef __EARS_WS35TLCD_PINS_H_
#define __EARS_WS35TLCD_PINS_H_

// Waveshare 3.5" ESP32-S3 Display Pin Definitions (Arduino GFX)

// TFT Display Pins (SPI)
#define GFX_BL     6     // Backlight
#define SPI_MISO   2
#define SPI_MOSI   1
#define SPI_SCLK   5
#define LCD_CS    -1     // Chip Select (not used)
#define LCD_DC     3     // Data/Command
#define LCD_RST   -1     // Reset (not used)

// SD Card Pins (SPI)
#define SD_CS      46
#define SD_MOSI    7
#define SD_MISO    8
#define SD_SCK     6

// Touch Screen Pins (I2C) - if needed later
#define TOUCH_SDA  38
#define TOUCH_SCL  39
#define TOUCH_INT  18
#define TOUCH_RST  -1

// Display Specifications
#define TFT_WIDTH  480
#define TFT_HEIGHT 320

#endif // __EARS_WS35TLCD_PINS_H_