#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include "argparsing.h"

static aligned_t greeter(void *arg)
{
    iprintf("Hello World! This is thread %i, running on shepherd %i, worker %i\n", qthread_id(), qthread_shep(), qthread_worker(NULL));

    return 0;
}

int main(int   argc,
         char *argv[])
{
    long       spawn = 0;
    aligned_t *rets  = NULL;

    assert(qthread_initialize() == 0);

    CHECK_VERBOSE();
    spawn = qthread_num_workers() * 2;
    NUMARG(spawn, "THREADS");

    iprintf("%i shepherds...\n", qthread_num_shepherds());
    iprintf("  %i threads total\n", qthread_num_workers());

    rets = malloc(sizeof(aligned_t) * spawn);
    assert(rets != NULL);

    for (long i = 0; i < spawn; i++) {
        int ret = qthread_fork(greeter, NULL, rets + i);
        assert(ret == QTHREAD_SUCCESS);
    }

    for (long i = 0; i < spawn; i++) {
        int ret = qthread_readFF(NULL, rets + i);
        assert(ret == QTHREAD_SUCCESS);
    }

    free(rets);

    return EXIT_SUCCESS;
}

/* vim:set expandtab */
