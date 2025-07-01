#include "argparsing.h"
#include <qthread/io.h>
#include <qthread/qthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int foo = 0;
static int initialized = 0;

static aligned_t user_func(void *arg) {
  iprintf("\tin user_func function\n");
  qt_begin_blocking_action();
  iprintf("\t\tinside blocking action\n");
  foo = 1;
  atomic_thread_fence(memory_order_acq_rel);
  iprintf("\t\tshep=%i\n", (signed)qthread_shep());
  test_check(qthread_shep() ==
             NO_SHEPHERD); // because we're in a blocking action
  if (initialized) {
    iprintf("\t\tid=%i\n", (signed)qthread_id());
    test_check(qthread_id() != (unsigned int)-1);
  }
  qt_end_blocking_action();
  iprintf("\texiting user_func function\n");

  return 0;
}

int main(int argc, char *argv[]) {
  aligned_t t;

  CHECK_VERBOSE();

  iprintf("Checking uninitialized external calls to qthread_shep()... %i "
          "(should be %i)\n",
          (int)qthread_shep(),
          (int)NO_SHEPHERD);
  test_check(qthread_shep() == NO_SHEPHERD);
  iprintf("Calling user_func outside of qthread context\n");
  user_func(NULL);
  test_check(foo == 1);
  foo = 0;

  test_check(qthread_initialize() == 0);
  initialized = 1;

  test_check(qthread_shep() != NO_SHEPHERD);
  iprintf("Calling user_func inside of qthread context (shep = %i)\n",
          (int)qthread_shep());
  user_func(NULL);
  iprintf("Back in normal world (shep = %i)\n", (int)qthread_shep());
  test_check(qthread_shep() != NO_SHEPHERD);
  test_check(foo == 1);
  foo = 0;

  iprintf("Spawning user_func task\n");
  qthread_fork(user_func, NULL, &t);
  qthread_readFF(NULL, &t);
  atomic_thread_fence(memory_order_acq_rel);
  test_check(foo == 1);

  return (foo == 1) ? 0 : EXIT_FAILURE;
}

/* vim:set expandtab */
