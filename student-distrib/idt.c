//////
// SPACE FOR IDT. Here we will propagate the idt with the appropriate vectors
//////

#include "x86_desc.h"
#include "idt.h"
#include "idtAssembly.h"
#include "lib.h"

void propagateIDTEntry(x86_desc_t idt_desc_ptr) {
  int x;
  for (x = 0; x<0x1F; x++) {
    idt[x].dpl          = 0;
    idt[x].present      = 1;
    idt[x].seg_selector = KERNEL_CS;
    idt[x].size         = 1;

    //uint16_t highSixteenAddress = (address >> 16) && 0x0000FFFF;
    //uint16_t lowSixteenAddress = address && 0x0000FFFF;
    //idt[x].offset_15_00 = lowSixteenAddress;

    idt[x].reserved4    = 0;
    idt[x].reserved3    = 0;
    idt[x].reserved2    = 0;
    idt[x].reserved1    = 0;
    idt[x].reserved0    = 0;
    //idt[x].offset_31_16 = highSixteenAddress;
    SET_IDT_ENTRY(idt[x], interruptHandler); //for the first entry set it's string to divideByZero
    }
    lidt(idt_desc_ptr); // load this after it has been propagated
}
//all but sys call, would have a descriptor privelege level of 0




void printException(int interruptNumber) {
  switch(interruptNumber) {
    case DIVISION_ERROR:
    printf("");
    break;

    case SINGLE_STEP_INTERRUPT:
    printf("");
    break;

    case NMI:
    printf("");
    break;

    case BREAK_POINT:
    printf("");
    break;

    case OVERFLOW:
    printf("");
    break;

    case BOUNDS:
    printf("");
    break;

    case INVALID_OPCODE:
    printf("");
    break;

    case COPROCESSOR_NOT_AVAILABLE:
    printf("");
    break;

    case DOUBLE_FAULT:
    printf("");
    break;

    case COPROCESSOR_SEGMENT_OVERRUN:
    printf("");
    break;

    case INVALID_TSS:
    printf("");
    break;

    case SEGMENT_NOT_PRESENT:
    printf("");
    break;

    case STACK_FAULT:
    printf("");
    break;

    case GENERAL_PROTECTION_FAULT:
    printf("");
    break;

    case PAGE_FAULT:
    printf("");
    break;

    case RESERVED:
    printf("");
    break;

    case MATH_FAULT:
    printf("");
    break;

    case ALIGNMENT_CHECK:
    printf("");
    break;

    case MACHINE_CHECK:
    printf("");
    break;

    case SIMD_FLOATING_POINT_EXCEPTION:
    printf("");
    break;

    case VIRTUALIZATION_EXCEPTION:
    printf("");
    break;

    case CONTROL_PROTECTION_EXCEPTION:
    printf("");
    break;

    default:
    break;
  }
}
