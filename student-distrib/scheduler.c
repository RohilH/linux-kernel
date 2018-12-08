#include "scheduler.h"

/*
 * PIT_INIT
 *     DESCRIPTION: Initializes the PIT and sets it's timer to 10 ms
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 */
int shellsStarted, firstShellStarted;
void PIT_INIT() {
  // Obtain high and low bits of frequency divider value
  uint8_t lowerEightFreq  = (uint8_t)((PIT_MAX_FREQ / RELOAD_VALUE) & LOW_BYTE_MASK);
  uint8_t higherEightFreq = (uint8_t)(((PIT_MAX_FREQ / RELOAD_VALUE) >> BIT_SHIFT) & LOW_BYTE_MASK);
  // Initialize variables for 3-terminal init
  shellsStarted = 0;
  firstShellStarted = 0;
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
    // Context switch if all three terminals are already launched
    if (terminals[0].launched == 1 && terminals[1].launched == 1 && terminals[2].launched == 1) {
        if (shellsStarted == 0) {
            shellsStarted = 1;
            mult_terminal_launch(0);
        }
        pcb_t * currPCB = generatePCBPointer(currProcessIndex);
        int curr_term = currPCB->terminal_id;
        // Increment curr_term to contextSwitch into
        curr_term = (curr_term + 1) % num_terminals;
        enable_irq(PIT_IRQ_NUM);
        contextSwitch(curr_term);
        return;
    // Otherwise, initialize all terminals
    } else {
      // Initialize all terminal address/properties
      if (firstShellStarted == 0) {
          firstShellStarted = 1;
          mult_terminal_init();
      }
      disable_irq(PIT_IRQ_NUM);
      if (terminals[1].launched == 0) {
          launch_terminal(1);
          enable_irq(PIT_IRQ_NUM);
          // Execute Shell
          uint8_t* shellCommand = (uint8_t*)"shell";
          execute(shellCommand);
      }
      if (terminals[2].launched == 0) {
          launch_terminal(2);
          enable_irq(PIT_IRQ_NUM);
          // Execute Shell
          uint8_t* shellCommand = (uint8_t*)"shell";
          execute(shellCommand);
      }
    }
}

/*
 * contextSwitch
 *   DESCRIPTION: Does a context switch to the process specified
 *   INPUTS: processNum
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
void contextSwitch(const int32_t nextTerminalIndex) {
    // Obtain PCB for current process and process to switch into
    pcb_t * currPCB = generatePCBPointer(currProcessIndex);
    pcb_t * nextPCB = generatePCBPointer(terminals[nextTerminalIndex].currentActiveProcess);

    uint8_t* screenStart;
    vidMap(&screenStart);
    if (currTerminalIndex != nextTerminalIndex) {
        getNew4KBPage((uint32_t)screenStart, (uint32_t)terminals[nextTerminalIndex].videoMemPtr);
    }
    // Save EBP
    asm volatile ("movl %%esp, %0" : "=r" (currPCB->currESP));
    asm volatile ("movl %%ebp, %0" : "=r" (currPCB->currEBP));
    // Map new page
    getNew4MBPage(VirtualStartAddress, kernelStartAddr + PageSize4MB*((terminals[nextTerminalIndex].currentActiveProcess) + 1));
    currProcessIndex = terminals[nextTerminalIndex].currentActiveProcess;
    // Update paging
    tss.ss0 = KERNEL_DS;
    tss.esp0 = PageSize8MB - PageSize8KB * (terminals[nextTerminalIndex].currentActiveProcess) - fourBytes;
    currTerminalScheduler = nextTerminalIndex;

    // Do Context Switch
    asm volatile ("movl %0, %%esp" : : "r" (nextPCB->currESP));
    asm volatile ("movl %0, %%ebp" : : "r" (nextPCB->currEBP));
}
