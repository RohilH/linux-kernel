// #ifndef _PAGING_H
// #define _PAGING_H

#include "types.h"
#include "x86_desc.h"

uint32_t pageDirectory[1024] __attribute__((aligned(4096)));
uint32_t pageTable[1024] __attribute__((aligned(4096)));


void PAGING_INIT();

// extern void KEYBOARD_HANDLER();

// #endif
