#include "paging.h"
//#include "lib.h"



void paging_init(){
    
    

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




