#ifndef STDIO_H
#define STDIO_H

#include "system.h"

void set_palette_colour(u8 index, u8 red, u8 green, u8 blue);
void draw_pixel(u16 yCor, u16 xCor, u8 colourIndex);

int serial_init();
int is_transmit_empty();

void write_serial_char(char a);
void write_serial_string(const char* str);

void int_to_str(int value, char *buffer, int base);
void serial_printf(const char *format, void **args);

#endif
