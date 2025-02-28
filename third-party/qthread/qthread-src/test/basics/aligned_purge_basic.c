#include "argparsing.h"
#include <qthread/qthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Test that a purge writes 0 and sets the FEB state to empty
static void testPurge(void) {
  aligned_t x;

  x = 45;
  test_check(qthread_feb_status(&x) == 1);

  iprintf("Before x=%d, x_full=%d\n", x, qthread_feb_status(&x));
  qthread_purge(&x);
  iprintf("After  x=%d, x_full=%d\n", x, qthread_feb_status(&x));

  test_check(qthread_feb_status(&x) == 0);
  test_check(x == 0);

  qthread_fill(&x); // fill to destroy hash entry
}

// Test that a purge_to on a full var performs the write, and changes the FEB
// state to empty.
static void testPurgeToOnFull(void) {
  aligned_t x, val;

  x = 45, val = 55;
  test_check(qthread_feb_status(&x) == 1);

  iprintf("Before x=%d, val=%d, x_full=%d\n", x, val, qthread_feb_status(&x));
  qthread_purge_to(&x, &val);
  iprintf("After  x=%d, val=%d, x_full=%d\n", x, val, qthread_feb_status(&x));

  test_check(qthread_feb_status(&x) == 0);
  test_check(x == 55);
  test_check(val == 55);

  qthread_fill(&x); // fill to destroy hash entry
}

// Test that a purge_to on an empty var performs the write, and leaves the FEB
// state unchanged
static void testPurgeToOnEmpty(void) {
  aligned_t x, val;

  x = 45, val = 55;
  qthread_empty(&x);

  iprintf("Before x=%d, val=%d, x_full=%d\n", x, val, qthread_feb_status(&x));
  qthread_purge_to(&x, &val);
  iprintf("After  x=%d, val=%d, x_full=%d\n", x, val, qthread_feb_status(&x));

  test_check(qthread_feb_status(&x) == 0);
  test_check(x == 55);
  test_check(val == 55);

  qthread_fill(&x); // fill to destroy hash entry
}

int main(int argc, char *argv[]) {
  CHECK_VERBOSE();
  test_check(qthread_initialize() == 0);
  iprintf("%i shepherds...\n", qthread_num_shepherds());
  iprintf("  %i threads total\n", qthread_num_workers());

  testPurge();
  testPurgeToOnFull();
  testPurgeToOnEmpty();

  return 0;
}

/* vim:set expandtab */
