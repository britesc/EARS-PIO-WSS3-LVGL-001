/**
 * @file NVSEeprom.cpp
 * @author Julian (51fiftyone51fiftyone@gmail.com)
 * @brief NVS EEPROM wrapper class implementation
 * @version 0.1
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

/******************************************************************************
 * End of File
 *****************************************************************************/
