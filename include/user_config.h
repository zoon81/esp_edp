#ifndef INC_USER_CONFIG_H
#define INC_USER_CONFIG_H


#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include <gpio.h>
#include <os_type.h>
#ifdef ENABLE_GDB
	#include "gdbstub.h"
#endif

#include "HAL/spi.h"
#include "driver/edp.h"
#include "driver/fs.h"
#include "fonts.h"
#include "widgets/weathericons.h"
#include "ui.h"
#include "http.h"

void some_timerfunc(void *arg);
void user_init();

#endif