/**
 * @file main.cpp
 *
 * @mainpage EARS Project
 *
 * @section description Description
 * EARS - Equipment and Ammunition Reporting  System.
 *
 * @section libraries Libraries
 * - GFX Library for Arduino (https://github.com/moononournation/Arduino_GFX)
 *   - Graphics Gateway.
 * - Widgets Library for Arduino (https://github.com/lvgl/lvgl)
 *   - Widgets
 *
 * @section tools Tools
 * - VSCode 1.81.1.
 * - PlatformIO IDE 3.6.3.
 * - EEZ Studio 0.23.2.
 *
 * @section notes Notes
 * - Comments are Doxygen compatible.
 *
 * @section todo TODO
 * - Don't use Doxygen style formatting inside the body of a function.
 *
 * @section author Author
 * - Created by JTB on 20251220.
 * - Modified by by JTB on 20251228.
 *
 * Copyright (c) 2025 JTB.  All rights reserved.
 */

 /******************************************************************************
  * App Version Information
  *****************************************************************************/
 #define APP_VERSION_MAJOR "4"
 #define APP_VERSION_MINOR "0"
 #define APP_VERSION_PATCH "1"
 #define APP_VERSION_BUILD "(Dev)"
 #define APP_VERSION       APP_VERSION_MAJOR + "." + APP_VERSION_MINOR + "." + APP_VERSION_PATCH + " " + APP_VERSION_BUILD 

 /******************************************************************************
  * Includes Information
  *****************************************************************************/
#include <Arduino.h>
#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include "WS35TLCD_PINS.h"
#include "RGB565_COLORS.h"
#include "NVSEeprom.h"

/******************************************************************************
 * Start of Object Declarations 
 *****************************************************************************/

/******************************************************************************
 * Display settings
 *****************************************************************************/
static const uint32_t screenWidth = TFT_WIDTH;
static const uint32_t screenHeight = TFT_HEIGHT;

/******************************************************************************
 * LVGL draw buffers
 *****************************************************************************/
static lv_color_t buf1[screenWidth * 40];
static lv_color_t buf2[screenWidth * 40];

/******************************************************************************
 * Arduino GFX display object
 *****************************************************************************/
Arduino_DataBus *bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, SPI_SCLK, SPI_MOSI, SPI_MISO);
Arduino_GFX *gfx = new Arduino_ILI9488_18bit(bus, LCD_RST, 0 /* rotation */, false /* IPS */);

/******************************************************************************
 * LVGL display object
 *****************************************************************************/
lv_display_t *disp;

/******************************************************************************
 * NVS EEPROM object
 *****************************************************************************/
NVSEeprom nvs;

/******************************************************************************
 * End of Object Declarations 
 *****************************************************************************/

/**
 * @brief LVGL display flush callback
 * 
 * @param display 
 * @param area 
 * @param px_map 
 */
 void my_disp_flush(lv_display_t *display, const lv_area_t *area, uint8_t *px_map) {
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);

    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);

    lv_display_flush_ready(display);
}

/**
 * @brief Primary setup function
 * @description
 * Initializes Serial, Display, LVGL, and NVS.
 * @return void
 */
void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("LVGL Trial Starting...");

    // Initialize backlight
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);

    // Initialize display
    gfx->begin();
    gfx->fillScreen(RGB565_BLACK);
    
    Serial.println("Display initialized");

    /* Initialise LVGL */
    lv_init();
  
    /*Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb(millis_cb);
     
    // Create LVGL display
    disp = lv_display_create(screenWidth, screenHeight);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    Serial.println("LVGL initialized");

    // Create a simple test screen
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(RGB565_NAVY), 0);

    // Create a label
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label, "LVGL Trial\nWorking!");
    lv_obj_set_style_text_color(label, lv_color_hex(RGB565_WHITE), 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_center(label);

    Serial.println("Test screen created");

    // Initialize NVS partition - ADD THIS
    if (nvs.begin()) {
        Serial.println("NVS initialized");
    } else {
        Serial.println("Failed to initialize NVS");
    }    

    // NVS Test Step 1
    nvs.putHash("test", "abc123");
    String retrieved = nvs.getHash("test");
    Serial.println(retrieved);    
}

/**
 * @brief Core0 loop function
 * @desription
 * Handles LVGL timer tasks.
 */
void loop() {
    lv_timer_handler();
    delay(5);
}

/**
 * @brief Callback function to provide millis() to LVGL
 * @description
 * LVGL requires a millisecond tick source to manage its timing.
 * @return uint32_t 
 */
uint32_t millis_cb(void) {
    return millis();
}
  