#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "lib.h"
#include "file_sys.h"
#include "rtc.h"
#include "paging.h"
#include "terminal.h"
#include "x86_desc.h"
#include "types.h"

#define MAGIC0    0x7f
#define MAGIC1    0x45
#define MAGIC2    0x4c
#define MAGIC3    0x46


#define EIGHT_MB 0x800000
#define EIGHT_KB 0x2000


#define ELF_START       24

void fop_init();
pcb_t* get_cur_pcb();
pcb_t* get_pcb(uint32_t pid);

int32_t null_read(int32_t fd, void* buf, int32_t nbytes);
int32_t null_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t null_open(const uint8_t* fname);
int32_t null_close(int32_t fd);
int32_t halt(uint8_t status);
int32_t execute(const uint8_t* command);
int32_t open(const uint8_t* fname);
int32_t close(int32_t fd);
int32_t read(int32_t fd, void* buf, int32_t nbytes);
int32_t write(int32_t fd, void* buf, int32_t nbytes);
int32_t getargs(uint8_t* buf, int32_t nbytes);
int32_t vidmap(uint32_t** screen_start);

#endif
