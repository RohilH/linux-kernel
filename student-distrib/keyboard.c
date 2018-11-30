#include "keyboard.h"
#include "i8259.h"
#include "lib.h"

uint32_t scanCode; // Current key being pressed
uint32_t prevScanCode; // Previous key pressed
volatile int shift, caps, ctrl, alt, enterPressed; // Key flags

/* https://wiki.osdev.org/PS/2_Keyboard#Scan_Code_Set_1 */

volatile char charBuffer[BUFFSIZE];
int buffIndex;
                                    // 60 keys for 4 different situations listed above each array
                                    // Neither caps lock or shift pressed
static char scanCodeToChar[4][60] = {{'\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
                                    '-', '=', ' ', '\0', 'q', 'w', 'e', 'r', 't', 'y', 'u',
                                    'i', 'o', 'p', '[', ']', '\n', '\0', 'a', 's', 'd',
                                    'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '\0', '\\',
                                    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '\0',
                                    '*', '\0', ' ', '\0'},
                                    // Caps lock pressed
                                    {'\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
                                    '-', '=', ' ', '\0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U',
                                    'I', 'O', 'P', '[', ']', '\n', '\0', 'A', 'S', 'D',
                                    'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', '\0', '\\',
                                    'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', '\0',
                                    '*', '\0', ' ', '\0'},
                                    // Shift pressed
                                    {'\0', '\0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
                                    '_', '+', ' ', '\0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U',
                                    'I', 'O', 'P', '{', '}', '\n', '\0', 'A', 'S', 'D',
                                    'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', '\0', '|',
                                    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', '\0',
                                    '*', '\0', ' ', '\0'},
                                    // Caps and shift pressed
                                    {'\0', '\0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
                                    '_', '+', ' ', '\0', 'q', 'w', 'e', 'r', 't', 'y', 'u',
                                    'i', 'o', 'p', '{', '}', '\n', '\0', 'a', 's', 'd',
                                    'f', 'g', 'h', 'j', 'k', 'l', ':', '\"', '~', '\0', '|',
                                    'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', '\0',
                                    '*', '\0', ' ', '\0'}};

/*
 * KEYBOARD_INIT
 *     DESCRIPTION: Initialize keyboard IRQ and flags
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 */
void KEYBOARD_INIT() {
    cli();
    enable_irq(IRQ_LINE_KEYS); // Enable keyboard IRQ line

    // Initialize all vars to zero
    scanCode = 0, prevScanCode = 0;
    shift = 0, caps = 0, ctrl = 0, alt = 0, enterPressed = 0;
    buffIndex = 0;
    commandIndex = 0;
    sti();
}

/*
 * KEYBOARD_HANDLER
 *     DESCRIPTION: Handles keyboard press
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 */
void KEYBOARD_HANDLER() {
    cli();

    scanCode = inb(0x60); // Obtain key scan code
    if (scanCode != 0 && scanCode < KEY_PRESSED) { // Check validity
        if (prevScanCode != scanCode) { // Check spamming
            if (ctrl && (scanCode == L_PRESSED)) { // Handle clear screen
                clear();
                clearCharBuffer();
            } else if (scanCode == ENTER_PRESSED) { // Handle enter
                enter();
            } else if (scanCode == BACKSPACE_PRESSED) { // Handle backspace
                backspace();
            } else if (scanCode == UP_ARROW_PRESSED) { // Handle up-arrow for commands
                //upArrow();
            } else if (scanCode == DOWN_ARROW_PRESSED) {
                //downArrow();
            } else if (caps && shift) { // Handle caps and shift
                addCharToBuffer(scanCode, 3);
            } else if (shift) { // Handle shift only
                addCharToBuffer(scanCode, 2);
            } else if (caps) { // Handle caps only
                addCharToBuffer(scanCode, 1);
            } else if (scanCode == F1_PRESSED && alt) { // Handle ALT + Fn
                send_eoi(IRQ_LINE_KEYS); // Send end of interrupt to IRQ line 1
                mult_terminal_launch(terminal_one);
            } else if (scanCode == F2_PRESSED && alt) {
                send_eoi(IRQ_LINE_KEYS); // Send end of interrupt to IRQ line 1
                mult_terminal_launch(terminal_two);
            } else if (scanCode == F3_PRESSED && alt) {
                send_eoi(IRQ_LINE_KEYS); // Send end of interrupt to IRQ line 1
                mult_terminal_launch(terminal_three);
            } else if (scanCode < 0x3B){ // Handle valid keypress
                addCharToBuffer(scanCode, 0);
            }
        }
    }
    // Handle alternating caps logic
    if (scanCode == CAPS_RELEASED) {
        caps = (caps == 0) ? 1 : 0;
    }

    // Handle shift logic
    if (scanCode == LSHIFT_PRESSED || scanCode == RSHIFT_PRESSED) shift = 1;
    if (scanCode == LSHIFT_RELEASED || scanCode == RSHIFT_RELEASED) shift = 0;

    // Handle CTRL logic
    if (scanCode == CRTL_PRESSED) ctrl = 1;
    if (scanCode == CRTL_RELEASED) ctrl = 0;

    // Handle ALT logic
    if (scanCode == ALT_PRESSED) alt = 1;
    if (scanCode == ALT_RELEASED) alt = 0;

    prevScanCode = scanCode; // Update previous scan code

    send_eoi(IRQ_LINE_KEYS); // Send end of interrupt to IRQ line 1

    sti();
}

/*
 * addCharToBuffer
 *     DESCRIPTION: Handles printing character to screen
 *     INPUTS: scanCodeKey (scan code of key pressed), charType (determines which char to print)
 *     OUTPUTS: none
 *     RETURN VALUE: none
 */
void addCharToBuffer(uint32_t scanCodeKey, uint8_t charType) {
    char charToAdd = scanCodeToChar[charType][scanCodeKey]; // Obtain scan code
    if (buffIndex < BUFFSIZE && charToAdd != '\0') { // Check out of bounds error and null-terminating char
        charBuffer[buffIndex] = charToAdd; // Add to buffer
        if(commandIndex == 0) {
          typedBuffer[buffIndex] = charToAdd;
        }
        buffIndex++;
        putc(charToAdd); // Add key to stream
    }
}

/*
 * backspace
 *     DESCRIPTION: Handles removing character from key buffer and screen
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 */
void backspace() {
    if (buffIndex > 0) { // Check for remaining chars in buffer
        buffIndex--;
        charBuffer[buffIndex] = '\0'; // Add null-reminating char
        typedBuffer[buffIndex] = '\0';
        removec();
    }
}

/*
 * enter
 *     DESCRIPTION: Handles enter being pressed for terminal read
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 */
void enter() {
    int i;
    int currCommand;
      for(currCommand = COMMAND_LIMIT-1; currCommand > 0; currCommand--) { //Shift all the commands right, effectively popping off the right most
          for(i = 0; i<BUFFSIZE; i++) { //clear what is going to be replaced
            if(commandStorage[currCommand][i] != '\0') {
              commandStorage[currCommand][i] = '\0';
            }
          }
          for(i = 0; i<BUFFSIZE; i++) {
            if(commandStorage[currCommand - 1][i] != '\0') { //move the buffer up an index
              commandStorage[currCommand][i] = commandStorage[currCommand - 1][i];
            }
          }
      }
      for(i = 0; i<BUFFSIZE; i++) { //clear the first stored buffer
        if(commandStorage[0][i] != '\0') {
          commandStorage[0][i] = '\0';
        }
      }
      for(i = 0; i<BUFFSIZE; i++) { //replace the first stored buffer with what was entered by user
        if(charBuffer[i] != '\0') {
          commandStorage[0][i] = charBuffer[i];
        }
      }
      if(currStored != COMMAND_LIMIT) {
        currStored++; //increment how many buffers are being
      }
    commandIndex = 0; //reset commandIndex back to 0
    enterPressed = 1;
}

/*
 * upArrow
 *     DESCRIPTION: Puts the previous command(s) on to the terminal and into the buffer
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 */
void upArrow() {
  int i;
  if(commandIndex < (currStored)) { //check that the current command is within the bounds of what is actually stored
    for(i = 0; i<BUFFSIZE; i++) {
      if(charBuffer[i] != '\0') { //clear the current charBuffer
        charBuffer[i] = '\0';
        removec(); //remove it from the screen
      }
    }
    buffIndex = 0; //reset buffer index
    for(i=0;i<BUFFSIZE; i++) {
      if(commandStorage[commandIndex][i] != '\0') {
        charBuffer[i] = commandStorage[commandIndex][i];
        putc(charBuffer[i]);
        buffIndex++;
      }
    }
    commandIndex++;
  }
}

/*
 * downArrow
 *     DESCRIPTION: Puts the previous command(s) on to the terminal and into the buffer
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 */
void downArrow() {
  if(commandIndex>=1) {
    commandIndex--;
    int i;
    if(commandIndex == 0) { //case for when the user wants to return back to the command they originally typed
      for(i = 0; i<BUFFSIZE; i++) {
        if(charBuffer[i] != '\0') {
          charBuffer[i] = '\0';
          removec();
        }
      }
      buffIndex = 0; //reset BuffIndex
      for(i=0;i<BUFFSIZE; i++) {
        if(typedBuffer[i] != '\0') {
          charBuffer[i] = typedBuffer[i]; //use typed buffer which is unaffected by any up or down arrow presses
          putc(charBuffer[i]); //output to terminal
          buffIndex++;
        }
      }
    }
    else{
      for(i = 0; i<BUFFSIZE; i++) { //remove previous command from the buffer and the terminal
        if(charBuffer[i] != '\0') {
          charBuffer[i] = '\0';
          removec();
        }
      }
      buffIndex = 0;
      for(i=0;i<BUFFSIZE; i++) { //puts the command history that is one down into the buffer and onto the terminal
        if(commandStorage[commandIndex - 1][i] != '\0') {
          charBuffer[i] = commandStorage[commandIndex - 1][i];
          putc(charBuffer[i]);
          buffIndex++;
        }
      }
    }
  }
}

/*
 * clearCharBuffer
 *     DESCRIPTION: Handles clearing characters from key buffer
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 */
void clearCharBuffer() {
    int i;
    for (i = 0; i < BUFFSIZE; i++) {
      charBuffer[i] = '\0'; // Set elements to null-terminating char
      typedBuffer[i] = '\0'; // Set elements to null-terminating char
    }
    buffIndex = 0;
}
