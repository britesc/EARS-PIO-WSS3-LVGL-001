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
 #include "ears_version.h"

 /******************************************************************************
  * Includes Information
  *****************************************************************************/
#include <Arduino.h>
#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include "WS35TLCD_PINS.h"
#include "RGB565_COLORS.h"
#include "RGB888_COLORS.h"
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
Arduino_GFX *gfx = new Arduino_ILI9488_18bit(bus, LCD_RST, 1 /* rotation */, true /* IPS */);

/******************************************************************************
 * LVGL display object
 *****************************************************************************/
lv_display_t *disp;

/******************************************************************************
 * NVS EEPROM object
 *****************************************************************************/
// Global validation result - shared between cores
NVSValidationResult g_nvsResult;

// Global NVS instance
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
 * @brief Callback function to provide millis() to LVGL
 * @description
 * LVGL requires a millisecond tick source to manage its timing.
 * @return uint32_t 
 */
uint32_t millis_cb(void) {
    return millis();
}

/**
 * @brief Initialize Serial for Debugging.
 * @description
 * Sets up Serial communication at the defined baud rate and includes a delay to ensure readiness.
 */
void init_serial() {
      Serial.flush();
      Serial.begin(115200);
      int x = 0;
      while (x < 1500) {
        x++;
        if (Serial.available()) {
          x = 1500 + 1;
        }
      }
}

/**
 * @brief Core1 NVS Validation Task
 * 
 * @param parameter 
 */
void core1_nvsValidationTask(void* parameter) {
    // Step 1: Initialize NVS
    if (!nvs.begin()) {
        g_nvsResult.status = NVSStatus::INITIALIZATION_FAILED;
        vTaskDelete(NULL);
        return;
    }
    
    // Step 2: Validate entire NVS
    g_nvsResult = nvs.validateNVS();
    
    // Step 3: Report results
    Serial.println("=== Core1 NVS Validation Results ===");
    Serial.print("Status: ");
    switch (g_nvsResult.status) {
        case NVSStatus::VALID:
            Serial.println("VALID");
            break;
        case NVSStatus::UPGRADED:
            Serial.println("UPGRADED");
            break;
        case NVSStatus::INVALID_VERSION:
            Serial.println("INVALID_VERSION");
            break;
        case NVSStatus::MISSING_ZAPNUMBER:
            Serial.println("MISSING_ZAPNUMBER");
            break;
        case NVSStatus::MISSING_PASSWORD:
            Serial.println("MISSING_PASSWORD");
            break;
        case NVSStatus::CRC_FAILED:
            Serial.println("CRC_FAILED - TAMPERING DETECTED!");
            break;
        case NVSStatus::INITIALIZATION_FAILED:
            Serial.println("INITIALIZATION_FAILED");
            break;
        default:
            Serial.println("NOT_CHECKED");
    }
    
    Serial.printf("Version: Current=%d, Expected=%d\n", 
                  g_nvsResult.currentVersion, 
                  g_nvsResult.expectedVersion);
    Serial.printf("ZapNumber: Valid=%d, Value=%s\n", 
                  g_nvsResult.zapNumberValid, 
                  g_nvsResult.zapNumber);
    Serial.printf("Password: Valid=%d\n", g_nvsResult.passwordHashValid);
    Serial.printf("CRC: Valid=%d, Value=0x%08X\n", 
                  g_nvsResult.crcValid, 
                  g_nvsResult.calculatedCRC);
    Serial.printf("Upgraded: %d\n", g_nvsResult.wasUpgraded);
    Serial.println("====================================");
    
    // Core1 task complete
    vTaskDelete(NULL);
}

/**
 * @brief Core0 Loader Logic Decision
 * 
 */
void core0_loaderLogic() {
  Serial.println("=== Core0 Loader Decision ===");
  
  // Wait for Core1 to finish validation (in real code, use proper synchronization)
  while (g_nvsResult.status == NVSStatus::NOT_CHECKED) {
      delay(10);
  }
  
  // Make decisions based on validation results
  switch (g_nvsResult.status) {
      case NVSStatus::VALID:
      case NVSStatus::UPGRADED:
          Serial.println("Decision: Proceed to login screen");
          Serial.printf("ZapNumber: %s\n", g_nvsResult.zapNumber);
          // TODO: Show login screen, validate password
          break;
          
      case NVSStatus::MISSING_ZAPNUMBER:
          Serial.println("Decision: Show ZapNumber setup wizard");
          // TODO: Launch setup wizard to collect ZapNumber
          break;
          
      case NVSStatus::MISSING_PASSWORD:
          Serial.println("Decision: Show password setup wizard");
          Serial.printf("Using ZapNumber: %s\n", g_nvsResult.zapNumber);
          // TODO: Launch password setup wizard
          break;
          
      case NVSStatus::CRC_FAILED:
          Serial.println("Decision: SECURITY ALERT - Data tampering detected!");
          Serial.println("Action: Factory reset required");
          // TODO: Show security warning, require factory reset
          break;
          
      case NVSStatus::INVALID_VERSION:
          Serial.println("Decision: Version mismatch");
          Serial.printf("NVS version %d incompatible with code version %d\n",
                       g_nvsResult.currentVersion,
                       g_nvsResult.expectedVersion);
          // TODO: Show error, offer factory reset
          break;
          
      case NVSStatus::INITIALIZATION_FAILED:
          Serial.println("Decision: Hardware error");
          // TODO: Show hardware error message
          break;
          
      default:
          Serial.println("Decision: Unknown state");
          break;
  }
  Serial.println("=============================");
}



/**
 * @brief Primary setup functions for Core0
 * @description
 * Initializes Serial, Display, LVGL, and NVS.
 * @return void
 */
void setup() {
    init_serial();
    
    Serial.println("LVGL Trial Starting...");

    // Initialize backlight
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);

    // Initialize display
    gfx->begin();
    gfx->fillScreen(EARS_RGB565_BLACK);
    
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
    lv_obj_set_style_bg_color(scr, lv_color_hex(EARS_RGB888_BLACK), 0);

    // Create a label
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label, "LVGL Trial\nWorking!");
    lv_obj_set_style_text_color(label, lv_color_hex(EARS_RGB888_WHITE), 0);
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
  
    // Step 2 Test: Hash generation and comparison
    String testData = "Hello EARS!";
    String hash = nvs.makeHash(testData);
    Serial.print("Generated hash: ");
    Serial.println(hash);
    
    // Store the hash
    nvs.putHash("test_hash", hash);
    
    // Retrieve and compare
    String retrievedHash = nvs.getHash("test_hash");
    if (nvs.compareHash(testData, retrievedHash)) {
      Serial.println("Hash verification SUCCESS!");
    } else {
      Serial.println("Hash verification FAILED!");
    }


}

/**
 * @brief Core0 loop function
 * @desription
 * Handles LVGL timer tasks.
 * @return void
 */
void loop() {
    lv_timer_handler();
    delay(5);
}

/**
 * @brief Primary setup functions for Core1
 * @description
 * Manages functions such as NVSEeprom.
 * @return void

 */
void setup1() {
}

/**
 * @brief Core1 loop function
 * @desription
 * Handles Functional tasks, such as the Eeprom.
 * @return void
 */
void loop1() {
}


/******************************************************************************
 * End of File
 *****************************************************************************/