#include <stdint.h>

#include "qthread/qthread.h"
#include "qthread/qloop.h"

#include "endCount.h"

static aligned_t decTask(void* arg) {
  EndCount* endCount = *((EndCount**)arg);
  downEndCount(endCount, 1);
  return 0;
}

// Spawn and wait for tasks similar to how Chapel does (heap allocated end
// count, fork_copyargs per task, and an atomic task counter incremented once
// per task.)
static void qtChplLikeTaskSpawn(int64_t trials, int64_t numTasks, int64_t runSerial) {
  int i, j;

  for (i=0; i<trials; i++) {
    EndCount *endCount = constructEndCount();
    initEndCount(endCount);
    for (j=0; j<numTasks; j++) {
      upEndCount(endCount, 1);
      if (runSerial) {
        decTask(&endCount);
      } else {
        qthread_fork_copyargs(decTask, &(endCount), sizeof(EndCount), NULL);
      }
    }
    waitEndCount(endCount);
    freeEndCount(endCount);
  }
}

// Spawn and wait for tasks in a manner than an optimized chapel might (regular
// non-copy fork, avoid EndCount allocation, increment atomic once instead of
// once per task.)
static void qtChplOptTaskSpawn(int64_t trials, int64_t numTasks, int64_t runSerial) {
  int i, j;

  for (i=0; i<trials; i++) {
    EndCount endCountBase;
    EndCount *endCount = &endCountBase;
    initEndCount(endCount);
    upEndCount(endCount, numTasks);
    for (j=0; j<numTasks; j++) {
      if (runSerial) {
        decTask(&endCount);
      } else {
        qthread_fork(decTask, &(endCount), NULL);
      }
    }
    waitEndCount(endCount);
  }
}
