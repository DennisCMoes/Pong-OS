[bits 16]
[org 0x7c00]                ; Add offset
KERNEL_OFFSET equ 0x1000    ; Memory off in which we will load our kernel

mov [BOOT_DRIVE_ID], dl     ; Store the boot drive ID from dl into BOOT_DRIVE_ID

mov bp, 0x9000              ; Set up stack at 0x9000
mov sp, bp

mov bx, KERNEL_OFFSET       ; Load 15 sectors to 0x0000(ES):0x9000(BX)
mov dh, 0x15                ; (15 * 512b) = 7.680kb + 512b (bootloader)
mov dl, [BOOT_DRIVE_ID]
call disk_load

mov ah, 0x00                ; int=10/ah=0x0 -> BIOS video mode
mov al, 0x13                ; 
int 0x10                    ; Fire in the hole

; Switch to 32 bit protected mode
call switch_to_pm

%include "src/bootloader/print_function.asm"
%include "src/bootloader/load_disk.asm"
%include "src/bootloader/gdt.asm"
%include "src/bootloader/protected_mode.asm"

[bits 32]
begin_pm:
    call KERNEL_OFFSET

    jmp $

BOOT_DRIVE_ID db 0
MSG_PROT_MODE db "Successfully landed in 32-bit mode", 0

times 510-($-$$) db 0       ; Pad the boot sector with zeroes
dw 0xaa55                   ; Last two bytes tells the BIOS it's a boot sector

times 256 dw 0xdada
times 256 dw 0xface