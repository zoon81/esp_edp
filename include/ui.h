#ifndef INC_UI_H
#define INC_UI_H
#include "user_config.h"

void draw_char(uint8_t x, uint8_t y, uint8_t c, const font_t* font);
void draw_string(uint8_t x, uint8_t y, uint8_t *str, const font_t* font);

#endif