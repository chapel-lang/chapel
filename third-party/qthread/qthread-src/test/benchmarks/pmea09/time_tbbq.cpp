#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/time.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/concurrent_queue.h>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>

using namespace tbb;
using namespace std;

concurrent_queue<void*> cq;

class loop_queuer
{
public:
    void operator() (const blocked_range<size_t>&r) const
    {
        size_t begin = r.begin();
        size_t end = r.end();

        for (size_t i = begin; i < end; i++) {
            cq.push((void*)(i + 1));
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

        for (size_t i = begin; i < end; i++) {
            void* j;
            cq.try_pop(j);
            assert(NULL != j);
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

    gettimeofday(&start, NULL);
    parallel_for(blocked_range<size_t>(0, nElements), loop_queuer());
    gettimeofday(&stop, NULL);
    printf("queue time: %f secs\n",
           (stop.tv_sec + stop.tv_usec *
            1e-6) - (start.tv_sec + start.tv_usec * 1e-6));

    gettimeofday(&start, NULL);
    parallel_for(blocked_range<size_t>(0, nElements), loop_dequeuer());
    gettimeofday(&stop, NULL);
    printf("dequeue time: %f secs\n",
           (stop.tv_sec + stop.tv_usec *
            1e-6) - (start.tv_sec + start.tv_usec * 1e-6));

    assert(cq.empty());

    gettimeofday(&start, NULL);
    parallel_for(blocked_range<size_t>(0, nElements), loop_queuer());
    parallel_for(blocked_range<size_t>(0, nElements), loop_dequeuer());
    gettimeofday(&stop, NULL);
    printf("both time: %f secs\n",
           (stop.tv_sec + stop.tv_usec *
            1e-6) - (start.tv_sec + start.tv_usec * 1e-6));

    printf("success!\n");
    return 0;
}

/* vim:set expandtab */
