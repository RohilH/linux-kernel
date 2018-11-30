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
      int32_t process = getNextProcess();
      contextSwitch(process);
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
void contextSwitch(const int32_t processNum) {

}

/*
 * getNextProcess
 *   DESCRIPTION: Gets the next process to be executed
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
int32_t getNextProcess() {

    return 0;
}
