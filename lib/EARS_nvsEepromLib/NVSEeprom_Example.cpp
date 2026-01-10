/**
 * @file NVSEeprom_Example.cpp
 * @brief Example showing how Core0 and Core1 use NVSEeprom
 * 
 * USAGE PATTERN:
 * - Core1: Validates NVS and populates result struct
 * - Core0: Reads result struct and decides what to do
 */

#include "NVSEeprom.h"

// Global validation result - shared between cores
NVSValidationResult g_nvsResult;

// Global NVS instance
NVSEeprom nvs;

/**
 * CORE 1 TASK - Validation and Setup
 * This runs on Core1 and validates the NVS
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
 * CORE 0 - Main Loader Logic
 * This runs on Core0 and decides what to do based on validation results
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
 * EXAMPLE: First-time setup
 */
void example_firstTimeSetup() {
    Serial.println("\n=== Example: First Time Setup ===");
    
    // Initialize NVS
    nvs.begin();
    
    // Set ZapNumber (from user input)
    String newZapNumber = "AB1234";
    if (nvs.isValidZapNumber(newZapNumber)) {
        nvs.putString(NVSEeprom::KEY_ZAPNUMBER, newZapNumber);
        Serial.println("ZapNumber saved: " + newZapNumber);
    }
    
    // Set password hash (from user input)
    String password = "MySecurePassword123";
    String passwordHash = nvs.makeHash(password);
    nvs.putHash(NVSEeprom::KEY_PASSWORD_HASH, passwordHash);
    Serial.println("Password hash saved: " + passwordHash);
    
    // Set version
    nvs.putVersion(NVSEeprom::KEY_VERSION, NVSEeprom::CURRENT_VERSION);
    Serial.printf("Version saved: %d\n", NVSEeprom::CURRENT_VERSION);
    
    // Calculate and save CRC
    nvs.updateNVSCRC();
    Serial.println("CRC updated");
    
    Serial.println("Setup complete!\n");
}

/**
 * EXAMPLE: Login validation
 */
void example_validateLogin(const String& enteredPassword) {
    Serial.println("\n=== Example: Login Validation ===");
    
    // Get stored password hash
    String storedHash = nvs.getHash(NVSEeprom::KEY_PASSWORD_HASH);
    
    // Compare entered password with stored hash
    if (nvs.compareHash(enteredPassword, storedHash)) {
        Serial.println("Login SUCCESS!");
    } else {
        Serial.println("Login FAILED - incorrect password");
    }
    Serial.println("");
}

/**
 * Arduino setup() - runs on Core1 by default
 */
void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n\n=== NVSEeprom Example ===\n");
    
    // OPTION A: Create Core1 task explicitly
    xTaskCreatePinnedToCore(
        core1_nvsValidationTask,  // Task function
        "NVS_Validation",          // Task name
        4096,                      // Stack size
        NULL,                      // Parameters
        1,                         // Priority
        NULL,                      // Task handle
        1                          // Core 1
    );
    
    // OPTION B: Run validation directly (if not using tasks)
    // g_nvsResult = nvs.validateNVS();
}

/**
 * Arduino loop() - runs on Core1 by default
 * In real application, Core0 would run the loader
 */
void loop() {
    static bool hasRun = false;
    
    if (!hasRun) {
        delay(500); // Wait for Core1 validation to complete
        
        // Core0 logic
        core0_loaderLogic();
        
        // Example usage
        example_firstTimeSetup();
        example_validateLogin("MySecurePassword123");
        example_validateLogin("WrongPassword");
        
        hasRun = true;
    }
    
    delay(1000);
}
