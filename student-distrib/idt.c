//////
// SPACE FOR IDT. Here we will propagate the idt with the appropriate vectors
//////

#include "x86_desc.h"
#include "idt.h"
#include "lib.h"

void IDT_Initializer() {

  void (*interruptHandler[22])(void) = {DIVISION_ERROR_HANDLER, SINGLE_STEP_INTERRUPT_HANDLER, NMI_HANDLER, BREAK_POINT_HANDLER,
                                      OVERFLOW_HANDLER, BOUNDS_HANDLER, INVALID_OPCODE_HANDLER, COPROCESSOR_NOT_AVAILABLE_HANDLER,
                                      DOUBLE_FAULT_HANDLER, COPROCESSOR_SEGMENT_OVERRUN_HANDLER, INVALID_TSS_HANDLER, SEGMENT_NOT_PRESENT_HANDLER,
                                      STACK_FAULT_HANDLER, GENERAL_PROTECTION_FAULT_HANDLER, PAGE_FAULT_HANDLER, RESERVED_HANDLER,
                                      MATH_FAULT_HANDLER, ALIGNMENT_CHECK_HANDLER, MACHINE_CHECK_HANDLER, SIMD_FLOATING_POINT_EXCEPTION_HANDLER,
                                      VIRTUALIZATION_EXCEPTION_HANDLER, CONTROL_PROTECTION_EXCEPTION_HANDLER};
  int x;
  for (x = 0; x < 32; x++) {
    idt[x].present      = 1;
    idt[x].dpl          = 0;
    idt[x].reserved0    = 0;
    idt[x].size         = 1;
    idt[x].reserved1    = 1;
    idt[x].reserved2    = 1;
    idt[x].reserved3    = 0;
    idt[x].reserved4    = 0;
    idt[x].seg_selector = KERNEL_CS;
    if (x > 21) {
      idt[x].present    = 0;
    }
    else
      SET_IDT_ENTRY(idt[x], interruptHandler[x]);
  }
  // lidt(idt_desc_ptr); // load this after it has been propagated
}


void DIVISION_ERROR_HANDLER() {
  printf("DIVISION_ERROR Occured");
}

void SINGLE_STEP_INTERRUPT_HANDLER() {
  printf("SINGLE_STEP_INTERRUPT Occured");
}

void NMI_HANDLER() {
  printf("NMI Occured");
}

void BREAK_POINT_HANDLER() {
  printf("BREAK_POINT Occured");
}

void OVERFLOW_HANDLER() {
  printf("OVERFLOW Occured");
}

void BOUNDS_HANDLER() {
  printf("BOUNDS Occured");
}

void INVALID_OPCODE_HANDLER() {
  printf("INVALID_OPCODE Occured");
}

void COPROCESSOR_NOT_AVAILABLE_HANDLER() {
  printf("COPROCESSOR_NOT_AVAILABLE Occured");
}

void DOUBLE_FAULT_HANDLER() {
  printf("DOUBLE_FAULT Occured");
}

void COPROCESSOR_SEGMENT_OVERRUN_HANDLER() {
  printf("COPROCESSOR_SEGMENT_OVERRUN Occured");
}

void INVALID_TSS_HANDLER() {
  printf("INVALID_TSS Occured");
}

void SEGMENT_NOT_PRESENT_HANDLER() {
  printf("SEGMENT_NOT_PRESENT Occured");
}

void STACK_FAULT_HANDLER() {
  printf("STACK_FAULT Occured");
}

void GENERAL_PROTECTION_FAULT_HANDLER() {
  printf("GENERAL_PROTECTION Occured");
}

void PAGE_FAULT_HANDLER() {
  printf("PAGE_FAULT Occured");
}

void RESERVED_HANDLER() {
  printf("RESERVED Occured");
}

void MATH_FAULT_HANDLER() {
  printf("MATH_FAULT Occured");
}

void ALIGNMENT_CHECK_HANDLER() {
  printf("ALIGNMENT_CHECK Occured");
}

void MACHINE_CHECK_HANDLER() {
  printf("MACHINE_CHECK Occured");
}

void SIMD_FLOATING_POINT_EXCEPTION_HANDLER() {
  printf("SIMD_FLOATING_POINT_EXCEPTION Occured");
}

void VIRTUALIZATION_EXCEPTION_HANDLER() {
  printf("VIRTUALIZATION_EXCEPTION Occured");
}

void CONTROL_PROTECTION_EXCEPTION_HANDLER() {
  printf("CONTROL_PROTECTION_EXCEPTION Occured");
}
