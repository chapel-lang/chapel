#include <stdio.h>                     /* for printf() */
#include <stdlib.h>                    /* for strtol() */
#include <assert.h>                    /* for assert() */
#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

int MACHINE_READABLE = 0;
size_t TEST_SELECTION = 0xffffffff;
size_t ITERATIONS = 1000000;
size_t MAXPARALLELISM = 256;
aligned_t incrementme = 0;
aligned_t *increments = NULL;

// PTREE_DEPTH controls the tradeoff between
// overhead and concurrent capacity of the Index: larger
// PTREE_DEPTH ==> greater capacity for beating on increment().
#define PTREE_DEPTH     0              /* equivalent to DUMB_INCREMENT */
#define ROOT_PRISM_SIZE (1 << PTREE_DEPTH)
#define NUM_LEAVES      (1 << PTREE_DEPTH)
aligned_t prism[(1 << (PTREE_DEPTH + 1)) - 1][ROOT_PRISM_SIZE + 1];

static void init_prism(
    void)
{
    memset(prism, 0, sizeof(prism));
}

/* the following int_fetch_inc() function borrowed from Simon Kahan's 2010
 * PRMHTS talk */
static aligned_t int_fetch_inc(
    void)
{
    unsigned int offset, node, depth;

    for (offset = 0, node = 0, depth = 0; depth < PTREE_DEPTH; ++depth) {
        unsigned int pindex =
            qtimer_fastrand() % (ROOT_PRISM_SIZE >> depth) + 1;
        aligned_t try1 = qthread_incr(&prism[node][pindex], 1);
        if (try1 & 0x1) {              // try is odd
            node = 2 * node + 2;       // go right
            offset += 1 << depth;
        } else {                       // try is even
            if (prism[node][pindex] != try1 + 1) {      // did anyone come by?
                node = 2 * node + 1;   // yes, so I'll go left
            } else {                   // no...
                aligned_t try2;
                qthread_readFE(&try2, &prism[node][pindex]);
                if (try2 != try1 + 1) { // anyone now?
                    qthread_fill(&prism[node][pindex]); // yes, so go left
                    node = 2 * node + 1;
                } else {               // still no one...
                    qthread_writeF(&prism[node][pindex], &try1);        // as if never in'prism'ed
                    aligned_t addend = qthread_incr(&prism[node][0], 1) & 0x1;
                    if (addend) {
                        offset += 1 << depth;
                    }
                    node = 2 * node + 1 + addend;
                }
            }
        }
    }
    return offset + NUM_LEAVES *qthread_incr(&prism[node][0], 1);
}

static void balanced_incr(
    const size_t startat,
    const size_t stopat,
    void *arg)
{                                      /*{{{ */
    size_t i;

    for (i = startat; i < stopat; i++) {
        qthread_incr((aligned_t *)arg, 1);
    }
}                                      /*}}} */

static void diffract_incr(
    const size_t startat,
    const size_t stopat,
    void *arg)
{                                      /*{{{ */
    size_t i;
    aligned_t sum = 0;

    for (i = startat; i < stopat; ++i) {
        sum = int_fetch_inc();
    }
    ((aligned_t *)arg)[qthread_id()] = sum + 1;
}                                      /*}}} */

static void balanced_falseshare(
    const size_t startat,
    const size_t stopat,
    void *arg)
{                                      /*{{{ */
    size_t i;
    qthread_shepherd_id_t shep = qthread_shep();
    aligned_t *volatile myinc = increments + shep;

    for (i = startat; i < stopat; i++) {
        qthread_incr(myinc, 1);
    }
}                                      /*}}} */

static void balanced_noncomp(
    const size_t startat,
    const size_t stopat,
    void *arg)
{                                      /*{{{ */
    size_t i;
    qthread_shepherd_id_t shep = qthread_shep();
    aligned_t myinc;

    assert(shep != NO_SHEPHERD);
    for (i = startat; i < stopat; i++) {
        qthread_incr(&myinc, 1);
    }
}                                      /*}}} */

static aligned_t incrloop(
    void *arg)
{                                      /*{{{ */
    unsigned int i;

    for (i = 0; i < ITERATIONS; i++) {
        qthread_incr(&incrementme, 1);
    }
    return 0;
}                                      /*}}} */

static aligned_t incrloop_falseshare(
    void *arg)
{                                      /*{{{ */
    unsigned int offset = (unsigned int)(intptr_t)arg;
    unsigned int i;
    aligned_t *volatile myinc = increments + offset;

    for (i = 0; i < ITERATIONS; i++) {
        qthread_incr(myinc, 1);
    }
    return 0;
}                                      /*}}} */

static aligned_t incrloop_nocompete(
    void *arg)
{                                      /*{{{ */
    unsigned int i;
    aligned_t myinc;

    for (i = 0; i < ITERATIONS; i++) {
        qthread_incr(&myinc, 1);
    }
    return 0;
}                                      /*}}} */

static aligned_t incrstream(
    void *arg)
{                                      /*{{{ */
    unsigned int i;
    aligned_t *const myinc = (aligned_t *)arg;

    for (i = 0; i < ITERATIONS; i++) {
        qthread_incr(myinc + i, 1);
    }
    return 0;
}                                      /*}}} */

static aligned_t addloop_falseshare(
    void *arg)
{                                      /*{{{ */
    unsigned int offset = (unsigned int)(intptr_t)arg;
    unsigned int i;
    aligned_t *volatile myinc = increments + offset;

    for (i = 0; i < ITERATIONS; i++) {
        (*myinc)++;
    }
    return *myinc;
}                                      /*}}} */

static aligned_t addloop_nocompete(
    void *arg)
{                                      /*{{{ */
    unsigned int i;
    aligned_t myinc = 0;

    for (i = 0; i < ITERATIONS; i++) {
        myinc++;
    }
    return myinc;
}                                      /*}}} */

static void streaming_incr(
    const size_t startat,
    const size_t stopat,
    void *arg)
{                                      /*{{{ */
    size_t i;

    for (i = startat; i < stopat; i++) {
        qthread_incr(increments + i, 1);
    }
}                                      /*}}} */

static void streaming_naincr(
    const size_t startat,
    const size_t stopat,
    void *arg)
{                                      /*{{{ */
    size_t i;

    for (i = startat; i < stopat; i++) {
        increments[i]++;
    }
}                                      /*}}} */

static char *human_readable_rate(
    double rate)
{
    static char readable_string[100] = { 0 };
    const double GB = 1024 * 1024 * 1024;
    const double MB = 1024 * 1024;
    const double kB = 1024;

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
}

int main(
    int argc,
    char *argv[])
{
    qtimer_t timer = qtimer_create();
    double rate;
    unsigned int i;
    aligned_t *rets;
    unsigned int shepherds = 1;

    /* setup */
    assert(qthread_initialize() == QTHREAD_SUCCESS);

    CHECK_VERBOSE();
    NUMARG(ITERATIONS, "ITERATIONS");
    NUMARG(MAXPARALLELISM, "MAXPARALLELISM");
    NUMARG(TEST_SELECTION, "TEST_SELECTION");
    NUMARG(MACHINE_READABLE, "MACHINE_READABLE");
    shepherds = qthread_num_shepherds();
    if (!MACHINE_READABLE) {
        printf("%u shepherds...\n", shepherds);
    }
    rets = malloc(sizeof(aligned_t) * MAXPARALLELISM);
    assert(rets);

    /* BALANCED INCREMENT LOOP (strong scaling) */
    if (TEST_SELECTION & 1) {
        if (!MACHINE_READABLE) {
            printf("\tBalanced competing loop: ");
            fflush(stdout);
        }
        increments = (aligned_t *)calloc(1, sizeof(aligned_t));
        qtimer_start(timer);
        qt_loop_balance(0, MAXPARALLELISM * ITERATIONS, balanced_incr,
                        increments);
        qtimer_stop(timer);
        assert(*increments == MAXPARALLELISM * ITERATIONS);
        free(increments);
        increments = NULL;

        rate =
            (ITERATIONS * MAXPARALLELISM * sizeof(aligned_t)) /
            qtimer_secs(timer);
        switch (MACHINE_READABLE) {
            case 0:
                printf("%19g secs (%u-threads %u iters)\n",
                       qtimer_secs(timer), shepherds,
                       (unsigned)(ITERATIONS * MAXPARALLELISM));
                iprintf("\t + average increment time: %17g secs\n",
                        qtimer_secs(timer) / (ITERATIONS * MAXPARALLELISM));
                printf("\t = increment throughput: %19f increments/sec\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer));
                printf("\t = data throughput: %24g bytes/sec %s\n", rate,
                       human_readable_rate(rate));
                break;
            case 1:
                printf("%f,%g,",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
                break;
            case 2:
                printf("%f\n%g\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
                break;
        }
    }

    /* BALANCED FALSE-SHARING LOOP */
    if (TEST_SELECTION & (1 << 1)) {
        if (!MACHINE_READABLE) {
            printf("\tBalanced false-sharing loop: ");
            fflush(stdout);
        }
        increments = (aligned_t *)calloc(shepherds, sizeof(aligned_t));
        qtimer_start(timer);
        qt_loop_balance(0, ITERATIONS * 256, balanced_falseshare, NULL);
        qtimer_stop(timer);
        free(increments);
        increments = NULL;

        rate = (ITERATIONS * 256 * sizeof(aligned_t)) / qtimer_secs(timer);
        switch (MACHINE_READABLE) {
            case 0:
                printf("%15g secs (%u-threads %u iters)\n",
                       qtimer_secs(timer), shepherds,
                       (unsigned)(ITERATIONS * 256));
                iprintf("\t + average increment time: %17g secs\n",
                        qtimer_secs(timer) / (ITERATIONS * 256));
                printf("\t = increment throughput: %19f increments/sec\n",
                       (ITERATIONS * 256) / qtimer_secs(timer));
                printf("\t = data throughput: %24g bytes/sec %s\n", rate,
                       human_readable_rate(rate));
                break;
            case 1:
                printf("%f,%g,", (ITERATIONS * 256) / qtimer_secs(timer),
                       rate);
                break;
            case 2:
                printf("%f\n%g\n", (ITERATIONS * 256) / qtimer_secs(timer),
                       rate);
                break;
        }
    }

    if (TEST_SELECTION & (1 << 2)) {
        /* BALANCED INDEPENDENT LOOP */
        if (!MACHINE_READABLE) {
            printf("\tBalanced independent loop: ");
            fflush(stdout);
        }
        qtimer_start(timer);
        qt_loop_balance(0, ITERATIONS * 256, balanced_noncomp, NULL);
        qtimer_stop(timer);

        rate = (ITERATIONS * 256 * sizeof(aligned_t)) / qtimer_secs(timer);
        switch (MACHINE_READABLE) {
            case 0:
                printf("%17g secs (%u-threads %u iters)\n",
                       qtimer_secs(timer), shepherds,
                       (unsigned)(ITERATIONS * 256));
                iprintf("\t + average increment time: %17g secs\n",
                        qtimer_secs(timer) / (ITERATIONS * 256));
                printf("\t = increment throughput: %19f increments/sec\n",
                       (ITERATIONS * 256) / qtimer_secs(timer));
                printf("\t = data throughput: %24g bytes/sec %s\n", rate,
                       human_readable_rate(rate));
                break;
            case 1:
                printf("%f,%g,", (ITERATIONS * 256) / qtimer_secs(timer),
                       rate);
                break;
            case 2:
                printf("%f\n%g\n", (ITERATIONS * 256) / qtimer_secs(timer),
                       rate);
                break;
        }
    }

    if (TEST_SELECTION & (1 << 3)) {
        /* OVER-SUBSCRIBED COMPETING INCREMENT LOOP */
        if (!MACHINE_READABLE) {
            printf("\tOver-subscribed competing loop: ");
            fflush(stdout);
        }
        qtimer_start(timer);
        for (i = 0; i < MAXPARALLELISM; i++) {
            qthread_fork(incrloop, NULL, rets + i);
        }
        for (i = 0; i < MAXPARALLELISM; i++) {
            qthread_readFF(NULL, rets + i);
        }
        qtimer_stop(timer);
        assert(incrementme == ITERATIONS * MAXPARALLELISM);

        rate =
            (ITERATIONS * MAXPARALLELISM * sizeof(aligned_t)) /
            qtimer_secs(timer);
        switch (MACHINE_READABLE) {
            case 0:
                printf("%12g secs (%u-threads %u iters)\n",
                       qtimer_secs(timer), (unsigned)MAXPARALLELISM,
                       (unsigned)ITERATIONS);
                iprintf("\t + average increment time: %17g secs\n",
                        qtimer_secs(timer) / (ITERATIONS * MAXPARALLELISM));
                printf("\t = increment throughput: %19f increments/sec\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer));
                printf("\t = data throughput: %24g bytes/sec %s\n", rate,
                       human_readable_rate(rate));
                break;
            case 1:
                printf("%f,%g,",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
                break;
            case 2:
                printf("%f\n%g\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
                break;
        }
    }

    if (TEST_SELECTION & (1 << 4)) {
        /* OVER-SUBSCRIBED FALSE-SHARING INCREMENT LOOP */
        if (!MACHINE_READABLE) {
            printf("\tOver-subscribed false-sharing loop: ");
            fflush(stdout);
        }
        increments = (aligned_t *)calloc(MAXPARALLELISM, sizeof(aligned_t));
        qtimer_start(timer);
        for (i = 0; i < MAXPARALLELISM; i++) {
            qthread_fork(incrloop_falseshare, (void *)(intptr_t)i, rets + i);
        }
        for (i = 0; i < MAXPARALLELISM; i++) {
            qthread_readFF(NULL, rets + i);
        }
        qtimer_stop(timer);
        free(increments);
        increments = NULL;

        rate =
            (ITERATIONS * MAXPARALLELISM * sizeof(aligned_t)) /
            qtimer_secs(timer);
        switch (MACHINE_READABLE) {
            case 0:
                printf("%8g secs (%u-threads %u iters)\n", qtimer_secs(timer),
                       (unsigned)MAXPARALLELISM, (unsigned)ITERATIONS);
                iprintf("\t + average increment time: %17g secs\n",
                        qtimer_secs(timer) / (ITERATIONS * MAXPARALLELISM));
                printf("\t = increment throughput: %19f increments/sec\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer));
                printf("\t = data throughput: %24g bytes/sec %s\n", rate,
                       human_readable_rate(rate));
                break;
            case 1:
                printf("%g,%f,",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
                break;
            case 2:
                printf("%g\n%f\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
                break;
        }
    }

    if (TEST_SELECTION & (1 << 5)) {
        /* OVER-SUBSCRIBED INDEPENDENT INCREMENT LOOP */
        if (!MACHINE_READABLE) {
            printf("\tOver-subscribed independent loop: ");
            fflush(stdout);
        }
        qtimer_start(timer);
        for (i = 0; i < MAXPARALLELISM; i++) {
            qthread_fork(incrloop_nocompete, (void *)(intptr_t)i, rets + i);
        }
        for (i = 0; i < MAXPARALLELISM; i++) {
            qthread_readFF(NULL, rets + i);
        }
        qtimer_stop(timer);

        rate =
            (ITERATIONS * MAXPARALLELISM * sizeof(aligned_t)) /
            qtimer_secs(timer);
        switch (MACHINE_READABLE) {
            case 0:
                printf("%g secs (%u-threads %u iters)\n", qtimer_secs(timer),
                       (unsigned)MAXPARALLELISM, (unsigned)ITERATIONS);
                iprintf("\t + average increment time: %17g secs\n",
                        qtimer_secs(timer) / (ITERATIONS * MAXPARALLELISM));
                printf("\t = increment throughput: %19f increments/sec\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer));
                printf("\t = data throughput: %24g bytes/sec %s\n", rate,
                       human_readable_rate(rate));
                break;
            case 1:
                printf("%f,%g,",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
                break;
            case 2:
                printf("%f\n%g\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
                break;
        }
    }

    if (TEST_SELECTION & (1 << 6)) {
        /* INDEPENDENT ADD LOOP */
        if (!MACHINE_READABLE) {
            printf("\tNon-atomic false-sharing loop: ");
            fflush(stdout);
        }
        increments = (aligned_t *)calloc(MAXPARALLELISM, sizeof(aligned_t));
        qtimer_start(timer);
        for (i = 0; i < MAXPARALLELISM; i++) {
            qthread_fork(addloop_falseshare, (void *)(intptr_t)i, rets + i);
        }
        for (i = 0; i < MAXPARALLELISM; i++) {
            qthread_readFF(NULL, rets + i);
        }
        qtimer_stop(timer);
        free(increments);
        increments = NULL;

        rate =
            (ITERATIONS * MAXPARALLELISM * sizeof(aligned_t)) /
            qtimer_secs(timer);
        switch (MACHINE_READABLE) {
            case 0:
                printf("%13g secs (%u-way %u iters)\n", qtimer_secs(timer),
                       (unsigned)MAXPARALLELISM, (unsigned)ITERATIONS);
                iprintf("\t + average increment time: %17g secs\n",
                        qtimer_secs(timer) / (ITERATIONS * MAXPARALLELISM));
                printf("\t = increment throughput: %19f increments/sec\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer));
                printf("\t = data throughput: %24g bytes/sec %s\n", rate,
                       human_readable_rate(rate));
                break;
            case 1:
                printf("%f,%g,",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
                break;
            case 2:
                printf("%f\n%g\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
                break;
        }
    }

    if (TEST_SELECTION & (1 << 7)) {
        /* INDEPENDENT NON-ATOMIC LOOP */
        if (!MACHINE_READABLE) {
            printf("\tNon-atomic independent loop: ");
            fflush(stdout);
        }
        qtimer_start(timer);
        for (i = 0; i < MAXPARALLELISM; i++) {
            qthread_fork(addloop_nocompete, (void *)(intptr_t)i, rets + i);
        }
        for (i = 0; i < MAXPARALLELISM; i++) {
            qthread_readFF(NULL, rets + i);
        }
        qtimer_stop(timer);

        rate =
            (ITERATIONS * MAXPARALLELISM * sizeof(aligned_t)) /
            qtimer_secs(timer);
        switch (MACHINE_READABLE) {
            case 0:
                printf("%15g secs (%u-way %u iters)\n", qtimer_secs(timer),
                       (unsigned)MAXPARALLELISM, (unsigned)ITERATIONS);
                iprintf("\t + average increment time: %17g secs\n",
                        qtimer_secs(timer) / (ITERATIONS * MAXPARALLELISM));
                printf("\t = increment throughput: %19f increments/sec\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer));
                printf("\t = data throughput: %24g bytes/sec %s\n", rate,
                       human_readable_rate(rate));
                break;
            case 1:
                printf("%f,%g,",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
                break;
            case 2:
                printf("%f\n%g\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
                break;
        }
    }

    if (TEST_SELECTION & (1 << 8)) {
        if (!MACHINE_READABLE) {
            printf("\tBalanced streaming loop: ");
            fflush(stdout);
        }
        increments =
            (aligned_t *)calloc(MAXPARALLELISM * ITERATIONS,
                                sizeof(aligned_t));
        qtimer_start(timer);
        qt_loop_balance(0, MAXPARALLELISM * ITERATIONS, streaming_incr, NULL);
        qtimer_stop(timer);
        for (i = 0; i < MAXPARALLELISM * ITERATIONS; i++) assert(
                increments[i] == 1);
        free(increments);
        increments = NULL;

        rate =
            (ITERATIONS * MAXPARALLELISM * sizeof(aligned_t)) /
            qtimer_secs(timer);
        switch (MACHINE_READABLE) {
            case 0:
                printf("%19g secs (%u-threads %u iters)\n",
                       qtimer_secs(timer), shepherds,
                       (unsigned)(ITERATIONS * MAXPARALLELISM));
                iprintf("\t + average increment time: %17g secs\n",
                        qtimer_secs(timer) / (ITERATIONS * MAXPARALLELISM));
                printf("\t = increment throughput: %19f increments/sec\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer));
                printf("\t = data throughput: %24g bytes/sec %s\n", rate,
                       human_readable_rate(rate));
                break;
            case 1:
                printf("%f,%g,",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
                break;
            case 2:
                printf("%f\n%g\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
                break;
        }
    }

    if (TEST_SELECTION & (1 << 9)) {
        if (!MACHINE_READABLE) {
            printf("\tOver-subscribed streaming loop: ");
            fflush(stdout);
        }
        increments =
            (aligned_t *)calloc(MAXPARALLELISM * ITERATIONS,
                                sizeof(aligned_t));
        qtimer_start(timer);
        for (i = 0; i < MAXPARALLELISM; i++) {
            qthread_fork(incrstream, increments + (i * ITERATIONS), rets + i);
        }
        for (i = 0; i < MAXPARALLELISM; i++) {
            qthread_readFF(NULL, rets + i);
        }
        qtimer_stop(timer);
        for (i = 0; i < MAXPARALLELISM * ITERATIONS; i++) assert(
                increments[i] == 1);
        free(increments);
        increments = NULL;

        rate =
            (ITERATIONS * MAXPARALLELISM * sizeof(aligned_t)) /
            qtimer_secs(timer);
        switch (MACHINE_READABLE) {
            case 0:
                printf("%6g secs (%u-threads %u iters)\n", qtimer_secs(timer),
                       shepherds, (unsigned)(ITERATIONS * MAXPARALLELISM));
                iprintf("\t + average increment time: %17g secs\n",
                        qtimer_secs(timer) / (ITERATIONS * MAXPARALLELISM));
                printf("\t = increment throughput: %19f increments/sec\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer));
                printf("\t = data throughput: %24g bytes/sec %s\n", rate,
                       human_readable_rate(rate));
                break;
            case 1:
                printf("%f,%g,",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
                break;
            case 2:
                printf("%f\n%g\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
                break;
        }
    }

    if (TEST_SELECTION & (1 << 10)) {
        if (!MACHINE_READABLE) {
            printf("\tNon-atomic bal. streaming loop: ");
            fflush(stdout);
        }
        increments =
            (aligned_t *)calloc(MAXPARALLELISM * ITERATIONS,
                                sizeof(aligned_t));
        qtimer_start(timer);
        qt_loop_balance(0, MAXPARALLELISM * ITERATIONS, streaming_naincr,
                        NULL);
        qtimer_stop(timer);
        for (i = 0; i < MAXPARALLELISM * ITERATIONS; i++) assert(
                increments[i] == 1);
        free(increments);
        increments = NULL;

        rate =
            (ITERATIONS * MAXPARALLELISM * sizeof(aligned_t)) /
            qtimer_secs(timer);
        switch (MACHINE_READABLE) {
            case 0:
                printf("%6g secs (%u-threads %u iters)\n", qtimer_secs(timer),
                       shepherds, (unsigned)(ITERATIONS * MAXPARALLELISM));
                iprintf("\t + average increment time: %17g secs\n",
                        qtimer_secs(timer) / (ITERATIONS * MAXPARALLELISM));
                printf("\t = increment throughput: %19f increments/sec\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer));
                printf("\t = data throughput: %24g bytes/sec %s\n", rate,
                       human_readable_rate(rate));
                break;
            case 1:
                printf("%f,%g,",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
                break;
            case 2:
                printf("%f\n%g\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
                break;
        }
    }

    /* Diffracting Trees */
    if (TEST_SELECTION & 1 << 11) {
        if (!MACHINE_READABLE) {
            printf("\tDiffracting Trees: ");
            fflush(stdout);
        }
        init_prism();
        increments = (aligned_t *)calloc(MAXPARALLELISM, sizeof(aligned_t));
        qtimer_start(timer);
        qt_loop_balance(0, MAXPARALLELISM * ITERATIONS, diffract_incr,
                        increments);
        qtimer_stop(timer);
        {
            int foundmax = 0;
            for (size_t i = 0; i < MAXPARALLELISM; ++i) {
                if (increments[i] == MAXPARALLELISM * ITERATIONS) {
                    foundmax = 1;
                    break;
                }
            }
            assert(foundmax == 1);
        }
        free(increments);

        rate =
            (ITERATIONS * MAXPARALLELISM * sizeof(aligned_t)) /
            qtimer_secs(timer);
        switch (MACHINE_READABLE) {
            case 0:
                printf("%19g secs (%u-threads %u iters)\n",
                       qtimer_secs(timer), shepherds,
                       (unsigned)(ITERATIONS * MAXPARALLELISM));
                iprintf("\t + average increment time: %17g secs\n",
                        qtimer_secs(timer) / (ITERATIONS * MAXPARALLELISM));
                printf("\t = increment throughput: %19f increments/sec\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer));
                printf("\t = data throughput: %24g bytes/sec %s\n", rate,
                       human_readable_rate(rate));
                break;
            case 1:
                printf("%f,%g",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
                break;
            case 2:
                printf("%f\n%g\n",
                       (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer),
                       rate);
        }
    }
    if (MACHINE_READABLE == 1) {
        printf("\n");
    }

    qtimer_destroy(timer);
    free(rets);

    return 0;
}

/* vim:set expandtab */