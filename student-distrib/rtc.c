#include "rtc.h"

#define MAX_FREQ 1024

char rate = 0x0F; //default rate value of 15
int rtcFlag = 0;
int32_t frequency = 2;
int counter = 0;
/*reference: https://wiki.osdev.org/RTC */
/* rtc_init()
* Inputs: none
* Outputs: none
* Function: Initializes the RTC with the default 1024 Hz rate
*/
void rtc_init(){ 
    frequency = 2;
    outb(0x8B, 0x70); //select Register B, and disable NMI
    char prev = inb(0x71); //read the current value of register B
    outb(0x8B, 0x70); //set the index again
    outb(prev | 0x40, 0x71); //write the previous value ORed with 0x40. This turns on bit 6 of Register B
    
    // cli();
    // outb(0x8A, 0x70);
    // prev = inb(0x71); //read the current value of register B
    // outb(0x8A, 0x70);
    // outb((prev & 0xF0) | 15, 0x71);

    // sti();

    enable_irq(8); //enable IRQ with 8 for RTC IRQ num
    //printf("RTC Init");
}

/* rtc_handler()
* Inputs: none
* Outputs: none
* Function: calls test_interrupts to test RTC and ensures another interrupt is received by selecting register C
*/
void rtc_handler(){
    cli();

    //test_interrupts(); //used to test RTC, increments video memory
    // printf("RTC running");
    outb(0x0C, 0x70); //select register C
    inb(0x71);        //throw away contents
    send_eoi(8); //send EOI with 8(RTC IRQ num)
    sti();

    counter++; //tick value
    // printf("%d", counter);
    //printf("%d", frequency);
    // printf("above if statement handler\n");
    if (counter % (MAX_FREQ/frequency) == 0){ //virtualization -- checks to see period of time to reset
        // printf("inside if statement handler\n");
        // printf("%d   %d   %d\n", counter, 1024/frequency, frequency);
        rtcFlag = 0;
        counter = 0;
        // printf("%d   %d", rtcFlag, counter);
    }

    ///////sti();
    // printf("below if statement handler\n");

}

/* open()
* Inputs: Pointer to unsigned int uint8_t
* Outputs: 0 upon success, -1 upon failure
* Function: A system call specifically providing access to RTC files
*/
int32_t rtc_open (const uint8_t* filename) { /*const uint8_t* filename*/
    //unsigned int rate2 = calculateRate(2);

    // cli();
    // outb(0x70, 0x8B);		// select register B, and disable NMI
    // char prev=inb(0x71);	// read the current value of register B
    // outb(0x70, 0x8B);		// set the index again (a read will reset the index to register D)
    // outb(0x71, prev | 0x40);	// write the previous value ORed with 0x40. This turns on bit 6 of register B
    // sti(); 


    /*from wiki, so this one switched*/
    // outb(0x70, 0x8A);
    // char prevv = inb(0x71); //read the current value of register B
    // outb(0x70, 0x8A);
    // outb(0x71, (prevv & 0xF0) | 15);

    // outb(0x8A, 0x70);
    // char prevv = inb(0x71); //read the current value of register B
    // outb(0x8A, 0x70);
    // outb((prevv & 0xF0) | 15, 0x71);

    /*null check*/
    if (filename == NULL) {
        return -1;
    }

    frequency = 2; //set frequency to instructed default of 2Hz
    return 0;

}

/* read()
* Inputs: A 32-bit signed integer fd , a pointer to a buffer, and a 32-bit signed integer nbytes representing number of bytes
* Outputs: Always 0 (but only after interrupt)
* Function: A system call that reads in data from RTC device and keeps flag set until the interrupt handler clears it
*/
int32_t rtc_read (int32_t fd, void* buf, int32_t nbytes) { /*int32_t fd, void* buf, int32_t nbytes*/
    rtcFlag = 1; /*set flag to 1*/
    /*kept in while loop until flag is cleared*/
    while (rtcFlag == 1); //{printf("stuck in read\n");};
    // printf("read went thru\n");
    return 0;

}

/* write()
* Inputs: A 32-bit signed integer fd , a pointer to a buffer, and a 32-bit signed integer nbytes representing number of bytes
* Outputs: Returns number of bytes written out upon success, and -1 on failure
* Function: Writes information regarding the desired interrupt rate
*/
int32_t rtc_write (int32_t fd, const void* buf, int32_t nbytes){ /*(int32_t fd, const void* buf, int32_t nbytes*/

    // frequency = ((int32_t*) buf);
    // frequency = (int32_t) buf;
    // int32_t derivedRate = calculateRate(frequency);

    // if ((derivedRate && (derivedRate - 1)) != 0) {
    //     return -1;
    // }

    // derivedRate &= 0x0F;			// rate must be above 2 and not over 15
    // //disable_ints();
    // cli();
    // outb(0x70, 0x8A);		// set index to register A, disable NMI
    // char prev=inb(0x71);	// get initial value of register A
    // outb(0x70, 0x8A);		// reset index to A
    // outb(0x71, (prev & 0xF0) | derivedRate); //write only our rate to A. Note, rate is the bottom 4 bits.
    // //enable_ints();
    // sti();

    // return sizeof(derivedRate);

    /*null check*/
   if (buf == NULL){
        return -1;
    }

    frequency = *((int32_t*)buf);
    int32_t derivedRate = calculateRate(frequency);

    if ((derivedRate && (derivedRate - 1)) != 0) {
        return -1;
    }

    //sourced from wiki OS Dev
    derivedRate &= 0x0F;			// rate must be above 2 and not over 15
    //disable_ints();
    cli();
    outb(0x8A, 0x70);		// set index to register A, disable NMI
    char prev=inb(0x71);	// get initial value of register A
    outb(0x8A, 0x70);		// reset index to A
    outb((prev & 0xF0) | derivedRate, 0x71); //write only our rate to A. Note, rate is the bottom 4 bits.
    //enable_ints();
    sti();

    return sizeof(derivedRate);
}

/* close()
* Inputs: 32-bit signed integer fd
* Outputs: 0 when successful, -1 if failure
* Function: Closes the file to allow for return from later "open" calls
*/
int32_t rtc_close (int32_t fd) {
    return 0;
}

/* calculateRate()
* Inputs: 32-bit unsigned integer f
* Outputs: Integer ranging from 6-15 specifying calculated equivalent rate value, or -1 if invalid range
* Function: Calculates and returns rate given a frequency using the formula: frequency =  32768 >> (rate-1)
*/
int32_t calculateRate (uint32_t f) {

    /*series of if-else statements determining appropriate calculated value for rate given frequency input parameter*/
    if (f == 2) {
        return 15;
    } else if (f == 4) {
        return 14;
    } else if (f == 8) {
        return 13;
    } else if (f == 16) {
        return 12;
    } else if (f == 32) {
        return 11;
    } else if (f == 64) {
        return 10;
    } else if (f == 128) {
        return 9;
    } else if (f == 256) {
        return 8;
    } else if (f == 512) {
        return 7;
    } else if (f == 1024) {
        return 6;
    } else {
        return -1;
    }
}
