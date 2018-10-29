#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include "types.h"
#include "multiboot.h"

int32_t halt(uint8_t status);

int32_t execute(const uint8_t * command);

int32_t read(int32_t fd, void* buf, int32_t nBytes);

int32_t write(int32_t fd, const void* buf, int32_t nBytes);

int32_t open(const uint8_t* fileName);

int32_t close(int32_t fd);

int32_t getArgs(uint8_t * buf, int32_t nBytes);

int32_t vidMap(uint8_t ** screenStart);

int32_t setHandler(int32_t sigNum, void* handlerAddress);

int32_t sigReturn(void);

#endif
