#ifndef _IDT_H
#define _IDT_H
#include "x86_desc.h"
#include "exception_linkage.h"
#include "lib.h"



void idt_init(); //Initializes the IDT and sets IDT entries 

void exception_handler(uint32_t vector, uint32_t error); //printing support for the corresponding exception and if the error is raised



#endif /* _IDT_H */
