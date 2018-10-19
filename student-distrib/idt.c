//////
// SPACE FOR IDT. Here we will propagate the idt with the appropriate vectors
//////

#include "x86_desc.h"
#include "idt.h"
#include "lib.h"
//extern void //     popAll();
//extern void //   pushAll();
static void DIVISION_ERROR_HANDLER();

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


static void DIVISION_ERROR_HANDLER() {
  asm("pusha");
  printf("DIVISION_ERROR Occured\n");
  asm("popa");
}

void RESERVED_HANDLER() {
  asm("pusha");
  printf("RESERVED Occured\n");
  asm("popa");
}

void NMI_HANDLER() {
  asm("pusha");
  printf("NMI Occured\n");
  asm("popa");
}

void BREAK_POINT_HANDLER() {
  asm("pusha");
  printf("BREAK_POINT Occured\n");
  asm("popa");
}

void OVERFLOW_HANDLER() {
  asm("pusha");
  printf("OVERFLOW Occured\n");
  asm("popa");
}

void BOUNDS_HANDLER() {
  asm("pusha");
  printf("BOUNDS Occured\n");
  asm("popa");
}

void INVALID_OPCODE_HANDLER() {
  asm("pusha");
  printf("INVALID_OPCODE Occured\n");
  asm("popa");
}

void DEVICE_NOT_AVAILABLE_HANDLER() {
  asm("pusha");
  printf("DEVICE_NOT_AVAILABLE Occured\n");
  asm("popa");
}

void DOUBLE_FAULT_HANDLER() {
  asm("pusha");
  printf("DOUBLE_FAULT Occured\n");
  asm("popa");
}

void COPROCESSOR_SEGMENT_OVERRUN_HANDLER() {
  asm("pusha");
  printf("COPROCESSOR_SEGMENT_OVERRUN Occured\n");
  asm("popa");
}

void INVALID_TSS_HANDLER() {
  asm("pusha");
  printf("INVALID_TSS Occured\n");
  asm("popa");
}

void SEGMENT_NOT_PRESENT_HANDLER() {
  asm("pusha");
  printf("SEGMENT_NOT_PRESENT Occured\n");
  asm("popa");
}

void STACK_SEGMENT_FAULT_HANDLER() {
  asm("pusha");
  printf("STACK_SEGMENT_FAULT Occured\n");
  asm("popa");
}

void GENERAL_PROTECTION_HANDLER() {
  asm("pusha");
  printf("GENERAL_PROTECTION Occured\n");
  asm("popa");
}

void PAGE_FAULT_HANDLER() {
  asm("pusha");
  printf("PAGE_FAULT Occured\n");
  asm("popa");
}

void MATH_FPU_FAULT_HANDLER() {
  asm("pusha");
  printf("MATH_FAULT Occured\n");
  asm("popa");
}

void ALIGNMENT_CHECK_HANDLER() {
  asm("pusha");
  printf("ALIGNMENT_CHECK Occured\n");
  asm("popa");
}

void MACHINE_CHECK_HANDLER() {
  asm("pusha");
  printf("MACHINE_CHECK Occured\n");
  asm("popa");
}

void SIMD_FLOATING_POINT_EXCEPTION_HANDLER() {
  asm("pusha");
  printf("SIMD_FLOATING_POINT_EXCEPTION Occured\n");
  asm("popa");
}
