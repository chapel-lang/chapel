#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include "argparsing.h"

#include <inttypes.h>

/* More or less, this code matches the prodCons.chpl code from the open-source
 * Chapel compiler. This is a demonstration generated in an attempt to figure
 * out whether a given race condition is in qthreads on in Chapel */

static uint64_t bufferSize = 1024;      /* size of the circular buffer */
static uint64_t numItems;       /* number of items to write to buffer */

/*
 * Circular buffer of synchronization variables, which store
 * full/empty state in addition to their numeric values. By default, reads to
 * these variables will block until they are "full", leaving them "empty".
 * Writes will block until "empty", leaving "full".
 */
static aligned_t *buff = NULL;

/*
 * the producer loops over the requested number of items, writing them to the
 * buffer starting at location 0 and wrapping around when it hits the end of
 * the buffer. It then writes the value -1 as a sentinel to the next position.
 */
static aligned_t producer(void *arg)
{
    for (unsigned int i = 0; i < numItems; ++i) {
        const unsigned int buffInd = i % bufferSize;
        qthread_writeEF_const(&buff[buffInd], i);
        iprintf("producer wrote value #%u\n", i);
    }
    qthread_writeEF_const(&buff[numItems % bufferSize], -1);
    iprintf("producer wrote terminus value #%"PRIu64"\n", -1);

    return 0;
}

/*
 * The readFromBuff() iterator simply reads values from the shared buffer
 * starting at the 0th position and yields them.
 */
static int64_t readFromBuff(void)
{
    static unsigned int ind = 0;
    saligned_t nextVal;

    qthread_readFE((aligned_t*)&nextVal, &buff[ind]);
    if (nextVal != -1) {
        ind = (ind + 1) % bufferSize;
    }
    return nextVal;
}

/*
 * the consumer invokes an iterator to control its loop and yield values from
 * the shared buffer. It writes them out to the console.
 */
static aligned_t consumer(void *arg)
{
    int64_t buffVal;

    while ((buffVal = readFromBuff()) != -1) {
        iprintf("Consumer got: %li\n", (long)buffVal);
    }

    return 0;
}

/*
 * The main procedure simply creates a producer and a consumer task to run in
 * parallel
 */
int main(int argc,
         char *argv[])
{
    aligned_t t[2];

    assert(qthread_initialize() == 0);

    CHECK_VERBOSE();
    NUMARG(bufferSize, "BUFFERSIZE");
    numItems = 8 * bufferSize;
    NUMARG(numItems, "NUMITEMS");

    iprintf("%i threads...\n", qthread_num_shepherds());

    buff = malloc(sizeof(aligned_t) * bufferSize);
    for (unsigned int i = 0; i < bufferSize; ++i) {
        buff[i] = 0;
    }

    qthread_fork(consumer, NULL, &t[0]);
    qthread_fork(producer, NULL, &t[1]);
    qthread_readFF(NULL, &t[0]);
    qthread_readFF(NULL, &t[1]);

    /* cleanup... unnecessary in general, but for the moment I'm tracking down
     * errors in the FEB system, so let's clean up */
    for (unsigned int i = 0; i < bufferSize; ++i) {
	qthread_fill(buff + i);
    }

    free(buff);

    iprintf("Success!\n");

    return 0;
}

/* vim:set expandtab */
