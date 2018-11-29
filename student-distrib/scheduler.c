#include "scheduler.h"
#include "sysCalls.h"
#include "lib.h"
#include "types.h"
#include "fileSystem.h"
#include "terminal.h"
#include "paging.h"
#include "i8259.h"

/*
 * PIT_INIT
 *     DESCRIPTION: Initializes the PIT and sets it's timer to 10 ms
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 */
void PIT_INIT() {
  // uint8_t lowerEightFreq  = (uint8_t)(_100_HZ_VAL & LOW_BYTE_MASK);
  // uint8_t higherEightFreq = (uint8_t)((_100_HZ_VAL >> 8) & LOW_BYTE_MASK);
  //
  // outb(PIT_MODE_SQR_WAV, PIT_CMD_REGISTER); //mode 3 square wave
  // outb(lowerEightFreq, PIT_CHANNEL_0);
  // outb(higherEightFreq, PIT_CHANNEL_0);
  //
  // enable_irq(PIT_IRQ_NUM);
}

void PIT_HANDLER() {
  // send_eoi(PIT_IRQ_NUM);
  // cli();
  //
  // sti();
}

/*
Things to keep in mind:
Need to keep track of the active process for each terminal. If we do this it makes life a lot easier






*/
