#include "isr.h"
#include "../stdio.h"

// __attribute__((noreturn))
void exception_handler(void) {
  __asm__ volatile ("cli; hlt"); // Hang the computer
}

void divide_by_zero_exception() {
  write_serial_string("Divide by zero exception");
  exception_handler();
}

void debug(void) {
  write_serial_string("Debug exception");
  exception_handler();
}

void non_maskable_interrupt(void) {
  write_serial_string("Non-maskable interrupt");
  exception_handler();
}

void breakpoint(void) {
  write_serial_string("Breakpoint");
  exception_handler();
}

void overflow(void) {
  write_serial_string("Overflow");
  exception_handler();
}

void bound_range_exceeded(void) {
  write_serial_string("Bound Range Exceeded");
  exception_handler();
}

void invalid_opcode(void) {
  write_serial_string("Invalid Optcode");
  exception_handler();
}

void device_not_available(void) {
  write_serial_string("Device Not Available");
  exception_handler();
}

void double_fault(void) {
  write_serial_string("Double Fault");
  exception_handler();
}

void invalid_tss(void) {
  write_serial_string("Invalid TSS");
  exception_handler();
}

void segment_not_present(void) {
  write_serial_string("Segment Not Present");
  exception_handler();
}

void stack_segment_fault(void) {
  write_serial_string("Stack-Segment Fault");
  exception_handler();
}

void general_protection(void) {
  write_serial_string("General Protection Fault");
  exception_handler();
}

void page_fault(void) {
  write_serial_string("Page Fault");
  exception_handler();
}

void x87_fpu_error(void) {
  write_serial_string("x87 Floating-Point Exception");
  exception_handler();
}

void alignment_check(void) {
  write_serial_string("Alignment Check");
  exception_handler();
}

void machine_check(void) {
  write_serial_string("Machine Check");
  exception_handler();
}

void simd_error(void) {
  write_serial_string("SIMD Floating-Point Exception");
  exception_handler();
}

