#define VGA_VIDEO_MEMORY 0xA0000

#define SCREEN_WIDTH 320
#define SCREEN_HEIGH 200

int _main() {
    int yCor = 10;
    int xCor = 30;

    unsigned char* vga_memory = (unsigned char*)VGA_VIDEO_MEMORY;

    for (int y = yCor; y < yCor + 10; y++) {
        for (int x = xCor; x < xCor + 50; x++) {
            unsigned int offset = (y * SCREEN_WIDTH) + x;
            vga_memory[offset] = 4;
        }
    }

    return 0;
}
