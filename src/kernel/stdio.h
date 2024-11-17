#ifndef STDIO_H
#define STDIO_H

#include "system.h"

void set_palette_colour(u8 index, u8 red, u8 green, u8 blue);
void draw_pixel(u16 yCor, u16 xCor, u8 colourIndex);

int is_transmit_empty();
void write_serial(char a);
void write_serial_string(const char* str);

#endif
