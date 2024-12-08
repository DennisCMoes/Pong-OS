#include "irq.h"
#include "isr.h"
#include "idt.h"
#include "system.h"

// PIC ports and settings
#define PIC1 0x20             // Master PIC command port
#define PIC1_OFFSET 0x20      // Offset for IRQs 0-7
#define PIC1_DATA (PIC1 + 1)  // Master PIC data port

#define PIC2 0xA0             // Slave PIC command port
#define PIC2_OFFSET 0x28      // Offset for IRQs 8-15
#define PIC2_DATA (PIC2 + 1)  // Slave PIC data port

#define PIC_EOI 0x20          // End of interrupt command
#define PIC_MODE_8086 0x01    // 8086 mode
#define ICW1_ICW4 0x01        // ICW4 (Initialization Command Word) present
#define ICW1_INIT 0x10        // Initialization command
//
// Wait macro to wait for PIC commands
#define PIC_WAIT() do { \
  asm (                 \
      "jmp 1f\n"        \
      "1: jmp 2f\n"     \
      "2:");            \
} while (0)

// Handler table for IRQs
static void (*handlers[32])(struct Registers *regs) = { 0 };

// Function that gets called when an IRQ occurs and checks if the handler exists
static void stub(struct Registers *regs) {
  if (regs->int_no <= 47 && regs->int_no >= 32) {
    size_t irq_no = regs->int_no - 32;

    if (handlers[irq_no]) {
      handlers[irq_no](regs);
    }
  }

  // Send End of Interrupt (EOI) to the PICs
  if (regs->int_no >= 0x40) {
    port_byte_out(PIC2, PIC_EOI);
  }

  port_byte_out(PIC1, PIC_EOI);
}

// Remaps the PIC to avoid conflicts with the CPU exceptions
static void irq_remap() {
  // Save the current IRQ masks
  u8 mask1 = port_byte_in(PIC1_DATA);
  u8 mask2 = port_byte_in(PIC2_DATA);

  port_byte_out(PIC1, 0x11);
  port_byte_out(PIC2, 0x11);
  port_byte_out(PIC1_DATA, PIC1_OFFSET);
  port_byte_out(PIC2_DATA, PIC2_OFFSET);
  port_byte_out(PIC1_DATA, 0x04); // PIC2 at IRQ2
  port_byte_out(PIC2_DATA, 0x02); // Cascade indentity
  port_byte_out(PIC1_DATA, PIC_MODE_8086);
  port_byte_out(PIC1_DATA, PIC_MODE_8086);
  port_byte_out(PIC1_DATA, mask1);
  port_byte_out(PIC2_DATA, mask2);
}

// Masks (disables) an IRQ line
static void irq_set_mask(size_t i) {
  u16 port = i < 8 ? PIC1_DATA : PIC2_DATA;
  u8 value = port_byte_in(port) | (1 << (i % 8));
  port_byte_out(port, value);
}

// Unmasks (enables) an IRQ line
static void irq_clear_mask(size_t i) {
  u16 port = i < 8 ? PIC1_DATA : PIC2_DATA;
  u8 value = port_byte_in(port) & ~(1 << i);
  port_byte_out(port, value);
}

// Installs a custom handler for a specific IRQ
void irq_install(size_t i, void (*handler)(struct Registers *)) {
  CLI();
  handlers[i] = handler;
  irq_clear_mask(i);
  STI();
}

// Initializes the IRQ system by remapping and setting the defaut handlers
void irq_init() {
  irq_remap();

  for (size_t i = 0; i < 16; i++) {
    isr_install(32 + i, stub);
  }
}
