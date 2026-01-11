/**
 * @file $(file) 
 * EARS_errorsLib.h
 * @author Julian (51fiftyone51fiftyone@gmail.com)
 * @brief Error Management Library for EARS Project
 * @version 1.0.0
 * @date 20260111
 * @details
 * Handles error/warning states, loads messages from JSON on TF card,
 * maintains error history, and provides non-blocking error notifications.
 * @copyright Copyright (c) 2025
 */

#pragma once
#ifndef __EARS_ERRORS_LIB_H__
#define __EARS_ERRORS_LIB_H__

/******************************************************************************
 * Includes Information
 *****************************************************************************/
#include <Arduino.h>
#include <SD.h>
#include <ArduinoJson.h>

class EARS_errors {
public:
    // Error severity levels (matching Logger functionality)
    enum ErrorLevel {
        NONE = 0,
        WARN = 1,
        ERROR = 2
    };

    // Constructor
    EARS_errors();
    
    // Destructor
    ~EARS_errors();

    // Initialize the library (load error messages from TF card)
    bool begin(const char* errorJsonPath = "/errors.json", 
               const char* logFilePath = "/error_log.txt");

    // Set an error or warning
    void setError(uint16_t code, ErrorLevel level);
    
    // Get current error information
    uint16_t getErrorCode();
    ErrorLevel getErrorLevel();
    String getErrorMessage();
    
    // Check if there's an active error/warning
    bool hasError();
    bool hasWarning();
    
    // User acknowledges the error (clears current state)
    void acknowledgeError();
    
    // Get level as string for display
    String getLevelString();
    
    // Manual reload of error messages (if JSON updated)
    bool reloadErrorMessages();

private:
    // Current error state
    uint16_t currentErrorCode;
    ErrorLevel currentErrorLevel;
    unsigned long errorTimestamp;
    
    // File paths
    String errorJsonPath;
    String logFilePath;
    
    // Error message storage (code -> message mapping)
    static const uint8_t MAX_ERROR_MESSAGES = 50;
    struct ErrorMessage {
        uint16_t code;
        String message;
    };
    ErrorMessage errorMessages[MAX_ERROR_MESSAGES];
    uint8_t errorMessageCount;
    
    // Internal methods
    bool loadErrorMessages();
    void logToHistory(uint16_t code, ErrorLevel level, const char* message);
    String findErrorMessage(uint16_t code);
    String levelToString(ErrorLevel level);
};

//////////////////////////////////////////////////////////////////////////////
// I do not understand why this is necessary?
// Global instance helper for easy access
extern EARS_errors errorsLib;

//////////////////////////////////////////////////////////////////////////////
// I do not understand why this is necessary?
// Convenience function for using the library
inline void using_ears_errors() {
    // Library is ready via global errorsLib instance
}

#endif // __EARS_ERRORS_LIB_H__

/************************************************************************
 * End of EARS_errorsLib.h
 ***********************************************************************/