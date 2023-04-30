/* types.h - Defines to use the familiar explicitly-sized types in this
 * OS (uint32_t, int8_t, etc.).  This is necessary because we don't want
 * to include <stdint.h> when building this OS
 * vim:ts=4 noexpandtab
 */

#ifndef _TYPES_H
#define _TYPES_H

#define NULL 0
#define FILE_SYS_NAME_LEN 32
#define   MAX_SPACES    1024      //Number of tables/pages in dir
#define   ALIGN_4KB		4096			 //(2^12)

#define MAX_FILENAME 32

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


// 1024 is size specified by discussion slides - 4096 2 ^ 12 and 1024 * 4
pde_t page_directory[1024] __attribute__((aligned (4096)));
pte_t page_table[1024] __attribute__((aligned (4096)));
pte_t page_table_vidmap[MAX_SPACES] __attribute__((aligned (ALIGN_4KB)));



//file sys stuff 

// dentry block - for filesys - stores info for linking dentry with file
// each filesys stores info in its own way
typedef struct __attribute__ ((packed)) dentry{
    int8_t filename[FILE_SYS_NAME_LEN];
    int32_t filetype;
    int32_t inode_num;
    int8_t reserved[24];
} dentry_t;

// boot block - first block that tells directory count, inode count, data blocks count, and has reserved space and directory entries defined also the entries and the reserved area
typedef struct __attribute__ ((packed)) boot_block{
    int32_t dir_count; 
    int32_t inode_count; 
    int32_t data_count; 
    int8_t  reserved[52];
    dentry_t direntries[63];
} boot_block_t;

// stores general information for the file such as amount of data blocks
typedef struct __attribute__ ((packed)) inode{
    int32_t length; 
    // 1023 specified in the lecture slides 
    int32_t data_block_num[1023];
}inode_t;

// 4096 - size as defined in appendix
typedef struct __attribute__ ((packed)) data_block{
    uint8_t data[4096];
}data_block_t;

/*file operation table*/
typedef struct {
    int32_t (*open)(const uint8_t* fname);
    int32_t (*close)(int32_t fd);
    int32_t (*read)(int32_t fd, void* buf, int32_t nbytes);
    int32_t (*write)(int32_t fd, const void* buf, int32_t nbytes);
} fop_table_t;

fop_table_t null_fop;   
fop_table_t rtc_fop;    
fop_table_t dir_fop;    
fop_table_t file_fop;   
fop_table_t stdin_fop;  
fop_table_t stdout_fop; 

/* file descriptor*/
typedef struct {
    fop_table_t* fop_table_ptr; // To implement in later checkpoints, when we implement wrap drivers around a unified file system call interface (like the POSIX API)
    uint32_t inode;
    uint32_t file_pos;
    uint32_t flag;
} file_descriptor_t;
typedef struct pcb pcb_t;
/* pcb */
struct pcb{
    file_descriptor_t fd_array[8];
    uint32_t pid;
    uint32_t parent_pid;
    uint32_t par_esp;
    uint32_t par_ebp;
    uint32_t tss_esp0;
    uint32_t user_eip;
    uint32_t user_esp;
    uint32_t scheduler_esp; 
    uint32_t scheduler_ebp; 
    //pcb_t* parent_ptr;
    uint8_t cmd_arg[FILE_SYS_NAME_LEN];
};

<<<<<<< Updated upstream
=======
typedef struct{ 
    //pcb_t* ac_pcb;
    uint32_t x_pos;
    uint32_t y_pos;
    uint32_t kbrd_idx; 
    uint32_t vidmem; 
    uint32_t prognum;
    uint8_t key_buf[128]; 
    uint32_t active_flag; 
    uint32_t shell;

} terminal_t ; 

//uint32_t cur_term_id = 0;
terminal_t tids[3]; 
>>>>>>> Stashed changes
inode_t * inode_ptr;
dentry_t * dentry_ptr;
boot_block_t * b_b_ptr;
data_block_t * d_b_ptr;


// 3/25 **TODO: Struct declarations for the file system"
// - node - length & data index 
// - data blocks - data 
// - dentry - name, type, node index, reserved 
// - boot - dentry, node, dataBlock, reserved, dentries

// - starting point for the system - defined sm where im p sure - out of struct
// - jump table stuff - out of struct

#endif /* ASM */

#endif /* _TYPES_H */
