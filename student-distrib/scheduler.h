#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include "x86_desc.h"
#include "lib.h"
#include "types.h"
#include "i8259.h"
#define PIT_MODE 0x36 
#define PIT_DATA_PORT 0x40
#define PIT_MODE_CMD_PORT 0x43
#define PIT_FREQUENCY 1193182



void pit_init(); 
void pit_handler();


#endif

