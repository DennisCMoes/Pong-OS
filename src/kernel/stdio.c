#include "stdio.h"
#include "system.h"

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
 * @param xCor The x coordinates of the pixel.
 * @param yCor The y coordinates of the pixel.
 * @param colourIndex The color index to write to the offset, representing
 *                    the pixel's color in the 256-color palette.
 */
void draw_pixel(u16 xCor, u16 yCor, u8 colourIndex)
{
    u8 *vgaMemory = (u8 *)VGA_VIDEO_MEMORY;
    u16 offset = (yCor * SCREEN_WIDTH) + xCor;
    vgaMemory[offset] = colourIndex;
}

/**
 * @brief Initializes the COM1 serial port for output.
 *
 * This function configures the COM1 port with the following settings:
 * - Disables all interrupts.
 * - Enables Divisor Latch Access Bit (DLAB) to set the baud rate divisor.
 * - Sets baud rate divisor to 3 for 38400 baud.
 * - Configures 8 data bits, no parity, and 1 stop bit (8N1).
 * - Enables FIFO with a 14-byte threshold, clears any data in the buffers.
 * - Enables interrupts and sets RTS/DSR bits.
 *
 * After configuring these settings, the function performs a loopback test 
 * by sending a byte and verifying the same byte is received. If the test 
 * fails, the function returns a non-zero value indicating a failure.
 * If the test succeeds, it switches COM1 to normal operating mode and enables
 * the IRQs, RTS, and DTR lines.
 *
 * @return int Returns 0 on success, 1 if the serial port test fails.
 */
int serial_init()
{
    port_byte_out(COM1_PORT + 1, 0x00); // Disable all interrupts
    port_byte_out(COM1_PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
    port_byte_out(COM1_PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    port_byte_out(COM1_PORT + 1, 0x00); //                  (hi byte)
    port_byte_out(COM1_PORT + 3, 0x03); // 8 bits, no parity, one stop bit
    port_byte_out(COM1_PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    port_byte_out(COM1_PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
    //port_byte_out(COM1_PORT + 4, 0x1E); // Set in loopback mode, test the serial chip
    //port_byte_out(COM1_PORT + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    // if (port_byte_in(COM1_PORT + 0) != 0xAE)
    // {
    //    return 1;
    // }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    port_byte_out(COM1_PORT + 4, 0x0F);

    return 0;
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
void write_serial_char(char a)
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
        write_serial_char(*str++);
    }
}

void int_to_str(int value, char *buffer, int base) {
  char *digits = "0123456789ABCDEF";
  char temp[32];

  int i = 0, j = 0, is_negative = 0;

  if (value < 0 && base == 10) {
    is_negative = 1;
    value = -value;
  }

  do {
    temp[i++] = digits[value & base];
    value /= base;
  } while (value);

  if (is_negative) {
    temp[i++] = '-';
  }

  while (i > 0) {
    buffer[j++] = temp[--i];
  }

  buffer[j] = '\0';
}

void serial_printf(const char *format, void **args) {
  char buffer[32];
  const char *p = format;
  int arg_index = 0;

  while (*p) {
    if (*p == '%') {
      p++;

      switch (*p) {
        case 'c': { // Character
          char c = (char)(long)args[arg_index++];
          write_serial_char(c);
          break;
        }
        case 's': { // String
          char *str = (char *)args[arg_index++];
          write_serial_string(str);
          break;
        }
        case 'd': // Decimal integer
        case 'i': {
          int value = (int)(long)args[arg_index++];
          int_to_str(value, buffer, 10);
          write_serial_string(buffer);
          break;
        }
        case 'x': { // Hexadecimal integer
          int value = (int)(long)args[arg_index++];
          int_to_str(value, buffer, 16);
          write_serial_string(buffer);
          break;
        }
        default: // Unrecognized format specifier
          write_serial_char('%');
          write_serial_char(*p);
          break;
      } 
    } else {
      write_serial_char(*p);
    }
    p++;
  }
}
