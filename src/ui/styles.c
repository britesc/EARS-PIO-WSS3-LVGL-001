#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: Light_Screen
//

void init_style_light_screen_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xff0a0b0b));
    lv_style_set_text_color(style, lv_color_hex(0xff9e9a75));
    lv_style_set_line_color(style, lv_color_hex(0xff604439));
    lv_style_set_border_color(style, lv_color_hex(0xff604439));
    lv_style_set_outline_color(style, lv_color_hex(0xff604439));
    lv_style_set_arc_color(style, lv_color_hex(0xff604439));
};

lv_style_t *get_style_light_screen_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_light_screen_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_light_screen(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_light_screen_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_light_screen(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_light_screen_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: Dark_Screen
//

void init_style_dark_screen_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xff0a0b0b));
    lv_style_set_text_color(style, lv_color_hex(0xffff6b6b));
};

lv_style_t *get_style_dark_screen_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_dark_screen_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_dark_screen(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_dark_screen_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_dark_screen(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_dark_screen_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_light_screen,
        add_style_dark_screen,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_light_screen,
        remove_style_dark_screen,
    };
    remove_style_funcs[styleIndex](obj);
}

