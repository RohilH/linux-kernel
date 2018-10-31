#include "sysCalls.h"
#include "lib.h"
#include "fileSystem.h"
#include "terminal.h"
int32_t emptyReturn () {
    return -1;
}

fileOpsTable_t stdin = {terminal_open, terminal_read, emptyReturn, terminal_close};
fileOpsTable_t stdout = {terminal_open, emptyReturn, terminal_write, terminal_close};
fileOpsTable_t fileTable = {file_open, file_read, file_write, file_close};
fileOpsTable_t dirTable = {dir_open, dir_read, dir_write, dir_close};
fileOpsTable_t RTCTable = {rtc_open, rtc_read, rtc_write, rtc_close};
fileOpsTable_t blankTable = {emptyReturn, emptyReturn, emptyReturn, emptyReturn};


int32_t initPCB() {
    int i = 0;
    fileDescriptor_t stdinFD;
    stdinFD.fileOpsTablePtr = stdin;
    pcb_instance.fileArray[0] = stdinFD;

    fileDescriptor_t stdoutFD;
    stdoutFD.fileOpsTablePtr = stdout;
    pcb_instance.fileArray[1] = stdoutFD;

    fileDescriptor_t emptyFD;
    emptyFD.fileOpsTablePtr = blankTable;

    for (i = 2; i < numProcesses; i++) {
        pcb_instance.fileArray[i] = emptyFD;
    }
    return 0;
}
int32_t halt(uint8_t status) {
  return 0;
}

int32_t execute(const uint8_t * command) {
    cli();

    /* NOTE: STEP 1: Parse command for file name and argument */

    int fileNameStart = 0, fileNameEnd = 0;
    while (command[fileNameStart] == ' ') // remove extra spaces
        fileNameStart++;
    fileNameEnd = fileNameStart;
    while (command[fileNameEnd] != ' ') // get filename string
        fileNameEnd++;

    if (fileNameEnd - fileNameStart >= maxFileNameSize) { // command cannot be executed
        printf("Command could not be executed: file name too long.");
        sti();
        return -1;
    }

    uint8_t filename[maxFileNameSize]; // INVALID: MUST BE STATIC NUM
    for (i = fileNameStart; i < fileNameEnd; i++) {
        filename[i - fileNameStart] = command[i];
    }
    filename[fileNameEnd - fileNameStart] = '\0'; // null terminated string

    fileNameEnd++;
    fileNameStart = fileNameEnd;
    while (command[fileNameStart] == ' ') // remove extra spaces
        fileNameStart++;
    fileNameEnd = fileNameStart;
    while (command[fileNameEnd] != ' ') // get argument string
        fileNameEnd++;

    if (fileNameEnd - fileNameStart >= maxFileNameSize) { // command cannot be executed
        printf("Command could not be executed: argument length too long.");
        sti();
        return -1;
    }

    uint8_t argToPass[maxFileSize]; // INVALID: MUST BE STATIC NUM
    for (i = fileNameStart; i < fileNameEnd; i++) {
        argToPass[i - fileNameStart] = command[i];
    }
    argToPass[fileNameEnd - fileNameStart] = '\0'; // null terminated string

    /* NOTE: STEP 2: Check for valid executable */
    dentry_t dentry;
    if (read_dentry_by_name(filename, dentry) != 0) {
        printf("Couldn't find file.");
        sti();
        return -1;
    }
    // 0: 0x7f; 1: 0x45; 2: 0x4c; 3: 0x46 magic numbers
    uint8_t tempBuffer[4];
    read_data (dentry.inodeNum, 0, tempBuffer, 4);
    if (tempBuffer[0] != 0x7f || tempBuffer[1] != 0x45 || tempBuffer[2] != 0x4c || tempBuffer[3] != 0x46) {
        printf("File is not an executable.");
        sti();
        return -1;
    }
    read_data (dentry.inodeNum, 27, tempBuffer, 4); // get bytes 24 to 27

    sti();
    return 0;
}

int32_t read(int32_t fd, void* buf, int32_t nBytes) {
    // printf("Did I page fault part 2?");

    if (fd < 0 || fd > numProcesses)
        return -1;
    if (buf == NULL)
        return -1;

    if (pcb_instance.fileArray[fd].flags == 0) // file not in use
        return -1;
    // printf("Did I page fault part 3?");

    return pcb_instance.fileArray[fd].fileOpsTablePtr.read(fd, buf, nBytes);
}

int32_t write(int32_t fd, const void* buf, int32_t nBytes) {
    if (fd < 0 || fd > numProcesses)
        return -1;
    // printf("Did I page fault part 3?");

    if (buf == NULL)
        return -1;
    // printf("Did I page fault part 4?");

    if (pcb_instance.fileArray[fd].flags == 0) // file not in use
        return -1;
    // printf("Did I page fault part 5?");
    return pcb_instance.fileArray[fd].fileOpsTablePtr.write(fd, buf, nBytes);
}

int32_t open(const uint8_t* fileName) {
    dentry_t dentry;
    if (read_dentry_by_name(fileName, &dentry) == -1) return -1;
    int i;
    for (i = 2; i < numProcesses; i++) {
        if ((pcb_instance.fileArray[i]).flags == 0) {
            pcb_instance.fileArray[i].flags = 1;
            pcb_instance.fileArray[i].filePosition = 0;
            break;
        }
    }
    if (i == 7) // if PCB is full
        return -1;

    if (dentry.fileType == 0) { // RTC
        if (rtc_open(fileName) == -1) { // failed rtc open
            return -1;
        }
        pcb_instance.fileArray[i].fileOpsTablePtr = RTCTable;
        pcb_instance.fileArray[i].inodeNum = NULL;
    }
    else if (dentry.fileType == 1) { // Directory
        if (dir_open(fileName) == -1) { // failed rtc open
            return -1;
        }
        pcb_instance.fileArray[i].fileOpsTablePtr = dirTable;
        pcb_instance.fileArray[i].inodeNum = NULL;
    }
    else if (dentry.fileType == 2) { // File
        if (file_open(fileName) == -1) { // failed rtc open
            return -1;
        }
        pcb_instance.fileArray[i].fileOpsTablePtr = fileTable;
        pcb_instance.fileArray[i].inodeNum = dentry.inodeNum;
    }
    return i;
}

int32_t close(int32_t fd) {
    if (fd < 0 || fd > numProcesses)
        return -1;

    if (pcb_instance.fileArray[fd].flags == 0) // file not in use
        return -1;

    pcb_instance.fileArray[fd].flags = 1; // set to in use
    return pcb_instance.fileArray[fd].fileOpsTablePtr.close(fd);
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
//
// pcb_t* getPCBloc() {
//   pcb_t* pcb_instance;
//   asm volatile(
//     "andl  %%esp, %0;"
//     :                       // no outputs
//     : "r" (pcb_instance)
//     : "eax"
//   )
// }
