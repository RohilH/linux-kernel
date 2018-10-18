/*
# idtAssembly.S - Save and restore all registers, call appropriate handler
# vim:ts=4 noexpandtab

#define ASM     1
#include "idtAssembly.h"

interruptHandler:
  movl %esp, %ebp
  pushl %ebp
  pusha # t
  movl 0(%esp),%eax
  pushl %eax
  call printException
  popl  %eax
  popa
  leave
  ret

  */
