#include "terminal.h"
#include "keyboard.h"
#include "lib.h"

int32_t terminalRead (int32_t fd, void* buf, int32_t nbytes) {
    int i = 0;
    char * buffer = (char*) buf;
    if (nbytes > 128)
        return -1;
    for (i = 0; i < nbytes; i++) {
        buffer[i] = charBuffer[i];
    }
    clearCharBuffer();
    return i;
}
int32_t terminalWrite (int32_t fd, const void* buf, int32_t nbytes) {
    int32_t ret;
    char * buffer = (char*) buf;
    ret = printf(*buffer);
    return ret;
}

int32_t terminalOpen (const uint8_t* filename) {
    return 0;
}
int32_t terminalClose (const uint8_t* filename) {
    return 0;
}
