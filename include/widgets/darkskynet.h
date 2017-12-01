#ifndef INC_DARKSKYNET_H
#define INC_DARKSKYNET_H
// https://api.darksky.net/forecast/74059380d075a80395c17900caa2dab0/47.4607,19.1148,2017-11-28T15:00:00?units=si&lang=hu&exclude=currently,minutely,hourly,alerts,flags
#define DS_BASEURL  "https://api.darksky.net/forecast/74059380d075a80395c17900caa2dab0/"
#define DS_UNITS    "?units=si"
#define DS_EXCLUDE  "&exclude=currently,minutely,hourly,alerts,flags"
#define DS_LANG     "&lang=hu"
#define DS_OPTS     DS_UNITS DS_EXCLUDE DS_LANG

#define DS_JSON_TEMPMAX ":daily:data:temperatureMax"
#define DS_JSON_TEMPMIN ":daily:data:temperatureMinTime"
#define DS_JSON_ICON ":daily:data:icon"

// https://darksky.net/dev/docs#response-format

#define DS_JSON_ICON_CLEAR_SKY      "cle"
#define DS_JSON_ICON_RAIN           "rai"
#define DS_JSON_ICON_SNOW           "sno"
#define DS_JSON_ICON_SLEET          "sle"
#define DS_JSON_ICON_WIND           "win"
#define DS_JSON_ICON_FOG            "fog"
#define DS_JSON_ICON_CLOUDY         "clo"
#define DS_JSON_ICON_PARTLY_CLOUDY  "par"
#define DS_JSON_ICON_HAIL           "hai"
#define DS_JSON_ICON_TUNDERSSTORM   "tun"
#define DS_JSON_ICON_TORNADO        "tor"

#endif