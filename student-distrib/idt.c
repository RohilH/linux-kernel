//////
// SPACE FOR IDT. Here we will propagate the idt with the appropriate vectors
//////

#include "x86_desc.h"
#include "idt.h"

void propagateIDTEntry(x86_desc_t idt_desc_ptr) {
  int x;
  lidt(idt_desc_ptr);
  uint32_t address = idt_desc_ptr.addr;
  for (x = 0; x<0x1F; x++) {
    idt[x].dpl          = 0;
    idt[x].present      = 1;
    idt[x].seg_selector = KERNEL_CS;
    idt[x].size         = 1;


  }
    //uint16_t highSixteenAddress = (address >> 16) && 0x0000FFFF;
    //uint16_t lowSixteenAddress = address && 0x0000FFFF;
    //idt[x].offset_15_00 = lowSixteenAddress;

    idt[x].reserved4    = 0;
    idt[x].reserved3    = 0;
    idt[x].reserved2    = 0;
    idt[x].reserved1    = 0;
    idt[x].reserved0    = 0;
    //idt[x].offset_31_16 = highSixteenAddress;
}


SET_IDT_ENTRY(idt[0], divideByZero); //for the first entry set it's string to divideByZero

// asm
divideByZero: //divideByZero label, this will have the address that's put into the offset
Push registers //caller save
call divideByZeroHandler //call the
pop registers //pop from caller save

void divideByZeroHandler() {
  cli(); //should we use cli()?
  printf("Exception Tried to Divide By Zero");
  sti();
}
