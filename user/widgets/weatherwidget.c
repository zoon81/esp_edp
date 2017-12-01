#include "widgets/weatherwidget.h"
// Main source code for the weather widget

weatherwidget_ui_t ui;

void ICACHE_FLASH_ATTR owm_getWeatherinfo(uint16_t cityID){
    char url_current[] = OWM_WEATHER_BASEURL OWM_CITY_ID_BUDAPEST_IX OWM_API_KEY OWM_METRIC_UNIT;
    http_get(url_current, "", &owm_http_callback_current);  //what happend if no internet access?
}

void ICACHE_FLASH_ATTR weatherwidget_updateUI(){
    draw_number(WW_POS_CURRENT_TEMP_X,WW_POS_CURRENT_TEMP_Y, ui.temp, &numbers72, UI_SHOW_SIGN);
    draw_icon(WW_POS_CELSIUS_X, WW_POS_CELSIUS_Y, &celsius_symbol);
    draw_number(WW_POS_CURRENT_MIN_TEMP_X,WW_POS_CURRENT_MIN_TEMP_Y,ui.temp_max, &numbers24, UI_SHOW_SIGN);
    draw_number(WW_POS_CURRENT_MAX_TEMP_X,WW_POS_CURRENT_MAX_TEMP_Y,ui.temp_min, &numbers24, UI_SHOW_SIGN);
    draw_icon(WW_POS_CURRENT_ICON_X,WW_POS_CURRENT_ICON_Y, ui.icon);
    draw_icon(WW_POS_DAY1_ICON_X,WW_POS_DAY1_ICON_Y, ui.day1_icon);
    draw_number(WW_POS_DAY1_MAX_TEMP_X, WW_POS_DAY1_MAX_TEMP_Y, ui.day1_temp_max, &numbers24, UI_SHOW_SIGN);
   // draw_number(WW_POS_DAY1_MIN_TEMP_X, WW_POS_DAY1_MIN_TEMP_Y, ui.day1_temp_min, &numbers24, UI_SHOW_SIGN);
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
    http_get(TZDB_BASEURL TZDB_JSONFORMAT TZDB_BY TZDB_LAT BP_LAT TZDB_LNG BP_LNG TZDB_FIELDS, "", &timezone_db_callback);

	}
}

const icon_t* ICACHE_FLASH_ATTR getIconByID(uint8_t icon_id){
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
        char timestamp_str[15];
        json_getValue(response_body, TZDB_JSON_TIMESTAMP, timestamp_str, 15);
        uint32_t timestamp = atoi(timestamp_str);
        os_printf("TS:%d",timestamp);
        char url[170] = {0};
        darkskynet_url_build(BP_LAT, BP_LNG, timestamp, 1, url);
        http_get(url, "", &darkskynet_callback_day1);
    }
    
}
void ICACHE_FLASH_ATTR darkskynet_callback_day1(char * response_body, int http_status, char * response_headers, int body_size)
{
	os_printf("http_status=%d\n", http_status);
	if (http_status != HTTP_STATUS_GENERIC_ERROR) {
		os_printf("strlen(headers)=%d\n", strlen(response_headers));
		os_printf("body_size=%d\n", body_size);
		os_printf("body=%s<EOF>\n", response_body); // FIXME: this does not handle binary data.
        char tmp[10];
        json_getValue(response_body, DS_JSON_TEMPMAX, tmp, 10);
        os_printf("day1max:%s",tmp);
        ui.day1_temp_max = atoi(tmp);
        os_printf("day1max:%d",ui.day1_temp_max);
        json_getValue(response_body, DS_JSON_TEMPMIN, tmp, 10);
        os_printf("day1min:%s",tmp);
        ui.day1_temp_min = atoi(tmp);
        os_printf("day1min:%d",ui.day1_temp_min);
        json_getValue(response_body, DS_JSON_ICON, tmp, 4);
        ds_get_icon_by_id(tmp);
        os_printf("day1icon:%s",tmp);
        weatherwidget_updateUI();
    }
}
void ICACHE_FLASH_ATTR ds_get_icon_by_id(char *weather_icon){
    if(! (strcmp(DS_JSON_ICON_CLEAR_SKY, weather_icon))){
        ui.day1_icon = &clear_sky.icon_48;

    }else if(!(strcmp(DS_JSON_ICON_RAIN, weather_icon))){
        ui.day1_icon = &rain.icon_48;

    }else if(!(strcmp(DS_JSON_ICON_CLOUDY, weather_icon))){
        ui.day1_icon = &scattered_clouds.icon_48;

    }else if(!(strcmp(DS_JSON_ICON_FOG, weather_icon))){
        ui.day1_icon = &mist.icon_48;

    }else if(!(strcmp(DS_JSON_ICON_HAIL, weather_icon))){
        ui.day1_icon = &hail.icon_48;

    }else if(!(strcmp(DS_JSON_ICON_PARTLY_CLOUDY, weather_icon))){
        ui.day1_icon = &few_clouds.icon_48;

    }else if(!(strcmp(DS_JSON_ICON_SLEET, weather_icon))){
        ui.day1_icon = &snow.icon_48;

    }else if(!(strcmp(DS_JSON_ICON_SNOW, weather_icon))){
        ui.day1_icon = &snow.icon_48;
        
    }else if(!(strcmp(DS_JSON_ICON_TUNDERSSTORM, weather_icon))){
        ui.day1_icon = &thunderstorm.icon_48;
        
    }else if(!(strcmp(DS_JSON_ICON_TORNADO, weather_icon))){        //missing icon
        ui.day1_icon = &thunderstorm.icon_48;
        
    }else if(!(strcmp(DS_JSON_ICON_WIND, weather_icon))){           //missing too
        ui.day1_icon = &thunderstorm.icon_48;
        
    }else {
        ui.day1_icon = &thunderstorm.icon_48;                       //default
    }
}
void ICACHE_FLASH_ATTR darkskynet_url_build(char *lat, char *lng, uint32_t unixtime, uint8_t day_offset, char *url){
    unixtime += day_offset * 60 * 60 * 24; //get next days in second
    os_printf("TS+:%d", unixtime);
    char time[12];
    uint_to_str(time, unixtime);
    os_printf("TS+:%s", time);
    strcat(url , DS_BASEURL);
    strcat(url , lat);
    strcat(url , ",");
    strcat(url , lng);
    strcat(url , ",");
    strcat(url , time);
    strcat(url, DS_OPTS);
    os_printf("DSURL:%s", url);
}

    
