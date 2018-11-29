#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include "sysCalls.h"
#include "types.h"
#include "multiboot.h"
#include "rtc.h"
#include "terminal.h"
#include "fileSystem.h"

#define PIT_ADDR            32 // 0x20 (IRQ0)
#define PIT_IRQ_NUM          0
#define PIT_CHANNEL_0       64 //0x40
#define PIT_CMD_REGISTER    67 //0x43
#define PIT_MODE_SQR_WAV    54 //0x36
#define LOW_BYTE_MASK     0xFF
#define _100_HZ_VAL       11932

// Initialization + Handler
extern void PIT_INIT();
extern void PIT_HANDLER();



#endif /* _SCHEDULER_H */
