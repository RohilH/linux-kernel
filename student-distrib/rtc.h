#ifndef _RTC_H
#define _RTC_H

extern void RTC_INIT();
extern void RTC_HANDLER();

#define IO_PORT1 0x70 // Index specifier / Disable NMI
#define IO_PORT2 0x71 // R/W Port from/to CMO
#define REG_A 0x8A
#define REG_B 0x8B
#define REG_C 0x0C
#define BIT_SIX_MASK 0x40

#define IRQ_LINE_RTC 8

#endif
