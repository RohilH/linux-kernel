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
  for (i = 0; i < 1024; i++) {
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
    : "eax" // clobbers
  );
}

/*
 * getNew4MBPage
 *     DESCRIPTION: Sets up 4MB user page for process
 *     INPUTS: virtualAddress, physicalAddress
 *     OUTPUTS: none
 *     RETURN VALUE: New page in page directory and TLB flushed
 */
void getNew4MBPage(uint32_t virtualAddress, uint32_t physicalAddress) {
    uint32_t pageDirIndex = virtualAddress/PageSize4MB;
    pageDirectory[pageDirIndex] = physicalAddress + PageSize4MBEnable + UserPrivilege + ReadWriteEnable + PresentEnable;

    // Flush TLB
    asm volatile (
      "movl  %%cr3, %%eax;"
      "movl  %%eax, %%cr3;"
      :
      :
      : "eax" // clobbers
    );
}

/*
 * getNew4KBPage
 *     DESCRIPTION: Sets up 4KB video mem page for process
 *     INPUTS: virtualAddress, physicalAddress
 *     OUTPUTS: none
 *     RETURN VALUE: New page in page directory and TLB flushed
 */
void getNew4KBPage(uint32_t virtualAddress, uint32_t physicalAddress) {
    uint32_t pageDirIndex = virtualAddress/PageSize4MB;
    pageDirectory[pageDirIndex] = ((uint32_t) vidmapPageTable) + UserPrivilege + ReadWriteEnable + PresentEnable;
    vidmapPageTable[0] = physicalAddress + UserPrivilege + ReadWriteEnable + PresentEnable;
    // Flush TLB
    asm volatile (
      "movl  %%cr3, %%eax;"
      "movl  %%eax, %%cr3;"
      :
      :
      : "eax" // clobbers
    );
}

/*
 * getNewTerminal4KBPage
 *     DESCRIPTION: Sets up 4KB user page for current terminal process
 *     INPUTS: virtualAddress, physicalAddress
 *     OUTPUTS: none
 *     RETURN VALUE: New page in page directory and TLB flushed
 */
void getNewTerminal4KBPage(uint32_t virtualAddress, uint32_t physicalAddress, uint8_t currTerm) {
    uint32_t pageDirIndex = virtualAddress/PageSize4MB;
    pageDirectory[pageDirIndex] = ((uint32_t) userPageTable) + UserPrivilege + ReadWriteEnable + PresentEnable;
    userPageTable[currTerm] = physicalAddress + UserPrivilege + ReadWriteEnable + PresentEnable;
    // Flush TLB
    asm volatile (
      "movl  %%cr3, %%eax;"
      "movl  %%eax, %%cr3;"
      :
      :
      : "eax" // clobbers
    );
}
