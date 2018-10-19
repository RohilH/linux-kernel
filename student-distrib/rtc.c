#include "rtc.h"
#include "i8259.h"
// #include "x86_desc.h"
// #include "idt.h"
// #include "lib.h"

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
  OUTB(0x70, 0x8B);
  uint8_t prev = INB(0x71);
  OUTB(0x70, 0x8B);
  OUTB(0x71, prev | 0x40);

  OUTB(0x70, 0x8A);
  uint8_t rate = 0x0F;
  prev = INB(0x71);
  prev = (prev) | rate;
  OUTB(0x70, 0x8A);
  OUTB(0x71, prev);
  enable_irq(8);
}

void RTC_HANDLER() {
  //OUTB(0x70, 0x0C);
  //INB(0x71);
  printf("RTC INTERRUPT");
  //send_eoi(8);
  while(1);

}
