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
  // //printf("Gets here 1?");
  for (i = 0; i < bootBlockStart->dirCount; i++) {
    // //printf("Gets here 2?");

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
  // //printf("Gets to index?");

  // get the correct dentry for the given index
  dentry_t* direntry = &(bootBlockStart->direntries[index]);
  //printf("Initializes direntry?");

  // copy the file name, file type, and inode number to the given dentry
  (void)strncpy((int8_t*)dentry->fileName, (int8_t*)direntry->fileName, 32);
  //printf("Copies Correct!");

  dentry->fileType = direntry->fileType;
  dentry->inodeNum = direntry->inodeNum;

  return 0;
}

int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length) {
  // check if given inode, offset, and length are within the valid range
  int32_t numInodes = bootBlockStart->inodeCount;
  // printf("How many nodes yo %d\n!", numInodes);
  // printf("Which inode yo %d\n!", inode);
  if (inode < 0 || inode >= numInodes) return -1;
  if (offset < 0 || length < 0 || (length-offset) < 0) return -1;
  // get the starting address of the inode block and number of data blocks
  // printf("bootBlock Address %d\n", (int) bootBlockStart);
  //printf("bootBlock Address + one %d\n", (int) (bootBlockStart + 1));
  inode_t* inodeBlockStart = (inode_t*)(bootBlockStart + 1 + inode);
  // printf("inode Address %d\n", (int) inodeBlockStart);
  //printf("Good shit 2!");
  if (length > inodeBlockStart->length)
    length = inodeBlockStart->length;

  //printf("Good shit 2.5!");


  int32_t bytesRead = 0;

  int i, currByte;
  int32_t dataBlockToRead = offset/4096;
  int32_t currDataBlock = inodeBlockStart->dataBlockNum[dataBlockToRead];
  uint8_t * dataBlockAdr = (uint8_t*)(bootBlockStart + 1 + numInodes + currDataBlock);
  // //printf("Good shit 3!");
  // printf("dataBlock Address %d\n", (int) dataBlockAdr);

  for (i = offset; i < length; i++) {
      // currDataBlock = inodeBlockStart->dataBlockNum[dataBlockToRead];
      // dataBlockAdr = (uint8_t*)(bootBlockStart + bootBlockSize + inodeSize*numInodes + dataBlockSize*currDataBlock);

      currByte = i;
      if (currByte == dataBlockSize) {
          dataBlockToRead++;
          currDataBlock = inodeBlockStart->dataBlockNum[dataBlockToRead];
          dataBlockAdr = (uint8_t*)(bootBlockStart + 1 + numInodes + currDataBlock);
      }
      currByte = i % dataBlockSize;
      // printf("Char set: %c\n", dataBlockAdr[currByte]);
      buf[i - offset] = dataBlockAdr[currByte];
      bytesRead++;

  }
  // printf("Bytes Read: %d\n", bytesRead);
  //printf("Good shit 4! %d\n", bytesRead);

  return bytesRead;
}

int32_t file_read (int32_t fd, void* buf, int32_t nBytes) {

}

int32_t file_write (int32_t fd, const void* buf, int32_t nBytes) {

}

int32_t file_open (const uint8_t* fileName) {

}

int32_t file_close (int32_t fd) {

}

int32_t dir_read (int32_t fd, void* buf, int32_t nBytes) {

}

int32_t dir_write (int32_t fd, const void* buf, int32_t nBytes) {

}

int32_t dir_open (const uint8_t* fileName) {

}

int32_t dir_close (int32_t fd) {

}
