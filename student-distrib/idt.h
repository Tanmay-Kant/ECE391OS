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

void exception_handler(int index); //printing support for the corresponding exception

void system_calls(void);


#endif /* _IDT_H */
