[bits 32]

extern _main

global _start
_start:
    mov esp, stack
    and esp, 0xFFFFFFF0
    mov eax, 0xDEADBEEF
    push esp
    push eax

    cli
    call _main

ALIGN 4
global idt_load
idt_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret

%macro ISR_NO_ERR 1
    global _isr%1
_isr%1:
    cli
    push dword 0        ; Push a dummy error code
    push dword %1       ; Push the interrupt index
    jmp isr_common
%endmacro

%macro ISR_ERR 1
    global _isr%1
_isr%1:
    cli
    push dword %1       ; Push the interrupt index
    jmp isr_common
%endmacro

; Define ISR handlers
ISR_NO_ERR 0
ISR_NO_ERR 1
ISR_NO_ERR 2
ISR_NO_ERR 3
ISR_NO_ERR 4
ISR_NO_ERR 5
ISR_NO_ERR 6
ISR_NO_ERR 7
ISR_ERR 8
ISR_NO_ERR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_NO_ERR 15
ISR_NO_ERR 16
ISR_NO_ERR 17
ISR_NO_ERR 18
ISR_NO_ERR 19
ISR_NO_ERR 20
ISR_NO_ERR 21
ISR_NO_ERR 22
ISR_NO_ERR 23
ISR_NO_ERR 24
ISR_NO_ERR 25
ISR_NO_ERR 26
ISR_NO_ERR 27
ISR_NO_ERR 28
ISR_NO_ERR 29
ISR_NO_ERR 30
ISR_NO_ERR 31
ISR_NO_ERR 32
ISR_NO_ERR 33
ISR_NO_ERR 34
ISR_NO_ERR 35
ISR_NO_ERR 36
ISR_NO_ERR 37
ISR_NO_ERR 38
ISR_NO_ERR 39
ISR_NO_ERR 40
ISR_NO_ERR 41
ISR_NO_ERR 42
ISR_NO_ERR 43
ISR_NO_ERR 44
ISR_NO_ERR 45
ISR_NO_ERR 46
ISR_NO_ERR 47

; defined in isr.c
extern isr_handler

isr_common:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    cld

    push esp
    call isr_handler
    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds

    popa
    add esp, 8
    iret

ALIGN 32
stack_begin:
    times 0x4000 db 0     ; Allocate 0x4000 bytes for the stack
stack:
