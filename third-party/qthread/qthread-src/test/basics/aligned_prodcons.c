#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include "argparsing.h"

static aligned_t x;
static aligned_t id = 1;
static aligned_t readout = 0;

static aligned_t consumer(void *arg)
{
    int me;

    iprintf("consumer locking id(%p)\n", &id);
    qthread_lock(&id);
    me = id++;
    iprintf("consumer unlocking id(%p), result is %i\n", &id, me);
    qthread_unlock(&id);

    qthread_readFE(&readout, &x);

    return 0;
}

static aligned_t producer(void *arg)
{
    int me;

    iprintf("producer locking id(%p)\n", &id);
    qthread_lock(&id);
    me = id++;
    iprintf("producer unlocking id(%p), result is %i\n", &id, me);
    qthread_unlock(&id);

    iprintf("producer filling x(%p)\n", &x);
    qthread_writeEF_const(&x, 55);

    return 0;
}

int main(int argc,
         char *argv[])
{
    aligned_t t;
    int ret;

    assert(qthread_initialize() == 0);

    x = 0;
    CHECK_VERBOSE();

    iprintf("%i shepherds...\n", qthread_num_shepherds());
    iprintf("  %i threads total\n", qthread_num_workers());
    iprintf("Initial value of x: %lu\n", (unsigned long)x);

    ret = qthread_fork(consumer, NULL, NULL);
    assert(ret == QTHREAD_SUCCESS);
    ret = qthread_fork(producer, NULL, &t);
    assert(ret == QTHREAD_SUCCESS);
    ret = qthread_readFF(&t, &t);
    assert(ret == QTHREAD_SUCCESS);

    if (x == 55) {
        iprintf("Success! x==55\n");
        return 0;
    } else {
        fprintf(stderr, "Final value of x=%lu (expected 55)\n", (unsigned long)x);
        return -1;
    }
}

/* vim:set expandtab */
