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
      uint32_t idx = getNextProcess(currTerminalExecuted);
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
    pcb_t * currPCB = generatePCBPointer(terminals[currTerminalDisplayed].currentActiveProcess);
    pcb_t * nextPCB = generatePCBPointer(terminals[nextTerminalIndex].currentActiveProcess);
    // //printf("curr: %u, next: %u \n", currPCB -> terminal_id, nextPCB->terminal_id);
    // // Update paging
    getNew4MBPage(VirtualStartAddress, kernelStartAddr + PageSize4MB*((terminals[nextTerminalIndex].currentActiveProcess) + 1));


    tss.ss0 = KERNEL_DS;
    tss.esp0 = PageSize8MB - PageSize8KB * (terminals[nextTerminalIndex].currentActiveProcess) - fourBytes;
    // //
    currProcessIndex = terminals[nextTerminalIndex].currentActiveProcess;
    currTerminalExecuted = nextTerminalIndex;

    //
    // // Do Context Switch

    asm volatile ("movl %%esp, %0" : "=r" (currPCB->pcbESP));
    asm volatile ("movl %%ebp, %0" : "=r" (currPCB->pcbEBP));

    asm volatile ("movl %0, %%esp" : : "r" (nextPCB->pcbESP));
    asm volatile ("movl %0, %%ebp" : : "r" (nextPCB->pcbEBP));

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
  int32_t nextTerminalIndex = currTerminalExecuted;
  // If no other terminals have a running process, it'll go back to itself
  int32_t term_iter;
  // Go to next terminal # and cycle if necessary
  for(term_iter = (currTerminalExecuted + 1); (currTerminalExecuted%num_terminals)<3; (term_iter++)) {
    term_iter %= num_terminals;
    if(terminals[term_iter].launched == 1) {
      break;
    }
  }
  nextTerminalIndex = term_iter;
  // Return next terminal index
  return nextTerminalIndex;
}
