[bits 16]
enable_video_mode:
    pusha

    mov ah, 0x00
    mov al, 0x13
    int 0x10

    popa
    ret

[bits 32]

global draw_pixel

; Base address of VGA video memory in mode 13h
VGA_VIDEO_MEMORY    equ 0xA0000

draw_pixel:
    pusha
    mov ebx, VGA_VIDEO_MEMORY

    mov eax, [esp + 4]                 ; Y-coordinate
    mov edx, [esp + 8]                ; Screen width in pixels
    mov ecx, 320
    mul ecx                     ; Multiply Y by 320 to get the row offset
    add eax, [esp + 12]                ; Add X-coordinate (100)

    mov byte [ebx + eax], 4     ; Write the pixel color (4 = red)

    popa
    ret