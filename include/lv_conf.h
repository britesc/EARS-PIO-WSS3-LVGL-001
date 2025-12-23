#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/* Color settings */
#define LV_COLOR_DEPTH 16
#define LV_COLOR_16_SWAP 0

/* Memory settings */
#define LV_MEM_CUSTOM 0
#define LV_MEM_SIZE (128U * 1024U)

/* Display settings */
#define LV_DPI_DEF 130

/* Feature usage */
#define LV_USE_PERF_MONITOR 1
#define LV_USE_MEM_MONITOR 1

/* Enable filesystem support for SD card images */
#define LV_USE_FS_STDIO 1
#define LV_FS_STDIO_LETTER 'S'
#define LV_FS_STDIO_PATH "/sd"
#define LV_FS_STDIO_CACHE_SIZE 0

/* Image decoder - enable PNG and BMP */
#define LV_USE_BMP 1
#define LV_USE_PNG 1
#define LV_USE_SJPG 1

/* Font support */
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 1
#define LV_FONT_MONTSERRAT_20 1

/* Logging */
#define LV_USE_LOG 1
#define LV_LOG_LEVEL LV_LOG_LEVEL_DEBUG
#define LV_LOG_PRINTF 1

#endif