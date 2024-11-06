#include <stdio.h>

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
void set_palette_color(u8 index, u8 red, u8 green, u8 blue)
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
void write_pixel(u16 offset, u8 colourIndex)
{
    u8 *vgaMemory = (u8 *)VGA_VIDEO_MEMORY;
    vgaMemory[offset] = colourIndex;
}