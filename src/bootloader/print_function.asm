; Loops through the string in bx
print_string:
    push bx                 ; Save the value of BX

print_char:
    mov cl, byte [bx]       ; Move the byte at address in BX to CL
    cmp cl, 0x0             ; Check if the byte is null-terminator
    je end_part             ; If zero, jump to end

    cmp cl, 0x0A            ; Check if the byte is newline('\n')
    jne not_newline         ; If not newline, continue to print character

    ; Handle newline
    mov al, 0x0D            ; Carriage return (move cursor to start of line)
    mov ah, 0x0E            ; BIOS teletype function
    int 0x10                ; Print carriage return

    mov al, 0x0A            ; Line feed (move cursor to next line)
    int 0x10                ; Print line feed

    jmp continue_print      ; Skip printing this character

not_newline:
    mov al, cl              ; Move the character into AL
    mov ah, 0x0e            ; int=10/ah=0x0e -> BIOS teletype
    int 0x10                ; Print the character in al

continue_print:
    inc bx                  ; Move to the next character in the string
    jmp print_char          ; Repeat the loop

end_part:
    pop bx                  ; Restore the original value of BX
    ret

; Prints a 16-bit hexadecimal value in AX
print_hex:
    pusha

    mov cx, 4               ; Print 4 nibbles (16 bits)

print_hex_loop:
    rol ax, 4               ; Rotate left by 4 bits to get the next nibble in the lowest 4 bits
    mov bl, al              ; Move the lower nibble of AX to BL
    and bl, 0x0F            ; Mask out the higher bits, so BL contains only the nibble

    ; Convert nibble to ASCII
    cmp bl, 0x09            ; Check if nibble is 0-9
    jbe print_number        ; If <= 9, it's a number (0-9)
    add bl, 0x37            ; Convert to ASCII 'A'-'F' (0x37 is needed to map 0x0A -> 'A')
    jmp print_char_hex

print_number:
    add bl, 0x30            ; Convert to ASCII '0'-'9'

print_char_hex:
    mov al, bl              ; Move the ASCII character to AL
    mov ah, 0x0E            ; BIOS teletype function
    int 0x10                ; Print the character in AL

    loop print_hex_loop     ; Repeat for all 4 nibbles
    
    popa
    ret