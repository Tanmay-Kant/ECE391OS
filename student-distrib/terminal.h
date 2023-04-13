#ifndef TERMINAL_H
#define TERMINAL_H

#include "lib.h"
#include "types.h"
#include "keyboard.h"
#include "terminal.h"

#define buffer_size (128) //size of the buffer

extern int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes);
extern int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes);
extern int32_t terminal_open(const uint8_t* fname);
extern int32_t terminal_close(int32_t fd);


extern void keyboard_buffer(uint8_t output); //store output from keyboard into keyboard buffer


#endif /* TERMINAL_H */
