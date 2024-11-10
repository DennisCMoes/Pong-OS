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

/**
 * @brief Checks if the transmit buffer is empty for the COM1 serial port.
 *
 * This function reads the Line Status Register (LSR) of the COM1 port to 
 * determine if the transmit buffer is empty, indicating that the port is 
 * ready to accept a new byte for transmission.
 *
 * @return int Returns non-zero if the transmit buffer is empty; 
 *         otherwise, returns 0.
 */
int is_transmit_empty()
{
    return port_byte_in(COM1_PORT + 5) & 0x20;
}

/**
 * @brief Sends a single character to the COM1 serial port.
 *
 * This function waits until the transmit buffer is empty (ready to accept
 * a new byte), then writes the specified character to the COM1 data port.
 * 
 * @param a The character to be sent over the COM1 serial port.
 */
void write_serial(char a)
{
    while(is_transmit_empty() == 0);
    port_byte_out(COM1_PORT, a);
}

/**
 * @brief Sends a null-terminated string to the COM1 serial port.
 *
 * This function iterates over each character in the provided string and 
 * sends it to the COM1 serial port by calling `write_serial`. The function 
 * stops once it reaches the null terminator of the string.
 *
 * @param str Pointer to the null-terminated string to be sent.
 */
void write_serial_string(const char* str)
{
    while (*str)
    {
        write_serial(*str++);
    }
}
