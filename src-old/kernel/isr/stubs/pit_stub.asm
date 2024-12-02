global pit_isr
extern pit_isr_handler

pit_isr:
  cli                     ; Disable interrupts
  pusha                   ; Save all registers
  call pit_isr_handler    ; Call the C handler
  popa                    ; Restore all registers
  sti                     ; Re enable interrupts
  iretd                   ; Return from interrupt
