#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#define QTHREAD_NO_ASSERTS 1

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#if (HAVE_MEMALIGN && HAVE_MALLOC_H)
# include <malloc.h>                   /* for memalign() */
#endif

#include "qthread/qthread.h"
#include "qthread/cacheline.h"
#include "qthread/sinc.h"
#include "qt_asserts.h"
#include "qt_shepherd_innards.h"
#include "qt_expect.h"
#include "qt_visibility.h"
#include "qt_int_ceil.h"
#include "qt_alloc.h"

typedef aligned_t qt_sinc_count_t;

typedef struct {
    aligned_t c;
    uint8_t   pad[CACHELINE_WIDTH - sizeof(aligned_t)];
} qt_sinc_cache_count_t;

typedef struct qt_sinc_reduction_ {
    // Value-related info
    void *restrict values;
    qt_sinc_op_f   op;
    void *restrict result;
    void *restrict initial_value;
    size_t         sizeof_value;
    size_t         sizeof_shep_value_part;
} qt_sinc_reduction_t;

typedef struct qt_sinc_snzi_ {
    qt_sinc_cache_count_t *restrict counts;
    aligned_t                       remaining;
} qt_sinc_snzi_t;

typedef struct qt_sinc_s {
    qt_sinc_snzi_t      *snzi;
    aligned_t            ready;
    qt_sinc_reduction_t *rdata;
} qt_internal_sinc_t;

static size_t       num_sheps;
static size_t       num_workers;
static size_t       num_wps;
static unsigned int cacheline;

#define SNZI_ASSIGN(a, b) do {   \
        if ((b) == 0) { a = 0; } \
        else { a = (b) + 1; }    \
} while (0)

static void qt_sinc_internal_collate(qt_sinc_t *sinc);

void qt_sinc_init(qt_sinc_t *restrict  sinc_,
                  size_t               sizeof_value,
                  const void *restrict initial_value,
                  qt_sinc_op_f         op,
                  size_t               expect)
{
    assert(sinc_);
    qt_internal_sinc_t *sinc = (qt_internal_sinc_t *)sinc_;

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
        qt_sinc_reduction_t *rdata                  = sinc->rdata = qt_malloc(sizeof(qt_sinc_reduction_t));
        assert(rdata);
        rdata->op            = op;
        rdata->sizeof_value  = sizeof_value;
        rdata->initial_value = qt_malloc(2 * sizeof_value);
        assert(rdata->initial_value);
        memcpy(rdata->initial_value, initial_value, sizeof_value);
        rdata->result = ((uint8_t *)rdata->initial_value) + sizeof_value;
        assert(rdata->result);

        rdata->sizeof_shep_value_part = sizeof_shep_value_part;

        rdata->values = qt_internal_aligned_alloc(num_lines * cacheline, cacheline);
        assert(rdata->values);

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
    qt_sinc_snzi_t *snzi = sinc->snzi = qt_malloc(sizeof(qt_sinc_snzi_t));
    assert(snzi);

    assert(sizeof(qt_sinc_cache_count_t) <= cacheline);
    snzi->counts = qt_internal_aligned_alloc(num_sheps * cacheline, cacheline);
    assert(snzi->counts);
    // memset(sinc->counts, 0, num_sheps * cacheline);
    // memset(sinc->counts, 0, QTHREAD_SIZEOF_ALIGNED_T * num_sheps);

    // Initialize counts array
    if (expect > 0) {
        const size_t num_per_shep = expect / num_sheps;
        size_t       extras       = expect % num_sheps;

        // Set remaining count
        if (num_per_shep > 0) {
            snzi->remaining = num_sheps;
        } else {
            snzi->remaining = extras;
        }

        for (size_t s = 0; s < num_sheps; s++) {
#if defined(SINCS_PROFILE)
            snzi->count_spawns[s] = num_per_shep;
            if (extras > 0) {
                snzi->count_spawns[s]++;
            }
#endif      /* defined(SINCS_PROFILE) */
            if (extras > 0) {
                SNZI_ASSIGN(snzi->counts[s].c, num_per_shep + 1);
                extras--;
            } else {
                SNZI_ASSIGN(snzi->counts[s].c, num_per_shep);
            }
        }
        qthread_empty(&sinc->ready);
    } else {
        snzi->remaining = 0;
        qthread_fill(&sinc->ready);
    }
}

qt_sinc_t *qt_sinc_create(const size_t sizeof_value,
                          const void  *initial_value,
                          qt_sinc_op_f op,
                          const size_t expect)
{
    qt_sinc_t *sinc = qt_malloc(sizeof(qt_sinc_t));

    assert(sinc);
    qt_sinc_init(sinc, sizeof_value, initial_value, op, expect);

    return sinc;
}

void qt_sinc_reset(qt_sinc_t   *sinc_,
                   const size_t will_spawn)
{
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

    // Reset counts
    const size_t                   num_per_worker = will_spawn / num_sheps;
    size_t                         extras         = will_spawn % num_sheps;
    qt_sinc_snzi_t *const restrict snzi           = sinc->snzi;

    if (num_per_worker > 0) {
        snzi->remaining = num_sheps;
    } else {
        snzi->remaining = extras;
    }

    for (size_t s = 0; s < num_sheps; s++) {
        if (extras > 0) {
            extras--;
            SNZI_ASSIGN(snzi->counts[s].c, num_per_worker + 1);
        } else {
            SNZI_ASSIGN(snzi->counts[s].c, num_per_worker);
        }
    }

    // Reset ready flag
    if (will_spawn != 0) {
        qthread_empty(&sinc->ready);
    } else {
        qthread_fill(&sinc->ready);
    }
}

void qt_sinc_fini(qt_sinc_t *sinc_)
{
    qt_internal_sinc_t *const restrict sinc = (qt_internal_sinc_t *)sinc_;

#if defined(SINCS_PROFILE)
    for (size_t s = 0; s < num_sheps; s++) {
        for (size_t w = 0; w < num_wps; w++) {
            const size_t shep_offset = s;
            const size_t offset      = shep_offset + w;

            fprintf(stderr, "CI %lu %lu %lu\n", s, w, (unsigned long)sinc->count_incrs[offset]);
            fprintf(stderr, "CL %lu %lu %lu\n", s, w, (unsigned long)sinc->count_locals[offset]);
            fprintf(stderr, "CD %lu %lu %lu\n", s, w, (unsigned long)sinc->count_decrs[offset]);
            fprintf(stderr, "CR %lu %lu %lu\n", s, w, (unsigned long)sinc->count_remaining[offset]);
            fprintf(stderr, "CS %lu %lu %lu\n", s, w, (unsigned long)sinc->count_spawns[offset]);
            fprintf(stderr, "DM %lu %lu %lu\n", s, w, (unsigned long)sinc->dist_max[offset]);
            fprintf(stderr, "DT %lu %lu %lu\n", s, w, (unsigned long)sinc->dist_ttl[offset]);
            fprintf(stderr, "DC %lu %lu %lu\n", s, w, (unsigned long)sinc->dist_cnt[offset]);
            fprintf(stderr, "DA %lu %lu %f\n", s, w,
                    ((unsigned long)sinc->dist_ttl[offset] * 1.0) /
                    ((unsigned long)sinc->dist_cnt[offset] * 1.0));
        }
    }

    qt_free(sinc->count_incrs);
    qt_free(sinc->count_locals);
    qt_free(sinc->count_decrs);
    qt_free(sinc->count_remaining);
    qt_free(sinc->count_spawns);
    qt_free(sinc->dist_max);
    qt_free(sinc->dist_ttl);
#endif /* defined(SINCS_PROFILE) */

    assert(sinc);
    qt_sinc_snzi_t *const restrict snzi = sinc->snzi;
    assert(snzi);
    assert(snzi->counts);
    qt_internal_aligned_free(snzi->counts, cacheline);
    if (sinc->rdata) {
        qt_sinc_reduction_t *rdata = sinc->rdata;
        assert(rdata->result);
        assert(rdata->initial_value);
        qt_free(rdata->initial_value);
        assert(rdata->values);
        qt_internal_aligned_free(rdata->values, cacheline);
    }
}

void qt_sinc_destroy(qt_sinc_t *sinc_)
{
    qt_sinc_fini(sinc_);
    qt_free(sinc_);
}

#define SNZI_HALF 1
#define SNZI_ONE  2

/* Adds a new participant to the sinc.
 * Pre:  sinc was created
 * Post: aggregate count is positive
 */
void qt_sinc_expect(qt_sinc_t *sinc_,
                    size_t     count)
{
    assert(sinc_);
    qt_internal_sinc_t *const restrict sinc = (qt_internal_sinc_t *)sinc_;
    qt_sinc_snzi_t *const restrict     snzi = sinc->snzi;
    if (count > 0) {
        const size_t shep_id = qthread_shep();
        aligned_t   *C       = &snzi->counts[shep_id].c;
        int          succ    = 0;
        int          undoArr = 0;

        // Increment count
        do {
            aligned_t c = *C;
            if ((c >= SNZI_ONE) && (qthread_cas(C, c, c + count) == c)) {
                succ = 1;
            }
            if ((c == 0) && (qthread_cas(C, 0, SNZI_HALF) == 0)) {
                succ = 1;
                c    = SNZI_HALF;
            }
            if (c == SNZI_HALF) {
                qthread_incr(&snzi->remaining, 1);
                if (qthread_cas(C, SNZI_HALF, count + 1) != SNZI_HALF) {
                    undoArr--;
                } else {
                    succ = 1;
                }
            }
            COMPILER_FENCE;
        } while (!succ);
        if (undoArr != 0) {
            qthread_incr(&snzi->remaining, undoArr);
        }
    }
}

void *qt_sinc_tmpdata(qt_sinc_t *sinc_)
{
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
}

static void qt_sinc_internal_collate(qt_sinc_t *sinc_)
{
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
}

void qt_sinc_submit(qt_sinc_t *restrict sinc_,
                    const void *restrict      value)
{
    assert(sinc_);
    qt_internal_sinc_t *const restrict sinc    = (qt_internal_sinc_t *)sinc_;
    qthread_shepherd_id_t              shep_id = qthread_shep();

    if (value) {
        // Update value
        qt_sinc_reduction_t *const restrict rdata = sinc->rdata;
        assert(sinc->rdata);
        assert((rdata->result && rdata->initial_value) || (!rdata->result && !rdata->initial_value));

        const size_t sizeof_shep_value_part = rdata->sizeof_shep_value_part;
        const size_t sizeof_value           = rdata->sizeof_value;

        qthread_worker_id_t worker_id = qthread_readstate(CURRENT_WORKER);

        if (NULL != value) {
            const size_t shep_offset   = shep_id * sizeof_shep_value_part;
            const size_t worker_offset = worker_id * sizeof_value;
            void        *values        = (uint8_t *)rdata->values + shep_offset + worker_offset;

            rdata->op(values, value);
        }
    }

    qt_sinc_snzi_t *const restrict snzi = sinc->snzi;
    assert(snzi->counts);

    do {
        aligned_t x = snzi->counts[shep_id].c;
        aligned_t y;
        while (snzi->counts[shep_id].c == 0) {
            shep_id++;
            shep_id *= (shep_id < num_sheps);
        }
        if (x > SNZI_ONE) {
            y = x - 1;
        } else if (x == SNZI_ONE) {
            y = 0;
        } else {
            // shep_id has insufficient counts
            continue;
        }
        // printf("depart c=%i (%i)\n", (int)x, (int)(count_offset));
        assert(x >= SNZI_ONE);  // search for a good shep_id?
        if (qthread_cas(&snzi->counts[shep_id].c, x, y) == x) {
            if (x == SNZI_ONE) {
                x = snzi->remaining;
                // printf("snzi_depart x = %i (%p)\n", (int)x, snzi);
                assert(x >= 1);
                if (qthread_incr(&snzi->remaining, -1) == 1) {
                    qt_sinc_internal_collate(sinc_);
                    return;
                }
                // printf("            x = %i (%p)\n", (int)x, snzi);
            }
            // qthread_cas(&snzi->remaining, x, x-1);
            return;
        }
    } while (1);
}

void qt_sinc_wait(qt_sinc_t *restrict sinc_,
                  void *restrict      target)
{
    assert(sinc_);
    qt_internal_sinc_t *const restrict sinc = (qt_internal_sinc_t *)sinc_;
    qthread_readFF(NULL, &sinc->ready);

    if (target) {
        assert(sinc->rdata->sizeof_value > 0);
        memcpy(target, sinc->rdata->result, sinc->rdata->sizeof_value);
    }
}

/* vim:set expandtab: */
