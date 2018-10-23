#include "keyboard.h"
#include "i8259.h"
#include "lib.h"

uint32_t scanCode; // Current key being pressed
uint32_t prevScanCode; // Previous key pressed
int shift, caps, ctrl, alt; // Key flags

/* https://wiki.osdev.org/PS/2_Keyboard#Scan_Code_Set_1 */

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


// Initialize keyboard IRQ and flags
void KEYBOARD_INIT() {
    cli();
    enable_irq(IRQ_LINE_KEYS); // Enable keyboard IRQ line
    scanCode = 0, prevScanCode = 0; // Initialize all flags to zero
    shift = 0, caps = 0, ctrl = 0, alt = 0;
    sti();
}

void KEYBOARD_HANDLER() {
    asm("pusha");
    cli();

    scanCode = inb(0x60); // Obtain key scan code
    if (scanCode != 0 && scanCode < 0x80) { // Check validity
        if (prevScanCode != scanCode) { // Check spamming
            if (caps && shift) {
                putc(scanCodeToChar[3][scanCode]); // Handle normal chars
            }
            else if (shift)
            putc(scanCodeToChar[2][scanCode]); // Handle shift chars

            else if (caps) {
                putc(scanCodeToChar[1][scanCode]); // Handle caps chars
            }
            else if (ctrl && scanCode == 0x26) { // Handle clear screen
                clear();
                moveScreenPos(0,0);
            }
            else {
                putc(scanCodeToChar[0][scanCode]); // Handle normal chars
            }
        }
    }
    // Handle alternating caps logic
    if (prevScanCode == 0x3A && scanCode == 0xBA) {
        // caps = (caps == 0) ? 1 : 0;
        if (caps == 0) caps = 1;
        else caps = 0;
    }
    // Handle shift logic
    if (scanCode == 0x2A || scanCode == 0x36) shift = 1;
    if (scanCode == 0xAA || scanCode == 0xB6) shift = 0;


    // Handle CTRL logic
    if (scanCode == 0x1D) ctrl = 1;
    if (scanCode == 0x9D) ctrl = 0;



    prevScanCode = scanCode; // Update previous scan code

    send_eoi(IRQ_LINE_KEYS); // Send end of interrupt to IRQ line 1

    sti();
    asm("popa");
}
