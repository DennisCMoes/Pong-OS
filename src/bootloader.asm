[bits 16]
[org 0x7c00]            ; Add offset

mov [BOOT_DRIVE_ID], dl    ; Store the boot drive ID from dl into BOOT_DRIVE_ID

mov bp, 0x8000          ; Set up stack at 0x8000
mov sp, bp

mov bx, BOOT_MSG
call print_string

mov bx, 0x9000          ; Load 5 sectors to 0x0000(ES):0x9000(BX)
mov dh, 5
mov dl, [BOOT_DRIVE_ID]
call disk_load

mov dx, [0x9000 + 512]
call print_string

jmp $

%include "src/print_function.asm"
%include "src/load_disk.asm"

BOOT_MSG:       db 'BOOTING OS... ', 0
BOOT_DRIVE_ID:  db 0

times 510-($-$$) db 0   ; Pad the boot sector with zeroes
dw 0xaa55               ; Last two bytes tells the BIOS it's a boot sector

TEST_MSG: db 'OUTSIDE OF THE BOOTLOADER', 0

times 256 dw 0xdada
times 256 dw 0xface