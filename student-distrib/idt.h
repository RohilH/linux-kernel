//////
// SPACE FOR IDT. Here we will propagate the idt with the appropriate vectors
//////

#ifndef _IDT_H
#define _IDT_H

#include "x86_desc.h"
#include "idtAssembly.h"

#define DIVISION_ERROR 0
#define SINGLE_STEP_INTERRUPT 1
#define NMI 2
#define BREAK_POINT 3
#define OVERFLOW 4
#define BOUNDS 5
#define INVALID_OPCODE 6
#define COPROCESSOR_NOT_AVAILABLE 7
#define DOUBLE_FAULT 8
#define COPROCESSOR_SEGMENT_OVERRUN 9
#define INVALID_TSS 10
#define SEGMENT_NOT_PRESENT 11
#define STACK_FAULT 12
#define GENERAL_PROTECTION_FAULT 13
#define PAGE_FAULT 14
#define RESERVED 15
#define MATH_FAULT 16
#define ALIGNMENT_CHECK 17
#define MACHINE_CHECK 18
#define SIMD_FLOATING_POINT_EXCEPTION 19
#define VIRTUALIZATION_EXCEPTION 20
#define CONTROL_PROTECTION_EXCEPTION 21

void propagateIDTEntry(x86_desc_t idt_desc_ptr);
extern void printException(int interruptNumber);

#endif /* _IDT_H */
