#ifndef FONT_H
#define FONT_H

#include "system.h"
#include "screen.h"

#define font_str_doubled(_str, _x, _y, _primary, _secondary)\
  do {\
    font_str((_str), (_x) + 1, (_y) + 1, _secondary); \
    font_str((_str), (_x), (_y), _primary); \
  } while (0)

void font_char_scaled(u8 character, size_t xCor, size_t yCor, u8 colour, u8 scale);
void font_str_scaled(const char *str, size_t xCor, size_t yCor, u8 colour, u8 scale);
void font_char(u8 character, size_t xCor, size_t yCor, u8 colour);
void font_str(const char *str, size_t xCor, size_t yCor, u8 colour);

#endif
