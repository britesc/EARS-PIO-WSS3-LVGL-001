/*
 * Library Compilation Test Template
 * 
 * Instructions:
 * 1. Uncomment ONE library include at a time
 * 2. Uncomment the matching using_library line in setup()
 * 3. Compile and verify
 * 4. Move to next library
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

/******************************************************************************
 * Includes Information for EARS Libraries
 *****************************************************************************/
#include "EARS_nvsEepromLib.h"
#include "EARS_loggerLib.h"
#include "EARS_sdCardLib.h"
#include "EARS_screenSaverLib.h"
#include "EARS_errorsLib.h"
#include "EARS_backLightManagerLib.h"


// === STEP 1: Uncomment ONE library at a time ===
// #include "YourLibLib.h"

// === STEP 2: Create the library instance ===
// Example: YourLibLib using_yourliblib;
EARS_nvsEeprom using_nvseeprom;
// EARS_logger using_logger; Do not create instance - use singleton pattern


void setup() {
    // Initialize serial for debug output
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("=== Library Test Started ===");
    
    // === STEP 3: Initialize the library ===
    using_nvseeprom.begin();
    EARS_logger::getInstance().begin("/logs/debug.log", "/config/ears.config", nullptr);
    

    
    Serial.println("Library initialized successfully!");
    Serial.println("Test PASSED");
    Serial.println("=== Library Test Finished ===");    
}

void loop() {
    // Empty loop - just testing compilation and initialization
    delay(1000);
}