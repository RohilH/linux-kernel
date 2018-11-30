#include "terminal.h"
#include "keyboard.h"
#include "lib.h"
#include "sysCalls.h"
#include "paging.h"
/*
 * terminalRead
 *     DESCRIPTION: reads keyboard input after enterPressed = 1 and stores in buf; clears buffer
 *     INPUTS: fd (file descriptor), buf (memory to put keyboard info into), nBytes (number of bytes to read)
 *     OUTPUTS: none
 *     RETURN VALUE: number of bytes read
 */
int32_t terminal_read (int32_t fd, void* buf, int32_t nbytes) {
    if(buf == NULL) return -1;
    // printf("gets to beginning of read");
    int i = 0;
    char * buffer = (char*) buf;
    // printf("gets to derefencing buffer");
    // Continuously check for enter flag until it is pressed (volatile)
    while(enterPressed != 1);
    enterPressed = 0;
    // if (nbytes > 128)
    //     return -1;
    // Copy charBuffer into local buffer
    for (i = 0; i < nbytes && i < BUFFSIZE; i++) {
        buffer[i] = charBuffer[i]; // copy charBuffer into buffer
    }
    buffer[i] = '\0'; // Null terminated string
    char enterChar = '\n'; // New line
    putc(enterChar);
    clearCharBuffer(); // Reset terminal input line
    return i;
}

/*
 * terminalWrite
 *     DESCRIPTION: prints characters in buf to screen
 *     INPUTS: fd (file descriptor), buf (memory to print to screen), nbytes (number of bytes to read)
 *     OUTPUTS: none
 *     RETURN VALUE: number of bytes read
 */
int32_t terminal_write (int32_t fd, const void* buf, int32_t nbytes) {
    if(buf == NULL) return -1;
    int32_t ret;
    int i;
    char * buffer = (char*) buf;
    for (i = 0; i < nbytes; i++) {
      // Add key to output stream
      putc(buffer[i]);
    }
    // ret = printf((char*) buffer);
    // char enterChar = '\n';
    // putc(enterChar);
    return ret;
}

/*
 * terminalOpen
 *     DESCRIPTION: returns 0
 *     INPUTS: filename
 *     OUTPUTS: none
 *     RETURN VALUE: 0
 */
int32_t terminal_open (const uint8_t* filename) {
    return 0;
}

/*
 * terminalClose
 *     DESCRIPTION: returns 0
 *     INPUTS: filename (file descriptor)
 *     OUTPUTS: none
 *     RETURN VALUE: 0
 */
int32_t terminal_close (int32_t fd) {
    return 0;
}

/*
 * switch_terminal
 *     DESCRIPTION: Switch to specified terminal
 *     INPUTS: id - terminal to switch to (0-2)
 *     OUTPUTS: none
 *     RETURN VALUE: none
 */
void mult_terminal_switch(const int32_t destination) {
  cli();
  int char_iter;
  /***********************
  if the terminal launched is 0 then there are steps we need to take to launch
  the terminal that we need not take if the terminal is already done so.
  ***********************/
  if(terminals[destination] -> launched == 1) {
    mult_terminal_save(currTerminalIndex);
    mult_terminal_open(destination);
    for(char_iter = 0; char_iter < BUFFSIZE; char_iter++) {
      charBuffer[char_iter] = terminals[destination] -> charBuffer[char_iter];
    }

    //remap video memory;
    return;
  }
  else if(terminals[destination] -> launched == 0) {
    mult_terminal_save(currTerminalIndex);

    terminals[destination] -> launched = 1;
    for(char_iter = 0; char_iter < BUFFSIZE; char_iter++) {
      charBuffer[char_iter] = terminals[destination] -> charBuffer[char_iter];
    }

    uint8_t* shellCommand = (uint8_t*)"shell";
    execute(shellCommand);
    mult_terminal_open(destination);

  }
  return;
  // ...
  sti();
}

void mult_terminal_save(const int32_t id) {
  // terminals[id] -> screen_x = ;
  // terminals[id] -> currentActiveProce
}

void mult_terminal_open(const int32_t id) {

}

void mult_terminal_init() {
    int term_num;
    int char_iter;
    for(term_num = 0; term_num < 3; term_num++) {
        // char input_buf[BUFFSIZE];
        terminals[term_num] -> id = term_num;
        terminals[term_num] -> currentActiveProcess = -1;
        terminals[term_num] -> screen_x = 0;
        terminals[term_num] -> screen_y = 0;
        terminals[term_num] -> launched = 0;
        for(char_iter = 0; char_iter < BUFFSIZE; char_iter++) {
            terminals[term_num] -> charBuffer[char_iter] = nullChar;
        }
        getNewTerminal4KBPage(PageSize64MB, PageSize64MB + term_num * PageSize4KB, term_num);
        terminals[term_num] -> videoMemPtr = (uint8_t*)(PageSize64MB + term_num * PageSize4KB);
        // each terminal needs to be a diff color
    }
    for(char_iter = 0; char_iter < BUFFSIZE; char_iter++) {
        charBuffer[char_iter] = terminals[0] -> charBuffer[char_iter];
    }

    currTerminalIndex = 0;
    terminals[currTerminalIndex] -> launched = 1;
    uint8_t* shellCommand = (uint8_t*)"shell";
    execute(shellCommand);
}
