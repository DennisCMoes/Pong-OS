; Loops through the string in bx
print_string:
    push bx                     ; Save the value of BX

    print_char:
        mov cl, byte [bx]       ; Move the byte at address in BX to CL
        cmp cl, 0x0             ; Check if the byte is null-terminator
        je end_part             ; If zero, jump to end

        mov al, cl              ; Move the character into AL
        mov ah, 0x0e            ; int=10/ah=0x0e -> BIOS teletype
        int 0x10                ; Print the character in al

        inc bx                  ; Move to the next character in the string
        jmp print_char          ; Repeat the loop

    end_part:
        pop bx                  ; Restore the original value of BX
        ret