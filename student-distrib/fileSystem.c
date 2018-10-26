#include "fileSystem.h"
#include "lib.h"

bootBlock_t* bootBlockStart;

void fsInit (uint32_t startAddr) {
  // get the starting address of the boot block
  bootBlockStart = (bootBlock_t*)startAddr;
}

int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry) {
  // loops through all the dentries in the boot block
  uint32_t i;
  for (i = 0; i < bootBlockStart->dirCount; i++) {
    // get the correct dentry for the respective index
    dentry_t direntry = bootBlockStart->direntries[i];
    // check if the file name given matches
    if (!(strncmp((int8_t*) fname, direntry.fileName, 32))) {
      // populate the dentry parameter with file name, file type, and inode number
      return read_dentry_by_index(i, dentry);
    }
  }

  return -1;
}

int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry) {
  // check if given index is with the valid range
  if (index < 0 || index > bootBlockStart->dirCount - 1) return -1;

  // get the correct dentry for the given index
  dentry_t* direntry = &(bootBlockStart->direntries[index]);

  // copy the file name, file type, and inode number to the given dentry
  (void)strncpy((int8_t*)dentry->fileName, (int8_t*)direntry->fileName, 32);
  dentry->fileType = direntry->fileType;
  dentry->inodeNum = direntry->inodeNum;

  return 0;
}

int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length) {
  // check if given inode, offset, and length are within the valid range
  int32_t numInodes = bootBlockStart->inodeCount;
  if (inode < 0 || inode >= numInodes) return -1;
  if (offset < 0 || length < 0 || (length-offset) < 0) return -1;

  // get the starting address of the inode block and number of data blocks
  inode_t* inodeBlockStart = (inode_t*)(bootBlockStart + 1 + inode);

  if (length > inodeBlockStart->length)
    length = inodeBlockStart->length;

  int32_t bytesRead = 0;

  int i, currByte;
  int32_t dataBlockToRead = offset/4096;
  int32_t currDataBlock = inodeBlockStart->dataBlockNum[dataBlockToRead];
  uint8_t * dataBlockAdr = (uint8_t*)(bootBlockStart + 1 + numInodes + currDataBlock);

  for (i = offset; i < length; i++) {
      currByte = i;
      if (currByte == dataBlockSize) {
          dataBlockToRead++;
          currDataBlock = inodeBlockStart->dataBlockNum[dataBlockToRead];
          dataBlockAdr = (uint8_t*)(bootBlockStart + 1 + numInodes + currDataBlock);
      }
      currByte = i % dataBlockSize;
      buf[i - offset] = dataBlockAdr[currByte];
      bytesRead++;
  }

  return bytesRead;
}

int32_t file_read (int32_t fd, void* buf, int32_t nBytes) {
  // call read_data
  return 0;
}

int32_t file_write (int32_t fd, const void* buf, int32_t nBytes) {
  return -1;
}

int32_t file_open (const uint8_t* fileName) {
  return 0;
}

int32_t file_close (int32_t fd) {
  return 0;
}

int32_t dir_read (int32_t fd, void* buf, int32_t nBytes) {
  // read filenames
  return 0;
}

int32_t dir_write (int32_t fd, const void* buf, int32_t nBytes) {
  return -1;
}

int32_t dir_open (const uint8_t* fileName) {
  return 0;
}

int32_t dir_close (int32_t fd) {
  return 0;
}
