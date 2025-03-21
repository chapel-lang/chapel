#include "argparsing.h"
#include <qthread/qthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static aligned_t greeter(void *arg) {
  iprintf("Hello World! My input argument was %lu\n",
          (unsigned long)(uintptr_t)arg);

  return 0;
}

int main(int argc, char *argv[]) {
  aligned_t return_value = 0;
  int status;

  CHECK_VERBOSE(); // part of the testing harness; toggles iprintf() output

  status = qthread_initialize();
  test_check(status == QTHREAD_SUCCESS);

  iprintf("%i shepherds...\n", qthread_num_shepherds());
  iprintf("  %i threads total\n", qthread_num_workers());

  status = qthread_fork(greeter, NULL, &return_value);
  test_check(status == QTHREAD_SUCCESS);

  int ret = qthread_readFF(NULL, &return_value);
  test_check(ret == QTHREAD_SUCCESS);

  iprintf("greeter returned %lu\n", (unsigned long)return_value);

  return EXIT_SUCCESS;
}

/* vim:set expandtab */
