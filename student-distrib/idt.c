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
        
        idt[i].present = 0;
        idt[i].dpl = 0x0;
        idt[i].reserved0 = 0;
        idt[i].size = 1;
        idt[i].reserved1 = 1;
        idt[i].reserved2 = 1;
        idt[i].reserved3 = 0;
        idt[i].reserved4 = 0;
        //if(i > 0x13 || i == 0x0F)
        if(i < 32 && i != 15)
        {
            //idt[i].present = 0;
            idt[i].reserved3 = 1;

            idt[i].present = 1;
        
        }
        if ( i == SYSCALL_VEC_NUM){
            idt[i].dpl = 3;

        }
        idt[i].seg_selector = KERNEL_CS;
    }
    
    // write function to change present only for exception and interrupt cases
    // write out the handlers for exceptions and interrupts - initialize all the bits for the idt 
    /*
    idt[SYSCALL_VEC_NUM].present = 1;
    idt[SYSCALL_VEC_NUM].dpl = 0x3;
    SET_IDT_ENTRY(idt[SYSCALL_VEC_NUM], system_calls);
   */
    //lidt(idt_desc_ptr);
    idt[RTC_VEC_NUM].present = 1;
    idt[RTC_VEC_NUM].reserved3 = 0x1;
    SET_IDT_ENTRY(idt[RTC_VEC_NUM], RTC_INT);
    idt[KEYBOARD_VEC_NUM].present = 1;
    idt[KEYBOARD_VEC_NUM].reserved3 = 0x1;
    SET_IDT_ENTRY(idt[KEYBOARD_VEC_NUM], KEY_INT);

    
    SET_IDT_ENTRY(idt[0], division_error);
    SET_IDT_ENTRY(idt[1], debug);
    SET_IDT_ENTRY(idt[2], non_maskable_interrupt);
    SET_IDT_ENTRY(idt[3], breakpoint);
    SET_IDT_ENTRY(idt[4], overflow);
    SET_IDT_ENTRY(idt[5], bound_range_exceeded);
    SET_IDT_ENTRY(idt[6], invalid_opcode);
    SET_IDT_ENTRY(idt[7], device_not_available);
    SET_IDT_ENTRY(idt[8], double_fault);
    SET_IDT_ENTRY(idt[9], coprocessor_segment_overrun);
    SET_IDT_ENTRY(idt[10], invalid_tss);
    SET_IDT_ENTRY(idt[11], segment_not_present);
    SET_IDT_ENTRY(idt[12], stack_segment_fault);
    SET_IDT_ENTRY(idt[13], general_protection_fault);
    SET_IDT_ENTRY(idt[14], page_fault);
    SET_IDT_ENTRY(idt[16], floating_point_exception);
    SET_IDT_ENTRY(idt[17], alignment_check);
    SET_IDT_ENTRY(idt[18], machine_check);
    SET_IDT_ENTRY(idt[19], simd_floating_point_exception);

}
    void exception_handler(int32_t index){
        printf("\nException Found: %s \n", exception_name_list[index]);
        while(1){
        }
    }

