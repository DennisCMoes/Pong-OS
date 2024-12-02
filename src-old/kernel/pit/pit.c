#include "pit.h"
#include "../stdio.h"

volatile u32 tick = 0;

static void insert_number(u8 *value, u8 number, u8 position, u8 num_bits) {
  // Create a mask for the number of bits to insert
  u8 mask = ((1 << num_bits) - 1) << position;

  // Clear the target bits
  *value &= ~mask;
  
  // Align the number to the target position and insert insert
  *value |= (number << position);
}

void set_mode_register(u8 operating_mode, u8 access_mode, u8 channel) {
  // Bit 0 = Binary mode
  u8 result = 0;
  
  // Bit 1/2/3 -> Operating mode = mode 1
  insert_number(&result, operating_mode, 1, 3);

  // Bit 4/5 -> Access mode = lobyte only
  insert_number(&result, access_mode, 4, 2);

  // Bit 6/7 -> Select channel = 1
  insert_number(&result, channel, 6, 2);
}

void init_pit(u32 frequency) {
  int divisor = PIT_FREQUENCY / frequency;

  // Bits:
  // - 00:  Channel 0
  // - 11:  Access mode: low byte, then high byte
  // - 010: Operating mode: mode 2 (rate generator)
  // - 0:   Binary mode
  port_byte_out(PIT_COMMAND, 0x36);

  // Send the divisor low byte first, then high byte
  port_byte_out(PIT_CHANNEL_0, divisor & 0xFF);         // Low byte
  port_byte_out(PIT_CHANNEL_0, (divisor >> 8) & 0xFF);  // High byte
}

void pit_isr_handler() {
  tick++;
  port_byte_out(PIC1_COMMAND, 0x20);
}
