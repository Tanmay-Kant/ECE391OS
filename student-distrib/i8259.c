/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {

    //disables all interrupts and initializes masks
    master_mask = mask; 
    slave_mask = mask; 


    outb(ICW1, MASTER_8259_PORT);
    outb(ICW1, SLAVE_8259_PORT);

    // + 1 is offset to go from the command port to the data ports
    outb(ICW2_MASTER, MASTER_8259_PORT + 1);
    outb(ICW2_SLAVE, SLAVE_8259_PORT + 1);

    // + 1 is offset to go from the command port to the data ports 
    outb(ICW3_MASTER, MASTER_8259_PORT + 1);
    outb(ICW3_SLAVE, SLAVE_8259_PORT + 1);

    // sets into 8086 mode 
    // + 1 is offset to go from the command port to the data ports
    outb(ICW4, MASTER_8259_PORT + 1);
    outb(ICW4, SLAVE_8259_PORT + 1);


    // restores masks to data 
    // + 1 is offset to go from the command port to the data ports
    outb(slave_mask, SLAVE_8259_PORT + 1);
    outb(master_mask, MASTER_8259_PORT + 1);
    
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    if(irq_num > irq_max){return;}
    
    if(irq_num > irq_max_master){
        // fixes new value to the slave mask 
        slave_mask = slave_mask & ~(1 << (irq_num - irq_max_master));
        // + 1 is offset to go from the command port to the data ports 
        outb(slave_mask, SLAVE_8259_PORT + 1);
    }
    else{
        // fixes new value to the master mask 
        master_mask = master_mask & ~(1 << irq_num);
        // + 1 is offset to go from the command port to the data ports 
        outb(master_mask, MASTER_8259_PORT + 1);
    }
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    if(irq_num > irq_max){return;}
    
    if(irq_num > irq_max_master){
        // fixes new value to the slave mask
        slave_mask = slave_mask | (1 << (irq_num - irq_max_master));
        // + 1 is offset to go from the command port to the data ports 
        outb(slave_mask, SLAVE_8259_PORT + 1);
    }
    else{
        // fixes new value to the master mask 
        master_mask = master_mask | (1 << irq_num);
        // + 1 is offset to go from the command port to the data ports
        outb(master_mask, MASTER_8259_PORT + 1);
    }
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
    if(irq_num > irq_max){return;}
    if(irq_num < irq_max_master){
        outb((EOI | irq_num), MASTER_8259_PORT);
    }
    else{
        // - 2 is mask save irq - 2nd pic
        outb((EOI | 0x02), MASTER_8259_PORT);
        outb(EOI | (irq_num - irq_max_master), SLAVE_8259_PORT);
    }
}
