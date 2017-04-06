#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <float.h> /* for DBL_MAX, per C89 */
#include <signal.h>
#include <qthread/qthread.h>
#include <qthread/barrier.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

aligned_t     initme_idx = 1;
aligned_t    *initme     = NULL;
qt_barrier_t *wait_on_me;
int stack_size;
size_t       threads = 2;
aligned_t    *rets;

void bus_sigaction(int signal, siginfo_t *si, ucontext_t *arg)
{
    exit(0);
}

static aligned_t buserr_thread(void *arg)
{
    volatile char *eos; // end of stack

    eos = (volatile char*)((unsigned long long int)(((qthread_id() % 2) ? qthread_bos() + 256 : qthread_tos() - 256)));
    iprintf("%s: tos %p bos %p eos %p\n", __func__, qthread_tos(), qthread_bos(), eos);
    *eos = 1;

    return 0;
}

int main(int   argc,
         char *argv[])
{
    int i;
    qtimer_t     t;
    unsigned int iter, iterations = 1;
    double       total_time = 0.0;
    double       max_time   = 0.0;
    double       min_time   = DBL_MAX;
    struct sigaction sa;

    memset(&sa, 0, sizeof(sigaction));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = bus_sigaction;
    sa.sa_flags   = SA_SIGINFO;

    // TODO(npe) how does qthreads handle signals?
    sigaction(SIGBUS, &sa, NULL);

    assert(qthread_initialize() == 0);

    CHECK_VERBOSE();
    NUMARG(threads, "THREADS");
    NUMARG(iterations, "ITERATIONS");
    if (!getenv("QT_STACK_SIZE") && !getenv("QTHREAD_STACK_SIZE"))
      setenv("QT_STACK_SIZE", "4096", 0);
    if (!getenv("GUARD_PAGES"))
      setenv("GUARD_PAGES", "1", 0);

    stack_size = atoi(getenv("QT_STACK_SIZE"));
    rets = (aligned_t *)malloc(threads * sizeof(aligned_t));
    assert(rets);

    for (iter = 0; iter <= iterations; ++iter) {
        for (i = 1; i < threads; ++i) {
            qthread_spawn(buserr_thread, NULL, 0,
                          rets + i,
                          0, NULL, i, 0);
        }
    }

    // FIXME(npe) We should really catch the bus errors for the top and
    // bottom of the stack and then return via a synchronizing for
    // every exiting thread.
    for (i = 1; i < threads; ++i) {
        qthread_readFF(NULL, rets + i);
        if (initme[i] != iter + 1) {
            iprintf("initme[%i] = %i (should be %i)\n", (int)i,
                    (int)initme[i], iter + 1);
        }
        assert(initme[i] == iter + 1);
    }
    
    free(rets);

    iprintf("Failure!\n");
    exit(1);
}

/* vim:set expandtab */
