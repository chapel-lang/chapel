#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include "qthread/qthread.h"
#include "argparsing.h"

static aligned_t x;
static aligned_t id      = 1;
static aligned_t readout = 0;
static aligned_t t;
static aligned_t doneforking;

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

static void *external_thread(void *junk)
{
    qthread_fork(consumer, NULL, NULL);
    qthread_fork(producer, NULL, &t);
    qthread_fill(&doneforking);
    return NULL;
}

int main(int   argc,
         char *argv[])
{
    pthread_t external;

    assert(qthread_initialize() == 0);

    x = 0;
    CHECK_VERBOSE();

    iprintf("%i threads...\n", qthread_num_shepherds());
    iprintf("Initial value of x: %lu\n", (unsigned long)x);

    qthread_empty(&doneforking);

    pthread_create(&external, NULL, external_thread, NULL);
    pthread_detach(external);

    qthread_readFF(NULL, &doneforking); // wait for the pthread to finish without using pthread_join();
    qthread_readFF(NULL, &t);           // wait for the consumer/producer pair to finish

    if (x == 55) {
        iprintf("Success! x==55\n");
        return 0;
    } else {
        fprintf(stderr, "Final value of x=%lu (expected 55)\n",
                (unsigned long)x);
        return -1;
    }
}

/* vim:set expandtab */
