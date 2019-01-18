#include <stdint.h>

#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include <qthread/qtimer.h>

static aligned_t decTask(void* arg) {
  aligned_t * doneCount = (aligned_t*)arg;
  qthread_incr(doneCount, -1);

  return 0;
}
static void qtChplLikeTaskSpawn(int64_t trials, int64_t numTasks) {
  int i, j;

  for (i=0; i<trials; i++) {
    aligned_t doneCount;
    doneCount = 0;
    qthread_incr(&doneCount, numTasks);
    for (j=0; j<numTasks; j++) {
      qthread_fork(decTask, &(doneCount), NULL);
    }
    while (qthread_incr(&doneCount, 0)) {
      qthread_yield();
    }
  }
}

static void emptyFunction(size_t start, size_t stop, void* arg) { }
static void qtLoopTaskSpawn(int64_t trials, int64_t numTasks) {
  int i;
  for (i=0; i<trials; i++) {
    qt_loop_dc(0, numTasks, emptyFunction, NULL);
  }
}

int main() {
  qthread_initialize();
  int64_t numTasks = qthread_num_workers();
  int64_t numTrials = 500000;

  qtimer_t t = qtimer_create();
  qtimer_start(t);
  {
    qtLoopTaskSpawn    (numTrials, numTasks);
    //qtChplLikeTaskSpawn(numTrials, numTasks);
  }
  qtimer_stop(t);
  printf("Elapsed time for %lld workers: %f\n", numTasks, qtimer_secs(t));
}
