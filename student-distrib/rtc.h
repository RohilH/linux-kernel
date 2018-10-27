#ifndef _RTC_H
#define _RTC_H
#include "types.h"

#define IO_PORT1      0x70 // Index specifier / Disable NMI
#define IO_PORT2      0x71 // R/W Port from/to CMO
#define REG_A         0x8A
#define REG_B         0x8B
#define REG_C         0x0C
#define BIT_SIX_MASK  0x40
#define IRQ_LINE_RTC  8

extern void RTC_INIT();
extern void RTC_HANDLER();

int32_t rtc_read(int32_t fd, void* buf, int32_t nBytes);
int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t rtc_open(const uint8_t* fileName);
int32_t rtc_close(int32_t fd);
int32_t rtc_changeFreq(int32_t inputFreq);
uint8_t rtc_translateFrequency(int32_t inputFreq);

#endif
