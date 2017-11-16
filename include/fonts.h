#ifndef INC_FONTS_H
#define INC_FONTS_H

#include "user_config.h"

typedef struct {
	uint8_t font_x_size;
	uint8_t font_y_size;
	uint8_t width_in_px;
	const uint8_t* table;
} font_t;

extern const font_t font8;
extern const font_t font16;

#endif /* __FONTS_H */
