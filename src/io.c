#include "io.h"
#include "system.h"

#define COM1_PORT 0x3F8

int serial_init() {
    port_byte_out(COM1_PORT + 1, 0x00); // Disable all interrupts
    port_byte_out(COM1_PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
    port_byte_out(COM1_PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    port_byte_out(COM1_PORT + 1, 0x00); //                  (hi byte)
    port_byte_out(COM1_PORT + 3, 0x03); // 8 bits, no parity, one stop bit
    port_byte_out(COM1_PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    port_byte_out(COM1_PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
                                        //
    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    port_byte_out(COM1_PORT + 4, 0x0F);

    return 0;
}

int is_transmit_empty() {
    return port_byte_in(COM1_PORT + 5) & 0x20;
}

void write_serial_char(char a) {
    while (is_transmit_empty() == 0);
    port_byte_out(COM1_PORT, a);
}

void write_serial_string(const char *str) {
   while (*str) {
       write_serial_char(*str++);
   } 
}
