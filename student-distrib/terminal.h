#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "types.h"

#define num_terminals 3
#define bufSize 128
#define nullChar '\0'

typedef struct terminal {
  int32_t id;
  int32_t screen_x, screen_y;
  char input_buf[bufSize];
  int32_t currentActiveProcess;
  uint32_t videoMemAddr;
  // ...
} terminal_t;

terminal_t terminals[num_terminals];

// System Calls for terminal
int32_t terminal_read (int32_t fd, void* buf, int32_t nbytes);
int32_t terminal_write (int32_t fd, const void* buf, int32_t nbytes);
int32_t terminal_open (const uint8_t* filename);
int32_t terminal_close (int32_t fd);

// Multiple Terminal Support
void m_terminal_intialize();
void m_terminal_switch(const int32_t destination);
void m_terminal_save(const int32_t id);
void m_terminal_open(const int32_t id);
uint32_t currentTerminal;


#endif
