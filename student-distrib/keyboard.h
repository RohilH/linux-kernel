#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "types.h"

#define IRQ_LINE_KEYS 1
#define BUFFSIZE 128

extern volatile char charBuffer[BUFFSIZE];
extern volatile int enterPressed;

extern void KEYBOARD_INIT();
extern void KEYBOARD_HANDLER();
void addCharToBuffer(uint32_t scanCodeKey, uint8_t charType);
void clearCharBuffer();
void backspace();
void enter();
#endif
