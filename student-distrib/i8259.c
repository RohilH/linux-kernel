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

  outb(slave_mask, SLAVE_8259_PORT_DATA); // Mask all of 8259A Slave
  outb(master_mask, MASTER_8259_PORT_DATA); // Mask all of 8259A Master

  // Master Interrupt Controller Setup
  outb(ICW1, MASTER_8259_PORT); // Select Master init (ICW1)
  outb(ICW2_MASTER, MASTER_8259_PORT_DATA); // Map IR0,...,IR7 to 0x20,...,0x27 (ICW2)
  outb(ICW3_MASTER, MASTER_8259_PORT_DATA); // Slave on IR2 (ICW3)
  outb(ICW4, MASTER_8259_PORT_DATA); // Normal EOI (ICW4)

  // Slave Interrupt Controller Setup
  outb(ICW1, SLAVE_8259_PORT); // Select Slave init (ICW1)
  outb(ICW2_SLAVE, SLAVE_8259_PORT_DATA); // Map IR0,...,IR7 to 0x28,...,0x2f (ICW2)
  outb(ICW3_SLAVE, SLAVE_8259_PORT_DATA); // Slave on IR2 (ICW3)
  outb(ICW4, SLAVE_8259_PORT_DATA); // EOI (ICW4)

  outb(slave_mask, SLAVE_8259_PORT_DATA); // Restore slave IRQ mask
  outb(master_mask, MASTER_8259_PORT_DATA); // Restore master IRQ mask

  enable_irq(IRQ_LINE_PIC); // Enable handling of IRQ line 2
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
  // Check if IRQ for slave
  if(irq_num >= SLAVE_MIN && irq_num <= SLAVE_MAX) { // IRQ corresponds to slave
    slave_mask = slave_mask & ~(0x1 << (irq_num - SLAVE_MIN)); // Update irq_num bit
    outb(slave_mask,SLAVE_8259_PORT_DATA); // Update slave
  } else { // IRQ corresponds to master
    master_mask = master_mask & ~(0x1 << (irq_num)); // Update irq_num bit
    outb(master_mask, MASTER_8259_PORT_DATA); // Update master
  }
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
  // Check if IRQ for slave
  if(irq_num >= SLAVE_MIN && irq_num <= SLAVE_MAX) { // IRQ corresponds to slave
    slave_mask = slave_mask | (0x1 << (irq_num - SLAVE_MIN)); // Update irq_num bit
    outb(slave_mask,SLAVE_8259_PORT_DATA); // Update slave
  } else { // IRQ corresponds to master
    master_mask = master_mask | (0x1 << (irq_num)); // Update irq_num bit
    outb(master_mask, MASTER_8259_PORT_DATA);// Update master
  }
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
  // Send EOI to appropriate port based on irq_num
  if(irq_num >= SLAVE_MIN && irq_num <= SLAVE_MAX)
    outb(EOI, SLAVE_8259_PORT);
  else
    outb(EOI, MASTER_8259_PORT);
}
