#include "idt.h"

__attribute__((aligned(0x10)))
idt_entry_t idt[IDT_ENTRIES];
idtr_t idtr;

static int vectors[IDT_ENTRIES];

void idt_init() {
  idtr.base = (u32)&idt[0];
  idtr.limit = (u16)sizeof(idt_entry_t) * IDT_ENTRIES - 1;

  for (u8 vector = 0; vector < 32; vector++) {
    idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    vectors[vector] = 1;
  }

  __asm__ volatile ("lidt %0" : : "m"(idtr)); // Load the new IDT
  __asm__ volatile ("sti"); // Set the interrupt flag
}

void idt_set_descriptor(u8 vector, void* isr, u8 flags) {
  idt_entry_t* descriptor = &idt[vector];

  descriptor->isr_low     = (u32)isr & 0xFFFF;
  descriptor->selector    = 0x08; // Offset of kernel code selector in GDT
  descriptor->attributes  = flags;
  descriptor->isr_high    = (u32)isr >> 16;
  descriptor->zero        = 0;
}
