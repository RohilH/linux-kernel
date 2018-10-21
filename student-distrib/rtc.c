#include "rtc.h"
#include "i8259.h"
#include "lib.h"

void RTC_INIT() {
  // SRC: https://wiki.osdev.org/RTC

  // cli();
  outb(REG_B, IO_PORT1); // Status Register B / Disable NMI
  char prev = inb(IO_PORT2);	// Register B value
  outb(REG_B, IO_PORT1); // Reset idx
  outb(prev | BIT_SIX_MASK, IO_PORT2); // ORed with 0x40: Register B
  // sti();

  // uint8_t rate = 0x0F; // rate must be above 2 and not over 15
  // outb(0x70, 0x8A); // set index to register A, disable NMI
  // prev = inb(0x71); // get initial value of register A
  // outb(0x71, (prev & 0xF0) | rate); //write only our rate to A. Note, rate is the bottom 4 bits.
  // outb(0x70, 0x8A); // reset index to A

  enable_irq(IRQ_LINE_RTC); // Enable RTC IRQ
  // sti(); // Enable interrupts
}

void RTC_HANDLER() {
  send_eoi(IRQ_LINE_RTC); // Send EOI to RTC IRQ
  cli();
  outb(REG_C, IO_PORT1);
  inb(IO_PORT2);

  sti();
}
