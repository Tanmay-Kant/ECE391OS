# exception_linkage.S - sets up linkage macro for all exceptions based on if they have an error code
#linkage macro reference from 3.1 review slides

.align 4
syscall_table:
    .long 0x0         
    .long halt
    .long execute
    .long read
    .long write
    .long open
    .long close
    .long getargs
    .long vidmap

#define EXCEPTION_ERROR(name,vector)   \
    .GLOBL name                         ;\
    name:                               ;\
        pushfl                          ;\
        pushal                          ;\
        pushl	$vector                 ;\
        call    exception_handler       ;\
        addl    $8, %esp                ;\
        popal                           ;\
        popfl                           ;\
        iret


#define EXCEPTION_HANDLE(name, vector)    \
        .globl name                     ;\
        name:                           ;\
                pushal                  ;\
                pushfl                  ;\
                pushl $vector           ;\
                call exception_handler  ;\
                addl $4, %esp           ;\
                popfl                   ;\
                popal                   ;\
                iret

#define INTERRUPTS(name, vector)        \
        .globl name                     ;\
        name:                           ;\
                pushal                  ;\
                pushfl                  ;\
                call vector             ;\
                popfl                   ;\
                popal                   ;\
                iret


.globl syscall_handler
syscall_handler:
    pushfl
    pushl   %ecx
    pushl   %edx
    pushl   %ebx
    pushl   %esp
    pushl   %ebp
    pushl   %esi
    pushl   %edi
    pushl %edx
    pushl %ecx
    pushl %ebx
    cmpl    $0, %eax       
    jz      syscall_err
    cmpl    $10, %eax       
    ja      syscall_err
    call    *syscall_table(, %eax, 4)   
    jmp     syscall_leave

syscall_err:
    movl    $-1, %eax       

syscall_leave:
    addl    $12, %esp       
    popl    %edi
    popl    %esi
    popl    %ebp
    popl    %esp
    popl    %ebx
    popl    %edx
    popl    %ecx
    popfl
    iret

EXCEPTION_HANDLE(division_error, 0);
EXCEPTION_HANDLE(debug, 1);
EXCEPTION_HANDLE(non_maskable_interrupt, 2);
EXCEPTION_HANDLE(breakpoint, 3);
EXCEPTION_HANDLE(overflow, 4);
EXCEPTION_HANDLE(bound_range_exceeded, 5);
EXCEPTION_HANDLE(invalid_opcode, 6);
EXCEPTION_HANDLE(device_not_available, 7);
EXCEPTION_ERROR(double_fault, 8);
EXCEPTION_HANDLE(coprocessor_segment_overrun, 9);
EXCEPTION_ERROR(invalid_tss, 10);
EXCEPTION_ERROR(segment_not_present, 11);
EXCEPTION_ERROR(stack_segment_fault, 12);
EXCEPTION_ERROR(general_protection_fault, 13);
EXCEPTION_ERROR(page_fault, 14);
EXCEPTION_HANDLE(floating_point_exception, 16);
EXCEPTION_ERROR(alignment_check, 17);
EXCEPTION_HANDLE(machine_check, 18);
EXCEPTION_HANDLE(simd_floating_point_exception, 19);
INTERRUPTS(RTC_INIT, rtc_handler);
INTERRUPTS(KEY_INIT, keyboard_handler);   
INTERRUPTS(PIT_INIT, scheduler)



