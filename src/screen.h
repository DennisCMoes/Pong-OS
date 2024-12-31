#ifndef SCREEN_H
#define SCREEN_H

#include "system.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

#define SCREEN_CENTER_X (SCREEN_WIDTH / 2)
#define SCREEN_CENTER_Y (SCREEN_HEIGHT / 2)

extern u8 screen_buffers[2][SCREEN_SIZE];
extern u8 screen_back;

#define screen_buffer() (_screen_buffers[_screen_back])
#define screen_offset(x, y) ((y) * SCREEN_WIDTH + (x))
#define screen_set(x, y, colour) (screen_buffers[screen_back][screen_offset(x, y)]=colour)

void screen_swap();
void screen_clear(u8 color);

#endif
