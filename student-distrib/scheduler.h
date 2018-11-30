#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include "multiboot.h"
#include "rtc.h"
#include "sysCalls.h"
#include "lib.h"
#include "types.h"
#include "fileSystem.h"
#include "terminal.h"
#include "paging.h"
#include "i8259.h"

#define PIT_ADDR            32 // 0x20 (IRQ0)
#define PIT_IRQ_NUM          0
#define PIT_CHANNEL_0       64 //0x40
#define PIT_CMD_REGISTER    67 //0x43
#define PIT_MODE_SQR_WAV    54 //0x36 - Mode 3
#define PIT_MAX_FREQ        1193182
// #define _100_HZ_VAL         11932
#define LOW_BYTE_MASK       0xFF
#define RELOAD_VALUE        100 // In Hz
#define BIT_SHIFT           8

// Initialization + Handler
extern void PIT_INIT();
extern void PIT_HANDLER();

void contextSwitch();
int32_t getNextProcess();

#endif /* _PIT_H */
