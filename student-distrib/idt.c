//////
// SPACE FOR IDT. Here we will propagate the idt with the appropriate vectors
//////

#include "x86_desc.h"
#include "idt.h"

void propagateIDTEntry(x86_desc_t idt_desc_ptr) {
  lidt(idt_desc_ptr);
  int x;
  for (x = 0; x<NUM_VEC; x++) {
    idt[x].offset_15_00 = 0;
    idt[x].seg_selector = 0;
    idt[x].reserved4    = 0;
    idt[x].reserved3    = 0;
    idt[x].reserved2    = 0;
    idt[x].reserved1    = 0;
    idt[x].size         = 0;
    idt[x].reserved0    = 0;
    idt[x].dpl          = 0;
    idt[x].present      = 0;
    idt[x].offset_31_16 = 0;
  }
}
