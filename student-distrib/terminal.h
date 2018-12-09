#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "keyboard.h"
#include "lib.h"
#include "sysCalls.h"
#include "paging.h"
#include "rtc.h"
#include "i8259.h"

#define NUM_TERMINALS 3
#define BUF_SIZE 128
#define NULL_CHAR '\0'

typedef struct terminal {
  int32_t screen_x, screen_y;
  uint8_t launched;
  volatile char charBuffer[BUF_SIZE];
  int buffIndex;
  volatile int enterPressed;
  int32_t currentActiveProcess;
  uint8_t * videoMemPtr;
  int32_t programFrequencyRTC;
  volatile int32_t rtcInterruptFlag;

  // ...
} terminal_t;

volatile uint8_t c_flag;

volatile uint8_t currTerminalIndex;
terminal_t terminals[NUM_TERMINALS];

// System Calls for terminal
int32_t terminal_read (int32_t fd, void* buf, int32_t nbytes);
int32_t terminal_write (int32_t fd, const void* buf, int32_t nbytes);
int32_t terminal_open (const uint8_t* filename);
int32_t terminal_close (int32_t fd);

// Multiple Terminal Support
void mult_terminal_init();
int32_t mult_terminal_launch(const int32_t id);
int32_t mult_terminal_save(const int32_t id);
int32_t mult_terminal_restore(const int32_t id);
void launch_terminal(uint32_t id);

#endif
