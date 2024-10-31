#include "kernel/test.c"
#include "kernel/types.c"
#include "drivers/screen.c"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

int main()
{
    int yCoor = 10;
    int xCoor = 30;

    set_palette_color(4, 11, 132, 23);

    uint16 *vga_memory = (uint16 *)VGA_VIDEO_MEMORY;
    write_pixel(test() * test(), 4);

    for (int y = yCoor; y < yCoor + 10; y++)
    {
        for (int x = xCoor; x < xCoor + 50; x++)
        {
            uint16 offset = (y * SCREEN_WIDTH) + x;
            write_pixel(offset, 4);
        }
    }
}