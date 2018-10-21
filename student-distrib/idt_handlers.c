#include "lib.h"
#include "idt_handlers.h"

///////////////////////////
//////// Handlers /////////
///////////////////////////

extern void DIVISION_ERROR_HANDLER() {
  asm("pusha");
  printf("DIVISION_ERROR Occured\n");
  asm("popa");
  while(1);
}

extern void RESERVED_HANDLER() {
  asm("pusha");
  printf("RESERVED Occured\n");
  asm("popa");
  while(1);
}

extern void NMI_HANDLER() {
  asm("pusha");
  printf("NMI Occured\n");
  asm("popa");
  while(1);
}

extern void BREAK_POINT_HANDLER() {
  asm("pusha");
  printf("BREAK_POINT Occured\n");
  asm("popa");
  while(1);
}

extern void OVERFLOW_HANDLER() {
  asm("pusha");
  printf("OVERFLOW Occured\n");
  asm("popa");
  while(1);
}

extern void BOUNDS_HANDLER() {
  asm("pusha");
  printf("BOUNDS Occured\n");
  asm("popa");
  while(1);
}

extern void INVALID_OPCODE_HANDLER() {
  asm("pusha");
  printf("INVALID_OPCODE Occured\n");
  asm("popa");
  while(1);
}

extern void DEVICE_NOT_AVAILABLE_HANDLER() {
  asm("pusha");
  printf("DEVICE_NOT_AVAILABLE Occured\n");
  asm("popa");
  while(1);
}

extern void DOUBLE_FAULT_HANDLER() {
  asm("pusha");
  printf("DOUBLE_FAULT Occured\n");
  asm("popa");
  while(1);
}

extern void COPROCESSOR_SEGMENT_OVERRUN_HANDLER() {
  asm("pusha");
  printf("COPROCESSOR_SEGMENT_OVERRUN Occured\n");
  asm("popa");
  while(1);
}

extern void INVALID_TSS_HANDLER() {
  asm("pusha");
  printf("INVALID_TSS Occured\n");
  asm("popa");
  while(1);
}

extern void SEGMENT_NOT_PRESENT_HANDLER() {
  asm("pusha");
  printf("SEGMENT_NOT_PRESENT Occured\n");
  asm("popa");
  while(1);
}

extern void STACK_SEGMENT_FAULT_HANDLER() {
  asm("pusha");
  printf("STACK_SEGMENT_FAULT Occured\n");
  asm("popa");
  while(1);
}

extern void GENERAL_PROTECTION_HANDLER() {
  asm("pusha");
  printf("GENERAL_PROTECTION Occured\n");
  asm("popa");
  while(1);
}

extern void PAGE_FAULT_HANDLER() {
  asm("pusha");
  printf("PAGE_FAULT Occured\n");
  asm("popa");
  while(1);
}

extern void MATH_FPU_FAULT_HANDLER() {
  asm("pusha");
  printf("MATH_FAULT Occured\n");
  asm("popa");
  while(1);
}

extern void ALIGNMENT_CHECK_HANDLER() {
  asm("pusha");
  printf("ALIGNMENT_CHECK Occured\n");
  asm("popa");
  while(1);
}

extern void MACHINE_CHECK_HANDLER() {
  asm("pusha");
  printf("MACHINE_CHECK Occured\n");
  asm("popa");
  while(1);
}

extern void SIMD_FLOATING_POINT_EXCEPTION_HANDLER() {
  asm("pusha");
  printf("SIMD_FLOATING_POINT_EXCEPTION Occured\n");
  asm("popa");
  while(1);
}
