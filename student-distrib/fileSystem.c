#include "fileSystem.h"
#include "lib.h"

bootBlock_t* bootBlockStart;


/*
 * fsInit
 *     DESCRIPTION: Initializes file system.
 *     INPUTS: startAddr (starting address of boot block)
 *     OUTPUTS: none
 *     RETURN VALUE: none
 */
void fsInit (uint32_t startAddr) {
  // get the starting address of the boot block
  bootBlockStart = (bootBlock_t*)startAddr;
}

/*
 * read_dentry_by_name
 *     DESCRIPTION: gets dentry for given file name and stores in dentry.
 *     INPUTS: fname (file name), dentry (dentry that holds information of file)
 *     OUTPUTS: none
 *     RETURN VALUE: 0 if found, -1 if not found
 */
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

/*
 * read_dentry_by_index
 *     DESCRIPTION: gets dentry for given file index in bootblock and stores in dentry.
 *     INPUTS: index (index of file dentry), dentry (dentry that holds information of file)
 *     OUTPUTS: none
 *     RETURN VALUE: 0 if found, -1 if invalid index
 */
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

/*
 * read_data
 *     DESCRIPTION: reads data of file and stores in buf.
 *     INPUTS: inode (inode for corresponding file), offset (starting point to read from),
 *             length (ending point to read from)
 *     OUTPUTS: none
 *     RETURN VALUE: number of bytes read, -1 if invalid input
 */
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

/*
 * file_read
 *     DESCRIPTION: reads file given file descriptor and stores in buf (currently doesn't use fd).
 *     INPUTS: fd (file descriptor), buf (memory to put file info into), nBytes (number of bytes to read)
 *     OUTPUTS: none
 *     RETURN VALUE: number of bytes read
 */
dentry_t testD;
int32_t file_read (int32_t fd, void* buf, int32_t nBytes) {
  int bytesRead;
  uint8_t* buffer = (uint8_t*) buf;

  // read file data and store in buf
  bytesRead = read_data(testD.inodeNum, 0, buffer, nBytes);
  printf("Bytes read: %d\n", bytesRead);
  return bytesRead;
}

/*
 * file_write
 *     DESCRIPTION: returns -1
 *     INPUTS: fd (file descriptor), buf (memory to put file info into), nBytes (number of bytes to read)
 *     OUTPUTS: none
 *     RETURN VALUE: -1
 */
int32_t file_write (int32_t fd, const void* buf, int32_t nBytes) {
  return -1;
}

/*
 * file_open
 *     DESCRIPTION: opens file to be used by file_read
 *     INPUTS: filename
 *     OUTPUTS: none
 *     RETURN VALUE: 0 if file found, -1 otherwise
 */
int32_t file_open (const uint8_t* fileName) {
    int i;
    // opens file and stores dentry in global var (for now; will change for syscalls)
    i = read_dentry_by_name(fileName, &testD);
    if (i == -1) {
        printf("No file by that name");
        return -1;
    }
    return 0;
}

/*
 * file_close
 *     DESCRIPTION: returns 0
 *     INPUTS: fd (file descriptor)
 *     OUTPUTS: none
 *     RETURN VALUE: 0
 */
int32_t file_close (int32_t fd) {
  return 0;
}


/*
 * dir_read
 *     DESCRIPTION: reads directory and prints to screen
 *     INPUTS: fd (file descriptor), buf (unused), nBytes (number of bytes to read)
 *     OUTPUTS: prints file info of directory to screen
 *     RETURN VALUE: 0
 */
int32_t dir_read (int32_t fd, void* buf, int32_t nBytes) {
  int numOfDirectories = bootBlockStart->dirCount;
  int i, j;
  int8_t filename[33]; // file variable to deal w large file names
  // loops through files in directory and prints values
  for (i = 0; i < numOfDirectories; i++) {
    dentry_t direntry;

    read_dentry_by_index (i, &direntry);

    for (j = 0; j < 32; j++)
      filename[j] = direntry.fileName[j];
    filename[32] = '\0';
    inode_t* inodeBlockStart = (inode_t*)(bootBlockStart + 1 + direntry.inodeNum);
    // Prints file info
    printf("File Name:  %s, File Type: %d, Bytes Read: %d\n", filename, direntry.fileType, inodeBlockStart->length);

  }
  return 0;
}


/*
 * dir_write
 *     DESCRIPTION: returns -1
 *     INPUTS: fd (file descriptor), buf (memory to put file info into), nBytes (number of bytes to read)
 *     OUTPUTS: none
 *     RETURN VALUE: -1
 */
int32_t dir_write (int32_t fd, const void* buf, int32_t nBytes) {
  return -1;
}

/*
 * dir_open
 *     DESCRIPTION: returns 0
 *     INPUTS: filename
 *     OUTPUTS: none
 *     RETURN VALUE: 0
 */
int32_t dir_open (const uint8_t* fileName) {
  return 0;
}

/*
 * dir_close
 *     DESCRIPTION: returns 0
 *     INPUTS: fd (file descriptor)
 *     OUTPUTS: none
 *     RETURN VALUE: 0
 */
int32_t dir_close (int32_t fd) {
  return 0;
}
