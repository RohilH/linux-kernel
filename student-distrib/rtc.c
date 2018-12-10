#include "rtc.h"
#include "i8259.h"
#include "lib.h"
#include "terminal.h"

volatile int interruptFlag;

/*
 * RTC_INIT
 *     DESCRIPTION: Initializes the rtc
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 */
void RTC_INIT() {
  // SRC: https://wiki.osdev.org/RTC
  // RTC Virtualization
  currTerminalScheduler = 0;
  terminals[0].rtcInterruptFlag = FREQ_512 / FREQ_512;
  terminals[1].rtcInterruptFlag = FREQ_512 / FREQ_512;
  terminals[2].rtcInterruptFlag = FREQ_512 / FREQ_512;
  terminals[0].programFrequencyRTC = FREQ_512;
  terminals[1].programFrequencyRTC = FREQ_512;
  terminals[2].programFrequencyRTC = FREQ_512;

  // RTC init
  outb(REG_B, IO_PORT1);                // Idx --> Reg B / NMI_HANDLER
  char prev = inb(IO_PORT2);            // Obtain Reg A val
  outb(REG_B, IO_PORT1);                // Reset idx
  outb(prev | BIT_SIX_MASK, IO_PORT2);  // Reg B bit 6 mask
  enable_irq(IRQ_LINE_RTC);             // Enable RTC IRQ

  uint8_t rate = 0x0F;
  outb(REG_A, IO_PORT1);                // Idx --> Reg B / NMI_HANDLER
  prev = inb(IO_PORT2);                 // Obtain Reg A val
  outb(REG_A, IO_PORT1);                // Reset idx
  outb((prev & 0xF0) | rate, IO_PORT2);

  // Change frequency
  rtc_changeFreq(FREQ_512);
}

/*
 * rtc_read
 *     DESCRIPTION: Spins until an rtc interrupt is read
 *     INPUTS: fd, buf, nBytes
 *     OUTPUTS: 0
 *     RETURN VALUE: Returns 0 for success
 */
int32_t rtc_read(int32_t fd, void* buf, int32_t nBytes) {
  disable_irq(KEYBOARD_IRQ); //disable keyboard interrupts
  terminals[currTerminalScheduler].rtcInterruptFlag = FREQ_512 / terminals[currTerminalScheduler].programFrequencyRTC;
  //loop breaks out when interrupt is triggered
  while(terminals[currTerminalScheduler].rtcInterruptFlag > 0); {
    // Spin until interrupt is raised
  }
  enable_irq(KEYBOARD_IRQ); //reenable keyboard interrupts
  return SUCCESS;  // return success
}

/*
 * rtc_write
 *     DESCRIPTION: Changes the frequency and returns nBytes
 *     INPUTS: fd, buf, nBytes
 *     OUTPUTS: nBytes
 *     RETURN VALUE: 4 or -1
 */
int32_t rtc_write(int32_t fd, const void* buf, int32_t nBytes) {
  // ensure nbytes is 4
  if(nBytes != BYTE_CHECK) {
    // if not four bytes return -1
    return FAILURE;
  }
  terminals[currTerminalScheduler].programFrequencyRTC = *(int32_t*)buf;
  return nBytes; //return the number of bytes
}

/*
 * rtc_open
 *     DESCRIPTION: Returns if frequency changed correctly
 *     INPUTS: fileName
 *     OUTPUTS: rtc_changeFreq
 *     RETURN VALUE: 0 or -1
 */
int32_t rtc_open(const uint8_t* fileName) {
    terminals[currTerminalScheduler].rtcInterruptFlag = FREQ_512 / FREQ_2;  //set the frquency back to 2
    // return the success or failure based on frequency change
    return 0;
}

/*
 * rtc_close
 *     DESCRIPTION: Returns if frequency changed correctly
 *     INPUTS: fd
 *     OUTPUTS: rtc_changeFreq
 *     RETURN VALUE: 0 or -1
 */
int32_t rtc_close(int32_t fd) {
    terminals[currTerminalScheduler].rtcInterruptFlag = FREQ_512 / FREQ_2;  //set the frquency back to 2,
  //  return the success or failure based on frequency change
    return 0;
}

/*
 * rtc_changeFreq
 *     DESCRIPTION: Changes the frequency of the rtc
 *     INPUTS: inputFreq
 *     OUTPUTS: 0 or -1
 *     RETURN VALUE: Returns 0 for success
 */
int32_t rtc_changeFreq(int32_t inputFreq) {
  char prev; // 8 Bit integer
  outb(REG_A, IO_PORT1); // Idx --> Reg B / NMI_HANDLER
  prev = inb(IO_PORT2); // Obtain Reg A val

  uint8_t rate = rtc_translateFrequency(inputFreq); // Translate the input frequency to 8 bits
  if(rate == HEX_FREQ_DEF) { //check for failure from the frequency translation
    return FAILURE; // returns -1 to indicate failure
  }

  outb(REG_A, IO_PORT1); // Idx --> Reg B / NMI_HANDLER
  outb((prev & MASK_HIGH_4) | rate, IO_PORT2); //Clear the low four bits
  return SUCCESS; //return 0 to indicate success
}

/*
 * rtc_translateFrequency
 *     DESCRIPTION: Translates the input frequency to 8 bits
 *     INPUTS: inputFreq
 *     OUTPUTS: none
 *     RETURN VALUE: translated frequency
 */
uint8_t rtc_translateFrequency(int32_t inputFreq) {
  switch(inputFreq) {
      case FREQ_2  : // 2 HZ
        return HEX_FREQ_2;
      case FREQ_4  : // 4 HZ
        return HEX_FREQ_4;
      case FREQ_8  : // 8 HZ
        return HEX_FREQ_8;
      case FREQ_16  : // 16 HZ
        return HEX_FREQ_16;
      case FREQ_32  : // 32 HZ
        return HEX_FREQ_32;
      case FREQ_64  : // 64 HZ
        return HEX_FREQ_64;
      case FREQ_128  : // 128 HZ
        return HEX_FREQ_128;
      case FREQ_256  : // 256 HZ
        return HEX_FREQ_256;
      case FREQ_512  : // 512 HZ
        return HEX_FREQ_512;
      case FREQ_1024  : // 1024 HZ
        return HEX_FREQ_1024;
      default :
        return HEX_FREQ_2;
  }
}

/*
 * RTC_HANDLER
 *     DESCRIPTION: Handles the rtc interrupts
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 */
void RTC_HANDLER() {
  send_eoi(IRQ_LINE_RTC); // Send EOI to RTC IRQ
  cli(); // mask interrupts
  outb(REG_C, IO_PORT1); // handle register c contents to ensure that we'll get new interrupts
  inb(IO_PORT2);
  terminals[0].rtcInterruptFlag--; // Set interrupt flag to high
  terminals[1].rtcInterruptFlag--; // Set interrupt flag to high
  terminals[2].rtcInterruptFlag--; // Set interrupt flag to high

  sti(); //unmask interrupts
}
