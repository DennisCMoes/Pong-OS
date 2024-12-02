#include "isr.h"
#include "../stdio.h"

// __attribute__((noreturn))
void exception_handler(void) {
  write_serial_string("Exception occurred!\n");
  __asm__ __volatile__ ("cli; hlt"); // Hang the computer
}

void divide_by_zero_exception() {
  write_serial_string("Divide by zero exception\n");
  exception_handler();
}

void debug(void) {
  write_serial_string("Debug exception\n");
  exception_handler();
}

void non_maskable_interrupt(void) {
  write_serial_string("Non-maskable interrupt\n");
  exception_handler();
}

void breakpoint(void) {
  write_serial_string("Breakpoint\n");
  exception_handler();
}

void overflow(void) {
  write_serial_string("Overflow\n");
  exception_handler();
}

void bound_range_exceeded(void) {
  write_serial_string("Bound Range Exceeded\n");
  exception_handler();
}

void invalid_opcode(void) {
  write_serial_string("Invalid Optcode\n");
  exception_handler();
}

void device_not_available(void) {
  write_serial_string("Device Not Available\n");
  exception_handler();
}

void double_fault(void) {
  write_serial_string("Double Fault\n");
  exception_handler();
}

void invalid_tss(void) {
  write_serial_string("Invalid TSS\n");
  exception_handler();
}

void segment_not_present(void) {
  write_serial_string("Segment Not Present\n");
  exception_handler();
}

void stack_segment_fault(void) {
  write_serial_string("Stack-Segment Fault\n");
  exception_handler();
}

void general_protection(void) {
  write_serial_string("General Protection Fault\n");
  exception_handler();
}

void page_fault(void) {
  write_serial_string("Page Fault\n");
  exception_handler();
}

void x87_fpu_error(void) {
  write_serial_string("x87 Floating-Point Exception\n");
  exception_handler();
}

void alignment_check(void) {
  write_serial_string("Alignment Check\n");
  exception_handler();
}

void machine_check(void) {
  write_serial_string("Machine Check\n");
  exception_handler();
}

void simd_error(void) {
  write_serial_string("SIMD Floating-Point Exception\n");
  exception_handler();
}

