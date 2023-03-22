#if !defined(_EXCEPTION_LINKAGE_H_)
#define _EXCEPTION_LINKAGE_H_

//extern void KEYBOARD_INT();
extern void RTC_INT();
extern void KEY_INT();


extern void division_error(void);
extern void debug(void);
extern void non_maskable_interrupt(void);
extern void breakpoint(void);
extern void overflow(void);
extern void bound_range_exceeded(void);
extern void invalid_opcode(void);
extern void device_not_available(void);
extern void double_fault(void);
extern void coprocessor_segment_overrun(void);
extern void invalid_tss(void);
extern void segment_not_present(void);
extern void stack_segment_fault(void);
extern void general_protection_fault(void);
extern void page_fault(void);
extern void floating_point_exception(void);
extern void alignment_check(void);
extern void machine_check(void);
extern void simd_floating_point_exception(void);

extern void system_calls(void);

#endif /*_EXCEPTION_LINKAGE_H*/
