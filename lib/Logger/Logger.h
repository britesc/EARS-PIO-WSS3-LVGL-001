/**
 * @file Logger.h
 * @author JTB
 * @brief Enhanced logging system with hierarchical levels and unified config
 * @version 2.1
 * @date 20260105
 * 
 * @copyright Copyright (c) 2026 JTB. All rights reserved.
 */

#pragma once
#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <Arduino.h>
#include <SD.h>
#include <ArduinoJson.h>
#include "EARS_sdCardLib.h"

/**
 * @brief Hierarchical log level enumeration
 * 
 * Each level includes all levels below it:
 * - NONE: No logging
 * - ERROR: Only errors
 * - WARN: Warnings + Errors
 * - INFO: Info + Warnings + Errors
 * - DEBUG: Everything
 */
enum class LogLevel {
    NONE = 0,   // No logging
    ERROR = 1,  // Only errors
    WARN = 2,   // Warnings and above
    INFO = 3,   // Info and above
    DEBUG = 4   // Everything (most verbose)
};

/**
 * @brief Logger configuration structure
 */
struct LoggerConfig {
    LogLevel currentLevel;
    uint32_t maxFileSizeBytes;
    uint8_t maxRotatedFiles;
    
    // Default constructor - Development defaults
    LoggerConfig() : 
        currentLevel(LogLevel::DEBUG),  // Most verbose for development
        maxFileSizeBytes(1048576),      // 1MB
        maxRotatedFiles(3) {}
};

/**
 * @brief Enhanced Logger class with hierarchical levels
 */
class Logger {
public:
    /**
     * @brief Get singleton instance
     */
    static Logger& getInstance();
    
    /**
     * @brief Initialize logger
     * 
     * @param logFilePath Path to log file (e.g., "/logs/debug.log")
     * @param configFilePath Path to unified config file (e.g., "/config/ears.config")
     * @param sdCard Pointer to SDCard instance
     * @return true if initialization successful
     * @return false if initialization failed
     */
    bool begin(const char* logFilePath, const char* configFilePath, SDCard* sdCard);
    
    /**
     * @brief Check if logger is initialized
     * @return true if initialized
     * @return false if not initialized
     */
    bool isInitialized() const { return _initialized; }
    
    /**
     * @brief Log a message at DEBUG level
     * @return void
     */
    void debug(const char* message);
    
    /**
     * @brief Log a formatted message at DEBUG level
     * @param format printf-style format string
     * @return void
     */
    void debugf(const char* format, ...);
    
    /**
     * @brief Log a message at INFO level
     * @param message Message to log
     * @return void
     */
    void info(const char* message);
    
    /**
     * @brief Log a formatted message at INFO level
     * @param format printf-style format string
     * @return void
     */
    void infof(const char* format, ...);
    
    /**
     * @brief Log a message at WARN level
     * @param message Message to log
     * @return void
     */
    void warn(const char* message);
    
    /**
     * @brief Log a formatted message at WARN level
     * @param format printf-style format string
     * @return void
     */
    void warnf(const char* format, ...);
    
    /**
     * @brief Log a message at ERROR level
     * @param message Message to log
     * @return void
     */
    void error(const char* message);
    
    /**
     * @brief Log a formatted message at ERROR level
     * @param format printf-style format string
     * @return void
     */
    void errorf(const char* format, ...);
    
    /**
     * @brief Set current log level 
     * @param level New log level (NONE, ERROR, WARN, INFO, DEBUG)
     * @return void
     */
    void setLogLevel(LogLevel level);
    
    /**
     * @brief Get current log level
     * @param level New log level (NONE, ERROR, WARN, INFO, DEBUG)
     * @return LogLevel current log level
     */
    LogLevel getLogLevel() const { return _config.currentLevel; }
    
    /**
     * @brief Get log level as string
     * @return String "NONE", "ERROR", "WARN", "INFO", or "DEBUG"
     */
    String getLogLevelString() const;
    
    /**
     * @brief Set log level from string
     * 
     * @param levelStr "NONE", "ERROR", "WARN", "INFO", or "DEBUG"
     * @return true if valid level string
     * @return false if invalid level string
     */
    bool setLogLevelFromString(const String& levelStr);
    
    /**
     * @brief Save logger config to unified ears.config
     * @return true if save successful
     * @return false if save failed
     * 
     * Only updates the logger section, preserves other sections
     */
    bool saveConfig();
    
    /**
     * @brief Load logger config from unified ears.config
     * @return true if load successful
     * @return false if load failed
     */
    bool loadConfig();
    
    /**
     * @brief Clear/delete current log file
     * @return true if log cleared successfully
     * @return false if clearing log failed
     */
    bool clearLog();
    
    /**
     * @brief Get current log file size in bytes
     * @return uint32_t log file size in bytes
     */
    uint32_t getLogFileSize();
    
    /**
     * @brief Get current log file size in MB
     * @return float log file size in megabytes
     */
    float getLogFileSizeMB();
    
    /**
     * @brief Force log rotation (for testing)
     * @return true if rotation successful
     * @return false if rotation failed
     */
    bool rotateLog();
    
    /**
     * @brief Check if a message at given level would be logged
     * @param level LogLevel to check
     * @return true if message at this level would be logged
     * @return false if message at this level would be ignored
     * 
     * Useful for avoiding expensive string operations
     */
    bool wouldLog(LogLevel level) const;
    
private:
    // Singleton - private constructor
    Logger();
    ~Logger();
    
    // Delete copy constructor and assignment
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    bool _initialized;
    String _logFilePath;
    String _configFilePath;
    SDCard* _sdCard;
    LoggerConfig _config;
    
    /**
     * @brief Core logging function
     * @param level LogLevel of the message
     * @param message Message to log
     * @return void
     */
    void log(LogLevel level, const char* message);
    
    /**
     * @brief Core formatted logging function
     * @param level LogLevel of the message
     * @param format printf-style format string
     * @param args va_list of arguments
     * @return void
     */
    void logf(LogLevel level, const char* format, va_list args);
    
    /**
     * @brief Check if level should be logged (hierarchical)
     * @param level LogLevel to check
     * @return true if level should be logged
     * @return false if level should be ignored
     */
    bool shouldLog(LogLevel level) const;
    
    /**
     * @brief Get level string for log entry
     */
    const char* getLevelString(LogLevel level) const;
    
    /**
     * @brief Get current timestamp string
     * @return String timestamp in "YYYY-MM-DD HH:MM:SS" format
     */
    String getTimestamp() const;
    
    /**
     * @brief Check if log needs rotation
     * @return true if rotation needed
     * @return false if rotation not needed
     */
    bool needsRotation();
    
    /**
     * @brief Perform log rotation
     * @return true if rotation successful
     * @return false if rotation failed
     */
    bool performRotation();
    
    /**
     * @brief Load entire unified config file
     * @param doc JsonDocument to load config into
     * @return true if load successful
     * @return false if load failed
     */
    bool loadUnifiedConfig(JsonDocument& doc);
    
    /**
     * @brief Save entire unified config file
     * @param doc JsonDocument containing config to save
     * @return true if save successful
     * @return false if save failed
     */
    bool saveUnifiedConfig(const JsonDocument& doc);
    
    /**
     * @brief Parse log level from string
     * @param levelStr "NONE", "ERROR", "WARN", "INFO", or "DEBUG"
     * @return LogLevel parsed log level
     */
    LogLevel parseLevelString(const String& levelStr) const;
    
    /**
     * @brief Convert log level to string
     * @param level LogLevel to convert
     * @return String log level as string
     */
    String levelToString(LogLevel level) const;
};

// Convenience macros for easy logging
#define LOG_DEBUG(msg) Logger::getInstance().debug(msg)
#define LOG_DEBUGF(fmt, ...) Logger::getInstance().debugf(fmt, __VA_ARGS__)
#define LOG_INFO(msg) Logger::getInstance().info(msg)
#define LOG_INFOF(fmt, ...) Logger::getInstance().infof(fmt, __VA_ARGS__)
#define LOG_WARN(msg) Logger::getInstance().warn(msg)
#define LOG_WARNF(fmt, ...) Logger::getInstance().warnf(fmt, __VA_ARGS__)
#define LOG_ERROR(msg) Logger::getInstance().error(msg)
#define LOG_ERRORF(fmt, ...) Logger::getInstance().errorf(fmt, __VA_ARGS__)

// Legacy macros for backward compatibility (map to INFO level)
#define LOG_INIT(path, sd) Logger::getInstance().begin(path, "/config/ears.config", sd)
#define LOG(msg) Logger::getInstance().info(msg)
#define LOGF(fmt, ...) Logger::getInstance().infof(fmt, __VA_ARGS__)

#endif // __LOGGER_H__

/****************************************************************************
 * End of Logger.h
 ***************************************************************************/
