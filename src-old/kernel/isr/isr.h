#ifndef ISR_H
#define ISR_H

void exception_handler(void);
void divide_by_zero_exception(void);
void debug(void);
void non_maskable_interrupt(void);
void breakpoint(void);
void overflow(void);
void bound_range_exceeded(void);
void invalid_opcode(void);
void device_not_available(void);
void double_fault(void);
void invalid_tss(void);
void segment_not_present(void);
void stack_segment_fault(void);
void general_protection(void);
void page_fault(void);
void x87_fpu_error(void);
void alignment_check(void);
void machine_check(void);
void simd_error(void);

#endif
