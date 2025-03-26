#include "argparsing.h"
#include <qthread/qthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Test that a readXX on a full var performs the read, and leaves the FEB state
// unchanged
static void testReadXXOnFull(void) {
  aligned_t x, val;

  x = 45, val = 55;
  test_check(qthread_feb_status(&x) == 1);

  iprintf("Before x=%d, val=%d, x_full=%d\n", x, val, qthread_feb_status(&x));
  qthread_readXX(&val, &x);
  iprintf("After  x=%d, val=%d, x_full=%d\n", x, val, qthread_feb_status(&x));

  test_check(qthread_feb_status(&x) == 1);
  test_check(x == 45);
  test_check(val == 45);
}

// Test that a readXX on an empty var performs the read, and leaves the FEB
// state unchanged
static void testReadXXOnEmpty(void) {
  aligned_t x, val;

  x = 45, val = 55;
  qthread_empty(&x);

  iprintf("Before x=%d, val=%d, x_full=%d\n", x, val, qthread_feb_status(&x));
  qthread_readXX(&val, &x);
  iprintf("After  x=%d, val=%d, x_full=%d\n", x, val, qthread_feb_status(&x));

  test_check(qthread_feb_status(&x) == 0);
  test_check(x == 45);
  test_check(val == 45);
}

int main(int argc, char *argv[]) {
  CHECK_VERBOSE();
  test_check(qthread_initialize() == 0);
  iprintf("%i shepherds...\n", qthread_num_shepherds());
  iprintf("  %i threads total\n", qthread_num_workers());

  testReadXXOnFull();
  testReadXXOnEmpty();

  return 0;
}

/* vim:set expandtab */
