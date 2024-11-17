#include "system.h"

/**
 * @brief Reads a byte (8 bits) from the specified I/O port.
 *
 * This function uses inline assembly to execute the `in` instruction,
 * reading an 8-bit value from the given port address.
 *
 * @param port the I/O port to read from.
 * @return The byte read from the specified port
 */
byte port_byte_in(word port)
{
    byte result;
    // "=a" (result): output constraint, stores result in variable 'result'
    // "d" (port): input constraint, loads the port address into DX
    __asm__ volatile("in %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}

/**
 * @brief Writes a byte (8 bits) to the specified I/O port.
 *
 * This function uses inline assembly to execute the `out` instruction,
 * sending an 8-bit value to the given port address.
 *
 * @param port The I/O port to write to.
 * @param data The byte to send to the specified port.
 */
void port_byte_out(word port, byte data)
{
    // "a" (data): input constraint, loads 'data' into AL
    // "d" (port): input constraint, loads the port address into DX
    __asm__ volatile("out %%al, %%dx" : : "a"(data), "d"(port));
}

/**
 * @brief Reads a word (16 bits) from the specified I/O port.
 *
 * This function uses inline assembly to execute the `in` instruction,
 * reading a 16-bit value from the given port address.
 *
 * @param port The I/O port to read from.
 * @return The word read from the specified port.
 */
word port_word_in(word port)
{
    word result;
    // "=a" (result): output constraint, stores result in variable 'result'
    // "d" (port): input constraint, loads the port address into DX
    __asm__ volatile("in %%dx, %%ax" : "=a"(result) : "d"(port));
    return result;
}

/**
 * @brief Writes a word (16 bits) to the specified I/O port.
 *
 * This function uses inline assembly to execute the `out` instruction,
 * sending a 16-bit value to the given port address.
 *
 * @param port The I/O port to write to.
 * @param data The word to send to the specified port.
 */
void port_word_out(word port, word data)
{
    // "a" (data): input constraint, loads 'data' into AX
    // "d" (port): input constraint, loads the port address into DX
    __asm__ volatile("out %%ax, %%dx" : : "a"(data), "d"(port));
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
    if (port_byte_in(COM1_PORT + 0) != 0xAE)
    {
       return 1;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    port_byte_out(COM1_PORT + 4, 0x0F);
    return 0;
}
