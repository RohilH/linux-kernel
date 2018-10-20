#include "rtc.h"
#include "i8259.h"
#include "lib.h"

#define status_register_a 0x8A
#define status_register_b 0x8B

void RTC_INIT() {
  // SRC: https://wiki.osdev.org/RTC

  // cli();
  outb(0x8B, 0x70); // Status Register B / Disable NMI
  char prev = inb(0x71);	// Register B value
  outb(0x8B, 0x70); // Reset idx
  outb(prev | 0x40, 0x71); // ORed with 0x40: Register B
  // sti();

  // uint8_t rate = 0x0F; // rate must be above 2 and not over 15
  // outb(0x70, 0x8A); // set index to register A, disable NMI
  // prev = inb(0x71); // get initial value of register A
  // outb(0x71, (prev & 0xF0) | rate); //write only our rate to A. Note, rate is the bottom 4 bits.
  // outb(0x70, 0x8A); // reset index to A

  enable_irq(8); // Enable RTC IRQ
  // sti(); // Enable interrupts
}

void RTC_HANDLER() {
  send_eoi(8); // Send EOI to RTC IRQ
  cli();
  outb(0x0C, 0x70);	// select register C
  inb(0x71);		// just throw away contents

  // test_interrupts();
  sti();
}
