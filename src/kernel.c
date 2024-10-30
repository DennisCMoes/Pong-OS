#define VGA_VIDEO_MEMORY 0xA0000

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

int main() {
    int yCoor = 10;
    int xCoor = 30;

    unsigned char* vga_memory = (unsigned char*)VGA_VIDEO_MEMORY;

    for (int y = yCoor; y < yCoor + 10; y++) {
        for (int x = xCoor; x < xCoor + 50; x++) {
            unsigned int offset = (y * SCREEN_WIDTH) + x;
            vga_memory[offset] = 4;
        }
    }


}