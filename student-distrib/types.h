/* types.h - Defines to use the familiar explicitly-sized types in this
 * OS (uint32_t, int8_t, etc.).  This is necessary because we don't want
 * to include <stdint.h> when building this OS
 * vim:ts=4 noexpandtab
 */

#ifndef _TYPES_H
#define _TYPES_H

#define NULL 0

#ifndef ASM

/* Types defined here just like in <stdint.h> */
typedef int int32_t;
typedef unsigned int uint32_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef char int8_t;
typedef unsigned char uint8_t;

// struct for page directory - elected to use one struct with both items defined inside instead of 2 too avoid syntax error when referencing 
typedef struct pde {
    union {
        struct {
            uint32_t p : 1;
            uint32_t rw : 1;
            uint32_t us : 1;
            uint32_t pt : 1;
            uint32_t pd : 1;
            uint32_t a : 1;
            uint32_t d : 1;
            uint32_t ps : 1;
            uint32_t gp : 1;
            uint32_t avl : 3;
            uint32_t pat : 1;
            uint32_t res : 9;
            uint32_t addrs : 10;
        }__attribute__ ((packed));
        struct {
            uint32_t p : 1;
            uint32_t rw : 1;
            uint32_t us : 1;
            uint32_t pt : 1;
            uint32_t pd : 1;
            uint32_t a : 1;
            uint32_t d : 1;
            uint32_t ps : 1;
            uint32_t gp : 1;
            uint32_t avl : 3;
            uint32_t addrl : 20;
        }__attribute__ ((packed));
    };
} pde_t;

// struct for page table entries 
typedef struct __attribute__ ((packed)) pte{
    uint32_t p : 1;
    uint32_t rw : 1;
    uint32_t us : 1;
    uint32_t pt : 1;
    uint32_t pd : 1;
    uint32_t a : 1;
    uint32_t d : 1;
    uint32_t pat : 1;
    uint32_t gp : 1;
    uint32_t avl : 3;
    uint32_t addr : 20;
} pte_t;


// 1024 is size specified by discussion slides 
pde_t page_directory[1024] __attribute__((aligned (4096)));
pte_t page_table[1024] __attribute__((aligned (4096)));


#endif /* ASM */

#endif /* _TYPES_H */
