#include "kernel/stdio.h"
#include "kernel/idt/idt.h"
#include "kernel/pic/pic.h"
#include "kernel/pit/pit.h"
#include "kernel/system.h"

extern volatile u32 tick;

int main() {
  serial_init();
  idt_init();
  init_pic();
  init_pit(60); // 60 fps

  void *args[] = {
      (void *)"Hello, OS!",
      (void *)(long)1234,
      (void *)(long)0xABCD,
      (void *)(long)'A',
      (void *)"Test string",
  };

  serial_printf("%s\n", args);
  serial_printf("Value: %d\n", args + 1);
  serial_printf("Hex: %x\n", args + 2);
  serial_printf("Char: %c\n", args + 3);
  serial_printf("String: %s\n", args + 4);

  u16 colourIndex = 0;

  u32 last_tick = tick;

  while (1) {
    // Wait for the next tick
    while (tick - last_tick < 6);

    last_tick = tick;
    
    for (u16 yCor = 0; yCor < SCREEN_HEIGHT; yCor++) {
      for (u16 xCor = 0; xCor < SCREEN_WIDTH; xCor++) {
        draw_pixel(xCor, yCor, colourIndex);
      }
    
      colourIndex++;
      if (colourIndex == 257) {
        colourIndex = 0;
      }
    }
  }
}
