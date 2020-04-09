#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include <qthread/qpool.h>
#include <pthread.h>
#include <qthread/qtimer.h>
#ifdef QTHREAD_HAVE_LIBNUMA
# include <numa.h>
#endif
#include "argparsing.h"

#define ELEMENT_COUNT   10000
#define THREAD_COUNT    128

qpool *qp = NULL;
size_t **allthat;

void **ptr = NULL;
pthread_mutex_t *ptr_lock;

static void mutexpool_allocator(const size_t startat, const size_t stopat,
                                void *arg)
{                                      /*{{{ */
    size_t i;
    qthread_shepherd_id_t shep = qthread_shep();

    for (i = startat; i < stopat; i++) {
        pthread_mutex_lock(ptr_lock + shep);
        if ((ptr == NULL) || (ptr[shep] == NULL)) {
            fprintf(stderr, "mutex pool failed! (pool_allocator %i)\n",
                    (int)shep);
            exit(-1);
        }
        allthat[i] = ptr[shep];
        ptr[shep] = *((void **)(ptr[shep]));
        pthread_mutex_unlock(ptr_lock + shep);
        allthat[i][0] = i;
    }
}                                      /*}}} */

static void mutexpool_deallocator(const size_t startat, const size_t stopat,
                                  void *arg)
{                                      /*{{{ */
    size_t i;
    qthread_shepherd_id_t shep = qthread_shep();

    for (i = startat; i < stopat; i++) {
        pthread_mutex_lock(ptr_lock + shep);
        *(void **)(allthat[i]) = ptr[shep];
        ptr[shep] = allthat[i];
        pthread_mutex_unlock(ptr_lock + shep);
    }
}                                      /*}}} */

static void pool_allocator(const size_t startat, const size_t stopat,
                           void *arg)
{                                      /*{{{ */
    size_t i;
    qpool *p = (qpool *)arg;

    for (i = startat; i < stopat; i++) {
        if ((allthat[i] = qpool_alloc(p)) == NULL) {
            fprintf(stderr, "qpool_alloc() failed! (pool_allocator)\n");
            exit(-1);
        }
        allthat[i][0] = i;
    }
}                                      /*}}} */

static void pool_deallocator(const size_t startat, const size_t stopat,
                             void *arg)
{                                      /*{{{ */
    size_t i;
    qpool *p = (qpool *)arg;

    for (i = startat; i < stopat; i++) {
        qpool_free(p, allthat[i]);
    }
}                                      /*}}} */

static void malloc_allocator(const size_t startat, const size_t stopat,
                             void *arg)
{                                      /*{{{ */
    size_t i;

    for (i = startat; i < stopat; i++) {
        if ((allthat[i] = malloc(44)) == NULL) {
            fprintf(stderr, "malloc() failed!\n");
            exit(-1);
        }
        allthat[i][0] = i;
    }
}                                      /*}}} */

static void malloc_deallocator(const size_t startat, const size_t stopat,
                               void *arg)
{                                      /*{{{ */
    size_t i;

    for (i = startat; i < stopat; i++) {
        free(allthat[i]);
    }
}                                      /*}}} */

int main(
    int argc,
    char *argv[])
{
    size_t i;
    unsigned long iterations = 1000;
    qtimer_t timer = qtimer_create();
    void **numa_allocs;
    void **numa_pools;
    size_t numa_size;
    size_t numshep;

    assert(qthread_initialize() == QTHREAD_SUCCESS);

    CHECK_VERBOSE();
    NUMARG(iterations, "TEST_ITERATIONS");

    allthat = malloc(sizeof(void *) * iterations);
    assert(allthat != NULL);

    qtimer_start(timer);
    qt_loop_balance(0, iterations, malloc_allocator, NULL);
    qtimer_stop(timer);
    printf("Time to alloc %lu malloc blocks in parallel: %f\n", iterations,
           qtimer_secs(timer));
    qtimer_start(timer);
    qt_loop_balance(0, iterations, malloc_deallocator, NULL);
    qtimer_stop(timer);
    printf("Time to free %lu malloc blocks in parallel: %f\n", iterations,
           qtimer_secs(timer));

    /* heat the pool */
    numshep = qthread_num_shepherds();
    ptr = malloc(sizeof(void *) * numshep);
    ptr_lock = malloc(sizeof(pthread_mutex_t) * numshep);
    numa_allocs = malloc(sizeof(void *) * numshep);
    numa_pools = malloc(sizeof(void *) * numshep);
    numa_size = iterations * 48 / numshep;
    iprintf("numa_size = %i\n", (int)numa_size);
    for (i = 0; i < numshep; i++) {
#ifdef QTHREAD_HAVE_LIBNUMA
        numa_allocs[i] = numa_alloc_onnode(numa_size, i);
#else
        numa_allocs[i] = malloc(numa_size);
#endif
        pthread_mutex_init(ptr_lock + i, NULL);
    }
    memcpy(numa_pools, numa_allocs, sizeof(void *) * numshep);
    for (i = 0; i < iterations; i++) {
        void *p;
        int shep = i % numshep;
        /* pull from numa_allocs[shep] */
        p = numa_allocs[shep];
        numa_allocs[shep] = ((char *)p) + 48;
        /* push to ptr[shep] */
        *(void **)p = ptr[shep];
        ptr[shep] = p;
    }
    qtimer_start(timer);
    qt_loop_balance(0, iterations, mutexpool_allocator, qp);
    qtimer_stop(timer);
    printf("Time to alloc %lu mutex pooled blocks in parallel: %f\n",
           iterations, qtimer_secs(timer));
    qtimer_start(timer);
    qt_loop_balance(0, iterations, mutexpool_deallocator, qp);
    qtimer_stop(timer);
    printf("Time to free %lu mutex pooled blocks in parallel: %f\n",
           iterations, qtimer_secs(timer));
    for (i = 0; i < numshep; i++) {
#ifdef QTHREAD_HAVE_LIBNUMA
        numa_free(numa_pools[i], numa_size);
#else
        free(numa_pools[i]);
#endif
    }
    free(numa_pools);
    free(numa_allocs);

    if ((qp = qpool_create(44)) == NULL) {
        fprintf(stderr, "qpool_create() failed!\n");
        exit(-1);
    }

    qt_loop_balance(0, iterations, pool_allocator, qp);
    qt_loop_balance(0, iterations, pool_deallocator, qp);
    qtimer_start(timer);
    qt_loop_balance(0, iterations, pool_allocator, qp);
    qtimer_stop(timer);
    printf("Time to alloc %lu pooled blocks in parallel: %f\n", iterations,
           qtimer_secs(timer));
    qtimer_start(timer);
    qt_loop_balance(0, iterations, pool_deallocator, qp);
    qtimer_stop(timer);
    printf("Time to free %lu pooled blocks in parallel: %f\n", iterations,
           qtimer_secs(timer));

    qpool_destroy(qp);

    free(allthat);

    qtimer_destroy(timer);
    iprintf("success!\n");
    return 0;
}

/* vim:set expandtab */