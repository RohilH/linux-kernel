#ifndef _PAGING_H
#define _PAGING_H

#include "types.h"
#include "x86_desc.h"

#define PresentEnable       0x01
#define ReadWriteEnable     0x02
#define UserPrivilege       0x04
#define PageSize4MBEnable   0x80
#define KernelOffset        0x82
#define pageSize            1024
#define kernelStartAddr     0x00400000
#define videoMemAddr        0x000B8000
#define videoMemIndex       0xB8
#define PageSize4MB         0x00400000

uint32_t pageDirectory[1024] __attribute__((aligned(4096)));
uint32_t pageTable[1024] __attribute__((aligned(4096)));

void PAGING_INIT();
void getNewPage(uint32_t virtualAddress, uint32_t physicalAddress);
#endif
