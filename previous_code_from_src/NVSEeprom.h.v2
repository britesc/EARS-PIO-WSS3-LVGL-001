/**
 * @file NVSEeprom.h
 * @author Julian (51fiftyone51fiftyone@gmail.com)
 * @brief NVS EEPROM wrapper class header
 * @version 0.2
 * @date 20251229
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once
#ifndef __NVSEEPROM_H_
#define __NVSEEPROM_H_

/******************************************************************************
 * Includes Information
 *****************************************************************************/
#include <Preferences.h>
#include <Arduino.h>
#include <nvs.h>
#include <nvs_flash.h>

/******************************************************************************
 * Validation Status Enum
 *****************************************************************************/
enum class NVSStatus : uint8_t {
    NOT_CHECKED = 0,
    VALID = 1,
    INVALID_VERSION = 2,
    MISSING_ZAPNUMBER = 3,
    MISSING_PASSWORD = 4,
    CRC_FAILED = 5,
    UPGRADED = 6,
    INITIALIZATION_FAILED = 7
};

/******************************************************************************
 * Core0/Core1 Communication Struct
 *****************************************************************************/
struct NVSValidationResult {
    NVSStatus status;           // Overall validation status
    uint16_t currentVersion;    // Version found in NVS
    uint16_t expectedVersion;   // Version expected by code
    bool zapNumberValid;        // ZapNumber (AANNNN) exists and valid
    bool passwordHashValid;     // Password hash exists
    bool crcValid;              // Overall CRC32 check passed
    bool wasUpgraded;           // NVS was upgraded during validation
    uint32_t calculatedCRC;     // The calculated CRC32 value
    char zapNumber[7];          // The ZapNumber value (AANNNN format + null)
    
    // Constructor to initialize values
    NVSValidationResult() : 
        status(NVSStatus::NOT_CHECKED),
        currentVersion(0),
        expectedVersion(0),
        zapNumberValid(false),
        passwordHashValid(false),
        crcValid(false),
        wasUpgraded(false),
        calculatedCRC(0) {
        zapNumber[0] = '\0';
    }
};

/**
 * @brief NVS EEPROM wrapper class
 * @description
 * This class provides a simple interface for storing and retrieving data
 */
class NVSEeprom : public Preferences {
public:
    // NVS Version - increment when NVS structure changes
    static const uint16_t CURRENT_VERSION = 1;
    
    // Standard NVS keys
    static const char* KEY_VERSION;
    static const char* KEY_ZAPNUMBER;
    static const char* KEY_PASSWORD_HASH;
    static const char* KEY_NVS_CRC;
    
    // Constructor and Destructor
    NVSEeprom();
    ~NVSEeprom();
    
    // Initialize NVS - call this in setup()
    bool begin();
    
    // Hash functions - Step 1
    String getHash(const char* key, const String& defaultValue = "");
    bool putHash(const char* key, const String& value);

    // Hash generation and comparison - Step 2
    String makeHash(const String& data);
    bool compareHash(const String& data, const String& storedHash);

    // Version management - Step 3
    uint16_t getVersion(const char* key, uint16_t defaultVersion = 0);
    bool putVersion(const char* key, uint16_t version);    

    // NEW: Overall validation and tamper detection - Step 4
    NVSValidationResult validateNVS();
    bool updateNVSCRC();
    uint32_t calculateNVSCRC();
    
    // NEW: ZapNumber validation
    bool isValidZapNumber(const String& zapNumber);

private:
    // NVS Namespace
    static const char* NAMESPACE;
    uint32_t calculateCRC32(const uint8_t* data, size_t length);
    
    // Internal upgrade function
    bool upgradeNVS(uint16_t fromVersion, uint16_t toVersion);
};

#endif // __NVSEEPROM_H_
