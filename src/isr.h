#ifndef ISR_H
#define ISR_H

#include "system.h"

// Holds all CPU registers that get pushed onto the stack when an interrupt occurs
struct Registers {
    u32 __ignored;  // Placeholder to align the structure 
    u32 fs, es, ds; // Segment registers
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // General-purpose registers
    u32 int_no, err_no; // Interrupt number and error code
    u32 eip, cs, efl, useresp, ss; // Processor state when the interrupt occurred
};

// Function to install an interrupt service routine (ISR) handler for a specific interrupt
void isr_install(size_t i, void (*handler)(struct Registers*));

// Function to intiialize all ISRs
void isr_init();

#endif
