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

    iprintf("consumer(%i) locking id(%p)\n", qthread_id(), &id);
    qthread_lock(&id);
    me = id++;
    iprintf("consumer(%i) unlocking id(%p), result is %i\n", qthread_id(),
            &id, me);
    qthread_unlock(&id);

    qthread_readFE(&readout, &x);

    return 0;
}

static aligned_t producer(void *arg)
{
    int me;

    iprintf("producer(%i) locking id(%p)\n", qthread_id(), &id);
    qthread_lock(&id);
    me = id++;
    iprintf("producer(%i) unlocking id(%p), result is %i\n", qthread_id(),
            &id, me);
    qthread_unlock(&id);

    iprintf("producer(%i) filling x(%p)\n", qthread_id(), &x);
    qthread_writeEF_const(&x, 55);

    return 0;
}

static int realmain(void)
{
    aligned_t t;

    iprintf("%i threads...\n", qthread_num_shepherds());
    iprintf("Initial value of x: %lu\n", (unsigned long)x);

    qthread_fork(consumer, NULL, NULL);
    qthread_fork(producer, NULL, &t);
    qthread_readFF(NULL, &t);

    if (x == 55) {
        iprintf("Success! x==55\n");
        return 0;
    } else {
        fprintf(stderr, "Final value of x=%lu\n", (unsigned long)x);
        return -1;
    }
}

int main(int argc,
         char *argv[])
{
    assert(qthread_initialize() == 0);

    x = 0;
    CHECK_VERBOSE();

    iprintf("initialized, calling realmain()\n");
    realmain();
    iprintf("finalizing...\n");
    qthread_finalize();
    iprintf("ready to reinitialize!\n");
    qthread_init(1);
    iprintf("reinitialized, calling realmain()\n");
    realmain();
    iprintf("finalizing...\n");
    qthread_finalize();
    iprintf("exiting!\n");
    return 0;
}

/* vim:set expandtab */
