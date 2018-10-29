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
	int bytes = 128;
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
		int32_t buffer[1];
		int32_t freqs[10] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
		int32_t numBytes = 1;
		int32_t i;
		int32_t x;

		rtc_write(0, &freqs[0], 4);
		while (i < 10) {
			rtc_read(0, buffer, numBytes);
			printf("a");
			i++;
		}
		while(x < 10) {
			rtc_read(0, buffer, numBytes);
			removec();
			x++;
		}

		rtc_write(0, &freqs[1], 4);
		while (i < 25) {
			rtc_read(0, buffer, numBytes);
			printf("b");
			i++;
		}
		while(x < 25) {
			rtc_read(0, buffer, numBytes);
			removec();
			x++;
		}

		rtc_write(0, &freqs[2], 4);
		while (i < 40) {
			rtc_read(0, buffer, numBytes);
			printf("c");
			i++;
		}
		while(x < 40) {
			rtc_read(0, buffer, numBytes);
			removec();
			x++;
		}

		rtc_write(0, &freqs[3], 4);
		while (i < 60) {
			rtc_read(0, buffer, numBytes);
			printf("d");
			i++;
		}
		while(x < 60) {
			rtc_read(0, buffer, numBytes);
			removec();
			x++;
		}


		rtc_write(0, &freqs[4], 4);
		while (i < 85) {
			rtc_read(0, buffer, numBytes);
			printf("e");
			i++;
		}
		while(x < 85) {
			rtc_read(0, buffer, numBytes);
			removec();
			x++;
		}

		rtc_write(0, &freqs[5], 4);
		while (i < 120) {
			rtc_read(0, buffer, numBytes);
			printf("f");
			i++;
		}
		while(x < 120) {
			rtc_read(0, buffer, numBytes);
			removec();
			x++;
		}

		rtc_write(0, &freqs[6], 4);
		while (i < 200) {
			rtc_read(0, buffer, numBytes);
			printf("g");
			i++;
		}
		while(x < 200) {
			rtc_read(0, buffer, numBytes);
			removec();
			x++;
		}

		rtc_write(0, &freqs[7], 4);
		while (i < 350) {
			rtc_read(0, buffer, numBytes);
			printf("h");
			i++;
		}
		while(x < 350) {
			rtc_read(0, buffer, numBytes);
			removec();
			x++;
		}

		rtc_write(0, &freqs[8], 4);
		while (i < 600) {
			rtc_read(0, buffer, numBytes);
			printf("i");
			i++;
		}
		while(x < 600) {
			rtc_read(0, buffer, numBytes);
			removec();
			x++;
		}

		rtc_write(0, &freqs[9], 4);
		while (i < 1000) {
			rtc_read(0, buffer, numBytes);
			printf("j");
			i++;
		}
		while(x < 1000) {
			rtc_read(0, buffer, numBytes);
			removec();
			x++;
		}
}

void test_RTC_invalid_freq() {
		int32_t buffer[1];
		int32_t freqs[2] = {1023, 2048};
		int32_t numBytes = 1;
		int32_t i;
		int32_t x;

		rtc_write(0, &freqs[0], 4);
		while (i < 10) {
			rtc_read(0, buffer, numBytes);
			printf("a");
			i++;
		}
		while(x < 10) {
			rtc_read(0, buffer, numBytes);
			removec();
			x++;
		}
		rtc_write(0, &freqs[1], 4);
		while (i < 20) {
			rtc_read(0, buffer, numBytes);
			printf("b");
			i++;
		}
		while(x < 20) {
			rtc_read(0, buffer, numBytes);
			removec();
			x++;
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

void test_dirRead() {
	int32_t fileNames[42];
	dir_read(2, fileNames, 32);
	// terminalWrite(1, fileNames, 32);
}
void test_fileSys() {
	uint8_t buffer[1000000];
	// dentry_t testD;
	int32_t numBytes = 1000000;
	int32_t i;
	int read_bytes;
	// replace with different file names
	i = file_open((uint8_t*)"counter");
	if (i == -1) return;
	read_bytes = file_read(2, buffer, numBytes);
	terminalWrite(1, buffer, read_bytes);
}

void test_divide0() {
	int i;
	int k = 1;
	int j = 1;
	i = 1/(j - k);
}

/* Test suite entry point */
void launch_tests(){
	// Checkpoint 1 Tests
	// test_interrupts();
	// test_keyboard();
	// test_page();
	// test_divide0();

	// Checkpoint 2 Tests
	// test_fileSys();
	// test_dirRead();
	// test_RTC();
	// test_RTC_invalid_freq();
	test_terminal();
}
