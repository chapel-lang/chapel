#include "chpl-atomics.h"
#include "chpl-mem.h"

typedef struct _EndCount {
  atomic_int_least64_t runningTasks;
} EndCount;

static EndCount* constructEndCount(void) {
  EndCount *endCount = chpl_mem_alloc(sizeof(EndCount), 0, 0, 0);
  return endCount;
}

static void initEndCount(EndCount* endCount) {
  atomic_init_int_least64_t(&(endCount->runningTasks), 0);
}

static void freeEndCount(EndCount* endCount) {
  chpl_mem_free(endCount, 0, 0);
}

static void upEndCount(EndCount* endCount, int64_t value) {
  atomic_fetch_add_int_least64_t(&(endCount->runningTasks), value);
}

static void downEndCount(EndCount* endCount, int64_t value) {
  atomic_fetch_sub_int_least64_t(&(endCount->runningTasks), value);
}

static void waitEndCount(EndCount* endCount) {
  while (atomic_load_int_least64_t(&(endCount->runningTasks)) != 0) {
    qthread_yield();
  }
}
