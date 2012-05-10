#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#if (HAVE_MEMALIGN && HAVE_MALLOC_H)
# include <malloc.h>                   /* for memalign() */
#endif

#include "qthread/qthread.h"
#include "qthread/cacheline.h"
#include "qthread_asserts.h"
#include "qt_shepherd_innards.h"
#include "qt_visibility.h"

#include "qthread/qt_sinc.h"

typedef aligned_t qt_sinc_count_t;

struct qt_sinc_s {
    // Termination-detection-related info
    qt_sinc_count_t  counter;
    syncvar_t        ready;

    // Value-related info
    void            *restrict values;
    qt_sinc_op_f     op;
    void            *restrict result;
    void            *restrict initial_value;
    size_t           sizeof_value;
    size_t           sizeof_shep_value_part;
    size_t           sizeof_shep_count_part;
};

static size_t       num_sheps;
static size_t       num_workers;
static size_t       num_wps;
static unsigned int cacheline;

#ifdef HAVE_MEMALIGN
#define ALIGNED_ALLOC(val, size, align) (val) = memalign((align), (size))
#elif defined(HAVE_POSIX_MEMALIGN)
#define ALIGNED_ALLOC(val, size, align) posix_memalign((void **)&(val), (align), (size))
#elif defined(HAVE_WORKING_VALLOC)
#define ALIGNED_ALLOC(val, size, align) (val) = valloc((size))
#elif defined(HAVE_PAGE_ALIGNED_MALLOC)
#define ALIGNED_ALLOC(val, size, align) (val) = malloc((size))
#else
#define ALIGNED_ALLOC(val, size, align) (val) = valloc((size)) /* cross your fingers! */
#endif

qt_sinc_t *qt_sinc_create(const size_t sizeof_value,
                          const void  *initial_value,
                          qt_sinc_op_f op,
                          const size_t will_spawn)
{
    qt_sinc_t *sinc = malloc(sizeof(qt_sinc_t));
    assert(sinc);

    if (num_sheps == 0) {
        num_sheps   = qthread_readstate(TOTAL_SHEPHERDS);
        num_workers = qthread_readstate(TOTAL_WORKERS);
        num_wps     = num_workers / num_sheps;
        cacheline   = qthread_cacheline();
    }

    sinc->op           = op;
    sinc->sizeof_value = sizeof_value;

    assert((0 == sizeof_value && NULL == initial_value) ||
           (0 != sizeof_value && NULL != initial_value));

    // Allocate values array
    if (NULL != initial_value) {
        const size_t sizeof_shep_values     = num_wps * sizeof_value;
        const size_t num_lines_per_shep     = ceil(sizeof_shep_values * 1.0 / cacheline);
        const size_t num_lines              = num_sheps * num_lines_per_shep;
        const size_t sizeof_shep_value_part = num_lines_per_shep * cacheline;

        sinc->initial_value = malloc(sizeof_value);
        assert(sinc->initial_value);
        memcpy(sinc->initial_value, initial_value, sizeof_value);

        sinc->sizeof_shep_value_part = sizeof_shep_value_part;

        ALIGNED_ALLOC(sinc->values, num_lines * cacheline, cacheline);
        assert(sinc->values);

        // Initialize values
        for (size_t s = 0; s < num_sheps; s++) {
            const size_t shep_offset = s * sizeof_shep_value_part;
            for (size_t w = 0; w < num_wps; w++) {
                const size_t worker_offset = w * sinc->sizeof_value;
                memcpy((uint8_t *)sinc->values + shep_offset + worker_offset,
                       initial_value,
                       sizeof_value);
            }
        }
        sinc->result = malloc(sinc->sizeof_value);
        assert(sinc->result);
    } else {
        sinc->initial_value          = NULL;
        sinc->values                 = NULL;
        sinc->sizeof_shep_value_part = 0;
        sinc->result                 = NULL;
    }
    assert((sinc->result && sinc->initial_value) || (!sinc->result && !sinc->initial_value));

    // Initialize termination detection
    sinc->counter = will_spawn;
    sinc->ready = SYNCVAR_EMPTY_INITIALIZER;

    return sinc;
}

void qt_sinc_reset(qt_sinc_t   *sinc,
                   const size_t will_spawn)
{
    // Reset values
    if (NULL != sinc->values) {
        const size_t sizeof_shep_value_part = sinc->sizeof_shep_value_part;
        const size_t sizeof_value           = sinc->sizeof_value;
        for (size_t s = 0; s < num_sheps; s++) {
            const size_t shep_offset = s * sizeof_shep_value_part;
            for (size_t w = 0; w < num_wps; w++) {
                const size_t worker_offset = w * sizeof_value;
                memcpy((uint8_t *)sinc->values + shep_offset + worker_offset,
                       sinc->initial_value,
                       sizeof_value);
            }
        }
    }

    // Reset termination detection
    sinc->counter = will_spawn;
    sinc->ready = SYNCVAR_EMPTY_INITIALIZER;
}

void qt_sinc_destroy(qt_sinc_t *sinc)
{
    assert(sinc);
    if (sinc->result || sinc->values) {
        assert(sinc->result);
        free(sinc->result);
        assert(sinc->values);
        free(sinc->values);
    }
    free(sinc);
}

/* Adds a new participant to the sinc.
 * Pre:  sinc was created
 * Post: aggregate count is positive
 */
void qt_sinc_willspawn(qt_sinc_t *sinc,
                       size_t     count)
{
    assert(sinc);
    qthread_incr(&sinc->counter, count);
}

void *qt_sinc_tmpdata(qt_sinc_t *sinc)
{
    if (NULL != sinc->values) {
        const size_t shep_offset   = qthread_shep() * sinc->sizeof_shep_value_part;
        const size_t worker_offset = qthread_readstate(CURRENT_WORKER) * sinc->sizeof_value;
        return (uint8_t *)sinc->values + shep_offset + worker_offset;
    } else {
        return NULL;
    }
}

static void qt_sinc_internal_collate(qt_sinc_t *sinc)
{
    if (sinc->values) {
        // step 1: collate results
        const size_t sizeof_value           = sinc->sizeof_value;
        const size_t sizeof_shep_value_part = sinc->sizeof_shep_value_part;

        memcpy(sinc->result, sinc->initial_value, sizeof_value);
        for (qthread_shepherd_id_t s = 0; s < num_sheps; ++s) {
            const size_t shep_offset = s * sizeof_shep_value_part;
            for (size_t w = 0; w < num_wps; ++w) {
                sinc->op(sinc->result,
                        (uint8_t *)sinc->values + shep_offset + (w * sizeof_value));
            }
        }
    }
    // step 2: release waiters
    qthread_syncvar_writeF_const(&sinc->ready, 42);
}

void qt_sinc_submit(qt_sinc_t *restrict sinc,
                    void      *restrict value)
{
    // Update value
    assert(NULL != sinc->values || NULL == value);
    assert((sinc->result && sinc->initial_value) || (!sinc->result && !sinc->initial_value));

    const size_t sizeof_shep_value_part = sinc->sizeof_shep_value_part;
    const size_t sizeof_value           = sinc->sizeof_value;

    qthread_shepherd_id_t shep_id   = qthread_shep();
    qthread_worker_id_t   worker_id = qthread_readstate(CURRENT_WORKER);

    if (NULL != value) {
        const size_t shep_offset   = shep_id * sizeof_shep_value_part;
        const size_t worker_offset = worker_id * sizeof_value;
        void        *values        = (uint8_t *)sinc->values + shep_offset + worker_offset;

        sinc->op(values, value);
    }

    // Update counter
    qt_sinc_count_t count = qthread_incr(&sinc->counter, -1);
    if (1 == count) { // This is the final submit
        qt_sinc_internal_collate(sinc);
    }
}

void qt_sinc_wait(qt_sinc_t *restrict sinc,
                  void      *restrict target)
{
    qthread_syncvar_readFF(NULL, &sinc->ready);

    if (target) {
        assert(sinc->sizeof_value > 0);
        memcpy(target, sinc->result, sinc->sizeof_value);
    }
}

/* vim:set expandtab: */
