#include "types.h"
#include "lib.h"
#include "x86_desc.h"

#ifndef _PAGING_H
#define _PAGING_H


#define reserved_addrs_mask 0xFFC00000
#define addrs_mask 0xFFFFF000
#define kernel_Addrs 0x400000
#define pages 1024
#define pse_Flag 0x10
#define pg_flag 0x80000000
#define rw_present 0x3
#define page_s 0x80
#define asm


extern unsigned int page_dir; 
extern unsigned int page_tab;

extern unsigned int * dir_pointer;
extern unsigned int * tab_pointer;

extern void paging_init();


#endif /* _PAGING_H */


