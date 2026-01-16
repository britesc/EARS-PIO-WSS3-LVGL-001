/**
 * @file main.cpp - UPDATED with standardized EARS library pattern
 *
 * @mainpage EARS Project
 * 
 * @version file EARS_versionDef.h
 *
 * @section description Description
 * EARS - Equipment and Ammunition Reporting System.
 * Dual-Core ESP32-S3 Implementation:
 * - Core 0: NVS Validation and background tasks
 * - Core 1: LVGL display, animation, and UI
 *
 * All libraries now use the standardized using_xxxxx() pattern
 *
 * @section author Author
 * - Created by JTB on 20251220.
 * - Updated for standardized library pattern on 20260114.
 *
 * Copyright (c) 2025 JTB.  All rights reserved.
 */

 /******************************************************************************
  * App Version Information
  *****************************************************************************/
 #include "EARS_versionDef.h"

 /******************************************************************************
  * Includes Information
  *****************************************************************************/
#include <Arduino.h>
#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include "EARS_ws35tlcdPins.h"
#include "EARS_rgb565ColoursDef.h"
#include "EARS_rgb888ColoursDef.h"
#include "EARS_nvsEepromLib.h"
#include "EARS_loggerLib.h"
#include "EARS_sdCardLib.h"
#include "EARS_screenSaverLib.h"
#include "EARS_errorsLib.h"
#include "EARS_backLightManagerLib.h"


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
 * LVGL Animation objects
 *****************************************************************************/
lv_obj_t *spinner = NULL;
lv_obj_t *status_label = NULL;

/******************************************************************************
 * NVS EEPROM - Global validation result (shared between cores)
 *****************************************************************************/
NVSValidationResult g_nvsResult;

/******************************************************************************
 * NO MORE GLOBAL LIBRARY INSTANCES!
 * All libraries are now accessed via using_xxxxx() functions
 * This eliminates global declarations and makes code cleaner
 *****************************************************************************/

/******************************************************************************
 * Task Handles for Core Management
 *****************************************************************************/
TaskHandle_t Core0_ValidationTask;
TaskHandle_t Core1_DisplayTask;

/******************************************************************************
 * Synchronization Variables
 *****************************************************************************/
volatile bool validationComplete = false;
SemaphoreHandle_t displayMutex;

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
 * @details
 * LVGL requires a millisecond tick source to manage its timing.
 * @return uint32_t 
 */
uint32_t millis_cb(void) {
    return millis();
}

/**
 * @brief Initialize EARS_logger
 */
void init_logger() {
    // NEW: Use using_logger() instead of getInstance()
    // NEW: Pass reference to SD card via using_sdcard()
    using_logger().begin("/logs/debug.log", "/config/ears.config", &using_sdcard());
    
    // NEW: All log calls now use using_logger()
    using_logger().info("Setup started.");
    using_logger().infof("Free memory: %d bytes", ESP.getFreeHeap());
}

/**
 * @brief Update status message on display
 * @param message Status message to display
 */
void updateStatus(const char* message) {
    if (xSemaphoreTake(displayMutex, portMAX_DELAY)) {
        if (status_label != NULL) {
            lv_label_set_text(status_label, message);
        }
        xSemaphoreGive(displayMutex);
    }
}

/**
 * @brief Initialize Errors
 */
void init_errors() {
    SD.begin();
    
    // NEW: Use using_errors() instead of global instance    
    using_errors().begin();
    
    using_logger().info("Errors system: Initialized");
}

/**
 * @brief Core 0 Task: NVS Validation
 * @details
 * Runs on Core 0, performs NVS validation while Core 1 shows animation
 * @param parameter Task parameters (unused)
 */
void Core0_NVSValidation(void* parameter) {
    using_logger().info("[Core 0] NVS Validation Task Started");
    
    // Simulate some validation steps with status updates
    updateStatus("Checking NVS...");
    delay(500);
    
    // Step 1: Initialize NVS
    // NEW: Use using_nvseeprom() instead of global instance
    updateStatus("Initializing NVS...");
    if (!using_nvseeprom().begin()) {
        g_nvsResult.status = NVSStatus::INITIALIZATION_FAILED;
        updateStatus("NVS Init Failed!");
        validationComplete = true;
        vTaskDelete(NULL);
        return;
    }
    using_logger().info("[Core 0] NVS Initialized");
    delay(500);
    
    // Step 2: Validate entire NVS
    // NEW: Use using_nvseeprom() for validation
    updateStatus("Validating data...");
    g_nvsResult = using_nvseeprom().validateNVS();
    delay(500);
    
    // Step 3: Report results
    using_logger().info("=== Core 0 NVS Validation Results ===");
    
    switch (g_nvsResult.status) {
        case NVSStatus::VALID:
            using_logger().info("Status: VALID");
            updateStatus("NVS: Valid");
            break;
        case NVSStatus::UPGRADED:
            using_logger().info("Status: UPGRADED");
            updateStatus("NVS: Upgraded");
            break;
        case NVSStatus::MISSING_ZAPNUMBER:
            using_logger().info("Status: MISSING_ZAPNUMBER");
            updateStatus("NVS: Missing ZapNumber");
            break;
        case NVSStatus::MISSING_PASSWORD:
            using_logger().info("Status: MISSING_PASSWORD");
            updateStatus("NVS: Missing Password");
            break;
        case NVSStatus::CRC_FAILED:
            using_logger().error("Status: CRC_FAILED - TAMPERING DETECTED!");
            updateStatus("NVS: TAMPERED!");
            break;
        case NVSStatus::INVALID_VERSION:
            using_logger().error("Status: INVALID_VERSION");
            updateStatus("NVS: Invalid Version");
            break;
        case NVSStatus::INITIALIZATION_FAILED:
            using_logger().error("Status: INITIALIZATION_FAILED");
            updateStatus("NVS: Init Failed");
            break;
        default:
            using_logger().error("Status: UNKNOWN");
            updateStatus("NVS: Unknown");
            break;
    }
    
    using_logger().infof("Version: Current=%d, Expected=%d", 
                    g_nvsResult.currentVersion, 
                    g_nvsResult.expectedVersion);
    using_logger().infof("ZapNumber: Valid=%d, Value=%s", 
                    g_nvsResult.zapNumberValid, 
                    g_nvsResult.zapNumber);
    using_logger().infof("Password: Valid=%d", g_nvsResult.passwordHashValid);        
    using_logger().infof("CRC: Valid=%d, Value=0x%08X", 
                  g_nvsResult.crcValid, 
                  g_nvsResult.calculatedCRC);
    using_logger().infof("Upgraded: %d", g_nvsResult.wasUpgraded);
    using_logger().info("====================================");
    
    delay(1000); // Show final status for 1 second
    
    // Mark validation as complete
    validationComplete = true;
    
    using_logger().info("[Core 0] Validation Complete - Task Ending");
    
    // Task complete, delete itself
    vTaskDelete(NULL);
}

/**
 * @brief Core 0 Loader Logic Decision
 * @description
 * Called after validation completes to determine next action
 */
void core0_loaderLogic() {
    using_logger().info("=== Loader Decision ===");
    
    // Make decisions based on validation results
    switch (g_nvsResult.status) {
        case NVSStatus::VALID:
        case NVSStatus::UPGRADED:
            using_logger().info("Decision: Proceed to login screen");
            using_logger().infof("ZapNumber: %s", g_nvsResult.zapNumber);
            updateStatus("Ready - Login");
            // TODO: Show login screen, validate password
            break;
            
        case NVSStatus::MISSING_ZAPNUMBER:
            using_logger().info("Decision: Show ZapNumber setup wizard");
            updateStatus("Setup: ZapNumber");
            // TODO: Launch setup wizard to collect ZapNumber
            break;
            
        case NVSStatus::MISSING_PASSWORD:
            using_logger().info("Decision: Show password setup wizard");
            using_logger().infof("Using ZapNumber: %s", g_nvsResult.zapNumber);
            updateStatus("Setup: Password");
            // TODO: Launch password setup wizard
            break;
            
        case NVSStatus::CRC_FAILED:
            using_logger().error("Decision: SECURITY ALERT - Data tampering detected!");
            using_logger().info("Action: Factory reset required");
            updateStatus("Factory Reset Needed");
            // TODO: Show security warning, require factory reset
            break;
            
        case NVSStatus::INVALID_VERSION:
            using_logger().info("Decision: Version mismatch");
            using_logger().infof("NVS version %d incompatible with code version %d",
                         g_nvsResult.currentVersion,
                         g_nvsResult.expectedVersion);
            updateStatus("Version Mismatch");
            // TODO: Show error, offer factory reset
            break;
            
        case NVSStatus::INITIALIZATION_FAILED:
            using_logger().error("Decision: Hardware error");
            updateStatus("Hardware Error");
            // TODO: Show hardware error message
            break;
            
        default:
            using_logger().error("Decision: Unknown state");
            updateStatus("Unknown State");
            break;
    }
    using_logger().info("========================");
}

/**
 * @brief Core 1 Task: LVGL Display Handler
 * @details
 * Runs on Core 1, handles LVGL timer and display updates
 * 
 * @param parameter Task parameters (unused)
 */
void Core1_DisplayHandler(void* parameter) {
    using_logger().info("[Core 1] Display Handler Task Started");
    
    while (true) {
        // Handle LVGL timer
        if (xSemaphoreTake(displayMutex, portMAX_DELAY)) {
            lv_timer_handler();
            xSemaphoreGive(displayMutex);
        }
        
        // Check if validation is complete
        if (validationComplete) {
            // Hide spinner
            if (spinner != NULL) {
                lv_obj_del(spinner);
                spinner = NULL;
            }
            
            // Run loader logic
            core0_loaderLogic();
            
            // End this task as validation phase is done
            using_logger().info("[Core 1] Display task ending - validation complete");
            vTaskDelete(NULL);
            return;
        }
        
        delay(5);
    }
}

/**
 * @brief Primary setup function
 * @details
 * Initializes hardware and creates tasks for both cores
 * Runs on Core 1 by default
 * 
 * @return void
 */
void setup() {
    // Initialize Serial for debugging (keep for emergency fallback)
    Serial.begin(115200);
    delay(1000);
    
    // NEW: Initialize SD card first using using_sdcard()
    if (!using_sdcard().begin()) {
        Serial.println("FATAL: SD Card initialization failed!");
        while(1) delay(1000);
    }
    
    // Initialize Errors system
    init_errors();
    
    // Initialize Logger
    init_logger();
    
    using_logger().info("\n\n=== EARS Dual-Core System Starting ===");
    using_logger().infof("Setup running on Core: %d", xPortGetCoreID());
    
    // Create mutex for display synchronization
    displayMutex = xSemaphoreCreateMutex();
    
    // Step 1: Initialize backlight using new pattern
    // NEW: Use using_backlightmanager() instead of global instance
    if (using_backlightmanager().begin(GFX_BL, 0)) {
        using_logger().info("Backlight: Initialized");
    }
    
    // Step 2: Initialize display
    using_logger().info("Initializing display...");
    gfx->begin();
    gfx->fillScreen(EARS_RGB565_BLACK);
    using_logger().info("Display: Initialized");
    
    // Step 3: Initialize LVGL
    using_logger().info("Initializing LVGL...");
    lv_init();
    lv_tick_set_cb(millis_cb);
     
    disp = lv_display_create(screenWidth, screenHeight);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
    using_logger().info("LVGL: Initialized");
    
    // Step 4: Initialize screensaver using new pattern
    // NEW: Use using_screensaver() instead of global instance
    using_screensaver().begin(disp);
    using_screensaver().setTimeout(30);
    using_logger().info("Screensaver: Initialized");
    
    // Step 5: Create loading screen with spinner
    using_logger().info("Creating loading screen...");
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(EARS_RGB888_BLACK), 0);
    
    // Create spinner (loading animation)
    spinner = lv_spinner_create(scr);
    lv_obj_set_size(spinner, 80, 80);
    lv_obj_center(spinner);
    lv_obj_set_style_arc_color(spinner, lv_color_hex(0x00FF00), LV_PART_INDICATOR);
    
    // Create status label below spinner
    status_label = lv_label_create(scr);
    lv_label_set_text(status_label, "Starting...");
    lv_obj_set_style_text_color(status_label, lv_color_hex(EARS_RGB888_WHITE), 0);
    lv_obj_set_style_text_font(status_label, &lv_font_montserrat_16, 0);
    lv_obj_align(status_label, LV_ALIGN_CENTER, 0, 60);
    
    using_logger().info("Loading screen: Created");
    
    // Step 6: Create Core 0 task for NVS validation
    using_logger().info("Creating Core 0 validation task...");
    xTaskCreatePinnedToCore(
        Core0_NVSValidation,      // Task function
        "NVS_Validation",         // Name
        4096,                     // Stack size (bytes)
        NULL,                     // Parameters
        1,                        // Priority (1 = low, higher = more priority)
        &Core0_ValidationTask,    // Task handle
        0                         // Core 0
    );
    
    // Step 7: Create Core 1 task for display handling
    using_logger().info("Creating Core 1 display task...");
    xTaskCreatePinnedToCore(
        Core1_DisplayHandler,     // Task function
        "Display_Handler",        // Name
        8192,                     // Stack size (bytes) - larger for LVGL
        NULL,                     // Parameters
        2,                        // Priority (higher than validation)
        &Core1_DisplayTask,       // Task handle
        1                         // Core 1
    );
    
    using_logger().info("=== Setup Complete ===");
    using_logger().info("Core 0: Running NVS validation");
    using_logger().info("Core 1: Running display animation\n");
}

/**
 * @brief Main loop function
 * @details
 * During development, this loop monitors system status.
 * @return void
 */
void loop() {
    // Development monitoring
    static uint32_t lastPrint = 0;
    if (millis() - lastPrint > 5000) {
        using_logger().infof("[Monitor] Free heap: %d bytes", ESP.getFreeHeap());
        using_logger().infof("[Monitor] Running on Core: %d", xPortGetCoreID());
        lastPrint = millis();
    }
    
    // NEW: Update screensaver using new pattern
    using_screensaver().update();
    
    delay(1000);
}

/******************************************************************************
 * End of File main.cpp
 *****************************************************************************/