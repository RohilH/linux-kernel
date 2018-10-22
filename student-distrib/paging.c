#include "paging.h"
#include "types.h"
#include "lib.h"

#define PresentEnable 0x01
#define ReadWriteEnable 0x02
#define UserPrivilege 0x04
#define PageSize4MB 0x80
#define KernelOffset 0x82
#define pageSize 1024
#define kernelStartAddr 0x00400000
#define videoMemAddr 0x000B8000
#define videoMemIndex 0xB8


void PAGING_INIT() {
  int i;
  for (i = 0; i < pageSize; i++) {
    pageDirectory[i] = 0;
    pageTable[i] = 0; // shift 12 to set address
  }
  pageDirectory[0] = ((uint32_t) pageTable) + UserPrivilege + ReadWriteEnable + PresentEnable; // first 4 MB ReadWriteEnable + PresentEnable
  pageDirectory[1] = kernelStartAddr + PageSize4MB + ReadWriteEnable + PresentEnable;
  pageTable[videoMemIndex] = videoMemAddr + UserPrivilege + ReadWriteEnable + PresentEnable; //

  /* gotten from osdev */
  asm volatile (
    "movl  %0, %%eax;"
    "movl  %%eax, %%cr3;"
    "movl  %%cr4, %%eax;"
    "orl   $0x10, %%eax;"
    "movl  %%eax, %%cr4;"
    "movl  %%cr0, %%eax;"
    "orl   $0x80000001, %%eax;"
    "movl  %%eax, %%cr0;"
    :                       // no outputs
    : "r" (pageDirectory)
    : "eax"
  );
}
