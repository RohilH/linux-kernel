#ifndef _RTC_H
#define _RTC_H

extern void RTC_INIT();
extern void RTC_HANDLER();

#define IO_PORT1 0x70 // Index specifier / Disable NMI
#define IO_PORT2 0x71 // R/W Port from/to CMO

#endif
