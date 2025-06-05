#include "argparsing.h"
#include <qthread/cacheline.h>
#include <qthread/qthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int cacheline = 0;

  qthread_initialize();
  cacheline = qthread_cacheline();
  CHECK_VERBOSE();
  iprintf("%i bytes\n", cacheline);
  test_check(cacheline > 0);
  return 0;
}

/* vim:set expandtab */
