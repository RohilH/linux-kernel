# #define ASM 1

# .text

# #.globl DIVISION_ERROR_HANDLER_WRAPPER
# DIVISION_ERROR_HANDLER_WRAPPER:
#   pushal
#   pushfl
#   call DIVISION_ERROR_HANDLER
#   popfl
#   popal
#   iret
