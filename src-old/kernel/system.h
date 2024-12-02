#ifndef SYSTEM_H
#define SYSTEM_H

// Custom types
typedef unsigned char u8;    // 8-bit unsigned integer, range: 0 to 255
typedef unsigned short u16;  // 16-bit unsigned integer, range: 0 to 65,535
typedef unsigned int u32;    // 32-bit unsigned integer, range: 0 to 4,294,967,295

typedef char byte;              // Byte type, used for 8-bit signed integer data
typedef short word;             // Word type, used for 16-bit signed integer data
typedef long dword;             // Dword type, used for 32-bit signed integer data

// System definitions for screen dimensions
#define SCREEN_WIDTH 320        // Width of the screen in pixels
#define SCREEN_HEIGHT 200       // Height of the screen in pixels

// Memory locations specific to VGA hardware
#define VGA_VIDEO_MEMORY         0xA0000    // Address for video memory
#define VGA_COLOUR_REGISTER_PORT 0x3C8      // Port address to set the color palette index
#define VGA_COLOUR_DATA_PORT     0x3C9      // Port address to set RGB values for the selected color index

#define COM1_PORT 0x3F8

byte port_byte_in(word port);
void port_byte_out(word port, byte data);

word port_word_in(word port);
void port_word_out(word port, word data);

#endif
