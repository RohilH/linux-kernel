#include "paging.h"
#include "types.h"
#include "lib.h"

#define pageSize 1024
#define ReadWriteEnable 0x10
#define PresentEnable 0x01
#define kernelStartAddr 0x400000
#define videoMemAddr 0xB8000
#define videoMemIndex 0xB8


void PAGING_INIT() {
  int i;
  for (i = 0; i < pageSize; i++) {
    pageDirectory[i] = 0;
    pageTable[i] = 0; // shift 12 to set address
  }
  pageDirectory[0] = ((uint32_t) pageTable) + ReadWriteEnable + PresentEnable; // first 4 MB
  pageDirectory[1] = kernelStartAddr + ReadWriteEnable + PresentEnable;
  pageTable[videoMemIndex] = videoMemAddr + ReadWriteEnable + PresentEnable;

  asm volatile ("             \n\
    movl  %0, %%eax           \n\
    movl  %%eax, %%cr3        \n\
    orl   0x10, %%eax         \n\
    movl  %%eax, %%cr4        \n\
    movl  %%cr0, %%eax        \n\
    orl   0x80000001, %%eax   \n\
    movl  %%eax, %%cr0        \n\
    "
    :                       // no outputs
    : "r" (pageDirectory)
    : "eax"
  );
}
