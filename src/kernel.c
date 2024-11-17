#include "kernel/system.h"
#include "kernel/stdio.h"

int main()
{
    serial_init();
    char msg[] = "hello from the debug menu!\n";
    // write_serial_string(msg);

    void *args[] = {
      (void *)"Hello, OS!",
      (void *)(long)1234,
      (void *)(long)0xABCD,
      (void *)(long)'A',
      (void *)"Test string",
    };

    serial_printf("%s\n", args);
    serial_printf("Value: %d\n", args+1);
    serial_printf("Hex: %x\n", args+2);
    serial_printf("Char: %c\n", args+3);
    serial_printf("String: %s\n", args+4);

    // draw_pixel(100, 300, 2);
    // draw_char('d', 80, 80);
    // u8 *vgamemory = (u8*)vga_video_memory;
    // vgamemory[2000] = 2;
    
    while (1);
}
