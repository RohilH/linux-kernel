#include "scheduler.h"

void schedule() {
  cli();

  sti();
}

/*
Things to keep in mind:
Need to keep track of the active process for each terminal. If we do this it makes life a lot easier






*/
