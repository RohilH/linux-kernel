#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include "types.h"
#include "multiboot.h"
#include "rtc.h"
#include "terminal.h"
#include "fileSystem.h"

#define numFiles 8
#define maxFileNameSize 32 // 32 char
#define VirtualStartAddress 0x08000000 // 128MB
#define VidmapStartAddress  0x08400000 // 132MB

#define ProgramImageAddress 0x08048000

#define max_processes 6
#define del_CHAR 0x7f
#define e_CHAR 0x45
#define l_CHAR 0x4c
#define f_CHAR 0x46

#define eightMB 0x00800000
#define eightKB 0x2000
#define fourBytes 4
#define execStartByte 24

#define bufSize 128
/*
 * struct of function pointers used in fileDescriptor_t
 * open - function pointer to open
 * read - function pointer to read
 * write - function pointer to write
 * cloes - function pointer to write
*/
typedef struct fileOpsTable_t {
    int32_t (*open) (const uint8_t* fileName);
    int32_t (*read) (int32_t fd, void* buf, int32_t nBytes);
    int32_t (*write) (int32_t fd, const void* buf, int32_t nBytes);
    int32_t (*close) (int32_t fd);
} fileOpsTable_t;

/*
 * struct of function pointers used in pcb_t
 * fileOpsTablePtr - jumptable of function pointers
 * inodeNum - inode number of file
 * filePosition - current position in current file
 * flags - indicates used fds
*/
typedef struct fileDescriptor_t {
    fileOpsTable_t * fileOpsTablePtr; // jumptable pointer to open read write close
    int32_t inodeNum;
    int32_t filePosition;
    int32_t flags;
} fileDescriptor_t;

/*
 * struct of function pointers used in pcb_t
 * fileArray - array of fileDescriptor_t's
 * pcbESP - stack pointer of curr pcb
 * pcbEBP - base pointer of curr pcb
*/
typedef struct pcb_t {
    fileDescriptor_t fileArray[8]; // jumptable pointer to open read write close
    // struct pcb_t* parentPtr;
    uint32_t pcbESP;
    uint32_t pcbEBP;
    uint8_t bufferArgs[bufSize];
    // struct pcb_t* childPtr;
} pcb_t;


// pcb_t pcb_instance[8];
int currProcessIndex; // Initial process index set to null
// PCB initialization/setup
pcb_t* initPCB();
// System Call: halt
int32_t halt(uint8_t status);
// System Call: execute
int32_t execute(const uint8_t * command);
// System Call: read
int32_t read(int32_t fd, void* buf, int32_t nBytes);
// System Call: write
int32_t write(int32_t fd, const void* buf, int32_t nBytes);
// System Call: open
int32_t open(const uint8_t* fileName);
// System Call: close
int32_t close(int32_t fd);
// System Call: getargs
int32_t getArgs(uint8_t * buf, int32_t nBytes);
// System Call: vidmap
int32_t vidMap(uint8_t ** screenStart);
// System Call: set handler
int32_t setHandler(int32_t sigNum, void* handlerAddress);
// System Call: sigreturn
int32_t sigReturn(void);
// Get current pcb
pcb_t* generatePCBPointer(int currProcessIndex);
// Parse filename command
int32_t parseCommands(const uint8_t * command, uint8_t * filename, uint8_t * argToPass);

#endif
