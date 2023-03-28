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
int terminal_driver_test(){
	TEST_HEADER;
	int nbytes;
	unsigned char buf[128];
	nbytes = terminal_read(0, buf, 140); 
	return terminal_write(0, buf, nbytes);
}

int open_test(){
	open(NULL);
	int i;
	for (i = 0; i < 5; i++) {
		read(NULL, NULL, NULL);
		printf("%d", 1);
	}
	return PASS;
}

int write_test(){
	open(NULL);
	int a, b;
	for (a = 2; a < 1025; a *= 2) { //the upper bound is because we should limit user space program to 1024 Hz
		printf("Freq is %d: \n", a);
		//printf(" %d: ", a);
		write(NULL, (const void *)a, NULL);
		//printf("got past");

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

	// open();
	// int32_t passOrFail =  write(133);
	// if (passOrFail == -1) {
	// 	return FAIL;
	// }
	// int iijk;
	// for (iijk = 0; iijk < 5; iijk++) {
	// 	read();
	// 	printf("%d", 1);
	// }
	// return PASS;

	// return -5555;
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
<<<<<<< HEAD
	//dereferencing_null_test();
	// while(1){
	// terminal_driver_test();
	// }
	// clear();
	//open_test();
	clear();
	write_test();
=======
	dereferencing_null_test();
>>>>>>> origin/master
	//launch your tests here
}
