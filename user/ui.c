#include "ui.h"

void draw_char(uint8_t x, uint8_t y, uint8_t c, const font_t *font){
    int chr_offset = (c - 32) * font->font_x_size * font->font_y_size;
    os_printf("char_offset=%d", chr_offset);
    edp_setFrameMemory(x, y, x + ((font->font_x_size - 1) * 8), y + font->font_y_size , &(*font).table[chr_offset], 0);
}

void draw_string(uint8_t x, uint8_t y, uint8_t *str, const font_t *font){
    uint8_t x_index = x;
    while(*str){
        draw_char(x_index, y, *str++, font);
        x_index += font->width_in_px;
    }
}
void draw_number(uint8_t x, uint8_t y, uint8_t number, const font32_t *font){
    char buffer[4];
    uint8_t i = 0;
    uint16_t offset = 0;
    uint16_to_str(buffer, number);
    while(buffer[i]){
        int chr_offset = (buffer[i] - 48) * ((font->font_x_size * font->font_y_size) / 32);
        os_printf("char_offset=%d", chr_offset);
        edp_setFrameMemory32(x + offset, y, x + offset + font->font_x_size, y + font->font_y_size, &(*font).table[chr_offset], 1);
        i++;
        offset += font->width_in_px;
    }
}

void draw_icon(uint8_t x, uint8_t y, const icon_t *icons){
    os_printf("draw_icon");
    edp_setFrameMemory32(x, y, x + icons->icon_x_size, y + icons->icon_y_size, icons->icon_tables, 0);
}