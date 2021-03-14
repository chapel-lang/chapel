#ifdef HAVE_CONFIG_H
# include "config.h" /* for _GNU_SOURCE */
#endif

#include <assert.h>
#include <stdio.h>
#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include "argparsing.h"

static aligned_t donecount = 0;

static aligned_t null_task(void *args_)
{
    return qthread_incr(&donecount, 1);
}

static void par_null_task(size_t start,
                          size_t stop,
                          void  *args_)
{}

int main(int   argc,
         char *argv[])
{
    uint64_t count = 524288; // 1048576;

    CHECK_VERBOSE();

    NUMARG(count, "MT_COUNT");
    assert(0 != count);

    assert(qthread_initialize() == 0);

    for (uint64_t i = 0; i < count; i++) {
        qthread_fork(null_task, NULL, NULL);
    }
    do {
	aligned_t tmp = donecount;
        qthread_yield();
	if (tmp != donecount) {
	    iprintf("donecount = %u, tmp = %u\n", (unsigned int)donecount, (unsigned)tmp);
	}
    } while (donecount != count);
    iprintf("manually spawned and sync'd %u threads\n", (unsigned)count);

    qt_loop(0, count, par_null_task, NULL);
    iprintf("automatically spawned and sync'd %u threads\n", (unsigned)count);

    return 0;
}

/* vim:set expandtab */
