#include <assert.h>
#include <stdio.h>                     /* for printf() */
#include <stdlib.h>                    /* for strtol() */
#include <assert.h>                    /* for assert() */
#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

size_t ITERATIONS = 1000000;
size_t MAXPARALLELISM = 256;

syncvar_t *FEBbuffer;
aligned_t **FEBtable;

qtimer_t **sending;
double *total_sending_time;
double *total_roundtrip_time;
double *total_p1_sending_time;
double *total_p2_sending_time;

aligned_t incrementme = 0;

static aligned_t FEB_consumer(void *arg)
{
    uint64_t pong = 0;

    assert(qthread_syncvar_readFE(&pong, arg) == 0);
    if (pong != 1) {
        iprintf("ERROR: pong = %u\n", (unsigned)pong);
        assert(pong == 1);
    }
    return pong;
}

static aligned_t FEB_producer(void *arg)
{
    uint64_t ping = 1;

    assert(qthread_syncvar_writeEF(arg, &ping) == 0);
    return ping;
}

static aligned_t FEB_producerloop(void *arg)
{
    unsigned int offset = (unsigned int)(intptr_t)arg;
    uint64_t timer = 0;
    unsigned int i;

    for (i = 0; i < ITERATIONS; i++) {
        qtimer_start(sending[offset][timer]);
        assert(qthread_syncvar_writeEF(FEBbuffer + offset, &timer) == 0);
        timer = timer ? 0 : 1;
    }
    return 0;
}

static aligned_t FEB_consumerloop(void *arg)
{
    unsigned int offset = (unsigned int)(intptr_t)arg;
    uint64_t timer = 0;
    unsigned int i;

    for (i = 0; i < ITERATIONS; i++) {
        assert(qthread_syncvar_readFE(&timer, FEBbuffer + offset) == 0);
        qtimer_stop(sending[offset][timer]);
        total_sending_time[offset] += qtimer_secs(sending[offset][timer]);
    }
    return 0;
}

static aligned_t FEB_player2(void *arg)
{
    unsigned int offset = (unsigned int)(intptr_t)arg;
    aligned_t paddle = 0;
    unsigned int i;

    for (i = 0; i < ITERATIONS; i++) {
        assert(qthread_readFE(&paddle, FEBtable[offset]) == 0);
        qtimer_stop(sending[offset][0]);

        total_p1_sending_time[offset] += qtimer_secs(sending[offset][0]);

        qtimer_start(sending[offset][1]);
        assert(qthread_writeEF(FEBtable[offset] + 1, &paddle) == 0);
    }
    return 0;
}

static aligned_t FEB_player1(void *arg)
{
    unsigned int offset = (unsigned int)(intptr_t)arg;
    aligned_t paddle = 1;
    unsigned int i;
    qtimer_t roundtrip_timer = qtimer_create();

    /* serve */
    qtimer_start(sending[offset][0]);
    qtimer_start(roundtrip_timer);
    assert(qthread_writeEF(FEBtable[offset], &paddle) == 0);

    for (i = 0; i < ITERATIONS; i++) {
        assert(qthread_readFE(&paddle, FEBtable[offset] + 1) == 0);
        qtimer_stop(sending[offset][1]);
        qtimer_stop(roundtrip_timer);

        total_roundtrip_time[offset] += qtimer_secs(roundtrip_timer);
        total_p2_sending_time[offset] += qtimer_secs(sending[offset][1]);

        if (i + 1 < ITERATIONS) {
            qtimer_start(sending[offset][0]);
            qtimer_start(roundtrip_timer);
            assert(qthread_writeEF(FEBtable[offset], &paddle) == 0);
        }
    }
    qtimer_destroy(roundtrip_timer);
    return 0;
}

static char *human_readable_rate(double rate)
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

int main(int argc, char *argv[])
{
    qtimer_t timer = qtimer_create();
    double rate;
    unsigned int i;
    aligned_t rets[MAXPARALLELISM];

    /* setup */
    assert(qthread_initialize() == QTHREAD_SUCCESS);
    CHECK_VERBOSE();
    if (!verbose) {
        return 0;
    }
    NUMARG(ITERATIONS, "ITERATIONS");
    NUMARG(MAXPARALLELISM, "MAXPARALLELISM");

    FEBbuffer = calloc(MAXPARALLELISM, sizeof(syncvar_t));
    FEBtable = calloc(MAXPARALLELISM, sizeof(aligned_t *));
    sending = malloc(MAXPARALLELISM * sizeof(qtimer_t *));
    total_sending_time = malloc(MAXPARALLELISM * sizeof(double));
    total_roundtrip_time = malloc(MAXPARALLELISM * sizeof(double));
    total_p1_sending_time = malloc(MAXPARALLELISM * sizeof(double));
    total_p2_sending_time = malloc(MAXPARALLELISM * sizeof(double));
    for (i = 0; i < MAXPARALLELISM; i++) {
        qthread_syncvar_empty(FEBbuffer + i);
        sending[i] = malloc(2 * sizeof(qtimer_t));
        sending[i][0] = qtimer_create();
        sending[i][1] = qtimer_create();
        FEBtable[i] = malloc(sizeof(aligned_t) * 2);
        qthread_empty(&(FEBtable[i][0]));
        qthread_empty(&(FEBtable[i][1]));
    }
    printf("Testing producer/consumer (with %i sheps):\n",
           (int)qthread_num_shepherds());

    /* SINGLE FEB SEND/RECEIVE TEST */
    printf("\tSingle FEB send/receive: ");
    qtimer_start(timer);
    qthread_fork(FEB_consumer, (void*)FEBbuffer, rets);
    qthread_fork(FEB_producer, (void*)FEBbuffer, NULL);
    qthread_readFF(NULL, rets);
    qtimer_stop(timer);

    printf("%19g secs\n", qtimer_secs(timer));
    rate = sizeof(aligned_t) / qtimer_secs(timer);
    printf("\t = throughput: %29g bytes/sec %s\n", rate,
           human_readable_rate(rate));

    /* PARALLEL SINGLE FEB SEND/RECEIVE TEST */
    printf("\tParallel single FEB send/receive: ");
    qtimer_start(timer);
    for (i = 0; i < MAXPARALLELISM; i++) {
        qthread_fork(FEB_consumer, (void*)(FEBbuffer + i), rets + i);
        qthread_fork(FEB_producer, (void*)(FEBbuffer + i), NULL);
    }
    for (i = 0; i < MAXPARALLELISM; i++) {
        qthread_readFF(NULL, rets + i);
    }
    qtimer_stop(timer);

    printf("%10g secs (%u parallel)\n", qtimer_secs(timer),
           (unsigned)MAXPARALLELISM);
    rate = (MAXPARALLELISM * sizeof(aligned_t)) / qtimer_secs(timer);
    printf("\t = throughput: %29g bytes/sec %s\n", rate,
           human_readable_rate(rate));

    memset(total_sending_time, 0, sizeof(double) * MAXPARALLELISM);

    /* FEB PRODUCER/CONSUMER LOOP */
    printf("\tFEB producer/consumer loop: ");
    qtimer_start(timer);
    qthread_fork(FEB_consumerloop, NULL, rets);
    qthread_fork(FEB_producerloop, NULL, NULL);
    qthread_readFF(NULL, rets);
    qtimer_stop(timer);

    printf("%16g secs (%u iterations)\n", qtimer_secs(timer),
           (unsigned)ITERATIONS);
    printf("\t - total sending time: %21g secs\n", total_sending_time[0]);
    iprintf("\t + external average time: %18g secs\n",
            qtimer_secs(timer) / ITERATIONS);
    iprintf("\t + internal average time: %18g secs\n",
            total_sending_time[0] / ITERATIONS);
    printf("\t = message throughput: %21g msgs/sec\n",
           ITERATIONS / total_sending_time[0]);
    rate = (ITERATIONS * sizeof(aligned_t)) / total_sending_time[0];
    printf("\t = data throughput: %24g bytes/sec %s\n", rate,
           human_readable_rate(rate));

    memset(total_sending_time, 0, sizeof(double) * MAXPARALLELISM);

    /* PARALLEL FEB PRODUCER/CONSUMER LOOPS */
    printf("\tParallel FEB producer/consumer loop: ");
    qtimer_start(timer);
    for (i = 0; i < MAXPARALLELISM; i++) {
        qthread_fork(FEB_consumerloop, (void *)(intptr_t)i, rets + i);
    }
    for (i = 0; i < MAXPARALLELISM; i++) {
        qthread_fork(FEB_producerloop, (void *)(intptr_t)i, NULL);
    }
    for (i = 0; i < MAXPARALLELISM; i++) {
        qthread_readFF(NULL, rets + i);
    }
    qtimer_stop(timer);

    for (i = 1; i < MAXPARALLELISM; i++) {
        total_sending_time[0] += total_sending_time[i];
    }
    printf("%6g secs (%u-way %u iters)\n", qtimer_secs(timer),
           (unsigned)MAXPARALLELISM, (unsigned)ITERATIONS);
    printf("\t - total sending time: %21g secs\n", total_sending_time[0]);
    iprintf("\t + external average time: %18g secs\n",
            qtimer_secs(timer) / (ITERATIONS * MAXPARALLELISM));
    iprintf("\t + internal average time: %18g secs\n",
            total_sending_time[0] / (ITERATIONS * MAXPARALLELISM));
    printf("\t = message throughput: %21g msgs/sec\n",
           (ITERATIONS * MAXPARALLELISM) / qtimer_secs(timer));
    rate =
        (ITERATIONS * MAXPARALLELISM * sizeof(aligned_t)) /
        qtimer_secs(timer);
    printf("\t = data throughput: %24g bytes/sec %s\n", rate,
           human_readable_rate(rate));

    memset(total_p1_sending_time, 0, sizeof(double) * MAXPARALLELISM);
    memset(total_p2_sending_time, 0, sizeof(double) * MAXPARALLELISM);
    memset(total_roundtrip_time, 0, sizeof(double) * MAXPARALLELISM);

    /* FEB PING-PONG LOOP */
    printf("\tFEB ping-pong loop: ");
    qtimer_start(timer);
    qthread_fork(FEB_player2, NULL, rets);
    qthread_fork(FEB_player1, NULL, NULL);
    qthread_readFF(NULL, rets);
    qtimer_stop(timer);

    printf("%24g secs (%u round trips)\n", qtimer_secs(timer),
           (unsigned)ITERATIONS);
    printf("\t - total rtts: %29g secs\n", total_roundtrip_time[0]);
    printf("\t - total sending time: %21g secs\n",
           total_p1_sending_time[0] + total_p2_sending_time[0]);
    iprintf("\t + external avg rtt: %23g secs\n",
            qtimer_secs(timer) / ITERATIONS);
    iprintf("\t + internal avg rtt: %23g secs\n",
            total_roundtrip_time[0] / ITERATIONS);
    iprintf("\t + average p1 sending time: %16g secs\n",
            total_p1_sending_time[0] / ITERATIONS);
    iprintf("\t + average p2 sending time: %16g secs\n",
            total_p2_sending_time[0] / ITERATIONS);
    iprintf("\t + average sending time: %19g secs\n",
            (total_p1_sending_time[0] +
             total_p2_sending_time[0]) / (ITERATIONS * 2));
    /* each rt is 2 messages, thus: */
    printf("\t = messaging throughput: %19g msgs/sec\n",
           (ITERATIONS * 2) / total_roundtrip_time[0]);
    /* each rt is 1 message of aligned_t bytes each, thus: */
    rate = (ITERATIONS * sizeof(aligned_t)) / total_roundtrip_time[0];
    printf("\t = data roundtrip tput: %20g bytes/sec %s\n", rate,
           human_readable_rate(rate));
    /* each send is 1 messsage of aligned_t bytes, thus: */
    rate = (ITERATIONS * sizeof(aligned_t)) / total_p1_sending_time[0];
    printf("\t = p1 hop throughput: %22g bytes/sec %s\n", rate,
           human_readable_rate(rate));
    rate = (ITERATIONS * sizeof(aligned_t)) / total_p2_sending_time[0];
    printf("\t = p2 hop throughput: %22g bytes/sec %s\n", rate,
           human_readable_rate(rate));
    rate =
        (ITERATIONS * 2 * sizeof(aligned_t)) / (total_p1_sending_time[0] +
                                                total_p2_sending_time[0]);
    printf("\t = data hop throughput: %20g bytes/sec %s\n", rate,
           human_readable_rate(rate));

    memset(total_p1_sending_time, 0, sizeof(double) * MAXPARALLELISM);
    memset(total_p2_sending_time, 0, sizeof(double) * MAXPARALLELISM);
    memset(total_roundtrip_time, 0, sizeof(double) * MAXPARALLELISM);

    /* PARALLEL FEB PING-PONG LOOP */
    printf("\tParallel FEB ping-pong loop: ");
    qtimer_start(timer);
    for (i = 0; i < MAXPARALLELISM; i++) {
        qthread_fork(FEB_player2, (void *)(intptr_t)i, rets + i);
    }
    for (i = 0; i < MAXPARALLELISM; i++) {
        qthread_fork(FEB_player1, (void *)(intptr_t)i, NULL);
    }
    for (i = 0; i < MAXPARALLELISM; i++) {
        qthread_readFF(NULL, rets + i);
    }
    qtimer_stop(timer);

    for (i = 1; i < MAXPARALLELISM; i++) {
        total_roundtrip_time[0] += total_roundtrip_time[i];
        total_p1_sending_time[0] += total_p1_sending_time[i];
        total_p2_sending_time[0] += total_p2_sending_time[i];
    }
    printf("%15g secs (%u-way %u rts)\n", qtimer_secs(timer),
           (unsigned)MAXPARALLELISM, (unsigned)ITERATIONS);
    printf("\t - total rtts: %29g secs\n", total_roundtrip_time[0]);
    printf("\t - total sending time: %21g secs\n",
           total_p1_sending_time[0] + total_p2_sending_time[0]);
    iprintf("\t + external avg rtt: %23g secs\n",
            qtimer_secs(timer) / (MAXPARALLELISM * ITERATIONS));
    iprintf("\t + internal avg rtt: %23g secs\n",
            total_roundtrip_time[0] / (MAXPARALLELISM * ITERATIONS));
    iprintf("\t + average p1 sending time: %16g secs\n",
            total_p1_sending_time[0] / (MAXPARALLELISM * ITERATIONS));
    iprintf("\t + average p2 sending time: %16g secs\n",
            total_p2_sending_time[0] / (MAXPARALLELISM * ITERATIONS));
    iprintf("\t + average sending time: %19g secs\n",
            (total_p1_sending_time[0] +
             total_p2_sending_time[0]) / (MAXPARALLELISM * ITERATIONS * 2));
    /* each rt is 2 messages, thus: */
    printf("\t = messaging throughput: %19g msgs/sec\n",
           (MAXPARALLELISM * ITERATIONS * 2) / total_roundtrip_time[0]);
    /* each rt is 1 message of aligned_t bytes each, thus: */
    rate =
        (MAXPARALLELISM * ITERATIONS * sizeof(aligned_t)) /
        total_roundtrip_time[0];
    printf("\t = data roundtrip tput: %20g bytes/sec %s\n", rate,
           human_readable_rate(rate));
    /* each send is 1 messsage of aligned_t bytes, thus: */
    rate =
        (MAXPARALLELISM * ITERATIONS * sizeof(aligned_t)) /
        total_p1_sending_time[0];
    printf("\t = p1 hop throughput: %22g bytes/sec %s\n", rate,
           human_readable_rate(rate));
    rate =
        (MAXPARALLELISM * ITERATIONS * sizeof(aligned_t)) /
        total_p2_sending_time[0];
    printf("\t = p2 hop throughput: %22g bytes/sec %s\n", rate,
           human_readable_rate(rate));
    rate =
        (MAXPARALLELISM * ITERATIONS * 2 * sizeof(aligned_t)) /
        (total_p1_sending_time[0] + total_p2_sending_time[0]);
    printf("\t = data hop throughput: %20g bytes/sec %s\n", rate,
           human_readable_rate(rate));

    qtimer_destroy(timer);
    for (i = 0; i < MAXPARALLELISM; i++) {
        qtimer_destroy(sending[i][0]);
        qtimer_destroy(sending[i][1]);
        free(sending[i]);
        free(FEBtable[i]);
    }
    free((void*)FEBbuffer);
    free(FEBtable);
    free(sending);
    free(total_sending_time);
    free(total_roundtrip_time);
    free(total_p1_sending_time);
    free(total_p2_sending_time);

    return 0;
}

/* vim:set expandtab */