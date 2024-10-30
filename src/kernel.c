#include "kernel/test.c"

#define VGA_VIDEO_MEMORY 0xA0000

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

struct Color
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

int main() {
    int yCoor = 10;
    int xCoor = 30;

    unsigned char* vga_memory = (unsigned char*)VGA_VIDEO_MEMORY;
    
    vga_memory[test() * test()] = 0xf;

    struct Color color;

    color.blue = 0xB3;
    color.green = 0x2E;
    color.red = 0xFF;

    for (int y = yCoor; y < yCoor + 10; y++) {
        for (int x = xCoor; x < xCoor + 50; x++) {
            unsigned int offset = (y * SCREEN_WIDTH) + x;
            vga_memory[offset] = 4; // 4 is red
        }
    }
}