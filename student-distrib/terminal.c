#include "terminal.h"

static uint8_t kb_buffer[buffer_size]; //buffer to store chars from keyboard

static uint32_t num_char = 0; //keeps track of the number of chars
static uint32_t enter_flag = 0; //keeps track of whether enter is pressed



/* terminal_read(int32_t fd, uint8_t *buf, int32_t nbytes)
* Inputs: fd - file descriptor of the opened file, buf - address of the buffer
*         in the process's address space to which the data will be transferred
          nbytes - the number of bytes to read
* Outputs: bytes_read - num of bytes successfully copied
* Function: copies from the keyboard handler's kb_buff to function argument buf
* Return Value: returns num bytes successfully copied
*/
int32_t terminal_read(int32_t fd, uint8_t *buf, int32_t nbytes){
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
            buf[i] = kb_buffer[i]; //copy from keyboard_handlers buffer to function argument buf
            kb_buffer[i] = ' '; //clears the keyboard_handler buffer after copying
            if((buf[i]) == '\n'){ //check if the newline is written and is smaller than nbytes
                bytes_read = i + 1;
            }

            if((i == nbytes - 1) && (buf[i] != '\n')){ //if nbytes is reached and last char is not a newline
                buf[i] = '\n'; //set char to newline
                bytes_read = i + 1;
            }
        }
    }
    else{
        for(i = 0; i < buffer_size; i++){ //if nbytes is >= the size of the buffer
            buf[i] = kb_buffer[i]; //copy from keyboard_handlers buffer to function argument buf
            kb_buffer[i] = ' '; //clears the keyboard_handler buffer after copying

            if((buf[i]) == '\n'){ //check for newline char
                bytes_read = i + 1;
            }
        }
    }
    num_char = 0;   //reset the kb char buffer
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

int32_t terminal_write(int32_t fd, uint8_t* buf, int32_t nbytes){
    uint32_t i;

    if((buf == NULL) || (nbytes <= 0)){ //NULL check 
       return -1;
    }

        
    for(i = 0; i < nbytes; i++){
        if(buf[i] != '\0'){ //check for a null byte
            putc(buf[i]); //prints function argument buffer to the screen
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
        if(num_char > 0){       //check if there is a char before backspace is pressed
            if(num_char <= buffer_size){ //check if number of chars has not exceeded buffer size
                kb_buffer[num_char] = output;
                --num_char; //decrement number of chars
            }
        }
    }
    else if (output == '\n'){ //if keyboard char is newline

        enter_flag = 1;

        if(num_char >= buffer_size){
            kb_buffer[buffer_size - 1] = '\n'; //set newline if number of chars has exceeded buffer size     
        }
        else{
            kb_buffer[num_char] = '\n'; //add newline to keyboard buffer
        }
    }
    else if(num_char <= buffer_size){ //check if buffer size is exceeded limit
        kb_buffer[num_char] = output; //store char into kb buffer
        ++num_char; //increment the number of chars
    }
    else{
        ++num_char; 
    }
}