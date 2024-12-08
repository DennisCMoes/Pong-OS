#ifndef IRQ_H
#define IRQ_H

#include "system.h"
#include "isr.h"

// Function to install a handler for a specific IRQ
void irq_install(size_t i, void (*handler)(struct Registers*));

// Function to initialize the IRQs and set up remapping
void irq_init();

#endif
