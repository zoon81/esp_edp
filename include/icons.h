#ifndef INC_ICONS_H
#define INC_ICONS_H

#include "user_config.h"

#define WEATHER_ICON_POS_MOSTLYCLOUDY   0
#define WEATHER_ICON_POS_SUNNY          1
#define WEATHER_ICON_POS_RAINY          2
#define WEATHER_ICON_POS_SNOWY          3

typedef struct{
    uint8_t icon_x_size;
    uint8_t icon_y_size;
    const uint32_t* icon_tables[];
} icon_t;

extern const icon_t weather_icons;

#endif