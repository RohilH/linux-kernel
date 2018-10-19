#include "keyboard.h"
#include "i8259.h"
// #include "x86_desc.h"
// #include "idt.h"
#include "lib.h"

/* Port read functions */
/* Inb reads a byte and returns its value as a zero-extended 32-bit
 * unsigned int */
// static inline uint32_t inb(port) {
//     uint32_t val;
//     asm volatile ("             \n\
//             xorl %0, %0         \n\
//             inb  (%w1), %b0     \n\
//             "
//             : "=a"(val)
//             : "d"(port)
//             : "memory"
//     );
//     return val;
// }

/* https://wiki.osdev.org/PS/2_Keyboard#Scan_Code_Set_1 */
static char scanCodeToChar[60] = { ' ', ' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
                                   '-', '=', ' ', ' ', 'q', 'w', 'e', 'r', 't', 'y', 'u',
                                   'i', 'o', 'p', '[', ']', ' ', ' ', 'a', 's', 'd',
                                   'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', ' ', '\\',
                                   'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', ' ',
                                   '*', ' ', ' ', ' '};
//


void KEYBOARD_INIT() {
  enable_irq(1);
}

void KEYBOARD_HANDLER() {
  asm("pusha");
  uint32_t prevScanCode = 0;
  while(1) {
    uint32_t scanCode = inb(0x60);
    if (scanCode != 0 && scanCode < 0x80) {
      if (scanCode != prevScanCode)
         printf("%c", scanCodeToChar[scanCode]);
        //printf("Key is pressed");
      // else
      //   printf("                  ");
    }
    if (scanCodeToChar[scanCode] == '`')
      break;
    prevScanCode = scanCode;
  }
  send_eoi(1);
  asm("popa");
  // disable_irq(1);
}
