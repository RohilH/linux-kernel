#include "rtc.h"
#include "i8259.h"
#include "lib.h"

#define status_register_a 0x8A
#define status_register_b 0x8B

#define OUTB(port, val)                                 \
do {                                                    \
    asm volatile("                                    \n\
        outb %b1, (%w0)                               \n\
        "                                               \
        : /* no outputs */                              \
        : "d"((port)), "a"((val))                       \
        : "memory", "cc"                                \
    );                                                  \
} while (0)

//TAKEN FROM OSDEV
static inline uint8_t INB(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

void RTC_INIT() {
  // SRC: https://wiki.osdev.org/RTC
  cli(); // Disable interrupts
  OUTB(0x70, 0x8A);	// select Status Register A, and disable NMI (by setting the 0x80 bit)
  OUTB(0x71, 0x20);	// write to CMOS/RTC RAM

  OUTB(0x70, 0x8B); // select register B, and disable NMI
  uint8_t prev = INB(0x71);	// read the current value of register B
  OUTB(0x70, 0x8B); // set the index again (a read will reset the index to register D)
  OUTB(0x71, prev | 0x40); // write the previous value ORed with 0x40. This turns on bit 6 of register B

  uint8_t rate = 0x0F; // rate must be above 2 and not over 15
  OUTB(0x70, 0x8A); // set index to register A, disable NMI
  prev = INB(0x71); // get initial value of register A
  OUTB(0x70, 0x8A); // reset index to A
  OUTB(0x71, (prev & 0xF0) | rate); //write only our rate to A. Note, rate is the bottom 4 bits.

  enable_irq(8); // Enable RTC IRQ
  sti(); // Enable interrupts
}

void RTC_HANDLER() {
  printf("RTC INTERRUPT");
  cli();
  disable_irq(8);
  send_eoi(8); // Send EOI to RTC IRQ
  OUTB(0x70, 0x0C);	// select register C
  INB(0x71);		// just throw away contents
  enable_irq(8);
  sti();
}
