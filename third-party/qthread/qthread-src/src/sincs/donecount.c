#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* The API */
#include "qthread/qthread.h"
#include "qthread/sinc.h"
#include "qthread/cacheline.h"

/* Internal Headers */
#include "qt_asserts.h"
#include "qt_shepherd_innards.h"
#include "qt_expect.h"
#include "qt_visibility.h"
#include "qt_alloc.h"
#include "qt_debug.h"
#include "qt_int_ceil.h"

typedef aligned_t qt_sinc_count_t;

typedef struct qt_sinc_reduction_ {
    // Value-related info
    void *restrict values;
    qt_sinc_op_f   op;
    void *restrict result;
    void *restrict initial_value;
    size_t         sizeof_value;
    size_t         sizeof_shep_value_part;
    size_t         sizeof_shep_count_part;
} qt_sinc_reduction_t;

typedef struct qt_sinc_s {
    qt_sinc_count_t      counter;
    aligned_t            ready;
    qt_sinc_reduction_t *rdata;
} qt_internal_sinc_t;

static size_t       num_sheps;
static size_t       num_workers;
static size_t       num_wps;
static unsigned int cacheline;

void API_FUNC qt_sinc_init(qt_sinc_t *restrict  sinc_,
                           size_t               sizeof_value,
                           const void *restrict initial_value,
                           qt_sinc_op_f         op,
                           size_t               expect)
{   /*{{{*/
    assert((0 == sizeof_value && NULL == initial_value) ||
           (0 != sizeof_value && NULL != initial_value));
    qt_internal_sinc_t *const restrict sinc = (struct qt_sinc_s *)sinc_;
    assert(sinc);

    qassert_aligned(sinc->counter, QTHREAD_ALIGNMENT_ALIGNED_T);

    if (QTHREAD_EXPECT((num_sheps == 0), 0)) {
        num_sheps   = qthread_readstate(TOTAL_SHEPHERDS);
        num_workers = qthread_readstate(TOTAL_WORKERS);
        num_wps     = num_workers / num_sheps;
        cacheline   = qthread_cacheline();
    }

    if (sizeof_value == 0) {
        sinc->rdata = NULL;
    } else {
        const size_t                        sizeof_shep_values     = num_wps * sizeof_value;
        const size_t                        num_lines_per_shep     = QT_CEIL_RATIO(sizeof_shep_values, cacheline);
        const size_t                        num_lines              = num_sheps * num_lines_per_shep;
        const size_t                        sizeof_shep_value_part = num_lines_per_shep * cacheline;
        qt_sinc_reduction_t *const restrict rdata                  = sinc->rdata = MALLOC(sizeof(qt_sinc_reduction_t));
        assert(rdata);
        rdata->op            = op;
        rdata->sizeof_value  = sizeof_value;
        rdata->initial_value = MALLOC(2 * sizeof_value);
        assert(rdata->initial_value);
        memcpy(rdata->initial_value, initial_value, sizeof_value);
        rdata->result = ((uint8_t *)rdata->initial_value) + sizeof_value;
        assert(rdata->result);

        rdata->sizeof_shep_value_part = sizeof_shep_value_part;

        rdata->values = qt_internal_aligned_alloc(num_lines * cacheline, cacheline);
        assert(rdata->values);
        ALLOC_SCRIBBLE(rdata->values, num_lines * cacheline);

        // Initialize values
        for (size_t s = 0; s < num_sheps; s++) {
            const size_t shep_offset = s * sizeof_shep_value_part;
            for (size_t w = 0; w < num_wps; w++) {
                const size_t worker_offset = w * rdata->sizeof_value;
                memcpy((uint8_t *)rdata->values + shep_offset + worker_offset,
                       initial_value,
                       sizeof_value);
            }
        }
    }
    sinc->counter = expect;
    if (sinc->counter != 0) {
        qthread_debug(FEB_DETAILS, "tid %u emptying sinc ready (%p)\n", qthread_id(), &sinc->ready);
        qthread_empty(&sinc->ready);
    } else {
        qthread_fill(&sinc->ready);
    }
    assert(NULL == sinc->rdata ||
           ((sinc->rdata->result && sinc->rdata->initial_value) ||
            (!sinc->rdata->result && !sinc->rdata->initial_value)));
} /*}}}*/

qt_sinc_t API_FUNC *qt_sinc_create(const size_t sizeof_value,
                                   const void  *initial_value,
                                   qt_sinc_op_f op,
                                   const size_t will_spawn)
{   /*{{{*/
    qt_sinc_t *const restrict sinc = MALLOC(sizeof(qt_sinc_t));

    assert(sinc);

    qt_sinc_init(sinc, sizeof_value, initial_value, op, will_spawn);

    return sinc;
} /*}}}*/

void API_FUNC qt_sinc_reset(qt_sinc_t   *sinc_,
                            const size_t will_spawn)
{   /*{{{*/
    qt_internal_sinc_t *const restrict sinc = (qt_internal_sinc_t *)sinc_;

    assert(sinc);

    qt_sinc_reduction_t *const restrict rdata = sinc->rdata;

    // Reset values
    if (NULL != rdata) {
        const size_t sizeof_shep_value_part = rdata->sizeof_shep_value_part;
        const size_t sizeof_value           = rdata->sizeof_value;
        for (size_t s = 0; s < num_sheps; s++) {
            const size_t shep_offset = s * sizeof_shep_value_part;
            for (size_t w = 0; w < num_wps; w++) {
                const size_t worker_offset = w * sizeof_value;
                memcpy((uint8_t *)rdata->values + shep_offset + worker_offset,
                       rdata->initial_value,
                       sizeof_value);
            }
        }
    }

    // Reset termination detection
    sinc->counter = will_spawn;
    if (sinc->counter != 0) {
        qthread_debug(FEB_DETAILS, "tid %u emptying sinc ready (%p)\n", qthread_id(), &sinc->ready);
        qthread_empty(&sinc->ready);
        /*} else {
         *  qthread_fill(&sinc->ready);*/
    }
} /*}}}*/

// add akp for power throttling
void API_FUNC qt_sinc_resize(qt_sinc_t   *sinc_,
                             const size_t diff)
{   /*{{{*/
    qt_internal_sinc_t *const restrict sinc = (qt_internal_sinc_t *)sinc_;

    assert(sinc && (0 == sinc->counter));
    assert(qthread_feb_status(&sinc->ready) == 1);

    qt_sinc_reduction_t *const restrict rdata = sinc->rdata;

    // Reset values
    if (NULL != rdata) {
        printf("not sure resize is safe when storing values in sinc\n");
    }

    // Reset termination detection
    qt_sinc_count_t count = qthread_incr(&sinc->counter, diff);

    if (count + diff <= 0) {
        qthread_fill(&sinc->ready);
    }
} /*}}}*/

// end akp add

void API_FUNC qt_sinc_fini(qt_sinc_t *sinc_)
{   /*{{{*/
    qt_internal_sinc_t *const restrict sinc = (qt_internal_sinc_t *)sinc_;

    assert(sinc);

    if (sinc->rdata) {
        qt_sinc_reduction_t *const restrict rdata = sinc->rdata;
        assert(rdata->result);
        assert(rdata->initial_value);
        FREE(rdata->initial_value, 2 * rdata->sizeof_value);
        assert(rdata->values);
        qt_internal_aligned_free(rdata->values, cacheline);
        FREE(rdata, sizeof(qt_sinc_reduction_t));
        sinc->rdata = NULL;
    }
    qthread_debug(FEB_DETAILS, "tid %u filling sinc ready as part of destruction (%p)\n", qthread_id(), &sinc->ready);
    qassert(qthread_fill(&sinc->ready), QTHREAD_SUCCESS);
} /*}}}*/

void API_FUNC qt_sinc_destroy(qt_sinc_t *sinc_)
{   /*{{{*/
    qt_internal_sinc_t *const restrict sinc = (qt_internal_sinc_t *)sinc_;

    qt_sinc_fini(sinc_);
    if (sinc->rdata) {
        FREE(sinc->rdata, sizeof(qt_sinc_reduction_t));
    }
    FREE(sinc_, sizeof(qt_internal_sinc_t));
} /*}}}*/

/* Adds a new participant to the sinc.
 * Pre:  sinc was created
 * Post: aggregate count is positive
 */
void API_FUNC qt_sinc_expect(qt_sinc_t *sinc_,
                             size_t     count)
{   /*{{{*/
    qt_internal_sinc_t *const restrict sinc = (qt_internal_sinc_t *)sinc_;

    assert(sinc);

    if (count != 0) {
        if (qthread_incr(&sinc->counter, count) == 0) {
            qthread_debug(FEB_DETAILS, "tid %u emptying sinc ready (%p)\n", qthread_id(), &sinc->ready);
            qthread_empty(&sinc->ready);
        }
    }

    assert(sinc && (0 < sinc->counter));
} /*}}}*/

void *qt_sinc_tmpdata(qt_sinc_t *sinc_)
{   /*{{{*/
    qt_internal_sinc_t *const restrict sinc = (qt_internal_sinc_t *)sinc_;

    assert(sinc);

    if (NULL != sinc->rdata) {
        qt_sinc_reduction_t *const restrict rdata         = sinc->rdata;
        const size_t                        shep_offset   = qthread_shep() * rdata->sizeof_shep_value_part;
        const size_t                        worker_offset = qthread_readstate(CURRENT_WORKER) * rdata->sizeof_value;
        return (uint8_t *)rdata->values + shep_offset + worker_offset;
    } else {
        return NULL;
    }
} /*}}}*/

static void qt_sinc_internal_collate(qt_sinc_t *sinc_)
{   /*{{{*/
    qt_internal_sinc_t *const restrict sinc = (qt_internal_sinc_t *)sinc_;

    assert(sinc);

    if (sinc->rdata) {
        qt_sinc_reduction_t *const restrict rdata = sinc->rdata;

        // step 1: collate results
        const size_t sizeof_value           = rdata->sizeof_value;
        const size_t sizeof_shep_value_part = rdata->sizeof_shep_value_part;

        memcpy(rdata->result, rdata->initial_value, sizeof_value);
        for (qthread_shepherd_id_t s = 0; s < num_sheps; ++s) {
            const size_t shep_offset = s * sizeof_shep_value_part;
            for (size_t w = 0; w < num_wps; ++w) {
                rdata->op(rdata->result,
                          (uint8_t *)rdata->values + shep_offset + (w * sizeof_value));
            }
        }
    }

    // step 2: release waiters
    qthread_fill(&sinc->ready);
} /*}}}*/

void API_FUNC qt_sinc_submit(qt_sinc_t *restrict  sinc_,
                             const void *restrict value)
{   /*{{{*/
    qt_internal_sinc_t *const restrict sinc = (qt_internal_sinc_t *)sinc_;

    assert(sinc);

    if (value) {
        // Update value
        qt_sinc_reduction_t *const restrict rdata = sinc->rdata;
        assert(sinc->rdata);
        assert((rdata->result && rdata->initial_value) || (!rdata->result && !rdata->initial_value));

        const size_t sizeof_shep_value_part = rdata->sizeof_shep_value_part;
        const size_t sizeof_value           = rdata->sizeof_value;

        const qthread_shepherd_id_t shep_id   = qthread_shep();
        const qthread_worker_id_t   worker_id = qthread_readstate(CURRENT_WORKER);

        if (NULL != value) {
            const size_t shep_offset   = shep_id * sizeof_shep_value_part;
            const size_t worker_offset = worker_id * sizeof_value;
            void        *values        = (uint8_t *)rdata->values + shep_offset + worker_offset;

            rdata->op(values, value);
        }
    }

    // Update counter
    qt_sinc_count_t count = qthread_incr(&sinc->counter, -1);
    assert(count > 0);
    if (1 == count) { // This is the final submit
        qt_sinc_internal_collate(sinc_);
    }
} /*}}}*/

void API_FUNC qt_sinc_wait(qt_sinc_t *restrict sinc_,
                           void *restrict      target)
{   /*{{{*/
    qt_internal_sinc_t *const restrict sinc = (qt_internal_sinc_t *)sinc_;

    assert(sinc);
    assert(NULL == sinc->rdata ||
           (((NULL != sinc->rdata->values) && (0 < sinc->rdata->sizeof_value) && sinc->rdata->op) || (NULL == target)));

    qthread_readFF(NULL, &sinc->ready);

    // XXX: race with many waiters, few cores - first waiter to finish hits `qt_sinc_destroy()`.
    // XXX: need a count of waiters and barrier to protect access to sinc members.
    if (target && sinc->rdata && sinc->rdata->sizeof_value && sinc->rdata->result) {
        memcpy(target, sinc->rdata->result, sinc->rdata->sizeof_value);
    }
} /*}}}*/

/* vim:set expandtab: */
