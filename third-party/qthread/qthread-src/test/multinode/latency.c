#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include <qthread/spr.h>
#include <qthread/multinode.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

static size_t payload_size = 0;
static aligned_t *payload = NULL;
static size_t count = 1;
static qtimer_t timer;

static int size;
static int rank;
static int next;
static aligned_t done;

static aligned_t intra_ping(void *arg)
{
    int shep = (int)qthread_shep();
    int next = (shep+1 == size) ? 0 : shep+1;

    iprintf("Ping shep %03d\n", shep);

    if (shep != 0) {
        qthread_fork_copyargs_to(intra_ping, payload, payload_size, NULL, next);
    } else if (count != 0) {
        count -= 1;
        qthread_fork_copyargs_to(intra_ping, payload, payload_size, NULL, 1);
    } else {
        qtimer_stop(timer);
        qthread_writeF_const(&done, 1);
    }

    return 0;
}

static aligned_t ping(void *arg)
{
    iprintf("Ping %03d\n", rank);

    if (rank != 0) {
        qthread_fork_remote(ping, payload, NULL, next, payload_size);
    } else if (count != 0) {
        count -= 1;
        qthread_fork_remote(ping, payload, NULL, 1, payload_size);
    } else {
        qtimer_stop(timer);
        qthread_writeF_const(&done, 1);
    }

    return 0;
}

static aligned_t intra_node_test(void *arg)
{
    qthread_empty(&done);
    qtimer_start(timer);
    qthread_fork_copyargs_to(intra_ping, payload, payload_size, NULL, 1);
    qthread_readFF(NULL, &done);

    return 0;
}

int main(int argc, char *argv[])
{
    CHECK_VERBOSE();

    qthread_f funcs[2] = {ping, NULL};
    assert(spr_init(SPR_SPMD, funcs) == SPR_OK);

    size = spr_num_locales();
    rank = spr_locale_id();
    next = (rank+1 == size) ? 0 : rank+1;
    iprintf("rank %d next %d size %d\n", rank, next, size);

    if (size < 2) {
        printf("Need more than one locale. Skipping test.\n");
        return 0;
    }

    assert(spr_unify() == 0);

    NUMARG(count, "COUNT");
    size_t total_count = count * size;
    count--;

    NUMARG(payload_size, "SIZE");
    if (payload_size > 0) {
        payload = calloc(payload_size, sizeof(aligned_t));
        assert(payload);
    }

    timer = qtimer_create();

    qthread_empty(&done);
    qtimer_start(timer);
    qthread_fork_remote(ping, payload, NULL, 1, payload_size);
    qthread_readFF(NULL, &done);

    double total_time = qtimer_secs(timer);
    iprintf("tot-time %f\n", total_time);
    iprintf("avg-time %f\n", total_time / total_count);

    if (qthread_num_shepherds() > 1) {
        // Run intra-node test
        count = total_count / size;
        int size = (int)qthread_num_shepherds();
        total_count = count * size;

        aligned_t ret;
        qthread_fork_to(intra_node_test, NULL, &ret, 0);
        qthread_readFF(NULL, &ret);

        total_time = qtimer_secs(timer);
        iprintf("intra-tot-time %f\n", total_time);
        iprintf("intra-avg-time %f\n", total_time / total_count);
    }

    return 0;
}

