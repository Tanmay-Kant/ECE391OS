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
#define   MAX_SPACES    1024      //Number of tables/pages in dir
#define   ALIGN_4KB		4096			 //(2^12)
#define   VIDMEM_ADDR   0xB8000    //Video memory address in physical memory
#define   KERNEL_ADDR   0x400000    //Kernel address in physical memory

#define   PAGE_4MB      0x400000  //4 MB page size
#define   USER_MEM      0x8000000 //Page address for user stack
#define   USER_INDEX    32        //The directory index for this address.
#define   VIDEO_INDEX   34        //directory used for vidmap  

#define VM_VIDEO 0x8800000
//#define asm


extern void paging_init();
extern void loadPaging();
extern void paging_vmap();



#endif /* _PAGING_H */


