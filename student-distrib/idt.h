//////
// SPACE FOR IDT. Here we will propagate the idt with the appropriate vectors
//////

#ifndef _IDT_H
#define _IDT_H

#include "x86_desc.h"

void propagateIDTEntry(x86_desc_t idt_desc_ptr);

#endif /* _IDT_H */
