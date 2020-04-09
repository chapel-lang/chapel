#include <stdio.h>                     /* for printf() */
#include <stdlib.h>                    /* for strtol() */
#include <assert.h>                    /* for assert() */
#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

size_t     ARRAY_SIZE     = 4096*4096;
size_t     TEST_SELECTION = 0xffffffff;
size_t     ITERATIONS     = 100000;
size_t     MAXPARALLELISM = 256;
aligned_t  incrementme    = 0;
aligned_t *increments     = NULL;

static aligned_t syncvar_writer(void *arg)
{
    syncvar_t *shared = (syncvar_t*)arg;

    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
	qthread_syncvar_writeF_const(&(shared[i]), i);
    }
    return 0;
}

static aligned_t syncvar_reader(void *arg)
{
    syncvar_t *shared = (syncvar_t*)arg;

    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
	qthread_syncvar_readFF(NULL, &(shared[i]));
    }
    return 0;
}

static aligned_t aligned_writer(void *arg)
{
    aligned_t *shared = (aligned_t*)arg;

    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
	qthread_writeF_const(&(shared[i]), i);
    }
    return 0;
}

static aligned_t aligned_reader(void *arg)
{
    aligned_t *shared = (aligned_t*)arg;

    for (size_t i = 0; i < ARRAY_SIZE; ++i) {
	qthread_readFF(NULL, &(shared[i]));
    }
    return 0;
}

static void syncvar_writer_parallel(const size_t startat,
                                    const size_t stopat,
                                    void        *arg)
{                                      /*{{{ */
    size_t i;

    for (i = startat; i < stopat; i++) {
        qthread_syncvar_writeF_const(&(((syncvar_t *)arg)[i]), 1);
    }
}                                      /*}}} */

static void syncvar_reader_parallel(const size_t startat,
                                    const size_t stopat,
                                    void        *arg)
{                                      /*{{{ */
    size_t i;

    for (i = startat; i < stopat; i++) {
        qthread_syncvar_readFF(NULL, &(((syncvar_t *)arg)[i]));
    }
}                                      /*}}} */

struct arg {
    size_t start, stop;
    void *arg;
};

static aligned_t syncvar_prodcons_reader(void *arg)
{
    struct arg* a = (struct arg*)arg;
    syncvar_reader_parallel(a->start, a->stop, a->arg);
    return 0;
}

static aligned_t syncvar_prodcons_writer(void *arg)
{
    struct arg* a = (struct arg*)arg;
    syncvar_writer_parallel(a->start, a->stop, a->arg);
    return 0;
}

static void syncvar_prodcons(const size_t startat,
                             const size_t stopat,
                             void        *arg)
{                                      /*{{{ */
    syncvar_t rets[2];
    struct arg a = {startat, stopat, arg};

    qthread_fork_syncvar_copyargs(syncvar_prodcons_reader, &a, sizeof(struct arg), &rets[0]);
    qthread_fork_syncvar_copyargs(syncvar_prodcons_writer, &a, sizeof(struct arg), &rets[1]);
    qthread_yield();
    qthread_syncvar_readFF(NULL, &rets[0]);
    qthread_syncvar_readFF(NULL, &rets[1]);
}                                      /*}}} */

static void aligned_writer_parallel(const size_t startat,
                                    const size_t stopat,
                                    void        *arg)
{                                      /*{{{ */
    size_t i;

    for (i = startat; i < stopat; i++) {
        qthread_writeF_const(&(((aligned_t *)arg)[i]), 1);
    }
}                                      /*}}} */

static void aligned_reader_parallel(const size_t startat,
                                    const size_t stopat,
                                    void        *arg)
{                                      /*{{{ */
    size_t i;

    for (i = startat; i < stopat; i++) {
        qthread_readFF(NULL, &(((aligned_t *)arg)[i]));
    }
}                                      /*}}} */

static aligned_t aligned_prodcons_reader(void *arg)
{
    struct arg* a = (struct arg*)arg;
    aligned_reader_parallel(a->start, a->stop, a->arg);
    return 0;
}

static aligned_t aligned_prodcons_writer(void *arg)
{
    struct arg* a = (struct arg*)arg;
    aligned_writer_parallel(a->start, a->stop, a->arg);
    return 0;
}

static void aligned_prodcons(const size_t startat,
                             const size_t stopat,
                             void        *arg)
{                                      /*{{{ */
    syncvar_t rets[2];
    struct arg a = {startat, stopat, arg};

    qthread_fork_syncvar_copyargs(aligned_prodcons_reader, &a, sizeof(struct arg), &rets[0]);
    qthread_fork_syncvar_copyargs(aligned_prodcons_writer, &a, sizeof(struct arg), &rets[1]);
    qthread_yield();
    qthread_syncvar_readFF(NULL, &rets[0]);
    qthread_syncvar_readFF(NULL, &rets[1]);
}                                      /*}}} */

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
    NUMARG(ARRAY_SIZE, "ARRAY_SIZE");
    NUMARG(MAXPARALLELISM, "MAXPARALLELISM");
    NUMARG(TEST_SELECTION, "TEST_SELECTION");
    workers = qthread_num_workers();
    printf("%u threads...\n", workers);
    rets = malloc(sizeof(aligned_t) * MAXPARALLELISM);
    assert(rets);

    if (TEST_SELECTION & (1 << 0)) {
        syncvar_t *shared;
        printf("\tsyncvar readFF, uncontended: ");
        fflush(stdout);
        shared = (syncvar_t *)calloc(ARRAY_SIZE, sizeof(syncvar_t));
	for (size_t i = 0; i < ARRAY_SIZE; ++i) {
	    shared[i] = SYNCVAR_EMPTY_INITIALIZER;
	}
        qtimer_start(timer);
	qthread_fork(syncvar_writer, (void*)shared, rets);
	qthread_readFF(NULL, rets);
	qthread_fork(syncvar_reader, (void*)shared, rets);
	qthread_readFF(NULL, rets);
        qtimer_stop(timer);
        free(shared);

        printf("%11g secs (%u iters)\n", qtimer_secs(timer),
               (unsigned)(ARRAY_SIZE));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ARRAY_SIZE));
        printf("\t = throughput: %30f ops/sec\n",
               (2*ARRAY_SIZE) / qtimer_secs(timer));
        rate = (ARRAY_SIZE * sizeof(syncvar_t)) / qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }
    if (TEST_SELECTION & (1 << 1)) {
        aligned_t *shared;
        printf("\taligned_t readFF, uncontended: ");
        fflush(stdout);
        shared = (aligned_t *)calloc(ARRAY_SIZE, sizeof(aligned_t));
        qtimer_start(timer);
	qthread_fork(aligned_writer, (void*)shared, rets);
	qthread_readFF(NULL, rets);
	qthread_fork(aligned_reader, (void*)shared, rets);
	qthread_readFF(NULL, rets);
        qtimer_stop(timer);
        free(shared);

        printf("%11g secs (%u iters)\n", qtimer_secs(timer),
               (unsigned)(ARRAY_SIZE));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ARRAY_SIZE));
        printf("\t = throughput: %30f ops/sec\n",
               (2*ARRAY_SIZE) / qtimer_secs(timer));
        rate = (ARRAY_SIZE * sizeof(syncvar_t)) / qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }
    if (TEST_SELECTION & (1 << 2)) {
        syncvar_t *shared;
        printf("\tsyncvar readFF, contended: ");
        fflush(stdout);
        shared = (syncvar_t *)calloc(ARRAY_SIZE, sizeof(syncvar_t));
	for (size_t i = 0; i < ARRAY_SIZE; ++i) {
	    shared[i] = SYNCVAR_EMPTY_INITIALIZER;
	}
        qtimer_start(timer);
	qthread_fork(syncvar_writer, (void*)shared, rets);
	qthread_fork(syncvar_reader, (void*)shared, rets + 1);
	qthread_readFF(NULL, rets);
	qthread_readFF(NULL, rets + 1);
        qtimer_stop(timer);
        free(shared);

        printf("%11g secs (%u iters)\n", qtimer_secs(timer),
               (unsigned)(ARRAY_SIZE));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ARRAY_SIZE));
        printf("\t = throughput: %30f ops/sec\n",
               (2*ARRAY_SIZE) / qtimer_secs(timer));
        rate = (ARRAY_SIZE * sizeof(syncvar_t)) / qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }
    if (TEST_SELECTION & (1 << 3)) {
        aligned_t *shared;
        printf("\taligned_t readFF, contended: ");
        fflush(stdout);
        shared = (aligned_t *)calloc(ARRAY_SIZE, sizeof(aligned_t));
        qtimer_start(timer);
	qthread_fork(aligned_writer, (void*)shared, rets);
	qthread_fork(aligned_reader, (void*)shared, rets + 1);
	qthread_readFF(NULL, rets);
	qthread_readFF(NULL, rets + 1);
        qtimer_stop(timer);
        free(shared);

        printf("%11g secs (%u iters)\n", qtimer_secs(timer),
               (unsigned)(ARRAY_SIZE));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ARRAY_SIZE));
        printf("\t = throughput: %30f ops/sec\n",
               (2*ARRAY_SIZE) / qtimer_secs(timer));
        rate = (ARRAY_SIZE * sizeof(syncvar_t)) / qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }
    if (TEST_SELECTION & (1 << 4)) {
        syncvar_t *shared;
        printf("\tsyncvar readFF, parallel uncontended: ");
        fflush(stdout);
        shared = (syncvar_t *)calloc(ARRAY_SIZE, sizeof(syncvar_t));
	for (size_t i = 0; i < ARRAY_SIZE; ++i) {
	    shared[i] = SYNCVAR_EMPTY_INITIALIZER;
	}
        qtimer_start(timer);
	qt_loop_balance(0, ARRAY_SIZE, syncvar_writer_parallel, (void*)shared);
	qt_loop_balance(0, ARRAY_SIZE, syncvar_reader_parallel, (void*)shared);
        qtimer_stop(timer);
        free(shared);

        printf("%11g secs (%u iters)\n", qtimer_secs(timer),
               (unsigned)(ARRAY_SIZE));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ARRAY_SIZE));
        printf("\t = throughput: %30f ops/sec\n",
               (2*ARRAY_SIZE) / qtimer_secs(timer));
        rate = (ARRAY_SIZE * sizeof(syncvar_t)) / qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }
    if (TEST_SELECTION & (1 << 5)) {
        aligned_t *shared;
        printf("\taligned_t readFF, parallel uncontended: ");
        fflush(stdout);
        shared = (aligned_t *)calloc(ARRAY_SIZE, sizeof(aligned_t));
        qtimer_start(timer);
	qt_loop_balance(0, ARRAY_SIZE, aligned_writer_parallel, (void*)shared);
	qt_loop_balance(0, ARRAY_SIZE, aligned_reader_parallel, (void*)shared);
        qtimer_stop(timer);
        free(shared);

        printf("%11g secs (%u iters)\n", qtimer_secs(timer),
               (unsigned)(ARRAY_SIZE));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ARRAY_SIZE));
        printf("\t = throughput: %30f ops/sec\n",
               (2*ARRAY_SIZE) / qtimer_secs(timer));
        rate = (ARRAY_SIZE * sizeof(syncvar_t)) / qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }
    if (TEST_SELECTION & (1 << 6)) {
        syncvar_t *shared;
        printf("\tsyncvar readFF, contended parallel: ");
        fflush(stdout);
        shared = (syncvar_t *)calloc(ARRAY_SIZE, sizeof(syncvar_t));
	for (size_t i = 0; i < ARRAY_SIZE; ++i) {
	    shared[i] = SYNCVAR_EMPTY_INITIALIZER;
	}
        qtimer_start(timer);
	qt_loop_balance(0, ARRAY_SIZE, syncvar_prodcons, (void*)shared);
        qtimer_stop(timer);
        free(shared);

        printf("%11g secs (%u iters)\n", qtimer_secs(timer),
               (unsigned)(ARRAY_SIZE));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ARRAY_SIZE));
        printf("\t = throughput: %30f ops/sec\n",
               (2*ARRAY_SIZE) / qtimer_secs(timer));
        rate = (ARRAY_SIZE * sizeof(syncvar_t)) / qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }
    if (TEST_SELECTION & (1 << 7)) {
        aligned_t *shared;
        printf("\taligned_t readFF, contended parallel: ");
        fflush(stdout);
        shared = (aligned_t *)calloc(ARRAY_SIZE, sizeof(aligned_t));
        qtimer_start(timer);
	qt_loop_balance(0, ARRAY_SIZE, aligned_prodcons, (void*)shared);
        qtimer_stop(timer);
        free(shared);

        printf("%11g secs (%u iters)\n", qtimer_secs(timer),
               (unsigned)(ARRAY_SIZE));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ARRAY_SIZE));
        printf("\t = throughput: %30f ops/sec\n",
               (2*ARRAY_SIZE) / qtimer_secs(timer));
        rate = (ARRAY_SIZE * sizeof(syncvar_t)) / qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }
    if (TEST_SELECTION & (1 << 8)) {
        syncvar_t *shared;
        printf("\tsyncvar readFF, contended 1-N: ");
        fflush(stdout);
        shared = (syncvar_t *)calloc(ARRAY_SIZE, sizeof(syncvar_t));
	for (size_t i = 0; i < ARRAY_SIZE; ++i) {
	    shared[i] = SYNCVAR_EMPTY_INITIALIZER;
	}
        qtimer_start(timer);
	qthread_fork(syncvar_writer, (void*)shared, rets);
	qt_loop_balance(0, ARRAY_SIZE, syncvar_reader_parallel, (void*)shared);
	qthread_readFF(NULL, rets);
        qtimer_stop(timer);
        free(shared);

        printf("%11g secs (%u iters)\n", qtimer_secs(timer),
               (unsigned)(ARRAY_SIZE));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ARRAY_SIZE));
        printf("\t = throughput: %30f ops/sec\n",
               (2*ARRAY_SIZE) / qtimer_secs(timer));
        rate = (ARRAY_SIZE * sizeof(syncvar_t)) / qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }
    if (TEST_SELECTION & (1 << 9)) {
        aligned_t *shared;
        printf("\taligned_t readFF, contended 1-N: ");
        fflush(stdout);
        shared = (aligned_t *)calloc(ARRAY_SIZE, sizeof(aligned_t));
        qtimer_start(timer);
	qthread_fork(aligned_writer, (void*)shared, rets);
	qt_loop_balance(0, ARRAY_SIZE, aligned_reader_parallel, (void*)shared);
	qthread_readFF(NULL, rets);
        qtimer_stop(timer);
        free(shared);

        printf("%11g secs (%u iters)\n", qtimer_secs(timer),
               (unsigned)(ARRAY_SIZE));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ARRAY_SIZE));
        printf("\t = throughput: %30f ops/sec\n",
               (2*ARRAY_SIZE) / qtimer_secs(timer));
        rate = (ARRAY_SIZE * sizeof(syncvar_t)) / qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }
    if (TEST_SELECTION & (1 << 10)) {
        syncvar_t *shared;
        printf("\tsyncvar readFF, uncontended 1-N: ");
        fflush(stdout);
        shared = (syncvar_t *)calloc(ARRAY_SIZE, sizeof(syncvar_t));
	for (size_t i = 0; i < ARRAY_SIZE; ++i) {
	    shared[i] = SYNCVAR_EMPTY_INITIALIZER;
	}
        qtimer_start(timer);
	qthread_fork(syncvar_writer, (void*)shared, rets);
	qthread_readFF(NULL, rets);
	qt_loop_balance(0, ARRAY_SIZE, syncvar_reader_parallel, (void*)shared);
        qtimer_stop(timer);
        free(shared);

        printf("%11g secs (%u iters)\n", qtimer_secs(timer),
               (unsigned)(ARRAY_SIZE));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ARRAY_SIZE));
        printf("\t = throughput: %30f ops/sec\n",
               (2*ARRAY_SIZE) / qtimer_secs(timer));
        rate = (ARRAY_SIZE * sizeof(syncvar_t)) / qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }
    if (TEST_SELECTION & (1 << 11)) {
        aligned_t *shared;
        printf("\taligned_t readFF, uncontended 1-N: ");
        fflush(stdout);
        shared = (aligned_t *)calloc(ARRAY_SIZE, sizeof(aligned_t));
        qtimer_start(timer);
	qthread_fork(aligned_writer, (void*)shared, rets);
	qthread_readFF(NULL, rets);
	qt_loop_balance(0, ARRAY_SIZE, aligned_reader_parallel, (void*)shared);
        qtimer_stop(timer);
        free(shared);

        printf("%11g secs (%u iters)\n", qtimer_secs(timer),
               (unsigned)(ARRAY_SIZE));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ARRAY_SIZE));
        printf("\t = throughput: %30f ops/sec\n",
               (2*ARRAY_SIZE) / qtimer_secs(timer));
        rate = (ARRAY_SIZE * sizeof(syncvar_t)) / qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }
    if (TEST_SELECTION & (1 << 12)) {
        syncvar_t *shared;
        printf("\tsyncvar readFF, contended N-1: ");
        fflush(stdout);
        shared = (syncvar_t *)calloc(ARRAY_SIZE, sizeof(syncvar_t));
	for (size_t i = 0; i < ARRAY_SIZE; ++i) {
	    shared[i] = SYNCVAR_EMPTY_INITIALIZER;
	}
        qtimer_start(timer);
	qthread_fork(syncvar_reader, (void*)shared, rets);
	qt_loop(0, ARRAY_SIZE, syncvar_writer_parallel, (void*)shared);
	qthread_readFF(NULL, rets);
        qtimer_stop(timer);
        free(shared);

        printf("%11g secs (%u iters)\n", qtimer_secs(timer),
               (unsigned)(ARRAY_SIZE));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ARRAY_SIZE));
        printf("\t = throughput: %30f ops/sec\n",
               (2*ARRAY_SIZE) / qtimer_secs(timer));
        rate = (ARRAY_SIZE * sizeof(syncvar_t)) / qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }
    if (TEST_SELECTION & (1 << 13)) {
        aligned_t *shared;
        printf("\taligned_t readFF, contended N-1: ");
        fflush(stdout);
        shared = (aligned_t *)calloc(ARRAY_SIZE, sizeof(aligned_t));
        qtimer_start(timer);
	qthread_fork(aligned_reader, (void*)shared, rets);
	qt_loop(0, ARRAY_SIZE, aligned_writer_parallel, (void*)shared);
	qthread_readFF(NULL, rets);
        qtimer_stop(timer);
        free(shared);

        printf("%11g secs (%u iters)\n", qtimer_secs(timer),
               (unsigned)(ARRAY_SIZE));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ARRAY_SIZE));
        printf("\t = throughput: %30f ops/sec\n",
               (2*ARRAY_SIZE) / qtimer_secs(timer));
        rate = (ARRAY_SIZE * sizeof(syncvar_t)) / qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }
    if (TEST_SELECTION & (1 << 14)) {
        syncvar_t *shared;
        printf("\tsyncvar readFF, uncontended N-1: ");
        fflush(stdout);
        shared = (syncvar_t *)calloc(ARRAY_SIZE, sizeof(syncvar_t));
	for (size_t i = 0; i < ARRAY_SIZE; ++i) {
	    shared[i] = SYNCVAR_EMPTY_INITIALIZER;
	}
        qtimer_start(timer);
	qt_loop_balance(0, ARRAY_SIZE, syncvar_writer_parallel, (void*)shared);
	qthread_fork(syncvar_reader, (void*)shared, rets);
	qthread_readFF(NULL, rets);
        qtimer_stop(timer);
        free(shared);

        printf("%11g secs (%u iters)\n", qtimer_secs(timer),
               (unsigned)(ARRAY_SIZE));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ARRAY_SIZE));
        printf("\t = throughput: %30f ops/sec\n",
               (2*ARRAY_SIZE) / qtimer_secs(timer));
        rate = (ARRAY_SIZE * sizeof(syncvar_t)) / qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }
    if (TEST_SELECTION & (1 << 15)) {
        aligned_t *shared;
        printf("\taligned_t readFF, uncontended N-1: ");
        fflush(stdout);
        shared = (aligned_t *)calloc(ARRAY_SIZE, sizeof(aligned_t));
        qtimer_start(timer);
	qt_loop_balance(0, ARRAY_SIZE, aligned_writer_parallel, (void*)shared);
        qtimer_stop(timer);
	qthread_fork(aligned_reader, (void*)shared, rets);
	qthread_readFF(NULL, rets);
        free(shared);

        printf("%11g secs (%u iters)\n", qtimer_secs(timer),
               (unsigned)(ARRAY_SIZE));
        iprintf("\t + average read time: %28g secs\n",
                qtimer_secs(timer) / (ARRAY_SIZE));
        printf("\t = throughput: %30f ops/sec\n",
               (2*ARRAY_SIZE) / qtimer_secs(timer));
        rate = (ARRAY_SIZE * sizeof(syncvar_t)) / qtimer_secs(timer);
        printf("\t = data throughput: %30g bytes/sec %s\n", rate,
               human_readable_rate(rate));
    }

    qtimer_destroy(timer);
    free(rets);

    return 0;
}

/* vim:set expandtab */
