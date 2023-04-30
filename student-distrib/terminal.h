#ifndef TERMINAL_H
#define TERMINAL_H

#include "lib.h"
#include "types.h"
#include "keyboard.h"
#include "terminal.h"
#include "systemcall.h"

#define buffer_size (128) //size of the buffer

//extern uint32_t cur_term_id;

extern int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes);
extern int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes);
extern int32_t terminal_open(const uint8_t* fname);
extern int32_t terminal_close(int32_t fd);

void terminal_init();


extern void keyboard_buffer(uint8_t output); //store output from keyboard into keyboard buffer
<<<<<<< Updated upstream
=======
extern void terminal_switch(uint32_t t_id); 
extern uint32_t cur_term_idx();
>>>>>>> Stashed changes


#endif /* TERMINAL_H */
