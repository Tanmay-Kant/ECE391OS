#include "keyboard.h"

/* Reference: https://wiki.osdev.org/Interrupts#From_the_keyboard.27s_perspective */

void keyboard_init(){
    enable_irq(1); //enable IRQ1 for Keyboard Interrupt
}

void keyboard_handler(){

    inb(0x60); //reads data port from the keyboard controller
    send_eoi(1); //send end of interrupt with IRQ1(keyboard interrupt)
}
