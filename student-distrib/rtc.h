#ifndef _RTC_H
#define _RTC_H
#include "types.h"

#define IO_PORT1 0x70 // Index specifier / Disable NMI
#define IO_PORT2 0x71 // R/W Port from/to CMO
#define REG_A 0x8A
#define REG_B 0x8B
#define REG_C 0x0C
#define BIT_SIX_MASK 0x40

#define IRQ_LINE_RTC 8


extern void RTC_INIT();
extern void RTC_HANDLER();
int RTC_OPEN();
int RTC_READ();
int RTC_WRITE(int32_t fd, const void* buf, int32_t nbytes);
int RTC_CLOSE();
int RTC_CHANGE_FREQ(int32_t inputFreq);
uint8_t translateFrequency(int32_t inputFreq);



#endif
