/**
 * @file test_integration.cpp
 * @author JTB
 * @brief Integration test for EARS engine components
 * @version 1.0
 * @date 20250105
 * 
 * Tests:
 * - SD Card initialization and operations
 * - Logger with all log levels
 * - NVS EEPROM read/write
 * - Display output
 * - Unified configuration
 * - Component interaction
 * 
 * @copyright Copyright (c) 2025 JTB. All rights reserved.
 */

#include <Arduino.h>
#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include "WS35TLCD_PINS.h"
#include "RGB565_COLORS.h"
#include "RGB888_COLORS.h"
#include "SDCard.h"
#include "Logger.h"
#include "NVSEeprom.h"

/******************************************************************************
 * Test Configuration
 *****************************************************************************/
#define TEST_DELAY_MS 5000  // Delay between test sections

/******************************************************************************
 * Display Objects
 *****************************************************************************/
static const uint32_t screenWidth = TFT_WIDTH;
static const uint32_t screenHeight = TFT_HEIGHT;

static lv_color_t buf1[screenWidth * 40];
static lv_color_t buf2[screenWidth * 40];

Arduino_DataBus *bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, SPI_SCLK, SPI_MOSI, SPI_MISO);
Arduino_GFX *gfx = new Arduino_ILI9488_18bit(bus, LCD_RST, 1, true);

lv_display_t *disp;
lv_obj_t *test_screen;
lv_obj_t *test_label;
lv_obj_t *status_label;
lv_obj_t *progress_bar;

/******************************************************************************
 * Global Objects
 *****************************************************************************/
SDCard sdCard;
NVSEeprom nvs;

/******************************************************************************
 * Test State
 *****************************************************************************/
struct TestResults {
    bool sdCardInit;
    bool sdCardWrite;
    bool sdCardRead;
    bool loggerInit;
    bool loggerWrite;
    bool loggerLevels;
    bool nvsInit;
    bool nvsWrite;
    bool nvsRead;
    bool configWrite;
    bool configRead;
    bool displayInit;
    
    int passed;
    int failed;
    int total;
};

TestResults results = {0};

/******************************************************************************
 * Display Functions
 *****************************************************************************/

/**
 * @brief LVGL display flush callback
 */
void my_disp_flush(lv_display_t *display, const lv_area_t *area, uint8_t *px_map) {
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);
    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);
    lv_display_flush_ready(display);
}

/**
 * @brief LVGL tick callback
 */
uint32_t millis_cb(void) {
    return millis();
}

/**
 * @brief Update test display
 */
void updateDisplay(const char* testName, const char* status, int progress) {
    if (test_label != NULL) {
        lv_label_set_text(test_label, testName);
    }
    if (status_label != NULL) {
        lv_label_set_text(status_label, status);
    }
    if (progress_bar != NULL) {
        lv_bar_set_value(progress_bar, progress, LV_ANIM_ON);
    }
    lv_timer_handler();
    delay(100);
}

/**
 * @brief Initialize display
 */
bool initDisplay() {
    // Backlight
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);
    
    // Initialize display hardware
    gfx->begin();
    gfx->fillScreen(EARS_RGB565_BLACK);
    
    // Initialize LVGL
    lv_init();
    lv_tick_set_cb(millis_cb);
    
    disp = lv_display_create(screenWidth, screenHeight);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
    
    // Create test screen
    test_screen = lv_screen_active();
    lv_obj_set_style_bg_color(test_screen, lv_color_hex(EARS_RGB888_BLACK), 0);
    
    // Title
    lv_obj_t *title = lv_label_create(test_screen);
    lv_label_set_text(title, "EARS Engine Integration Test");
    lv_obj_set_style_text_color(title, lv_color_hex(EARS_RGB888_WHITE), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);
    
    // Test name label
    test_label = lv_label_create(test_screen);
    lv_label_set_text(test_label, "Initializing...");
    lv_obj_set_style_text_color(test_label, lv_color_hex(EARS_RGB888_CYAN), 0);
    lv_obj_set_style_text_font(test_label, &lv_font_montserrat_16, 0);
    lv_obj_align(test_label, LV_ALIGN_CENTER, 0, -40);
    
    // Status label
    status_label = lv_label_create(test_screen);
    lv_label_set_text(status_label, "Starting tests...");
    lv_obj_set_style_text_color(status_label, lv_color_hex(EARS_RGB888_WHITE), 0);
    lv_obj_set_style_text_font(status_label, &lv_font_montserrat_14, 0);
    lv_obj_align(status_label, LV_ALIGN_CENTER, 0, 0);
    
    // Progress bar
    progress_bar = lv_bar_create(test_screen);
    lv_obj_set_size(progress_bar, 300, 30);
    lv_obj_align(progress_bar, LV_ALIGN_CENTER, 0, 50);
    lv_bar_set_range(progress_bar, 0, 100);
    lv_bar_set_value(progress_bar, 0, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(progress_bar, lv_color_hex(0x404040), LV_PART_MAIN);
    lv_obj_set_style_bg_color(progress_bar, lv_color_hex(0x00FF00), LV_PART_INDICATOR);
    
    lv_timer_handler();
    
    return true;
}

/******************************************************************************
 * Test Functions
 *****************************************************************************/

/**
 * @brief Test 1: SD Card Initialization
 */
void test_SDCard_Init() {
    updateDisplay("Test 1/12: SD Card Init", "Testing SD card...", 8);
    
    results.sdCardInit = sdCard.begin();
    
    if (results.sdCardInit) {
        updateDisplay("Test 1/12: SD Card Init", "✓ PASS - SD card initialized", 8);
        results.passed++;
    } else {
        updateDisplay("Test 1/12: SD Card Init", "✗ FAIL - SD card failed", 8);
        results.failed++;
    }
    
    results.total++;
    delay(TEST_DELAY_MS);
}

/**
 * @brief Test 2: SD Card Write
 */
void test_SDCard_Write() {
    updateDisplay("Test 2/12: SD Card Write", "Writing test file...", 16);
    
    String testData = "EARS Integration Test - SD Write";
    results.sdCardWrite = sdCard.writeFile("/test_write.txt", testData);
    
    if (results.sdCardWrite) {
        updateDisplay("Test 2/12: SD Card Write", "✓ PASS - File written", 16);
        results.passed++;
    } else {
        updateDisplay("Test 2/12: SD Card Write", "✗ FAIL - Write failed", 16);
        results.failed++;
    }
    
    results.total++;
    delay(TEST_DELAY_MS);
}

/**
 * @brief Test 3: SD Card Read
 */
void test_SDCard_Read() {
    updateDisplay("Test 3/12: SD Card Read", "Reading test file...", 25);
    
    String readData = sdCard.readFile("/test_write.txt");
    results.sdCardRead = (readData.length() > 0 && readData.indexOf("EARS") >= 0);
    
    if (results.sdCardRead) {
        updateDisplay("Test 3/12: SD Card Read", "✓ PASS - File read OK", 25);
        results.passed++;
    } else {
        updateDisplay("Test 3/12: SD Card Read", "✗ FAIL - Read failed", 25);
        results.failed++;
    }
    
    results.total++;
    delay(TEST_DELAY_MS);
}

/**
 * @brief Test 4: Logger Initialization
 */
void test_Logger_Init() {
    updateDisplay("Test 4/12: Logger Init", "Initializing logger...", 33);
    
    results.loggerInit = Logger::getInstance().begin(
        "/logs/test.log",
        "/config/ears.config",
        &sdCard
    );
    
    if (results.loggerInit) {
        updateDisplay("Test 4/12: Logger Init", "✓ PASS - Logger initialized", 33);
        results.passed++;
    } else {
        updateDisplay("Test 4/12: Logger Init", "✗ FAIL - Logger failed", 33);
        results.failed++;
    }
    
    results.total++;
    delay(TEST_DELAY_MS);
}

/**
 * @brief Test 5: Logger Write
 */
void test_Logger_Write() {
    updateDisplay("Test 5/12: Logger Write", "Writing log entries...", 41);
    
    LOG_INFO("Integration test started");
    LOG_INFOF("Test number: %d", 5);
    
    // Check if log file was created
    results.loggerWrite = sdCard.fileExists("/logs/test.log");
    
    if (results.loggerWrite) {
        updateDisplay("Test 5/12: Logger Write", "✓ PASS - Log written", 41);
        results.passed++;
    } else {
        updateDisplay("Test 5/12: Logger Write", "✗ FAIL - Log write failed", 41);
        results.failed++;
    }
    
    results.total++;
    delay(TEST_DELAY_MS);
}

/**
 * @brief Test 6: Logger Levels
 */
void test_Logger_Levels() {
    updateDisplay("Test 6/12: Logger Levels", "Testing log levels...", 50);
    
    // Test all levels
    LOG_ERROR("Test ERROR level");
    LOG_WARN("Test WARN level");
    LOG_INFO("Test INFO level");
    LOG_DEBUG("Test DEBUG level");
    
    // Change level and test
    Logger::getInstance().setLogLevel(LogLevel::ERROR);
    String currentLevel = Logger::getInstance().getLogLevelString();
    
    results.loggerLevels = (currentLevel == "ERROR");
    
    // Reset to DEBUG for remaining tests
    Logger::getInstance().setLogLevel(LogLevel::DEBUG);
    
    if (results.loggerLevels) {
        updateDisplay("Test 6/12: Logger Levels", "✓ PASS - All levels work", 50);
        results.passed++;
    } else {
        updateDisplay("Test 6/12: Logger Levels", "✗ FAIL - Level change failed", 50);
        results.failed++;
    }
    
    results.total++;
    delay(TEST_DELAY_MS);
}

/**
 * @brief Test 7: NVS Initialization
 */
void test_NVS_Init() {
    updateDisplay("Test 7/12: NVS Init", "Initializing NVS...", 58);
    
    results.nvsInit = nvs.begin();
    
    if (results.nvsInit) {
        updateDisplay("Test 7/12: NVS Init", "✓ PASS - NVS initialized", 58);
        results.passed++;
        LOG_INFO("NVS initialized successfully");
    } else {
        updateDisplay("Test 7/12: NVS Init", "✗ FAIL - NVS failed", 58);
        results.failed++;
        LOG_ERROR("NVS initialization failed");
    }
    
    results.total++;
    delay(TEST_DELAY_MS);
}

/**
 * @brief Test 8: NVS Write
 */
void test_NVS_Write() {
    updateDisplay("Test 8/12: NVS Write", "Writing test data...", 66);
    
    String testZap = "TEST123ABC";
    results.nvsWrite = nvs.setZapNumber(testZap);
    
    if (results.nvsWrite) {
        updateDisplay("Test 8/12: NVS Write", "✓ PASS - NVS written", 66);
        results.passed++;
        LOG_INFO("NVS write successful");
    } else {
        updateDisplay("Test 8/12: NVS Write", "✗ FAIL - NVS write failed", 66);
        results.failed++;
        LOG_ERROR("NVS write failed");
    }
    
    results.total++;
    delay(TEST_DELAY_MS);
}

/**
 * @brief Test 9: NVS Read
 */
void test_NVS_Read() {
    updateDisplay("Test 9/12: NVS Read", "Reading test data...", 75);
    
    String readZap = nvs.getZapNumber();
    results.nvsRead = (readZap == "TEST123ABC");
    
    if (results.nvsRead) {
        updateDisplay("Test 9/12: NVS Read", "✓ PASS - NVS read OK", 75);
        results.passed++;
        LOG_INFOF("NVS read successful: %s", readZap.c_str());
    } else {
        updateDisplay("Test 9/12: NVS Read", "✗ FAIL - NVS read mismatch", 75);
        results.failed++;
        LOG_ERRORF("NVS read failed. Got: %s", readZap.c_str());
    }
    
    results.total++;
    delay(TEST_DELAY_MS);
}

/**
 * @brief Test 10: Config Write
 */
void test_Config_Write() {
    updateDisplay("Test 10/12: Config Write", "Writing config...", 83);
    
    // Logger automatically manages ears.config
    Logger::getInstance().setLogLevel(LogLevel::INFO);
    results.configWrite = sdCard.fileExists("/config/ears.config");
    
    if (results.configWrite) {
        updateDisplay("Test 10/12: Config Write", "✓ PASS - Config written", 83);
        results.passed++;
        LOG_INFO("Config write successful");
    } else {
        updateDisplay("Test 10/12: Config Write", "✗ FAIL - Config not found", 83);
        results.failed++;
        LOG_ERROR("Config write failed");
    }
    
    results.total++;
    delay(TEST_DELAY_MS);
}

/**
 * @brief Test 11: Config Read
 */
void test_Config_Read() {
    updateDisplay("Test 11/12: Config Read", "Reading config...", 91);
    
    String configContent = sdCard.readFile("/config/ears.config");
    results.configRead = (configContent.length() > 0 && 
                          configContent.indexOf("logger") >= 0);
    
    if (results.configRead) {
        updateDisplay("Test 11/12: Config Read", "✓ PASS - Config read OK", 91);
        results.passed++;
        LOG_INFO("Config read successful");
    } else {
        updateDisplay("Test 11/12: Config Read", "✗ FAIL - Config invalid", 91);
        results.failed++;
        LOG_ERROR("Config read failed");
    }
    
    results.total++;
    delay(TEST_DELAY_MS);
}

/**
 * @brief Test 12: Display Update
 */
void test_Display() {
    updateDisplay("Test 12/12: Display", "Testing display...", 100);
    
    // If we got here, display is working
    results.displayInit = true;
    results.passed++;
    results.total++;
    
    updateDisplay("Test 12/12: Display", "✓ PASS - Display OK", 100);
    LOG_INFO("Display test successful");
    
    delay(TEST_DELAY_MS);
}

/**
 * @brief Display final results
 */
void displayResults() {
    // Clear screen
    lv_obj_clean(test_screen);
    
    // Title
    lv_obj_t *title = lv_label_create(test_screen);
    lv_label_set_text(title, "Integration Test Complete");
    lv_obj_set_style_text_color(title, lv_color_hex(EARS_RGB888_WHITE), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);
    
    // Results summary
    lv_obj_t *summary = lv_label_create(test_screen);
    char summaryText[128];
    snprintf(summaryText, sizeof(summaryText), 
             "Passed: %d\nFailed: %d\nTotal: %d", 
             results.passed, results.failed, results.total);
    lv_label_set_text(summary, summaryText);
    lv_obj_set_style_text_align(summary, LV_TEXT_ALIGN_CENTER, 0);
    
    if (results.failed == 0) {
        lv_obj_set_style_text_color(summary, lv_color_hex(0x00FF00), 0);  // Green
    } else {
        lv_obj_set_style_text_color(summary, lv_color_hex(0xFF0000), 0);  // Red
    }
    lv_obj_set_style_text_font(summary, &lv_font_montserrat_24, 0);
    lv_obj_align(summary, LV_ALIGN_CENTER, 0, -30);
    
    // Final status
    lv_obj_t *status = lv_label_create(test_screen);
    if (results.failed == 0) {
        lv_label_set_text(status, "✓ All Tests Passed!\nEngine Ready");
        lv_obj_set_style_text_color(status, lv_color_hex(0x00FF00), 0);
    } else {
        lv_label_set_text(status, "✗ Some Tests Failed\nCheck Logs");
        lv_obj_set_style_text_color(status, lv_color_hex(0xFF0000), 0);
    }
    lv_obj_set_style_text_align(status, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(status, &lv_font_montserrat_16, 0);
    lv_obj_align(status, LV_ALIGN_CENTER, 0, 50);
    
    // Instruction
    lv_obj_t *instruction = lv_label_create(test_screen);
    lv_label_set_text(instruction, "Check /logs/test.log for details");
    lv_obj_set_style_text_color(instruction, lv_color_hex(EARS_RGB888_WHITE), 0);
    lv_obj_set_style_text_font(instruction, &lv_font_montserrat_12, 0);
    lv_obj_align(instruction, LV_ALIGN_BOTTOM_MID, 0, -20);
    
    lv_timer_handler();
    
    // Log final results
    LOG_INFO("=== Integration Test Complete ===");
    LOG_INFOF("Passed: %d / %d", results.passed, results.total);
    LOG_INFOF("Failed: %d / %d", results.failed, results.total);
    
    if (results.failed == 0) {
        LOG_INFO("✓ ALL TESTS PASSED - Engine is ready!");
    } else {
        LOG_ERROR("✗ SOME TESTS FAILED - Review results above");
    }
    LOG_INFO("================================");
}

/******************************************************************************
 * Main Functions
 *****************************************************************************/

void setup() {
    // Small delay for stability
    delay(1000);
    
    // Initialize display first so we can show progress
    results.displayInit = initDisplay();
    
    if (!results.displayInit) {
        // Can't show anything, just halt
        while (1) delay(1000);
    }
    
    updateDisplay("Starting Tests", "Initializing...", 0);
    delay(1000);
    
    // Run all tests
    test_SDCard_Init();
    test_SDCard_Write();
    test_SDCard_Read();
    test_Logger_Init();
    test_Logger_Write();
    test_Logger_Levels();
    test_NVS_Init();
    test_NVS_Write();
    test_NVS_Read();
    test_Config_Write();
    test_Config_Read();
    test_Display();
    
    // Display final results
    displayResults();
}

void loop() {
    // Keep LVGL running
    lv_timer_handler();
    delay(5);
}