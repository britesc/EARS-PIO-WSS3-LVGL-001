/**
 * @file EARS_sdCardLib.h
 * @author JTB
 * @brief SD Card wrapper library for ESP32-S3 with separate SPI bus
 * @version 1.1.0
 * @date 20260105
 * 
 * @copyright Copyright (c) 2026 JTB. All rights reserved.
 */

#pragma once
#ifndef __EARS_SDCARD_LIB_H__
#define __EARS_SDCARD_LIB_H__

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

/**
 * @brief SD Card management class
 * 
 * Handles SD card initialization and provides basic file operations
 * for the Waveshare ESP32-S3 3.5" LCD with dedicated SD card SPI bus
 */
class EARS_sdCard {
public:
    /**
     * @brief Construct a new EARS_sdCard object
     */
    EARS_sdCard();
    
    /**
     * @brief Destroy the EARS_sdCard object
     */
    ~EARS_sdCard();
    
    /**
     * @brief Initialize the SD card
     * 
     * @return true if SD card initialized successfully
     * @return false if SD card initialization failed
     */
    bool begin();
    
    /**
     * @brief Check if SD card is available
     * 
     * @return true if SD card is available
     * @return false if SD card is not available
     */
    bool isAvailable();
    
    /**
     * @brief Get SD card type
     * 
     * @return String describing card type (SD, SDHC, etc.)
     */
    String getCardType();
    
    /**
     * @brief Get SD card size in MB
     * 
     * @return uint64_t card size in megabytes
     */
    uint64_t getCardSizeMB();
    
    /**
     * @brief Get SD card free space in MB
     * 
     * @return uint64_t free space in megabytes
     */
    uint64_t getFreeSpaceMB();
    
    /**
     * @brief Create a directory on SD card
     * 
     * @param path Directory path
     * @return true if directory created or already exists
     * @return false if creation failed
     */
    bool createDirectory(const char* path);
    
    /**
     * @brief Check if file exists
     * 
     * @param path File path
     * @return true if file exists
     * @return false if file does not exist
     */
    bool fileExists(const char* path);
    
    /**
     * @brief Check if directory exists
     * 
     * @param path Directory path
     * @return true if directory exists
     * @return false if directory does not exist
     */
    bool directoryExists(const char* path);
    
    /**
     * @brief Remove a file
     * 
     * @param path File path
     * @return true if file removed
     * @return false if removal failed
     */
    bool removeFile(const char* path);
    
    /**
     * @brief Remove a directory
     * 
     * @param path Directory path
     * @return true if directory removed
     * @return false if removal failed
     */
    bool removeDirectory(const char* path);
    
    /**
     * @brief List directory contents
     * 
     * @param path Directory path
     * @param indent Indentation level for display
     */
    void listDirectory(const char* path, uint8_t indent = 0);
    
    /**
     * @brief Read entire file into String
     * 
     * @param path File path
     * @return String file contents (empty if failed)
     */
    String readFile(const char* path);
    
    /**
     * @brief Write String to file (overwrites existing)
     * 
     * @param path File path
     * @param content Content to write
     * @return true if write successful
     * @return false if write failed
     */
    bool writeFile(const char* path, const String& content);
    
    /**
     * @brief Append String to file
     * 
     * @param path File path
     * @param content Content to append
     * @return true if append successful
     * @return false if append failed
     */
    bool appendFile(const char* path, const String& content);
    
private:
    bool _initialized;
    SPIClass* _spi;
    
    /**
     * @brief Initialize SPI bus for SD card
     */
    void initSPI();
};

#endif // __EARS_SDCARD_LIB_H__

/****************************************************************************
 * End of EARS_sdCardLib.h
 ***************************************************************************/