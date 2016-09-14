#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include "argparsing.h"

static aligned_t checkres(void *arg)
{
    qthread_shepherd_id_t myshep = qthread_shep();

    assert(myshep == 1 || myshep == 0 || myshep == 2);

    if ((intptr_t)arg >= 0) {
        iprintf("checkres: myshep = %i, should be %i\n", myshep,
                (int)(intptr_t)arg);
        assert(myshep == (qthread_shepherd_id_t)(intptr_t)arg);
    } else {
        iprintf("checkres: myshep = %i, should NOT be %i\n", myshep,
                -1 * (int)(intptr_t)arg);
        assert(myshep != (qthread_shepherd_id_t)(-1 * (intptr_t)arg));
    }

    return 0;
}

static aligned_t migrant(void *arg)
{
    int myshep = qthread_shep();

    assert(myshep == 1 || myshep == 0);

    if (myshep == 1) {
        qthread_migrate_to(0);
        assert(qthread_shep() == 0);
    } else {
        qthread_migrate_to(1);
        iprintf("migrant starting on %i, aimed at 1, ended up on %i\n",
                myshep, qthread_shep());
        if (arg == (void *)2) {
            assert(qthread_shep() != 1);
        } else {
            assert(qthread_shep() == 1);
        }
    }

    return 0;
}

#ifdef __INTEL_COMPILER
int setenv(const char *name,
           const char *value,
           int overwrite);
#endif

int main(int argc,
         char *argv[])
{
    aligned_t ret;
    int qret;

    qthread_init(3);
    qthread_disable_shepherd(1);

    CHECK_VERBOSE();

    assert(qthread_readstate(TOTAL_SHEPHERDS) == 3);
    assert(qthread_readstate(ACTIVE_SHEPHERDS) == 2);
    iprintf("now to fork to shepherd 0...\n");
    qret = qthread_fork_to(checkres, (void *)0, &ret, 0);
    assert(qret == QTHREAD_SUCCESS);
    qthread_readFF(&ret, &ret);
    iprintf("\tsuccess in forking to shepherd 0!\n");
    iprintf("now to fork to shepherd 1...\n");
    qret = qthread_fork_to(checkres, (void *)-1, &ret, 1);
    iprintf("qret = %i\n", qret);
    qthread_readFF(&ret, &ret);
    iprintf("\tsuccessfully failed to fork to shepherd 1!\n");
    iprintf("now to fork to shepherd 2...\n");
    qret = qthread_fork_to(checkres, (void *)2, &ret, 2);
    assert(qret == QTHREAD_SUCCESS);
    qthread_readFF(&ret, &ret);
    iprintf("\tsuccess in forking to shepherd 2!\n");
    iprintf("now to fork the migrant...\n");
    qthread_fork_to(migrant, (void *)2, &ret, 0);
    iprintf("success in forking migrant!\n");
    qthread_readFF(&ret, &ret);
    iprintf("migrant returned successfully!\n");
    qthread_enable_shepherd(1);
    iprintf("now to fork the second migrant...\n");
    qthread_fork_to(migrant, (void *)1, &ret, 0);
    iprintf("success in forking second migrant!\n");
    qthread_readFF(&ret, &ret);
    iprintf("migrant returned successfully!\n");

    return 0;
}

/* vim:set expandtab */
