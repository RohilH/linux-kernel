#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "rtc.h"
#include "terminal.h"
#include "fileSystem.h"

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

void test_terminal() {
	int bytes = 1234;
	int32_t fdd = 0;
	char charBuffer[bytes];
	int read = terminalRead(fdd, charBuffer, bytes);
	terminalWrite(fdd, charBuffer, bytes);

	while(1) {
		read = terminalRead(fdd, charBuffer, bytes);
		terminalWrite(fdd, charBuffer, bytes);
	}


}

void test_RTC() {
	uint8_t buffer[1];
	int32_t numBytes = 1;
	int32_t i;
	for (i=0; i < 100; i++) {
		rtc_read(0, buffer, numBytes);
		printf("1");
	}
}

void test_page() {
	// int * lowInvalidAddr = (int*) 0x0;
	// int * low2InvalidAddr = (int*) 0x000B7FF0;
	int * videoMem = (int*) 0x000B8040;
	int * highInvalidAddr = (int*) 0x00800010;
	// x = 0x400000
	// printf("Invalid Low Address: %d\n", *lowInvalidAddr);
	printf("Valid Address: %d\n", *videoMem);
	// printf("Invalid low 2 Address: %d\n", *low2InvalidAddr);
	printf("Invalid High Address: %d\n", *highInvalidAddr);
}

void test_keyboard() {
	printf("print the follow characters: abc123yee");

}


void test_fileSys() {
	uint8_t buffer[1000000];
	// dentry_t testD;
	int32_t numBytes = 1000000;
	int32_t i;
	i = file_open("testprint");
	if (i == -1) return;
	file_read(2, buffer, numBytes);
	terminalWrite(1, buffer, numBytes);
	// uint8_t  nameOfFile[10];
	// uint8_t * nameOfFile;
	// char nameOfFile[100];
	// printf("Page Faulting 0?");
	// strncpy(nameOfFile, "verylargetextwithverylongname.txt", 100);

	// nameOfFile[0] = 'f';
	// nameOfFile[1] = 'r';
	// nameOfFile[2] = 'a';
	// nameOfFile[3] = 'm';
	// nameOfFile[4] = 'e';
	// nameOfFile[5] = '1';
	// nameOfFile[6] = '.';
	// nameOfFile[7] = 't';
	// nameOfFile[8] = 'x';
	// nameOfFile[9] = 't';
	//
	// int i;
	// // printf("Page Faulting 1?");
	// i = read_dentry_by_name(nameOfFile, &testD);
	// // printf("Page Faulting 2?");
	//
	// if (i == -1) {
	// 	printf("No file by that name");
	// 	return;
	// }
	// int bytesRead;
	// bytesRead = read_data(testD.inodeNum, 0, fileData, 100000);
	// printf("bytes read? %d\n", bytesRead);
	// // printf("Page Faulting 3?");
	//
	// for (i = 0; i < bytesRead; i++) {
	// 	putc(fileData[i]);
	// }
}

void test_divide0() {
	int i;
	int k = 1;
	int j = 1;
	i = 1/(j - k);
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
	// test_fileSys();
	test_RTC();
	// test_terminal();
	// test_page();
	// test_divide0();
}
