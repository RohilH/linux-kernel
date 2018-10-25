#include "terminal.h"
#include "keyboard.h"
#include "lib.h"

int32_t terminalRead (int32_t fd, void* buf, int32_t nbytes) {
    // printf("gets to beginning of read");
    int i = 0;
    char * buffer = (char*) buf;
    // printf("gets to derefencing buffer");
    while(enterPressed != 1);
    enterPressed = 0;
    if (nbytes > 128)
        return -1;
    for (i = 0; i < nbytes; i++) {
        // if (charBuffer[i] == '\n')
        //     break;
        buffer[i] = charBuffer[i];
    }
    // printf("accessed buffer");
    clearCharBuffer();
    return i;
}
int32_t terminalWrite (int32_t fd, const void* buf, int32_t nbytes) {
    int32_t ret;
    char * buffer = (char*) buf;
    ret = printf((char*) buffer);
    return ret;
}

int32_t terminalOpen (const uint8_t* filename) {
    return 0;
}
int32_t terminalClose (const uint8_t* filename) {
    return 0;
}
