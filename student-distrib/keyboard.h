#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "lib.h"
#include "i8259.h"

void keyboard_init(); //initializes the keyboard
void keyboard_handler(); 

#endif /* KEYBOARD_H */
