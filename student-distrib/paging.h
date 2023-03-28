#include "types.h"

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
//#define asm


extern void paging_init();
extern void loadPaging();


#endif /* _PAGING_H */


