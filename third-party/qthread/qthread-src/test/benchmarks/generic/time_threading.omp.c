#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <qthread/qthread.h> // because we're build within the qthread source
#include <qthread/qloop.h>
#include <qthread/qtimer.h>
#include <omp.h>
#include "argparsing.h"

unsigned long THREADS = 1000000;
aligned_t count = 0;

static aligned_t null_thread(void *arg)
{
    if (arg) {
	pthread_mutex_unlock((pthread_mutex_t*)arg);
    }
    return 0;
}

double output[10];

int main(void)
{
    qtimer_t     timer = qtimer_create();
    double       spawn;
    pthread_mutex_t   *rets;
    unsigned int shepherds = 1;

    CHECK_VERBOSE();
    NUMARG(THREADS, "THREADS");
    /*{
     *  const size_t bytes =
     *      (THREADS * sizeof(ucontext_t) +
     *       THREADS * (sizeof(unsigned int) + sizeof(int) +
     *                  sizeof(unsigned char) +
     *                  sizeof(void *) * 11)) / shepherds;
     *  const size_t kbytes = bytes / 1024;
     *  const size_t mbytes = kbytes / 1024;
     *  printf("With %u shepherd%s, qthread_fork() will need to be able to\n"
     *         "allocate %lu threads all at the same time. This will require\n"
     *         "at least %lu %s on this machine, and may affect spawn time.\n",
     *         shepherds, (shepherds > 1) ? "s" : "", THREADS / shepherds,
     *         (mbytes > 0) ? mbytes : ((kbytes > 0) ? kbytes : bytes),
     *         (mbytes > 0) ? "MB" : ((kbytes > 0) ? "kB" : "bytes"));
     * }*/

    rets = malloc(sizeof(pthread_mutex_t) * THREADS);
    for (int i=0; i<THREADS; i++) {
	pthread_mutex_init(&rets[i], NULL);
	pthread_mutex_lock(&rets[i]);
    }

#pragma omp parallel
#pragma omp single
    {
	shepherds = omp_get_num_threads();
	iprintf("%u threads...\n", shepherds);

	iprintf("Priming...\n");

	qtimer_start(timer);
	for (unsigned long i = 0; i < THREADS; ++i) {
#pragma omp task untied
	    null_thread(NULL);
	}
	qtimer_stop(timer);
	spawn = qtimer_secs(timer);
#pragma omp taskwait
	qtimer_stop(timer);
	output[0] = 1000000 * spawn / THREADS;
	iprintf("\tCold spawn time: %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
	spawn = qtimer_secs(timer) - spawn;
	iprintf("\tCold sync time : %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
	output[1] = 1000000 * spawn / THREADS;

	iprintf("Timing forking...\n");
	qtimer_start(timer);
	for (unsigned long i = 0; i < THREADS; ++i) {
#pragma omp task untied
	    null_thread(NULL);
	}
	qtimer_stop(timer);

	spawn = qtimer_secs(timer);

#pragma omp taskwait
	qtimer_stop(timer);

	iprintf("\tWarm spawn time: %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
	output[2] = 1000000 * spawn / THREADS;
	spawn = qtimer_secs(timer) - spawn;
	iprintf("\tWarm sync time : %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
	output[3] = 1000000 * spawn / THREADS;

	qtimer_start(timer);
#pragma omp parallel for
	for (unsigned long i = 0; i < THREADS; ++i) {
#pragma omp task untied
	    null_thread(NULL);
	}
	qtimer_stop(timer);

	spawn = qtimer_secs(timer);

#pragma omp taskwait
	qtimer_stop(timer);

	iprintf("\tParallel spawn time: %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
	output[4] = 1000000 * spawn / THREADS;
	spawn = qtimer_secs(timer) - spawn;
	iprintf("\tParallel sync time : %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
	output[5] = 1000000 * spawn / THREADS;

	qtimer_start(timer);
	for (unsigned long i = 0; i < THREADS; ++i) {
#pragma omp task untied
	    null_thread(rets + i);
	}
	qtimer_stop(timer);

	spawn = qtimer_secs(timer);

	for (unsigned long i = 0; i < THREADS; ++i) {
	    pthread_mutex_lock(rets+i);
	}
	qtimer_stop(timer);

	iprintf("\tWarm mutex spawn time: %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
	output[6] = 1000000 * spawn / THREADS;
	spawn = qtimer_secs(timer) - spawn;
	iprintf("\tWarm mutex sync time : %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
	output[7] = 1000000 * spawn / THREADS;

	qtimer_start(timer);
#pragma omp parallel for
	for (unsigned long i = 0; i < THREADS; ++i) {
#pragma omp task untied
	    null_thread(rets + i);
	}
	qtimer_stop(timer);

	spawn = qtimer_secs(timer);

#pragma omp parallel for
	for (unsigned long i = 0; i < THREADS; ++i) {
	    pthread_mutex_lock(rets+i);
	}
	qtimer_stop(timer);

	iprintf("\tParallel mutex spawn time: %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
	output[8] = 1000000 * spawn / THREADS;
	spawn = qtimer_secs(timer) - spawn;
	iprintf("\tParallel mutex sync time : %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
	output[9] = 1000000 * spawn / THREADS;
    }

    for (unsigned long i = 0; i<THREADS; ++i) {
	pthread_mutex_destroy(rets+i);
    }
    free(rets);

    qtimer_destroy(timer);

    printf("%u %lu ", shepherds, (unsigned long)THREADS);
    for (unsigned long i = 0; i < 10; ++i) {
	printf("%f ", output[i]);
    }
    printf("\n");

    return 0;
}

/* vim:set expandtab */
