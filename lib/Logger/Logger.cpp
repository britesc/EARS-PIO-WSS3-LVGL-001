#include "Logger.h"

#if EARS_DEBUG

File Logger::logFile;
char Logger::logPath[64];

// Helper to extract just filename from full path
const char* Logger::extractFilename(const char* path) {
    const char* filename = strrchr(path, '/');
    if (!filename) {
        filename = strrchr(path, '\\');
    }
    return filename ? filename + 1 : path;
}

void Logger::init(const char* filename) {
    // Initialize LittleFS
    if (!LittleFS.begin()) {
        return;
    }
    
    strncpy(logPath, filename, sizeof(logPath) - 1);
    
    // Create logs directory if it doesn't exist
    if (strncmp(filename, "/logs/", 6) == 0) {
        LittleFS.mkdir("/logs");
    }
    
    // Open file in append mode
    logFile = LittleFS.open(filename, "a");
    if (logFile) {
        logFile.println("\n=== New Session ===");
        logFile.printf("Compiled: %s %s\n", __DATE__, __TIME__);
        logFile.flush();
    }
}

void Logger::log(const char* message) {
    if (logFile) {
        logFile.println(message);
        logFile.flush();
    }
}

void Logger::logf(const char* format, ...) {
    if (logFile) {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        
        logFile.println(buffer);
        logFile.flush();
    }
}

void Logger::logWithInfo(const char* file, int line, const char* func, const char* message) {
    if (logFile) {
        unsigned long ms = millis();
        logFile.printf("[%lu.%03lu] %s:%d (%s) - %s\n", 
                      ms / 1000, ms % 1000,
                      extractFilename(file), line, func, message);
        logFile.flush();
    }
}

void Logger::logfWithInfo(const char* file, int line, const char* func, const char* format, ...) {
    if (logFile) {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        
        unsigned long ms = millis();
        logFile.printf("[%lu.%03lu] %s:%d (%s) - %s\n", 
                      ms / 1000, ms % 1000,
                      extractFilename(file), line, func, buffer);
        logFile.flush();
    }
}

void Logger::flush() {
    if (logFile) {
        logFile.flush();
    }
}

void Logger::clear() {
    if (logFile) {
        logFile.close();
        LittleFS.remove(logPath);
        logFile = LittleFS.open(logPath, "a");
    }
}

#endif // EARS_DEBUG