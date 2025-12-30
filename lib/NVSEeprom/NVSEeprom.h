/**
 * @file NVSEeprom.h
 * @author Julian (51fiftyone51fiftyone@gmail.com)
 * @brief NVS EEPROM wrapper class header
 * @version 0.1
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

/**
 * @brief NVS EEPROM wrapper class
 * @description
 * This class provides a simple interface for storing and retrieving data
 */
class NVSEeprom : public Preferences {
public:
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

private:
    // NVS Namespace
    static const char* NAMESPACE;
    uint32_t calculateCRC32(const uint8_t* data, size_t length);    
};

#endif // __NVSEEPROM_H_