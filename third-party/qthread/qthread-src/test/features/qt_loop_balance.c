#ifdef HAVE_CONFIG_H
# include "config.h"                   /* for _GNU_SOURCE */
#endif
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <qthread/qloop.h>
#include "argparsing.h"

static aligned_t threads = 0;
static aligned_t numincrs = 1024;

static void sum(const size_t startat,
                const size_t stopat,
                void        *arg_)
{
    qthread_incr(&threads, stopat - startat);
}

int main(int   argc,
         char *argv[])
{
    assert(qthread_initialize() == QTHREAD_SUCCESS);
    CHECK_VERBOSE();
    NUMARG(numincrs, "NUM_INCRS");
    // future_init(128);
    iprintf("%i shepherds\n", qthread_num_shepherds());
    iprintf("%i threads\n", qthread_num_workers());

    qt_loop_balance(0, numincrs, sum, NULL);

    if (threads != numincrs) {
        iprintf("threads == %lu, not %lu\n", (unsigned long)threads, (unsigned long)numincrs);
    }
    assert(threads == numincrs);

    return 0;
}

/* vim:set expandtab */
