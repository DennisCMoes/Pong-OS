#include "io.h"
#include "irq.h"
#include "screen.h"
#include "isr.h"
#include "idt.h"
#include "timer.h"

static u8 *BUFFER = (u8 *)0xA0000;
static u32 colourIndex = 0;

void update_rainbow() {
    for (u32 yCor = 0; yCor < SCREEN_HEIGHT; yCor++) {
        for (u32 xCor = 0; xCor < SCREEN_WIDTH; xCor++) {
            screen_set(xCor, yCor, (colourIndex + xCor + yCor) % 256);
        }
    }

    screen_swap();
    colourIndex++;
    if (colourIndex == 256) {
        colourIndex = 0;
    }
}

static void rainbow_timer_handler(struct Registers *regs) {
    update_rainbow();
}

int _main() {
    serial_init();
    idt_init();
    isr_init();
    irq_init();
    timer_init();

    irq_install(0, rainbow_timer_handler);
    asm("sti");

    while (1) {
        
    }

    return 0;
}
