#include "terminal.h"

//static uint8_t kb_buffer[buffer_size]; //buffer to store chars from keyboard
// static uint32_t num_char = 0; //keeps track of the number of chars
static uint32_t enter_flag = 0; //keeps track of whether enter is pressed
uint32_t cur_term_id = 0; 


/* cur_term_idx
* Inputs: none
* Outputs: none
* Function: global call that tells what terminal is currently being outputted
*/
uint32_t cur_term_idx(){
    return cur_term_id;
}

/* terminal_init
* Inputs: none
* Outputs: none
* Function: initializes all the bits of the terminal for saved data during initialization.
*/
void terminal_init(){
    int t_id = 0;
    clear();
    reset_cursor();
    for(t_id = 0; t_id < 3; t_id++){
        // sets the positions to 0
        tids[t_id].x_pos = 0; 
        tids[t_id].y_pos = 0; 
        // says if it is the terminal that is being displayed
        tids[t_id].active_flag = 0; 
        // maps the memory address to the terminal
        tids[t_id].vidmem = 0xB8 + 1 + t_id; 
        // sets the index for where in buffer the current character is
        tids[t_id].kbrd_idx = 0;
        // checks if a shell is ran in the specific terminal later
        tids[t_id].shell = 0;
        terminal_switch(t_id);
        //execute((const uint8_t *)"shell");
    }
    terminal_switch(0);
    //execute((const uint8_t *)"shell");
    return;

}


/* terminal_read(int32_t fd, uint8_t *buf, int32_t nbytes)
* Inputs: fd - file descriptor of the opened file, buf - address of the buffer
*         in the process's address space to which the data will be transferred
          nbytes - the number of bytes to read
* Outputs: bytes_read - num of bytes successfully copied
* Function: copies from the keyboard handler's kb_buff to function argument buf
* Return Value: returns num bytes successfully copied
*/
int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes){
    uint32_t i;
    uint32_t bytes_read = 0;
    if(nbytes <= 0){ //check if they are 0 number of bytes
        return 0;
    }

    if(buf == NULL){ //null check
        return -1;
    }

    while(enter_flag == 0){} //performs terminal read until enter is pressed

    cli();


    if(nbytes < buffer_size){
        for(i = 0; i < nbytes; i++){
            ((char*) buf)[i] = tids[cur_term_id].key_buf[i]; //copy from keyboard_handlers buffer to function argument buf
            tids[cur_term_id].key_buf[i] = ' '; //clears the keyboard_handler buffer after copying
            if(((char*) buf)[i] == '\n'){ //check if the newline is written and is smaller than nbytes
                bytes_read = i + 1;
            }

            if((i == nbytes - 1) && ((char*) buf)[i] != '\n'){ //if nbytes is reached and last char is not a newline
                ((char*) buf)[i] = '\n'; //set char to newline
                bytes_read = i + 1;
            }
        }
    }
    else{
        for(i = 0; i < buffer_size; i++){ //if nbytes is >= the size of the buffer
            ((char*) buf)[i] = tids[cur_term_id].key_buf[i]; //copy from keyboard_handlers buffer to function argument buf
            tids[cur_term_id].key_buf[i] = ' '; //clears the keyboard_handler buffer after copying

            if(((char*) buf)[i] == '\n'){ //check for newline char
                bytes_read = i + 1;
            }
        }
    }
    tids[cur_term_id].kbrd_idx = 0;   //reset the kb char buffer
    enter_flag = 0; //reset the enter flag
    sti();


    return bytes_read;
    
}


/* terminal_write(int32_t fd, uint8_t *buf, int32_t nbytes)
* Inputs: fd - file descriptor of the opened file, buf - address of the buffer
*         in the process's address space to which the data will be transferred
          nbytes - the number of bytes to read
* Outputs: bytes_read - num of bytes successfully copied
* Function: prints the contents of the argument buffer to the screen
* Return Value: returns num bytes successfully copied
*/

int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes){
    uint32_t i;
    char curr_char;
    if((buf == NULL) || (nbytes <= 0)){ //NULL check 
       return -1;
    }

    
    for(i = 0; i < nbytes; i++){
        curr_char = ((char*) buf)[i];
        if(curr_char != '\0'){ //check for a null byte
            putc(curr_char); //prints function argument buffer to the screen
        }
    }
        
    return nbytes;
    
}


/* terminal_open(const uint8_t* filename)
* Inputs: none
* Outputs: none
* Function: Initializes terminal stuff
* Return Value: returns 0
*/
int32_t terminal_open(const uint8_t* filename){
    return 0;
}


/* terminal_close(int32_t fd)
* Inputs: file descriptor of the opened file
* Outputs: none
* Function: Initializes terminal stuff
* Return Value: returns 0
*/
int32_t terminal_close(int32_t fd){
    return 0;
}


/* keyboard_buffer(uint8_t output)
* Inputs: output - keyboard char from keyboard driver
* Outputs: 
* Function: stores keyboard char from keyboard driver to a kb buffer which is used for
*           terminal_read and terminal_write
*/
void keyboard_buffer(uint8_t output){

    if(output == 0x08){    //check if keyboard char is backspace
        if(tids[cur_term_id].kbrd_idx > 0){       //check if there is a char before backspace is pressed
            if(tids[cur_term_id].kbrd_idx <= buffer_size){ //check if number of chars has not exceeded buffer size
                tids[cur_term_id].key_buf[tids[cur_term_id].kbrd_idx] = output;
                tids[cur_term_id].kbrd_idx--; //decrement number of chars
            }
        }
    }
    else if (output == '\n'){ //if keyboard char is newline

        enter_flag = 1;

        if(tids[cur_term_id].kbrd_idx >= buffer_size){
            tids[cur_term_id].key_buf[buffer_size - 1] = '\n'; //set newline if number of chars has exceeded buffer size     
        }
        else{
            tids[cur_term_id].key_buf[tids[cur_term_id].kbrd_idx] = '\n'; //add newline to keyboard buffer
        }
    }
    else if(tids[cur_term_id].kbrd_idx <= buffer_size){ //check if buffer size is exceeded limit
        tids[cur_term_id].key_buf[tids[cur_term_id].kbrd_idx] = output; //store char into kb buffer
        tids[cur_term_id].kbrd_idx++; //increment the number of chars
    }
    else{
       tids[cur_term_id].kbrd_idx++; 
    }
}


/* terminal_switch
* Inputs: terminal
* Outputs: none
* Function: Switches which terminal is being outputted and saves all necessary data
*/
void terminal_switch(uint32_t t_id){
    // bounds check and check if it is a change or not
    // store old data into the terminal struct
    // restore with new idx 
    // printf("terminal switching %x", t_id);
    // bounds check 
    if( t_id == cur_term_id || t_id > 2 || t_id < 0){
        return; 
    }
    if (t_id == cur_term_id){
        // used to radjust for the case when it added an extra space when switching terminals 
        putc_backspace();
        return;
    }    
    uint32_t old_t = cur_term_id;
    // saves data for the specific terminal
    lib_saves();
    tids[cur_term_id].active_flag = 0;
    // adjusts the terminal that is being outputted
    cur_term_id = t_id;

    // sets the page table at vidmem page to the original values
    page_table[0xB8].us = 1;
    page_table[0xB8].rw = 1; 
    page_table[0xB8].p = 1;
    page_table[0xB8].addr = 0xB8;
    flush_tlb();

    // saves the memory of the current video memory 
    //into a buffer for the specific terminal which is going to stop being displated
    memcpy((void*) ((0xB9 + old_t) * 0x1000), (const void*) (0xB8 * 0x1000), 0x1000);
    // sends data from the buffer address for the specific terminal to the video memory page
    memcpy((void*) (0xB8 * 0x1000), (const void*) ((0xB9 + cur_term_id)* 0x1000), 0x1000);


    // sets the cursor and other data back to the saved data    
    lib_restores();

    // sets that the specific terminal is being outputted to
    tids[t_id].active_flag = 1;


}

