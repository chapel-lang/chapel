#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qthread/qthread.h>
#include <qthread/multinode.h>
#include "argparsing.h"

int my_id, world_size;

static aligned_t
returner(void *arg)
{
    iprintf("(%03d) returner called, returning %ld\n", 
            my_id, *((long*)arg));
    return *((long*)arg);
}

int main(int argc,
         char *argv[])
{
    CHECK_VERBOSE();

    aligned_t tmp, ret = 0;
    int retval;
    long foobar = 1234567890;

    setenv("QT_MULTINODE","yes",1);

    qthread_initialize();

    my_id = qthread_multinode_rank();
    world_size = qthread_multinode_size();

    iprintf("(%03d) Rank %d of %d is alive\n", my_id, my_id, world_size);

    retval = qthread_multinode_register(2, returner);
    if (retval != 0){
        fprintf(stderr, "(%03d) multinode_register returned %d\n", my_id, retval);
        return 1;
    }

    qthread_multinode_run();
    if (my_id != 0) return 2;

    int target = (world_size > 1) ? 1 : 0;
    retval = qthread_fork_remote(returner, &foobar, &ret, target, sizeof(long));
    if (retval != 0) {
        fprintf(stderr, "(%03d) fork_remote returned %d\n", my_id, retval);
        return 3;
    }

    retval = qthread_readFE(&tmp, &ret);
    iprintf("(%03d) returner returned %ld\n", my_id, (long) tmp);
    if (retval != 0) {
        fprintf(stderr, "(%03d) readFE returned %d (%d)\n", my_id, retval, (int) tmp);
        return 4;
    }

    qthread_finalize();

    return (tmp == foobar) ? 0 : 5;
}

/* vim:set expandtab */
