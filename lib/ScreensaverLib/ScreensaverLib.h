#pragma once
#ifndef SCREENSAVER_LIB_H
#define SCREENSAVER_LIB_H

#include <Arduino.h>
#include <lvgl.h>

// Screensaver modes
enum ScreensaverMode {
    SS_MODE_BLACK = 0,
    SS_MODE_EARS_TEXT = 1,
    SS_MODE_BUILTIN_IMAGE = 2,
    SS_MODE_USER_IMAGE = 3
};

// Screensaver settings structure
struct ScreensaverSettings {
    bool enabled;
    uint8_t timeout_seconds;        // 0-120
    ScreensaverMode mode;
    uint8_t animation_speed;        // 1-10 scale
    bool bounce_mode;               // true=bounce, false=wrap
    uint8_t backlight_restore;      // Value to restore backlight to
};

// Screensaver manager class
class ScreensaverLib {
public:
    ScreensaverLib();
    
    // Initialization
    void begin(lv_display_t* display);
    
    // Settings management
    void setEnabled(bool enabled);
    void setTimeout(uint8_t seconds);
    void setMode(ScreensaverMode mode);
    void setAnimationSpeed(uint8_t speed);
    void setBounceMode(bool bounce);
    
    // Control functions
    void reset();                   // Reset inactivity timer
    void activate();                // Manually activate screensaver
    void deactivate();              // Wake from screensaver
    void update();                  // Call regularly in loop
    
    // State queries
    bool isActive();
    ScreensaverSettings getSettings();
    
private:
    lv_display_t* _display;
    ScreensaverSettings _settings;
    uint32_t _last_activity_ms;
    bool _is_active;
    lv_obj_t* _screensaver_screen;
    
    // Internal functions
    void createScreensaverScreen();
    void destroyScreensaverScreen();
    void saveBacklight();
    void restoreBacklight();
    void updateAnimation();
};

#endif // SCREENSAVER_LIB_H