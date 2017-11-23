#ifndef INC_OPENWEATHERMAP_H
#define INC_OPENWEATHERMAP_H

#include "os_types.h"
#include "weathericons.h"

#define CITY_ID_BUDAPEST_IX 7284825
#define API_KEY 6061903e424eba6767361c83272ac8e7


typedef struct {
    const uint32_t                      icon_name[];
    const weathericon_collections_t     *icon_bitmap;
} openwm_icons_t;

// Openweathermap.com icon name mapping for my icons
openwm_icons_t lookUpTable [] = {
                {"01d", &clear_sky           },
                {"02d", &few_clouds          },
                {"03d", &scattered_clouds    }
};

#endif