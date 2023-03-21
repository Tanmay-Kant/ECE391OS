#ifndef _IDT_H
#define _IDT_H

#include "x86_desc.h"
#include "lib.h"
#define NUM_EXCEPTIONS      21
#define SYSCALL_VEC_NUM     (0x80)
#define RTC_VEC_NUM         (40)
#define KEYBOARD_VEC_NUM    (33)
#define E15    (15)



void idt_init(); //Initializes the IDT and sets IDT entries 

void exception_handler(uint32_t vector); //printing support for the corresponding exception

void division_error(void);
void debug(void);
void non_maskable_interrupt(void);
void breakpoint(void);
void overflow(void);
void bound_range_exceeded(void);
void invalid_opcode(void);
void device_not_available(void);
void double_fault(void);
void coprocessor_segment_overrun(void);
void invalid_tss(void);
void segment_not_present(void);
void stack_segment_fault(void);
void general_protection_fault(void);
void page_fault(void);
void floating_point_exception(void);
void alignment_check(void);
void machine_check(void);
void simd_floating_point_exception(void);

void system_calls(void);


#endif /* _IDT_H */
