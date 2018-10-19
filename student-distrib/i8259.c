/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

/* macro used to write a byte to a port */
#define OUTB(val, port)                                 \
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


/* Initialize the 8259 PIC */
void i8259_init(void) {

  master_mask = MASK_LOW8;
  slave_mask = MASK_LOW8;

	 OUTB(slave_mask, SLAVE_8259_PORT_DATA);
	 OUTB(master_mask, MASTER_8259_PORT_DATA);
  //
	// /* init master interrupt controller */
	 OUTB(ICW1, MASTER_8259_PORT); /* Start init sequence */
	 OUTB(ICW2_MASTER, MASTER_8259_PORT_DATA); /* Vector base */
	 OUTB(ICW3_MASTER, MASTER_8259_PORT_DATA); /* edge triggered, Cascade (slave) on IRQ2 */
	 OUTB(ICW4, MASTER_8259_PORT_DATA); /* Select 8086 mode */
  //
	// /* init slave interrupt controller */
	 OUTB(ICW1, SLAVE_8259_PORT); /* Start init sequence */
	 OUTB(ICW2_SLAVE, SLAVE_8259_PORT_DATA); /* Vector base */
	 OUTB(ICW3_SLAVE, SLAVE_8259_PORT_DATA); /* edge triggered, Cascade (slave) on IRQ2 */
	 OUTB(ICW4, SLAVE_8259_PORT_DATA); /* Select 8086 mode */

   OUTB(slave_mask, SLAVE_8259_PORT_DATA);
   OUTB(master_mask, MASTER_8259_PORT_DATA);
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
  uint8_t x;
  if(irq_num >= 8) {
    x = INB(SLAVE_8259_PORT_DATA);
    x = x & ~(0x1 << (irq_num - 8));
    OUTB(x,MASTER_8259_PORT_DATA);
  }
  else {
    x = INB(MASTER_8259_PORT_DATA);
    x = x & ~(0x1 << (irq_num));
    OUTB(x, MASTER_8259_PORT_DATA);
  }
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
  uint8_t x;
  if(irq_num >= 8) {
    x = INB(SLAVE_8259_PORT_DATA);
    x = x | (0x1 << (irq_num - 8));
    OUTB(x,MASTER_8259_PORT_DATA);
  }
  else {
    x = INB(MASTER_8259_PORT_DATA);
    x = x | (0x1 << (irq_num));
    OUTB(x, MASTER_8259_PORT_DATA);
  }
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
  if(irq_num >= 8) {
    uint32_t slaveIrqNum = irq_num - 8;
    OUTB(EOI | slaveIrqNum, SLAVE_8259_PORT);
  }
  OUTB(EOI | irq_num, MASTER_8259_PORT);
}
