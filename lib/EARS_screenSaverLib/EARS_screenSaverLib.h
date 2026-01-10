/**
 * @file EARS_screenSaver.h.h
 * @author Julian (51fiftyone51fiftyone@gmail.com)
 * @brief Screensaver library implementation header file
 * @version 1.0.0
 * @date 20260110
 * @update 20260107
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once
#ifndef __EARS_SCREENSAVER_LIB_H__
#define __EARS_SCREENSAVER_LIB_H__

#include <Arduino.h>
#include <lvgl.h>

/**
 * @brief Screensaver modes
 * @enum ScreensaverMode
 * @description Different modes for the screensaver
 * Modes:
 * - SS_MODE_BLACK: Blank screen
 * - SS_MODE_EARS_TEXT: "EARS" text animation
 * - SS_MODE_BUILTIN_IMAGE: Built-in image display
 * - SS_MODE_USER_IMAGE: User-defined image display
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
 * @description Holds configuration for the screensaver
 * Members:
 * - enabled: Whether the screensaver is enabled
 * - timeout_seconds: Inactivity timeout in seconds (0-120)
 * - mode: Screensaver mode (ScreensaverMode)
 * - animation_speed: Animation speed (1-10 scale)
 * - bounce_mode: Bounce mode (true=bounce, false=wrap)
 * - backlight_restore: Backlight level to restore to
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
class EARS_screenSaver {
public:
    EARS_screenSaver();
    
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

#endif // __EARS_SCREENSAVER_LIB_H__

/************************************************************************
 * End of EARS_screenSaverLib
 ***********************************************************************/