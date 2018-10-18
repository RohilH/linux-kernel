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
    idt[x].reserved4    = 0;
    idt[x].reserved3    = 0;
    idt[x].reserved2    = 0;
    idt[x].reserved1    = 0;
    idt[x].reserved0    = 0;
    SET_IDT_ENTRY(idt[x], interruptHandler[x]);
    }
    lidt(idt_desc_ptr); // load this after it has been propagated
}

void DIVISION_ERROR_HANDLER() {

}
void SINGLE_STEP_INTERRUPT_HANDLER() {

}
void NMI_HANDLER() {

}
void BREAK_POINT_HANDLER() {

}
void OVERFLOW_HANDLER() {

}
void BOUNDS_HANDLER() {

}
void INVALID_OPCODE_HANDLER() {

}
void COPROCESSOR_NOT_AVAILABLE_HANDLER() {

}
void DOUBLE_FAULT_HANDLER() {

}
void COPROCESSOR_SEGMENT_OVERRUN_HANDLER() {

}
void INVALID_TSS_HANDLER() {

}
void SEGMENT_NOT_PRESENT_HANDLER() {

}
void STACK_FAULT_HANDLER() {

}
void GENERAL_PROTECTION_FAULT_HANDLER() {

}
void PAGE_FAULT_HANDLER() {

}
void RESERVED_HANDLER() {

}
void MATH_FAULT_HANDLER() {

}
void ALIGNMENT_CHECK_HANDLER() {

}
void MACHINE_CHECK_HANDLER() {

}
void SIMD_FLOATING_POINT_EXCEPTION_HANDLER() {

}
void VIRTUALIZATION_EXCEPTION_HANDLER() {

}
void CONTROL_PROTECTION_EXCEPTION_HANDLER() {

}
