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
int32_t terminalRead (int32_t fd, void* buf, int32_t nbytes) {
    int i = 0;
    char * buffer = (char*) buf;
    while(enterPressed != 1); // wait until enter is pressed by keyboard
    enterPressed = 0;
    for (i = 0; i < nbytes && i < BUFFSIZE; i++) {
        buffer[i] = charBuffer[i]; // copy charBuffer into buffer
    }
    buffer[i] = '\0'; // end with null terminated string
    char enterChar = '\n'; // new line
    putc(enterChar);
    clearCharBuffer(); // clear char buffer
    return i; // return bytes read
}

/*
 * terminalWrite
 *     DESCRIPTION: prints characters in buf to screen
 *     INPUTS: fd (file descriptor), buf (memory to print to screen), nbytes (number of bytes to read)
 *     OUTPUTS: none
 *     RETURN VALUE: number of bytes read
 */
int32_t terminalWrite (int32_t fd, const void* buf, int32_t nbytes) {
    int i;
    char * buffer = (char*) buf;
    for (i = 0; i < nbytes; i++) {
      putc(buffer[i]); // print buffer
    }
    char enterChar = '\n';
    putc(enterChar); // new line
    return i;
}

/*
 * terminalOpen
 *     DESCRIPTION: returns 0
 *     INPUTS: filename
 *     OUTPUTS: none
 *     RETURN VALUE: 0
 */
int32_t terminalOpen (const uint8_t* filename) {
    return 0;
}

/*
 * terminalClose
 *     DESCRIPTION: returns 0
 *     INPUTS: filename (file descriptor)
 *     OUTPUTS: none
 *     RETURN VALUE: 0
 */
int32_t terminalClose (const uint8_t* fd) {
    return 0;
}
