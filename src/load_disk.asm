; Load DH sectors to ES:BX from drive DL
disk_load:
    push dx

    mov ah, 0x02                    ; int=13/ah=0x2h -> Read sectors from drive
    mov al, dh                      ; Read DH sectors
    mov ch, 0x00                    ; Cylinder 0
    mov dh, 0x00                    ; Head 0
    mov cl, 0x02                    ; Load in Sector number 2

    int 0x13                        ; Fire in the hole!!
    jc  disk_error                  ; If CF (carry flag) is set, jump to disk_error

    pop dx                          ; Restore DX from stack
    cmp dh, al                      ; if AL (sectors read) != DH (sectors expected)
    jne disk_error 
    ret

    disk_error:
        mov bx, DISK_ERROR_MSG      ; Mov string to parameter
        call print_string           ; Printing disk error message
        jmp $

DISK_ERROR_MSG: db 'Disk read error!', 0