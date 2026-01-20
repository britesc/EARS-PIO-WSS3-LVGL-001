#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/* Color settings */
#define LV_COLOR_DEPTH 16
#define LV_COLOR_16_SWAP 0

/* Memory settings */
#define LV_MEM_CUSTOM 1
#define LV_MEM_SIZE (48 * 1024U)  // Can be smaller now since using PSRAM

#if LV_MEM_CUSTOM == 1
    #define LV_MEM_CUSTOM_INCLUDE <stdlib.h>
    #define LV_MEM_CUSTOM_ALLOC   heap_caps_malloc
    #define LV_MEM_CUSTOM_FREE    heap_caps_free
    #define LV_MEM_CUSTOM_REALLOC heap_caps_realloc
    #define LV_MEM_CUSTOM_ALLOC_PARAM   MALLOC_CAP_SPIRAM
#endif

/* Display settings */
#define LV_DPI_DEF 130

/* Feature usage */
#define LV_USE_PERF_MONITOR 1
#define LV_USE_MEM_MONITOR 1

/* CRITICAL: Matrix support - MUST be before vector graphics */
#define LV_USE_MATRIX 1

/* CRITICAL: Disable ARM-specific optimizations for ESP32 */
#define LV_USE_DRAW_SW_ASM LV_DRAW_SW_ASM_NONE

/* Enable filesystem support for SD card images */
#define LV_USE_FS_STDIO 1
#define LV_FS_STDIO_LETTER 'S'
#define LV_FS_STDIO_PATH "/sd"
#define LV_FS_STDIO_CACHE_SIZE 0

/* Image decoder - enable PNG and BMP */
#define LV_USE_BMP 1
#define LV_USE_PNG 1
#define LV_USE_SJPG 1

/* Image decoder - enable SVG (requires LV_USE_MATRIX = 1 above) */
#define LV_USE_VECTOR_GRAPHIC 1
#define LV_USE_SVG 1

/* Font support - ENABLE MONTSERRAT FONTS */
#define LV_FONT_MONTSERRAT_8  1
#define LV_FONT_MONTSERRAT_10 0
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 1
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_20 1
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 1
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_28 0
#define LV_FONT_MONTSERRAT_30 0
#define LV_FONT_MONTSERRAT_32 0
#define LV_FONT_MONTSERRAT_34 0
#define LV_FONT_MONTSERRAT_36 0
#define LV_FONT_MONTSERRAT_38 1
#define LV_FONT_MONTSERRAT_40 0
#define LV_FONT_MONTSERRAT_42 0
#define LV_FONT_MONTSERRAT_44 0
#define LV_FONT_MONTSERRAT_46 0
#define LV_FONT_MONTSERRAT_48 0

/* Logging */
#define LV_USE_LOG 1
#define LV_LOG_LEVEL LV_LOG_LEVEL_INFO
#define LV_LOG_PRINTF 1

#endif /*LV_CONF_H*/