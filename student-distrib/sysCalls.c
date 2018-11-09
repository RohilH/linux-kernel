#include "sysCalls.h"
#include "lib.h"
#include "types.h"
#include "fileSystem.h"
#include "terminal.h"
#include "paging.h"

int32_t emptyReturn () {
    return -1;
}

fileOpsTable_t stdin = {terminal_open, terminal_read, emptyReturn, terminal_close};
fileOpsTable_t stdout = {terminal_open, emptyReturn, terminal_write, terminal_close};
fileOpsTable_t fileTable = {file_open, file_read, file_write, file_close};
fileOpsTable_t dirTable = {dir_open, dir_read, dir_write, dir_close};
fileOpsTable_t RTCTable = {rtc_open, rtc_read, rtc_write, rtc_close};
fileOpsTable_t blankTable = {emptyReturn, emptyReturn, emptyReturn, emptyReturn};


pcb_t* initPCB() {
    // if (currProcessIndex == -2)
    //     currProcessIndex = 0;
    currProcessIndex++;
    if (currProcessIndex >= 8) {
        printf("Too many processes running; cannot create new PCB.");
        currProcessIndex--;
        return NULL;
    }
    pcb_t* currPCB = generatePCBPointer(currProcessIndex); // put PCB at top of respective kernel stack

    // Setup STDIN
    fileDescriptor_t stdinFD;
    stdinFD.fileOpsTablePtr = stdin;
    currPCB->fileArray[0] = stdinFD;
    currPCB->fileArray[0].flags = 1;

    // Setup STDOUT
    fileDescriptor_t stdoutFD;
    stdoutFD.fileOpsTablePtr = stdout;
    currPCB->fileArray[1] = stdoutFD;
    currPCB->fileArray[1].flags = 1;

    // Setup Blank tables for remaining files
    fileDescriptor_t emptyFD;
    emptyFD.fileOpsTablePtr = blankTable;

    int i = 0;
    for (i = 2; i < numFiles; i++) {
        currPCB->fileArray[i] = emptyFD;
        currPCB->fileArray[i].flags = 0;
    }
    // if (currProcessIndex == 0) {
    //     currPCB->parentPtr = NULL;
    // }
    // else if (currProcessIndex < 8) {
    //     currPCB->parentPtr = generatePCBPointer(currProcessIndex - 1);
    // }
    return currPCB;
}
int32_t halt(uint8_t status) {
  //restart shell if halting last process
  if(currProcessIndex == 0) {
    //pcb_instance[currProcessIndex] = NULL;
    currProcessIndex--;
    uint8_t* shellCommand = (uint8_t*)"shell";
    execute(shellCommand);
  }
  cli();

  int i;
  uint32_t storeESP;
  uint32_t storeEBP;

  pcb_t* currPCB = generatePCBPointer(currProcessIndex);

  // Close relevant File descriptors
  for(i = 0; i < numFiles; i++) {
    currPCB->fileArray[i].fileOpsTablePtr = blankTable;
    currPCB->fileArray[i].flags = 0;
    currPCB->fileArray[i].fileOpsTablePtr.close(i);
  }
  //
  // //restore parent paging
  getNewPage(VirtualStartAddress, kernelStartAddr + PageSize4MB*((currProcessIndex - 1) + 1));
  // //Jump to execute return
  storeESP = currPCB->pcbESP;
  storeEBP = currPCB->pcbEBP;
  // //Modify TSS according to the parent
  tss.esp0 = currPCB->pcbESP;

  uint32_t castStatus = (uint32_t)status;
  currProcessIndex--;
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


  return 0; //never get to this point}
}

int32_t execute(const uint8_t * command) {
    cli();
    /* NOTE: STEP 1: Parse command for file name and argument */
    uint8_t filename[maxFileNameSize]; // initialize filename
    uint8_t argToPass[maxFileNameSize]; // INVALID: MUST BE STATIC NUM
    int ret;
    ret = parseCommands(command, filename, argToPass);
    if (ret == -1) {
        sti();
        return -1;
    }

    /* NOTE: STEP 2: Check for valid executable */
    dentry_t dentry;
    if (read_dentry_by_name(filename, &dentry) != 0) {
        sti();
        return -1;
    }
    uint8_t tempBuffer[4];
    read_data (dentry.inodeNum, 0, tempBuffer, 4);
    // 0: 0x7f; 1: 0x45; 2: 0x4c; 3: 0x46 magic numbers (makes sure file is executable)
    if (tempBuffer[0] != 0x7f || tempBuffer[1] != 0x45 || tempBuffer[2] != 0x4c || tempBuffer[3] != 0x46) {
        sti();
        return -1;
    }

    /* NOTE: STEP 3: Setup new PCB */

    pcb_t* currPCB = initPCB();
    uint32_t storeESP;
    uint32_t storeEBP;
    asm volatile ("movl %%esp, %0" : "=r" (storeESP));
    asm volatile ("movl %%ebp, %0" : "=r" (storeEBP));
    currPCB->pcbESP = storeESP;
    currPCB->pcbEBP = storeEBP;

    if (currPCB == NULL) {
        sti();
        return -1;
    }

    read_data (dentry.inodeNum, 24, tempBuffer, 4); // get bytes 24 to 27
    uint32_t entryPoint = *((uint32_t*) tempBuffer);
    /* NOTE: STEP 4: Setup paging */
    getNewPage(VirtualStartAddress, kernelStartAddr + PageSize4MB*(currProcessIndex+1));

    /* NOTE: STEP 5: Setup User level Program Loader */
    read_data (dentry.inodeNum, 0, (uint8_t*) ProgramImageAddress, PageSize4MB); // loads executable into user video mem

    /* NOTE: STEP 6: Create TSS for context switching  */

    tss.ss0 = KERNEL_DS;
    tss.esp0 = 0x800000 - 0x2000 *(currProcessIndex) - 4;

    int userStackPtr = VirtualStartAddress + PageSize4MB - 4;
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

int32_t read(int32_t fd, void* buf, int32_t nBytes) {
    // printf("Did I page fault part 2?");

    if (fd < 0 || fd > numFiles)
        return -1;
    if (buf == NULL)
        return -1;

    pcb_t* currPCB = generatePCBPointer(currProcessIndex);
    if (currPCB->fileArray[fd].flags == 0) // file not in use
        return -1;
    // printf("Did I page fault part 3?");

    return currPCB->fileArray[fd].fileOpsTablePtr.read(fd, buf, nBytes);
}

int32_t write(int32_t fd, const void* buf, int32_t nBytes) {
    if (fd < 0 || fd > numFiles)
        return -1;
    // printf("Did I page fault part 3?");

    if (buf == NULL)
        return -1;
    // printf("Did I page fault part 4?");
    pcb_t* currPCB = generatePCBPointer(currProcessIndex);

    if (currPCB->fileArray[fd].flags == 0) // file not in use
        return -1;
    // printf("Did I page fault part 5?");
    return currPCB->fileArray[fd].fileOpsTablePtr.write(fd, buf, nBytes);
}

int32_t open(const uint8_t* fileName) {
    dentry_t dentry;
    if (read_dentry_by_name(fileName, &dentry) == -1) return -1;
    int i;
    pcb_t* currPCB = generatePCBPointer(currProcessIndex);
    for (i = 2; i < numFiles; i++) {
        if ((currPCB->fileArray[i]).flags == 0) {
            currPCB->fileArray[i].flags = 1;
            currPCB->fileArray[i].filePosition = 0;
            break;
        }
    }
    if (i == 7) // if PCB is full
        return -1;

    if (dentry.fileType == 0) { // RTC
        if (rtc_open(fileName) == -1) { // failed rtc open
            return -1;
        }
        currPCB->fileArray[i].fileOpsTablePtr = RTCTable;
        currPCB->fileArray[i].inodeNum = NULL;
    }
    else if (dentry.fileType == 1) { // Directory
        if (dir_open(fileName) == -1) { // failed dir open
            return -1;
        }
        currPCB->fileArray[i].fileOpsTablePtr = dirTable;
        currPCB->fileArray[i].inodeNum = NULL;
    }
    else if (dentry.fileType == 2) { // File
        if (file_open(fileName) == -1) { // failed file open
            return -1;
        }
        currPCB->fileArray[i].fileOpsTablePtr = fileTable;
        currPCB->fileArray[i].inodeNum = dentry.inodeNum;
    }
    return i;
}

int32_t close(int32_t fd) {
    if (fd < 0 || fd > numFiles)
        return -1;
    pcb_t* currPCB = generatePCBPointer(currProcessIndex);
    if (currPCB->fileArray[fd].flags == 0) // file not in use
        return -1;

    currPCB->fileArray[fd].flags = 0; // set to in use
    return currPCB->fileArray[fd].fileOpsTablePtr.close(fd);
}

int32_t getArgs(uint8_t * buf, int32_t nBytes) {
  return 0;
}

int32_t vidMap(uint8_t ** screenStart) {
  return 0;
}

int32_t setHandler(int32_t sigNum, void* handlerAddress) {
  return 0;
}

int32_t sigReturn(void) {
  return 0;
}

pcb_t* generatePCBPointer(int currProcessIndex) {
  return (pcb_t*)(0x00800000 - 0x2000*(currProcessIndex + 1)); //8mb - 4kb*currProcessIndex
}

int32_t parseCommands(const uint8_t * command, uint8_t * filename, uint8_t * argToPass) {
    int i;
    int fileNameStart = 0, fileNameEnd = 0;
    while (command[fileNameStart] == ' ') // remove extra spaces
        fileNameStart++;
    fileNameEnd = fileNameStart;
    while (command[fileNameEnd] != ' ' && command[fileNameEnd] != '\0') // get filename string
        fileNameEnd++;

    if (fileNameEnd - fileNameStart >= maxFileNameSize) { // command cannot be executed
        printf("Command could not be executed: file name too long.");
        sti();
        return -1;
    }
    // uint8_t filename[maxFileNameSize]; // initialize filename
    for (i = fileNameStart; i < fileNameEnd; i++) {
        filename[i - fileNameStart] = command[i];
    }
    filename[fileNameEnd - fileNameStart] = '\0'; // null terminated string


    fileNameEnd++;
    fileNameStart = fileNameEnd;
    while (command[fileNameStart] == ' ') // remove extra spaces
        fileNameStart++;
    fileNameEnd = fileNameStart;
    while (command[fileNameEnd] != ' ' && command[fileNameEnd] != '\0') // get argument string
        fileNameEnd++;

    if (fileNameEnd - fileNameStart >= maxFileNameSize) { // command cannot be executed
        sti();
        return -1;
    }

    // uint8_t argToPass[maxFileNameSize]; // INVALID: MUST BE STATIC NUM
    for (i = fileNameStart; i < fileNameEnd; i++) {
        argToPass[i - fileNameStart] = command[i];
    }
    argToPass[fileNameEnd - fileNameStart] = '\0'; // null terminated string
    return 0;
}
