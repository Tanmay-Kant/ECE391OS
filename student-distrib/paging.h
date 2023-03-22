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


typedef struct pde {
    union {
        struct {
            uint32_t p : 1;
            uint32_t rw : 1;
            uint32_t us : 1;
            uint32_t pwt : 1;
            uint32_t pcd : 1;
            uint32_t a : 1;
            uint32_t d : 1;
            uint32_t ps : 1;
            uint32_t g : 1;
            uint32_t avl : 3;
            uint32_t pat : 1;
            uint32_t res : 9;
            uint32_t addrshort : 10;
        }__attribute__ ((packed));
        struct {
            uint32_t p : 1;
            uint32_t rw : 1;
            uint32_t us : 1;
            uint32_t pwt : 1;
            uint32_t pcd : 1;
            uint32_t a : 1;
            uint32_t d : 1;
            uint32_t ps : 1;
            uint32_t g : 1;
            uint32_t avl : 3;
            uint32_t addrlong : 20;
        }__attribute__ ((packed));
    };
} pde_t;

typedef struct __attribute__ ((packed)) pte{
    uint32_t p : 1;
    uint32_t rw : 1;
    uint32_t us : 1;
    uint32_t pwt : 1;
    uint32_t pcd : 1;
    uint32_t a : 1;
    uint32_t d : 1;
    uint32_t pat : 1;
    uint32_t g : 1;
    uint32_t avl : 3;
    uint32_t addr : 20;
} pte_t;

pde_t page_directory[1024] __attribute__((aligned (4096)));
pte_t page_table[1024] __attribute__((aligned (4096)));


extern unsigned int * dir_pointer;
extern unsigned int * tab_pointer;

extern void paging_init();
extern void loadPaging();


#endif /* _PAGING_H */


