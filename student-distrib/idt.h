#ifndef _IDT_H
#define _IDT_H

#include "x86_desc.h"

// Interrupt Handlers
void DIVISION_ERROR_HANDLER();
void RESERVED_HANDLER();
void NMI_HANDLER();
void BREAK_POINT_HANDLER();
void OVERFLOW_HANDLER();
void BOUNDS_HANDLER();
void INVALID_OPCODE_HANDLER();
void DEVICE_NOT_AVAILABLE_HANDLER();
void DOUBLE_FAULT_HANDLER();
void COPROCESSOR_SEGMENT_OVERRUN_HANDLER();
void INVALID_TSS_HANDLER();
void SEGMENT_NOT_PRESENT_HANDLER();
void STACK_SEGMENT_FAULT_HANDLER();
void GENERAL_PROTECTION_HANDLER();
void PAGE_FAULT_HANDLER();

void MATH_FPU_FAULT_HANDLER();
void ALIGNMENT_CHECK_HANDLER();
void MACHINE_CHECK_HANDLER();
void SIMD_FLOATING_POINT_EXCEPTION_HANDLER();

// Initialize IDT table
void IDT_Initializer();

#define NUM_EXCEPTIONS      32 // 32 Intel defined exceptions
#define PIT_ADDR            32 // 0x20 (IRQ0)
#define KEY_ADDR            33 // 0x21 (IRQ1)
#define RTC_ADDR            40 // 0x28 (IRQ8)
#define SYSCALL_ADDR       128 // 0x80

#endif /* _IDT_H */
