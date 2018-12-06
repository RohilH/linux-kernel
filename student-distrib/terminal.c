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
    if(buf == NULL) return -1;
    // printf("gets to beginning of read");
    int i = 0;
    char * buffer = (char*) buf;
    // printf("gets to derefencing buffer");
    // Continuously check for enter flag until it is pressed (volatile)
    pcb_t* currPCB = generatePCBPointer(currProcessIndex);
    while(terminals[currPCB->terminal_id].enterPressed != 1);
    terminals[currPCB->terminal_id].enterPressed = 0;
    // if (nbytes > 128)
    //     return -1;
    // Copy charBuffer into local buffer
    for (i = 0; i < nbytes && i < BUFFSIZE; i++) {
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
    if(buf == NULL) return -1;
    int i;
    char * buffer = (char*) buf;
    // printf("currProcessIndex: %d\n", currProcessIndex);
    pcb_t* currPCB = generatePCBPointer(currProcessIndex);
    for (i = 0; i < nbytes; i++) {
        if (currPCB->terminal_id == currTerminalIndex) {
            putc(buffer[i]);
        }
        else {
            putcTerm(buffer[i], currPCB->terminal_id);
        //     // *(uint8_t *)(terminals[currPCB->terminal_id].videoMemPtr + ((NUM_COLS * get_screenY() + get_screenX()) << 1)) = buffer[i];
        }
        // putc(buffer[i]);

      // Add key to output stream
    }
    // ret = printf((char*) buffer);
    // char enterChar = '\n';
    // putc(enterChar);
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
    // check if id is within max terminal limit
    if (id < 0 || id > num_terminals - 1)
        return -1;
    // check if current terminal is the id
    if (currTerminalIndex == id)
        return 0;

    // save the state of the current terminal
    mult_terminal_save(currTerminalIndex);

    pcb_t* currPCB = generatePCBPointer(currProcessIndex);
    // if the terminal is already launched, restore the state
    if(terminals[id].launched == 1) {
        // int32_t currProcessNum = terminals[currTerminalIndex].currentActiveProcess;
        // int32_t nextProcessNum = terminals[id].currentActiveProcess;
        currTerminalIndex = id;
        mult_terminal_restore(id);
        // pcb_t * currPCB = generatePCBPointer(currProcessIndex);
        // pcb_t * nextPCB = generatePCBPointer(terminals[nextTerminalIndex].currentActiveProcess);
        //
        // // printf("currProcessIndex: %d, nextProcessIndex: %d \n", currProcessIndex, terminals[nextTerminalIndex].currentActiveProcess);
        // getNew4MBPage(VirtualStartAddress, kernelStartAddr + PageSize4MB*((terminals[nextTerminalIndex].currentActiveProcess) + 1));
        //
        uint8_t* screenStart;
        vidMap(&screenStart);
        if (id != currTerminalIndex) {
            getNew4KBPage((uint32_t)screenStart, (uint32_t)terminals[id].videoMemPtr);
        }
        // // if (nextTerminalIndex != currTerminalIndex)
        //
        // // Update paging
        // tss.ss0 = KERNEL_DS;
        // tss.esp0 = PageSize8MB - PageSize8KB * (terminals[nextTerminalIndex].currentActiveProcess) - fourBytes;
        // currProcessIndex = terminals[nextTerminalIndex].currentActiveProcess;
        //
        // asm volatile ("movl %%esp, %0" : "=r" (currPCB->currESP));
        // asm volatile ("movl %%ebp, %0" : "=r" (currPCB->currEBP));
        //
        // //
        // // // Do Context Switch
        // asm volatile ("movl %0, %%esp" : : "r" (nextPCB->currESP));
        // asm volatile ("movl %0, %%ebp" : : "r" (nextPCB->currEBP));

        // uint8_t* screenStart;
        // vidMap(&screenStart);
        // if (currPCB->terminal_id != currTerminalIndex) {
        //     getNew4KBPage((uint32_t)screenStart, (uint32_t)terminals[currPCB->terminal_id].videoMemPtr);
        // }
        // printf("Current active process in this terminal: %d\n", terminals[currTerminalIndex].currentActiveProcess);
        sti();
        return 0;
    }
    // Set launched value to 1
    // terminals[id].launched = 1;
    currTerminalIndex = id;


    mult_terminal_restore(id);
    // Store ESP and EBP in pcb
    uint32_t storeESP;
    uint32_t storeEBP;
    asm volatile ("movl %%esp, %0" : "=r" (storeESP));
    asm volatile ("movl %%ebp, %0" : "=r" (storeEBP));
    // printf("Current active process in this terminal: %d\n", terminals[currTerminalIndex].currentActiveProcess);

    // Update current PCB
    currPCB->currESP = storeESP;
    currPCB->currEBP = storeEBP;
    uint8_t* shellCommand = (uint8_t*)"shell";
    sti();
    execute(shellCommand);
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
    // Save screen_x, screen_y
    terminals[id].screen_x = get_screenX();
    terminals[id].screen_y = get_screenY();
    terminals[id].buffIndex = buffIndex;

    // Save charBuffer[bufSize];
    memcpy((int8_t *)terminals[id].charBuffer, (int8_t *)charBuffer, bufSize);

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
    // Restore screen_x, screen_y
    moveScreenPos(terminals[id].screen_x, terminals[id].screen_y);
    buffIndex = terminals[id].buffIndex;

    // Restore charBuffer[bufSize];
    memcpy((int8_t *)charBuffer, (int8_t *)terminals[id].charBuffer, bufSize);

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
        // char input_buf[BUFFSIZE];
        terminals[term_num].id = term_num;
        terminals[term_num].currentActiveProcess = -1;
        terminals[term_num].screen_x = 0;
        terminals[term_num].screen_y = 0;
        terminals[term_num].launched = 0;
        terminals[term_num].buffIndex = 0;
        terminals[term_num].enterPressed = 0;
        for(char_iter = 0; char_iter < BUFFSIZE; char_iter++) {
            terminals[term_num].charBuffer[char_iter] = nullChar;
        }
        getNewTerminal4KBPage(PageSize64MB, PageSize64MB + term_num * PageSize4KB, term_num);
        terminals[term_num].videoMemPtr = (uint8_t*)(PageSize64MB + term_num * PageSize4KB);
        // each terminal needs to be a diff color
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
    for(char_iter = 0; char_iter < BUFFSIZE; char_iter++) {
        charBuffer[char_iter] = terminals[0].charBuffer[char_iter];
    }
    for (i = 0; i < max_processes; i++) {
        activeProcessArray[i] = 0;
    }
    currProcessIndex = -1; // Initialize curr process index for PCB use
    currTerminalIndex = 0;
    uint8_t* shellCommand = (uint8_t*)"shell";
    execute(shellCommand);
}
