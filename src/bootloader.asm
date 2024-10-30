[bits 16]
[org 0x7c00]                ; Add offset
KERNEL_OFFSET equ 0x1000    ; Memory off in which we will load our kernel

mov [BOOT_DRIVE_ID], dl     ; Store the boot drive ID from dl into BOOT_DRIVE_ID

mov bp, 0x9000              ; Set up stack at 0x9000
mov sp, bp

mov bx, KERNEL_OFFSET       ; Load 5 sectors to 0x0000(ES):0x9000(BX)
mov dh, 15
mov dl, [BOOT_DRIVE_ID]
call disk_load

call enable_video_mode

; Switch to 32 bit protected mode
call switch_to_pm

%include "src/print_function.asm"
%include "src/load_disk.asm"
%include "src/gdt.asm"
%include "src/protected_mode.asm"
%include "src/print_pm.asm"
%include "src/screen.asm"

[bits 32]
begin_pm:
    call draw_pixel
    call KERNEL_OFFSET

    jmp $

BOOT_DRIVE_ID db 0
MSG_PROT_MODE db "Successfully landed in 32-bit mode", 0

times 510-($-$$) db 0       ; Pad the boot sector with zeroes
dw 0xaa55                   ; Last two bytes tells the BIOS it's a boot sector

times 256 dw 0xdada
times 256 dw 0xface