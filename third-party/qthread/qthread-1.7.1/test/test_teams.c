#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include "argparsing.h"

#define MAX(a,b) a >= b ? a : b

static aligned_t hello_new_team(void *arg_) {
    qt_team_id_t id = qt_team_id();

    iprintf("`hello_new_team` executing in team %lu\n", 
        (unsigned long)id);

    return id;
}

static aligned_t hello_in_team(void *arg_) {
    qt_team_id_t id = qt_team_id();

    iprintf("`hello_in_team` executing in team %lu\n", 
        (unsigned long)id);

    return id;
}

static aligned_t hello_new_team_in_team(void *arg_) {
    qt_team_id_t id = qt_team_id();

    iprintf("`hello_new_team_in_team` executing in team %lu\n", 
        (unsigned long)id);

    aligned_t ret;
    qthread_fork(hello_in_team, NULL, &ret);
    qthread_readFF(&ret, &ret);

    return MAX(id, ret);
}

static aligned_t hello_new_team_new_team(void *arg_) {
    qt_team_id_t id = qt_team_id();

    iprintf("`hello_new_team_new_team` executing in team %lu\n", 
        (unsigned long)id);

    aligned_t ret;
    qthread_fork_new_team(hello_new_team, NULL, &ret);
    qthread_readFF(&ret, &ret);

    return MAX(id, ret);
}

// //////////////////////////////////////////////////////////////////////////////
int main(int   argc,
         char *argv[])
{
    int count = 0;

    aligned_t max = 0;
    aligned_t tmp = 0;

    assert(qthread_initialize() == 0);

    CHECK_VERBOSE();
    NUMARG(count, "COUNT");

    iprintf("Main executing in team %lu\n", (unsigned long)qt_team_id());

    aligned_t hello_in_team_ret;
    qthread_fork(hello_in_team, NULL, &hello_in_team_ret);
    qthread_readFF(&tmp, &hello_in_team_ret);
    max = MAX(max, tmp);

    aligned_t hello_new_team_rets[count];
    for (int i = 0; i < count; i++) {
        qthread_fork_new_team(hello_new_team, NULL, &hello_new_team_rets[i]);
    }
    for (int i = 0; i < count; i++) {
        qthread_readFF(&tmp, &hello_new_team_rets[i]);
        max = MAX(max, tmp);
    }

    aligned_t hello_new_team_in_team_ret;
    qthread_fork_new_team(
        hello_new_team_in_team, NULL, &hello_new_team_in_team_ret);
    qthread_readFF(&tmp, &hello_new_team_in_team_ret);
    max = MAX(max, tmp);

    aligned_t hello_new_team_new_team_ret;
    qthread_fork_new_team(
        hello_new_team_new_team, NULL, &hello_new_team_new_team_ret);
    qthread_readFF(&tmp, &hello_new_team_new_team_ret);
    max = MAX(max, tmp);

    iprintf("max is %lu\n", (unsigned long)max);

    if (count + 3 == max) {
        iprintf("SUCCEEDED with count %lu and max team id %lu\n",
            (unsigned long)count,
            (unsigned long)max);
        return 0;
    } else {
        iprintf("FAILED with count %lu and max team id %lu\n",
            (unsigned long)count,
            (unsigned long)max);
        return 1;
    }
}

/* vim:set expandtab */
