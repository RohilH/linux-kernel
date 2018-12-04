#include "sysCalls.h"
#include "lib.h"
#include "types.h"
#include "fileSystem.h"
#include "paging.h"

/*
* emptyReturn
*     DESCRIPTION: Returns -1 for fileops failure
*     INPUTS: none
*     OUTPUTS: none
*     RETURN VALUE: -1
*/
int32_t emptyReturn() { return -1; }

// File operation tables for std, file, dir, rtc
fileOpsTable_t stdin = {terminal_open, terminal_read, emptyReturn, terminal_close};
fileOpsTable_t stdout = {terminal_open, emptyReturn, terminal_write, terminal_close};
fileOpsTable_t fileTable = {file_open, file_read, file_write, file_close};
fileOpsTable_t dirTable = {dir_open, dir_read, dir_write, dir_close};
fileOpsTable_t RTCTable = {rtc_open, rtc_read, rtc_write, rtc_close};
fileOpsTable_t blankTable = {emptyReturn, emptyReturn, emptyReturn, emptyReturn};

/*
 * initPCB
 *     DESCRIPTION: Setup PCB of current process
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: PCB of current process
 */
pcb_t* initPCB() {
    int i;
    for (i = 0; i < max_processes; i++) {
        if (activeProcessArray[i] == 1)
            continue;
        activeProcessArray[i] = 1;
        break;
    }
    // Check if # processes exceeds max # processes
    if (i >= max_processes) {
        // printf("Too many processes running; cannot create new PCB.");
        return NULL;
    }

    // Put PCB at top of respective kernel stack
    pcb_t* currPCB = generatePCBPointer(i);
    currPCB->prevPcbIdx = currProcessIndex;
    currProcessIndex = i;

    terminals[currTerminalIndex].currentActiveProcess = currProcessIndex;
    if (terminals[currTerminalIndex].launched == 0) {
        currPCB->prevPcbIdx = currProcessIndex;
        terminals[currTerminalIndex].launched = 1;
    }
    // Setup STDIN
    fileDescriptor_t stdinFD;
    // Point to respective file op function
    stdinFD.fileOpsTablePtr = &stdin;
    // Update current PCB with stdin
    currPCB->fileArray[0] = stdinFD;
    currPCB->fileArray[0].flags = 1;

    // Setup STDOUT
    fileDescriptor_t stdoutFD;
    // Point to respective file op function
    stdoutFD.fileOpsTablePtr = &stdout;
    // Update current PCB with stdout
    currPCB->fileArray[1] = stdoutFD;
    currPCB->fileArray[1].flags = 1;

    // Setup Blank tables for remaining files
    fileDescriptor_t emptyFD;
    emptyFD.fileOpsTablePtr = &blankTable;

    // Empty rest of current PCB
    for (i = 2; i < numFiles; i++) {
        currPCB->fileArray[i] = emptyFD;
        currPCB->fileArray[i].flags = 0;
    }

    return currPCB;
}

/*
 * halt
 *     DESCRIPTION: Terminate the running process
 *     INPUTS: uint8_t status - parent process value
 *     OUTPUTS: none
 *     RETURN VALUE: Parent is restored and process is terminated
 */
int32_t halt(uint8_t status) {
    // Restart shell if halting last process
    pcb_t* currPCB = generatePCBPointer(currProcessIndex);
    activeProcessArray[currProcessIndex] = 0;

    if(terminals[currTerminalIndex].currentActiveProcess == currPCB->prevPcbIdx) {
        terminals[currTerminalIndex].launched = 0;
        currProcessIndex--;
        uint8_t* shellCommand = (uint8_t*)"shell";
        execute(shellCommand);
    }
    cli();

    int i;
    uint32_t storeESP;
    uint32_t storeEBP;

    // Obtain current pcb_t *
    terminals[currPCB->terminal_id].currentActiveProcess = currPCB->prevPcbIdx;
    // Close relevant File descriptors
    for(i = 0; i < numFiles; i++) {
        currPCB->fileArray[i].fileOpsTablePtr = &blankTable;
        currPCB->fileArray[i].flags = 0;
        currPCB->fileArray[i].fileOpsTablePtr->close(i);
    }


    // Restore parent paging
    getNew4MBPage(VirtualStartAddress, kernelStartAddr + PageSize4MB*((currPCB->prevPcbIdx) + 1));
    // Jump to execute return
    storeESP = currPCB->parentESP;
    storeEBP = currPCB->parentEBP;
    // Modify TSS according to the parent
    tss.esp0 = currPCB->parentESP;
    uint32_t castStatus = (uint32_t)status;
    currProcessIndex = currPCB->prevPcbIdx;
    // IRET return
    asm volatile (
        "movl   %0, %%eax;"
        "movl   %1, %%esp;"
        "movl   %2, %%ebp;"
        "jmp execute_end;"
        // "leave;"
        // "ret;"
        :                       // no outputs
        : "r" (castStatus), "r" (storeESP), "r" (storeEBP)
        : "eax" // clobbers
        //
    );


  return 0; //never get to this point
}

/*
 * execute
 *     DESCRIPTION: Execute new program
 *     INPUTS: uint8_t * command - file name and arguments (separated by space)
 *     OUTPUTS: none
 *     RETURN VALUE: If program is not executed, return -1
                     If program is executed, return 0
 */
int32_t execute(const uint8_t * command) {
    if(command == NULL) return -1;
    cli();
    ///////* NOTE: STEP 1: Parse command for file name and argument */
    uint8_t filename[maxFileNameSize]; // initialize filename
    uint8_t argToPass[bufSize]; // INVALID: MUST BE STATIC NUM
    int ret;
    ret = parseCommands(command, filename, argToPass);
    // Return error check
    if (ret == -1) {
        sti();
        return -1;
    }

    ///////* NOTE: STEP 2: Check for valid executable */
    dentry_t dentry;
    if (read_dentry_by_name(filename, &dentry) != 0) {
        sti();
        return -1;
    }
    uint8_t tempBuffer[fourBytes];
    read_data (dentry.inodeNum, 0, tempBuffer, fourBytes);
    // 0: 0x7f; 1: 0x45; 2: 0x4c; 3: 0x46 magic numbers (makes sure file is executable)
    // Check for ELF in beginning four bytes of buffer
    if (tempBuffer[0] != del_CHAR || tempBuffer[1] != e_CHAR || tempBuffer[2] != l_CHAR || tempBuffer[3] != f_CHAR) {
        sti();
        return -1;
    }

    ///////* NOTE: STEP 3: Setup new PCB */
    pcb_t* currPCB = initPCB();
    // Check null
    if (currPCB == NULL) {
      sti();
      return -1;
    }
    uint32_t storeESP;
    uint32_t storeEBP;
    // Store ESP and EBP in pcb
    asm volatile ("movl %%esp, %0" : "=r" (storeESP));
    asm volatile ("movl %%ebp, %0" : "=r" (storeEBP));
    // Update current PCB
    currPCB->parentESP = storeESP;
    currPCB->parentEBP = storeEBP;
    currPCB->terminal_id = currTerminalIndex;

    strncpy((int8_t*)currPCB->bufferArgs, (int8_t*)argToPass, bufSize);
    read_data (dentry.inodeNum, execStartByte, tempBuffer, fourBytes); // get bytes 24 to 27
    uint32_t entryPoint = *((uint32_t*) tempBuffer);

    ///////* NOTE: STEP 4: Setup paging */
    getNew4MBPage(VirtualStartAddress, kernelStartAddr + PageSize4MB*(currProcessIndex + 1));

    ///////* NOTE: STEP 5: Setup User level Program Loader */
    read_data (dentry.inodeNum, 0, (uint8_t*) ProgramImageAddress, PageSize4MB); // loads executable into user video mem

    ///////* NOTE: STEP 6: Create TSS for context switching  */
    // Save ss0, esp0 in TSS
    tss.ss0 = KERNEL_DS;
    tss.esp0 = 0x800000 - 0x2000 * (currProcessIndex) - fourBytes;
    int userStackPtr = VirtualStartAddress + PageSize4MB - fourBytes;
    // Fake IRET
    asm volatile (
      "mov   $0x2B, %%ax;"
      "mov   %%ax, %%ds;"
      "mov   %%ax, %%es;"
      "mov   %%ax, %%fs;"
      "mov   %%ax, %%gs;"
      "pushl   $0x2B;"
      "pushl %0;"
      "pushfl;"
      "popl %%edx;"
      "orl $0x00000200, %%edx;"
      "pushl    %%edx;"
      "pushl   $0x23;"
      "pushl   %1;"
      "iret;"
      "execute_end:;"
      "leave;"
      "ret;"
      :                       // no outputs
      : "r" (userStackPtr), "r" (entryPoint)
      : "eax", "edx" // clobbers
    );
    sti();
    return 0;
}

/*
 * read
 *     DESCRIPTION: Dispatch read descriptor file operation function
 *     INPUTS: fd, buf, nBytes
 *     OUTPUTS: none
 *     RETURN VALUE: If any inputs are invalid or null, return -1
 *                   Else, return numner of read bytes.
 */
int32_t read(int32_t fd, void* buf, int32_t nBytes) {
    // Check for out of bounds and null erors
    if (fd < 0 || fd >= numFiles) return -1;
    if (buf == NULL) return -1;

    pcb_t* currPCB = generatePCBPointer(currProcessIndex);
    if (currPCB->fileArray[fd].flags == 0) // File not in use
        return -1;
    return currPCB->fileArray[fd].fileOpsTablePtr->read(fd, buf, nBytes);
}

/*
 * write
 *     DESCRIPTION: Dispatch write descriptor file operation function
 *     INPUTS: fd, buf, nBytes
 *     OUTPUTS: none
 *     RETURN VALUE: If any inputs are invalid or null, return -1
 *                   Else, return numner of read bytes.
 */
int32_t write(int32_t fd, const void* buf, int32_t nBytes) {
    // Check for out of bounds and null erors
    if (fd < 0 || fd >= numFiles) return -1;
    // Check for NULL buffer
    if (buf == NULL)  return -1;
    // Get pcb pointer
    pcb_t* currPCB = generatePCBPointer(currProcessIndex);
    // File not in use
    if (currPCB->fileArray[fd].flags == 0) return -1;
    return currPCB->fileArray[fd].fileOpsTablePtr->write(fd, buf, nBytes);
}

/*
 * open
 *     DESCRIPTION: Accesses filesystem and opens specified file
 *     INPUTS: fileName
 *     OUTPUTS: none
 *     RETURN VALUE: If file not opened, return -1
 *                   Else, return file index
 */
int32_t open(const uint8_t* fileName) {
    if(fileName == NULL) return -1;
    dentry_t dentry;
    // Check for invalid fileName
    if (read_dentry_by_name(fileName, &dentry) == -1) return -1;
    int i;
    // Get current pcb pointer
    pcb_t* currPCB = generatePCBPointer(currProcessIndex);
    for (i = 2; i < numFiles; i++) {
        if ((currPCB->fileArray[i]).flags == 0) { // if file slot is open
            currPCB->fileArray[i].flags = 1;
            currPCB->fileArray[i].filePosition = 0;
            break;
        }
    }
    // If PCB files are full
    if (i == numFiles) return -1;

    if (dentry.fileType == 0) { // RTC
        if (rtc_open(fileName) == -1) { // failed rtc open
            return -1;
        }
        currPCB->fileArray[i].fileOpsTablePtr = &RTCTable;
        currPCB->fileArray[i].inodeNum = NULL;
    }
    else if (dentry.fileType == 1) { // Directory
        if (dir_open(fileName) == -1) { // failed dir open
            return -1;
        }
        currPCB->fileArray[i].fileOpsTablePtr = &dirTable;
        currPCB->fileArray[i].inodeNum = NULL;
    }
    else if (dentry.fileType == 2) { // File
        if (file_open(fileName) == -1) { // failed file open
            return -1;
        }
        currPCB->fileArray[i].fileOpsTablePtr = &fileTable;
        currPCB->fileArray[i].inodeNum = dentry.inodeNum;
    }
    return i;
}

/*
 * close
 *     DESCRIPTION: Close specified file
 *     INPUTS: fileName
 *     OUTPUTS: none
 *     RETURN VALUE: If file not opened, return -1
 *                   Else, return file index
 */
int32_t close(int32_t fd) {
    // Check for bound error
    if (fd < 2 || fd >= numFiles) return -1;
    // Get current pcb pointer
    pcb_t* currPCB = generatePCBPointer(currProcessIndex);
    // File not in use
    if (currPCB->fileArray[fd].flags == 0) return -1;
    // Set to not in use
    currPCB->fileArray[fd].flags = 0;
    return currPCB->fileArray[fd].fileOpsTablePtr->close(fd);
}

/*
 * getArgs
 *     DESCRIPTION: No implementation
 *     INPUTS: N/A
 *     OUTPUTS: N/A
 *     RETURN VALUE: Returns 0 on success. -1 on error.
 */
int32_t getArgs(uint8_t * buf, int32_t nBytes) {
  // Access PCB->arguments
  pcb_t * currPCB = generatePCBPointer(currProcessIndex);
  // Error Checking: Valid Pointers
  if(buf == NULL) return -1;
  // Error Checking: Valid arguments
  if(currPCB->bufferArgs[0] == '\0' || nBytes <= 0) return -1;
  // Error Checking: Valid args length (Ensure length of buf < nBytes)
  if(strlen((const int8_t *)currPCB->bufferArgs) > nBytes) return -1;
  // Copy PCB->arguments to buffer
  strcpy((int8_t *)buf, (const int8_t *)currPCB->bufferArgs);
  return 0;
}

/*
 * vidMap
 *     DESCRIPTION: No implementation
 *     INPUTS: screenstart - pointer to video memory
 *     OUTPUTS: N/A
 *     RETURN VALUE: 0
 */
int32_t vidMap(uint8_t ** screenStart) {
  // 1) Error Checking
  // Invalid Pointers
  if(screenStart == NULL || screenStart == (uint8_t**)PageSize4MB) return -1;
  // 2) Map virtual address to video memory
  getNew4KBPage(VidmapStartAddress, videoMemAddr);
  // 3) *screenStart = virtual address (133MB)
  *screenStart = (uint8_t*)(VidmapStartAddress);
  // 4) Return virtual address
  return VidmapStartAddress;
}

/*
 * setHandler
 *     DESCRIPTION: No implementation
 *     INPUTS: N/A
 *     OUTPUTS: N/A
 *     RETURN VALUE: -1
 */
int32_t setHandler(int32_t sigNum, void* handlerAddress) {
  return -1;
}

/*
 * sigReturn
 *     DESCRIPTION: No implementation
 *     INPUTS: N/A
 *     OUTPUTS: N/A
 *     RETURN VALUE: -1
 */
int32_t sigReturn(void) {
  return -1;
}

/*
 * generatePCBPointer
 *     DESCRIPTION: Generate pointer to active pcb
 *     INPUTS: currProcessIndex
 *     OUTPUTS: none
 *     RETURN VALUE: pcb_t pointer
 */
pcb_t* generatePCBPointer(int currProcessIndex) {
  //8mb - 4kb*currProcessIndex
  return (pcb_t*)(eightMB - eightKB*(currProcessIndex + 1));
}


/*
 * parseCommands
 *     DESCRIPTION: Splice space-separated command for filename and arguments
 *     INPUTS: command, filename, argToPass
 *     OUTPUTS: none
 *     RETURN VALUE: If command is invalid, return -1,
 *                   Else, return 0
 */
int32_t parseCommands(const uint8_t * command, uint8_t * filename, uint8_t * argToPass) {
    int i;
    int fileNameStart = 0, fileNameEnd = 0;
    // Remove extra spaces
    while (command[fileNameStart] == ' ')
        fileNameStart++;
    fileNameEnd = fileNameStart;
    // Get filename string
    while (command[fileNameEnd] != ' ' && command[fileNameEnd] != '\0')
        fileNameEnd++;

    // // Command cannot be executed if filename length exceeds 32 chars
    // if (fileNameEnd - fileNameStart > maxFileNameSize) {
    //     printf("Command could not be executed: file name too long.");
    //     sti();
    //     return -1;
    // }
    // uint8_t filename[maxFileNameSize]; // initialize filename
    for (i = fileNameStart; i < fileNameEnd; i++) {
        filename[i - fileNameStart] = command[i];
    }
    // Null terminated string
    filename[fileNameEnd - fileNameStart] = '\0';


    fileNameEnd++;
    // Move to beginning of next word
    fileNameStart = fileNameEnd;
    // Remove extra spaces
    while (command[fileNameStart] == ' ')
        fileNameStart++;
    fileNameEnd = fileNameStart;
    // Get argument string
    while (command[fileNameEnd] != ' ' && command[fileNameEnd] != '\0')
        fileNameEnd++;
    // // Command cannot be executed
    // if (fileNameEnd - fileNameStart > maxFileNameSize) {
    //     printf("Command could not be executed: file name too long.");
    //     sti();
    //     return -1;
    // }

    // uint8_t argToPass[maxFileNameSize]; // INVALID: MUST BE STATIC NUM
    for (i = fileNameStart; i < fileNameEnd; i++) {
        argToPass[i - fileNameStart] = command[i];
    }
    // Null terminated string
    argToPass[fileNameEnd - fileNameStart] = '\0';
    return 0;
}
