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

void keyboard_init(){
    enable_irq(1); //enables the keyboard interrupt line on the pic
}

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
