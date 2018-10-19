//////
// SPACE FOR IDT. Here we will propagate the idt with the appropriate vectors
//////

#ifndef _IDT_H
#define _IDT_H

#include "x86_desc.h"

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

void IDT_Initializer();

#endif /* _IDT_H */
