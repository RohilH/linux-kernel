#include "x86_desc.h"
#include "idt.h"
#include "lib.h"
#include "keyboard.h"
#include "rtc.h"

extern int DIVISION_ERROR_WRAPPER_HANDLER;

// Populate and initialize the IDT table entry attributes
void IDT_Initializer() {

  // void (*interruptHandler[23])(void) = {DIVISION_ERROR_HANDLER, RESERVED_HANDLER, NMI_HANDLER, BREAK_POINT_HANDLER,
  //                                       OVERFLOW_HANDLER, BOUNDS_HANDLER, INVALID_OPCODE_HANDLER, DEVICE_NOT_AVAILABLE_HANDLER,
  //                                       DOUBLE_FAULT_HANDLER, COPROCESSOR_SEGMENT_OVERRUN_HANDLER, INVALID_TSS_HANDLER, SEGMENT_NOT_PRESENT_HANDLER,
  //                                       STACK_SEGMENT_FAULT_HANDLER, GENERAL_PROTECTION_HANDLER, PAGE_FAULT_HANDLER, MATH_FPU_FAULT_HANDLER,
  //                                       ALIGNMENT_CHECK_HANDLER, MACHINE_CHECK_HANDLER, SIMD_FLOATING_POINT_EXCEPTION_HANDLER, GENERIC_EXCEPTION_HANDLER };

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
  SET_IDT_ENTRY(idt[0], &DIVISION_ERROR_WRAPPER_HANDLER);
  // SET_IDT_ENTRY(idt[1], RESERVED_HANDLER);
  // SET_IDT_ENTRY(idt[2], NMI_HANDLER);
  // SET_IDT_ENTRY(idt[3], BREAK_POINT_HANDLER);
  // SET_IDT_ENTRY(idt[4], OVERFLOW_HANDLER);
  // SET_IDT_ENTRY(idt[5], BOUNDS_HANDLER);
  // SET_IDT_ENTRY(idt[6], INVALID_OPCODE_HANDLER);
  // SET_IDT_ENTRY(idt[7], DEVICE_NOT_AVAILABLE_HANDLER);
  // SET_IDT_ENTRY(idt[8], DOUBLE_FAULT_HANDLER);
  // SET_IDT_ENTRY(idt[9], COPROCESSOR_SEGMENT_OVERRUN_HANDLER);
  // SET_IDT_ENTRY(idt[10], INVALID_TSS_HANDLER);
  // SET_IDT_ENTRY(idt[11], SEGMENT_NOT_PRESENT_HANDLER);
  // SET_IDT_ENTRY(idt[12], STACK_SEGMENT_FAULT_HANDLER);
  // SET_IDT_ENTRY(idt[13], GENERAL_PROTECTION_HANDLER);
  // SET_IDT_ENTRY(idt[14], PAGE_FAULT_HANDLER);
  // idt[15].present = 0; // Unused Interrupt
  // SET_IDT_ENTRY(idt[16], MATH_FPU_FAULT_HANDLER);
  // SET_IDT_ENTRY(idt[17], ALIGNMENT_CHECK_HANDLER);
  // SET_IDT_ENTRY(idt[18], MACHINE_CHECK_HANDLER);
  // SET_IDT_ENTRY(idt[19], SIMD_FLOATING_POINT_EXCEPTION_HANDLER);

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

  SET_IDT_ENTRY(idt[KEY_ADDR], KEYBOARD_HANDLER); // Set keyboard handler
  SET_IDT_ENTRY(idt[RTC_ADDR], RTC_HANDLER); // Set RTC handler

}
