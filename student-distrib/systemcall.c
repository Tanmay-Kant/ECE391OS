#include "systemcall.h"
//variables for keeping track of the pid values
uint32_t cur_pid = -1;
uint32_t parent_pid = -1;
uint32_t pid_array[6] = {0,0,0,0,0,0};

uint32_t store_ebp;
uint32_t store_esp;
//uint32_t cur_pcb;

/* int32_t halt(uint8_t status)
 * Inputs      : status
 * Output      : 0
 * Function    :  */
int32_t halt(uint8_t status){
    if (cur_pid == 0){
        execute((const uint8_t *)"shell");
        return status;
    }

    pcb_t* cur_pcb = get_cur_pcb(); 
    pid_array[cur_pid] = 0;

    int fd;
    for(fd = 2; fd < 8; fd++){
        cur_pcb -> fd_array[fd].flag = 1;
        cur_pcb -> fd_array[fd].inode = -1;
        cur_pcb -> fd_array[fd].file_pos = -1;
        cur_pcb -> fd_array[fd].fop_table_ptr = NULL;
    }

    int k = 32;
    page_directory[k].rw = 1;
    page_directory[k].us = 1;
    page_directory[k].pt = 0;
    page_directory[k].pd = 0;
    page_directory[k].a = 0;
    page_directory[k].d = 0;
    page_directory[k].gp = 0;
    page_directory[k].p = 1;
    page_directory[k].res = 0;
    page_directory[k].ps = 1;       
    uint32_t phys_addr = (parent_pid * PAGE_4MB) + 0x800000;      
    page_directory[k].addrs = phys_addr >> 22;

    flush_tlb();

    pcb_t* parent_pcb = get_pcb(parent_pid);
    cur_pid = parent_pid; 
    parent_pid = parent_pcb->parent_pid; 

    uint32_t saved_esp = cur_pcb->par_esp;
    uint32_t saved_ebp = cur_pcb->par_ebp;

    tss.esp0 =  EIGHT_MB - (EIGHT_KB*parent_pcb->pid) - 4;
    tss.ss0 = KERNEL_DS;

    asm volatile(
        "movl %0,   %%ebp        ;"
        "movl %1,   %%eax        ;"
        "movl %2,   %%esp        ;"
        "leave                   ;"
        "ret                     ;"
        :
        : "r"(saved_ebp), "r"((uint32_t)status), "r"(saved_esp)
        
    );

    return status;
}

fop_table_t rtc_fop = {rtc_open, rtc_close, rtc_read, rtc_write};
fop_table_t stdin_fop = {terminal_open, terminal_close, terminal_read, null_write};
fop_table_t stdout_fop = {terminal_open, terminal_close, null_read, terminal_write};
fop_table_t file_fop = {file_open, file_close, file_read, file_write};
fop_table_t dir_fop = {dir_open, dir_close, dir_read, dir_write};
fop_table_t null_fop = {null_open, null_close, null_read, null_write};


/* int32_t execute(const uint8_t* command)
 * Inputs      : command - sequence of words used for commands or arguemtns
 * Output      : 
 * Function    :  */
int32_t execute(const uint8_t* command){
    //clear();
    cli();
    uint32_t i,j;          
    int cmd_start = 0;    
    int arg_start = 0;
    int blank_count =0;

    int file_cmd_length = 0;
    int file_arg_length = 0;

    uint8_t file_cmd[FILE_SYS_NAME_LEN];
    uint8_t file_arg[FILE_SYS_NAME_LEN];
    
    dentry_t temp_dentry;
    uint8_t elf_buf[4];

    uint32_t eip_arg;
    uint32_t esp_arg;

    for (i=0;i< 32;++i){
      file_cmd[i] = '\0';
      file_arg[i] = '\0';
    }

    for(i = 0; i < strlen((const int8_t*)command); ++i){
        if(command[i] != ' '){
            file_cmd[cmd_start] = command[i];
            ++file_cmd_length;
            ++cmd_start;
        }
        else{
            ++blank_count;
            if(file_cmd_length > 0)
                break;
        }
    }

    //parse arg
    for(i = cmd_start+blank_count; i< strlen((const int8_t*)command); ++i){
        if(command[i] != ' '){
            for(j=i;j<strlen((const int8_t*)command);j++){
                file_arg[arg_start] = command[j];
                ++arg_start;                
            }    
            break;
        }
        else{
            if(file_arg_length > 0)
                break;
        }
    }

    if(read_dentry_by_name(file_cmd, &temp_dentry)==-1){
        return -1; 
    }

    else if(read_data(temp_dentry.inode_num, 0, elf_buf, sizeof(int32_t)) == -1){
        return -1;  
    }

    else if(!(elf_buf[0] == MAGIC0 && elf_buf[1] == MAGIC1 &&
         elf_buf[2] == MAGIC2 && elf_buf[3] == MAGIC3)) {
        return -1; 
    }

    //int pid_flag = 0;
    for(i = 0; i < 6;i++){         
        if(pid_array[i] == 0){
            parent_pid = cur_pid;
            pid_array[i] = 1;
            cur_pid = i;        
            break;
        }
    }


    if(i == 6){
            printf("pid full \n");
            return -1;
        }
    int holder_i = i;
    int k = 32;
    page_directory[k].rw = 1;
    page_directory[k].us = 1;
    page_directory[k].pt = 0;
    page_directory[k].pd = 0;
    page_directory[k].a = 0;
    page_directory[k].d = 0;
    page_directory[k].gp = 0;
    page_directory[k].p = 1;
    page_directory[k].res = 0;
    page_directory[k].ps = 1;         
    uint32_t phys_addr = (cur_pid * PAGE_4MB) + 0x800000;      
    page_directory[k].addrs = phys_addr >> 22;
    flush_tlb();

    read_data(temp_dentry.inode_num, (uint32_t)0, (uint8_t*)0x08048000,0x400000); 

    
    pcb_t* pcb_ptr = get_cur_pcb();
    pcb_ptr->parent_pid = parent_pid; 
    pcb_ptr->pid = cur_pid;          
    
    for (i = 0; i < 8; i++) {
        pcb_ptr->fd_array[i].fop_table_ptr = &null_fop;
        pcb_ptr->fd_array[i].inode = 0;
        pcb_ptr->fd_array[i].file_pos = 0;
        pcb_ptr->fd_array[i].flag = 1;
        if( i == 0 ){
            pcb_ptr->fd_array[i].fop_table_ptr = &stdin_fop;  
            pcb_ptr->fd_array[i].flag = 0;
        }
        else if( i == 1 ){
            pcb_ptr->fd_array[i].fop_table_ptr = &stdout_fop;  
            pcb_ptr->fd_array[i].flag = 0;
        }
    }
    strncpy((int8_t*)pcb_ptr->cmd_arg, (int8_t*)(file_arg), strlen((int8_t*)file_arg));

    uint8_t eip_buf[4];
    if(read_data(temp_dentry.inode_num, ELF_START, eip_buf, sizeof(int32_t)) == -1){
        //if(cur_pid != 0)pcb_ptr->pid = cur_pid;
        pid_array[holder_i] = 0;
        return -1;
    }

    eip_arg = eip_buf[0] + (eip_buf[1] << 8) + (eip_buf[2] << 16) + (eip_buf[3] << 24);
    esp_arg = 0x8400000-4;
    pcb_ptr->user_eip = eip_arg;
    pcb_ptr->user_esp = esp_arg;


    tss.ss0 = KERNEL_DS;
    tss.esp0 = EIGHT_MB - (EIGHT_KB*pcb_ptr->pid) - 4;

    pcb_ptr->tss_esp0 = tss.esp0;

    register uint32_t store_ebp asm("ebp");
    register uint32_t store_esp asm("esp");
    pcb_ptr->par_esp = store_esp;
    pcb_ptr->par_ebp = store_ebp;
       
    sti();
    asm volatile(
        "pushl   %1              ;"
        "pushl   %3              ;"
        "pushfl                  ;"
        "popl    %%eax           ;"
        "orl     $0x200,%%eax    ;"
        "pushl   %%eax           ;"
        "pushl   %2              ;"
        "pushl   %0              ;"
        :
        : "r"(eip_arg), "r"(USER_DS), "r"(USER_CS), "r"(esp_arg)
        : "%eax", "memory"
    );
    asm volatile(
        "iret                    ;"
    );
    return 0;
}

/* void fop_init()
 * Inputs      : none
 * Return Value: none
 * Function    :  initializes fop table   */
void fop_init(){
    null_fop.read = null_read;
    null_fop.write = null_write;
    null_fop.open = null_open;
    null_fop.close = null_close;

    rtc_fop.read = rtc_read;
    rtc_fop.write = rtc_write;
    rtc_fop.open = rtc_open;
    rtc_fop.close = rtc_close;

    dir_fop.read = dir_read;
    dir_fop.write = dir_write;
    dir_fop.open = dir_open;
    dir_fop.close = dir_close;

    file_fop.read = file_read;
    file_fop.write = file_write;
    file_fop.open = file_open;
    file_fop.close = file_close;

    stdin_fop.read = terminal_read;
    stdin_fop.write = null_write;
    stdin_fop.open = terminal_open;
    stdin_fop.close = terminal_close;

    stdout_fop.read = null_read;
    stdout_fop.write = terminal_write;
    stdout_fop.open = terminal_open;
    stdout_fop.close = terminal_close;
}


/* pcb_t* get_cur_pcb(){
 * Function: get addres to current pcb */
pcb_t* get_cur_pcb(){
    return (pcb_t*)(EIGHT_MB - EIGHT_KB*(cur_pid+1));
}
/* pcb_t* get_pcb(){
 * Function: get addres to pcb corresponding to input pid */
pcb_t* get_pcb(uint32_t pid){
    return (pcb_t*)(EIGHT_MB - EIGHT_KB*(pid+1));
}

int32_t getargs(uint8_t* buf, int32_t nbytes){
    if(buf == NULL){
        return -1;
    }
    pcb_t* cur_pcb = get_cur_pcb();
    if(cur_pcb->cmd_arg[0] == NULL){
        return -1;
    }
    strncpy((int8_t*)buf, (int8_t*)(cur_pcb->cmd_arg), nbytes);
    return 0;
}

int32_t null_read(int32_t fd, void* buf, int32_t nbytes){
    return -1;
}

int32_t null_write(int32_t fd, const void* buf, int32_t nbytes){
    return -1;
}

int32_t null_open(const uint8_t* fname){
    return -1;
}
int32_t null_close(int32_t fd){
    return -1;
}

int32_t vidmap(uint32_t** screen_start){


    return -1;
}
int32_t write(int32_t fd, void* buf, int32_t nbytes){
    if( fd < 0 || fd > 8){
        return -1;
    }
    if( nbytes <= 0 ){
        return -1;
    }
    if( buf == NULL ){
        return -1;
    }
    pcb_t* tPCB = get_cur_pcb();

    if(tPCB->fd_array[fd].flag == 1){
        return -1;
    }
    return (int32_t) tPCB->fd_array[fd].fop_table_ptr->write(fd,buf,nbytes);

}
int32_t read(int32_t fd, void* buf, int32_t nbytes){
    if( fd < 0 || fd > 8){
        return -1;
    }
    if( nbytes <= 0 ){
        return -1;
    }
    if( buf == NULL ){
        return -1;
    }
    pcb_t* tPCB = get_cur_pcb();

    if(tPCB->fd_array[fd].flag == 1){
        return -1;
    }
    fop_table_t ptr = *tPCB->fd_array[fd].fop_table_ptr;
    return ptr.read(fd,buf,nbytes);
    //return (int32_t)tPCB->fd_array[fd].fop_table_ptr->read(fd, buf, nbytes);
}

int32_t close(int32_t fd){
    if(fd < 0 || fd > 8){
        return -1;
    }

    pcb_t* tPCB = get_cur_pcb();
    tPCB->fd_array[fd].flag = 1;
    fop_table_t ptr = *tPCB->fd_array[fd].fop_table_ptr;
    return ptr.close(fd);
    //return (int32_t)tPCB->fd_array[fd].fop_table_ptr->close(fd);
}

int32_t open(const uint8_t* fname){
    int hold; 
    
    if (fname == NULL || strlen((char*)fname) == 0){
        return -1; 
    }
    // if(file_open(fname)== -1){
    //     return -1;
    // }
    dentry_t temp_dent;
    read_dentry_by_name(fname, &temp_dent);

    pcb_t* tPCB = get_cur_pcb();

    int32_t ftype; 
    ftype = temp_dent.filetype;

    for(hold = 2; hold < 8; hold++){
        if( tPCB->fd_array[hold].flag == 1){// check
            tPCB->fd_array[hold].file_pos = 0;
            tPCB->fd_array[hold].inode = temp_dent.inode_num;
            if(ftype == 0){
                tPCB->fd_array[hold].flag = 0;
                tPCB->fd_array[hold].fop_table_ptr = &rtc_fop;
            }
            if(ftype == 1){
                tPCB->fd_array[hold].flag = 0;
                tPCB->fd_array[hold].fop_table_ptr = &dir_fop;
            }
            if(ftype == 2){
                tPCB->fd_array[hold].flag = 0;
                tPCB->fd_array[hold].fop_table_ptr = &file_fop;
            }
            return hold;
            
        }
    }
    return -1;

}
