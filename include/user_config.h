#ifndef INC_USER_CONFIG_H
#define INC_USER_CONFIG_H


#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "gpio.h"
#include "os_type.h"
#include "HAL/spi.h"
#include "driver/edp.h"
#include "fonts.h"
#include "weathericons.h"
#include "ui.h"
#include "http.h"

void some_timerfunc(void *arg);
void ICACHE_FLASH_ATTR user_init();

#endif