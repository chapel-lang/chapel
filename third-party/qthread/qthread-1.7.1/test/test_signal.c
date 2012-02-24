#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <signal.h>
#include <qthread/qthread.h>
#include "argparsing.h"
#include "qthread_innards.h"
#include "qt_shepherd_innards.h"

static aligned_t t  = 0;
static aligned_t t2 = 1;

static aligned_t waiter(void *arg)
{
    iprintf("waiter alive!\n");
    qthread_writeF_const(&t, 1);
    while(t == 1) {
	COMPILER_FENCE;
    }
    iprintf("waiter exiting!\n");

    return 0;
}

int main(int   argc,
         char *argv[])
{
    int ret = 0;

    ret = qthread_init(2);
    if (ret != QTHREAD_SUCCESS) {
	fprintf(stderr, "initialization error\n");
	abort();
    }

    CHECK_VERBOSE();

    iprintf("%i shepherds...\n", qthread_num_shepherds());
    iprintf("  %i threads total\n", qthread_num_workers());

    qthread_empty(&t);

    qthread_fork_to(waiter, NULL, &t2, 1);

    qthread_readFF(NULL, &t);

    iprintf("killing waiter task (on shep 1, wkr 0)\n");
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    ret = pthread_kill(qlib->shepherds[1].workers[0].worker, SIGUSR1);
#else
    ret = pthread_kill(qlib->shepherds[1].shepherd, SIGUSR1);
#endif
    if (ret != 0) {
        perror("pthread_kill");
	abort();
    }
    iprintf("sent signal\n");

    sleep(1);
    MACHINE_FENCE;
    iprintf("setting t=2\n");
    t = 2;

    qthread_readFF(NULL, &t2);
    iprintf("t2 = %lu\n", (unsigned long)t2);
    if (t2 == 0) {
        fprintf(stderr, "waiter task erroneously ran to completion!\n");
        abort();
    }

    iprintf("exiting...\n");

    return 0;
}

/* vim:set expandtab */
