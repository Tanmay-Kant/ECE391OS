#include "scheduler.h"
#define ASM 1

uint32_t shell_tid = 0;

void pit_init(){

    outb(PIT_MODE, PIT_MODE_CMD_PORT); 
    int freq_divisor = PIT_FREQUENCY / 100; 

    outb(freq_divisor & 0xFF, PIT_DATA_PORT);
    outb((freq_divisor & 0xFF00) >> 8, PIT_DATA_PORT);

    enable_irq(0);

}
void scheduler(){
    send_eoi(0x0);

    if(tids[2].shell == 0){
        tids[2].shell = 1; 
        terminal_switch(2);
        sti();
        execute((const uint8_t *)"shell");
    }
    else if(tids[1].shell == 0){
        tids[1].shell = 1; 
        terminal_switch(1);
        sti();
        execute((const uint8_t *)"shell");
    }
    else if(tids[0].shell == 0){
        tids[0].shell = 1; 
        terminal_switch(0);
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
