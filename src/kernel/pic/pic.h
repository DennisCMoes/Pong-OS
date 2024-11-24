#ifndef PIC_H
#define PIC_H

#include "../system.h"

#define PIC1_COMMAND  0x20 // Master PIC command port
#define PIC1_DATA     0x21 // Master PIC data port
#define PIC2_COMMAND  0xA0 // Slave PIC command port
#define PIC2_DATA     0xA1 // Slave PIC data port

// Initialization Control Words (ICW)
#define ICW1_INIT     0x10 // Initialization command
#define ICW1_ICW4     0x01 // Expect ICW4
#define ICW4_8086     0x01 // 8086/88 mode

void init_pic();

#endif
