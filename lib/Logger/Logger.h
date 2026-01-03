#ifndef LOGGER_H
#define LOGGER_H

// Set to 1 for development, 0 for production
#define EARS_DEBUG 1

#if EARS_DEBUG

#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>

class Logger {
    public:
        static void init(const char* filename = "/logs/debug.log");
        static void log(const char* message);
        static void logf(const char* format, ...);
        static void logWithInfo(const char* file, int line, const char* func, const char* message);
        static void logfWithInfo(const char* file, int line, const char* func, const char* format, ...);
        static void flush();
        static void clear();
    private:
        static File logFile;
        static char logPath[64];
        static const char* extractFilename(const char* path);
    };

// Enhanced macros with file/line info
#define LOG_INIT(file) Logger::init(file)
#define LOG(msg) Logger::logWithInfo(__FILE__, __LINE__, __func__, msg)
#define LOGF(fmt, ...) Logger::logfWithInfo(__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#define LOG_FLUSH() Logger::flush()
#define LOG_CLEAR() Logger::clear()

#else

// When EARS_DEBUG is 0, these become empty (compile to nothing)
#define LOG_INIT(file) ((void)0)
#define LOG(msg) ((void)0)
#define LOGF(fmt, ...) ((void)0)
#define LOG_FLUSH() ((void)0)
#define LOG_CLEAR() ((void)0)

#endif // EARS_DEBUG

#endif // LOGGER_H