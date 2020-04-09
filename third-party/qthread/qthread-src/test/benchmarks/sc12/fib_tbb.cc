#include <stdio.h>                     /* for printf() */
#include <assert.h>                    /* for assert() */
#include <qthread/qthread.h>
#include <qthread/qtimer.h>

#define SILENT_ARGPARSING
#include "argparsing.h"
#include "log.h"

#include "tbb/task.h"
#include <tbb/task_scheduler_init.h>

static aligned_t validation[] = {
    0,        // 0
    1,        // 1
    1,        // 2
    2,        // 3
    3,        // 4
    5,        // 5
    8,        // 6
    13,       // 7
    21,       // 8
    34,       // 9
    55,       // 10
    89,       // 11
    144,      // 12
    233,      // 13
    377,      // 14
    610,      // 15
    987,      // 16
    1597,     // 17
    2584,     // 18
    4181,     // 19
    6765,     // 20
    10946,    // 21
    17711,    // 22
    28657,    // 23
    46368,    // 24
    75025,    // 25
    121393,   // 26
    196418,   // 27
    317811,   // 28
    514229,   // 29
    832040,   // 30
    1346269,  // 31
    2178309,  // 32
    3524578,  // 33
    5702887,  // 34
    9227465,  // 35
    14930352, // 36
    24157817, // 37
    39088169  // 38
};

class FibTask : public tbb::task
{
public:
    const uint64_t  n;
    uint64_t *const sum;

    FibTask(uint64_t  n_,
            uint64_t *sum_) :
        n(n_), sum(sum_) {}

    task *execute()
    {
        if (n < 2) {
            *sum = n;
            return NULL;
        }
        uint64_t x, y;
        FibTask &a = *new(allocate_child())FibTask(n - 1, &x);
        FibTask &b = *new(allocate_child())FibTask(n - 2, &y);
        set_ref_count(3);
        spawn(b);
        spawn_and_wait_for_all(a);
        *sum = x + y;
        return NULL;
    }
};

uint64_t simple_parallel_fib(uint64_t n)
{
    uint64_t sum;
    FibTask& a = *new(tbb::task::allocate_root())FibTask(n, &sum);

    tbb::task::spawn_root_and_wait(a);
    return sum;
}

int main(int   argc,
         char *argv[])
{
    unsigned  threads = tbb::task_scheduler_init::default_num_threads();
    qtimer_t  timer   = qtimer_create();
    aligned_t n       = 20;
    aligned_t ret     = 0;

    /* setup */
    CHECK_VERBOSE();
    NUMARG(n, "FIB_INPUT");
    NUMARG(threads, "TBB_THREADS");

    {
        tbb::task_scheduler_init init(threads);
        qtimer_start(timer);
        ret = simple_parallel_fib(n);
        qtimer_stop(timer);
    }

    if (validation[n] == ret) {
        LOG_FIB_YAML(n, ret, qtimer_secs(timer))
        LOG_ENV_TBB_YAML(threads)
    } else {
        iprintf("Fail %lu (== %lu) in %f sec\n",
                (unsigned long)ret,
                (unsigned long)validation[n],
                qtimer_secs(timer));
    }

    qtimer_destroy(timer);

    return 0;
}

/* vim:set expandtab */
