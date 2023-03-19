# exception_linkage.S - sets up linkage macro for all exceptions based on if they have an error code

#define ASM     1
#include "exception_linkage.h"


#define EXCEPTION_NO_ERROR(name, vector)
    .globl name
    name:
            pushl $0
            pushl $number
            call exception_handler
            addl $4, %esp #pop number
            addl $4, %esp #pop error value
            iret

 #define EXCEPTION_ERROR(name, vector)
    .globl name
    name:
            pushl $number
            call exception_handler
            addl $4, %esp #pop number
            addl $4, %esp #pop error value
            iret           

EXCEPTION_NO_ERROR(division_error, 0x00);
EXCEPTION_NO_ERROR(debug, 0x01);
EXCEPTION_NO_ERROR(non_maskable_interrupt, 0x02);
EXCEPTION_NO_ERROR(breakpoint, 0x03);
EXCEPTION_NO_ERROR(overflow, 0x04);
EXCEPTION_NO_ERROR(bound_range_exceeded, 0x05);
EXCEPTION_NO_ERROR(invalid_opcode, 0x06);
EXCEPTION_NO_ERROR(device_not_available, 0x07);
EXCEPTION_ERROR(double_fault, 0x08); # contains an error code
EXCEPTION_NO_ERROR(coprocessor_segment_overrun, 0x09);
EXCEPTION_ERROR(invalid_tss, 0x0A);
EXCEPTION_ERROR(segment_not_present, 0x0B);
EXCEPTION_ERROR(stack_segment_fault, 0x0C);
EXCEPTION_ERROR(general_protection_fault, 0x0D);
EXCEPTION_ERROR(page_fault, 0x0E);
EXCEPTION_NO_ERROR(floating_point_exception, 0x10);
EXCEPTION_ERROR(alignment_check, 0x11);
EXCEPTION_NO_ERROR(machine_check, 0x12);
EXCEPTION_NO_ERROR(simd_floating_point_exception, 0x13);   

EXCEPTION_NO_ERROR(system_calls, 0x80);