#ifdef HAVE_CONFIG_H
# include "config.h" /* for _GNU_SOURCE */
#endif

#include <assert.h>
#include <stdio.h>

#include <qthread/qthread.h>
#include "argparsing.h"

#include <qthread/qtimer.h>

static int start = 1;
static int stop  = 13;

static aligned_t donecount = 0;

static aligned_t test_async(void *args_)
{
    int depth = *(int *)args_;

    if (depth > 0) {
        --depth;
        for (int test_case = start; test_case <= stop; test_case++)
        {
            switch (test_case) {
                case 1:
                    qthread_incr(&donecount, 1);
                    qthread_fork_copyargs(test_async, &depth, sizeof(int), NULL);
                    break;
                case 2:
                    qthread_incr(&donecount, 1);
                    qthread_fork_copyargs_new_team(test_async, &depth, sizeof(int), NULL);
                    break;
                case 3:
                    qthread_incr(&donecount, 1);
                    qthread_fork_copyargs_new_subteam(test_async, &depth, sizeof(int), NULL);
                    break;
                case 4:
                    qthread_incr(&donecount, 2);
                    qthread_fork_copyargs(test_async, &depth, sizeof(int), NULL);
                    qthread_fork_copyargs(test_async, &depth, sizeof(int), NULL);
                    break;
                case 5:
                    qthread_incr(&donecount, 2);
                    qthread_fork_copyargs(test_async, &depth, sizeof(int), NULL);
                    qthread_fork_copyargs_new_team(test_async, &depth, sizeof(int), NULL);
                    break;
                case 6:
                    qthread_incr(&donecount, 2);
                    qthread_fork_copyargs(test_async, &depth, sizeof(int), NULL);
                    qthread_fork_copyargs_new_subteam(test_async, &depth, sizeof(int), NULL);
                    break;
                case 7:
                    qthread_incr(&donecount, 2);
                    qthread_fork_copyargs_new_team(test_async, &depth, sizeof(int), NULL);
                    qthread_fork_copyargs(test_async, &depth, sizeof(int), NULL);
                    break;
                case 8:
                    qthread_incr(&donecount, 2);
                    qthread_fork_copyargs_new_team(test_async, &depth, sizeof(int), NULL);
                    qthread_fork_copyargs_new_team(test_async, &depth, sizeof(int), NULL);
                    break;
                case 9:
                    qthread_incr(&donecount, 2);
                    qthread_fork_copyargs_new_team(test_async, &depth, sizeof(int), NULL);
                    qthread_fork_copyargs_new_subteam(test_async, &depth, sizeof(int), NULL);
                    break;
                case 10:
                    qthread_incr(&donecount, 2);
                    qthread_fork_copyargs_new_subteam(test_async, &depth, sizeof(int), NULL);
                    qthread_fork_copyargs(test_async, &depth, sizeof(int), NULL);
                    break;
                case 11:
                    qthread_incr(&donecount, 2);
                    qthread_fork_copyargs_new_subteam(test_async, &depth, sizeof(int), NULL);
                    qthread_fork_copyargs_new_team(test_async, &depth, sizeof(int), NULL);
                    break;
                case 12:
                    qthread_incr(&donecount, 2);
                    qthread_fork_copyargs_new_subteam(test_async, &depth, sizeof(int), NULL);
                    qthread_fork_copyargs_new_subteam(test_async, &depth, sizeof(int), NULL);
                    break;
                case 13:
                    qthread_incr(&donecount, 3);
                    qthread_fork_copyargs(test_async, &depth, sizeof(int), NULL);
                    qthread_fork_copyargs_new_team(test_async, &depth, sizeof(int), NULL);
                    qthread_fork_copyargs_new_subteam(test_async, &depth, sizeof(int), NULL);
                    break;
                default:
                    iprintf("No valid test case selected\n");
            }
        }
    }

    qthread_incr(&donecount, -1);

    return 0;
}

static aligned_t test(void *args_)
{
    int depth = *(int *)args_;
    aligned_t ret[3] = {0,0,0};

    if (depth > 0) {
        --depth;
        for (int test_case = start; test_case <= stop; test_case++)
        {
            switch (test_case) {
                case 1:
                    qthread_fork(test, &depth, &ret[0]);
                    qthread_readFF(NULL, &ret[0]);
                    break;
                case 2:
                    qthread_fork_new_team(test, &depth, &ret[0]);
                    qthread_readFF(NULL, &ret[0]);
                    break;
                case 3:
                    qthread_fork_new_subteam(test, &depth, &ret[0]);
                    qthread_readFF(NULL, &ret[0]);
                    break;
                case 4:
                    qthread_fork(test, &depth, &ret[0]);
                    qthread_readFF(NULL, &ret[0]);
                    qthread_fork(test, &depth, &ret[1]);
                    qthread_readFF(NULL, &ret[1]);
                    break;
                case 5:
                    qthread_fork(test, &depth, &ret[0]);
                    qthread_readFF(NULL, &ret[0]);
                    qthread_fork_new_team(test, &depth, &ret[1]);
                    qthread_readFF(NULL, &ret[1]);
                    break;
                case 6:
                    qthread_fork(test, &depth, &ret[0]);
                    qthread_readFF(NULL, &ret[0]);
                    qthread_fork_new_subteam(test, &depth, &ret[1]);
                    qthread_readFF(NULL, &ret[1]);
                    break;
                case 7:
                    qthread_fork_new_team(test, &depth, &ret[0]);
                    qthread_readFF(NULL, &ret[0]);
                    qthread_fork(test, &depth, &ret[1]);
                    qthread_readFF(NULL, &ret[1]);
                    break;
                case 8:
                    qthread_fork_new_team(test, &depth, &ret[0]);
                    qthread_readFF(NULL, &ret[0]);
                    qthread_fork_new_team(test, &depth, &ret[1]);
                    qthread_readFF(NULL, &ret[1]);
                    break;
                case 9:
                    qthread_fork_new_team(test, &depth, &ret[0]);
                    qthread_readFF(NULL, &ret[0]);
                    qthread_fork_new_subteam(test, &depth, &ret[1]);
                    qthread_readFF(NULL, &ret[1]);
                    break;
                case 10:
                    qthread_fork_new_subteam(test, &depth, &ret[0]);
                    qthread_readFF(NULL, &ret[0]);
                    qthread_fork(test, &depth, &ret[1]);
                    qthread_readFF(NULL, &ret[1]);
                    break;
                case 11:
                    qthread_fork_new_subteam(test, &depth, &ret[0]);
                    qthread_readFF(NULL, &ret[0]);
                    qthread_fork_new_team(test, &depth, &ret[1]);
                    qthread_readFF(NULL, &ret[1]);
                    break;
                case 12:
                    qthread_fork_new_subteam(test, &depth, &ret[0]);
                    qthread_readFF(NULL, &ret[0]);
                    qthread_fork_new_subteam(test, &depth, &ret[1]);
                    qthread_readFF(NULL, &ret[1]);
                    break;
                case 13:
                    qthread_fork(test, &depth, &ret[0]);
                    qthread_readFF(NULL, &ret[0]);
                    qthread_fork_new_team(test, &depth, &ret[1]);
                    qthread_readFF(NULL, &ret[1]);
                    qthread_fork_new_subteam(test, &depth, &ret[2]);
                    qthread_readFF(NULL, &ret[2]);
                    break;
                default:
                    iprintf("No valid test case selected\n");
            }
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    qtimer_t timer;

    int depth       = 2;
    int sync_tests  = 1;
    int async_tests = 1;

    CHECK_VERBOSE();

    NUMARG(depth, "DEPTH");
    NUMARG(start, "START");
    NUMARG(stop, "STOP");
    NUMARG(sync_tests, "SYNC_TESTS");
    NUMARG(async_tests, "ASYNC_TESTS");

    iprintf("=== Test Subteams ===\n");

    assert(qthread_initialize() == 0);

    timer = qtimer_create();

    // Run synchronized tests
    if (sync_tests) {
        qtimer_start(timer);
        test(&depth);
        qtimer_stop(timer);
        iprintf("Ran synchronized tests in   %f sec\n", 
                qtimer_secs(timer));
    }

    // Run unsynchronized tests
    if (async_tests) {
        qtimer_start(timer);
        qthread_incr(&donecount, 1);
        test_async(&depth);
        while (donecount) {
            qthread_yield();
        }
        qtimer_stop(timer);
        iprintf("Ran unsynchronized tests in %f sec\n", 
                qtimer_secs(timer));
    }

    qtimer_destroy(timer);

    return 0;
}
