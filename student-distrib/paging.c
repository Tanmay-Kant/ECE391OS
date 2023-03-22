#include "paging.h"
//#include "lib.h"



void paging_init(){
    int i; 

    //pt 
    for( i = 0; i < 1024; i++){
        page_tab[i].pr = 0; 
        page_tab[i].rw = 1;
        page_tab[i].usr = 0;
        page_tab[i].wr = 0;
        page_tab[i].ca = 0;
        page_tab[i].ac = 0;
        page_tab[i].di = 0;
        page_tab[i].pt = 0;
        page_tab[i].gp = 1;
        page_tab[i].av = 0;
        page_tab[i].addr = 0x2;
        page_dir.m_pde[i].pr = 0;
        page_dir.m_pde[i].rw = 1;
        page_dir.m_pde[i].usr = 0;
        page_dir.m_pde[i].wr = 0;
        page_dir.m_pde[i].ca = 0;
        page_dir.m_pde[i].ac = 0;
        page_dir.m_pde[i].di = 0;
        page_dir.m_pde[i].ps = 0;
        page_dir.m_pde[i].gp = 0;
        page_dir.m_pde[i].av = 0;
        page_dir.m_pde[i].pat = 0;
        page_dir.m_pde[i].res = 0;
        page_dir.m_pde[i].addr = 0;
    }
    page_tab[0xB8].addr = 0xB8;
    page_tab[0xB8].pr = 1;

    page_dir[0].k_pde.pr = 1;
    page_dir[0].k_pde.rw = 0;
    page_dir[0].k_pde.usr = 0;
    page_dir[0].k_pde.wr = 0;
    page_dir[0].k_pde.ca = 0;
    page_dir[0].k_pde.ac = 0;
    page_dir[0].k_pde.re = 0;
    page_dir[0].k_pde.ps = 0;
    page_dir[0].k_pde.gp = 0;
    page_dir[0].k_pde.av = 0;
    page_dir[0].k_pde.addr = (uint32_t)page_tab>>12;

    page_dir[1].m_pde.pr = 1;
    page_dir[1].m_pde.rw = 1;
    page_dir[1].m_pde.usr = 0;
    page_dir[1].m_pde.wr = 0;
    page_dir[1].m_pde.ca = 0;
    page_dir[1].m_pde.ac = 0;
    page_dir[1].m_pde.di = 0;
    page_dir[1].m_pde.ps = 1;
    page_dir[1].m_pde.gp = 1;
    page_dir[1].m_pde.av = 0;
    page_dir[1].m_pde.pat = 0;
    page_dir[1].m_pde.res = 0;
    page_dir[1].m_pde.addr = 0x4000000 >> 22;

    loadPaging(page_dir);
    return;
    
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




