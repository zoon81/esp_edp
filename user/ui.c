#include "ui.h"

void draw_char(uint8_t x, uint8_t y, uint8_t c, const font_t* font){
    int chr_offset = (c - 32) * font->font_x_size * font->font_y_size;
    os_printf("char_offset=%d", chr_offset);
    edp_setFrameMemory(x, y, x + ((font->font_x_size - 1) * 8), y + font->font_y_size , &(*font).table[chr_offset], 0);
}

void draw_string(uint8_t x, uint8_t y, uint8_t *str, const font_t* font){
    uint8_t x_index = x;
    while(*str){
        draw_char(x_index, y, *str++, font);
        x_index += font->width_in_px;
    }
}