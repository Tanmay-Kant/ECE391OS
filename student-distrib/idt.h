#ifndef _IDT_H
#define _IDT_H

#include "x86_desc.h"
#include "exception_linkage.h"
#include "lib.h"
#define NUM_EXCEPTIONS      21
#define SYSCALL_VEC_NUM     (0x80)
#define RTC_VEC_NUM         (40)
#define KEYBOARD_VEC_NUM    (33)
#define E15    (15)



void idt_init(); //Initializes the IDT and sets IDT entries 

void exception_handler(uint32_t vector); //printing support for the corresponding exception



#endif /* _IDT_H */
