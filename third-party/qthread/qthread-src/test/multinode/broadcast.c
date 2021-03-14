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
static aligned_t count = 1;
static aligned_t donecount = 0;
static int msgs = 1;

static int size;
static int rank;

static aligned_t pong(void *arg)
{
    iprintf("pong from %d\n", rank);
    qthread_incr(&donecount, 1);

    return 0;
}

static aligned_t ping(void *arg)
{
    iprintf("ping donecout %d msgs %d\n", donecount, msgs);
    if (msgs-1 == qthread_incr(&donecount, 1)) {
        iprintf("\tping donecout %d msgs %d\n", donecount, msgs);
        qthread_fork_remote(pong, NULL, NULL, 0, 0);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    qtimer_t timer;
    qthread_f funcs[3] = {ping, pong, NULL};

    CHECK_VERBOSE();

    spr_init(SPR_SPMD, funcs);

    size = qthread_multinode_size();
    rank = qthread_multinode_rank();
    iprintf("rank %d size %d\n", rank, size);

    NUMARG(count, "COUNT");

    if (size < 2) {
        printf("Need more than one locale. Skipping test.\n");
        return 0;
    }

    msgs = count / (size-1);

    assert(spr_unify() == 0);

    NUMARG(payload_size, "SIZE");
    if (payload_size > 0) {
        payload = calloc(payload_size, sizeof(aligned_t));
        assert(payload);
    }

    timer = qtimer_create();

    int next = 1;
    qtimer_start(timer);
    for (int i = 0; i < msgs*(size-1); i++) {
        iprintf("Spawned task %d to %d\n", i, next);
        qthread_fork_remote(ping, payload, NULL, next, payload_size);
        next = (next+1 == size) ? 1 : next+1;
    }
    iprintf("Waiting ...\n");
    while (donecount < size-1) {
        qthread_yield();
    }
    iprintf("Done.\n");
    qtimer_stop(timer);

    iprintf("exec_time %f\n", qtimer_secs(timer));
    iprintf("msg_rate %f\n", (msgs*(size-1))/qtimer_secs(timer));

    return 0;
}

