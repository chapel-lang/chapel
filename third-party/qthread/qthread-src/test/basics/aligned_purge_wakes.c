#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include "argparsing.h"

static aligned_t concurrent_t;
static aligned_t writeEF_wrapper(void *arg)
{
    iprintf("2: writeEF wrapper started\n");
    qthread_writeEF_const(&concurrent_t, 55);
    iprintf("2: writeEF completed\n");
    return 0;
}

// Test that purge wakes a blocked writeEF
// Requires that only one worker is running. Basically does:
//     1: fork(writeEF)
//     1: yields
//     2: starts runnning
//     2: hits writeEF, and yields since var is full
//     1: purge
//     1: hits readFF on forked task and yields
//     2: running again, finishes writeEF, task returns
//     1: readFF competes, finishes
static void testPurgeWakes(void)
{
    aligned_t ret;
    concurrent_t=45;
    assert(qthread_num_workers() == 1);

    iprintf("1: Forking writeEF wrapper\n");
    qthread_fork_to(writeEF_wrapper, NULL, &ret, qthread_shep());
    iprintf("1: Forked, now yielding to 2\n");
    qthread_yield();
    iprintf("1: Back from yield\n");

    // verify that writeEF has not completed
    assert(qthread_feb_status(&concurrent_t) == 1);
    assert(concurrent_t != 55);

    iprintf("1: purging\n");
    qthread_purge(&concurrent_t);

    // wait for writeEF wrapper to complete
    qthread_readFF(NULL, &ret);

    // veify that writeEF completed and that FEB is full
    iprintf("1: concurrent_t=%d\n", concurrent_t);
    assert(qthread_feb_status(&concurrent_t) == 1);
    assert(concurrent_t == 55);
}


int main(int argc,
         char *argv[])
{
    CHECK_VERBOSE();
    assert(qthread_init(1) == 0);
    iprintf("%i shepherds...\n", qthread_num_shepherds());
    iprintf("  %i threads total\n", qthread_num_workers());

    testPurgeWakes();

    return 0;
}

/* vim:set expandtab */
