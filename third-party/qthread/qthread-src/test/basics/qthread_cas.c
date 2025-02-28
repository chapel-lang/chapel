#include "argparsing.h"
#include <assert.h>
#include <qthread/qthread.h>
#include <stdatomic.h>
#include <stdio.h>

aligned_t _Atomic master = 0;

static aligned_t incr(void *arg) {
  aligned_t localmaster, addition, ret;

  ret = atomic_load_explicit(&master, memory_order_relaxed);
  do {
    localmaster = ret;
    addition = localmaster + 1;
    ret = qthread_cas((aligned_t *)&master, ret, addition);
  } while (ret != localmaster);
  return 0;
}

uint32_t four = 4;
uint64_t eight = 8;
void *ptr = NULL;

int main(int argc, char *argv[]) {
  int i;
  aligned_t rets[30];

  memset(rets, 0, sizeof(aligned_t) * 30);
  assert(qthread_initialize() == QTHREAD_SUCCESS);
  CHECK_VERBOSE();

  rets[0] = qthread_cas((aligned_t *)&master, 0, 1);
  assert(atomic_load_explicit(&master, memory_order_relaxed) == 1);
  assert(rets[0] == 0);
  atomic_store_explicit(&master, 0, memory_order_relaxed);
  for (i = 0; i < 30; i++) { qthread_fork(incr, NULL, &(rets[i])); }
  for (i = 0; i < 30; i++) { qthread_readFF(NULL, rets + i); }
  aligned_t localmaster = atomic_load_explicit(&master, memory_order_relaxed);
  if (localmaster != 30) {
    fprintf(
      stderr, "master is %lu rather than 30\n", (long unsigned)localmaster);
  }

  assert(qthread_cas32(&four, 4, 0xdeadbeef) == 4);
#if (QTHREAD_ASSEMBLY_ARCH != QTHREAD_POWERPC32)
  assert(qthread_cas64(&eight, (uint64_t)8, (uint64_t)0xdeadbeefc0d1f1edULL) ==
         8);
#endif
  assert(four == 0xdeadbeef);
#if (QTHREAD_ASSEMBLY_ARCH != QTHREAD_POWERPC32)
  assert(eight == (uint64_t)0xdeadbeefc0d1f1edULL);
#endif
  assert(qthread_cas32(&four, 0xdeadbeef, 5) == 0xdeadbeef);
#if (QTHREAD_ASSEMBLY_ARCH != QTHREAD_POWERPC32)
  assert(qthread_cas64(&eight, (uint64_t)0xdeadbeefc0d1f1edULL, (uint64_t)9) ==
         (uint64_t)0xdeadbeefc0d1f1edULL);
#endif
  assert(four == 5);
#if (QTHREAD_ASSEMBLY_ARCH != QTHREAD_POWERPC32)
  assert(eight == 9);
#endif

  assert(qthread_cas_ptr(&ptr, NULL, &i) == NULL);
  assert(ptr == &i);
  iprintf("success!\n");

  return 0;
}

/* vim:set expandtab */
