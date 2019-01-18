#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include "argparsing.h"

#define TEST_OPTION(option, opr, val) do { \
    size_t v = qthread_readstate(option); \
    iprintf( "%s: " #option " = %zu, expect " #opr " %zu\n", (v opr val)?"GOOD":" BAD", v, val); \
    assert(v opr val); \
} while (0)

aligned_t spinner(void *arg)
{
    while (*(aligned_t*)arg == 0) ;
    return 1;
}

int main(int   argc,
         char *argv[])
{
    aligned_t return_value = 0;
    int status;

    CHECK_VERBOSE(); // part of the testing harness; toggles iprintf() output

    status = qthread_initialize();
    assert(status == QTHREAD_SUCCESS);

    iprintf("%i shepherds...\n", qthread_num_shepherds());
    iprintf("  %i threads total\n", qthread_num_workers());

    size_t output;

    TEST_OPTION(STACK_SIZE, >=, 2048);
    {
	aligned_t r = 0;
	TEST_OPTION(BUSYNESS, ==, 1); // Just this thread
	TEST_OPTION(NODE_BUSYNESS, ==, 1);
	TEST_OPTION(WORKER_OCCUPATION, ==, 1);
	qthread_fork(spinner, &r, &r);
	qthread_flushsc();
	TEST_OPTION(BUSYNESS, >=, 1);
	TEST_OPTION(BUSYNESS, <=, 2);
	/* This is <= instead of == because spinner might be dequeued but not
	 * yet executing */
	TEST_OPTION(NODE_BUSYNESS, <=, 2);
	TEST_OPTION(WORKER_OCCUPATION, >=, 1);
	TEST_OPTION(WORKER_OCCUPATION, <=, 2);
	r = 1;
	qthread_readFF(NULL, &r);
    }
    {
	size_t sheps;
	TEST_OPTION(TOTAL_SHEPHERDS, >=, 1);
	sheps = qthread_readstate(TOTAL_SHEPHERDS);
	assert(sheps >= 1);
	TEST_OPTION(ACTIVE_SHEPHERDS, >=, 1);
	TEST_OPTION(ACTIVE_SHEPHERDS, <=, sheps);
    }
    {
	size_t wkrs;
	TEST_OPTION(TOTAL_WORKERS, >=, 1);
	wkrs = qthread_readstate(TOTAL_WORKERS);
	assert(wkrs >= 1);
	TEST_OPTION(ACTIVE_WORKERS, >=, 1);
	TEST_OPTION(ACTIVE_WORKERS, <=, wkrs);
    }
    TEST_OPTION(CURRENT_SHEPHERD, ==, 0); // maybe this will change someday
    TEST_OPTION(CURRENT_WORKER, ==, 0); // maybe this will change someday
    TEST_OPTION(CURRENT_UNIQUE_WORKER, ==, 0); // maybe this will change someday
    TEST_OPTION(CURRENT_TEAM, ==, QTHREAD_DEFAULT_TEAM_ID);
    TEST_OPTION(PARENT_TEAM, ==, QTHREAD_NON_TEAM_ID);

    return EXIT_SUCCESS;
}

/* vim:set expandtab */
