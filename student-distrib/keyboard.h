#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "types.h"

#define IRQ_LINE_KEYS     1
#define BUFFSIZE          128
#define COMMAND_LIMIT     10
#define KEY_PRESSED       0x80
#define L_PRESSED         0x26
#define ENTER_PRESSED     0x1C
#define BACKSPACE_PRESSED 0x0E
#define CAPS_PRESSED      0x3A
#define CAPS_RELEASED     0xBA
#define LSHIFT_PRESSED    0x2A
#define RSHIFT_PRESSED    0x36
#define LSHIFT_RELEASED   0xAA
#define RSHIFT_RELEASED   0xB6
#define CRTL_PRESSED      0x1D
#define CRTL_RELEASED     0x9D
#define UP_ARROW_PRESSED  0x48
#define UP_ARROW_RELEASED 0xC8
#define DOWN_ARROW_PRESSED  0x50
#define DOWN_ARROW_RELEASED 0xD0

extern volatile char charBuffer[BUFFSIZE];
char typedBuffer[BUFFSIZE];
char commandStorage[COMMAND_LIMIT][BUFFSIZE];
extern volatile int enterPressed;
int commandIndex;
int currStored;
// see function descriptions in keyboard.c
extern void KEYBOARD_INIT();
extern void KEYBOARD_HANDLER();
void addCharToBuffer(uint32_t scanCodeKey, uint8_t charType);
void clearCharBuffer();
void backspace();
void enter();
void upArrow();
void downArrow();
#endif
