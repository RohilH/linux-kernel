#include "paging.h"
#include "types.h"
#include "lib.h"

/*
 * PAGING_INIT
 *     DESCRIPTION: Initialize paging
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 */
void PAGING_INIT() {
  // shift 12 to set address
  int i;
  for (i = 0; i < pageSize; i++) {
    pageDirectory[i] = 0;
    pageTable[i] = 0;
  }

  // first 4 MB ReadWriteEnable + PresentEnable
  pageDirectory[0] = ((uint32_t) pageTable) + UserPrivilege + ReadWriteEnable + PresentEnable;
  pageDirectory[1] = kernelStartAddr + PageSize4MBEnable + ReadWriteEnable + PresentEnable;
  pageTable[videoMemIndex] = videoMemAddr + UserPrivilege + ReadWriteEnable + PresentEnable;

  // got from osdev
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

void getNewPage(uint32_t virtualAddress, uint32_t physicalAddress) {
    uint32_t pageDirIndex = virtualAddress/PageSize4MB;
    pageDirectory[pageDirIndex] = physicalAddress + PageSize4MBEnable + UserPrivilege + ReadWriteEnable + PresentEnable;
}
