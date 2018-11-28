#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "types.h"

#define num_terminals 3
#define bufSize 128

typedef struct terminal {
  int32_t id;
  int32_t screen_x, screen_y;
  char input_buf[bufSize];
  // ...
} terminal_t;

terminal_t terminals[num_terminals];

// System Calls for terminal
int32_t terminal_read (int32_t fd, void* buf, int32_t nbytes);
int32_t terminal_write (int32_t fd, const void* buf, int32_t nbytes);
int32_t terminal_open (const uint8_t* filename);
int32_t terminal_close (int32_t fd);

// Multiple Terminal Support
void switch_terminal(const int32_t id);

#endif
