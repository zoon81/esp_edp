#ifndef INC_WEATHERICONS_H
#define INC_WEATHERICONS_H

#include "os_type.h"

#define RODATA_ATTR  __attribute__((section(".irom.text"))) __attribute__((aligned(4)))
#define WEATHER_ICON_COLLECTION_ICON_BIG 0

// This type is represent an icon
typedef struct{
    uint8_t icon_x_size;
    uint8_t icon_y_size;
    const uint32_t* icon_tables;
} icon_t;

// This type represent an icon collection which is including a bigger icon and a smaller one
typedef struct {
    icon_t icon_96;
    icon_t icon_48;
} weather_icons_collection_t;

// Weather icons
extern const weather_icons_collection_t clear_sky;
extern const weather_icons_collection_t few_clouds;
extern const weather_icons_collection_t scattered_clouds;
extern const weather_icons_collection_t broken_clouds;
extern const weather_icons_collection_t shower_rain;
extern const weather_icons_collection_t rain;
extern const weather_icons_collection_t hail;
extern const weather_icons_collection_t thunderstorm;
extern const weather_icons_collection_t snow;
extern const weather_icons_collection_t mist;
extern const icon_t celsius_symbol;

#endif