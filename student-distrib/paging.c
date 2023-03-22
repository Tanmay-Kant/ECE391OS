#include "paging.h"
//#include "lib.h"



void paging_init(){
    int i; 

    //pt 
    for (i = 0; i < 1024; i++) {
        if (i == 0xB8){
            page_table[i].p = 1;
            page_table[i].addr = 0xB8;
        }
        else{
            page_table[i].p = 0;
            page_table[i].addr= 0;
        }
        page_table[i].pcd = 0;
        page_table[i].rw = 1;
        page_table[i].us = 0;
        page_table[i].pwt = 0;
        page_table[i].a = 0;
        page_table[i].d = 0 ;
        page_table[i].pat= 0;
        page_table[i].g = 1;
        page_table[i].avl = 0;
    }
    for (i = 0; i < 1024; i++) {
        page_directory[i].rw = 1;
        page_directory[i].us = 0;
        page_directory[i].pwt = 0;
        page_directory[i].pcd = 0;
        page_directory[i].a = 0;
        page_directory[i].d = 0;
        page_directory[i].g = 0;
        if (i == 0){
            page_directory[i].p = 1;
            page_directory[i].res = 1;
            page_directory[i].ps = 0;
            page_directory[i].addrlong = (uint32_t)page_table >> 12;
        }
        else if (i == 1){
            page_directory[i].p = 1;
            page_directory[i].res = 1;
            page_directory[i].ps = 1;
            page_directory[i].addrlong = 1 << 10;
        }
        else{
            page_directory[i].p = 0;
            page_directory[i].res = 0;
            page_directory[i].ps = 0;
            page_directory[i].addrlong = 0;
        }
        
    }
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




