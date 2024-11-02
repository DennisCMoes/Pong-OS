// #include "system.h"
#include "drivers/screen.c"

int main()
{
    uint16 colorIndex = 0;

    for (uint16 yCor = 0; yCor < SCREEN_HEIGHT; yCor++) {
        for (uint16 xCor = 0; xCor < SCREEN_WIDTH; xCor++) {
            uint16 offset = (yCor * SCREEN_WIDTH) + xCor;

            write_pixel(offset, colorIndex);
        }

        colorIndex++;
        if (colorIndex == 257) {
            colorIndex = 0;
        }
    }
}