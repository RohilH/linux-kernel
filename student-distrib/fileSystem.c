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
    if (!(strncmp((int8_t*) fname, direntry.fileName, FILENAMESIZE))) {
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
  (void)strncpy((int8_t*)dentry->fileName, (int8_t*)direntry->fileName, FILENAMESIZE);
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

  // check if length bytes is within max inode length
  if (length > inodeBlockStart->length)
    length = inodeBlockStart->length;

  int32_t bytesRead = 0;
  int i, currByte;
  // get current data block
  int32_t dataBlockToRead = offset/INODESIZE;
  int32_t currDataBlock = inodeBlockStart->dataBlockNum[dataBlockToRead];
  uint8_t * dataBlockAdr = (uint8_t*)(bootBlockStart + 1 + numInodes + currDataBlock);

  // loop through bytes to put into buffer
  for (i = offset; i < length; i++) {
      currByte = i;
      // check if reached end of data block
      if (currByte == DATABLOCKSIZE) {
          dataBlockToRead++;
          currDataBlock = inodeBlockStart->dataBlockNum[dataBlockToRead];
          dataBlockAdr = (uint8_t*)(bootBlockStart + 1 + numInodes + currDataBlock);
      }
      // put bytes into buffer
      currByte = i % DATABLOCKSIZE;
      buf[i - offset] = dataBlockAdr[currByte];
      bytesRead++;
  }
  return bytesRead;
}

dentry_t testD;
int32_t file_read (int32_t fd, void* buf, int32_t nBytes) {
  int bytesRead;
  uint8_t* buffer = (uint8_t*) buf;
  bytesRead = read_data(testD.inodeNum, 0, buffer, nBytes);
  printf("Bytes read: %d\n", bytesRead);
  return bytesRead;
}

int32_t file_write (int32_t fd, const void* buf, int32_t nBytes) {
  return -1;
}

int32_t file_open (const uint8_t* fileName) {
    int i;
    i = read_dentry_by_name(fileName, &testD);
    if (i == -1) {
        printf("No file by that name");
        return -1;
    }
    return 0;
}

int32_t file_close (int32_t fd) {
  return 0;
}

int32_t dir_read (int32_t fd, void* buf, int32_t nBytes) {
  int numOfDirectories = bootBlockStart->dirCount;
  int i, j;
  int8_t filename[33];
  for (i = 0; i < numOfDirectories; i++) {
    dentry_t direntry;

    read_dentry_by_index (i, &direntry);
    for (j = 0; j < 32; j++)
      filename[j] = direntry.fileName[j];
    filename[32] = '\0';
    inode_t* inodeBlockStart = (inode_t*)(bootBlockStart + 1 + direntry.inodeNum);
    // get the correct dentry for the given index
    printf("File Name:  %s, File Type: %d, Bytes Read: %d\n", filename, direntry.fileType, inodeBlockStart->length);

    // (void)strncpy(filenames[i], (int8_t*)direntry->fileName, 32);
    // filenames[i] = currFile;
  }
  // return 0;

  // copy the file name, file type, and inode number to the given dentry
  // (void)strncpy((int8_t*)dentry->fileName, (int8_t*)direntry->fileName, FILENAMESIZE);
  // dentry->fileType = direntry->fileType;
  // dentry->inodeNum = direntry->inodeNum;

  // return 0;
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
