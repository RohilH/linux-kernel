# idtAssemblyHelpers.S

.globl pushAll
.globl popAll
.section .text

pushAll:
        pushl %ebp
        movl %esp, %ebp
        pusha
        leave
        ret

popAll:
        pushl %ebp
        movl %esp, %ebp
        popa
        leave
        ret
.end
