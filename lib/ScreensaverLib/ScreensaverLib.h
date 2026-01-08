/**
 * @file ScreensaverLib.h
 * @author Julian (51fiftyone51fiftyone@gmail.com)
 * @brief Screensaver library implementation header file
 * @version 1.0.0
 * @date 20260107
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once
#ifndef SCREENSAVER_LIB_H
#define SCREENSAVER_LIB_H

#include <Arduino.h>
#include <lvgl.h>

/**
 * @brief Screensaver modes
 * @enum ScreensaverMode
 * 
 */
enum ScreensaverMode {
    SS_MODE_BLACK = 0,
    SS_MODE_EARS_TEXT = 1,
    SS_MODE_BUILTIN_IMAGE = 2,
    SS_MODE_USER_IMAGE = 3
};

/**
 * @brief Screensaver settings structure
 * @struct ScreensaverSettings
 */
struct ScreensaverSettings {
    bool enabled;
    uint8_t timeout_seconds;        // 0-120
    ScreensaverMode mode;
    uint8_t animation_speed;        // 1-10 scale
    bool bounce_mode;               // true=bounce, false=wrap
    uint8_t backlight_restore;      // Value to restore backlight to
};

/**
 * @brief Screensaver Library Class
 * 
 */
class ScreensaverLib {
public:
    ScreensaverLib();
    
    /**
     * @brief Initialization
     * @param display
     * @return void 
     */
    void begin(lv_display_t* display);
    
    // Settings management
    void setEnabled(bool enabled);
    void toggleEnabled();
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

/************************************************************************
 * End of ScreensaverLib.h
 ***********************************************************************/