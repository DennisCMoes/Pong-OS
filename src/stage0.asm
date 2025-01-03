[bits 16]
[org 0x7C00]

global _start
_start:
    cli

    ; segment setup
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Place stack pointer in the middle of the free memory area
    mov sp, 0x3000

    ; Save drive number to read kernel later with
    mov [drive_num], dl

    sti

    ; Print "PONG OS"
    mov si, welcome_str
    call print

    ; Prepare disk_packet
    mov byte [disk_packet], 0x10
    mov byte [disk_packet + 1], 0x00
    mov word [disk_packet + 2], 0x0040
    mov word [disk_packet + 4], 0x0000
    mov word [disk_packet + 6], 0x1000
    mov dword [disk_packet + 8], 1
    mov dword [disk_packet + 12], 0

    mov si, disk_packet
    mov ah, 0x42
    mov dl, [drive_num]
    int 0x13
    jc disk_error

    ; Set video mode: 320x200 with 16 bit colors
    mov ah, 0x00
    mov al, 0x13
    int 0x10
        
    ; Enable A20 line and disable interrupts
    cli

    ; Read and save state
    call enable_a20_wait0
    mov al, 0xD0
    out 0x64, al
    call enable_a20_wait0
    xor ax, ax
    in al, 0x60

    ; Write new state with A20 bit set (0x2)
    push ax
    call enable_a20_wait0
    mov al, 0xD1
    out 0x64, al
    call enable_a20_wait0
    pop ax
    or ax, 0x2
    out 0x60, al

    ; Enable PE flag
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    ; Jump to flush the prefetch queue
    jmp flush

flush:
    lidt [idt]
    lgdt [gdtp]

    mov ax, gdt_data_segment - gdt_start
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x3000
    jmp 0x8:entry32

[bits 32]
entry32:
    ; Jump to the kernel, loaded at 0x10000
    mov eax, 0x10000
    jmp eax

_loop:
    jmp _loop

[bits 16]
enable_a20_wait0:
  xor ax, ax
  in al, 0x64
  bt ax, 1
  jc enable_a20_wait0
  ret

enable_a20_wait1:
  xor ax, ax
  in al, 0x64
  bt ax, 0
  jnc enable_a20_wait1
  ret

; Prints string at DS:SI
print:
    xor bh, bh
    mov ah, 0x0E
print_loop:
    lodsb
    cmp al, 0
    je print_done
    int 0x10
    jmp print_loop
print_done:
    ret

disk_error:
    mov si, disk_error_str
    call print

; Variables
welcome_str    db "PONG OS!!", 0
disk_error_str db "DISK ERROR", 0

drive_num db 0x00
disk_packet db 16 dup(0)


; GDT
gdtp:
    dw gdt_end - gdt_start - 1
    dd gdt_start

gdt_start:
gdt_null:
    dq 0
gdt_code_segment:
    dw 0xFFFF, 0x0000
    db 0x00, 0b10011010, 0b11001111, 0x00
gdt_data_segment:
    dw 0xFFFF, 0x0000
    db 0x00, 0b10010010, 0b11001111, 0x00
gdt_end:

; IDT
idt:
    dw 0
    dd 0

; MBR BOOT SIGNATURE
times 510-($-$$) db 0
dw 0xAA55
