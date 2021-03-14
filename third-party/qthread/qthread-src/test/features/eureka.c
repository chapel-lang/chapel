#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include "argparsing.h"
#include "qthread_innards.h"
#include "qt_shepherd_innards.h"

static aligned_t t  = 1;
static aligned_t t2 = 1;
static aligned_t t3 = 1;

static aligned_t waiter_count = 0;

aligned_t alive[6];

static void alive_check(const size_t a, const size_t b, void *junk)
{
    return;
}

static aligned_t live_waiter(void *arg)
{
    const int assigned = (int)(intptr_t)arg;
    const int id = qthread_id();
    qt_team_critical_section(BEGIN);
    iprintf("live_waiter %i alive! id %i wkr %u team %u\n", assigned, id, qthread_readstate(CURRENT_UNIQUE_WORKER), qt_team_id());
    qt_team_critical_section(END);
    qthread_fill(&alive[assigned]);
    qthread_flushsc();
    qt_team_critical_section(BEGIN);
    iprintf("live_waiter filled %p\n", &alive[assigned]);
    qt_team_critical_section(END);
    while(t == 1) {
	COMPILER_FENCE;
    }
    qthread_incr(&waiter_count, 1);
    iprintf("live_waiter %i exiting! id %i wkr %u\n", assigned, id, qthread_readstate(CURRENT_UNIQUE_WORKER));

    return 0;
}

static aligned_t live_parent(void *arg)
{
    iprintf("live_parent alive!\n");
    qthread_empty(alive+0);
    qthread_empty(alive+1);
    qthread_fork_to(live_waiter, (void*)(intptr_t)0, &t3, 1);
    qthread_fork_to(live_waiter, (void*)(intptr_t)1, &t3, 2);
    iprintf("live_parent waiting on %p\n", &alive[1]);
    qthread_readFF(NULL, &alive[1]);
    iprintf("live_parent saw live_waiter 1 report in\n");
    iprintf("live_parent waiting on %p\n", &alive[0]);
    qthread_readFF(NULL, &alive[0]);
    iprintf("live_parent saw live_waiter 0 report in\n");
    qthread_feb_status(&alive[0]); // guarantee the other task is not still in the lock
    qthread_feb_status(&alive[1]); // guarantee the other task is not still in the lock
    iprintf("live_parent about to eureka...\n");
    qt_team_eureka();
    iprintf("live_parent still alive!\n");
    COMPILER_FENCE;
    t = 0;
    return 0;
}

static aligned_t live_waiter2(void *arg)
{
    const int assigned = (int)(intptr_t)arg;
    const int id = qthread_id();
    qt_team_critical_section(BEGIN);
    iprintf("live_waiter2 %i alive! id %i wkr %u\n", assigned, id, qthread_readstate(CURRENT_UNIQUE_WORKER));
    qt_team_critical_section(END);
    while(t == 1) {
	COMPILER_FENCE;
    }
    qthread_incr(&waiter_count, 1);
    qt_team_critical_section(BEGIN);
    iprintf("live_waiter2 %i exiting! id %i wkr %u\n", assigned, id, qthread_readstate(CURRENT_UNIQUE_WORKER));
    qt_team_critical_section(END);

    return 0;
}

static aligned_t live_parent2(void *arg)
{
    iprintf("live_parent2 alive! id = %u\n", qthread_id());
    qthread_fork(live_waiter2, (void*)(intptr_t)0, &t3);
    qthread_fork(live_waiter2, (void*)(intptr_t)1, &t3);
    qthread_fork(live_waiter2, (void*)(intptr_t)2, &t3);
    qthread_fork(live_waiter2, (void*)(intptr_t)3, &t3);
    qthread_fork(live_waiter2, (void*)(intptr_t)4, &t3);
    qthread_fork(live_waiter2, (void*)(intptr_t)5, &t3);
    iprintf("live_parent2 spawned all tasks\n");
    qthread_flushsc();
    iprintf("live_parent2 about to eureka...\n");
    qt_team_eureka();
    iprintf("live_parent2 still alive!\n");
    COMPILER_FENCE;
    t = 0;
    iprintf("live_parent2 exiting!\n");
    return 0;
}

static aligned_t live_parent3(void *arg)
{
    iprintf("live_parent3 alive! id = %u\n", qthread_id());
    qthread_fork(live_waiter2, (void*)(intptr_t)0, &t3);
    iprintf("live_parent3 spawned all tasks\n");
    qthread_flushsc();
    iprintf("live_parent3 about to eureka...\n");
    qt_team_eureka();
    iprintf("live_parent3 still alive!\n");
    COMPILER_FENCE;
    t = 0;
    iprintf("live_parent3 exiting!\n");
    return 0;
}

static aligned_t live_parent_waiter(void *arg)
{
    const int assigned = (int)(intptr_t)arg;
    const int id = qthread_id();

    qthread_fork_to(live_waiter, (void*)(intptr_t)1, &t3, 2);
    iprintf("live_parent_waiter %i alive! id %i wkr %u team %u\n", assigned, id, qthread_readstate(CURRENT_UNIQUE_WORKER), qt_team_id());
    qthread_fill(&alive[assigned]);
    qthread_flushsc();
    qt_team_critical_section(BEGIN);
    iprintf("live_parent_waiter filled %p\n", &alive[assigned]);
    qt_team_critical_section(END);
    while(t == 1) {
	COMPILER_FENCE;
    }
    qthread_incr(&waiter_count, 1);
    iprintf("live_parent_waiter %i exiting! id %i wkr %u\n", assigned, id, qthread_readstate(CURRENT_UNIQUE_WORKER));

    return 0;
}

static aligned_t parent_eureka(void *arg)
{
    iprintf("parent_eureka alive! id = %u teamid = %u\n", qthread_id(), qt_team_id());
    qthread_empty(alive+0);
    qthread_empty(alive+1);
    qthread_spawn(live_parent_waiter, (void*)(intptr_t)0, 0, NULL, 0, &t3, 1, QTHREAD_SPAWN_NEW_SUBTEAM);
    iprintf("parent_eureka waiting on %p\n", &alive[0]);
    qthread_readFF(NULL, &alive[0]);
    iprintf("parent_eureka saw child 0 report in\n");
    iprintf("parent_eureka waiting on %p\n", &alive[1]);
    qthread_readFF(NULL, &alive[1]);
    iprintf("parent_eureka saw child 1 report in\n");
    qthread_feb_status(&alive[0]); // guarantee the other task is not still in the lock
    qthread_feb_status(&alive[1]); // guarantee the other task is not still in the lock
    iprintf("parent_eureka about to eureka...\n");
    qt_team_eureka();
    iprintf("parent_eureka still alive!\n");
    COMPILER_FENCE;
    t = 0;
    return 0;
}

aligned_t return_one(void *arg)
{
    return 1;
}

void clear_workers(void)
{
    iprintf("Checking that all workers are now clear again...\n");
    {
	aligned_t rets[3] = {0,0,0};
	for (int i=0; i<3; ++i) {
	    qthread_fork_to(return_one, NULL, &rets[i], i);
	}
	for (int i=0; i<3; ++i) {
	    aligned_t test;
	    qthread_readFF(&test, &rets[i]);
	    assert(test == 1);
	}
    }
    iprintf("...clear!\n");
}

int main(int   argc,
         char *argv[])
{
    int ret = 0;
    char stack_size[20];

    snprintf(stack_size, 20, "%u", 4096*2); // XXX: The root cause of this is hard to track down
    setenv("QT_STACK_SIZE", stack_size, 1);
    ret = qthread_init(3);
    if (ret != QTHREAD_SUCCESS) {
	fprintf(stderr, "initialization error\n");
	abort();
    }

    CHECK_VERBOSE();

    iprintf("%i shepherds...\n", qthread_num_shepherds());
    iprintf("  %i threads total\n", qthread_num_workers());

    qt_loop_balance(0, qthread_num_workers(), alive_check, NULL);

    iprintf("\n***************************************************************\n");
    iprintf("Testing a fully-live eureka (all member tasks running)...\n");
    qthread_spawn(live_parent, NULL, 0, &t2, 0, &t3, 0, QTHREAD_SPAWN_NEW_TEAM);
    qthread_readFF(NULL, &t2);
    assert(waiter_count == 0);
    t = 1;

    clear_workers();

    iprintf("\n\n***************************************************************\n");
    iprintf("Testing an over-subscribed eureka (some member tasks not running)...\n");
    qthread_fork_new_team(live_parent2, NULL, &t2);
    qthread_readFF(NULL, &t2);
    iprintf("main() woke up after live_parent2\n");
    assert(waiter_count == 0);
    t = 1;

    clear_workers();

    iprintf("\n\n***************************************************************\n");
    iprintf("Testing a under-subscribed eureka (some workers idle)...\n");
    qthread_fork_new_team(live_parent3, NULL, &t2);
    qthread_readFF(NULL, &t2);
    iprintf("main() woke up after live_parent3 (%u waiters survived)\n", waiter_count);
    assert(waiter_count == 0);
    t = 1;

    clear_workers();

    iprintf("\n\n***************************************************************\n");
    iprintf("Testing a parent-team eureka (subteam tasks must die)...\n");
    /* Ordinarily, spawning to specific shepherds would be unnecessary, but in
     * this case, I'm doing it to ensure that the situation I intend to test
     * actually happens. */
    qthread_fork_new_team_to(parent_eureka, NULL, &t2, 0);
    qthread_readFF(NULL, &t2);
    iprintf("main() woke up after parent_eureka\n");
    assert(waiter_count == 0);
    t = 1;

    clear_workers();

    iprintf("Success!\n");

    return 0;
}

/* vim:set expandtab */
