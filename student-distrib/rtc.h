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

#define HEX_FREQ_2 0x0F
#define HEX_FREQ_4 0x0E
#define HEX_FREQ_8 0x0D
#define HEX_FREQ_16 0x0C
#define HEX_FREQ_32 0x0B
#define HEX_FREQ_64 0x0A
#define HEX_FREQ_128 0x09
#define HEX_FREQ_256 0x08
#define HEX_FREQ_512 0x07
#define HEX_FREQ_1024 0x06
#define HEX_FREQ_DEF 0x00

#define FREQ_2 2
#define FREQ_4 4
#define FREQ_8 8
#define FREQ_16 16
#define FREQ_32 32
#define FREQ_64 64
#define FREQ_128 128
#define FREQ_256 256
#define FREQ_512 512
#define FREQ_1024 1024

#define HIGH_IF 1
#define LOW_IF 0
#define IRQ_LINE_RTC 8

#define KEYBOARD_IRQ 1

#define SUCCESS 0
#define FAILURE -1

#define MASK_HIGH_4 0xF0
#define MASK_LOW_4 0x0F

#define BYTE_CHECK 4

extern void RTC_INIT();
extern void RTC_HANDLER();

int32_t rtc_read(int32_t fd, void* buf, int32_t nBytes);
int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t rtc_open(const uint8_t* fileName);
int32_t rtc_close(int32_t fd);
int32_t rtc_changeFreq(int32_t inputFreq);
uint8_t rtc_translateFrequency(int32_t inputFreq);

#endif
