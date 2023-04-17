#include "types.h"
#include "lib.h"
#include "systemcall.h"

#ifndef FILE_SYS_H
#define FILE_SYS_H

#define BOOT_BLOCK_SIZE 64  // boot block size in bytes (64B)
#define BLOCK_SIZE 4096     // block size in bytes (4KB)

#define DENTRY_RESERVED_BYTES 24
#define BOOT_RESERVED_BYTES   52
#define BOOT_DENTRY_NUM       63
#define INODE_DATA_BLOCK_NUM  1023  
#define FILE_SYS_NAME_LEN 32

int32_t file_sys_init(uint32_t file_sys_start);
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

// open read write close declarations for directory
int32_t file_open(const uint8_t* fname);
int32_t file_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t file_close(int32_t fd);
int32_t file_read(int32_t fd, void* buf, int32_t nbytes);

// open read write close declarations for directory
int32_t dir_open(const uint8_t* fname);
int32_t dir_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t dir_close(int32_t fd);
int32_t dir_read(int32_t fd, void* buf, int32_t nbytes);

//void test_file_system();


/*
 * TODO: 3/25 
 * 1 - given functions that they capped abt 
 *      read dentry name 
 *      read dentry index 
 *      read data 
 * 2 - init & file size call?
 * 3 - file functions
 *      read
 *      write
 *      open
 *      close
 * 4 - dir variation 
 *      read
 *      write 
 *      open
 *      close
 * 
 * tackle init stuff first probably 
 */ 



// file type 
/*
0 - user-level acces to the rtc
1 - directory
2 - regular file
*/

// file_open
// file_read
// file_write
// file_close


// dir_open
// dir_read
// dir_write
// dir_close



// i_mode file type & access rights (e.g., readable by group)
// i_uid owner
// i_size length in bytes
// i_atime time of last access
// i_ctime time of last creation
// i_mtime time of last modification
// i_dtime time of last deletion
// i_gid group id
// i_links_count # of hard links
// i_blocks blocks in file (disk blocks, in 512B units)
// i_flags e.g., immutable, append-only
// i_block[15] data block #’s
// i_generation generation (incremented on each modification)

#endif 
