#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "argparsing.h"
#include <assert.h>
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
  assert(cacheline > 0);
  return 0;
}

/* vim:set expandtab */
