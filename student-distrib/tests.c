#include "tests.h"
// #include "x86_desc.h"
// #include "lib.h"
#include "types.h"
#include "terminal.h"

#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	//printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	//printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}

// add more tests here

/* Exception Test - Example
 * 
 * causes a divide by zero exception error
 * Inputs: None
 * Outputs: FAIL
 * Side Effects: None
 */


void divide_by_zero_exception_test(){
	//clear();
	TEST_HEADER;
	int zero = 0;
	int rand = 12;
	int result;
	result = rand / zero;

	//return 0;

}

/* Exception Test - Example
 * 
 * Tests any corresponding exception
 * Inputs: None
 * Outputs: prints the corresponding exception based on the instruction below
 * Side Effects: None
 */
void general_exception_test(){
	asm volatile("int $0x11"); //0x11 produces machine check
}

void sys_call_test(){
	asm volatile("int $0x80");
}

void rtc_test(){
	int i;
	for(i = 0; i < 10; i++){
		rtc_handler();
	}
}


/* Exception Test - Example
 * 
 * dereferencing a NULL pointer, should cause an exception
 * Inputs: None
 * Outputs: prints the corresponding exception based on the instruction below
 * Side Effects: None
 */
// void dereferencing_null_test(){
// 	int* ptr = NULL;
// 	int x;
// 	x = *ptr;

// 	int *a = 0xb8001;
//   	printf("%x\n", *a);
// }

/* Checkpoint 2 tests */

/* Terminal Driver Test
 * 
 * Calls and checks functionalities for both terminal read and write functions
 * Inputs: None
 * Outputs: integer value - # of bytes successfully copied
 * Side Effects: Terminal read and write functions are both implemented and called
 */
int terminal_driver_test(){
	TEST_HEADER;
	int nbytes;
	unsigned char buf[128];
	nbytes = terminal_read(0, buf, 140); //140 refers to the earlier known number of bytes
	return terminal_write(0, buf, nbytes);
}

/* RTC Open Test
 * 
 * Checks to see printing of 1s as a result of JUST the open() function
 * Inputs: None
 * Outputs: PASS if passed
 * Side Effects: Prints series of 1s at default value 
 */
int open_test(){
	open(NULL);
	int i;
	//magic number of 5 is because that seems like a reasonable number to test just open function
	for (i = 0; i < 5; i++) {
		read(NULL, NULL, NULL);
		printf("%d", 1);
	}
	return PASS;
}

/* RTC Write Test
 * 
 * Checks to see rate of 1s printed at different frequency values
 * Inputs: None
 * Outputs: PASS if passed
 * Side Effects: Prints series of 1s at the range of appropriate freq values 
 */
int write_test(){
	open(NULL);
	int a, b;
	for (a = 2; a < 1025; a *= 2) { //the upper bound is because we should limit user space program to 1024 Hz
		printf("Freq is %d: \n", a);
		//printf(" %d: ", a);
		write(NULL, (const void *)a, NULL);
		//printf("got past");

		/*each of these if statements has bounds of b that are reasonable compared to the number of 1s being printed*/

		if (a == 2 || a == 4 || a == 8 || a == 16 || a == 32) {
			for (b = 0; b < a * 2; b++) {
				//printf("before read");
				read(NULL, NULL, NULL);
				//printf("after read");	
				printf("%d", 1);
			}
			printf("\n");
		} else if (a == 64 || a == 128) {
			for (b = 0; b < a; b++) {
				//printf("before read");
				read(NULL, NULL, NULL);
				//printf("after read");	
				printf("%d", 1);
			}
			printf("\n");
		// } else if (a == 1024) {
		// 	for (b = 0; b < 120; b++) {
		// 		//printf("before read");
		// 		read(NULL, NULL, NULL);
		// 		//printf("after read");	
		// 		printf("%d", 1);
		// 	}
		// 	printf("\n");
		} else {
			for (b = 0; b < (a/4); b++) {
				//printf("before read");
				read(NULL, NULL, NULL);
				//printf("after read");	
				printf("%d", 1);
			}
			printf("\n");
		}
	}
	return PASS;

	// open(NULL);
	// int32_t passOrFail =  write(NULL, (const void *)32, NULL);
	// if (passOrFail == -1) {
	// 	return FAIL;
	// }
	// int iijk;
	// for (iijk = 0; iijk < 5; iijk++) {
	// 	read(NULL, NULL, NULL);
	// 	printf("%d", 1);
	// }
	// return PASS;
}


/* test_file_System
 * 
 * Checks to see if the read data function works as intended
 * Inputs: None
 * Outputs: Prints all the data about the file and the file 
 * 
 */

void test_file_system(){
	clear();
	dentry_t dentry;
	uint8_t file_names [FILE_SYS_NAME_LEN]  = "frame0.txt";


		int j = 0;
		// creates buffer to pass through - 4000 arbitrary large number
		uint8_t buf[4000];
		clear();
		printf("Searching for %s\n", file_names);
		printf("Read dentry by name: %x\n", read_dentry_by_name(file_names, &dentry));
		printf("Filename: %s\n", dentry.filename);
		printf("Filetype: %d\n", dentry.filetype);
		printf("Inode: %d\n", dentry.inode_num);
		printf("\n");
		//if (dentry.filetype == 2){

			printf("Reading file\n");
            //for(j = 0; j < 6; j++){
			// does read data for length much larger than the file to see if it handles it incorrectly - 4000 arbitrary large number
            read_data(dentry.inode_num, 0, buf, 4000);
			for(j = 0; j < 4000; j++){
        
            	putc(buf[j]);

            //}

			}
	}


/* test_fr
 * 
 * Checks to see if the file_read function is working correctly
 * Inputs: None
 * Outputs: Prints all the data about the file and the file 
 * 
 */
void test_fr(){
	clear();
	uint8_t fname [FILE_SYS_NAME_LEN]  = "frame0.txt";
	// creates buffer to pass through - 4000 arbitrary large number
	uint8_t buf[4000];
	file_read(fname, buf, 4000);
	printf("%s", buf);
}

/* ioctl_calls_fs()
 * 
 * Checks to see if the other functions of file sys aka file write, open, close and dir write, open, close work as intended
 * Inputs: None
 * Outputs: Prints all the data about the file and the file 
 * 
 */
void ioctl_calls_fs(){
	// function calls and prints output
	printf( "File open: %d\n", file_open((uint8_t *)"frame0.txt"));
	printf( "File write: %d\n", file_write((uint8_t *)"frame0.txt"));
	printf( "File close: %d\n", file_close((uint8_t *)"frame0.txt"));
	printf( "Dir open: %d\n", dir_open(2));
	printf( "Dir write: %d\n", dir_write(2));
	printf( "Dir close: %d\n", dir_close(2));
}

/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */

/* Test suite entry point */
void launch_tests(){
	//rtc_test();
	//TEST_OUTPUT("idt_test", idt_test());
	//divide_by_zero_exception_test();
	//general_exception_test();

	//dereferencing_null_test();


	/* terminal driver tests */
	// while(1){
	// terminal_driver_test();
	// }

	/* rtc tests */
	// clear();
	// open_test();
	// clear();
	// write_test();


	/* file system tests */
	//test_file_system();
	//test_fr();
	// uint8_t buf[4000];
	// dir_read(0, buf, 0);
	// ioctl_calls_fs();


	//launch your tests here
}
