#include "ports.c"
#include "../kernel/types.c"

#define VGA_VIDEO_MEMORY 0xA0000
#define VGA_COLOUR_REGISTER_PORT 0x3C8
#define VGA_COLOUR_DATA_PORT 0x3C9

/**
 * @brief Sets a colour in the VGA palette
 *
 * This function sends the RGB color components to the specified
 * palette index in VGA mode 13h.
 *
 * @param index The palette index to change (0-255).
 * @param red The red color component (0-255).
 * @param green The green color component (0-255).
 * @param blue The blue color component (0-255).
 */
void set_palette_color(uint8 index, uint8 red, uint8 green, uint8 blue)
{
    port_byte_out(VGA_COLOUR_REGISTER_PORT, index);
    port_byte_out(VGA_COLOUR_DATA_PORT, red / 4);
    port_byte_out(VGA_COLOUR_DATA_PORT, green / 4);
    port_byte_out(VGA_COLOUR_DATA_PORT, blue / 4);
}

/**
 * @brief Writes a pixel to the VGA video memory at a specified offset.
 *
 * This function sets the color of a pixel in VGA mode by writing
 * a color index to the given offset in video memory.
 *
 * @param offset The offset in video memory where the pixel will be written.
 *               This offset corresponds to the position on the screen.
 * @param colourIndex The color index to write to the offset, representing
 *                    the pixel's color in the 256-color palette.
 */
void write_pixel(uint16 offset, uint8 colourIndex)
{
    uint8 *vgaMemory = (uint8 *)VGA_VIDEO_MEMORY;
    vgaMemory[offset] = colourIndex;
}