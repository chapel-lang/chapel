#include <stdio.h>
#include <stdlib.h>
#include <qthread/qthread.h> // because we're build within the qthread source
#include <qthread/qloop.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

unsigned long THREADS = 1000000;

static aligned_t null_thread(void *arg)
{
    return 0;
}

static aligned_t team_thread(void *arg)
{
    qtimer_t     timer = qtimer_create();
    double       spawn;
    aligned_t   *rets;
    syncvar_t   *srets;

    rets = malloc(sizeof(aligned_t) * THREADS);

    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_empty(rets + i);
    }

    printf("Priming in team...\n");
    qtimer_start(timer);
    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_fork(null_thread, NULL, rets + i);
    }
    qtimer_stop(timer);
    spawn = qtimer_secs(timer);
    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_readFE(NULL, rets + i);
    }
    qtimer_stop(timer);
    printf("\tCold in team spawn time: %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
    spawn = qtimer_secs(timer) - spawn;
    printf("\tCold in team sync time : %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);

    printf("Timing in team forking...\n");
    qtimer_start(timer);
    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_fork(null_thread, NULL, rets + i);
    }
    qtimer_stop(timer);

    spawn = qtimer_secs(timer);

    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_readFF(NULL, rets + i);
    }
    qtimer_stop(timer);

    printf("\tWarm in team spawn time: %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
    spawn = qtimer_secs(timer) - spawn;
    printf("\tWarm in team sync time : %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
    free(rets);

    srets = calloc(THREADS, sizeof(syncvar_t));
    {
        const syncvar_t empty = { .u.s = { .data = 0, .state = 2, .lock = 0 } };
        for (unsigned long i = 0; i < THREADS; ++i) {
            srets[i] = empty;
        }
    }
    qtimer_start(timer);
    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_fork_syncvar(null_thread, NULL, srets + i);
    }
    qtimer_stop(timer);

    spawn = qtimer_secs(timer);

    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_syncvar_readFF(NULL, srets + i);
    }
    qtimer_stop(timer);

    printf("\tWarm in team syncvar_t spawn time: %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
    spawn = qtimer_secs(timer) - spawn;
    printf("\tWarm in team syncvar_t sync time : %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
    free(srets);

    qtimer_destroy(timer);
    return 0;
}

int main(void)
{
    qtimer_t     timer = qtimer_create();
    double       spawn;
    aligned_t   *rets;
    syncvar_t   *srets;
    unsigned int shepherds = 1;

    qthread_initialize();

    CHECK_VERBOSE();
    NUMARG(THREADS, "THREADS");
    shepherds = qthread_num_shepherds();
    printf("%u shepherds...\n", shepherds);
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

    rets = malloc(sizeof(aligned_t) * THREADS);

    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_empty(rets + i);
    }

    printf("Priming...\n");
    qtimer_start(timer);
    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_fork(null_thread, NULL, rets + i);
    }
    qtimer_stop(timer);
    spawn = qtimer_secs(timer);
    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_readFE(NULL, rets + i);
    }
    qtimer_stop(timer);
    printf("\tCold spawn time: %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
    spawn = qtimer_secs(timer) - spawn;
    printf("\tCold sync time : %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);

    printf("Timing forking...\n");
    qtimer_start(timer);
    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_fork(null_thread, NULL, rets + i);
    }
    qtimer_stop(timer);

    spawn = qtimer_secs(timer);

    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_readFF(NULL, rets + i);
    }
    qtimer_stop(timer);

    printf("\tWarm spawn time: %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
    spawn = qtimer_secs(timer) - spawn;
    printf("\tWarm sync time : %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
    free(rets);

    srets = calloc(THREADS, sizeof(syncvar_t));
    {
        const syncvar_t empty = { .u.s = { .data = 0, .state = 2, .lock = 0 } };
        for (unsigned long i = 0; i < THREADS; ++i) {
            srets[i] = empty;
        }
    }
    qtimer_start(timer);
    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_fork_syncvar(null_thread, NULL, srets + i);
    }
    qtimer_stop(timer);

    spawn = qtimer_secs(timer);

    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_syncvar_readFF(NULL, srets + i);
    }
    qtimer_stop(timer);

    printf("\tWarm syncvar_t spawn time: %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
    spawn = qtimer_secs(timer) - spawn;
    printf("\tWarm syncvar_t sync time : %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
    free(srets);

    rets = malloc(sizeof(aligned_t) * THREADS);

    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_empty(rets + i);
    }

    printf("Priming new team...\n");
    qtimer_start(timer);
    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_fork_new_team(null_thread, NULL, rets + i);
    }
    qtimer_stop(timer);
    spawn = qtimer_secs(timer);
    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_readFE(NULL, rets + i);
    }
    qtimer_stop(timer);
    printf("\tCold new team spawn time: %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
    spawn = qtimer_secs(timer) - spawn;
    printf("\tCold new team sync time : %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);

    printf("Timing new team forking...\n");
    qtimer_start(timer);
    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_fork_new_team(null_thread, NULL, rets + i);
    }
    qtimer_stop(timer);

    spawn = qtimer_secs(timer);

    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_readFF(NULL, rets + i);
    }
    qtimer_stop(timer);

    printf("\tWarm new team spawn time: %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
    spawn = qtimer_secs(timer) - spawn;
    printf("\tWarm new team sync time : %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
    free(rets);

    srets = calloc(THREADS, sizeof(syncvar_t));
    {
        const syncvar_t empty = { .u.s = { .data = 0, .state = 2, .lock = 0 } };
        for (unsigned long i = 0; i < THREADS; ++i) {
            srets[i] = empty;
        }
    }
    qtimer_start(timer);
    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_fork_syncvar_new_team(null_thread, NULL, srets + i);
    }
    qtimer_stop(timer);

    spawn = qtimer_secs(timer);

    for (unsigned long i = 0; i < THREADS; ++i) {
        qthread_syncvar_readFF(NULL, srets + i);
    }
    qtimer_stop(timer);

    printf("\tWarm new team syncvar_t spawn time: %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
    spawn = qtimer_secs(timer) - spawn;
    printf("\tWarm new team syncvar_t sync time : %f usecs, %f/sec\n", 1000000 * spawn / THREADS, THREADS / spawn);
    free(srets);

    qtimer_destroy(timer);

    aligned_t team_thread_ret;
    qthread_fork_new_team(team_thread, NULL, &team_thread_ret);
    qthread_readFF(NULL, &team_thread_ret);

    return 0;
}

/* vim:set expandtab */
