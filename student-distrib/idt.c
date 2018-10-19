//////
// SPACE FOR IDT. Here we will propagate the idt with the appropriate vectors
//////

#include "x86_desc.h"
#include "idt.h"
#include "lib.h"

extern void popAll();
extern void pushAll();

void IDT_Initializer() {

  // void (*interruptHandler[23])(void) = {DIVISION_ERROR_HANDLER, RESERVED_HANDLER, NMI_HANDLER, BREAK_POINT_HANDLER,
  //                                       OVERFLOW_HANDLER, BOUNDS_HANDLER, INVALID_OPCODE_HANDLER, DEVICE_NOT_AVAILABLE_HANDLER,
  //                                       DOUBLE_FAULT_HANDLER, COPROCESSOR_SEGMENT_OVERRUN_HANDLER, INVALID_TSS_HANDLER, SEGMENT_NOT_PRESENT_HANDLER,
  //                                       STACK_SEGMENT_FAULT_HANDLER, GENERAL_PROTECTION_HANDLER, PAGE_FAULT_HANDLER, MATH_FPU_FAULT_HANDLER,
  //                                       ALIGNMENT_CHECK_HANDLER, MACHINE_CHECK_HANDLER, SIMD_FLOATING_POINT_EXCEPTION_HANDLER, GENERIC_EXCEPTION_HANDLER };
  int x;
  for (x = 0; x < 32; x++) {
    idt[x].present      = 1;
    idt[x].dpl          = 0;
    idt[x].size         = 1;
    idt[x].reserved0    = 0;
    idt[x].reserved1    = 1;
    idt[x].reserved2    = 1;
    idt[x].reserved3    = 1;
    idt[x].reserved4    = 0;
    idt[x].seg_selector = KERNEL_CS;
  }
  idt[15].present = 0;
  SET_IDT_ENTRY(idt[0], DIVISION_ERROR_HANDLER);
  SET_IDT_ENTRY(idt[1], RESERVED_HANDLER);
  SET_IDT_ENTRY(idt[2], NMI_HANDLER);
  SET_IDT_ENTRY(idt[3], BREAK_POINT_HANDLER);
  SET_IDT_ENTRY(idt[4], OVERFLOW_HANDLER);
  SET_IDT_ENTRY(idt[5], BOUNDS_HANDLER);
  SET_IDT_ENTRY(idt[6], INVALID_OPCODE_HANDLER);
  SET_IDT_ENTRY(idt[7], DEVICE_NOT_AVAILABLE_HANDLER);
  SET_IDT_ENTRY(idt[8], DOUBLE_FAULT_HANDLER);
  SET_IDT_ENTRY(idt[9], COPROCESSOR_SEGMENT_OVERRUN_HANDLER);
  SET_IDT_ENTRY(idt[10], INVALID_TSS_HANDLER);
  SET_IDT_ENTRY(idt[11], SEGMENT_NOT_PRESENT_HANDLER);
  SET_IDT_ENTRY(idt[12], STACK_SEGMENT_FAULT_HANDLER);
  SET_IDT_ENTRY(idt[13], GENERAL_PROTECTION_HANDLER);
  SET_IDT_ENTRY(idt[14], PAGE_FAULT_HANDLER);
  //SET_IDT_ENTRY(idt[15], DIVISION_ERROR_HANDLER);
  SET_IDT_ENTRY(idt[16], MATH_FPU_FAULT_HANDLER);
  SET_IDT_ENTRY(idt[17], ALIGNMENT_CHECK_HANDLER);
  SET_IDT_ENTRY(idt[18], MACHINE_CHECK_HANDLER);
  SET_IDT_ENTRY(idt[19], SIMD_FLOATING_POINT_EXCEPTION_HANDLER);


  for (x = 32; x < NUM_VEC; x++) {
    idt[x].present      = 1;
    idt[x].dpl          = 0;
    idt[x].size         = 1;
    idt[x].reserved0    = 0;
    idt[x].reserved1    = 1;
    idt[x].reserved2    = 1;
    idt[x].reserved3    = 0;
    idt[x].reserved4    = 0;
    idt[x].seg_selector = KERNEL_CS;
  }
}


void DIVISION_ERROR_HANDLER() {
  pushAll();
  printf("DIVISION_ERROR Occured");
  popAll();
}

void RESERVED_HANDLER() {
  pushAll();
  printf("RESERVED Occured");
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

void DEVICE_NOT_AVAILABLE_HANDLER() {
  pushAll();
  printf("DEVICE_NOT_AVAILABLE Occured");
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

void STACK_SEGMENT_FAULT_HANDLER() {
  pushAll();
  printf("STACK_SEGMENT_FAULT Occured");
  popAll();
}

void GENERAL_PROTECTION_HANDLER() {
  pushAll();
  printf("GENERAL_PROTECTION Occured");
  popAll();
}

void PAGE_FAULT_HANDLER() {
  pushAll();
  printf("PAGE_FAULT Occured");
  popAll();
}

void MATH_FPU_FAULT_HANDLER() {
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
