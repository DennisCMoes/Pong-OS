global pit_isr
extern pit_isr_handler

pit_isr:
  cli
  pusha                   
  call pit_isr_handler    ; Call the C handler
  popa
  sti
  iretd
