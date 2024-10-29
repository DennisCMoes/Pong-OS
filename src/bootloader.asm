[bits 16]
[org 0x7c00]            ; Add offset

xor ax, ax
cli
mov ss, ax
mov sp, 0x7e00          ; Set up stack (512 bytes after the start)

mov bx, BOOT_MSG
call print_string

%include "src/print_function.S"

jmp $

BOOT_MSG:
    db 'BOOTING OS', 0

times 510-($-$$) db 0   ; Pad the boot sector with zeroes
dw 0xaa55               ; Last two bytes tells the BIOS it's a boot sector

db 'Hello, world - from disk sector numbero 2'
db 0