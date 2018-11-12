#ifndef _INTERRUPTHANDLER_H
#define _INTERRUPTHANDLER_H

// Assembly linkage for handlers
void keyboard_asmlinkage();
void rtc_asmlinkage();
void syscall_handler();

#endif
