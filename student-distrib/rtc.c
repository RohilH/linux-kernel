#include "rtc.h"
#include "i8259.h"
#include "lib.h"

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
}

/*
 * rtc_read
 *     DESCRIPTION: Spins until an rtc interrupt is read
 *     INPUTS: fd, buf, nBytes
 *     OUTPUTS: 0
 *     RETURN VALUE: Returns 0 for success
 */
int32_t rtc_read(int32_t fd, void* buf, int32_t nBytes) {
  disable_irq(1);
  interruptFlag = 0;
  while(interruptFlag == 0) {
    // Spin until interrupt is raised
  }
  enable_irq(1);
  return 0;
}

/*
 * rtc_write
 *     DESCRIPTION: Changes the frequency and returns nBytes
 *     INPUTS: fd, buf, nBytes
 *     OUTPUTS: nBytes
 *     RETURN VALUE: 4 or -1
 */
int32_t rtc_write(int32_t fd, const void* buf, int32_t nBytes) {
  // check if nBytes is 4
  if(nBytes != 4) {
    return -1;
  }
  // changes the frequency
  if (rtc_changeFreq(*(int32_t*)buf) != 0) {
    return -1;
  }
  return nBytes;
}

/*
 * rtc_open
 *     DESCRIPTION: Returns if frequency changed correctly
 *     INPUTS: fileName
 *     OUTPUTS: rtc_changeFreq
 *     RETURN VALUE: 0 or -1
 */
int32_t rtc_open(const uint8_t* fileName) {
  return rtc_changeFreq(2);
}

/*
 * rtc_close
 *     DESCRIPTION: Returns if frequency changed correctly
 *     INPUTS: fd
 *     OUTPUTS: rtc_changeFreq
 *     RETURN VALUE: 0 or -1
 */
int32_t rtc_close(int32_t fd) {
  return rtc_changeFreq(2);
}

/*
 * rtc_changeFreq
 *     DESCRIPTION: Changes the frequency of the rtc
 *     INPUTS: inputFreq
 *     OUTPUTS: 0 or -1
 *     RETURN VALUE: Returns 0 for success
 */
int32_t rtc_changeFreq(int32_t inputFreq) {
  char prev;
  outb(REG_A, IO_PORT1);                            // Idx --> Reg B / NMI_HANDLER
  prev = inb(IO_PORT2);                             // Obtain Reg A val

  uint8_t rate = rtc_translateFrequency(inputFreq); // Translate the input frequency to 8 bits
  if(rate == 0x00) {
    return -1;
  }

  outb(REG_A, IO_PORT1);
  outb((prev & 0xF0) | rate, IO_PORT2);
  return 0;
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
      case 2  :
        return 0x0F;
      case 4  :
        return 0x0E;
      case 8  :
        return 0x0D;
      case 16  :
        return 0x0C;
      case 32  :
        return 0x0B;
      case 64  :
        return 0x0A;
      case 128  :
        return 0x09;
      case 256  :
        return 0x08;
      case 512  :
        return 0x07;
      case 1024  :
        return 0x06;
      default :
        return 0x00;
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
  cli();
  outb(REG_C, IO_PORT1);
  inb(IO_PORT2);
  interruptFlag = 1;
  sti();
}
