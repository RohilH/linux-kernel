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
    if (terminals[0].launched == 1 && terminals[1].launched == 1 && terminals[2].launched == 1) {
        if (shellsStarted == 0) {
            shellsStarted = 1;
            mult_terminal_launch(0);
        }
        pcb_t * currPCB = generatePCBPointer(currProcessIndex);
        int curr_term = currPCB->terminal_id;
        // Default nextTerminalIndex to curr index
        while (1) {
            curr_term = (curr_term + 1) % num_terminals;
            if (terminals[curr_term].launched == 1)
                break;
        }
        enable_irq(PIT_IRQ_NUM);
        contextSwitch(curr_term);
        return;
    }

    if (firstShellStarted == 0) {
        firstShellStarted = 1;
        mult_terminal_init();
    }
    disable_irq(PIT_IRQ_NUM);
    if (terminals[1].launched == 0) {
        cli();
        mult_terminal_save(currTerminalIndex);

        pcb_t* currPCB = generatePCBPointer(currProcessIndex);
        // if the terminal is already launched, restore the state
        currTerminalIndex = 1;
        terminals[1].launched = 1;

        mult_terminal_restore(1);
        // Store ESP and EBP in pcb
        uint32_t storeESP;
        uint32_t storeEBP;
        asm volatile ("movl %%esp, %0" : "=r" (storeESP));
        asm volatile ("movl %%ebp, %0" : "=r" (storeEBP));
        // printf("Current active process in this terminal: %d\n", terminals[currTerminalIndex].currentActiveProcess);

        // Update current PCB
        currPCB->currESP = storeESP;
        currPCB->currEBP = storeEBP;
        uint8_t* shellCommand = (uint8_t*)"shell";
        sti();
        enable_irq(PIT_IRQ_NUM);
        execute(shellCommand);
    }
    if (terminals[2].launched == 0) {
        cli();
        mult_terminal_save(currTerminalIndex);

        pcb_t* currPCB = generatePCBPointer(currProcessIndex);
        // if the terminal is already launched, restore the state
        currTerminalIndex = 2;
        terminals[2].launched = 1;

        mult_terminal_restore(2);
        // Store ESP and EBP in pcb
        uint32_t storeESP;
        uint32_t storeEBP;
        asm volatile ("movl %%esp, %0" : "=r" (storeESP));
        asm volatile ("movl %%ebp, %0" : "=r" (storeEBP));
        // printf("Current active process in this terminal: %d\n", terminals[currTerminalIndex].currentActiveProcess);

        // Update current PCB
        currPCB->currESP = storeESP;
        currPCB->currEBP = storeEBP;
        uint8_t* shellCommand = (uint8_t*)"shell";
        sti();
        enable_irq(PIT_IRQ_NUM);
        execute(shellCommand);
    }
    // cli();
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
    if (currTerminalIndex != nextTerminalIndex) {
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
