#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "rtc.h"

#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

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
	i = 1/0;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) &&
			(idt[i].offset_31_16 == NULL)){
			// printf("%d\n", i);
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}

// add more tests here

void test_page() {
	// int * lowInvalidAddr = 0x0;
	// int * low2InvalidAddr = 0x000B7FF0;
	int * videoMem = 0x000B8040;
	// int * highInvalidAddr = 0x00800010;
	// x = 0x400000
	// printf("Invalid Low Address: %d\n", *lowInvalidAddr);
	printf("Valid Address: %d\n", *videoMem);
	// printf("Invalid low 2 Address: %d\n", *low2InvalidAddr);
	// printf("Invalid High Address: %d\n", *highInvalidAddr);
}

void test_keyboard() {
	printf("print the follow characters: abc123yee");

}

// void test_handlers() {
// 	printf("print the follow characters: abc123yee");
//
// }

/* Checkpoint 2 tests */
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	// TEST_OUTPUT("idt_test", idt_test());
	// test_interrupts();
	//test_keyboard();
	// launch your tests here
	test_page();
}
