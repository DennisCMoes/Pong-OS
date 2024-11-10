#include <system.h>
#include <stdio.h>

int main()
{
    char msg[] = "Hello from the debug menu!\n";

    serial_init();
    write_serial_string(msg);
    write_serial_string("This is my second line\n");

    //write_serial('D');
    //write_serial('E');
    //write_serial('N');
    //write_serial('N');
    //write_serial('I');
    //write_serial('S');

    unsigned short colorIndex = 0;
    unsigned char *vgaMemory = (unsigned char *)VGA_VIDEO_MEMORY;

    // unsigned short yCor = 0;
    // unsigned short xCor = 0;

    // while (yCor < SCREEN_HEIGHT)
    // {
    //     while (xCor < SCREEN_WIDTH)
    //     {
    //         unsigned short offset = (yCor * SCREEN_WIDTH) + xCor;
    //         vgaMemory[offset] = colorIndex;
    //         xCor++;
    //     }

    //     xCor = 0;
    //     colorIndex++;
    //     if (colorIndex == 257)
    //     {
    //         colorIndex = 0;
    //     }

    //     yCor++;
    //     if (yCor >= SCREEN_HEIGHT)
    //     {
    //         yCor = 0;
    //     }
    // }

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

    for (unsigned short yCor = 60; yCor < 70; yCor++)
    {
        for (unsigned short xCor = 100; xCor < 200; xCor++)
        {
            unsigned short offset = (yCor * SCREEN_WIDTH) + xCor;
            vgaMemory[offset] = 4;
        }
    }

    for (;;)
        ;
}
