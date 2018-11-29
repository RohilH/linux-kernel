#include "terminal.h"
#include "keyboard.h"
#include "lib.h"

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
void m_terminal_switch(const int32_t destination) {
    // cli();
    // printf("Switching terminal.....ruh roh");
    //
    // m_terminal_save(currentTerminal);
    // m_terminal_open(id);
    // currentTerminal = id;
    // // ...
    // sti();
}

void m_terminal_save(const int32_t id) {

}

void m_terminal_open(const int32_t id) {

}

void m_terminal_initialize() {
  // int term_num;
  // int char_iter;
  // for(int term_num = 0; term_num < 3; term_num++) {
  //   char input_buf[bufSize];
  //   terminals[term_num].id = term_num;
  //   terminals[term_num].currentActiveProcess = -1;
  //   terminals[term_num].screen_x = 0;
  //   terminals[term_num].screen_y = 0;
  //   for(char_iter = 0; char_iter < bufSize; char_iter++) {
  //     terminals[term_num].input_buf[char_iter] = nullChar;
  //   }
  //   videoMemAddr
  // }
  // each terminal needs to be
}
