#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "lib.h"
#include "file_sys.h"
#include "rtc.h"
#include "paging.h"
#include "terminal.h"
#include "x86_desc.h"
#include "types.h"


#define MAX_FD_NUM 8
#define MAGIC0    0x7f
#define MAGIC1    0x45
#define MAGIC2    0x4c
#define MAGIC3    0x46

#define PROGRAM_IMAGE_ADDR 0x08048000

#define EIGHT_MB 0x800000
#define EIGHT_KB 0x2000

#define PROGRAM_IMAGE_OFFSET 0x48000

#define PID_MAX         8
#define PHYS_MEM_START  1 //modified for debug

#define ELF_START       24

#define ELF0    0
#define ELF1    1
#define ELF2    2
#define ELF3    3

/* initializes file operation table */
void fop_init();

/* get address to current pcb */
pcb_t* get_cur_pcb();

/* get address to pcb with input pid */
pcb_t* get_pcb(uint32_t pid);

int32_t null_read(int32_t fd, void* buf, int32_t nbytes);
int32_t null_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t null_open(const uint8_t* fname);
int32_t null_close(int32_t fd);

/* terminates a process, returning the specified value to its parent process */
int32_t halt(uint8_t status);

/* load and execute a new program, handling off the processor to the new program till it terminates */
int32_t execute(const uint8_t* command);

/* provides access to file system */
int32_t open(const uint8_t* fname);

/* closes the specified file descriptor and make it available for next open call */
int32_t close(int32_t fd);

/* reads data from keyboard ,file, device, or directory*/
int32_t read(int32_t fd, void* buf, int32_t nbytes);

/* writes data to the terminal or to a device*/
int32_t write(int32_t fd, void* buf, int32_t nbytes);

/* reads the program's command line arg into a user-level buffer */
int32_t getargs(uint8_t* buf, int32_t nbytes);

/* maps the text-mode video memory into user space at a pre-set virtual address */
int32_t vidmap(uint32_t** screen_start);

#endif
