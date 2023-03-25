#include "paging.h"

/* void paging_init();
* Inputs: none
* Return: none
* Description: intializes paging throughout the whole system
* side effects: sets paging through out the system with specific cases for the kernel page directory and the first 4 kb including video memory
*/

void paging_init(){
    int i; 

    //pt - 1024 is size of the pages
    for (i = 0; i < 1024; i++) {
        // page table initialization by individual bits
        page_table[i].p = 0;
        page_table[i].addr= 0;
        page_table[i].pd = 0;
        page_table[i].rw = 1;
        page_table[i].us = 0;
        page_table[i].pt = 0;
        page_table[i].a = 0;
        page_table[i].d = 0 ;
        page_table[i].pat= 0;
        page_table[i].gp = 1;
        page_table[i].avl = 0;
        // page directory initialization by individual bits
        page_directory[i].rw = 1;
        page_directory[i].us = 0;
        page_directory[i].pt = 0;
        page_directory[i].pd = 0;
        page_directory[i].a = 0;
        page_directory[i].d = 0;
        page_directory[i].gp = 0;
        page_directory[i].p = 0;
        page_directory[i].res = 0;
        page_directory[i].ps = 0;
        page_directory[i].addrl = 0;
                
    }

    // page table for video memory address
    page_table[0xB8].p = 1;
    page_table[0xB8].addr = 0xB8;

    // page directory initialization for memory address
    page_directory[0].p = 1;
    page_directory[0].res = 1;
    page_directory[0].addrl = (uint32_t)page_table >> 12;

    // page directory for kernel case
    page_directory[1].p = 1;
    page_directory[1].res = 1;
    page_directory[1].ps = 1;
    page_directory[1].addrl = 1 << 10;

    // asm code call
    loadPaging((uint32_t*)page_directory);
    
    // directory 


    //user map 

    //vid memory 

    /*  
    unsigned int *dir_pointer = &page_dir;
    unsigned int *tab_pointer = &page_tab;
    int count;
    // size for 4 kb page - 0x1000
    unsigned holder = 0x1000; 
    dir_pointer[0] = (((unsigned int) tab_pointer) & addrs_mask) | rw_present;


    for(count = 1; count < pages; count++){
        tab_pointer[count] = holder | rw_present; 
        // increment by size of 4kb page
        holder += 0x1000;
    }
    unsigned int * addrs_pointer = (unsigned int *) kernel_Addrs;
    // 80 is size for 4 mb page
    dir_pointer[1] = (((unsigned int) addrs_pointer) & reserved_addrs_mask) | rw_present | 0x80 ;

    ENABLE_PAGING((unsigned int *)dir_pointer);

    */

}   




