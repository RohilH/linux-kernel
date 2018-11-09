#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include "types.h"
#include "multiboot.h"
#include "rtc.h"
#include "terminal.h"
#include "fileSystem.h"

#define numFiles 8
#define maxFileNameSize 32
#define VirtualStartAddress 0x08000000
#define ProgramImageAddress 0x08048000

typedef struct fileOpsTable_t {
    int32_t (*open) (const uint8_t* fileName);
    int32_t (*read) (int32_t fd, void* buf, int32_t nBytes);
    int32_t (*write) (int32_t fd, const void* buf, int32_t nBytes);
    int32_t (*close) (int32_t fd);
} fileOpsTable_t;

typedef struct fileDescriptor_t {
    fileOpsTable_t fileOpsTablePtr; // jumptable pointer to open read write close
    int32_t inodeNum;
    int32_t filePosition;
    int32_t flags;
} fileDescriptor_t;

typedef struct pcb_t {
    fileDescriptor_t fileArray[8]; // jumptable pointer to open read write close
    // struct pcb_t* parentPtr;
    uint32_t pcbESP;
    uint32_t pcbEBP;
    // struct pcb_t* childPtr;
} pcb_t;


pcb_t pcb_instance[8];
int currProcessIndex; // Initial process index set to null
pcb_t* initPCB();

int32_t halt(uint8_t status);

int32_t execute(const uint8_t * command);

int32_t read(int32_t fd, void* buf, int32_t nBytes);

int32_t write(int32_t fd, const void* buf, int32_t nBytes);

int32_t open(const uint8_t* fileName);

int32_t close(int32_t fd);

int32_t getArgs(uint8_t * buf, int32_t nBytes);

int32_t vidMap(uint8_t ** screenStart);

int32_t setHandler(int32_t sigNum, void* handlerAddress);

int32_t sigReturn(void);

pcb_t* generatePCBPointer(int currProcessIndex);



#endif
