#include <stdio.h>
#include <assert.h>
#include <qthread/qthread.h>
#include "argparsing.h"

static aligned_t whereami(void *arg)
{
    return qthread_shep();
}

int main(int argc,
         char *argv[])
{
    unsigned int i;
    aligned_t rets[30];
    qthread_shepherd_id_t numsheps;

    qthread_initialize();
    numsheps = qthread_num_shepherds();

    for (i = 0; i < 30; i++) {
        qthread_fork(whereami, NULL, &(rets[i]));
    }
    for (i = 0; i < 30; i++) {
        qthread_readFF(NULL, rets + i);
    }
    for (i = 0; i < 30; i++) {
        if (rets[i] != i % numsheps) {
            fprintf(stderr, "rets[%u] = %u ->? %u\n", i,
                    (unsigned int)rets[i], i % numsheps);
        }
        assert(rets[i] == i % numsheps);
    }
    iprintf("all %i shepherds can be queued to directly!\n", (int)numsheps);

    return 0;
}

/* vim:set expandtab */
