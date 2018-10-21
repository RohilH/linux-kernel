#include "rtc.h"
#include "i8259.h"
#include "lib.h"

void RTC_INIT() {
  // SRC: https://wiki.osdev.org/RTC

  // cli();
  outb(REG_B, IO_PORT1); // Idx --> Reg B / NMI_HANDLER
  char prev = inb(IO_PORT2); // Obtain Reg A val
  outb(REG_B, IO_PORT1); // Reset idx
  outb(prev | BIT_SIX_MASK, IO_PORT2); // Reg B bit 6 mask
  // sti();

  // uint8_t rate = 0x0F;
  // outb(0x70, 0x8A);
  // prev = inb(0x71);
  // outb(0x71, (prev & 0xF0) | rate);
  // outb(0x70, 0x8A);

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
