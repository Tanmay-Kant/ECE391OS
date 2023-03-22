#include "idt.h"
#include "exception_linkage.h"


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

void idt_init(void) {
    unsigned int i;
    for(i = 0; i < NUM_VEC; i++){
        idt[i].seg_selector = KERNEL_CS;
        idt[i].present = 1;
        idt[i].dpl = 0x3;
        idt[i].reserved0 = 0;
        idt[i].size = 1;
        idt[i].reserved1 = 1;
        idt[i].reserved2 = 1;
        idt[i].reserved3 = 0;
        idt[i].reserved4 = 0;
        //if(i > 0x13 || i == 0x0F)
        if(i < 32 || i == 0x0F)
        {
            //idt[i].present = 0;
            idt[i].reserved3 = 0x1;
            idt[i].dpl = 0x0;
            if( i < 20){
                idt[i].present = 1;
            }
        }
    }
    
    // write function to change present only for exception and interrupt cases
    // write out the handlers for exceptions and interrupts - initialize all the bits for the idt 
    idt[SYSCALL_VEC_NUM].present = 1;
    idt[SYSCALL_VEC_NUM].dpl = 0x3;
    SET_IDT_ENTRY(idt[SYSCALL_VEC_NUM], system_calls);
   
    //lidt(idt_desc_ptr);
    
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

}
    void exception_handler(int index){
        while(1){
        printf("\nException Found: %s \n", exception_name_list[index]);
        }
    }

