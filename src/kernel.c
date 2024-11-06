#include <system.h>

int main()
{
    unsigned short colorIndex = 0;
    unsigned char *vgaMemory = (unsigned char*)VGA_VIDEO_MEMORY;
    
    for (unsigned short yCor = 0; yCor < SCREEN_HEIGHT; yCor++)
    {
        for (unsigned short xCor = 0; xCor < SCREEN_WIDTH; xCor++)
        {
            unsigned short offset = (yCor * SCREEN_WIDTH) + xCor;
            vgaMemory[offset] = colorIndex;
        }

        colorIndex++;
        if (colorIndex == 257)
        {
            colorIndex = 0;
        }
    }
}