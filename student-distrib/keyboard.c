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
    enable_irq(1); //enable IRQ1 for Keyboard Interrupt
}

void keyboard_handler(){
    uint8_t buttons = inb(0x60);   // Intake key pressed
    if(buttons < 0x58)     // Make sure key is within range used
    {
        putc(buttons_data[buttons]);       // Place key pressed into memory and take from ASCII
        send_eoi(1);     // End of interrupt after press
    }
    else
    {
        send_eoi(1);     //End of interrupt after press
    }

}
