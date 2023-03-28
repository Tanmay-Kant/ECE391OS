#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "x86_desc.h"
#include "lib.h"
#include "types.h"
#include "i8259.h"
#include "terminal.h"

#define scancode_size (88) //total number of scan codes
#define ASCII_size (128) //total number of ascii values

uint8_t caps_flag;
uint8_t shift_flag;
uint8_t ctrl_flag;

void keyboard_init(); //initializes the keyboard
void keyboard_handler(); 

#endif /* KEYBOARD_H */
