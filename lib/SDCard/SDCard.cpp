/**
 * @file SDCard.cpp
 * @author JTB
 * @brief SD Card wrapper library implementation
 * @version 1.0
 * @date 20260105
 * 
 * @copyright Copyright (c) 2026  JTB. All rights reserved.
 */

#include "SDCard.h"
#include "WS35TLCD_PINS.h"

/**
 * @brief Construct a new SDCard object
 * @param spi SPI bus instance
 * @param Initialised flag
 * @return void
 */
SDCard::SDCard() : _initialized(false), _spi(nullptr) {
}

/**
 * @brief Destroy the SDCard object
 */
SDCard::~SDCard() {
    if (_spi) {
        _spi->end();
    }
}

/**
 * @brief Initialize SPI bus for SD card
 * @return void
 */
void SDCard::initSPI() {
    // Create new SPI instance for SD card
    _spi = new SPIClass(HSPI);
    
    // Initialize SPI with SD card pins
    _spi->begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
}

/**
 * @brief Initialize the SD card
 * @return true if SD card initialized successfully
 * @return false if SD card initialization failed
 */
bool SDCard::begin() {
    if (_initialized) {
        return true;
    }
    
    // Initialize SPI bus
    initSPI();
    
    // Try to initialize SD card
    if (!SD.begin(SD_CS, *_spi)) {
        Serial.println("[SDCard] Initialization failed!");
        _initialized = false;
        return false;
    }
    
    // Check card type
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        Serial.println("[SDCard] No SD card attached");
        _initialized = false;
        return false;
    }
    
    _initialized = true;
    
    // Print card info
    Serial.println("[SDCard] Initialization successful");
    Serial.print("[SDCard] Type: ");
    Serial.println(getCardType());
    Serial.print("[SDCard] Size: ");
    Serial.print(getCardSizeMB());
    Serial.println(" MB");
    Serial.print("[SDCard] Free: ");
    Serial.print(getFreeSpaceMB());
    Serial.println(" MB");
    
    return true;
}

/**
 * @brief Check if SD card is available
 * @return true if SD card is available
 * @return false if SD card is not available
 */
bool SDCard::isAvailable() {
    return _initialized;
}

/**
 * @brief Get SD card type
 * @return String describing card type (SD, SDHC, MMC, Unknown, NONE)
 */
String SDCard::getCardType() {
    if (!_initialized) return "NONE";
    
    uint8_t cardType = SD.cardType();
    
    switch (cardType) {
        case CARD_MMC:
            return "MMC";
        case CARD_SD:
            return "SD";
        case CARD_SDHC:
            return "SDHC";
        default:
            return "UNKNOWN";
    }
}

/**
 * @brief Get SD card size in MB
 * @return uint64_t card size in megabytes
 */
uint64_t SDCard::getCardSizeMB() {
    if (!_initialized) return 0;
    
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    return cardSize;
}

/**
 * @brief Get SD card free space in MB
 * @return uint64_t free space in megabytes
 */
uint64_t SDCard::getFreeSpaceMB() {
    if (!_initialized) return 0;
    
    uint64_t totalBytes = SD.totalBytes();
    uint64_t usedBytes = SD.usedBytes();
    uint64_t freeBytes = totalBytes - usedBytes;
    
    return freeBytes / (1024 * 1024);
}

/**
 * @brief Create a directory on SD card
 * @param path Directory path
 * @return true if directory created or already exists
 * @return false if creation failed
 */
bool SDCard::createDirectory(const char* path) {
    if (!_initialized) return false;
    
    if (SD.mkdir(path)) {
        Serial.print("[SDCard] Directory created: ");
        Serial.println(path);
        return true;
    } else {
        // Check if it already exists
        if (directoryExists(path)) {
            return true;
        }
        Serial.print("[SDCard] Failed to create directory: ");
        Serial.println(path);
        return false;
    }
}

/**
 * @brief Check if file exists
 * @param path File path
 * @return true if file exists
 * @return false if file does not exist
 */
bool SDCard::fileExists(const char* path) {
    if (!_initialized) return false;
    
    File file = SD.open(path);
    if (file) {
        bool isFile = !file.isDirectory();
        file.close();
        return isFile;
    }
    return false;
}

/**
 * @brief Check if directory exists
 * @param path Directory path
 * @return true if directory exists
 * @return false if directory does not exist
 */
bool SDCard::directoryExists(const char* path) {
    if (!_initialized) return false;
    
    File dir = SD.open(path);
    if (dir) {
        bool isDir = dir.isDirectory();
        dir.close();
        return isDir;
    }
    return false;
}

/**
 * @brief Remove a file
 * @param path File path
 * @return true if file removed
 * @return false if removal failed
 */
bool SDCard::removeFile(const char* path) {
    if (!_initialized) return false;
    
    if (SD.remove(path)) {
        Serial.print("[SDCard] File removed: ");
        Serial.println(path);
        return true;
    }
    Serial.print("[SDCard] Failed to remove file: ");
    Serial.println(path);
    return false;
}

/**
 * @brief Remove a directory
 * @param path Directory path
 * @return true if directory removed
 * @return false if removal failed
 */
bool SDCard::removeDirectory(const char* path) {
    if (!_initialized) return false;
    
    if (SD.rmdir(path)) {
        Serial.print("[SDCard] Directory removed: ");
        Serial.println(path);
        return true;
    }
    Serial.print("[SDCard] Failed to remove directory: ");
    Serial.println(path);
    return false;
}

/**
 * @brief List directory contents
 * @param path Directory path
 * @param indent Indentation level for display
 * @return void
 */
void SDCard::listDirectory(const char* path, uint8_t indent) {
    if (!_initialized) return;
    
    File dir = SD.open(path);
    if (!dir) {
        Serial.print("[SDCard] Failed to open directory: ");
        Serial.println(path);
        return;
    }
    
    if (!dir.isDirectory()) {
        Serial.println("[SDCard] Not a directory");
        dir.close();
        return;
    }
    
    File file = dir.openNextFile();
    while (file) {
        for (uint8_t i = 0; i < indent; i++) {
            Serial.print("  ");
        }
        
        Serial.print(file.name());
        
        if (file.isDirectory()) {
            Serial.println("/");
            // Recursively list subdirectories
            String subPath = String(path) + "/" + String(file.name());
            listDirectory(subPath.c_str(), indent + 1);
        } else {
            Serial.print(" - ");
            Serial.print(file.size());
            Serial.println(" bytes");
        }
        
        file.close();
        file = dir.openNextFile();
    }
    
    dir.close();
}

/**
 * @brief Read entire file into String
 * @param path File path
 * @return String file contents (empty if failed)
 */
String SDCard::readFile(const char* path) {
    if (!_initialized) return "";
    
    File file = SD.open(path, FILE_READ);
    if (!file) {
        Serial.print("[SDCard] Failed to open file for reading: ");
        Serial.println(path);
        return "";
    }
    
    String content = "";
    while (file.available()) {
        content += char(file.read());
    }
    
    file.close();
    return content;
}

/**
 * @brief Write String to file (overwrites existing)
 * @param path File path
 * @param content Content to write
 * @return true if write successful
 * @return false if write failed
 */
bool SDCard::writeFile(const char* path, const String& content) {
    if (!_initialized) return false;
    
    File file = SD.open(path, FILE_WRITE);
    if (!file) {
        Serial.print("[SDCard] Failed to open file for writing: ");
        Serial.println(path);
        return false;
    }
    
    size_t written = file.print(content);
    file.close();
    
    if (written == content.length()) {
        return true;
    }
    
    Serial.print("[SDCard] Write failed: ");
    Serial.println(path);
    return false;
}

/**
 * @brief Append String to file
 * @param path File path
 * @param content Content to append
 * @return true if append successful
 * @return false if append failed
 */
bool SDCard::appendFile(const char* path, const String& content) {
    if (!_initialized) return false;
    
    File file = SD.open(path, FILE_APPEND);
    if (!file) {
        Serial.print("[SDCard] Failed to open file for appending: ");
        Serial.println(path);
        return false;
    }
    
    size_t written = file.print(content);
    file.close();
    
    if (written == content.length()) {
        return true;
    }
    
    Serial.print("[SDCard] Append failed: ");
    Serial.println(path);
    return false;
}

/************************************************************************
 * End of SDCard.cpp
 ***********************************************************************/