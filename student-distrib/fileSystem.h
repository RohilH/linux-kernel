#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

#include "types.h"

#define inodeSize 4096
#define bootBlockSize 4096
#define dataBlockSize 4096

typedef struct dentry {
    int8_t fileName[32];
    int32_t fileType;
    int32_t inodeNum;
    int8_t reserved[24];
} __attribute__((packed)) dentry_t;

typedef struct bootBlock {
    int32_t dirCount;
    int32_t inodeCount;
    int32_t dataCount;
    int8_t reserved[52];
    dentry_t direntries[63];
} __attribute__((packed)) bootBlock_t;

typedef struct inode {
    int32_t length;
    int32_t dataBlockNum[1023];
} inode_t;

extern void fsInit (uint32_t startAddr);
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);
void copyDentry(dentry_t * dest, dentry_t * src);
#endif
