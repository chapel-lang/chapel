#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include "argparsing.h"

static syncvar_t x = SYNCVAR_STATIC_INITIALIZER;
static syncvar_t id = SYNCVAR_STATIC_INITIALIZER;
//static uint64_t iterations = 10000;
static uint64_t iterations = 10;

static aligned_t consumer(void *arg)
{
    uint64_t me;

    iprintf("consumer locking id(%p)\n", &id);
    qthread_syncvar_readFE(&me, &id);
    iprintf("consumer id's status became: %u\n", qthread_syncvar_status(&id));
    iprintf("consumer unlocking id(%p), result is %lu\n", &id,
            (unsigned long)me);
    me++;
    qthread_syncvar_writeEF(&id, &me);

    for (uint64_t i = 0; i < iterations; ++i) {
        iprintf("consumer readFE on x\n");
        qthread_syncvar_readFE(NULL, &x);
    }

    iprintf("thread %i exiting\n", (int)(uintptr_t)arg);

    return 0;
}

static aligned_t producer(void *arg)
{
    uint64_t me;

    iprintf("producer locking id(%p)\n", &id);
    qthread_syncvar_readFE(&me, &id);
    iprintf("producer unlocking id(%p), result is %lu\n", &id,
            (unsigned long)me);
    me++;
    qthread_syncvar_writeEF(&id, &me);

    for (uint64_t i = 0; i < iterations; ++i) {
        iprintf("producer x's status is: %s (expect empty)\n",
                qthread_syncvar_status(&x) ? "full" : "empty");
        iprintf("producer filling x(%p)\n", &x);
        qthread_syncvar_writeEF_const(&x, i);
    }
    iprintf("thread %i exiting\n", (int)(uintptr_t)arg);

    return 0;
}

int main(int argc,
         char *argv[])
{
    aligned_t *t[2];
    uint64_t x_value;

    uint64_t pairs;

    assert(qthread_initialize() == 0);
    pairs = qthread_num_shepherds() * 6;

    CHECK_VERBOSE();
    NUMARG(iterations, "ITERATIONS");
    NUMARG(pairs, "PAIRS");

    t[0] = calloc(pairs, sizeof(aligned_t));
    t[1] = calloc(pairs, sizeof(aligned_t));

    iprintf("%i threads...\n", qthread_num_shepherds());
    iprintf("Initial value of x: %lu\n", (unsigned long)x.u.w);

    qthread_syncvar_empty(&id);
    qthread_syncvar_writeF_const(&id, 1);
    iprintf("id = 0x%lx\n", (unsigned long)id.u.w);
    {
        uint64_t tmp = 0;
        qthread_syncvar_readFF(&tmp, &id);
        assert(tmp == 1);
    }
    iprintf("x's status is: %s (want full (and nowait))\n",
            qthread_syncvar_status(&x) ? "full" : "empty");
    assert(qthread_syncvar_status(&x) == 1);
    qthread_syncvar_readFE(NULL, &x);
    iprintf("x's status became: %s (want empty (and nowait))\n",
            qthread_syncvar_status(&x) ? "full" : "empty");
    assert(qthread_syncvar_status(&x) == 0);
    for (unsigned int i = 0; i < pairs; ++i) {
        qthread_fork(consumer, (void *)(uintptr_t)i, &(t[0][i]));
    }
    for (unsigned int i = 0; i < pairs; ++i) {
        qthread_fork(producer, (void *)(uintptr_t)(i + pairs), &(t[1][i]));
    }
    for (unsigned int i = 0; i < pairs; ++i) {
        qthread_readFF(NULL, &(t[0][i]));
        qthread_readFF(NULL, &(t[1][i]));
    }
    iprintf("shouldn't be blocking on x (current status: %s)\n",
            qthread_syncvar_status(&x) ? "full" : "empty");
    qthread_syncvar_fill(&x);
    iprintf("shouldn't be blocking on x (current status: %s)\n",
            qthread_syncvar_status(&x) ? "full" : "empty");
    qthread_syncvar_readFF(&x_value, &x);
    assert(qthread_syncvar_status(&x) == 1);

    free(t[0]);
    free(t[1]);

    if (x_value == iterations - 1) {
        iprintf("Success! x==%lu\n", (unsigned long)x_value);
        return 0;
    } else {
        fprintf(stderr, "Final value of x=%lu, expected %lu\n",
                (unsigned long)x_value, (unsigned long)(iterations - 1));
        return -1;
    }
}

/* vim:set expandtab */
