//////
// SPACE FOR IDT. Here we will propagate the idt with the appropriate vectors
//////

#include "x86_desc.h"
#include "idt.h"
#include "lib.h"

extern void popAll();
extern void pushAll();

void IDT_Initializer() {

  void (*interruptHandler[23])(void) = {DIVISION_ERROR_HANDLER, SINGLE_STEP_INTERRUPT_HANDLER, NMI_HANDLER, BREAK_POINT_HANDLER,
                                      OVERFLOW_HANDLER, BOUNDS_HANDLER, INVALID_OPCODE_HANDLER, COPROCESSOR_NOT_AVAILABLE_HANDLER,
                                      DOUBLE_FAULT_HANDLER, COPROCESSOR_SEGMENT_OVERRUN_HANDLER, INVALID_TSS_HANDLER, SEGMENT_NOT_PRESENT_HANDLER,
                                      STACK_FAULT_HANDLER, GENERAL_PROTECTION_FAULT_HANDLER, PAGE_FAULT_HANDLER, RESERVED_HANDLER,
                                      MATH_FAULT_HANDLER, ALIGNMENT_CHECK_HANDLER, MACHINE_CHECK_HANDLER, SIMD_FLOATING_POINT_EXCEPTION_HANDLER,
                                      VIRTUALIZATION_EXCEPTION_HANDLER, CONTROL_PROTECTION_EXCEPTION_HANDLER, GENERIC_EXCEPTION_HANDLER};
  int x;
  for (x = 0; x < 32; x++) {
    idt[x].present      = 1;
    idt[x].dpl          = 0;
    idt[x].reserved0    = 0;
    idt[x].size         = 1;
    idt[x].reserved1    = 1;
    idt[x].reserved2    = 1;
    idt[x].reserved3    = 1;
    idt[x].reserved4    = 0;
    idt[x].seg_selector = KERNEL_CS;
    if (x > 21) {
      idt[x].present    = 0;
      SET_IDT_ENTRY(idt[x], interruptHandler[23]);
    }
    else
      SET_IDT_ENTRY(idt[x], interruptHandler[x]);
  }
}


void DIVISION_ERROR_HANDLER() {
  pushAll();
  printf("DIVISION_ERROR Occured");
  popAll();
}

void SINGLE_STEP_INTERRUPT_HANDLER() {
  pushAll();
  printf("SINGLE_STEP_INTERRUPT Occured");
  popAll();
}

void NMI_HANDLER() {
  pushAll();
  printf("NMI Occured");
  popAll();
}

void BREAK_POINT_HANDLER() {
  pushAll();
  printf("BREAK_POINT Occured");
  popAll();
}

void OVERFLOW_HANDLER() {
  pushAll();
  printf("OVERFLOW Occured");
  popAll();
}

void BOUNDS_HANDLER() {
  pushAll();
  printf("BOUNDS Occured");
  popAll();
}

void INVALID_OPCODE_HANDLER() {
  pushAll();
  printf("INVALID_OPCODE Occured");
  popAll();
}

void COPROCESSOR_NOT_AVAILABLE_HANDLER() {
  pushAll();
  printf("COPROCESSOR_NOT_AVAILABLE Occured");
  popAll();
}

void DOUBLE_FAULT_HANDLER() {
  pushAll();
  printf("DOUBLE_FAULT Occured");
  popAll();
}

void COPROCESSOR_SEGMENT_OVERRUN_HANDLER() {
  pushAll();
  printf("COPROCESSOR_SEGMENT_OVERRUN Occured");
  popAll();
}

void INVALID_TSS_HANDLER() {
  pushAll();
  printf("INVALID_TSS Occured");
  popAll();
}

void SEGMENT_NOT_PRESENT_HANDLER() {
  pushAll();
  printf("SEGMENT_NOT_PRESENT Occured");
  popAll();
}

void STACK_FAULT_HANDLER() {
  pushAll();
  printf("STACK_FAULT Occured");
  popAll();
}

void GENERAL_PROTECTION_FAULT_HANDLER() {
  pushAll();
  printf("GENERAL_PROTECTION Occured");
  popAll();
}

void PAGE_FAULT_HANDLER() {
  pushAll();
  printf("PAGE_FAULT Occured");
  popAll();
}

void RESERVED_HANDLER() {
  pushAll();
  printf("RESERVED Occured");
  popAll();
}

void MATH_FAULT_HANDLER() {
  pushAll();
  printf("MATH_FAULT Occured");
  popAll();
}

void ALIGNMENT_CHECK_HANDLER() {
  pushAll();
  printf("ALIGNMENT_CHECK Occured");
  popAll();
}

void MACHINE_CHECK_HANDLER() {
  pushAll();
  printf("MACHINE_CHECK Occured");
  popAll();
}

void SIMD_FLOATING_POINT_EXCEPTION_HANDLER() {
  pushAll();
  printf("SIMD_FLOATING_POINT_EXCEPTION Occured");
  popAll();
}

void VIRTUALIZATION_EXCEPTION_HANDLER() {
  pushAll();
  printf("VIRTUALIZATION_EXCEPTION Occured");
  popAll();  popAll();
}

void CONTROL_PROTECTION_EXCEPTION_HANDLER() {
  pushAll();
  printf("CONTROL_PROTECTION_EXCEPTION Occured");
  popAll();
}

void GENERIC_EXCEPTION_HANDLER() {
  pushAll();
  printf("GENERIC_EXCEPTION_HANDLER Occured");
  popAll();
}
