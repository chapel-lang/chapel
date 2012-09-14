#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/concurrent_queue.h>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/tick_count.h>
#include <tbb/tbb_allocator.h>

using namespace tbb;
using namespace std;

concurrent_queue<void*> cq;

template <int S>
struct bigobj {
public:
    char o[S];
};

tbb_allocator<bigobj<1024> > tbballoc;
size_t objsize = 1024;

class loop_queuer
{
public:
    void operator() (const blocked_range<size_t>&r) const
    {
        size_t begin = r.begin();
        size_t end = r.end();

        for (size_t i = begin; i < end; i++) {
            void * tmp = tbballoc.allocate(1);
            memset(tmp, 1, objsize);
            cq.push(tmp);
        }
    }

    // constructor
    loop_queuer(){}
};

class loop_dequeuer
{
public:
    void operator() (const blocked_range<size_t>&r) const
    {
        size_t begin = r.begin();
        size_t end = r.end();
        void * ref = tbballoc.allocate(1);

        memset(ref, 1, objsize);
        for (size_t i = begin; i < end; i++) {
            void* j;
            cq.try_pop(j);
            assert(NULL != j);
            if (memcmp(ref, j, objsize)) {
                fprintf(stderr, "memory was corrupted!\n");
                exit(-3);
            }
            tbballoc.deallocate((bigobj<1024>*)j, 0);
        }
    }

    // constructor
    loop_dequeuer(){}
};

int main(int argc, char*argv[])
{
    size_t nElements = 1280000;
    size_t nthreads = 1;
    struct timeval start, stop;

    if (argc >= 2) {
        nthreads = atoi(argv[1]);
    }
    if (argc >= 3) {
        nElements = atoi(argv[2]);
    }
    if (argc >= 4) {
        objsize = atoi(argv[3]);
    }

    task_scheduler_init init(nthreads);

    for (uintptr_t i = 0; i < nElements; i++) {
        cq.push((void*)i);
    }
    for (uintptr_t i = 0; i < nElements; i++) {
        void* j;
        cq.try_pop(j);
        assert(i == (uintptr_t)j);
    }
    assert(cq.empty());

    tick_count t0 = tick_count::now();
    parallel_for(blocked_range<size_t>(0, nElements), loop_queuer());
    tick_count t1 = tick_count::now();
    printf("queue time: %f secs\n", (t1 - t0).seconds());

    t0 = tick_count::now();
    parallel_for(blocked_range<size_t>(0, nElements), loop_dequeuer());
    t1 = tick_count::now();
    printf("dequeue time: %f secs\n", (t1 - t0).seconds());

    assert(cq.empty());

    t0 = tick_count::now();
    parallel_for(blocked_range<size_t>(0, nElements), loop_queuer());
    parallel_for(blocked_range<size_t>(0, nElements), loop_dequeuer());
    t1 = tick_count::now();
    printf("both time: %f secs\n", (t1 - t0).seconds());

    assert(cq.empty());

    printf("success!\n");
    return 0;
}

/* vim:set expandtab */
