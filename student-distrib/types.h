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
/*
typedef union pde_m{
    uint32_t val;
    struct{
        uint32_t pr     :1;
        uint32_t rw     :1;
        uint32_t usr    :1;
        uint32_t wr     :1;
        uint32_t ca     :1;
        uint32_t ac     :1;
        uint32_t di     :1;
        uint32_t ps     :1;
        uint32_t gp     :1;
        uint32_t av     :3;
        uint32_t pat    :1;
        uint32_t res    :9;
        uint32_t addr   :10;
    } __attribute__ ((packed));
}pde_m;

typedef union pde_k{
    uint32_t val;
    struct{
        uint32_t pr     :1;
        uint32_t rw     :1;
        uint32_t usr    :1;
        uint32_t wr     :1;
        uint32_t ca     :1;
        uint32_t ac     :1;
        uint32_t re     :1;
        uint32_t ps     :1;
        uint32_t gp     :1;
        uint32_t av     :3;
        uint32_t addr   :20;
    } __attribute__ ((packed));
}pde_k;

typedef union pte_k{
    uint32_t val;
    struct{
        uint32_t pr     :1;
        uint32_t rw     :1;
        uint32_t usr    :1;
        uint32_t wr     :1;
        uint32_t ca     :1;
        uint32_t ac     :1;
        uint32_t di     :1;
        uint32_t pt     :1;
        uint32_t gp     :1;
        uint32_t av     :3;
        uint32_t addr   :20;
    } __attribute__ ((packed));

}pte_k;

typedef union pd{
    pde_k k_pde;
    pde_m m_pde;
}pd;

*/

#endif /* ASM */

#endif /* _TYPES_H */
