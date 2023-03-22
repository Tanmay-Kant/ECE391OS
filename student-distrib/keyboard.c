#include "keyboard.h"

char buttons_data[0x58] = {
    0x0, 0x0, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', 0x0, ' ', 'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0x0, 0x0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0x0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/', 0x0, 0x0, 0x0, ' '
};

/* Reference: https://wiki.osdev.org/Interrupts#From_the_keyboard.27s_perspective */


 /* void keyboard_init();
 * Inputs: void
 * Return Value: none
 * Function: enables IRQ1 for Keyboard Interrupt*/

void keyboard_init(){
    enable_irq(1); //enables the keyboard interrupt line on the pic
}


 /* void keyboard_handler();
 * Inputs: void
 * Return Value: none
 * Function: Handler in order to read data port from the keyboard controller and prints the corresponding character 
 * to the screen based on what key is pressed and released which is then ended by send_eoi to indicate end of 
 * interrupt */


void keyboard_handler(){
    // x60 is a post for the keyboard data - 0x58 is the number of keys that could be interpreted
    uint8_t buttons = inb(0x60);   // grab data from the press
    if(buttons < 0x58)     // range check 
    {
        putc(buttons_data[buttons]);       // places key val to memory
        send_eoi(1);     // end of interrupt
    }
    else
    {
        send_eoi(1);     // end of interrupt
    }

}
