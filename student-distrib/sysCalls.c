#include "sysCalls.h"
#include "lib.h"
#include "fileSystem.h"

int32_t halt(uint8_t status) {
  return 0;
}

int32_t execute(const uint8_t * command) {
  return 0;
}

int32_t read(int32_t fd, void* buf, int32_t nBytes) {
  return 0;
}

int32_t write(int32_t fd, const void* buf, int32_t nBytes) {
  return 0;
}

int32_t open(const uint8_t* fileName) {
    dentry_t* dentry;
    if (read_dentry_by_name(fileName, dentry) == -1) return -1;

    return 0;
}

int32_t close(int32_t fd) {
  return 0;
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
