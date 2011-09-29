#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "qthread/qthread.h"
#include "qthread/cacheline.h"
#include "qthread/qt_sinc.h"

qt_sinc_t *qt_sinc_create(void)
{
    qt_sinc_t *sinc = malloc(sizeof(qt_sinc_t));

    assert(sinc);

    // Create cacheline-buffered values and counts arrays
    {
        const int    cacheline   = qthread_cacheline();
        const size_t num_workers = qthread_readstate(TOTAL_WORKERS);

        sinc->factor = cacheline / sizeof(aligned_t);

        sinc->values = calloc(sinc->factor * num_workers, sizeof(aligned_t));
        assert(sinc->values);

        sinc->counts = calloc(sinc->factor * num_workers, sizeof(aligned_t));
        assert(sinc->counts);
    }

    sinc->result = SYNCVAR_EMPTY_INITIALIZER;

    return sinc;
}

void qt_sinc_destroy(qt_sinc_t *sinc)
{
    free(sinc->values);
    free(sinc->counts);
    free(sinc);
}

void qt_sinc_willspawn(qt_sinc_t *sinc,
                       size_t     count)
{
    qthread_incr(&sinc->counts[qthread_worker(NULL) * sinc->factor], count);
}

static void qt_sinc_check(qt_sinc_t *sinc)
{
    aligned_t    sum         = 0;
    const size_t num_workers = qthread_readstate(TOTAL_WORKERS);

    for (size_t i = 0; i < num_workers; i++) {
        qthread_incr(&sum, sinc->counts[i * sinc->factor]);
    }

    if (0 == sum) {
        qthread_syncvar_writeF_const(&sinc->result, 42);
    }
}

void qt_sinc_submit(qt_sinc_t *sinc,
                    aligned_t *value)
{
    const qthread_worker_id_t wid = qthread_worker(NULL);

    if (NULL != value) {
        qthread_incr(&sinc->values[wid * sinc->factor], *value);
    }
    qthread_incr(&sinc->counts[wid * sinc->factor], -1);

    qt_sinc_check(sinc);
}

void qt_sinc_wait(qt_sinc_t *sinc,
                  aligned_t *target)
{
    qthread_syncvar_readFF(NULL, &sinc->result);

    if (NULL != target) {
        aligned_t    sum         = 0;
        const size_t num_workers = qthread_readstate(TOTAL_WORKERS);
        for (size_t i = 0; i < num_workers; i++) {
            qthread_incr(&sum, sinc->values[i * sinc->factor]);
        }

        *target = sum;
    }
}

/* vim:set expandtab: */
