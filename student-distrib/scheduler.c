#include "scheduler.h"

/*
 * PIT_INIT
 *     DESCRIPTION: Initializes the PIT and sets it's timer to 10 ms
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 */
void PIT_INIT() {
  // Obtain high and low bits of frequency divider value
  uint8_t lowerEightFreq  = (uint8_t)((PIT_MAX_FREQ / RELOAD_VALUE) & LOW_BYTE_MASK);
  uint8_t higherEightFreq = (uint8_t)(((PIT_MAX_FREQ / RELOAD_VALUE) >> BIT_SHIFT) & LOW_BYTE_MASK);
  // Produce Mode 3 square wave rather than pulse in Mode 2
  outb(PIT_MODE_SQR_WAV, PIT_CMD_REGISTER); //mode 3 square wave
  outb(lowerEightFreq, PIT_CHANNEL_0);
  outb(higherEightFreq, PIT_CHANNEL_0);
  // Enable line 0
  enable_irq(PIT_IRQ_NUM);
}

/*
 * PIT_HANDLER
 *   DESCRIPTION: Manage PIT interrupts (round-robin scheduling)
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
void PIT_HANDLER() {
  send_eoi(PIT_IRQ_NUM);

  cli();
  if (terminals[1].launched == 1 || terminals[2].launched == 1) {
      uint32_t idx = getNextProcess(currTerminalIndex);
      contextSwitch(idx);
  }
  sti();
}

/*
 * contextSwitch
 *   DESCRIPTION: Does a context switch to the process specified
 *   INPUTS: processNum
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
void contextSwitch(const int32_t nextTerminalIndex) {
  ///// TODO /////
  // 1. Update pcb->terminal_id in execute()
  // 2. Figure out this paging garbage on line 64

  // Pointer to current and next pcb
  int32_t currProcessNum = terminals[currTerminalIndex].currentActiveProcess;
  int32_t nextProcessNum = terminals[nextTerminalIndex].currentActiveProcess;
  pcb_t * currPCB = generatePCBPointer(currProcessNum);
  pcb_t * nextPCB = generatePCBPointer(nextProcessNum);

  // Save esp/ebp
  asm volatile ("movl %%esp, %0" : "=r" (currPCB->pcbESP));
  asm volatile ("movl %%ebp, %0" : "=r" (currPCB->pcbEBP));
  // Update paging
  getNew4MBPage(PageSize128MB, (nextProcessNum * PageSize4MB) + PageSize8MB);
  // Check if terminal is being displayed currently
  if (nextTerminalIndex != nextPCB->terminal_id) {
      //// Virtual vidmap stuff
      //getNew4KBPage();
  } else {
      //// Normal video paging
      // getNew4KBPage();
  }
  // Update currentTerminalIndex
  currTerminalIndex = nextTerminalIndex;
  // Save ss0, esp0 in TSS
  tss.ss0 = KERNEL_DS;
  tss.esp0 = PageSize8MB - PageSize8KB * (nextProcessNum) - fourBytes;
  // Do Context Switch
  asm volatile("movl %0, %%esp" : :"r"(nextPCB->pcbESP));
  asm volatile("movl %0, %%ebp" : :"r"(nextPCB->pcbEBP));
  return;
}

/*
 * getNextProcess
 *   DESCRIPTION: Cycle to the next running process
 *   INPUTS: curr_idx - currentTerminalIndex
 *   OUTPUTS: none
 *   RETURN VALUE: current pid
 */
int32_t getNextProcess(int32_t curr_idx) {
  // Default nextTerminalIndex to curr index
  int32_t nextTerminalIndex = curr_idx;
  // If no other terminals have a running process, it'll go back to itself
  while(terminals[nextTerminalIndex].launched == 0) {
    // Go to next terminal # and cycle if necessary
    nextTerminalIndex++;
    nextTerminalIndex %= num_terminals;
  }
  // Return next pid
  return nextTerminalIndex;
}
