#include "rtc.h"
#include "i8259.h"
#include "lib.h"

void RTC_INIT() {
  // SRC: https://wiki.osdev.org/RTC

  cli();
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

  sti(); // Enable interrupts
}

void RTC_HANDLER() {
  disable_irq(IRQ_LINE_RTC);
  send_eoi(IRQ_LINE_RTC); // Send EOI to RTC IRQ
  outb(REG_C, IO_PORT1);
  inb(IO_PORT2);
  enable_irq(IRQ_LINE_RTC); // Enable RTC IRQ
}
