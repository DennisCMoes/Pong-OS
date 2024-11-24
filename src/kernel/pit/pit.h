#ifndef PIT_H
#define PIT_H

#include "../system.h"
#include "../pic/pic.h"

#define PIT_CHANNEL_0   0x40 // Channel 0 data port
#define PIT_COMMAND     0x43 // Command port
// #define PIC1_COMMAND    0xA0
#define PIT_FREQUENCY   1193182

extern volatile u32 tick;

/**
 * 
 * Bits         Usage
 * 6 and 7      Select channel :
 *               0 0 = Channel 0
 *               0 1 = Channel 1
 *               1 0 = Channel 2
 *               1 1 = Read-back command (8254 only)
 * 4 and 5      Access mode :
 *               0 0 = Latch count value command
 *               0 1 = Access mode: lobyte only
 *               1 0 = Access mode: hibyte only
 *               1 1 = Access mode: lobyte/hibyte
 * 1 to 3       Operating mode :
 *               0 0 0 = Mode 0 (interrupt on terminal count)
 *               0 0 1 = Mode 1 (hardware re-triggerable one-shot)
 *               0 1 0 = Mode 2 (rate generator)
 *               0 1 1 = Mode 3 (square wave generator)
 *               1 0 0 = Mode 4 (software triggered strobe)
 *               1 0 1 = Mode 5 (hardware triggered strobe)
 *               1 1 0 = Mode 2 (rate generator, same as 010b)
 *               1 1 1 = Mode 3 (square wave generator, same as 011b)
 * 0            BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD
 */
void set_mode_register(u8 operating_mode, u8 access_mode, u8 channel);
void init_pit(u32 frequency);
void pit_isr_handler();

#endif
