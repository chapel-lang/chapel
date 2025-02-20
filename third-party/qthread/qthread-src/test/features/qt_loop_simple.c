#include "argparsing.h"
#include <qthread/qloop.h>
#include <stdio.h>
#include <stdlib.h>

static aligned_t threads = 0;
static aligned_t numincrs = 1024;

static void sum(size_t const startat, size_t const stopat, void *arg_) {
  qthread_incr(&threads, stopat - startat);
}

int main(int argc, char *argv[]) {
  test_check(qthread_initialize() == QTHREAD_SUCCESS);
  CHECK_VERBOSE();
  NUMARG(numincrs, "NUM_INCRS");
  // future_init(128);
  iprintf("%i shepherds\n", qthread_num_shepherds());
  iprintf("%i threads\n", qthread_num_workers());

  qt_loop_simple(0, numincrs, sum, NULL);

  if (threads != numincrs) {
    iprintf("threads == %lu, not %lu\n",
            (unsigned long)threads,
            (unsigned long)numincrs);
  }
  test_check(threads == numincrs);

  return 0;
}

/* vim:set expandtab */
