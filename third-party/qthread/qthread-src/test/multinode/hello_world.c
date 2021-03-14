#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include <qthread/spr.h>
#include <qthread/multinode.h>
#include "argparsing.h"

static aligned_t say_hello(void *arg)
{
    int const rank = qthread_multinode_rank();

    iprintf("Hello from locale %03d!\n", rank);

    return 0;
}

int main(int argc, char *argv[])
{
    CHECK_VERBOSE();

    qthread_f funcs[2] = {say_hello, NULL};
    assert(spr_init(0, funcs) == SPR_OK);

    int const size = spr_num_locales();
    aligned_t rets[size];

    for (int i = 0; i < size; i++)
        qthread_fork_remote(say_hello, NULL, &rets[i], i, 0);
    for (int i = 0; i < size; i++)
        qthread_readFF(NULL, &rets[i]);

    return 0;
}

