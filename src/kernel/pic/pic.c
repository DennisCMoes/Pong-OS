#include "pic.h"

// Initialize the PIC by remapping IRQs
void init_pic() {
  u8 master_mask = port_byte_in(PIC1_DATA); // Master PIC mask
  u8 slave_mask = port_byte_in(PIC2_DATA);  // Slave PIC mask
  
  // Begin initialization
  port_byte_out(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); // Start master
  port_byte_out(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4); // Start slave

  // Remap IRQs
  port_byte_out(PIC1_DATA, 0x20); // Master PIC: IRQ0-IRQ7 -> 0x20-0x27
  port_byte_out(PIC2_DATA, 0x28); // Slave PIC: IRQ8-IRQ15 -> 0x28-0x2F

  // Configure cascade setup
  port_byte_out(PIC1_DATA, 0x04); // Tell master there is a slave PIC at IRQ2
  port_byte_out(PIC2_DATA, 0x02); // Tell slave it's cascade identity

  // Set 8086/88 mode
  port_byte_out(PIC1_DATA, ICW4_8086); // Set master to 8086 mode
  port_byte_out(PIC2_DATA, ICW4_8086); // Set slave to 8086 mode

  // Restore saved masks
  port_byte_out(PIC1_DATA, master_mask);
  port_byte_out(PIC2_DATA, slave_mask);
}
