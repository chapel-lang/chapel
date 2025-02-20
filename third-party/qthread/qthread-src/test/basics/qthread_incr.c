#include "argparsing.h"
#include <qthread/qthread.h>
#include <stdio.h>

aligned_t master = 0;

static aligned_t incr(void *arg) {
  qthread_incr(&master, 1);
  return 0;
}

static aligned_t incr5(void *arg) {
  qthread_incr(&master, 5);
  return 0;
}

int main(int argc, char *argv[]) {
  int i;
  aligned_t rets[30];

  test_check(qthread_initialize() == QTHREAD_SUCCESS);
  CHECK_VERBOSE();
  iprintf("%i shepherds\n", qthread_num_shepherds());

  rets[0] = qthread_incr(&master, 1);
  test_check(master == 1);
  test_check(rets[0] == 0);
  iprintf("basic increment succeeded\n");
  master = 0;
  for (i = 0; i < 30; i++) { qthread_fork(incr, NULL, &(rets[i])); }
  for (i = 0; i < 30; i++) { qthread_readFF(NULL, rets + i); }
  if (master != 30) {
    fprintf(stderr, "master is %lu rather than 30\n", (long unsigned)master);
  }
  test_check(master == 30);
  iprintf("30 concurrent threads successfully incremented by 1\n");
  master = 0;
  for (i = 0; i < 30; i++) { qthread_fork(incr5, NULL, &(rets[i])); }
  for (i = 0; i < 30; i++) { qthread_readFF(NULL, rets + i); }
  if (master != 150) {
    fprintf(stderr, "master is %lu rather than 150\n", (long unsigned)master);
  }
  test_check(master == 150);
  iprintf("30 concurrent threads successfully incremented by 5\n");

#if (QTHREAD_BITS == 64)
  master = 0xFFFFFFFF;
  qthread_incr(&master, 1);
  if (master != 0x100000000ULL) {
    fprintf(stderr,
            "master is %lx rather than 0x10000000 -- incr1 failed\n",
            (unsigned long)master);
    test_check(master == 0x100000000);
  }
  master = 0;
  qthread_incr(&master, (uint64_t)0x100000000ULL);
  if (master != (uint64_t)0x100000000ULL) {
    fprintf(stderr,
            "master is %lx rather than 0x10000000 -- incr2 failed\n",
            (unsigned long)master);
    test_check(master == 0x100000000);
  }
  iprintf("64-bit add appears to work\n");
#endif

  return 0;
}

/* vim:set expandtab */
