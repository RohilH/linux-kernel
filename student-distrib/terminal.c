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
<<<<<<< HEAD
int32_t terminal_read (int32_t fd, void* buf, int32_t nbytes) {
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
=======
int32_t terminalRead (int32_t fd, void* buf, int32_t nbytes) {
    int i = 0;
    char * buffer = (char*) buf;
    while(enterPressed != 1); // wait until enter is pressed by keyboard
    enterPressed = 0;
>>>>>>> master
    for (i = 0; i < nbytes && i < BUFFSIZE; i++) {
        buffer[i] = charBuffer[i]; // copy charBuffer into buffer
    }
<<<<<<< HEAD
    buffer[i] = '\0'; // Null terminated string
    char enterChar = '\n'; // New line
    putc(enterChar);
    clearCharBuffer(); // Reset terminal input line
    return i;
=======
    buffer[i] = '\0'; // end with null terminated string
    char enterChar = '\n'; // new line
    putc(enterChar);
    clearCharBuffer(); // clear char buffer
    return i; // return bytes read
>>>>>>> master
}

/*
 * terminalWrite
 *     DESCRIPTION: prints characters in buf to screen
 *     INPUTS: fd (file descriptor), buf (memory to print to screen), nbytes (number of bytes to read)
 *     OUTPUTS: none
 *     RETURN VALUE: number of bytes read
 */
<<<<<<< HEAD
int32_t terminal_write (int32_t fd, const void* buf, int32_t nbytes) {
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
=======
int32_t terminalWrite (int32_t fd, const void* buf, int32_t nbytes) {
    int i;
    char * buffer = (char*) buf;
    for (i = 0; i < nbytes; i++) {
      putc(buffer[i]); // print buffer
    }
    char enterChar = '\n';
    putc(enterChar); // new line
    return i;
>>>>>>> master
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
