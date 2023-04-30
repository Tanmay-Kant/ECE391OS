#include "scheduler.h"
#define ASM 1


/* PIT_INIT
* Inputs: none
* Outputs: none
* Function: initializes the Program Interval Timer which we use to interrupt the original execute 
* to run shell in the other terminals as well
*/
void pit_init(){
    // initialization for the pit as specified on OSDEV
    outb(PIT_MODE, PIT_MODE_CMD_PORT); 
    int freq_divisor = PIT_FREQUENCY / 100; 

    // all data as specified on osdev page on PIT initalization -- FF mask grabs the 2 ascii characters
    outb(freq_divisor & 0xFF, PIT_DATA_PORT);
    outb((freq_divisor & 0xFF00) >> 8, PIT_DATA_PORT);
    // allows interrupts
    enable_irq(0);

}

/* scheduler
* Inputs: none
* Outputs: none
* Function: Used to call execute shell in each of our terminal - handler 
* that checks if shell is already running and if not it runs shell. Occurs
* at every PIT interrupt.
*/
void scheduler(){
    send_eoi(0x0);
    // checks if terminal 2 has a shell
    if(tids[2].shell == 0){
        tids[2].shell = 1; 
        terminal_switch(2);
        // sets shell active and does terminal switch
        sti();
        execute((const uint8_t *)"shell");
    }
    // checks if terminal 1 has a shell
    else if(tids[1].shell == 0){
        tids[1].shell = 1; 
        terminal_switch(1);
        // sets shell active and does terminal switch
        sti();
        execute((const uint8_t *)"shell");
    }
    // check if terminal 0 has a shell 
    else if(tids[0].shell == 0){
        tids[0].shell = 1; 
        terminal_switch(0);
        // sets shell active and does terminal switch
        sti();
        execute((const uint8_t *)"shell");
    }
    return;

    //return; 
    // if(shell_tid < 3){
    //     shell_tid++;
    //     if(tids[shell_tid].shell == 0 ){
    //         terminal_switch(shell_tid);
    //         tids[shell_tid].shell = 1;
    //         sti();
    //         execute((const uint8_t *)"shell");
    //     }
    // }
    // if(shell_tid == 3){
    //     terminal_switch(0);
    // }
    // sti();
}
