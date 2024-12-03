#ifndef IO_H
#define IO_H

int serial_init();
int is_transmit_empty();

void write_serial_char(char a);
void write_serial_string(const char *str);

#endif
