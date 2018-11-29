#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "types.h"

#define num_terminals 3
#define bufSize 128
#define nullChar '\0'

typedef struct terminal {
  int32_t id;
  int32_t screen_x, screen_y;
  uint8_t launched;
  volatile char charBuffer[bufSize];
  int32_t currentActiveProcess;
  uint8_t * videoMemPtr;
  // ...
} terminal_t;

volatile uint8_t currTerminalIndex;
terminal_t terminals[num_terminals];

// System Calls for terminal
int32_t terminal_read (int32_t fd, void* buf, int32_t nbytes);
int32_t terminal_write (int32_t fd, const void* buf, int32_t nbytes);
int32_t terminal_open (const uint8_t* filename);
int32_t terminal_close (int32_t fd);

// Multiple Terminal Support
void init_mult_terms();
void switch_terminals(const int32_t destination);
void save_terminal_state(const int32_t id);
void open_terminals(const int32_t id);


#endif
