#include "file_sys.h" 

// used to read file from same point afterwards
uint32_t pos = 0;
// uint32_t dFileNum = 0;


/*
 * file_sys_init
 * 
 * DESCRIPTION: initializes file system
 * INPUT: file_sys_start - starting address of the file system from the module installed in kernel.c
 * OUTPUT: -1 if it fails, 0 if it succeeds
 * 
 */ 

int32_t file_sys_init( uint32_t file_sys_start ){
    // start point grabbed from the mounting in kernel.c
    
    b_b_ptr = (boot_block_t *) file_sys_start;
    // int i;
    // for (i = 0; i < 16; i++) {
    //             printf("0x%x ", *((char*)(b_b_ptr+i)));
    //         }
    // jumps as defined in appendix
    inode_ptr = (inode_t *)(file_sys_start + 4096);
    // size of boot block as defined in the appendix 
    dentry_ptr = (dentry_t *) (file_sys_start + b_b_ptr->inode_count); 
    d_b_ptr = (data_block_t *)(file_sys_start)+ 1 + b_b_ptr->inode_count;
    // uint32_t FDir[b_b_ptr->dir_count];
    return 0; 
}




/*
 * read dentry by name
 * 
 * DESCRIPTION: finds the dentry of a file based on the file name and sets the dentry pointer to start there
 * INPUT: fname - name of the file, dentry - temporary dentry point which is used in later functions for calls such as read data
 * OUTPUT: -1 if it fails, 0 if it succeeds
 * 
 */ 
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry){
    // check if the name of the file is max 32 characters 
    /* if( strlen((int8_t*)fname) > 32){
        return -1;
    } */
   
    int i;
    int32_t file_found = -1; 
    if( fname == NULL || dentry == NULL){ return file_found;}
    // a flag to indicate if the file was found
    for (i = 0; i < b_b_ptr->dir_count; i++) { 
        // assuming boot_block contains the directory entry table
        if (strncmp((int8_t*)fname, (int8_t*)b_b_ptr->direntries[i].filename, FILE_SYS_NAME_LEN) == 0) {
            // populate dentry struct 
            memcpy(dentry, (dentry_t *)(&b_b_ptr->direntries[i]), sizeof(dentry_t));
            file_found = 0;
            break;
        }
    }
    return file_found;
}

/*
 * read dentry by index
 * 
 * DESCRIPTION: finds the dentry of a file based on the index and sets the dentry pointer to start there
 * INPUT: index - index of the file, dentry - temporary dentry point which is used in later functions for calls such as read data
 * OUTPUT: -1 if it fails, 0 if it succeeds
 * 
 */ 
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry){
    // bounds check 
    if(index < b_b_ptr->dir_count){
        // populate dentry
        memcpy(dentry, (dentry_t *)(&b_b_ptr->direntries[index]),sizeof(dentry_t));
        return 0;
    }
    return -1; 
}

/*
 * read data
 * 
 * DESCRIPTION: reads data based on the inode and puts it inito a buffer which can be used after
 * INPUT: inode - inode value that indexs to the data block they want, offset - starting position in file, buf - buffer that holds the 
 * character from read data, length the amount of bites that the call will print
 * OUTPUT: -1 if it fails, 0 if it succeeds
 * 
 */ 
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    
    // checks if the buffer is uninitialized
    if ( buf == NULL ){
        return -1;
    }
    // int bytes; 
    // for(bytes = 0; bytes < length; bytes++){
    //     buf[bytes] = '\0';
    // }
    // checks if inode is a valid number
    if(inode < 0){
        return -1; 
    }
    // checks if inode number is within in the range
    if(inode > b_b_ptr->inode_count){
        return -1;
    }
    // creates a temporary inode to access
    inode_t * inode_temp = (inode_t*)(&(inode_ptr[inode]));
    int temp_length = 0;
    // checks if the offset is invalid for specified inode
    if (!(offset < inode_temp->length)){
        return -1;
    }
    uint32_t file_flag = 1; 
    // checks if reader is trying to read past the scopoe of the file and adjusts length for this case 
    if( offset + length > inode_temp->length){
        // change length in the case if it got to the end of the file - temp length is extra characters
        temp_length = length;
        length = (inode_temp->length) - offset; 
        // flag that tells that you reach end of file
        file_flag = 0; 
    }
   

    
    uint32_t i; // iterator
    uint32_t currBlock; // index of which data blk
    uint32_t BLKindex;  // index in blk 

    // loop that iterates through the buffer and stores values to be outputted
    for( i = 0; i < length; i++){
        //if( offset + i >= inode_temp->length){return 0;}
        //if(( offset >= inode_temp->length)){return 0;}

        // figures out the data block - 4096 size of block 
        BLKindex =(uint32_t)(offset/4096);
        // grabs data block
        currBlock = inode_temp->data_block_num[BLKindex];

        // puts value into buffer - 4096 size of block 
        buf[i] = *((uint8_t*)(d_b_ptr + currBlock) + offset%4096);
        offset++;
    }
    // checks if the size of the buffer is greater than the out put string
    // if(temp_length != 0) {
    //     for(j = i + 1; j < temp_length; j++) {
    //         // clears the extra characters to fix them writing the wrong characters
    //         buf[j] = '\0';
    //     }
    // } 

    //returns case that we hit end of file
    //if (file_flag != 1){return 0;}
    return i; 
    
}

/*
 * file_open
 * 
 * DESCRIPTION: checks if the file exits and returns that it is accessible
 * INPUT: fname - supposed to check if file name exists and represents that it is accessible - more to implement in later cp
 * OUTPUT: -1 if it fails, 0 if it succeeds
 * 
 */ 
int32_t file_open(const uint8_t* fname){
    //return 0;
    dentry_t temp_d;
    return(read_dentry_by_name(fname, &temp_d));
}


/*
 * file_read
 * 
 * DESCRIPTION: checks the file and puts all the characters in a buffer - includes a read dentry call and read data call
 * INPUT: fname - name of the file, buf - buffer that the output is held in, length - the amound of bits it reads out loud. 
 * OUTPUT: -1 if it fails, 0 if it succeeds
 * 
 */ 
int32_t file_read(int32_t fd, void* buf, int32_t nbytes){
    // int32_t length = nbytes;
    // if( fname == NULL || buf == NULL || length <= 0){
    //     return -1;
    // }
    
    // //read_dentry_by_name(fname, &dentry_ptr);
    // // temporary dentry for file read
    // dentry_t dentry_fr;
    // // call to see if file exists
    // int32_t dentval = 0 ;
    // dentval = read_dentry_by_name(fname, &dentry_fr);
    // if(dentval == -1){return -1;}
    // // call to read data plus adjustment to pos so next call starts at same place
    // pos += read_data(dentry_fr.inode_num, pos, buf, length);
    // return 0;
    if ( buf == NULL){ 
        return -1; 
    }
    pcb_t* pcb_ptr = get_cur_pcb();

    uint32_t num_bytes = read_data(pcb_ptr->fd_array[fd].inode, pcb_ptr->fd_array[fd].file_pos, buf, nbytes);
    
    if(num_bytes == -1) {
        return -1;
    } 
    else {
        pcb_ptr->fd_array[fd].file_pos += num_bytes; 
    }
    return num_bytes;

}

/*
 * file_close
 * 
 * DESCRIPTION: checks the file and closes it 
 * INPUT: fname - name of the file, 
 * OUTPUT: -1 if it fails, 0 if it succeeds
 * 
 */ 
int32_t file_close(int32_t fd){ return 0; }


/*
 * file_write
 * 
 * DESCRIPTION: checks the file and writes to it - since read only always fails
 * INPUT: fname - name of the file, 
 * OUTPUT: -1 if it fails, 0 if it succeeds
 * 
 */ 
int32_t file_write(int32_t fd, const void* buf, int32_t nbytes){ return -1; }

/*
 * dir_open
 * 
 * DESCRIPTION: checks the file and tells whether it can be accessed - always returns 0 - more to implement in later cp 
 * INPUT: dir - directory index
 * OUTPUT: -1 if it fails, 0 if it succeeds
 * 
 */
int32_t dir_open(const uint8_t* fname) { 
    dentry_t temp_d;
    return(read_dentry_by_name(fname, &temp_d));
}

/*
 * dir_write
 * 
 * DESCRIPTION: checks the file and tells whether it can be written to - always returns -1
 * INPUT: dir - directory index
 * OUTPUT: -1 if it fails, 0 if it succeeds
 * 
 */
int32_t dir_write(int32_t fd, const void* buf, int32_t nbytes) { return -1; }

/*
 * dir_close
 * 
 * DESCRIPTION: checks the file and tells whether it can be close - always returns 0 
 * INPUT: dir - directory index
 * OUTPUT: -1 if it fails, 0 if it succeeds
 * 
 */
int32_t dir_close(int32_t fd) { return 0; }


// display file size, file name, file type; 
// pass a name to directory read
/*
 * dir_read
 * 
 * DESCRIPTION: checks the file and tells whether it can be accessed - and prints the directorys, filenames, types, and lengths 
 * INPUT: dir - directory index
 * OUTPUT: -1 if it fails, 0 if it succeeds
 * 
 */
int32_t dir_read(int32_t fd, void* buf, int32_t nbytes){
    //int32 t fd, void* buf, int32 t nbytes
    // return 0;
    uint32_t length = nbytes;
    length++;
    // buf[1] = "d";
    // buf[2] = "i";
    // buf[3] = "r";
    // initializes temporary dentry to be used through out
    dentry_t dir_dent; 
    int j;
    
    while( read_dentry_by_index(fd, &dir_dent) != -1){
        // checks the length of the name and if is longer than the specified maximum then it will stop at first 32
        length = strlen(dentry_ptr[fd].filename);
        if(length > FILE_SYS_NAME_LEN){
            length = FILE_SYS_NAME_LEN;
        }    
        printf("Filename: ");
        // loop to print file name character by character
        for( j = 0; j < length; j++ ){
            printf("%c", dentry_ptr[fd].filename[j]);
        }
        // prints the file type and the length of the file
        printf(" ");
        printf("Filetype: %d ", dentry_ptr[fd].filetype);
        printf("File Size: %d\n", inode_ptr[dentry_ptr[fd].inode_num].length);
        fd++;
    }
    //int32_t fdout = (int32_t*)fd;
    return 0;
    //read_dentry_by_index
}
/* 
 * im j doing these in the order they make most sense
 * init and size
 * the "given ones"
 * file calls - read
 * directory calls - read
 * 
 * file operations structure
 *  jump table for file operations / character driver operations 
 *  generic instance for files on disk 
 *  distinct instances for sockets 
 *  one instance per device type
 */ 

