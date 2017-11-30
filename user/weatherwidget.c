#include "widgets/weatherwidget.h"
// Main source code for the weather widget

weatherwidget_ui_t ui;

void owm_getWeatherinfo(uint16_t cityID){
    char url_current[] = OWM_WEATHER_BASEURL OWM_CITY_ID_BUDAPEST_IX OWM_API_KEY OWM_METRIC_UNIT;
    http_get(url_current, "", &owm_http_callback_current);
    http_get(TZDB_URL(BP_LAT,BP_LNG), "", &timezone_db_callback);
    //http_get(url_forecast, "", &owm_http_callback_forecast);
}

void weatherwidget_updateUI(){
    draw_number(WW_POS_CURRENT_TEMP_X,WW_POS_CURRENT_TEMP_Y, ui.temp, &numbers72, UI_SHOW_SIGN);
    draw_number(WW_POS_CURRENT_MIN_TEMP_X,WW_POS_CURRENT_MIN_TEMP_Y,ui.temp_max, &numbers24, UI_SHOW_SIGN);
    draw_number(WW_POS_CURRENT_MAX_TEMP_X,WW_POS_CURRENT_MAX_TEMP_Y,ui.temp_min, &numbers24, UI_SHOW_SIGN);
    draw_icon(WW_POS_CURRENT_ICON_X,WW_POS_CURRENT_ICON_Y, ui.icon);
    edp_displayFrame();
}

void ICACHE_FLASH_ATTR owm_http_callback_current(char * response_body, int http_status, char * response_headers, int body_size)
{
	os_printf("http_status=%d\n", http_status);
	if (http_status != HTTP_STATUS_GENERIC_ERROR) {
		os_printf("strlen(headers)=%d\n", strlen(response_headers));
		os_printf("body_size=%d\n", body_size);
		os_printf("body=%s<EOF>\n", response_body); // FIXME: this does not handle binary data.
    
    char owm_tmp[5];
    //get the current temperature
    json_getValue(response_body, OWM_JSON_KEY_CURRENT_TEMPERATURE, owm_tmp, 5);
    ui.temp = atoi(owm_tmp);
    os_printf("temperature1: %d", ui.temp);
    //get the current temperature max
    json_getValue(response_body, OWM_JSON_KEY_CURRENT_TEMPMAX, owm_tmp, 5);
    ui.temp_max = atoi(owm_tmp);
    os_printf("temperature2: %d", ui.temp_max);
    //get the current temperature min
    json_getValue(response_body, OWM_JSON_KEY_CURRENT_TEMPMIN, owm_tmp, 5);
    ui.temp_min = atoi(owm_tmp);
    os_printf("temperature: %d", ui.temp_min);
    //get the weather icon
    json_getValue(response_body, OWM_JSON_KEY_CURRENT_WEATER_ICON, owm_tmp, 5);
    owm_tmp[2] = '\0';
    uint8_t icon_id =  atoi(owm_tmp);
    os_printf("ICON_ID: %d,%s", icon_id, owm_tmp);
    ui.icon = getIconByID(icon_id);

    weatherwidget_updateUI();

	}
}

const icon_t* getIconByID(uint8_t icon_id){
    // http://openweathermap.org/weather-conditions
    switch(icon_id){
        case 1:     return &clear_sky.icon_96; break;           // clear sky
        case 2:     return &few_clouds.icon_96; break;          // few clouds
        case 3:     return &scattered_clouds.icon_96; break;    // scattered clouds
        case 4:     return &broken_clouds.icon_96; break;       // broken clouds
        case 9:     return &shower_rain.icon_96; break;         // shower rain
        case 10:    return &rain.icon_96; break;                // rain
        case 11:    return &thunderstorm.icon_96; break;        // thunderstorm
        case 13:    return &snow.icon_96; break;                // snow
        case 50:    return &mist.icon_96; break;                // mist
        default:    return &clear_sky.icon_96; break; 
    }
}
// https://darksky.net/dev/docs#response-format

void ICACHE_FLASH_ATTR timezone_db_callback(char * response_body, int http_status, char * response_headers, int body_size)
{
	os_printf("http_status=%d\n", http_status);
	if (http_status != HTTP_STATUS_GENERIC_ERROR) {
		os_printf("strlen(headers)=%d\n", strlen(response_headers));
		os_printf("body_size=%d\n", body_size);
		os_printf("body=%s<EOF>\n", response_body); // FIXME: this does not handle binary data.
        char date_time[20];
        json_getValue(response_body, TZDB_JSON_FORMATTED_DATE_TIME, date_time, 20);
    }
}
    
