#include "widgets/weatherwidget.h"

weatherwidget_ui_t ui;

void owm_getWeatherinfo(uint16_t cityID){
    char url[] = OWM_WEATHER_BASEURL OWM_CITY_ID_BUDAPEST_IX OWM_API_KEY OWM_METRIC_UNIT;
    http_get(url, "", &owm_http_callback);

}

void weatherwidget_updateUI(){
    
}

void ICACHE_FLASH_ATTR owm_http_callback(char * response_body, int http_status, char * response_headers, int body_size)
{
    const weather_icons_collection_t *owm_icon_map[] = {
                    &clear_sky,         // 01d
                    &few_clouds,        // 02d
                    &scattered_clouds   // 03d
    };
	os_printf("http_status=%d\n", http_status);
	if (http_status != HTTP_STATUS_GENERIC_ERROR) {
		os_printf("strlen(headers)=%d\n", strlen(response_headers));
		os_printf("body_size=%d\n", body_size);
		os_printf("body=%s<EOF>\n", response_body); // FIXME: this does not handle binary data.
    
    char owm_tmp[5];
    //get the current temperature
    json_getValue(response_body, OWM_JSON_KEY_TEMPERATURE, owm_tmp, 5);
    os_printf("temperature: %s", owm_tmp);
    ui.temp = atoi(owm_tmp);
    //get the current temperature max
    json_getValue(response_body, OWM_JSON_KEY_TEMPMAX, owm_tmp, 5);
    ui.temp_max = atoi(owm_tmp);
    //get the current temperature min
    json_getValue(response_body, OWM_JSON_KEY_TEMPMIN, owm_tmp, 5);
    ui.temp_min = atoi(owm_tmp);
    //get the weather icon
    json_getValue(response_body, OWM_JSON_KEY_WEATER_ICON, owm_tmp, 5);
    owm_tmp[2] = '\0';
    ui.icon = &((owm_icon_map[atoi(owm_tmp)])->icon_96);



	}
}
