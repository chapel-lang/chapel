#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* API Headers */
#include "qthread/qthread.h"
#include "qthread/cacheline.h"
#include "qthread/sinc.h"

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

void qt_sinc_init(qt_sinc_t *restrict  sinc_,
                  size_t               sizeof_value,
                  const void *restrict initial_value,
                  qt_sinc_op_f         op,
                  size_t               expect)
{   /*{{{*/
    assert(sinc_);
    assert((0 == sizeof_value && NULL == initial_value) ||
           (0 != sizeof_value && NULL != initial_value));
    qt_internal_sinc_t *const restrict sinc = (struct qt_sinc_s *)sinc_;

    if (QTHREAD_EXPECT((num_sheps == 0), 0)) {
        num_sheps   = qthread_readstate(TOTAL_SHEPHERDS);
        num_workers = qthread_readstate(TOTAL_WORKERS);
        num_wps     = num_workers / num_sheps;
        cacheline   = qthread_cacheline();
    }

    if (sizeof_value == 0) {
        sinc->rdata = NULL;
    } else {
        const size_t         sizeof_shep_values     = num_wps * sizeof_value;
        const size_t         num_lines_per_shep     = QT_CEIL_RATIO(sizeof_shep_values, cacheline);
        const size_t         num_lines              = num_sheps * num_lines_per_shep;
        const size_t         sizeof_shep_value_part = num_lines_per_shep * cacheline;
        qt_sinc_reduction_t *rdata                  = sinc->rdata = MALLOC(sizeof(qt_sinc_reduction_t));
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
        qthread_empty(&sinc->ready);
    } else {
        qthread_fill(&sinc->ready);
    }
} /*}}}*/

qt_sinc_t *qt_sinc_create(const size_t sizeof_value,
                          const void  *initial_value,
                          qt_sinc_op_f op,
                          const size_t will_spawn)
{   /*{{{*/
    qt_sinc_t *const restrict sinc = MALLOC(sizeof(qt_sinc_t));

    assert(sinc);

    qt_sinc_init(sinc, sizeof_value, initial_value, op, will_spawn);

    return sinc;
} /*}}}*/

void qt_sinc_reset(qt_sinc_t   *sinc_,
                   const size_t will_spawn)
{   /*{{{*/
    qt_internal_sinc_t *const restrict sinc  = (qt_internal_sinc_t *)sinc_;
    qt_sinc_reduction_t               *rdata = sinc->rdata;

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
        qthread_empty(&sinc->ready);
    } else {
        qthread_fill(&sinc->ready);
    }
} /*}}}*/

void qt_sinc_fini(qt_sinc_t *sinc_)
{   /*{{{*/
    assert(sinc_);
    qt_internal_sinc_t *const restrict sinc = (qt_internal_sinc_t *)sinc_;
    if (sinc->rdata) {
        qt_sinc_reduction_t *rdata = sinc->rdata;
        assert(rdata->result);
        assert(rdata->initial_value);
        FREE(rdata->initial_value, rdata->sizeof_value);
        assert(rdata->values);
        qt_internal_aligned_free(rdata->values, cacheline);
    }
} /*}}}*/

void qt_sinc_destroy(qt_sinc_t *sinc_)
{   /*{{{*/
    qt_sinc_fini(sinc_);
    FREE(sinc_, sizeof(qt_sinc_t));
} /*}}}*/

/* Adds a new participant to the sinc.
 * Pre:  sinc was created
 * Post: aggregate count is positive
 */
void qt_sinc_expect(qt_sinc_t *sinc_,
                    size_t     count)
{   /*{{{*/
    assert(sinc_);
    qt_internal_sinc_t *const restrict sinc = (qt_internal_sinc_t *)sinc_;

    qt_sinc_count_t oldc = sinc->counter, newc = 0;
    while ((newc = qthread_cas(&sinc->counter, oldc, oldc + count)) != oldc) oldc = newc;
    if (oldc == 0) {
        qthread_empty(&sinc->ready);
    }
} /*}}}*/

void *qt_sinc_tmpdata(qt_sinc_t *sinc_)
{   /*{{{*/
    assert(sinc_);
    qt_internal_sinc_t *const restrict sinc = (qt_internal_sinc_t *)sinc_;
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
    assert(sinc_);
    qt_internal_sinc_t *const restrict sinc = (qt_internal_sinc_t *)sinc_;
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

void qt_sinc_submit(qt_sinc_t *restrict sinc_,
                    const void *restrict      value)
{   /*{{{*/
    assert(sinc_);
    qt_internal_sinc_t *const restrict sinc = (qt_internal_sinc_t *)sinc_;
    if (value) {
        // Update value
        qt_sinc_reduction_t *const restrict rdata = sinc->rdata;
        assert(sinc->rdata);
        assert((rdata->result && rdata->initial_value) || (!rdata->result && !rdata->initial_value));

        const size_t sizeof_shep_value_part = rdata->sizeof_shep_value_part;
        const size_t sizeof_value           = rdata->sizeof_value;

        qthread_shepherd_id_t shep_id   = qthread_shep();
        qthread_worker_id_t   worker_id = qthread_readstate(CURRENT_WORKER);

        if (NULL != value) {
            const size_t shep_offset   = shep_id * sizeof_shep_value_part;
            const size_t worker_offset = worker_id * sizeof_value;
            void        *values        = (uint8_t *)rdata->values + shep_offset + worker_offset;

            rdata->op(values, value);
        }
    }

    // Update counter
    qt_sinc_count_t oldc = sinc->counter, newc = 0;
    while ((newc = qthread_cas(&sinc->counter, oldc, oldc - 1)) != oldc) oldc = newc;
    if (1 == newc) { // This is the final submit
        qt_sinc_internal_collate(sinc_);
    }
} /*}}}*/

void qt_sinc_wait(qt_sinc_t *restrict sinc_,
                  void *restrict      target)
{   /*{{{*/
    assert(sinc_);
    qt_internal_sinc_t *const restrict sinc = (qt_internal_sinc_t *)sinc_;
    qthread_readFF(NULL, &sinc->ready);

    if (target) {
        assert(sinc->rdata->sizeof_value > 0);
        memcpy(target, sinc->rdata->result, sinc->rdata->sizeof_value);
    }
} /*}}}*/

/* vim:set expandtab: */
