#include "rtc.h"

char rate = 0x0F;
/*reference: https://wiki.osdev.org/RTC */
/* rtc_init()
* Inputs: none
* Outputs: none
* Function: Initializes the RTC with the default 1024 Hz rate
*/

void rtc_init(){ 
    outb(0x8B, 0x70); //select Register B, and disable NMI
    char prev = inb(0x71); //read the current value of register B
    outb(0x8B, 0x70); //set the index again
    outb(prev | 0x40, 0x71); //write the previous value ORed with 0x40. This turns on bit 6 of Register B
    
    outb(0x8A, 0x70);
    prev = inb(0x71); //read the current value of register B
    outb(0x8A, 0x70);
    outb((prev & 0xF0) | rate, 0x71);

    
    enable_irq(8); //enable IRQ with 8 for RTC IRQ num
    printf("RTC Init");
}

/* rtc_handler()
* Inputs: none
* Outputs: none
* Function: calls test_interrupts to test RTC and ensures another interrupt is received by selecting register C
*/


void rtc_handler(){
    cli();
    // test_interrupts(); //used to test RTC, increments video memory
    printf("RTC running");
    outb(0x0C, 0x70); //select register C
    inb(0x71);        //throw away contents
    send_eoi(8); //send EOI with 8(RTC IRQ num)
    sti();
}
