#include <Arduino.h>
#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include "WS35TLCD_PINS.h"
#include "RGB565_COLORS.h"
#include "NVSEeprom.h"

// Display settings
static const uint32_t screenWidth = TFT_WIDTH;
static const uint32_t screenHeight = TFT_HEIGHT;

// LVGL draw buffers
static lv_color_t buf1[screenWidth * 40];
static lv_color_t buf2[screenWidth * 40];

// Arduino GFX display object
Arduino_DataBus *bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, SPI_SCLK, SPI_MOSI, SPI_MISO);
Arduino_GFX *gfx = new Arduino_ILI9488_18bit(bus, LCD_RST, 0 /* rotation */, false /* IPS */);

// LVGL display object
lv_display_t *disp;

// LVGL display flush callback
void my_disp_flush(lv_display_t *display, const lv_area_t *area, uint8_t *px_map) {
    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);

    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);

    lv_display_flush_ready(display);
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("LVGL Trial Starting...");

    // Initialize backlight
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, HIGH);

    // Initialize display
    gfx->begin();
    gfx->fillScreen(RGB565_BLACK);
    
    Serial.println("Display initialized");

    // Initialize LVGL
    lv_init();
    
    // Create LVGL display
    disp = lv_display_create(screenWidth, screenHeight);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    Serial.println("LVGL initialized");

    // Create a simple test screen
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(RGB565_NAVY), 0);

    // Create a label
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label, "LVGL Trial\nWorking!");
    lv_obj_set_style_text_color(label, lv_color_hex(RGB565_WHITE), 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_center(label);

    Serial.println("Test screen created");

    // NVS Test Step 1
    nvs.putHash("test", "abc123");
    String retrieved = nvs.getHash("test");
    Serial.println(retrieved);    
}

void loop() {
    lv_timer_handler();
    delay(5);
}