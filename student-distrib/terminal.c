#include "terminal.h"
#include "lib.h"
/*
 * terminalRead
 *     DESCRIPTION: reads keyboard input after enterPressed = 1 and stores in buf; clears buffer
 *     INPUTS: fd (file descriptor), buf (memory to put keyboard info into), nBytes (number of bytes to read)
 *     OUTPUTS: none
 *     RETURN VALUE: number of bytes read
 */
int32_t terminal_read (int32_t fd, void* buf, int32_t nbytes) {
    // Error checking
    if(buf == NULL) return -1;
    int i = 0;
    char * buffer = (char*) buf;
    // Continuously check for enter flag until it is pressed (volatile)
    pcb_t* currPCB = generatePCBPointer(currProcessIndex);
    while(terminals[currPCB->terminal_id].enterPressed != 1);
    terminals[currPCB->terminal_id].enterPressed = 0;
    // Copy charBuffer into local buffer
    for (i = 0; i < nbytes && i < BUF_SIZE; i++) {
        buffer[i] = charBuffer[i]; // copy charBuffer into buffer
    }
    buffer[i] = '\0'; // Null terminated string
    char enterChar = '\n'; // New line
    putc(enterChar);
    clearCharBuffer(); // Reset terminal input line
    return i;
}

/*
 * terminalWrite
 *     DESCRIPTION: prints characters in buf to screen
 *     INPUTS: fd (file descriptor), buf (memory to print to screen), nbytes (number of bytes to read)
 *     OUTPUTS: none
 *     RETURN VALUE: number of bytes read
 */
int32_t terminal_write (int32_t fd, const void* buf, int32_t nbytes) {
    // Error checking
    if(buf == NULL) return -1;
    int i;
    char * buffer = (char*) buf;
    for (i = 0; i < nbytes; i++) {
        pcb_t* currPCB = generatePCBPointer(currProcessIndex);
        // Print buffer to displayed terminal screen
        if (currPCB->terminal_id == currTerminalIndex) putc(buffer[i]);
        // Print buffer to another terminal screen
        else putcTerm(buffer[i], currPCB->terminal_id);
        // Add key to output stream
    }
    return i;
}

/*
 * terminalOpen
 *     DESCRIPTION: returns 0
 *     INPUTS: filename
 *     OUTPUTS: none
 *     RETURN VALUE: 0
 */
int32_t terminal_open (const uint8_t* filename) {
    return 0;
}

/*
 * terminalClose
 *     DESCRIPTION: returns 0
 *     INPUTS: filename (file descriptor)
 *     OUTPUTS: none
 *     RETURN VALUE: 0
 */
int32_t terminal_close (int32_t fd) {
    return 0;
}

/*
 * mult_terminal_launch
 *     DESCRIPTION: Switchs to specified terminal
 *     INPUTS: id
 *     OUTPUTS: none
 *     RETURN VALUE: 0 or -1
 */
int32_t mult_terminal_launch(const int32_t id) {
    cli();
    // Check if id is within max terminal limit
    if (id < 0 || id > NUM_TERMINALS - 1)
        return -1;
    // Check if current terminal is the id
    if (currTerminalIndex == id)
        return 0;
    // Save the state of the current terminal
    mult_terminal_save(currTerminalIndex);
    // Obtain PCB for current process
    pcb_t* currPCB = generatePCBPointer(currProcessIndex);
    // Update currTerminalIndex with id of terminal to switch into
    currTerminalIndex = id;
    // Restore state of terminal
    mult_terminal_restore(id);
    // Remap video memory and obtain virtual start address
    uint8_t* screenStart;
    vidMap(&screenStart);
    // Is terminal to switch into the one being displayed?
    if (currPCB->terminal_id != currTerminalIndex) {
        // Remap virtual video memory
        getNew4KBPage((uint32_t)screenStart, (uint32_t)terminals[id].videoMemPtr);
    }
    sti();
    return 0;
}

/*
 * mult_terminal_save
 *     DESCRIPTION: Saves the state of specified terminal
 *     INPUTS: id
 *     OUTPUTS: none
 *     RETURN VALUE: 0
 */
int32_t mult_terminal_save(const int32_t id) {
    // Check if id is within max terminal limit
    if (id < 0 || id > NUM_TERMINALS - 1)
        return -1;
    // Save screen_x, screen_y
    terminals[id].screen_x = get_screenX();
    terminals[id].screen_y = get_screenY();
    terminals[id].buffIndex = buffIndex;
    // Save charBuffer[BUF_SIZE];
    memcpy((int8_t *)terminals[id].charBuffer, (int8_t *)charBuffer, BUF_SIZE);
    // Save video memory ptr
    memcpy(terminals[id].videoMemPtr, (int8_t *)VIDEO, NUM_ROWS * NUM_COLS * 2);
    return 0;
}

/*
 * mult_terminal_restore
 *     DESCRIPTION: Restores the state of specified terminal
 *     INPUTS: id
 *     OUTPUTS: none
 *     RETURN VALUE: 0
 */
int32_t mult_terminal_restore(const int32_t id) {
    // Check if id is within max terminal limit
    if (id < 0 || id > NUM_TERMINALS - 1)
        return -1;
    // Restore screen_x, screen_y
    moveScreenPos(terminals[id].screen_x, terminals[id].screen_y);
    buffIndex = terminals[id].buffIndex;
    // Restore charBuffer[BUF_SIZE];
    memcpy((int8_t *)charBuffer, (int8_t *)terminals[id].charBuffer, BUF_SIZE);
    // Restore video memory
    memcpy((int8_t *)VIDEO, terminals[id].videoMemPtr, NUM_ROWS * NUM_COLS * 2);
    return 0;
}

/*
 * mult_terminal_init
 *     DESCRIPTION: Initializes multiple terminal support
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 */
void mult_terminal_init() {
    int i, term_num, char_iter;
    for(term_num = 0; term_num < 3; term_num++) {
        // char input_buf[BUF_SIZE];
        // Default values for terminal struct properties
        terminals[term_num].currentActiveProcess = -1;
        terminals[term_num].screen_x = 0;
        terminals[term_num].screen_y = 0;
        terminals[term_num].launched = 0;
        terminals[term_num].buffIndex = 0;
        terminals[term_num].enterPressed = 0;
        for(char_iter = 0; char_iter < BUF_SIZE; char_iter++) {
            terminals[term_num].charBuffer[char_iter] = NULL_CHAR;
        }
        // Change each terminal address
        getNewTerminal4KBPage(PageSize64MB, PageSize64MB + term_num * PageSize4KB, term_num);
        // Update videoMemPtr to this address
        terminals[term_num].videoMemPtr = (uint8_t*)(PageSize64MB + term_num * PageSize4KB);
        // Update colors of each terminal
        int i;
        for (i = 0; i < NUM_ROWS * NUM_COLS; i++) {
            *(uint8_t *)(terminals[term_num].videoMemPtr + (i << 1)) = ' ';
            if (term_num == 1)
                *(uint8_t *)(terminals[term_num].videoMemPtr + (i << 1) + 1) = ATTRIB2;
            else if (term_num == 2)
                *(uint8_t *)(terminals[term_num].videoMemPtr + (i << 1) + 1) = ATTRIB3;
            else
                *(uint8_t *)(terminals[term_num].videoMemPtr + (i << 1) + 1) = ATTRIB;
        }

    }
    // Default to first terminal
    currTerminalIndex = 0;
    // Default the global charBuffer to the first terminal's charBuffer
    for(char_iter = 0; char_iter < BUF_SIZE; char_iter++) {
        charBuffer[char_iter] = terminals[currTerminalIndex].charBuffer[char_iter];
    }
    // No process running at initialization
    for (i = 0; i < max_processes; i++) {
        activeProcessArray[i] = 0;
    }
    // Initialize curr process index for PCB use
    currProcessIndex = -1;
    // Execute shell
    uint8_t* shellCommand = (uint8_t*)"shell";
    execute(shellCommand);
}

/*
 * launch_terminal
 *     DESCRIPTION: Launch terminal with id
 *     INPUTS: id
 *     OUTPUTS: none
 *     RETURN VALUE: none
 */
void launch_terminal(uint32_t id) {
  cli();
  // Save state of current terminal
  mult_terminal_save(currTerminalIndex);
  // Obtain PCB of current process
  pcb_t* currPCB = generatePCBPointer(currProcessIndex);
  // If the terminal is already launched, restore the state
  currTerminalIndex = id;
  // Restore state of terminal we're launching (default values)
  mult_terminal_restore(id);
  // Store ESP and EBP in pcb
  uint32_t storeESP;
  uint32_t storeEBP;
  asm volatile ("movl %%esp, %0" : "=r" (storeESP));
  asm volatile ("movl %%ebp, %0" : "=r" (storeEBP));
  // Update current PCB
  currPCB->currESP = storeESP;
  currPCB->currEBP = storeEBP;
  sti();
}
