/**
 * @file NVSEeprom.cpp
 * @author Julian (51fiftyone51fiftyone@gmail.com)
 * @brief NVS EEPROM wrapper class implementation
 * @version 0.2
 * @date 20251229
 * 
 * @copyright Copyright (c) 2025
 * 
 */

/******************************************************************************
 * Includes Information
 *****************************************************************************/
#include "NVSEeprom.h"

// NVS Namespace
const char* NVSEeprom::NAMESPACE = "EARS";

// Standard NVS Keys
const char* NVSEeprom::KEY_VERSION = "nvsVersion";
const char* NVSEeprom::KEY_ZAPNUMBER = "zapNumber";
const char* NVSEeprom::KEY_PASSWORD_HASH = "pwdHash";
const char* NVSEeprom::KEY_NVS_CRC = "nvsCRC";

// NVS Constructor 
NVSEeprom::NVSEeprom() {
}

// NVS Destructor
NVSEeprom::~NVSEeprom() {
}

/**
 * @brief Begin NVS
 * 
 * @return true 
 * @return false 
 */
bool NVSEeprom::begin() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    return (err == ESP_OK);
}

/**
 * @brief Get Hash from NVS
 * 
 * @param key 
 * @param defaultValue 
 * @return Hash String 
 */
String NVSEeprom::getHash(const char* key, const String& defaultValue) {
    // Open namespace in read-only mode
    if (!Preferences::begin(NAMESPACE, true)) {
        return defaultValue;
    }
    
    String hash = getString(key, defaultValue);
    end();
    
    return hash;
}

/**
 * @brief Put Hash into NVS
 * 
 * @param key 
 * @param value 
 * @return true 
 * @return false 
 */
bool NVSEeprom::putHash(const char* key, const String& value) {
    // Open namespace in read-write mode
    if (!Preferences::begin(NAMESPACE, false)) {
        return false;
    }
    
    size_t result = putString(key, value);
    end();
    
    return (result > 0);
}

/**
 * @brief Calculate CRC32 checksum.
 * 
 * @param data 
 * @param length 
 * @return uint32_t 
 */
uint32_t NVSEeprom::calculateCRC32(const uint8_t* data, size_t length) {
    uint32_t crc = 0xFFFFFFFF;
    
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
        }
    }
    
    return ~crc;
}

/**
 * @brief Generate CRC32 hash from data.
 * 
 * @param data 
 * @return String 
 */
String NVSEeprom::makeHash(const String& data) {
    uint32_t crc = calculateCRC32((const uint8_t*)data.c_str(), data.length());
    
    // Convert to 8-character hex string
    char hashStr[9];
    sprintf(hashStr, "%08X", crc);
    
    return String(hashStr);
}

/**
 * @brief Compare data against stored hash.
 * 
 * @param data 
 * @param storedHash 
 * @return true 
 * @return false 
 */
bool NVSEeprom::compareHash(const String& data, const String& storedHash) {
    String computedHash = makeHash(data);
    return (computedHash.equals(storedHash));
}

/**
 * @brief Get version number from NVS.
 * 
 * @param key 
 * @param defaultVersion 
 * @return uint16_t Version number (0-65535)
 */
uint16_t NVSEeprom::getVersion(const char* key, uint16_t defaultVersion) {
    // Open namespace in read-only mode
    if (!Preferences::begin(NAMESPACE, true)) {
        return defaultVersion;
    }
    
    uint16_t version = getUShort(key, defaultVersion);
    end();
    
    return version;
}

/**
 * @brief Put version number into NVS.
 * 
 * @param key 
 * @param version Version number (0-65535)
 * @return true Success
 * @return false Failed
 */
bool NVSEeprom::putVersion(const char* key, uint16_t version) {
    // Open namespace in read-write mode
    if (!Preferences::begin(NAMESPACE, false)) {
        return false;
    }
    
    size_t result = putUShort(key, version);
    end();
    
    return (result > 0);
}

/**
 * @brief Validate ZapNumber format (AANNNN)
 * 
 * @param zapNumber String to validate
 * @return true Valid format
 * @return false Invalid format
 */
bool NVSEeprom::isValidZapNumber(const String& zapNumber) {
    // Must be exactly 6 characters
    if (zapNumber.length() != 6) {
        return false;
    }
    
    // First two characters must be letters (A-Z)
    if (!isAlpha(zapNumber[0]) || !isAlpha(zapNumber[1])) {
        return false;
    }
    
    // Last four characters must be digits (0-9)
    for (int i = 2; i < 6; i++) {
        if (!isDigit(zapNumber[i])) {
            return false;
        }
    }
    
    return true;
}

/**
 * @brief Calculate CRC32 for entire NVS contents (excluding the CRC itself)
 * 
 * @return uint32_t CRC32 value
 */
uint32_t NVSEeprom::calculateNVSCRC() {
    String dataToHash = "";
    
    // Open namespace in read-only mode
    if (!Preferences::begin(NAMESPACE, true)) {
        return 0;
    }
    
    // Concatenate all critical data in a specific order
    // Version
    uint16_t version = getUShort(KEY_VERSION, 0);
    dataToHash += String(version);
    dataToHash += "|";
    
    // ZapNumber
    String zapNum = getString(KEY_ZAPNUMBER, "");
    dataToHash += zapNum;
    dataToHash += "|";
    
    // Password Hash
    String pwdHash = getString(KEY_PASSWORD_HASH, "");
    dataToHash += pwdHash;
    
    end();
    
    // Calculate CRC32 of concatenated data
    return calculateCRC32((const uint8_t*)dataToHash.c_str(), dataToHash.length());
}

/**
 * @brief Update the stored NVS CRC32 value
 * 
 * @return true Success
 * @return false Failed
 */
bool NVSEeprom::updateNVSCRC() {
    uint32_t crc = calculateNVSCRC();
    
    // Open namespace in read-write mode
    if (!Preferences::begin(NAMESPACE, false)) {
        return false;
    }
    
    size_t result = putUInt(KEY_NVS_CRC, crc);
    end();
    
    return (result > 0);
}

/**
 * @brief Upgrade NVS from one version to another
 * 
 * @param fromVersion Current version
 * @param toVersion Target version
 * @return true Upgrade successful
 * @return false Upgrade failed
 */
bool NVSEeprom::upgradeNVS(uint16_t fromVersion, uint16_t toVersion) {
    // Prevent downgrade
    if (toVersion <= fromVersion) {
        return false;
    }
    
    // Prevent upgrade beyond CURRENT_VERSION
    if (toVersion > CURRENT_VERSION) {
        return false;
    }
    
    // Open namespace in read-write mode
    if (!Preferences::begin(NAMESPACE, false)) {
        return false;
    }
    
    // Future: Add version-specific upgrade logic here
    // For now, just update the version number
    
    // Example upgrade paths:
    // if (fromVersion == 0 && toVersion >= 1) {
    //     // Upgrade from version 0 to 1
    //     // Add new keys, migrate data, etc.
    // }
    
    // Update version
    size_t result = putUShort(KEY_VERSION, toVersion);
    end();
    
    if (result == 0) {
        return false;
    }
    
    // Recalculate and update CRC after upgrade
    return updateNVSCRC();
}

/**
 * @brief Validate entire NVS storage
 * 
 * This function checks:
 * 1. Version matches or can be upgraded
 * 2. ZapNumber exists and is valid format
 * 3. Password hash exists
 * 4. Overall CRC32 is valid (no tampering)
 * 
 * @return NVSValidationResult Structure containing validation results
 */
NVSValidationResult NVSEeprom::validateNVS() {
    NVSValidationResult result;
    result.expectedVersion = CURRENT_VERSION;
    
    // Step 1: Check NVS initialization
    if (!Preferences::begin(NAMESPACE, true)) {
        result.status = NVSStatus::INITIALIZATION_FAILED;
        end();
        return result;
    }
    
    // Step 2: Get and check version
    result.currentVersion = getUShort(KEY_VERSION, 0);
    
    // Check if upgrade is needed
    if (result.currentVersion < CURRENT_VERSION) {
        end(); // Close read-only session
        
        // Attempt upgrade
        if (upgradeNVS(result.currentVersion, CURRENT_VERSION)) {
            result.wasUpgraded = true;
            result.currentVersion = CURRENT_VERSION;
        } else {
            result.status = NVSStatus::INVALID_VERSION;
            return result;
        }
        
        // Reopen for continued validation
        if (!Preferences::begin(NAMESPACE, true)) {
            result.status = NVSStatus::INITIALIZATION_FAILED;
            return result;
        }
    } else if (result.currentVersion > CURRENT_VERSION) {
        // Version from future - cannot handle
        result.status = NVSStatus::INVALID_VERSION;
        end();
        return result;
    }
    
    // Step 3: Check ZapNumber
    String zapNum = getString(KEY_ZAPNUMBER, "");
    if (zapNum.length() == 0 || !isValidZapNumber(zapNum)) {
        result.zapNumberValid = false;
        result.status = NVSStatus::MISSING_ZAPNUMBER;
        end();
        return result;
    }
    result.zapNumberValid = true;
    zapNum.toCharArray(result.zapNumber, 7);
    
    // Step 4: Check password hash
    String pwdHash = getString(KEY_PASSWORD_HASH, "");
    if (pwdHash.length() == 0) {
        result.passwordHashValid = false;
        result.status = NVSStatus::MISSING_PASSWORD;
        end();
        return result;
    }
    result.passwordHashValid = true;
    
    // Step 5: Check overall CRC32
    uint32_t storedCRC = getUInt(KEY_NVS_CRC, 0);
    end(); // Close before calculating new CRC
    
    uint32_t calculatedCRC = calculateNVSCRC();
    result.calculatedCRC = calculatedCRC;
    
    if (storedCRC != calculatedCRC) {
        result.crcValid = false;
        result.status = NVSStatus::CRC_FAILED;
        return result;
    }
    result.crcValid = true;
    
    // Step 6: All checks passed
    if (result.wasUpgraded) {
        result.status = NVSStatus::UPGRADED;
    } else {
        result.status = NVSStatus::VALID;
    }
    
    return result;
}

/******************************************************************************
 * End of File
 *****************************************************************************/
