/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {
  master_mask = MASK_LOW8;
  slave_mask = MASK_LOW8;

 outb(slave_mask, SLAVE_8259_PORT_DATA);
 outb(master_mask, MASTER_8259_PORT_DATA);

 /* init master interrupt controller */
 outb(ICW1, MASTER_8259_PORT); /* Start init sequence */
 outb(ICW2_MASTER, MASTER_8259_PORT_DATA); /* Vector base */
 outb(ICW3_MASTER, MASTER_8259_PORT_DATA); /* edge triggered, Cascade (slave) on IRQ2 */
 outb(ICW4, MASTER_8259_PORT_DATA); /* Select 8086 mode */

 /* init slave interrupt controller */
 outb(ICW1, SLAVE_8259_PORT); /* Start init sequence */
 outb(ICW2_SLAVE, SLAVE_8259_PORT_DATA); /* Vector base */
 outb(ICW3_SLAVE, SLAVE_8259_PORT_DATA); /* edge triggered, Cascade (slave) on IRQ2 */
 outb(ICW4, SLAVE_8259_PORT_DATA); /* Select 8086 mode */

 outb(slave_mask, SLAVE_8259_PORT_DATA);
 outb(master_mask, MASTER_8259_PORT_DATA);

 enable_irq(2);
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
  uint8_t x;
  if(irq_num >= 8 && irq_num < 16) {
    slave_mask = slave_mask & ~(0x1 << (irq_num - 8));
    outb(slave_mask,SLAVE_8259_PORT_DATA);
  } else {
    master_mask = master_mask & ~(0x1 << (irq_num));
    outb(master_mask, MASTER_8259_PORT_DATA);
  }
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
  uint8_t x;
  if(irq_num >= 8 && irq_num < 16) {
    slave_mask = slave_mask | (0x1 << (irq_num - 8));
    outb(slave_mask,SLAVE_8259_PORT_DATA);
  } else {
    master_mask = master_mask | (0x1 << (irq_num));
    outb(master_mask, MASTER_8259_PORT_DATA);
  }
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
  if(irq_num >= 8 && irq_num < 16) {
    outb(EOI, SLAVE_8259_PORT);
  } else {
    outb(EOI, MASTER_8259_PORT);
  }
}
