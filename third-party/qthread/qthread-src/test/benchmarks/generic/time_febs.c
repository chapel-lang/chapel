#include <stdio.h>                     /* for printf() */
#include <stdlib.h>                    /* for strtol() */
#include <assert.h>                    /* for assert() */
#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

size_t     TEST_SELECTION = 0xffffffff;
size_t     ITERATIONS     = 100000;
size_t     MAXPARALLELISM = 256;
aligned_t  incrementme    = 0;
aligned_t *increments     = NULL;

static void balanced_readFF(const size_t startat,
                            const size_t stopat,
                            void        *arg)
{                                      /*{{{ */
    size_t i;

    for (i = startat; i < stopat; i++) {
        qthread_readFF(NULL, (aligned_t *)arg);
    }
}                                      /*}}} */

static void balanced_syncvar_readFF(const size_t startat,
                                    const size_t stopat,
                                    void        *arg)
{                                      /*{{{ */
    size_t i;

    for (i = startat; i < stopat; i++) {
        qthread_syncvar_readFF(NULL, (syncvar_t *)arg);
    }
}                                      /*}}} */

static void balanced_falseshare_syncreadFF(const size_t startat,
                                           const size_t stopat,
                                           void        *arg)
{                                      /*{{{ */
    size_t                i;
    qthread_shepherd_id_t shep  = qthread_shep();
    syncvar_t            *myloc = ((syncvar_t *)arg) + shep;

    for (i = startat; i < stopat; i++) {
        qthread_syncvar_readFF(NULL, myloc);
    }
}                                      /*}}} */

static void balanced_falseshare_readFF(const size_t startat,
                                       const size_t stopat,
                                       void        *arg)
{                                      /*{{{ */
    size_t                i;
    qthread_shepherd_id_t shep  = qthread_shep();
    aligned_t            *myloc = ((aligned_t *)arg) + shep;

    for (i = startat; i < stopat; i++) {
        qthread_readFF(NULL, myloc);
    }
}                                      /*}}} */

static void balanced_noncomp_syncreadFF(const size_t startat,
                                        const size_t stopat,
                                        void        *arg)
{                                      /*{{{ */
    size_t    i;
    syncvar_t myinc = SYNCVAR_STATIC_INITIALIZER;

    for (i = startat; i < stopat; i++) {
        qthread_syncvar_readFF(NULL, &myinc);
    }
}                                      /*}}} */

static void balanced_noncomp_readFF(const size_t startat,
                                    const size_t stopat,
                                    void        *arg)
{                                      /*{{{ */
    size_t    i;
    aligned_t myinc = 0;

    for (i = startat; i < stopat; i++) {
        qthread_readFF(NULL, &myinc);
    }
}                                      /*}}} */

static aligned_t justreturn(void *arg)
{   /*{{{*/
    return 7;
} /*}}}*/

static char *human_readable_rate(double rate)
{   /*{{{*/
    static char  readable_string[100] = { 0 };
    const double GB                   = 1024 * 1024 * 1024;
    const double MB                   = 1024 * 1024;
    const double kB                   = 1024;

    if (rate > GB) {
        snprintf(readable_string, 100, "(%.1f GB/s)", rate / GB);
    } else if (rate > MB) {
        snprintf(readable_string, 100, "(%.1f MB/s)", rate / MB);
    } else if (rate > kB) {
        snprintf(readable_string, 100, "(%.1f kB/s)", rate / kB);
    } else {
        memset(readable_string, 0, 100 * sizeof(char));
    }
    return readable_string;
} /*}}}*/

int main(int   argc,
         char *argv[])
{
    qtimer_t     timer = qtimer_create();
    double       rate;
    aligned_t   *rets;
    unsigned int workers = 1;

    /* setup */
    assert(qthread_initialize() == QTHREAD_SUCCESS);

    CHECK_VERBOSE();
    NUMARG(ITERATIONS, "ITERATIONS");
    NUMARG(MAXPARALLELISM, "MAXPARALLELISM");
    NUMARG(TEST_SELECTION, "TEST_SELECTION");
    workers = qthread_num_workers();
    printf("%u threads...\n", workers);
    rets = malloc(sizeof(aligned_t) * MAXPARALLELISM);
    assert(rets);

    /* BALANCED SYNCVAR READFF LOOP (strong scaling) */
    if (TEST_SELECTION & 1) {
        syncvar_t *shared;
        printf("\tBalanced competing syncvar readFF: ");
        fflush(stdout);
        shared = (syncvar_t *)calloc(1, sizeof(syncvar_t));
        assert(shared);
        qtimer_start(timer);
        qt_loop_balance(0, MAXPARALLELISM * ITERATIONS,
                        balanced_syncvar_readFF,
                        shared);
        qtimer_stop(timer);
        free(shared);

        printf("%15g secs (%u-threads %u iters)\n", qtimer_secs(timer),
               workers, (unsigned)(ITERATIONS * MAXPARALLELISM));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ITERATIONS * MAXPARALLELISM));
        printf("\t = read throughput: %30f reads/sec\n",
               (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer));
        rate =
            (ITERATIONS * MAXPARALLELISM * sizeof(aligned_t)) /
            qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }
    /* BALANCED READFF LOOP (strong scaling) */
    if (TEST_SELECTION & (1 << 1)) {
        aligned_t *shared;
        printf("\tBalanced competing readFF: ");
        fflush(stdout);
        shared = (aligned_t *)calloc(1, sizeof(aligned_t));
        qtimer_start(timer);
        qt_loop_balance(0, MAXPARALLELISM * ITERATIONS, balanced_readFF,
                        shared);
        qtimer_stop(timer);
        free(shared);

        printf("%23g secs (%u-threads %u iters)\n", qtimer_secs(timer),
               workers, (unsigned)(ITERATIONS * MAXPARALLELISM));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ITERATIONS * MAXPARALLELISM));
        printf("\t = read throughput: %30f reads/sec\n",
               (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer));
        rate =
            (ITERATIONS * MAXPARALLELISM * sizeof(aligned_t)) /
            qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }

    if (TEST_SELECTION & (1 << 2)) {
        syncvar_t *shared;
        printf("\tBalanced false-sharing syncvar readFF: ");
        fflush(stdout);
        shared = (syncvar_t *)calloc(workers, sizeof(syncvar_t));
        qtimer_start(timer);
        qt_loop_balance(0, ITERATIONS * MAXPARALLELISM,
                        balanced_falseshare_syncreadFF,
                        shared);
        qtimer_stop(timer);
        free(shared);

        printf("%11g secs (%u-threads %u iters)\n", qtimer_secs(timer),
               workers, (unsigned)(ITERATIONS * MAXPARALLELISM));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ITERATIONS * MAXPARALLELISM));
        printf("\t = read throughput: %30f reads/sec\n",
               (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer));
        rate =
            (ITERATIONS * MAXPARALLELISM * sizeof(aligned_t)) / qtimer_secs(
                                                                            timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }

    if (TEST_SELECTION & (1 << 3)) {
        aligned_t *shared;
        printf("\tBalanced false-sharing readFF: ");
        fflush(stdout);
        shared = (aligned_t *)calloc(workers, sizeof(aligned_t));
        qtimer_start(timer);
        qt_loop_balance(0, ITERATIONS * MAXPARALLELISM,
                        balanced_falseshare_readFF,
                        shared);
        qtimer_stop(timer);
        free(shared);

        printf("%19g secs (%u-threads %u iters)\n", qtimer_secs(timer),
               workers, (unsigned)(ITERATIONS * MAXPARALLELISM));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ITERATIONS * MAXPARALLELISM));
        printf("\t = read throughput: %30f reads/sec\n",
               (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer));
        rate =
            (ITERATIONS * MAXPARALLELISM * sizeof(aligned_t)) / qtimer_secs(
                                                                            timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }

    if (TEST_SELECTION & (1 << 4)) {
        /* BALANCED INDEPENDENT LOOP */
        printf("\tBalanced independent syncvar readFF: ");
        fflush(stdout);
        qtimer_start(timer);
        qt_loop_balance(0, ITERATIONS * MAXPARALLELISM,
                        balanced_noncomp_syncreadFF,
                        NULL);
        qtimer_stop(timer);

        printf("%13g secs (%u-threads %u iters)\n", qtimer_secs(timer),
               workers, (unsigned)(ITERATIONS * MAXPARALLELISM));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ITERATIONS * MAXPARALLELISM));
        printf("\t = read throughput: %30f reads/sec\n",
               (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer));
        rate =
            (ITERATIONS * MAXPARALLELISM * sizeof(aligned_t)) / qtimer_secs(
                                                                            timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }

    if (TEST_SELECTION & (1 << 5)) {
        /* BALANCED INDEPENDENT LOOP */
        printf("\tBalanced independent readFF: ");
        fflush(stdout);
        /* prime it */
        qt_loop_balance(0, ITERATIONS * MAXPARALLELISM,
                        balanced_noncomp_readFF,
                        NULL);
        /* time it */
        qtimer_start(timer);
        qt_loop_balance(0, ITERATIONS * MAXPARALLELISM,
                        balanced_noncomp_readFF,
                        NULL);
        qtimer_stop(timer);

        printf("%21g secs (%u-threads %u iters)\n", qtimer_secs(timer),
               workers, (unsigned)(ITERATIONS * MAXPARALLELISM));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ITERATIONS * MAXPARALLELISM));
        printf("\t = read throughput: %30f reads/sec\n",
               (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer));
        rate =
            (ITERATIONS * MAXPARALLELISM * sizeof(aligned_t)) / qtimer_secs(
                                                                            timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }

    if (TEST_SELECTION & (1 << 6)) {
        syncvar_t *rets = calloc(ITERATIONS, sizeof(syncvar_t));
        size_t     i;
        /* LOOP SYNCHRONIZATION */
        printf("\tLoop syncvar synchronization: ");
        fflush(stdout);
        /* prime it */
        for (i = 0; i < ITERATIONS; i++) {
            qthread_fork_syncvar(justreturn, (void *)(intptr_t)i, rets + i);
        }
        for (i = 0; i < ITERATIONS; i++) {
            uint64_t r = 0;
            qthread_syncvar_readFF(&r, rets + i);
            assert(r == 7);
        }
        /* time it */
        qtimer_start(timer);
        for (i = 0; i < ITERATIONS; i++) {
            qthread_fork_syncvar(justreturn, (void *)(intptr_t)i, rets + i);
        }
        for (i = 0; i < ITERATIONS; i++) {
            qthread_syncvar_readFF(NULL, rets + i);
        }
        qtimer_stop(timer);
        free(rets);

        printf("%21g secs (%u-threads %u-qthreads)\n", qtimer_secs(timer),
               workers, (unsigned)(ITERATIONS));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ITERATIONS));
        printf("\t = read throughput: %30f reads/sec\n",
               (ITERATIONS) / qtimer_secs(timer));
        rate = (ITERATIONS * sizeof(syncvar_t)) / qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }

    if (TEST_SELECTION & (1 << 7)) {
        aligned_t *rets = calloc(ITERATIONS, sizeof(aligned_t));
        size_t     i;
        /* LOOP SYNCHRONIZATION */
        printf("\tLoop synchronization: ");
        fflush(stdout);
        /* prime it */
        for (i = 0; i < ITERATIONS; i++) {
            qthread_fork(justreturn, (void *)(intptr_t)i, rets + i);
        }
        for (i = 0; i < ITERATIONS; i++) {
            aligned_t r = 0;
            qthread_readFF(&r, rets + i);
            assert(r == 7);
        }
        /* time it */
        qtimer_start(timer);
        for (i = 0; i < ITERATIONS; i++) {
            qthread_fork(justreturn, (void *)(intptr_t)i, rets + i);
        }
        for (i = 0; i < ITERATIONS; i++) {
            qthread_readFF(NULL, rets + i);
        }
        qtimer_stop(timer);
        free(rets);

        printf("%21g secs (%u-threads %u-qthreads)\n", qtimer_secs(timer),
               workers, (unsigned)(ITERATIONS));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ITERATIONS));
        printf("\t = read throughput: %30f reads/sec\n",
               (ITERATIONS) / qtimer_secs(timer));
        rate = (ITERATIONS * sizeof(aligned_t)) / qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }

    qtimer_destroy(timer);
    free(rets);

    return 0;
}

/* vim:set expandtab */
