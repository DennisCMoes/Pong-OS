[org 0x7c00]            ; Add offset

mov bx, BOOT_MSG
call print_string

jmp $

%include "src/print_function.S"

BOOT_MSG:
    db 'BOOTING OS', 0

times 510-($-$$) db 0   ; Pad the boot sector with zeroes
dw 0xaa55               ; Last two bytes tells the BIOS it's a boot sector