#include "kernel/stdio.h"
#include "kernel/idt/idt.h"
#include "kernel/pic/pic.h"
#include "kernel/pit/pit.h"
#include "kernel/system.h"

extern volatile u32 tick;

int main() {
  idt_init();
  init_pic();
  init_pit(5); // 60 fps
  serial_init();

  __asm__ volatile("sti");

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

  u16 colorIndex = 0;
  u16 offset = 0;
  u8 *vga_memory = (u8 *)VGA_VIDEO_MEMORY;

  u32 last_tick = tick;

  while (1) {
    // Wait for the next tick
    while (tick - last_tick < 1);

    last_tick = tick;

    // write_serial_string("NEW LINE\n");

    for (u16 yCor = 0; yCor < SCREEN_HEIGHT; yCor++) {
      for (u16 xCor = 0; xCor < SCREEN_WIDTH; xCor++) {
        u16 offset = (yCor * SCREEN_WIDTH) + xCor;
        vga_memory[offset] = colorIndex + offset;
      }

      colorIndex++;
      if (colorIndex == 257) {
        colorIndex = 0;
      }
    }

    offset++;
    if (offset == 257) {
      offset = 0;
    }
  }
}
