/**
 * @file main.cpp
 *
 * @mainpage EARS Project
 *
 * @section description Description
 * EARS - Equipment and Ammunition Reporting System.
 * Dual-Core ESP32-S3 Implementation:
 * - Core 0: NVS Validation and background tasks
 * - Core 1: LVGL display, animation, and UI
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
 * - Modified for proper dual-core operation on 20250104.
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
#include "Logger.h"

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
 * NVS EEPROM object
 *****************************************************************************/
// Global validation result - shared between cores
NVSValidationResult g_nvsResult;

// Global NVS instance
NVSEeprom nvs;

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
 * @description
 * LVGL requires a millisecond tick source to manage its timing.
 * @return uint32_t 
 */
uint32_t millis_cb(void) {
    return millis();
}

/**
 * @brief Initialize Logger
 * 
 */
void init_logger() {
    LOG_INIT("/logs/debug.log");
    LOG("Setup started.");
    LOGF("Free memory: %d bytes", ESP.getFreeHeap());
}

/**
 * @brief Update status message on display
 * 
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
 * @brief Core 0 Task: NVS Validation
 * @description
 * Runs on Core 0, performs NVS validation while Core 1 shows animation
 * 
 * @param parameter Task parameters (unused)
 */
void Core0_NVSValidation(void* parameter) {
    Serial.println("[Core 0] NVS Validation Task Started");
    
    // Simulate some validation steps with status updates
    updateStatus("Checking NVS...");
    delay(500);
    
    // Step 1: Initialize NVS
    updateStatus("Initializing NVS...");
    if (!nvs.begin()) {
        g_nvsResult.status = NVSStatus::INITIALIZATION_FAILED;
        updateStatus("NVS Init Failed!");
        validationComplete = true;
        vTaskDelete(NULL);
        return;
    }
    Serial.println("[Core 0] NVS Initialized");
    delay(500);
    
    // Step 2: Validate entire NVS
    updateStatus("Validating data...");
    g_nvsResult = nvs.validateNVS();
    delay(500);
    
    // Step 3: Report results
    Serial.println("=== Core 0 NVS Validation Results ===");
    Serial.print("Status: ");
    
    switch (g_nvsResult.status) {
        case NVSStatus::VALID:
            Serial.println("VALID");
            updateStatus("Validation: PASSED");
            break;
        case NVSStatus::UPGRADED:
            Serial.println("UPGRADED");
            updateStatus("Validation: UPGRADED");
            break;
        case NVSStatus::INVALID_VERSION:
            Serial.println("INVALID_VERSION");
            updateStatus("Validation: BAD VERSION");
            break;
        case NVSStatus::MISSING_ZAPNUMBER:
            Serial.println("MISSING_ZAPNUMBER");
            updateStatus("Setup Required");
            break;
        case NVSStatus::MISSING_PASSWORD:
            Serial.println("MISSING_PASSWORD");
            updateStatus("Password Required");
            break;
        case NVSStatus::CRC_FAILED:
            Serial.println("CRC_FAILED - TAMPERING DETECTED!");
            updateStatus("SECURITY ALERT!");
            break;
        case NVSStatus::INITIALIZATION_FAILED:
            Serial.println("INITIALIZATION_FAILED");
            updateStatus("Hardware Error");
            break;
        default:
            Serial.println("NOT_CHECKED");
            updateStatus("Unknown Status");
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
    
    delay(1000); // Show final status for 1 second
    
    // Mark validation as complete
    validationComplete = true;
    
    Serial.println("[Core 0] Validation Complete - Task Ending");
    
    // Task complete, delete itself
    vTaskDelete(NULL);
}

/**
 * @brief Core 0 Loader Logic Decision
 * @description
 * Called after validation completes to determine next action
 */
void core0_loaderLogic() {
    Serial.println("=== Loader Decision ===");
    
    // Make decisions based on validation results
    switch (g_nvsResult.status) {
        case NVSStatus::VALID:
        case NVSStatus::UPGRADED:
            Serial.println("Decision: Proceed to login screen");
            Serial.printf("ZapNumber: %s\n", g_nvsResult.zapNumber);
            updateStatus("Ready - Login");
            // TODO: Show login screen, validate password
            break;
            
        case NVSStatus::MISSING_ZAPNUMBER:
            Serial.println("Decision: Show ZapNumber setup wizard");
            updateStatus("Setup: ZapNumber");
            // TODO: Launch setup wizard to collect ZapNumber
            break;
            
        case NVSStatus::MISSING_PASSWORD:
            Serial.println("Decision: Show password setup wizard");
            Serial.printf("Using ZapNumber: %s\n", g_nvsResult.zapNumber);
            updateStatus("Setup: Password");
            // TODO: Launch password setup wizard
            break;
            
        case NVSStatus::CRC_FAILED:
            Serial.println("Decision: SECURITY ALERT - Data tampering detected!");
            Serial.println("Action: Factory reset required");
            updateStatus("Factory Reset Needed");
            // TODO: Show security warning, require factory reset
            break;
            
        case NVSStatus::INVALID_VERSION:
            Serial.println("Decision: Version mismatch");
            Serial.printf("NVS version %d incompatible with code version %d\n",
                         g_nvsResult.currentVersion,
                         g_nvsResult.expectedVersion);
            updateStatus("Version Mismatch");
            // TODO: Show error, offer factory reset
            break;
            
        case NVSStatus::INITIALIZATION_FAILED:
            Serial.println("Decision: Hardware error");
            updateStatus("Hardware Error");
            // TODO: Show hardware error message
            break;
            
        default:
            Serial.println("Decision: Unknown state");
            updateStatus("Unknown State");
            break;
    }
    Serial.println("========================");
}

/**
 * @brief Core 1 Task: LVGL Display Handler
 * @description
 * Runs on Core 1, handles LVGL timer and display updates
 * 
 * @param parameter Task parameters (unused)
 */
void Core1_DisplayHandler(void* parameter) {
    Serial.println("[Core 1] Display Handler Task Started");
    
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
            Serial.println("[Core 1] Display task ending - validation complete");
            vTaskDelete(NULL);
            return;
        }
        
        delay(5);
    }
}

/**
 * @brief Primary setup function
 * @description
 * Initializes hardware and creates tasks for both cores
 * Runs on Core 1 by default
 * 
 * @return void
 */
void setup() {
    // Initialize Serial for debugging
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n\n=== EARS Dual-Core System Starting ===");
    Serial.printf("Setup running on Core: %d\n", xPortGetCoreID());
    
    // Create mutex for display synchronization
    displayMutex = xSemaphoreCreateMutex();
    
    // Step 1: Initialize backlight
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);
    Serial.println("Backlight: ON");
    
    // Step 2: Initialize display
    Serial.println("Initializing display...");
    gfx->begin();
    gfx->fillScreen(EARS_RGB565_BLACK);
    Serial.println("Display: Initialized");
    
    // Step 3: Initialize LVGL
    Serial.println("Initializing LVGL...");
    lv_init();
    lv_tick_set_cb(millis_cb);
     
    disp = lv_display_create(screenWidth, screenHeight);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
    Serial.println("LVGL: Initialized");
    
    // Step 4: Create loading screen with spinner
    Serial.println("Creating loading screen...");
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
    
    Serial.println("Loading screen: Created");
    
    // Step 5: Create Core 0 task for NVS validation
    Serial.println("Creating Core 0 validation task...");
    xTaskCreatePinnedToCore(
        Core0_NVSValidation,      // Task function
        "NVS_Validation",         // Name
        4096,                     // Stack size (bytes)
        NULL,                     // Parameters
        1,                        // Priority (1 = low, higher = more priority)
        &Core0_ValidationTask,    // Task handle
        0                         // Core 0
    );
    
    // Step 6: Create Core 1 task for display handling
    Serial.println("Creating Core 1 display task...");
    xTaskCreatePinnedToCore(
        Core1_DisplayHandler,     // Task function
        "Display_Handler",        // Name
        8192,                     // Stack size (bytes) - larger for LVGL
        NULL,                     // Parameters
        2,                        // Priority (higher than validation)
        &Core1_DisplayTask,       // Task handle
        1                         // Core 1
    );
    
    Serial.println("=== Setup Complete ===");
    Serial.println("Core 0: Running NVS validation");
    Serial.println("Core 1: Running display animation\n");
}

/**
 * @brief Main loop function
 * @description
 * During development, this loop monitors system status.
 * @return void
 */
void loop() {
    // Development monitoring
    static uint32_t lastPrint = 0;
    if (millis() - lastPrint > 5000) {
        Serial.printf("[Monitor] Free heap: %d bytes\n", ESP.getFreeHeap());
        Serial.printf("[Monitor] Running on Core: %d\n", xPortGetCoreID());
        lastPrint = millis();
    }
    delay(1000);
}

/******************************************************************************
 * End of File
 *****************************************************************************/
