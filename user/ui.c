#include "ui.h"

void draw_char(uint8_t x, uint8_t y, uint8_t c, const uint8_t *font){
    int chr_offset = (c - 32) * 8;
    edp_setFrameMemory(x, y, x, y + 8 /*char width*/, &font[chr_offset], 1);
}

void draw_string(uint8_t x, uint8_t y, uint8_t *str, const uint8_t *font){
    uint8_t x_index = x;
    while(*str){
        draw_char(x_index, y, *str++, font);
        x_index +=8;
    }
}