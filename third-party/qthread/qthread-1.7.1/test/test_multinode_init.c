#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qthread/qthread.h>
#include <qthread/qthread_multinode.h>

int my_id, world_size;

static aligned_t
returner(void *arg)
{
    fprintf(stderr, "(%03d) returner called, returning %ld\n", 
            my_id, *((long*)arg));
    return *((long*)arg);
}

int main(int argc,
         char *argv[])
{
    aligned_t tmp, ret = 0;
    int retval;
    long foobar = 1234567890;

    qthread_init(1);

    my_id = qthread_multinode_rank();
    world_size = qthread_multinode_size();

    fprintf(stderr, "(%03d) Rank %d of %d is alive\n", my_id, my_id, world_size);

    retval = qthread_multinode_register(2, returner);
    if (retval != 0){
        fprintf(stderr, "(%03d) multinode_register returned %d\n", my_id, retval);
        return 1;
    }
    
    qthread_multinode_run();
    if (my_id != 0) return 2;

    retval = qthread_fork_remote(returner, &foobar, &ret, 1, sizeof(long));
    if (retval != 0) {
        fprintf(stderr, "(%03d) fork_remote returned %d\n", my_id, retval);
        return 3;
    }

    retval = qthread_readFE(&tmp, &ret);
    fprintf(stderr, "(%03d) returner returned %ld\n", my_id, (long) tmp);
    if (retval != 0) {
        fprintf(stderr, "(%03d) readFE returned %d (%d)\n", my_id, retval, (int) tmp);
        return 4;
    }

    qthread_finalize();

    return (tmp == foobar) ? 0 : 5;
}

/* vim:set expandtab */
