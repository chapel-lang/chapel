#include <stdint.h>

#include "qthread/qthread.h"
#include <qthread/qloop.h>

#include "chpl-atomics.h"

// 
// Spawn and wait for tasks similar to how Chapel does (fork per task, and
// an atomic counter for task counting.)
//
static aligned_t decTask(void* arg) {
  atomic_fetch_sub_int_least64_t((atomic_int_least64_t*)arg, 1);
  return 0;
}
static void qtChplLikeTaskSpawn(int64_t trials, int64_t numTasks) {
  int i, j;
  atomic_int_least64_t runningTasks;
  atomic_init_int_least64_t(&runningTasks, 0);

  for (i=0; i<trials; i++) {
    for (j=0; j<numTasks; j++) {
      atomic_fetch_add_int_least64_t(&runningTasks, 1);
      qthread_fork(decTask, (void*)(&runningTasks), NULL);
    }
    while (atomic_load_int_least64_t(&runningTasks)) { qthread_yield(); }
  }
}

//
// Currently unused: 
//
// Spawn with qt_loop (or consider qt_loop_dc, qt_loop_sinc, qt_loop_sv, or
// qt_loop_aligned variants, which change how task completion is tracked using
// donecount, sinc_t, syncvar_t, and aligned_t respectively. These all seem to
// be much slower than the atomic counter we use.)
//
static void emptyFunction(size_t start, size_t stop, void* arg) { }
static void qtLoopTaskSpawn(int64_t trials, int64_t numTasks) {
  int i;
  for (i=0; i<trials; i++) {
    qt_loop(0, numTasks, emptyFunction, NULL);
  }
}

