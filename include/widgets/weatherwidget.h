#ifndef INC_WEATHERWIDGET_H
#define INC_WEATHERWIDGET_H

#include "os_type.h"
#include "weathericons.h"
#include "widgets/openweathermap.h"
#include "http.h"
#include "ui.h"
#include <stdlib.h>

typedef struct{
    //Current day data
    int8_t temp;
    int8_t temp_max;
    int8_t temp_min;
    const icon_t *icon;
    //Next days datas
    int8_t daynext1_temp;
    const icon_t daynext1_icon;
    
    int8_t daynext2_temp;
    const icon_t daynext2_icon;
    
    int8_t daynext3_temp;
    const icon_t daynext3_icon;
} weatherwidget_ui_t;

#endif