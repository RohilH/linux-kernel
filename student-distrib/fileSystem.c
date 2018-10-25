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
  for (i=0; i<bootBlockStart->dirCount; i++) {
    // get the correct dentry for the respective index
    dentry_t direntry = bootBlockStart->direntries[i];
    // check if the file name given matches
    if (!(strncmp((int8_t*) fname, direntry.fileName, 32))) {
      // populate the dentry parameter with file name, file type, and inode number
      read_dentry_by_index (i, dentry);
    }
  }

  return -1;
}

int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry) {
  // check if given index is with the valid range
  if (index<0 || index>bootBlockStart->dirCount) return -1;

  // get the correct dentry for the given index
  dentry_t* direntry = &(bootBlockStart->direntries[index]);

  // copy the file name, file type, and inode number to the given dentry
  (void)strncpy((int8_t*)dentry, (int8_t*)direntry, 32);
  dentry->fileType = direntry->fileType;
  dentry->inodeNum = direntry->inodeNum;

  return 0;
}

int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length) {
  // // check if given inode, offset, and length are within the valid range
  // if (inode<0 || inode>bootBlockStart->inodeCount) return -1;
  // if (offset<0 || length<0 || (length-offset)<0) return -1;
  //
  // // get the starting address of the inode block and number of data blocks
  // inode_t* inodeBlockStart = (inode_t*)(bootBlockStart + inode + 1);
  // int32_t numDataBlocks = (inodeBlockStart->length)/4096;
  //
  // int32_t numBytesToRead = length-offset;
  // int32_t bytesRead = 0;
  //
  // while (numDataBlocks > 0) {
  //   if (bytesRead == numBytesToRead) {
  //     numDataBlocks--;
  //     bytesRead = 0;
  //   }
  //
  //   bytesRead++;
  // }

  return 0;
}
