#include "rtc.h"
#include "i8259.h"
#include "lib.h"

volatile int interruptFlag;

void RTC_INIT() {
  // SRC: https://wiki.osdev.org/RTC

  //cli();
  outb(REG_B, IO_PORT1); // Idx --> Reg B / NMI_HANDLER
  char prev = inb(IO_PORT2); // Obtain Reg A val
  outb(REG_B, IO_PORT1); // Reset idx
  outb(prev | BIT_SIX_MASK, IO_PORT2); // Reg B bit 6 mask
  enable_irq(IRQ_LINE_RTC); // Enable RTC IRQ

  uint8_t rate = 0x0F;
  outb(REG_A, IO_PORT1); // Idx --> Reg B / NMI_HANDLER
  prev = inb(IO_PORT2); // Obtain Reg A val
  outb(REG_A, IO_PORT1); // Reset idx
  outb((prev & 0xF0) | rate, IO_PORT2);

//  sti(); // Enable interrupts
}

int RTC_OPEN() {
  int x = RTC_CHANGE_FREQ(2);
  if ( x != 0 ) {
    return -1;
  }
  return 0;
}

int RTC_READ() {
  disable_irq(1);
  interruptFlag = 0;
  while(interruptFlag == 0) {
    // Spin until interrupt is raised
  }
  enable_irq(1);
  return 0;
}

int RTC_WRITE(int32_t fd, const void* buf, int32_t nbytes) {
  if(nbytes != 4) {
    return -1;
  }
  int x = RTC_CHANGE_FREQ(*(int32_t*)buf);
  if ( x != 0 ) {
    return -1;
  }
  return nbytes;
}

int RTC_CLOSE() {
  int x = RTC_CHANGE_FREQ(2);
  if ( x != 0 ) {
    return -1;
  }
  return 0;
}

int RTC_CHANGE_FREQ(int32_t inputFreq) {
  char prev;
  outb(REG_A, IO_PORT1); // Idx --> Reg B / NMI_HANDLER
  prev = inb(IO_PORT2); // Obtain Reg A val

  uint8_t rate = translateFrequency(inputFreq); // Translate the input frequency to 8 bits
  if(rate == 0x00) {
    return -1;
  }

  outb(REG_A, IO_PORT1);
  outb((prev & 0xF0) | rate, IO_PORT2);
  return 0;
}

uint8_t translateFrequency(int32_t inputFreq) {
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


void RTC_HANDLER() {
  send_eoi(IRQ_LINE_RTC); // Send EOI to RTC IRQ
  cli();
  outb(REG_C, IO_PORT1);
  inb(IO_PORT2);
  RTC_CHANGE_FREQ(32);
  test_interrupts();
  //interruptFlag = 1;
  sti();
  return;
}
