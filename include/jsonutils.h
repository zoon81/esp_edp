#ifndef _JSONUTILS_H_
#define _JSONUTILS_H_

#include "osapi.h"
#include "os_type.h"
#include "mem.h"
#include "json/jsonparse.h"

#define JSON_TYPE_OBJECT_END '}'
#define JSON_TYPE_COMMA      ','

uint8_t ICACHE_FLASH_ATTR json_getValue(char *json, char *searchKey, char *result, uint16_t resultLen);

#endif