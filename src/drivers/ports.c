#include "../system.h"

/**
 * @brief Reads a byte (8 bits) from the specified I/O port.
 * 
 * This function uses inline assembly to execute the `in` instruction, 
 * reading an 8-bit value from the given port address.
 * 
 * @param port the I/O port to read from.
 * @return The byte read from the specified port
 */
byte port_byte_in(word port) {
    byte result;
    // "=a" (result): output constraint, stores result in variable 'result'
    // "d" (port): input constraint, loads the port address into DX
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
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
void port_byte_out(word port, byte data) {
    // "a" (data): input constraint, loads 'data' into AL
    // "d" (port): input constraint, loads the port address into DX
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
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
word port_word_in(word port) {
    word result;
    // "=a" (result): output constraint, stores result in variable 'result'
    // "d" (port): input constraint, loads the port address into DX
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
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
void port_word_out(word port, word data) {
    // "a" (data): input constraint, loads 'data' into AX
    // "d" (port): input constraint, loads the port address into DX
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}