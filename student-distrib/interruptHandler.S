#define ASM     1
#include "x86_desc.h"

.globl pit_asmlinkage
.globl keyboard_asmlinkage
.globl rtc_asmlinkage
.globl syscall_handler

#define ONE_INDEXED 0x0

# Assembly linkage for rtc handler
rtc_asmlinkage:
    pushal
    pushfl
    call RTC_HANDLER
    popfl
    popal
    iret

# Assembly linkage for keyboard handler
keyboard_asmlinkage:
    pushal
    pushfl
    call KEYBOARD_HANDLER
    popfl
    popal
    iret

# Assembly linkage for pit handler
pit_asmlinkage:
    pushal
    pushfl
    call PIT_HANDLER
    popfl
    popal
    iret

# Jump table to system calls
syscall_jumptable:
    # syscalls are 1 indexed
    .long ONE_INDEXED, halt, execute, read, write, open, close, getArgs, vidMap, setHandler, sigReturn

# Handles system calls by pushing arguments and restoring stack
syscall_handler:
    cli
    # push all registers (except eax) and flags
    pushl %es
    pushl %ds
    pushl %ebp
    pushl %esi
    pushl %edi
    pushl %edx
    pushl %ecx
    pushl %ebx
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
    cli
    jmp done

invalid_syscall:
    movl $-1, %eax

done:
    # pop all registers and flags off stack
    addl $24, %esp
    popfl
    popl %ebx
    popl %ecx
    popl %edx
    popl %edi
    popl %esi
    popl %ebp
    popl %ds
    popl %es
    sti
    iret
