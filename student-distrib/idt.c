#include "idt.h"
#include "x86_desc.h"
#include "lib.h"

static char* exception_name_list[20] = {
    "Division Error",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "x87 Floating-point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
};

void idt_init(){
    
}