/**
 * @file BacklightManagerLib.h
 * @author Julian (51fiftyone51fiftyone@gmail.com)
 * @brief Manages LCD backlight with PWM control, NVS storage, and screen saver integration
 * @version 0.1
 * @date 20260112
 * 
 * @copyright Copyright (c) 2025
 * 
 * Features:
 * - Analog PWM brightness control (0-100%)
 * - NVS storage for user preferences
 * - Screen saver integration
 * - Smooth fade transitions
 * - Initial device config detection (100% brightness)
 * - Default 75% after initial setup
 */

#pragma once
#ifndef __BACKLIGHT_MANAGER_LIB_H_
#define __BACKLIGHT_MANAGER_LIB_H_

/******************************************************************************
 * Includes Information
 *****************************************************************************/
#include <Arduino.h>
#include <Preferences.h>

class EARS_backLightManager {
public:
    /**
     * @brief Construct a new Backlight Manager
     */
    EARS_backLightManager();

    /**
     * @brief Initialize the backlight manager
     * @param pin GPIO pin for backlight control
     * @param pwmChannel PWM channel to use (0-15)
     * @param pwmFrequency PWM frequency in Hz (default 5000)
     * @param pwmResolution PWM resolution in bits (default 8)
     * @return true if initialization successful
     */
    bool begin(uint8_t pin, uint8_t pwmChannel = 0, 
               uint32_t pwmFrequency = 5000, uint8_t pwmResolution = 8);

    /**
     * @brief Set brightness level immediately
     * @param level Brightness level (0 = OFF, 100 = FULL ON)
     */
    void setBrightness(uint8_t level);

    /**
     * @brief Fade to brightness level smoothly
     * @param level Target brightness level (0-100)
     * @param durationMs Fade duration in milliseconds (default 200)
     */
    void fadeToBrightness(uint8_t level, uint16_t durationMs = 200);

    /**
     * @brief Get current brightness level
     * @return uint8_t Current brightness (0-100)
     */
    uint8_t getBrightness() const;

    /**
     * @brief Save current brightness to NVS
     * @return true if save successful
     */
    bool saveBrightness();

    /**
     * @brief Load brightness from NVS
     * @return true if load successful
     */
    bool loadBrightness();

    /**
     * @brief Turn backlight off
     */
    void off();

    /**
     * @brief Turn backlight to full brightness
     */
    void on();

    /**
     * @brief Check if this is initial device configuration
     * @return true if NVS has never been written
     */
    bool isInitialConfig() const;

    /**
     * @brief Mark initial configuration as complete
     */
    void completeInitialConfig();

    /**
     * @brief Store brightness before screen saver activates
     */
    void screenSaverActivate();

    /**
     * @brief Restore brightness after screen saver deactivates
     */
    void screenSaverDeactivate();

    /**
     * @brief Check if screen saver is currently active
     * @return true if screen saver active
     */
    bool isScreenSaverActive() const;

private:
    uint8_t _pin;
    uint8_t _pwmChannel;
    uint8_t _pwmResolution;
    uint32_t _maxDutyCycle;
    
    uint8_t _currentBrightness;
    uint8_t _savedBrightness;  // Brightness before screen saver
    bool _screenSaverActive;
    bool _initialized;

    Preferences _preferences;

    // NVS keys
    static constexpr const char* NVS_NAMESPACE = "backlight";
    static constexpr const char* NVS_BRIGHTNESS_KEY = "brightness";
    static constexpr const char* NVS_INIT_FLAG_KEY = "init_done";

    // Default values
    static constexpr uint8_t DEFAULT_BRIGHTNESS = 75;
    static constexpr uint8_t INITIAL_CONFIG_BRIGHTNESS = 100;

    /**
     * @brief Convert percentage (0-100) to PWM duty cycle
     * @param percentage Brightness percentage
     * @return uint32_t PWM duty cycle value
     */
    uint32_t percentageToDutyCycle(uint8_t percentage) const;
};

// Global instance access function
/////////////////////
// NO NO NO
/////////////////////
EARS_backLightManager& using_backlightmanager();

#endif // __BACKLIGHT_MANAGER_LIB_H_

/******************************************************************************
 * End of BacklightManagerLib.h
 ****************************************************************************/