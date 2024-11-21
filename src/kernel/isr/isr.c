#include "isr.h"
#include "../stdio.h"

// __attribute__((noreturn))
void exception_handler() {
  __asm__ volatile ("cli; hlt"); // Hang the computer
}

void divide_by_zero_exception() {
  write_serial_string("Divide by zero exception");
  exception_handler();
}
