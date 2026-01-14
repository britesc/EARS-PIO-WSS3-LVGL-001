/**
 * @file BacklightManagerLib.cpp
 * @author Julian (51fiftyone51fiftyone@gmail.com)
 * @brief Implementation of LCD backlight manager
 * @version 0.1
 * @date 20260112
 */

#include "EARS_backlightManagerLib.h"

// Constructor
EARS_backLightManager::EARS_backLightManager()
    : _pin(0),
      _pwmChannel(0),
      _pwmResolution(8),
      _maxDutyCycle(255),
      _currentBrightness(0),
      _savedBrightness(0),
      _screenSaverActive(false),
      _initialized(false) {
}

// Initialize the backlight manager
bool EARS_backLightManager::begin(uint8_t pin, uint8_t pwmChannel, 
                                 uint32_t pwmFrequency, uint8_t pwmResolution) {
    _pin = pin;
    _pwmChannel = pwmChannel;
    _pwmResolution = pwmResolution;
    _maxDutyCycle = (1 << pwmResolution) - 1;  // e.g., 255 for 8-bit

    // Configure PWM
    ledcSetup(_pwmChannel, pwmFrequency, _pwmResolution);
    ledcAttachPin(_pin, _pwmChannel);

    // Open NVS preferences
    if (!_preferences.begin(NVS_NAMESPACE, false)) {
        Serial.println("[BacklightManager] ERROR: Failed to open NVS");
        return false;
    }

    // Determine initial brightness
    uint8_t initialBrightness;
    
    if (isInitialConfig()) {
        // First time setup - use 100% brightness
        initialBrightness = INITIAL_CONFIG_BRIGHTNESS;
        Serial.println("[BacklightManager] Initial config detected - using 100% brightness");
    } else {
        // Load saved brightness or use default
        initialBrightness = _preferences.getUChar(NVS_BRIGHTNESS_KEY, DEFAULT_BRIGHTNESS);
        Serial.printf("[BacklightManager] Loaded brightness: %d%%\n", initialBrightness);
    }

    // Set initial brightness immediately
    setBrightness(initialBrightness);
    
    _initialized = true;
    Serial.printf("[BacklightManager] Initialized on pin %d, PWM channel %d, freq %d Hz\n", 
                  _pin, _pwmChannel, pwmFrequency);
    
    return true;
}

// Set brightness immediately
void EARS_backLightManager::setBrightness(uint8_t level) {
    if (!_initialized) {
        Serial.println("[BacklightManager] ERROR: Not initialized");
        return;
    }

    // Constrain to valid range
    level = constrain(level, 0, 100);
    
    _currentBrightness = level;
    
    // Convert to PWM duty cycle and apply
    uint32_t dutyCycle = percentageToDutyCycle(level);
    ledcWrite(_pwmChannel, dutyCycle);
    
    Serial.printf("[BacklightManager] Brightness set to %d%% (duty: %d)\n", level, dutyCycle);
}

// Fade to brightness smoothly
void EARS_backLightManager::fadeToBrightness(uint8_t targetLevel, uint16_t durationMs) {
    if (!_initialized) {
        Serial.println("[BacklightManager] ERROR: Not initialized");
        return;
    }

    // Constrain target level
    targetLevel = constrain(targetLevel, 0, 100);
    
    uint8_t startLevel = _currentBrightness;
    
    // If already at target, do nothing
    if (startLevel == targetLevel) {
        return;
    }

    // Calculate step parameters
    int16_t levelDiff = targetLevel - startLevel;
    uint32_t startTime = millis();
    uint32_t endTime = startTime + durationMs;
    
    Serial.printf("[BacklightManager] Fading from %d%% to %d%% over %dms\n", 
                  startLevel, targetLevel, durationMs);

    // Perform fade
    while (millis() < endTime) {
        uint32_t elapsed = millis() - startTime;
        float progress = (float)elapsed / (float)durationMs;
        
        // Linear interpolation
        uint8_t currentLevel = startLevel + (levelDiff * progress);
        
        uint32_t dutyCycle = percentageToDutyCycle(currentLevel);
        ledcWrite(_pwmChannel, dutyCycle);
        
        delay(10);  // Small delay for smooth transition
    }
    
    // Ensure we end at exact target
    setBrightness(targetLevel);
}

// Get current brightness
uint8_t EARS_backLightManager::getBrightness() const {
    return _currentBrightness;
}

// Save brightness to NVS
bool EARS_backLightManager::saveBrightness() {
    if (!_initialized) {
        Serial.println("[BacklightManager] ERROR: Not initialized");
        return false;
    }

    size_t written = _preferences.putUChar(NVS_BRIGHTNESS_KEY, _currentBrightness);
    
    if (written) {
        Serial.printf("[BacklightManager] Saved brightness: %d%%\n", _currentBrightness);
        return true;
    } else {
        Serial.println("[BacklightManager] ERROR: Failed to save brightness");
        return false;
    }
}

// Load brightness from NVS
bool EARS_backLightManager::loadBrightness() {
    if (!_initialized) {
        Serial.println("[BacklightManager] ERROR: Not initialized");
        return false;
    }

    if (_preferences.isKey(NVS_BRIGHTNESS_KEY)) {
        uint8_t savedLevel = _preferences.getUChar(NVS_BRIGHTNESS_KEY, DEFAULT_BRIGHTNESS);
        setBrightness(savedLevel);
        Serial.printf("[BacklightManager] Loaded brightness: %d%%\n", savedLevel);
        return true;
    } else {
        Serial.println("[BacklightManager] No saved brightness found");
        return false;
    }
}

// Turn backlight off
void EARS_backLightManager::off() {
    setBrightness(0);
}

// Turn backlight to full brightness
void EARS_backLightManager::on() {
    setBrightness(100);
}

// Check if this is initial device configuration
bool EARS_backLightManager::isInitialConfig() const {
    // Check if the init flag exists in NVS
    return !_preferences.isKey(NVS_INIT_FLAG_KEY);
}

// Mark initial configuration as complete
void EARS_backLightManager::completeInitialConfig() {
    _preferences.putBool(NVS_INIT_FLAG_KEY, true);
    
    // Save current brightness and set default for future
    saveBrightness();
    
    Serial.println("[BacklightManager] Initial config marked complete");
}

// Store brightness before screen saver activates
void EARS_backLightManager::screenSaverActivate() {
    if (_screenSaverActive) {
        return;  // Already active
    }

    _savedBrightness = _currentBrightness;
    _screenSaverActive = true;
    
    Serial.printf("[BacklightManager] Screen saver activated - saved brightness: %d%%\n", 
                  _savedBrightness);
    
    // Fade to off or dim level (you can make this configurable)
    fadeToBrightness(0, 500);  // 500ms fade to black
}

// Restore brightness after screen saver deactivates
void EARS_backLightManager::screenSaverDeactivate() {
    if (!_screenSaverActive) {
        return;  // Not active
    }

    _screenSaverActive = false;
    
    Serial.printf("[BacklightManager] Screen saver deactivated - restoring brightness: %d%%\n", 
                  _savedBrightness);
    
    // Fade back to saved brightness
    fadeToBrightness(_savedBrightness, 300);  // 300ms fade back
}

// Check if screen saver is active
bool EARS_backLightManager::isScreenSaverActive() const {
    return _screenSaverActive;
}

// Convert percentage to PWM duty cycle
uint32_t EARS_backLightManager::percentageToDutyCycle(uint8_t percentage) const {
    // Simple linear mapping: 0% = 0, 100% = maxDutyCycle
    return (_maxDutyCycle * percentage) / 100;
}

// Global instance
///////////////////////
// NO NO NO
//////////////////////
static EARS_backLightManager globalBacklightManager;

///////////////////////
// NO NO NO
//////////////////////
EARS_backLightManager& using_backlightmanager() {
    return globalBacklightManager;
}

/******************************************************************************
 * End of BacklightManagerLib.cpp
 ****************************************************************************/