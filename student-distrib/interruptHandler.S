#define ASM     1
#include "x86_desc.h"

.globl keyboard_asmlinkage
.globl rtc_asmlinkage
.globl syscall_handler

# .data
# SAVE_EAX:



rtc_asmlinkage:
    pushal
    pushfl
    call RTC_HANDLER
    popfl
    popal
    iret

keyboard_asmlinkage:
#    pushal
#    pushfl
    call KEYBOARD_HANDLER
#    popfl
#    popal
    iret

syscall_jumptable:
    .long 0x000B8000, halt, execute, read, write, open, close, getArgs, vidMap, setHandler, sigReturn

syscall_handler:
    cli
    # push all registers (except eax) and flags
    pushl %es
    pushl %ds
    pushl %ebp
    pushl %esi
    pushl %edi

    # push arguments 1, 2, 3 onto stack
    pushl %edx # 3
    pushl %ecx # 2
    pushl %ebx # 1
    pushfl

    # protect all of the registers from modification by the system call
    pushl %ebp
    pushl %esi
    pushl %edi

    # push arguments 1, 2, 3 onto stack
    pushl %edx # 3
    pushl %ecx # 2
    pushl %ebx # 1

    # check if the system call number requested is valid
    cmpl $1, %eax
    jl invalid_syscall
    cmpl $8, %eax
    jg invalid_syscall

    # jump to the system call number designated by eax
    sti
    call *syscall_jumptable(, %eax, 4)
    # movl %eax, SAVE_EAX
    cli
    jmp done

invalid_syscall:
    movl $-1, %eax

done:
    addl $24, %esp
    # pop all registers off stack
    # pop all registers and flags
    popfl
    popl %ebx
    popl %ecx
    popl %edx
    popl %edi
    popl %esi
    popl %ebp
    popl %ds
    popl %es
    # movl $SAVE_EAX, %eax

    # popal
    sti
    iret
