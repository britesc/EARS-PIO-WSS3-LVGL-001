/*
 * Library Compilation Test Template
 * 
 * Instructions:
 * 1. Uncomment ONE library include at a time
 * 2. Uncomment the matching using_library line in setup()
 * 3. Compile and verify
 * 4. Move to next library
 */

#include <Arduino.h>

// === STEP 1: Uncomment ONE library at a time ===
// #include "YourLibLib.h"

// === STEP 2: Create the library instance ===
// Example: YourLibLib using_yourliblib;


void setup() {
    // Initialize serial for debug output
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("=== Library Test Started ===");
    
    // === STEP 3: Initialize the library ===
    // using_yourliblib.begin();
    
    Serial.println("Library initialized successfully!");
    Serial.println("Test PASSED");
    Serial.println("=== Library Test Finished ===");    
}

void loop() {
    // Empty loop - just testing compilation and initialization
    delay(1000);
}