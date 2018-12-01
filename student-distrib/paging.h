#ifndef _PAGING_H
#define _PAGING_H

#include "types.h"
#include "x86_desc.h"

#define PresentEnable       0x01
#define ReadWriteEnable     0x02
#define UserPrivilege       0x04
#define PageSize4MBEnable   0x80
#define KernelOffset        0x82
#define kernelStartAddr     0x00400000
#define videoMemAddr        0x000B8000
#define videoMemIndex       0xB8
#define PageSize4KB         0x00001000
#define PageSize8KB         0x00002000
#define PageSize4MB         0x00400000
#define PageSize8MB         0x00800000
#define PageSize64MB        0x04000000
#define PageSize128MB        0x08000000

// Page Directory of 1024 PTEs
uint32_t pageDirectory[1024] __attribute__((aligned(4096)));
// Page Table of 1024 entries
uint32_t pageTable[1024] __attribute__((aligned(4096)));

uint32_t vidmapPageTable[1024] __attribute__((aligned(4096)));

uint32_t userPageTable[1024] __attribute__((aligned(4096)));
// Initialize Page
void PAGING_INIT();
void getNew4MBPage(uint32_t virtualAddress, uint32_t physicalAddress);
void getNew4KBPage(uint32_t virtualAddress, uint32_t physicalAddress);
void getNewTerminal4KBPage(uint32_t virtualAddress, uint32_t physicalAddress, uint8_t currTerm);
#endif
