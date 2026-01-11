/**
 * EARS_errorsLib.cpp
 * Error Management Library Implementation
 */

#include "EARS_errorsLib.h"

//////////////////////////////////////////////////////////////////////////////
// I do not understand why this is necessary?
// Global instance
EARS_errors errorsLib;

/**
 * Constructor
 */
EARS_errors::EARS_errors() {
    currentErrorCode = 0;
    currentErrorLevel = NONE;
    errorTimestamp = 0;
    errorMessageCount = 0;
}

/**
 * Destructor
 */
EARS_errors::~EARS_errors() {
    // Cleanup if needed
}

/**
 * Initialize the library
 * @param errorJsonPath Path to errors.json on TF card
 * @param logFilePath Path to error_log.txt on TF card
 * @return true if initialization successful
 */
bool EARS_errors::begin(const char* errorJsonPath, const char* logFilePath) {
    this->errorJsonPath = String(errorJsonPath);
    this->logFilePath = String(logFilePath);
    
    // Load error messages from JSON
    return loadErrorMessages();
}

/**
 * Set an error or warning
 * @param code Error code number
 * @param level Severity level (WARN or ERROR)
 */
void EARS_errors::setError(uint16_t code, ErrorLevel level) {
    if (level == NONE) {
        // Setting NONE just clears the error
        acknowledgeError();
        return;
    }
    
    currentErrorCode = code;
    currentErrorLevel = level;
    errorTimestamp = millis();
    
    // Get the message for this code
    String message = findErrorMessage(code);
    
    // Log to history file
    logToHistory(code, level, message.c_str());
}

/**
 * Get current error code
 * @return Current error code (0 if none)
 */
uint16_t EARS_errors::getErrorCode() {
    return currentErrorCode;
}

/**
 * Get current error level
 * @return Current error level
 */
EARS_errors::ErrorLevel EARS_errors::getErrorLevel() {
    return currentErrorLevel;
}

/**
 * Get current error message
 * @return Human-readable error message
 */
String EARS_errors::getErrorMessage() {
    if (currentErrorLevel == NONE) {
        return "No error";
    }
    return findErrorMessage(currentErrorCode);
}

/**
 * Check if there's an active error
 * @return true if current level is ERROR
 */
bool EARS_errors::hasError() {
    return currentErrorLevel == ERROR;
}

/**
 * Check if there's an active warning
 * @return true if current level is WARN
 */
bool EARS_errors::hasWarning() {
    return currentErrorLevel == WARN;
}

/**
 * User acknowledges the error (clears it)
 */
void EARS_errors::acknowledgeError() {
    currentErrorCode = 0;
    currentErrorLevel = NONE;
    errorTimestamp = 0;
}

/**
 * Get level as string for display
 * @return "NONE", "WARN", or "ERROR"
 */
String EARS_errors::getLevelString() {
    return levelToString(currentErrorLevel);
}

/**
 * Reload error messages from JSON file
 * @return true if successful
 */
bool EARS_errors::reloadErrorMessages() {
    return loadErrorMessages();
}

/**
 * Load error messages from JSON file on TF card
 * @return true if successful
 */
bool EARS_errors::loadErrorMessages() {
    if (!SD.exists(errorJsonPath.c_str())) {
        Serial.println("Error: errors.json not found on TF card");
        return false;
    }
    
    File file = SD.open(errorJsonPath.c_str(), FILE_READ);
    if (!file) {
        Serial.println("Error: Could not open errors.json");
        return false;
    }
    
    // Parse JSON
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    
    if (error) {
        Serial.print("Error parsing errors.json: ");
        Serial.println(error.c_str());
        return false;
    }
    
    // Clear existing messages
    errorMessageCount = 0;
    
    // Load error messages from JSON
    JsonArray errors = doc["errors"].as<JsonArray>();
    for (JsonObject error : errors) {
        if (errorMessageCount >= MAX_ERROR_MESSAGES) {
            Serial.println("Warning: Too many error messages, some ignored");
            break;
        }
        
        uint16_t code = error["code"];
        const char* message = error["message"];
        
        errorMessages[errorMessageCount].code = code;
        errorMessages[errorMessageCount].message = String(message);
        errorMessageCount++;
    }
    
    Serial.print("Loaded ");
    Serial.print(errorMessageCount);
    Serial.println(" error messages");
    
    return true;
}

/**
 * Log error to history file on TF card
 * @param code Error code
 * @param level Error level
 * @param message Error message
 */
void EARS_errors::logToHistory(uint16_t code, ErrorLevel level, const char* message) {
    File file = SD.open(logFilePath.c_str(), FILE_APPEND);
    if (!file) {
        Serial.println("Error: Could not open error_log.txt for writing");
        return;
    }
    
    // Create timestamp string
    char timestamp[32];
    unsigned long ms = millis();
    unsigned long seconds = ms / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    
    sprintf(timestamp, "%02lu:%02lu:%02lu.%03lu", 
            hours % 24, minutes % 60, seconds % 60, ms % 1000);
    
    // Write log entry: [timestamp] LEVEL Code:1234 Message
    file.print("[");
    file.print(timestamp);
    file.print("] ");
    file.print(levelToString(level));
    file.print(" Code:");
    file.print(code);
    file.print(" ");
    file.println(message);
    
    file.close();
}

/**
 * Find error message for a given code
 * @param code Error code to look up
 * @return Error message string or "Unknown error"
 */
String EARS_errors::findErrorMessage(uint16_t code) {
    for (uint8_t i = 0; i < errorMessageCount; i++) {
        if (errorMessages[i].code == code) {
            return errorMessages[i].message;
        }
    }
    return "Unknown error (code " + String(code) + ")";
}

/**
 * Convert error level to string
 * @param level Error level enum
 * @return String representation
 */
String EARS_errors::levelToString(ErrorLevel level) {
    switch (level) {
        case NONE:  return "NONE";
        case WARN:  return "WARN";
        case ERROR: return "ERROR";
        default:    return "UNKNOWN";
    }
}

/************************************************************************
 * End of EARS_errorsLib.cpp
 ***********************************************************************/
