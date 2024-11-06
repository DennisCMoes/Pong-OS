[bits 16]
[org 0x7c00]                ; Shift to bootloader offset
KERNEL_OFFSET equ 0x1000    ; Memory offset in which we will load our kernel

mov [BOOT_DRIVE_ID], dl

mov bp, 0x9000              ; Set up stack at 0x9000
mov sp, bp

xor ax, ax
mov ds, ax                  ; Make sure DS is set to 0

mov dl, [BOOT_DRIVE_ID]     ; Load the boot drive ID in DL
mov bx, KERNEL_OFFSET       ; Load 2 sectors to 0x0000(ES):0x1000(BX)
mov dh, 54                  ; (54 * 512b) = 27.648b + 512b (bootloader)
call disk_load

mov ah, 0x00                ; int=10/ah=0x0 -> BIOS video mode
mov al, 0x13                ; 
int 0x10                    ; Fire in the hole

; Enable a20 line
in al, 0xee

call switch_to_pm

jmp $

%include "src/bootloader/print_function.asm"
%include "src/bootloader/load_disk.asm"
%include "src/bootloader/gdt.asm"
%include "src/bootloader/protected_mode.asm"

[bits 32]
begin_pm:
    call KERNEL_OFFSET
    jmp $

BOOT_DRIVE_ID: db 0
BEFORE: db 'Before protected mode', 0x0A
PROTECTED_MODE: db 'We have entered protected mode', 0x0

times 510-($-$$) db 0       ; Pad the boot sector with zeroes
dw 0xaa55                   ; Last two bytes tells the BIOS it's a boot sector