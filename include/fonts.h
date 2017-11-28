#ifndef INC_FONTS_H
#define INC_FONTS_H
// Basic Font types and font collections definitions

#include "os_type.h"

#define RODATA_ATTR  __attribute__((section(".irom.text"))) __attribute__((aligned(4)))

typedef struct {
	uint8_t font_x_size;
	uint8_t font_y_size;
	uint8_t width_in_px;
	const uint8_t* table;
} font_t;

typedef struct {
	uint8_t font_x_size;
	uint8_t font_y_size;
	uint8_t width_in_px;
	const uint32_t* table;
} font32_t;

extern const font_t font8;
extern const font_t font16;
extern const font32_t numbers24;

#endif /* __FONTS_H */
