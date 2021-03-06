#ifndef INC_WEATHERWIDGET_H
#define INC_WEATHERWIDGET_H

#include "os_type.h"
#include "osapi.h"
#include "widgets/weathericons.h"
#include "widgets/openweathermap.h"
#include "widgets/timezonedb.h"
#include "widgets/darkskynet.h"
#include "http.h"
#include "ui.h"
#include "fonts.h"
#include <stdlib.h>

// Default positioning for the widget
#define WW_POS_CURRENT_TEMP_X 0
#define WW_POS_CURRENT_TEMP_Y 0
#define WW_POS_CELSIUS_X 80
#define WW_POS_CELSIUS_Y 0
#define WW_POS_CURRENT_MAX_TEMP_X 64
#define WW_POS_CURRENT_MAX_TEMP_Y 60
#define WW_POS_CURRENT_MIN_TEMP_X  8
#define WW_POS_CURRENT_MIN_TEMP_Y 60
#define WW_POS_CURRENT_ICON_X 104
#define WW_POS_CURRENT_ICON_Y 0
#define WW_POS_DESC_X 8
#define WW_POS_DESC_Y 64
#define WW_POS_DAY1_ICON_X 8
#define WW_POS_DAY1_ICON_Y 112
#define WW_POS_DAY1_MAX_TEMP_X 24
#define WW_POS_DAY1_MAX_TEMP_Y 160
#define WW_POS_DAY1_MIN_TEMP_X 24
#define WW_POS_DAY1_MIN_TEMP_Y 184
#define WW_POS_DAY2_ICON_X 72
#define WW_POS_DAY2_ICON_Y 112
#define WW_POS_DAY2_MAX_TEMP_X 88
#define WW_POS_DAY2_MAX_TEMP_Y 160
#define WW_POS_DAY2_MIN_TEMP_X 88
#define WW_POS_DAY2_MIN_TEMP_Y 184
#define WW_POS_DAY3_ICON_X 138
#define WW_POS_DAY3_ICON_Y 112
#define WW_POS_DAY3_MAX_TEMP_X 156
#define WW_POS_DAY3_MAX_TEMP_Y 160
#define WW_POS_DAY3_MIN_TEMP_X 156
#define WW_POS_DAY3_MIN_TEMP_Y 184

//demo data
#define BP_LAT "47.4607"
#define BP_LNG "19.1148"

#define RESPONSE_STATUS_CURRENT_DONE (1 << 0)
#define RESPONSE_STATUS_DAY1_DONE (1 << 1)
#define RESPONSE_STATUS_DAY2_DONE (1 << 2)
#define RESPONSE_STATUS_DAY3_DONE (1 << 3)
#define RESPONSE_STATUS_FINISHED 0X0F

// weather widget UI data
typedef struct{
    //Current day data
    int8_t          temp;
    int8_t          temp_max;
    int8_t          temp_min;
    const icon_t*   icon;

    //Next days datas
    int8_t          day1_temp_max;
    int8_t          day1_temp_min;
    const icon_t*   day1_icon;
    
    int8_t          day2_temp_max;
    int8_t          day2_temp_min;
    const icon_t*   day2_icon;
    
    int8_t          day3_temp_max;
    int8_t          day3_temp_min;
    const icon_t*   day3_icon;
} weatherwidget_ui_t;

void owm_getWeatherinfo(uint16_t cityID);
void weatherwidget_updateUI();
void ICACHE_FLASH_ATTR owm_http_callback_current(char * response_body, int http_status, char * response_headers, int body_size);
const icon_t* getIconByID(uint8_t icon_id);
void ICACHE_FLASH_ATTR timezone_db_callback(char * response_body, int http_status, char * response_headers, int body_size);
void ICACHE_FLASH_ATTR darkskynet_callback_day1(char * response_body, int http_status, char * response_headers, int body_size);
void ICACHE_FLASH_ATTR darkskynet_callback_day2(char * response_body, int http_status, char * response_headers, int body_size);
void ICACHE_FLASH_ATTR darkskynet_callback_day3(char * response_body, int http_status, char * response_headers, int body_size);
const icon_t *ds_get_icon_by_id(char *tmp);
void darkskynet_url_build(char *lat, char *lng, uint32_t unixtime, uint8_t day_offset, char *url);

#endif