#ifndef FONT_H
#define FONT_H

#include "system.h"
#include "screen.h"

void font_char(u8 character, size_t xCor, size_t yCor, u8 colour);
void font_str(const char *str, size_t xCor, size_t yCor, u8 colour);

#endif
