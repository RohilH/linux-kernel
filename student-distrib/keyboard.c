#include "keyboard.h"
#include "i8259.h"
#include "lib.h"

/* https://wiki.osdev.org/PS/2_Keyboard#Scan_Code_Set_1 */

                                    // neither caps lock or shift pressed
static char scanCodeToChar[3][60] = {{'\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
                                     '-', '=', ' ', '\0', 'q', 'w', 'e', 'r', 't', 'y', 'u',
                                     'i', 'o', 'p', '[', ']', '\n', '\0', 'a', 's', 'd',
                                     'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '\0', '\\',
                                     'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '\0',
                                     '*', '\0', ' ', '\0'},
                                    // caps lock pressed
                                    {'\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
                                      '-', '=', ' ', '\0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U',
                                      'I', 'O', 'P', '[', ']', '\n', '\0', 'A', 'S', 'D',
                                      'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', '\0', '\\',
                                      'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', '\0',
                                      '*', '\0', ' ', '\0'},
                                    // shift pressed
                                    {'\0', '\0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
                                     '_', '+', ' ', '\0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U',
                                     'I', 'O', 'P', '{', '}', '\n', '\0', 'A', 'S', 'D',
                                     'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', '\0', '\\',
                                     'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', '\0',
                                     '*', '\0', ' ', '\0'}};

void KEYBOARD_INIT() {
  enable_irq(1);
  clear();
}

void KEYBOARD_HANDLER() {
  asm("pusha");
  uint32_t prevScanCode = 0;
  int shift = 0;
  int caps = 0;
  while(1) {
    uint32_t scanCode = inb(0x60);
    if (scanCode!=0 && scanCode<0x80) {
      if (prevScanCode!=scanCode) {
        if (shift==1)
          printf("%c", scanCodeToChar[2][scanCode]);
        else if (caps==1)
          printf("%c", scanCodeToChar[1][scanCode]);
        else
          printf("%c", scanCodeToChar[0][scanCode]);
      }
    }
    if (prevScanCode==0x3A && scanCode == 0xBA) {
      if (caps==0) caps=1;
      else caps=0;
    }

    if (scanCode==0x2A || scanCode==0x36) shift = 1;
    if (scanCode==0xAA || scanCode==0xB6) shift = 0;
    prevScanCode = scanCode;

    if (scanCodeToChar[0][scanCode] == '`') {
      clear();
      break;
    }
  }
  send_eoi(1);
  asm("popa");
}
