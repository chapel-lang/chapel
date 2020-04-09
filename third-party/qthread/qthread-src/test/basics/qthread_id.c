#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qthread/qthread.h>
#include "argparsing.h"

aligned_t counter = 0;

static aligned_t thread(void *arg)
{
    int id = qthread_id();
    int ret;
    int ret2;

    // printf("first id = %i\n", id);
    if (id != 2) {
        iprintf("id == %i (expected 2)\n", id);
    }
    assert(id == 2);

    ret = qthread_incr(&counter, 1);
    // printf("first inc = %i\n", ret);
    assert(ret == 0);

    ret2 = qthread_incr(&counter, 1);
    // printf("second inc = %i\n", ret2);
    assert(ret2 == 1);
    return ret ^ ret2;
}

static aligned_t checkid(void *arg)
{
    int id = qthread_id();
    int want = (int)(intptr_t)arg;

    if (id != want) {
        iprintf("id == %i (expected %i)\n", id, want);
    }
    assert(id == want);
    return 0;
}

int main(int argc,
         char *argv[])
{
    aligned_t ret;
    aligned_t *rets;
    int my_id;
    size_t i;

    qthread_init(1);
    CHECK_VERBOSE();
    iprintf("Alive! Checking my id (expecting ID 1)\n");
    my_id = qthread_id();
    iprintf("My id is %i\n", my_id);
    if (my_id != 1) {
        fprintf(stderr, "my_id == %i (expected 1)\n", my_id);
    }
    assert(my_id == 1);
    qthread_fork(thread, NULL, &ret);
    qthread_readFF(NULL, &ret);
    rets = (aligned_t *)malloc(sizeof(aligned_t) * qthread_num_shepherds());
    for (i = 0; i < qthread_num_shepherds(); i++) {
        qthread_fork(checkid, (void *)(intptr_t)(i + 3), rets + i);
    }
    for (i = 0; i < qthread_num_shepherds(); i++) {
        qthread_readFF(NULL, rets + i);
    }
    free(rets);
    iprintf("success!\n");
    return 0;
}

/* vim:set expandtab */
