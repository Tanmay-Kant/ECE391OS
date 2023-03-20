#include "idt.h"


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

    lidt(idt_desc_ptr);
    unsigned int i;

    for(i = 0; i < NUM_VEC; i++){
        if(i != 0x0F || i <= 0x13){ //set all 20 exceptions except 15(reserved) to present
            idt[i].present = 0x1;
            idt[i].reserved4 = 0x0;
            idt[i].reserved3 = 0x0;
            idt[i].reserved2 = 0x1;     
            idt[i].reserved1 = 0x1;
            idt[i].reserved0 = 0x0;
            idt[i].dpl = 0x0;
            idt[i].seg_selector = KERNEL_CS;
            idt[i].size = 0x1;
        }
        else if(i == 0x21 || i == 0x28){ //if it is at a hardware interrupt (keyboard and rtc)
            idt[i].present = 0x1;
            idt[i].reserved4 = 0x0;
            idt[i].reserved3 = 0x1; //change for hardware interrupts
            idt[i].reserved2 = 0x1;     
            idt[i].reserved1 = 0x1;
            idt[i].reserved0 = 0x0;
            idt[i].dpl = 0x0;
            idt[i].seg_selector = KERNEL_CS;
            idt[i].size = 0x1; //size is 32 bits
        }
        else if(i == 0x80){ //system call, set dpl to ring 3
            idt[i].present = 0x1;
            idt[i].reserved4 = 0x0;
            idt[i].reserved3 = 0x0;
            idt[i].reserved2 = 0x1;
            idt[i].reserved1 = 0x1;
            idt[i].reserved0 = 0x0;
            idt[i].dpl = 0x3; //Userspace (ring 3)
            idt[i].seg_selector = KERNEL_CS;
            idt[i].size = 0x1;
        }
        else{
            idt[i].present = 0x0; //set present bit of all other interrupts to 0
        }

    
        
    }
    
    SET_IDT_ENTRY(idt[0x00], division_error);
    SET_IDT_ENTRY(idt[0x01], debug);
    SET_IDT_ENTRY(idt[0x02], non_maskable_interrupt);
    SET_IDT_ENTRY(idt[0x03], breakpoint);
    SET_IDT_ENTRY(idt[0x04], overflow);
    SET_IDT_ENTRY(idt[0x05], bound_range_exceeded);
    SET_IDT_ENTRY(idt[0x06], invalid_opcode);
    SET_IDT_ENTRY(idt[0x07], device_not_available);
    SET_IDT_ENTRY(idt[0x08], double_fault);
    SET_IDT_ENTRY(idt[0x09], coprocessor_segment_overrun);
    SET_IDT_ENTRY(idt[0x0A], invalid_tss);
    SET_IDT_ENTRY(idt[0x0B], segment_not_present);
    SET_IDT_ENTRY(idt[0x0C], stack_segment_fault);
    SET_IDT_ENTRY(idt[0x0D], general_protection_fault);
    SET_IDT_ENTRY(idt[0x0E], page_fault);
    SET_IDT_ENTRY(idt[0x10], floating_point_exception);
    SET_IDT_ENTRY(idt[0x11], alignment_check);
    SET_IDT_ENTRY(idt[0x12], machine_check);
    SET_IDT_ENTRY(idt[0x13], simd_floating_point_exception);

    SET_IDT_ENTRY(idt[0x80], system_calls);

    
}
    void exception_handler(uint32_t vector, uint32_t error){
        printf("\nException Found: %s (%d) \n\n", exception_name_list[vector], vector);
        printf("\nError Code: %#x\n", error);
    }

