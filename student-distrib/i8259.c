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
  //
  // unsigned long flags;
  //
	// /* initialize the controller */
	// raw_spin_lock_irqsave(&i8259_lock, flags);
  //
	// /* Mask all first */
	// OUTB(0xff, 0xA1);
	// OUTB(0xff, 0x21);
  //
	// /* init master interrupt controller */
	// OUTB(0x11, 0x20); /* Start init sequence */
	// OUTB(0x00, 0x21); /* Vector base */
	// OUTB(0x04, 0x21); /* edge triggered, Cascade (slave) on IRQ2 */
	// OUTB(0x01, 0x21); /* Select 8086 mode */
  //
	// /* init slave interrupt controller */
	// OUTB(0x11, 0xA0); /* Start init sequence */
	// OUTB(0x08, 0xA1); /* Vector base */
	// OUTB(0x02, 0xA1); /* edge triggered, Cascade (slave) on IRQ2 */
	// OUTB(0x01, 0xA1); /* Select 8086 mode */
  //
	// /* That thing is slow */
	// udelay(100);
  //
	// /* always read ISR */
	// OUTB(0x0B, 0x20);
	// OUTB(0x0B, 0xA0);
  //
	// /* Unmask the internal cascade */
	// cached_21 &= ~(1 << 2);
  //
	// /* Set interrupt masks */
	// OUTB(cached_A1, 0xA1);
	// OUTB(cached_21, 0x21);
  //
	// raw_spin_unlock_irqrestore(&i8259_lock, flags);
  //
	// /* create a legacy host */
	// i8259_host = irq_domain_add_legacy_isa(node, &i8259_host_ops, NULL);
	// if (i8259_host == NULL) {
	// 	printk(KERN_ERR "i8259: failed to allocate irq host !\n");
	// 	return;
	// }
  //
	// /* reserve our resources */
	// /* mXXX should we continue doing that ? it seems to cause problems
	//  * with further requesting of PCI IO resources for that range...
	//  * need to look into it.
	//  */
	// request_resource(&ioport_resource, &pic1_iores);
	// request_resource(&ioport_resource, &pic2_iores);
	// request_resource(&ioport_resource, &pic_edgectrl_iores);
  //
	// if (intack_addr != 0)
	// 	pci_intack = ioremap(intack_addr, 1);
  //
	// printk(KERN_INFO "i8259 legacy interrupt controller initialized\n");
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
}
