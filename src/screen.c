#include "screen.h"

static u8 *BUFFER = (u8 *)0xA0000;

// Double buffers for screen
u8 screen_buffers[2][SCREEN_SIZE];
u8 screen_back = 0;

#define CURRENT (screen_buffers[screen_back])
#define SWAP() (screen_back = 1 - screen_back)

void screen_swap() {
    memcpy(BUFFER, CURRENT, SCREEN_SIZE);
    SWAP();
}

void screen_clear(u8 color) {
    memset(&CURRENT, color, SCREEN_SIZE);
}

void screen_init() {}
