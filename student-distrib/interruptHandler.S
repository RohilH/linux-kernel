#define ASM     1
#include "x86_desc.h"

.globl syscall_handler

syscall_jumptable:
    .long 0x0, halt, execute, read, write, open, close, getArgs, vidMap, setHandler, sigReturn

syscall_handler:
    cli
    pusha

    cmpl $1, %eax
    jl invalid_syscall
    cmpl $8, %eax
    jg invalid_syscall

    sti
    call *syscall_jumptable(, %eax, 4)
    cli
    jmp done

invalid_syscall:
    movl $-1, %eax

done:
    popa
    sti
    iret
