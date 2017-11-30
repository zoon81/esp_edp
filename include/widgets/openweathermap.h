#ifndef INC_OPENWEATHERMAP_H
#define INC_OPENWEATHERMAP_H

#include "os_type.h"
#include "weathericons.h"

#define OWM_WEATHER_BASEURL             "http://api.openweathermap.org/data/2.5/weather?"
#define OWM_CITY_ID_BUDAPEST_IX         "id=7284825"
#define OWM_API_KEY                     "&appid=6061903e424eba6767361c83272ac8e7"
#define OWM_METRIC_UNIT                 "&units=metric"
// Current weather data
#define OWM_JSON_KEY_CURRENT_TEMPERATURE        ":main:temp"
#define OWM_JSON_KEY_CURRENT_TEMPMAX            ":main:temp_max"
#define OWM_JSON_KEY_CURRENT_TEMPMIN            ":main:temp_min"
#define OWM_JSON_KEY_CURRENT_WEATER_ID          ":weather:id"
#define OWM_JSON_KEY_CURRENT_WEATER_MAIN        ":weather:main"
#define OWM_JSON_KEY_CURRENT_WEATER_DESC        ":weather:description"
#define OWM_JSON_KEY_CURRENT_WEATER_ICON        ":weather:icon"
// Forecast data
//https://api.darksky.net/forecast/74059380d075a80395c17900caa2dab0/47.4607,19.1148,2017-11-28T15:00:00?units=si&lang=hu&exclude=currently,minutely,hourly,alerts,flags
#define DS_BASEURL          "https://api.darksky.net/forecast/74059380d075a80395c17900caa2dab0/"
#define DS_LOCATION         "47.4607,19.1148"
#define DS_TIME             ",2017-11-28T15:00:00?"
#define DS_OPTIONS           "?units=si&lang=hu&exclude=currently,minutely,hourly,alerts,flags"
#define DS_JSON_KEY_TEMPMAX ":daily:data:temperatureMax"
#define DS_JSON_KEY_TEMPMIN ":daily:data:temperatureMin"
#define DS_JSON_KEY_ICON    ":daily:data:icon"

// Openweathermap.com icon name mapping for my icons (this is an ordered list)
extern const weather_icons_collection_t *owm_icon_map[];

void owm_getWeatherinfo(uint16_t cityID);
void ICACHE_FLASH_ATTR owm_http_callback(char * response_body, int http_status, char * response_headers, int body_size);

#endif