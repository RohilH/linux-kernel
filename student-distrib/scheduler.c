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

    disable_irq(PIT_IRQ_NUM);
    // cli();
    if (terminals[1].launched == 1 || terminals[2].launched == 1) {
        pcb_t * currPCB = generatePCBPointer(currProcessIndex);
        int curr_term = currPCB->terminal_id;
        // Default nextTerminalIndex to curr index
        while (1) {
            curr_term = (curr_term + 1) % num_terminals;
            if (terminals[curr_term].launched == 1)
                break;
        }
        // printf("CurrActiveTermProc: %d ",curr_term);
        // uint32_t idx = getNextTerminal(curr_term);
        enable_irq(PIT_IRQ_NUM);
        contextSwitch(curr_term);
    }
    enable_irq(PIT_IRQ_NUM);
    // sti();
}

/*
 * contextSwitch
 *   DESCRIPTION: Does a context switch to the process specified
 *   INPUTS: processNum
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
void contextSwitch(const int32_t nextTerminalIndex) {
    pcb_t * currPCB = generatePCBPointer(currProcessIndex);
    pcb_t * nextPCB = generatePCBPointer(terminals[nextTerminalIndex].currentActiveProcess);

    // printf("currProcessIndex: %d, nextProcessIndex: %d \n", currProcessIndex, terminals[nextTerminalIndex].currentActiveProcess);

    uint8_t* screenStart;
    vidMap(&screenStart);
    if (nextTerminalIndex != currTerminalIndex) {
        getNew4KBPage((uint32_t)screenStart, (uint32_t)terminals[nextTerminalIndex].videoMemPtr);
    }
    // if (nextTerminalIndex != currTerminalIndex)
    asm volatile ("movl %%esp, %0" : "=r" (currPCB->currESP));
    asm volatile ("movl %%ebp, %0" : "=r" (currPCB->currEBP));

    getNew4MBPage(VirtualStartAddress, kernelStartAddr + PageSize4MB*((terminals[nextTerminalIndex].currentActiveProcess) + 1));
    currProcessIndex = terminals[nextTerminalIndex].currentActiveProcess;
    // Update paging
    tss.ss0 = KERNEL_DS;
    tss.esp0 = PageSize8MB - PageSize8KB * (terminals[nextTerminalIndex].currentActiveProcess) - fourBytes;

    currTerminalScheduler = nextTerminalIndex;
    //
    // // Do Context Switch
    asm volatile ("movl %0, %%esp" : : "r" (nextPCB->currESP));
    asm volatile ("movl %0, %%ebp" : : "r" (nextPCB->currEBP));

}

/*
 * getNextProcess
 *   DESCRIPTION: Cycle to the next running process
 *   INPUTS: curr_idx - currentTerminalIndex
 *   OUTPUTS: none
 *   RETURN VALUE: current pid
 */
int32_t getNextTerminal(int32_t curr_term) {
    pcb_t * currPCB = generatePCBPointer(currProcessIndex);

    // Default nextTerminalIndex to curr index
    int32_t i = currPCB->terminal_id;
    // If no other terminals have a running process, it'll go back to itself
    // Go to next terminal # and cycle if necessary
    for(i = 0; i < num_terminals; i++) {
        i = (i + 1) % num_terminals;
        if(terminals[i].launched == 1) {
          break;
        }
    }
    // Return next terminal index
    return i;
}
