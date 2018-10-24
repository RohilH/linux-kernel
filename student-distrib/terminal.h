#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "types.h"

int32_t terminalRead (int32_t fd, void* buf, int32_t nbytes);
int32_t terminalWrite (int32_t fd, const void* buf, int32_t nbytes);
int32_t terminalOpen (const uint8_t* filename);
int32_t terminalClose (const uint8_t* filename);

#endif
