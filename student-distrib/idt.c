#include "idt.h"
#include "exception_linkage.h"

/* List of Strings for Exceptions */
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

/* void idt_init(void);
 * Inputs: void
 * Return Value: none
 * Function: Sets IDT entries of all 20 exceptions, hardware interrupts(keyboard and rtc), and system call
 * Sets corresponding bits of the gate descriptor, set dpl bit to 3 for system call, reserved3 bit to 1 for rtc and keyboard,
 * IDT entries not used have their present bit set to 0  */

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
        if(i < 32 && i != 15) //set the reserved3 bit for exceptions
        {
            //idt[i].present = 0;
            idt[i].reserved3 = 1;

            idt[i].present = 1;
        
        }
        if ( i == SYSCALL_VEC_NUM){
            idt[i].dpl = 3;         //set dpl to ring3 for userspace

        }
        idt[i].seg_selector = KERNEL_CS; //segment selector is always KERNEL_CS
    }
    
   
    idt[RTC_VEC_NUM].present = 1;
    idt[RTC_VEC_NUM].reserved3 = 0;
    SET_IDT_ENTRY(idt[RTC_VEC_NUM], RTC_INT);
    idt[KEYBOARD_VEC_NUM].present = 1;
    idt[KEYBOARD_VEC_NUM].reserved3 = 0; //check this
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


/* void exception_handler(int32_t index);
 * Inputs: index(IDT vector number to determine which exception should be printed)
 * Return Value: none
 * Function: Prints the corresponding exception based on the list of strings above as well as the vector that is passed
 * in the exception_linkage.S file */
 

    void exception_handler(int32_t index){
        clear();
        printf("\nException Found: %s \n", exception_name_list[index]);
        while(1){
        }
    }

