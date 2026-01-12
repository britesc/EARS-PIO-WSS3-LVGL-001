/**
 * @file EARS_loggerLib.cpp
 * @author JTB
 * @brief Enhanced logging system implementation v2.1
 * @version 2.5.0
 * @date 20250105
 * 
 * @copyright Copyright (c) 2025 JTB. All rights reserved.
 */

#include "EARS_loggerLib.h"
#include <time.h>
#include <sys/time.h>

/**
 * @brief Get singleton instance.
 * @return Logger& Reference to Logger instance.
 */
EARS_logger& EARS_logger::getInstance() {
    static EARS_logger instance;
    return instance;
}

/**
 * @brief Private constructor.
 */
EARS_logger::EARS_logger() : 
    _initialized(false),
    _logFilePath(""),
    _configFilePath(""),
    _sdCard(nullptr) {
}

/**
 * @brief Destructor.
 */
EARS_logger::~EARS_logger() {
}

/**
 * @brief Initialize logger.
 * @return true if initialization successful.
 * @return false if initialization failed.
 * @param logFilePath Path to log file (e.g., "/logs/debug.log")
 * @param configFilePath Path to unified config file (e.g., "/config/ears.config")
 * @param sdCard Pointer to SDCard instance 
 * 
 */
bool EARS_logger::begin(const char* logFilePath, const char* configFilePath, EARS_sdCard* sdCard) {
    if (_initialized) {
        return true;
    }
    
    if (!sdCard || !sdCard->isAvailable()) {
        return false;
    }
    
    _sdCard = sdCard;
    _logFilePath = String(logFilePath);
    _configFilePath = String(configFilePath);
    
    // Ensure log directory exists
    int lastSlash = _logFilePath.lastIndexOf('/');
    if (lastSlash > 0) {
        String logDir = _logFilePath.substring(0, lastSlash);
        if (!_sdCard->directoryExists(logDir.c_str())) {
            _sdCard->createDirectory(logDir.c_str());
        }
    }
    
    // Ensure config directory exists
    lastSlash = _configFilePath.lastIndexOf('/');
    if (lastSlash > 0) {
        String configDir = _configFilePath.substring(0, lastSlash);
        if (!_sdCard->directoryExists(configDir.c_str())) {
            _sdCard->createDirectory(configDir.c_str());
        }
    }
    
    // Load configuration (creates default if not exists)
    loadConfig();
    
    _initialized = true;
    
    // Log initialization
    info("=== Logger v2.1 Initialized ===");
    infof("Log file: %s", _logFilePath.c_str());
    infof("Config file: %s", _configFilePath.c_str());
    infof("Log level: %s", getLogLevelString().c_str());
    infof("Max file size: %d bytes (%.2f MB)", _config.maxFileSizeBytes, _config.maxFileSizeBytes / 1048576.0);
    infof("Max rotated files: %d", _config.maxRotatedFiles);
    
    return true;
}

/**
 * @brief Log a message at DEBUG level
 * @param message Message to log
 * @return void
 */
void EARS_logger::debug(const char* message) {
    log(LogLevel::DEBUG, message);
}

/**
 * @brief Log a formatted message at DEBUG level
 * @param format Format string
 * @return void
 */
void EARS_logger::debugf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    logf(LogLevel::DEBUG, format, args);
    va_end(args);
}

/**
 * @brief Log a message at INFO level
 * @param message Message to log
 * @return void
 */
void EARS_logger::info(const char* message) {
    log(LogLevel::INFO, message);
}

/**
 * @brief Log a formatted message at INFO level
 * @param format Format string
 * @return void
 */
void EARS_logger::infof(const char* format, ...) {
    va_list args;
    va_start(args, format);
    logf(LogLevel::INFO, format, args);
    va_end(args);
}

/**
 * @brief Log a message at WARN level
 * @param message Message to log
 * @return void
 */
void EARS_logger::warn(const char* message) {
    log(LogLevel::WARN, message);
}

/**
 * @brief Log a formatted message at WARN level
 * @param format Format string
 * @return void
 */
void EARS_logger::warnf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    logf(LogLevel::WARN, format, args);
    va_end(args);
}

/**
 * @brief Log a message at ERROR level
 * @param message Message to log
 * @return void
 */
void EARS_logger::error(const char* message) {
    log(LogLevel::ERROR, message);
}

/**
 * @brief Log a formatted message at ERROR level
 * @param format Format string
 * @return void
 */
void EARS_logger::errorf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    logf(LogLevel::ERROR, format, args);
    va_end(args);
}

/**
 * @brief Core logging function
 * @param level Log level
 * @param message Message to log
 * @return void
 */
void EARS_logger::log(LogLevel level, const char* message) {
    if (!_initialized || !shouldLog(level)) {
        return;
    }
    
    // Check if rotation needed
    if (needsRotation()) {
        performRotation();
    }
    
    // Format log entry
    String logEntry = "[";
    logEntry += getTimestamp();
    logEntry += "] [";
    logEntry += getLevelString(level);
    logEntry += "] ";
    logEntry += message;
    logEntry += "\n";
    
    // Write to file
    _sdCard->appendFile(_logFilePath.c_str(), logEntry);
}

/**
 * @brief Core formatted logging function
 * @param level Log level
 * @param format Format string
 * @param args Variable argument list
 * @return void
 */
void EARS_logger::logf(LogLevel level, const char* format, va_list args) {
    if (!_initialized || !shouldLog(level)) {
        return;
    }
    
    char buffer[512];
    vsnprintf(buffer, sizeof(buffer), format, args);
    log(level, buffer);
}

/**
 * @brief Check if level should be logged (hierarchical)
 * @param level Log level to check
 * @return true if should log
 * @return false if should not log
 */
bool EARS_logger::shouldLog(LogLevel level) const {
    // Hierarchical: current level must be >= message level
    return (static_cast<int>(_config.currentLevel) >= static_cast<int>(level));
}

/**
 * @brief Check if a message at given level would be logged
 * @param level Log level to check
 * @return true if would log
 * @return false if would not log
 */
bool EARS_logger::wouldLog(LogLevel level) const {
    return _initialized && shouldLog(level);
}

/**
 * @brief Get level string for log entry
 * @param level Log level
 * @return const char* Level string
 */
const char* EARS_logger::getLevelString(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARN:
            return "WARN";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::NONE:
        default:
            return "NONE";
    }
}

/**
 * @brief Get current timestamp string
 * @return String Timestamp in "YYYY-MM-DD HH:MM:SS" format 
 */
String EARS_logger::getTimestamp() const {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    struct tm timeinfo;
    localtime_r(&tv.tv_sec, &timeinfo);
    
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
             timeinfo.tm_year + 1900,
             timeinfo.tm_mon + 1,
             timeinfo.tm_mday,
             timeinfo.tm_hour,
             timeinfo.tm_min,
             timeinfo.tm_sec);
    
    return String(buffer);
}

/**
 * @brief Set current log level
 * @param level New log level (NONE, ERROR, WARN, INFO, DEBUG)
 * @return void
 */
void EARS_logger::setLogLevel(LogLevel level) {
    _config.currentLevel = level;
    saveConfig();
    
    // Log the change at INFO level (if it will be visible)
    if (shouldLog(LogLevel::INFO)) {
        infof("Log level changed to: %s", levelToString(level).c_str());
    }
}

/**
 * @brief Get log level as string
 * @return String Log level string
 */
String EARS_logger::getLogLevelString() const {
    return levelToString(_config.currentLevel);
}

/**
 * @brief Set log level from string
 * @param levelStr "NONE", "ERROR", "WARN", "INFO", or "DEBUG"
 * @return true if valid level string
 * @return false if invalid level string
 */
bool EARS_logger::setLogLevelFromString(const String& levelStr) {
    LogLevel level = parseLevelString(levelStr);
    if (level == LogLevel::NONE && levelStr != "NONE") {
        return false;  // Invalid string
    }
    setLogLevel(level);
    return true;
}

/**
 * @brief Parse log level from string
 * @param levelStr Log level string
 * @return LogLevel Parsed log level
 */
LogLevel EARS_logger::parseLevelString(const String& levelStr) const {
    String upper = levelStr;
    upper.toUpperCase();
    
    if (upper == "NONE") return LogLevel::NONE;
    if (upper == "ERROR") return LogLevel::ERROR;
    if (upper == "WARN" || upper == "WARNING") return LogLevel::WARN;
    if (upper == "INFO") return LogLevel::INFO;
    if (upper == "DEBUG") return LogLevel::DEBUG;
    
    return LogLevel::NONE;  // Default to NONE if invalid
}

/**
 * @brief Convert log level to string
 * @param level Log level
 * @return String Log level string
 */
String EARS_logger::levelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::NONE:
            return "NONE";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::WARN:
            return "WARN";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::DEBUG:
            return "DEBUG";
        default:
            return "UNKNOWN";
    }
}

/**
 * @brief Load entire unified config file
 * @param doc JsonDocument to load into
 * @return true if load successful
 * @return false if load failed* 
 */
bool EARS_logger::loadUnifiedConfig(JsonDocument& doc) {
    if (!_sdCard || !_sdCard->fileExists(_configFilePath.c_str())) {
        return false;
    }
    
    String jsonString = _sdCard->readFile(_configFilePath.c_str());
    if (jsonString.length() == 0) {
        return false;
    }
    
    DeserializationError error = deserializeJson(doc, jsonString);
    return !error;
}

/**
 * @brief Save entire unified config file
 * @param doc JsonDocument to save
 * @return true if save successful
 * @return false if save failed
 */
bool EARS_logger::saveUnifiedConfig(const JsonDocument& doc) {
    String jsonString;
    serializeJsonPretty(doc, jsonString);
    return _sdCard->writeFile(_configFilePath.c_str(), jsonString);
}

/**
 * @brief Load logger config from unified ears.config
 * @return true if load successful
 * @return false if load failed
 */
bool EARS_logger::loadConfig() {
    JsonDocument doc;
    
    if (!loadUnifiedConfig(doc)) {
        // Config doesn't exist, create default
        doc["system"]["version"] = "1.0.0";
        doc["system"]["zap_number"] = "";  // Will be populated from NVS
        doc["system"]["device_name"] = "EARS";
        
        doc["logger"]["log_level"] = "DEBUG";
        doc["logger"]["max_file_size_bytes"] = 1048576;
        doc["logger"]["max_rotated_files"] = 3;
        
        saveUnifiedConfig(doc);
    }
    
    // Extract logger section
    JsonObject loggerObj = doc["logger"];
    if (loggerObj.isNull()) {
        return false;
    }
    
    String levelStr = loggerObj["log_level"] | "DEBUG";
    _config.currentLevel = parseLevelString(levelStr);
    _config.maxFileSizeBytes = loggerObj["max_file_size_bytes"] | 1048576;
    _config.maxRotatedFiles = loggerObj["max_rotated_files"] | 3;
    
    return true;
}

/**
 * @brief Save logger config to unified ears.config
 * @return true if save successful
 * @return false if save failed
 */
bool EARS_logger::saveConfig() {
    if (!_initialized) {
        return false;
    }
    
    JsonDocument doc;
    
    // Load existing config or create new
    loadUnifiedConfig(doc);
    
    // Update logger section
    doc["logger"]["log_level"] = levelToString(_config.currentLevel);
    doc["logger"]["max_file_size_bytes"] = _config.maxFileSizeBytes;
    doc["logger"]["max_rotated_files"] = _config.maxRotatedFiles;
    
    return saveUnifiedConfig(doc);
}

/**
 * @brief Clear/delete current log file
 * @return true if clear successful
 * @return false if clear failed
 */
bool EARS_logger::clearLog() {
    if (!_initialized) {
        return false;
    }
    
    bool result = _sdCard->removeFile(_logFilePath.c_str());
    
    if (result) {
        info("Log file cleared");
    }
    
    return result;
}

/**
 * @brief Get current log file size in bytes
 * @return uint32_t Log file size in bytes
 */
uint32_t EARS_logger::getLogFileSize() {
    if (!_initialized || !_sdCard->fileExists(_logFilePath.c_str())) {
        return 0;
    }
    
    File file = SD.open(_logFilePath.c_str(), FILE_READ);
    if (!file) {
        return 0;
    }
    
    uint32_t size = file.size();
    file.close();
    
    return size;
}

/**
 * @brief Get current log file size in MB
 * @return float Log file size in MB
 */
float EARS_logger::getLogFileSizeMB() {
    return getLogFileSize() / 1048576.0;
}

/**
 * @brief Check if log needs rotation
 * @return true if rotation needed
 * @return false if rotation not needed
 */
bool EARS_logger::needsRotation() {
    uint32_t currentSize = getLogFileSize();
    return (currentSize >= _config.maxFileSizeBytes);
}

/**
 * @brief Perform log rotation
 * @return true if rotation successful
 * @return false if rotation failed
 */
bool EARS_logger::performRotation() {
    if (!_initialized) {
        return false;
    }
    
    info("Starting log rotation...");
    
    // Delete oldest rotated file if it exists
    String oldestFile = _logFilePath + "." + String(_config.maxRotatedFiles);
    if (_sdCard->fileExists(oldestFile.c_str())) {
        _sdCard->removeFile(oldestFile.c_str());
    }
    
    // Shift all rotated files up by one
    for (int i = _config.maxRotatedFiles - 1; i >= 1; i--) {
        String oldName = _logFilePath + "." + String(i);
        String newName = _logFilePath + "." + String(i + 1);
        
        if (_sdCard->fileExists(oldName.c_str())) {
            // Read old file
            String content = _sdCard->readFile(oldName.c_str());
            // Write to new file
            _sdCard->writeFile(newName.c_str(), content);
            // Delete old file
            _sdCard->removeFile(oldName.c_str());
        }
    }
    
    // Rename current log to .1
    String rotatedName = _logFilePath + ".1";
    if (_sdCard->fileExists(_logFilePath.c_str())) {
        String content = _sdCard->readFile(_logFilePath.c_str());
        _sdCard->writeFile(rotatedName.c_str(), content);
        _sdCard->removeFile(_logFilePath.c_str());
    }
    
    info("Log rotation completed");
    
    return true;
}

/**
 * @brief Force log rotation (for testing)
 * @return true if rotation successful
 * @return false if rotation failed
 */
bool EARS_logger::rotateLog() {
    return performRotation();
}

/*****************************************************************************
 * End of EARS_loggerLib.cpp
 ****************************************************************************/