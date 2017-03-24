#ifdef HAVE_CONFIG_H
# include "config.h" /* for _GNU_SOURCE */
#endif

#include <assert.h>
#include <stdio.h>
#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include <qthread/qtimer.h>
#include "argparsing.h"

#include <tbb/task.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

using namespace tbb;

class NullTask : public task
{
public:
    task *execute(void)
    {
        return NULL;
    }
};

class NullTaskSpawner : public task
{
public:
    const uint64_t count;
    NullTaskSpawner(uint64_t _count) :
        count(_count) {}
    task *execute(void)
    {
        set_ref_count(count); // note the lack of a +1
        // note the -2; this task counts as one of the ones we're spawning
        for (uint64_t i = 0; i < count - 2; i++) {
            NullTask&a = *new(tbb::task::allocate_child())NullTask();

            spawn(a);
        }
        {
            NullTask&a = *new(tbb::task::allocate_child())NullTask();

            spawn_and_wait_for_all(a);
        }
        return NULL;
    }
};

class NullTaskParSpawner : public task
{
    class NullTaskRangeSpawner
    {
        NullTaskParSpawner &outer;
public:
        void operator() (const blocked_range<uint64_t>&r) const
        {
            for ( uint64_t I = r.begin(); I != r.end(); ++I) {
                NullTask &a = *new(outer.allocate_child())NullTask();
                spawn(a);
            }
        }

        NullTaskRangeSpawner(NullTaskParSpawner &ref) : outer(ref) {}
    };
public:
    const uint64_t count;
    NullTaskParSpawner(uint64_t _count) :
        count(_count) {}
    task *execute(void)
    {
        set_ref_count(count); // note the lack of a +1
        // note the -2; this task counts as one of the ones we're spawning
        parallel_for(blocked_range<uint64_t>(0, count - 2),
                     NullTaskRangeSpawner(*this),
                     tbb::auto_partitioner());
        {
            NullTask&a = *new(allocate_child())NullTask();

            spawn_and_wait_for_all(a);
        }
        return NULL;
    }
};

int main(int   argc,
         char *argv[])
{
    unsigned long threads  = task_scheduler_init::default_num_threads();
    uint64_t      count    = 0;
    int           par_fork = 0;

    qtimer_t timer;
    double   total_time = 0.0;

    CHECK_VERBOSE();

    NUMARG(count, "MT_COUNT");
    NUMARG(par_fork, "MT_PAR_FORK");
    NUMARG(threads, "TBB_THREADS");
    assert(0 != count);

    timer = qtimer_create();

    {
        task_scheduler_init init(threads);

        if (par_fork) {
            qtimer_start(timer);

            // we do it this way because the main task cannot wait on multiple sub-tasks
            NullTaskParSpawner&a = *new(task::allocate_root())NullTaskParSpawner(count);

            task::spawn_root_and_wait(a);
        } else {
            qtimer_start(timer);

            // we do it this way because the main task cannot wait on multiple sub-tasks
            NullTaskSpawner&a = *new(task::allocate_root())NullTaskSpawner(count);

            task::spawn_root_and_wait(a);
        }

        qtimer_stop(timer);
    }

    total_time = qtimer_secs(timer);

    qtimer_destroy(timer);

    printf("%lu %lu %f\n",
           (unsigned long)threads,
           (unsigned long)count,
           total_time);

    return 0;
}

/* vim:set expandtab */
