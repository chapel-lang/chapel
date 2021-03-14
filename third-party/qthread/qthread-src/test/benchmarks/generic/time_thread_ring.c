#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include <qthread/qtimer.h>


// native qthreads version of thread-ring, based on Chapel's release version
//   https://github.com/chapel-lang/chapel/blob/master/test/release/examples/benchmarks/shootout/threadring.chpl

//static int n = 1000, ntasks = 503;
static int n = 50000000, ntasks = 503;
static aligned_t *mailbox;

static void passTokens(size_t start, size_t stop, void* arg) {
  uint64_t id = start;

  // printf("entering tokens\n");
  uint64_t numPasses = 0;
  do {
    // printf("numpasses %lld n %d\n", numPasses, n);
    qthread_readFE(&numPasses, &mailbox[id]);
    qthread_writeEF_const(&mailbox[(id+1)%ntasks], numPasses+1);
    if (numPasses == n) {
      printf("%ld\n", (long)id+1);
    }
  } while (numPasses < n);
  //  printf("exiting tokens\n");
}

static void init() {
  assert(qthread_initialize() == 0);
  printf("%i threads...\n", qthread_num_workers());

  // init array of syncvars (code grabbed from stress/feb_stream.c)
  mailbox = malloc(ntasks * sizeof(aligned_t));
  for (int i=0; i<ntasks; i++) {
    mailbox[i] = 0;
    qthread_empty(&mailbox[i]);
  }
}

int main(int argc, char **argv) {
  qtimer_t timer = qtimer_create();
  double ring_time;
  init();

  qtimer_start(timer);
  qthread_writeEF_const(&mailbox[0], 0);
  qt_loop_simple(0, ntasks, passTokens, NULL);
  qtimer_stop(timer);
  ring_time = qtimer_secs(timer);

  printf("\tThread ring time: %f usecs, %f/sec\n", 1000000 * ring_time / ntasks, ntasks / ring_time);

  return 0;
}
