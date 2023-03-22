#include "rtc.h"


/*reference: https://wiki.osdev.org/RTC */
/* rtc_init()
* Inputs: none
* Outputs: none
* Function: Initializes the RTC with the default 1024 Hz rate
*/

void rtc_init(){ 
    cli();
    outb(0x8B, 0x70); //select Register B, and disable NMI
    char prev = inb(0x71); //read the current value of register B
    outb(0x8B, 0x70); //set the index again
    outb(prev | 0x40, 0x71); //write the previous value ORed with 0x40. This turns on bit 6 of Register B
    enable_irq(8); //enable IRQ with 8 for RTC IRQ num
    sti();
}

/* rtc_handler()
* Inputs: none
* Outputs: none
* Function: calls test_interrupts to test RTC and ensures another interrupt is received by selecting register C
*/


void rtc_handler(){
    cli();
    test_interrupts(); //used to test RTC, increments video memory
    outb(0x0C, 0x70); //select register C
    inb(0x71);        //throw away contents
    sti();
    send_eoi(8); //send EOI with 8(RTC IRQ num)
}
