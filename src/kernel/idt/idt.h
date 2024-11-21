#ifndef IDT_H
#define IDT_H

#include "../system.h"

#define IDT_ENTRIES 256

typedef struct {
  u16 isr_low;    // Lower 16 bits of the ISR address
  u16 selector;   // GDT segment selector
  u8  zero;       // Always zero
  u8  attributes; // Type an attributes
  u16 isr_high;   // Higher 16 bits of the ISR address
} __attribute__((packed)) idt_entry_t;

typedef struct {
  u16 limit;
  u16 base;
} __attribute__((packed)) idtr_t;

void idt_init(void);
void idt_set_descriptor(u8 vector, void* isr, u8 flags);

extern void* isr_stub_table[];

#endif
