; Load extra disk storage
mov ds, 0x0
xor ax, ax
cld

mov ah, 0x2h                    ; int=13/ah=0x2h -> Read sectors from drive
mov al, DISK_SECTORS            ; Read 2 sectors
mov ch, 0x0                     ; Cylinder 0
mov cl, 0x2                     ; Load in Sector number 2
mov dh, 0x0                     ; Use head number 0

xor bx, bx

mov es, bx
int 0x13h                       ; Execute
jc disk_error                   ; If CF (carry flag) is not empty jump to disk_error

mov bx, ah                      ; Move the return code into BH
call print_string

mov bx, al                      ; Move the read sectors count into BH
call print_string

mov bx, [0x7e00h + 32]          ; Print the hello string outside of the bootloader
call print_string

jmp 7e00h                       ; Jump to the next sector

disk_error:
    mov bx, DISK_ERROR_MSG      ; Mov string to parameter
    call print_string           ; Printing disk error message

DISK_ERROR_MSG:
    db 'Disk read error :(', 0

; Disk read variables
DISK_SECTORS:
    db 0x2