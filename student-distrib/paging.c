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
    page_table[0xB9].p = 1;
    page_table[0xB9].rw = 1;
    page_table[0xB9].us = 1;
    page_table[0xB9].addr = 0xB9;
    page_table[0xBA].p = 1;
    page_table[0xBA].us = 1;
    page_table[0xBA].rw = 1;
    page_table[0xBA].addr = 0xBA;
    page_table[0xBB].p = 1;
    page_table[0xBB].rw = 1;
    page_table[0xBB].us = 1;
    page_table[0xBB].addr = 0xBB;

    // page directory initialization for memory address
    page_directory[0].p = 1;
    page_directory[0].ps = 0;
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

    for (i = 0; i < 1024; i++) {
        page_table_vidmap[i].pat = 0;
        page_table_vidmap[i].p = 0;
        if (i == 0){
            // vidmap initialization
            page_table_vidmap[i].p = 1;
            page_table_vidmap[i].rw = 1;
            page_table_vidmap[i].us = 1;
            page_table_vidmap[i].addr = 0xB8;
        }
    }

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

/* void paging_vmap();
* Inputs: none
* Return: none
* Description: modifies bits in a page directory entry above the 32 index
*/
void paging_vmap(){
    // used 33 - just has to be some value over 32 and be kept constant through out
    page_directory[33].p = 1;
    page_directory[33].rw = 1;
    page_directory[33].us = 1;

    page_directory[33].pt = 0;
    page_directory[33].pd = 0;
    page_directory[33].a = 0;

    page_directory[33].ps = 0;
    page_directory[33].gp = 0;
    page_directory[33].avl = 0;
    page_directory[33].res = 0;
    page_directory[33].addrl = (uint32_t) page_table_vidmap >> 12;

    /*enables PAE, protection, and paging modes*/
    asm (
        "movl $page_directory, %%eax   ;"
        "andl $0xFFFFFC00, %%eax       ;"
        "movl %%eax, %%cr3             ;"
        "movl %%cr4, %%eax             ;"
        "orl  $0x00000010, %%eax        ;"
        "movl %%eax, %%cr4             ;"
        "movl %%cr0, %%eax             ;"
        "orl  $0x80000001, %%eax        ;"
        "movl %%eax, %%cr0             "
        : : : "eax", "cc" 
    );
}
