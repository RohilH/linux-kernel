#include "x86_desc.h"
#include "idt.h"
#include "lib.h"
#include "sysCalls.h"
#include "keyboard.h"
#include "interruptHandler.h"
#include "rtc.h"

/*
 * IDT_Initializer
 *     DESCRIPTION: Populate and initialize the IDT table entry attributes
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 */
void IDT_Initializer() {
  // Initialize first 32 Intel defined exception attributes
  int x;
  for (x = 0; x < NUM_EXCEPTIONS; x++) {
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

  // Manually set first 20 offset values with appropriate handler
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
  idt[15].present = 0; // Unused Interrupt
  SET_IDT_ENTRY(idt[16], MATH_FPU_FAULT_HANDLER);
  SET_IDT_ENTRY(idt[17], ALIGNMENT_CHECK_HANDLER);
  SET_IDT_ENTRY(idt[18], MACHINE_CHECK_HANDLER);
  SET_IDT_ENTRY(idt[19], SIMD_FLOATING_POINT_EXCEPTION_HANDLER);

  // Initialize the remaining interrupts
  for (x = NUM_EXCEPTIONS; x < NUM_VEC; x++) {
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
  // Set PIT handler
  SET_IDT_ENTRY(idt[PIT_ADDR], pit_asmlinkage);
  // Set Keyboard handler
  SET_IDT_ENTRY(idt[KEY_ADDR], keyboard_asmlinkage);
  // Set RTC handler
  SET_IDT_ENTRY(idt[RTC_ADDR], rtc_asmlinkage);
  // Set System Call Handler
  idt[SYSCALL_ADDR].dpl = 3;
  SET_IDT_ENTRY(idt[SYSCALL_ADDR], syscall_handler);

}

// Exception Handlers
void DIVISION_ERROR_HANDLER() {
  printf("DIVISION_ERROR Occured\n");
  halt(0);
}
void RESERVED_HANDLER() {
  printf("RESERVED Occured\n");
  halt(0);
}
void NMI_HANDLER() {
  printf("NMI Occured\n");
  halt(0);
}
void BREAK_POINT_HANDLER() {
  printf("BREAK_POINT Occured\n");
  halt(0);
}
void OVERFLOW_HANDLER() {
  printf("OVERFLOW Occured\n");
  halt(0);
}
void BOUNDS_HANDLER() {
  printf("BOUNDS Occured\n");
  halt(0);
}
void INVALID_OPCODE_HANDLER() {
  printf("INVALID_OPCODE Occured\n");
  halt(0);
}
void DEVICE_NOT_AVAILABLE_HANDLER() {
  printf("DEVICE_NOT_AVAILABLE Occured\n");
  halt(0);
}
void DOUBLE_FAULT_HANDLER() {
  printf("DOUBLE_FAULT Occured\n");
  halt(0);
}
void COPROCESSOR_SEGMENT_OVERRUN_HANDLER() {
  printf("COPROCESSOR_SEGMENT_OVERRUN Occured\n");
  halt(0);
}
void INVALID_TSS_HANDLER() {
  printf("INVALID_TSS Occured\n");
  halt(0);
}
void SEGMENT_NOT_PRESENT_HANDLER() {
  printf("SEGMENT_NOT_PRESENT Occured\n");
  halt(0);
}
void STACK_SEGMENT_FAULT_HANDLER() {
  printf("STACK_SEGMENT_FAULT Occured\n");
  halt(0);
}
void GENERAL_PROTECTION_HANDLER() {
  printf("GENERAL_PROTECTION Occured\n");
  halt(0);
}
void PAGE_FAULT_HANDLER() {
  printf("PAGE_FAULT Occured\n");
  halt(0);
}
void MATH_FPU_FAULT_HANDLER() {
  printf("MATH_FAULT Occured\n");
  halt(0);
}
void ALIGNMENT_CHECK_HANDLER() {
  printf("ALIGNMENT_CHECK Occured\n");
  halt(0);
}
void MACHINE_CHECK_HANDLER() {
  printf("MACHINE_CHECK Occured\n");
  halt(0);
}
void SIMD_FLOATING_POINT_EXCEPTION_HANDLER() {
  printf("SIMD_FLOATING_POINT_EXCEPTION Occured\n");
  halt(0);
}
