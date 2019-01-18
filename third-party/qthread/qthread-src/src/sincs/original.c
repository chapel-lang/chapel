#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#if (HAVE_MEMALIGN && HAVE_MALLOC_H)
# include <malloc.h>                   /* for memalign() */
#endif

/* API Headers */
#include "qthread/qthread.h"
#include "qthread/cacheline.h"
#include "qthread/sinc.h"

/* Internal Headers */
#include "qt_asserts.h"
#include "qt_shepherd_innards.h"
#include "qt_visibility.h"
#include "qt_int_ceil.h"
#include "qt_alloc.h"

typedef saligned_t qt_sinc_count_t;

struct qt_sinc_s {
    void *restrict            values;
    qt_sinc_count_t *restrict counts;
    qt_sinc_op_f              op;
    syncvar_t                 ready;
    void *restrict            result;
    void *restrict            initial_value;
    aligned_t                 remaining;
    size_t                    sizeof_value;
    size_t                    sizeof_shep_value_part;
    size_t                    sizeof_shep_count_part;
#if defined(SINCS_PROFILE)
    qt_sinc_count_t          *count_incrs;
    qt_sinc_count_t          *count_decrs;
    qt_sinc_count_t          *count_remaining;
    qt_sinc_count_t          *count_locals;
    qt_sinc_count_t          *count_spawns;
    qt_sinc_count_t          *dist_max;
    qt_sinc_count_t          *dist_ttl;
    qt_sinc_count_t          *dist_cnt;
#endif /* defined(SINCS_PROFILE) */
};

static size_t       num_sheps;
static size_t       num_workers;
static size_t       num_wps;
static unsigned int cacheline;

qt_sinc_t *qt_sinc_create(const size_t sizeof_value,
                          const void  *initial_value,
                          qt_sinc_op_f op,
                          const size_t will_spawn)
{
    struct qt_sinc_s *sinc = qt_malloc(sizeof(struct qt_sinc_s));

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
        const size_t num_lines_per_shep     = QT_CEIL_RATIO(sizeof_shep_values, cacheline);
        const size_t num_lines              = num_sheps * num_lines_per_shep;
        const size_t sizeof_shep_value_part = num_lines_per_shep * cacheline;

        sinc->initial_value = qt_malloc(sizeof_value);
        assert(sinc->initial_value);
        memcpy(sinc->initial_value, initial_value, sizeof_value);

        sinc->sizeof_shep_value_part = sizeof_shep_value_part;

        sinc->values = qt_internal_aligned_alloc(num_lines * cacheline, cacheline);
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
        sinc->result = qt_malloc(sinc->sizeof_value);
        assert(sinc->result);
    } else {
        sinc->initial_value          = NULL;
        sinc->values                 = NULL;
        sinc->sizeof_shep_value_part = 0;
        sinc->result                 = NULL;
    }
    assert((sinc->result && sinc->initial_value) || (!sinc->result && !sinc->initial_value));

    // Allocate counts array
    const size_t sizeof_shep_counts     = num_wps * QTHREAD_SIZEOF_ALIGNED_T;
    const size_t num_lines_per_shep     = QT_CEIL_RATIO(sizeof_shep_counts, cacheline);
    const size_t sizeof_shep_count_part = (num_lines_per_shep * cacheline) / QTHREAD_SIZEOF_ALIGNED_T;
    const size_t num_count_array_lines  = num_sheps * num_lines_per_shep;

    sinc->sizeof_shep_count_part = sizeof_shep_count_part;
    sinc->counts                 = qt_internal_aligned_alloc(num_count_array_lines * cacheline, cacheline);
    assert(sinc->counts);
    memset(sinc->counts, 0, num_count_array_lines * cacheline);

#if defined(SINCS_PROFILE)
    sinc->count_incrs = qt_internal_aligned_alloc(num_count_array_lines * cacheline, cacheline);
    assert(sinc->count_incrs);
    memset(sinc->count_incrs, 0, num_count_array_lines * cacheline);
    sinc->count_locals = qt_internal_aligned_alloc(num_count_array_lines * cacheline, cacheline);
    assert(sinc->count_locals);
    memset(sinc->count_locals, 0, num_count_array_lines * cacheline);
    sinc->count_decrs = qt_internal_aligned_alloc(num_count_array_lines * cacheline, cacheline);
    assert(sinc->count_decrs);
    memset(sinc->count_decrs, 0, num_count_array_lines * cacheline);
    sinc->count_remaining = qthreaad_internal_aligned_alloc(num_count_array_lines * cacheline, cacheline);
    assert(sinc->count_remaining);
    memset(sinc->count_remaining, 0, num_count_array_lines * cacheline);
    sinc->count_spawns = qt_internal_aligned_alloc(num_count_array_lines * cacheline, cacheline);
    assert(sinc->count_spawns);
    memset(sinc->count_spawns, 0, num_count_array_lines * cacheline);
    sinc->dist_max = qt_internal_aligned_alloc(num_count_array_lines * cacheline, cacheline);
    assert(sinc->dist_max);
    memset(sinc->dist_max, 0, num_count_array_lines * cacheline);
    sinc->dist_ttl = qt_internal_aligned_alloc(num_count_array_lines * cacheline, cacheline);
    assert(sinc->dist_ttl);
    memset(sinc->dist_ttl, 0, num_count_array_lines * cacheline);
    sinc->dist_cnt = qt_internal_aligned_alloc(num_count_array_lines * cacheline, cacheline);
    assert(sinc->dist_cnt);
    memset(sinc->dist_cnt, 0, num_count_array_lines * cacheline);
#endif /* defined(SINCS_PROFILE) */

    // Initialize counts array
    if (will_spawn > 0) {
        const size_t num_per_worker = will_spawn / num_workers;
        size_t       extras         = will_spawn % num_workers;

        // Set remaining count
        if (num_per_worker > 0) {
            sinc->remaining = num_workers;
        } else {
            sinc->remaining = extras;
        }

        for (size_t s = 0; s < num_sheps; s++) {
            for (size_t w = 0; w < num_wps; w++) {
                const size_t shep_offset = s * sinc->sizeof_shep_count_part;
                const size_t offset      = shep_offset + w;

#if defined(SINCS_PROFILE)
                sinc->count_spawns[offset] = num_per_worker;
                if (extras > 0) {
                    sinc->count_spawns[offset]++;
                }
#endif          /* defined(SINCS_PROFILE) */
                sinc->counts[offset] = num_per_worker;
                if (extras > 0) {
                    sinc->counts[offset]++;
                    extras--;
                }
            }
        }

        sinc->ready = SYNCVAR_EMPTY_INITIALIZER;
    } else {
        sinc->ready     = SYNCVAR_INITIALIZER;
        sinc->remaining = 0;
    }

    return (qt_sinc_t*)sinc;
}

void qt_sinc_reset(qt_sinc_t   *sinct,
                   const size_t will_spawn)
{
    struct qt_sinc_s *sinc = (struct qt_sinc_s*)sinct;
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

    // Reset counts
    const size_t num_per_worker = will_spawn / num_workers;
    size_t       extras         = will_spawn % num_workers;

    if (num_per_worker > 0) {
        sinc->remaining = num_workers;
    } else {
        sinc->remaining = extras;
    }

    const size_t sizeof_shep_count_part = sinc->sizeof_shep_count_part;
    for (size_t s = 0; s < num_sheps; s++) {
        for (size_t w = 0; w < num_wps; w++) {
            const size_t shep_offset = s * sizeof_shep_count_part;
            const size_t offset      = shep_offset + w;

            sinc->counts[offset] = num_per_worker;
            if (extras > 0) {
                extras--;
                sinc->counts[offset]++;
            }
        }
    }

    // Reset ready flag
    sinc->ready = SYNCVAR_EMPTY_INITIALIZER;
}

void qt_sinc_destroy(qt_sinc_t *sinct)
{
    struct qt_sinc_s *sinc = (struct qt_sinc_s *)sinct;
#if defined(SINCS_PROFILE)
    const size_t sizeof_shep_count_part = sinc->sizeof_shep_count_part;
    for (size_t s = 0; s < num_sheps; s++) {
        for (size_t w = 0; w < num_wps; w++) {
            const size_t shep_offset = s * sizeof_shep_count_part;
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

    qt_internal_aligned_free(sinc->count_incrs, cacheline);
    qt_internal_aligned_free(sinc->count_locals, cacheline);
    qt_internal_aligned_free(sinc->count_decrs, cacheline);
    qt_internal_aligned_free(sinc->count_remaining, cacheline);
    qt_internal_aligned_free(sinc->count_spawns, cacheline);
    qt_internal_aligned_free(sinc->dist_max, cacheline);
    qt_internal_aligned_free(sinc->dist_ttl, cacheline);
    qt_internal_aligned_free(sinc->dist_cnt, cacheline);
#endif /* defined(SINCS_PROFILE) */

    assert(sinc);
    assert(sinc->counts);
    qt_internal_aligned_free(sinc->counts, cacheline);
    if (sinc->result || sinc->values) {
        assert(sinc->result);
        qt_free(sinc->result);
        assert(sinc->values);
        qt_internal_aligned_free(sinc->values, cacheline);
    }
    qt_free(sinc);
}

/* Adds a new participant to the sinc.
 * Pre:  sinc was created
 * Post: aggregate count is positive
 */
void qt_sinc_expect(qt_sinc_t *sinct,
                    size_t     count)
{
  struct qt_sinc_s *sinc = (struct qt_sinc_s *)sinct;
    assert(sinc);
    if (count > 0) {
        const qthread_worker_id_t worker_id = qthread_readstate(CURRENT_WORKER);
        const size_t              shep_id   = qthread_shep();
        qt_sinc_count_t          *counts    = sinc->counts
                                              + (shep_id * sinc->sizeof_shep_count_part)
                                              + worker_id;

        // Increment count
        qt_sinc_count_t old = qthread_incr(counts, count);
#if defined(SINCS_PROFILE)
        (void)qthread_incr(sinc->count_incrs + (shep_id * sinc->sizeof_shep_count_part) + worker_id, 1);
        (void)qthread_incr(sinc->count_spawns + (shep_id * sinc->sizeof_shep_count_part) + worker_id, count);
#endif  /* defined(SINCS_PROFILE) */

        // Increment remaining and empty ready FEB, if necessary
        if (old == 0) {
            qthread_syncvar_empty(&sinc->ready);
            (void)qthread_incr(&sinc->remaining, 1);
#if defined(SINCS_PROFILE)
            (void)qthread_incr(sinc->count_incrs + (shep_id * sinc->sizeof_shep_count_part) + worker_id, 1);
            (void)qthread_incr(sinc->count_remaining + (shep_id * sinc->sizeof_shep_count_part) + worker_id, 1);
#endif
        }
    }
}

void *qt_sinc_tmpdata(qt_sinc_t *sinct)
{
    struct qt_sinc_s *sinc = (struct qt_sinc_s *)sinct;
    if (NULL != sinc->values) {
        const size_t shep_offset   = qthread_shep() * sinc->sizeof_shep_value_part;
        const size_t worker_offset = qthread_readstate(CURRENT_WORKER) * sinc->sizeof_value;
        return (uint8_t *)sinc->values + shep_offset + worker_offset;
    } else {
        return NULL;
    }
}

static void qt_sinc_internal_collate(qt_sinc_t *sinct)
{
    struct qt_sinc_s *sinc = (struct qt_sinc_s *)sinct;
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

void qt_sinc_submit(qt_sinc_t *restrict sinct,
                    const void *restrict      value)
{
    struct qt_sinc_s *sinc = (struct qt_sinc_s *)sinct;
    assert(NULL != sinc->values || NULL == value);
    assert((sinc->result && sinc->initial_value) || (!sinc->result && !sinc->initial_value));

    const size_t        sizeof_shep_value_part = sinc->sizeof_shep_value_part;
    const size_t        sizeof_shep_count_part = sinc->sizeof_shep_count_part;
    const size_t        sizeof_value           = sinc->sizeof_value;
    qthread_shepherd_t *this_shep              = qthread_internal_getshep();
    assert(this_shep);

    qthread_shepherd_id_t shep_id   = qthread_shep();
    qthread_worker_id_t   worker_id = qthread_readstate(CURRENT_WORKER);

    if (NULL != value) {
        const size_t shep_offset   = shep_id * sizeof_shep_value_part;
        const size_t worker_offset = worker_id * sizeof_value;
        void        *values        = (uint8_t *)sinc->values + shep_offset + worker_offset;

        sinc->op(values, value);
    }

#if defined(SINCS_PROFILE)
    int dist = 0;
#endif /* defined(SINCS_PROFILE) */

    // first check just this shepherd
    {
        const size_t shep_offset = shep_id * sizeof_shep_count_part;
        for (qthread_worker_id_t wkr_delta = 0; wkr_delta < num_wps; ++wkr_delta) {
            qthread_worker_id_t cur_wkr = (worker_id + wkr_delta) % num_wps;
            qt_sinc_count_t    *count   = sinc->counts + shep_offset + cur_wkr;
#if defined(SINCS_PROFILE)
            qt_sinc_count_t *count_incr      = sinc->count_incrs + shep_offset + worker_id;
            qt_sinc_count_t *count_decr      = sinc->count_decrs + shep_offset + worker_id;
            qt_sinc_count_t *count_remaining = sinc->count_remaining + shep_offset + worker_id;
            qt_sinc_count_t *count_local     = sinc->count_locals + shep_offset + worker_id;
            qt_sinc_count_t *dist_max        = sinc->dist_max + shep_offset + worker_id;
            qt_sinc_count_t *dist_ttl        = sinc->dist_ttl + shep_offset + worker_id;
            qt_sinc_count_t *dist_cnt        = sinc->dist_cnt + shep_offset + worker_id;
#endif      /* defined(SINCS_PROFILE) */
            // Try to decrement this worker's count
            qt_sinc_count_t old_count = *count;
            if (old_count > 0) {
                old_count = qthread_incr(count, -1);
#if defined(SINCS_PROFILE)
                (void)qthread_incr(count_incr, 1);
                if (dist == 0) { (void)qthread_incr(count_local, 1); }
#endif          /* defined(SINCS_PROFILE) */

                if (old_count < 1) {
                    (void)qthread_incr(count, 1);
#if defined(SINCS_PROFILE)
                    (void)qthread_incr(count_incr, 1);
                    (void)qthread_incr(count_decr, 1);
                    if (dist == 0) { (void)qthread_incr(count_local, 1); }
#endif              /* defined(SINCS_PROFILE) */
                    old_count = 0;
                }
#if defined(SINCS_PROFILE)
                else {
                    (void)qthread_incr(dist_ttl, dist);
                    (void)qthread_incr(dist_cnt, 1);
                    if ((dist > 0) && (dist > *dist_max)) {
                        qt_sinc_count_t old_max = *dist_max, new_max = 0;
                        while ((new_max = qthread_cas(dist_max, old_max, dist))
                               != old_max) {
                            old_max = *dist_max;
                            if (dist <= old_max) { break; }
                        }
                    }
                }
#endif          /* defined(SINCS_PROFILE) */
            } else {
                old_count = 0;
            }
            if (old_count == 1) {
                /* My counter went to zero, therefore I ned to decrement the global
                 * count of workers with non-zero counts (aka "remaining") */
                aligned_t oldr = qthread_incr(&sinc->remaining, -1);
#if defined(SINCS_PROFILE)
                (void)qthread_incr(count_incr, 1);
                (void)qthread_incr(count_remaining, 1);
#endif          /* defined(SINCS_PROFILE) */
                assert(oldr > 0);
                if (oldr == 1) { qt_sinc_internal_collate((qt_sinc_t*)sinc); }
                return;
            } else if (old_count != 0) {
                return;
            }

#if defined(SINCS_PROFILE)
            dist++;
#endif      /* defined(SINCS_PROFILE) */
        }
    }
    // now check other shepherds
    for (qthread_shepherd_id_t shep_delta = 0; shep_delta < (num_sheps - 1); ++shep_delta) {
        qthread_shepherd_id_t cur_shep    = this_shep->sorted_sheplist[shep_delta];
        const size_t          shep_offset = cur_shep * sizeof_shep_count_part;
        for (qthread_worker_id_t wkr = 0; wkr < num_wps; ++wkr) {
            qt_sinc_count_t *count = sinc->counts + shep_offset + wkr;
#if defined(SINCS_PROFILE)
            qt_sinc_count_t *count_incr      = sinc->count_incrs + shep_offset + wkr;
            qt_sinc_count_t *count_decr      = sinc->count_decrs + shep_offset + worker_id;
            qt_sinc_count_t *count_remaining = sinc->count_remaining + shep_offset + worker_id;
            qt_sinc_count_t *count_local     = sinc->count_locals + shep_offset + worker_id;
            qt_sinc_count_t *dist_max        = sinc->dist_max + shep_offset + worker_id;
            qt_sinc_count_t *dist_ttl        = sinc->dist_ttl + shep_offset + worker_id;
            qt_sinc_count_t *dist_cnt        = sinc->dist_cnt + shep_offset + worker_id;
#endif      /* defined(SINCS_PROFILE) */
            // Try to decrement this worker's count
            qt_sinc_count_t old_count = *count;
            if (old_count > 0) {
                old_count = qthread_incr(count, -1);
#if defined(SINCS_PROFILE)
                (void)qthread_incr(count_incr, 1);
                if (dist == 0) { (void)qthread_incr(count_local, 1); }
#endif          /* defined(SINCS_PROFILE) */
                if (old_count < 1) {
                    (void)qthread_incr(count, 1);
#if defined(SINCS_PROFILE)
                    (void)qthread_incr(count_incr, 1);
                    (void)qthread_incr(count_decr, 1);
                    if (dist == 0) { (void)qthread_incr(count_local, 1); }
#endif              /* defined(SINCS_PROFILE) */
                    old_count = 0;
                }
#if defined(SINCS_PROFILE)
                else {
                    (void)qthread_incr(dist_ttl, dist);
                    (void)qthread_incr(dist_cnt, 1);
                    if ((dist > 0) && (dist > *dist_max)) {
                        qt_sinc_count_t old_max = *dist_max, new_max = 0;
                        while ((new_max = qthread_cas(dist_max, old_max, dist))
                               != old_max) {
                            old_max = *dist_max;
                            if (dist <= old_max) { break; }
                        }
                    }
                }
#endif          /* defined(SINCS_PROFILE) */
            } else {
                old_count = 0;
            }
            if (old_count == 1) {
                /* My counter went to zero, therefore I ned to decrement the global
                 * count of workers with non-zero counts (aka "remaining") */
                aligned_t oldr = qthread_incr(&sinc->remaining, -1);
#if defined(SINCS_PROFILE)
                (void)qthread_incr(count_incr, 1);
                (void)qthread_incr(count_remaining, 1);
#endif          /* defined(SINCS_PROFILE) */
                assert(oldr > 0);
                if (oldr == 1) { qt_sinc_internal_collate((qt_sinc_t*)sinc); }
                return;
            } else if (old_count != 0) {
                return;
            }

#if defined(SINCS_PROFILE)
            dist++;
#endif      /* defined(SINCS_PROFILE) */
        }
    }
}

void qt_sinc_wait(qt_sinc_t *restrict sinct,
                  void *restrict      target)
{
    struct qt_sinc_s *sinc = (struct qt_sinc_s *)sinct;
    qthread_syncvar_readFF(NULL, &sinc->ready);

    if (target) {
        assert(sinc->sizeof_value > 0);
        memcpy(target, sinc->result, sinc->sizeof_value);
    }
}

/* vim:set expandtab: */
