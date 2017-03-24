#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <stdlib.h>                    /* for calloc() */
#include <sys/types.h>
#include <sys/mman.h>
#ifdef QTHREAD_USE_VALGRIND
# include <valgrind/memcheck.h>
#else
# define VALGRIND_MAKE_MEM_NOACCESS(a, b)
# define VALGRIND_MAKE_MEM_DEFINED(a, b)
#endif

/* Public Headers */
#include "qthread/qarray.h"

/* Local Headers */
#include "qt_visibility.h"
#include "qt_asserts.h"
#include "qt_shepherd_innards.h"           /* for shep_to_node */
#include "qt_debug.h"
#include "qt_alloc.h"
#include "qt_gcd.h"                    /* for qt_lcm() */
#include "qt_int_ceil.h"

static unsigned short pageshift                  = 0;
static aligned_t     *chunk_distribution_tracker = NULL;

/* local funcs */
/* this function is for DIST *ONLY*; it returns a pointer to the location that
 * the bookkeeping data is stored (i.e. the record of where this segment is
 * stored) */
static QINLINE qthread_shepherd_id_t *qarray_internal_segment_shep(const qarray *a,
                                                                   const void   *segment_head)
{                                      /*{{{ */
    char *ptr = (((char *)segment_head) + (a->segment_size * a->unit_size));

    qassert_ret(a->dist_type == DIST, NULL);
    /* ensure that it's 4-byte aligned
     * (mandatory on Sparc, good idea elsewhere) */
    if (((uintptr_t)ptr) & 3) {
        ptr += 4 - (((uintptr_t)ptr) & 3);
    }
    /* first, do we have the space? */
    qassert_ret((((ptr + sizeof(qthread_shepherd_id_t) - 1)
                  < a->segment_bytes + (const char *)segment_head)), NULL);
    return (qthread_shepherd_id_t *)ptr;
}                                      /*}}} */

static QINLINE qthread_shepherd_id_t qarray_internal_segment_shep_read(const qarray *a,
                                                                       const void   *segment_head)
{   /*{{{*/
    qthread_shepherd_id_t  retval;
    qthread_shepherd_id_t *ptr = qarray_internal_segment_shep(a, segment_head);

    VALGRIND_MAKE_MEM_DEFINED(ptr, sizeof(qthread_shepherd_id_t));
    retval = *ptr;
    VALGRIND_MAKE_MEM_NOACCESS(ptr, sizeof(qthread_shepherd_id_t));
    return retval;
} /*}}}*/

static QINLINE void qarray_internal_segment_shep_write(const qarray         *a,
                                                       const void           *segment_head,
                                                       qthread_shepherd_id_t shep)
{   /*{{{*/
    qthread_shepherd_id_t *ptr = qarray_internal_segment_shep(a, segment_head);

    VALGRIND_MAKE_MEM_DEFINED(ptr, sizeof(qthread_shepherd_id_t));
    *ptr = shep;
    VALGRIND_MAKE_MEM_NOACCESS(ptr, sizeof(qthread_shepherd_id_t));
} /*}}}*/

/* this function returns the shepherd that owns a given segment, specified by
 * the segment's index */
static inline qthread_shepherd_id_t qarray_internal_shepof_segidx(const qarray *a,
                                                                  const size_t  seg)
{                                      /*{{{ */
    switch (a->dist_type) {
        case ALL_SAME:
            return a->dist_specific.dist_shep;

        case FIXED_FIELDS:
        {
            const size_t segs_per_shep =
                a->dist_specific.stripes.segs_per_shep;
            const size_t extras = a->dist_specific.stripes.extras;
            if (seg / (segs_per_shep + 1) < extras) {
                return seg / (segs_per_shep + 1);
            } else {
                return (seg - extras) / segs_per_shep;
            }
        }
        case FIXED_HASH:
            return (qthread_shepherd_id_t)(seg % qthread_num_shepherds());

        case DIST:
            return qarray_internal_segment_shep_read(a,
                                                     qarray_elem_nomigrate(a,
                                                                           seg *
                                                                           a->
                                                                           segment_size));

        default:
            QTHREAD_TRAP();
            return 0;
    }
}                                      /*}}} */

/* this function returns the shepherd that owns a given segment, specified by
 * the pointer to the segment's first element */
static inline qthread_shepherd_id_t qarray_internal_shepof_ch(const qarray *a,
                                                              const void   *segment_head)
{                                      /*{{{ */
    switch (a->dist_type) {
        case ALL_SAME:
            return a->dist_specific.dist_shep;

        case FIXED_FIELDS:
        {
            const size_t bytes_in =
                (uintptr_t)segment_head - (uintptr_t)a->base_ptr;
            const size_t segments_in = bytes_in / a->segment_bytes;
            return qarray_internal_shepof_segidx(a, segments_in);
        }
        case FIXED_HASH:
        default:
            return (qthread_shepherd_id_t)(((((uintptr_t)segment_head) -
                                             ((uintptr_t)a->base_ptr)) /
                                            a->segment_bytes) %
                                           qthread_num_shepherds());

        case DIST:
            return qarray_internal_segment_shep_read(a, segment_head);
    }
}                                      /*}}} */

static void qarray_free_cdt(void)
{                                      /*{{{ */
    if (chunk_distribution_tracker != NULL) {
        FREE(chunk_distribution_tracker, qthread_num_shepherds() * sizeof(aligned_t));
        chunk_distribution_tracker = NULL;
    }
}                                      /*}}} */

static qarray *qarray_create_internal(const size_t         count,
                                      const size_t         obj_size,
                                      const distribution_t d,
                                      const char           tight,
                                      const int            seg_pages)
{                               /*{{{ */
    size_t  segment_count;      /* number of segments allocated */
    qarray *ret = NULL;

    qassert_ret((count > 0), NULL);
    qassert_ret((obj_size > 0), NULL);

    if (pageshift == 0) {
        size_t tmp = pagesize;
        while (tmp != 0) {
            pageshift++;
            tmp >>= 1;
        }
    }

    if (chunk_distribution_tracker == NULL) {
        aligned_t *tmp = qt_calloc(qthread_num_shepherds(), sizeof(aligned_t));
        qassert_ret((tmp != NULL), NULL);
        if (qthread_cas_ptr(&(chunk_distribution_tracker), NULL, tmp) != NULL) {
            FREE(tmp, qthread_num_shepherds() * sizeof(aligned_t));
        } else {
            atexit(qarray_free_cdt);
        }
    }
    ret = qt_calloc(1, sizeof(qarray));
    qassert_goto((ret != NULL), badret_exit);

    ret->count = count;
    /* make obj_size a multiple of 8 */
    if (!tight) {
        ret->unit_size =
            obj_size + ((obj_size & 7) ? (8 - (obj_size & 7)) : 0);
    } else {
        ret->unit_size = obj_size;
    }

    /* so, here's the idea: memory is assigned to shepherds in units I'm
     * choosing to call "segments" (chunk would also work, but that's overused
     * elsewhere in qthreads). Each segment can have its own shepherd. Which
     * shepherd a segment is assigned to is stored in the segment itself
     * (otherwise we'd have to use a hash table, and we'd lose all our cache
     * benefits). In SOME cases, such as FIXED_* and ALL_SAME, this is
     * unnecessary, so we can be a little more efficient with things by NOT
     * storing a shepherd ID in the segments. */
    /***************************
    * Choose allocation sizes *
    ***************************/
    switch (d) {
        case ALL_LOCAL:
        case ALL_RAND:
        case ALL_LEAST:
        case ALL_SAME:                 /* assumed equivalent to ALL_LOCAL */
        case FIXED_FIELDS:
        case FIXED_HASH:
        default:
            if (seg_pages == 0) {
                ret->segment_bytes = 16 * pagesize;
                if (ret->unit_size > ret->segment_bytes) {
                    ret->segment_bytes = qt_lcm(ret->unit_size, pagesize);
                }
            } else {
                ret->segment_bytes = seg_pages * pagesize;
            }
            ret->segment_size = ret->segment_bytes / ret->unit_size;
            assert(ret->segment_size > 0);
            assert(ret->segment_bytes > 0);
            break;
        case DIST_STRIPES:
        case DIST_FIELDS:
        case DIST_RAND:
        case DIST_LEAST:
        case DIST:                     /* assumed equivalent to DIST_RAND */
            /* since we will be storing a qthread_shepherd_id_t in each
             * segment, we need to leave space in the segment for that data.
             * The way we'll do this is that we'll just reduce the segment_size
             * by 1 (thus providing space for the shepherd identifier, as long
             * as the unit-size is bigger than a shepherd identifier). */
            if (seg_pages == 0) {
                ret->segment_bytes = 16 * pagesize;
            } else {
                ret->segment_bytes = seg_pages * pagesize;
            }
            ret->segment_size = ret->segment_bytes / ret->unit_size;
            if ((ret->segment_bytes - (ret->segment_size * ret->unit_size)) <
                4) {
                ret->segment_size--;
                /* avoid wasting too much memory */
                if (ret->unit_size > pagesize) {
                    ret->segment_bytes -=
                        (ret->unit_size / pagesize) * pagesize;
                    if (ret->unit_size % pagesize == 0) {
                        ret->segment_bytes += pagesize;
                    }
                }
            }
            assert(ret->segment_size > 0);
            assert(ret->segment_bytes > 0);
            break;
    }

    /*****************
    * Set dist_type *
    *****************/
    switch (d) {
        case ALL_LOCAL:
        case ALL_RAND:
        case ALL_LEAST:
        case ALL_SAME:
            ret->dist_type = ALL_SAME;
            break;
        case FIXED_FIELDS:
            ret->dist_type = FIXED_FIELDS;
            break;
        case FIXED_HASH:
        default:
            ret->dist_type = FIXED_HASH;
            break;
        case DIST_STRIPES:
        case DIST_FIELDS:
        case DIST_RAND:
        case DIST_LEAST:
        case DIST:
            ret->dist_type = DIST;
            break;
    }

    segment_count =
        count / ret->segment_size + ((count % ret->segment_size) ? 1 : 0);

    /* figure out dist_specific data */
    qthread_debug(QARRAY_DETAILS,
                  "qarray_create(): figuring out dist_specific data (d=%i)\n",
                  (int)d);
    switch (d) {
        case ALL_SAME:
        case ALL_LOCAL:
            ret->dist_specific.dist_shep = qthread_shep();
            qthread_incr(&chunk_distribution_tracker
                         [ret->dist_specific.dist_shep], segment_count);
            break;
        case ALL_RAND:
            ret->dist_specific.dist_shep  = (qthread_shepherd_id_t)random();
            ret->dist_specific.dist_shep %=
                (qthread_shepherd_id_t)qthread_num_shepherds();
            qthread_incr(&chunk_distribution_tracker
                         [ret->dist_specific.dist_shep], segment_count);
            break;
        case ALL_LEAST:
        {
            qthread_shepherd_id_t i, least = 0;

            for (i = 1; i < qthread_num_shepherds(); i++) {
                if (chunk_distribution_tracker[i] <
                    chunk_distribution_tracker[least]) {
                    least = i;
                }
            }
            ret->dist_specific.dist_shep = least;
            qthread_incr(&chunk_distribution_tracker[least], segment_count);
            break;
        }
        case FIXED_FIELDS:
            ret->dist_specific.stripes.segs_per_shep =
                segment_count / qthread_num_shepherds();
            if (ret->dist_specific.stripes.segs_per_shep == 0) {
                ret->dist_specific.stripes.segs_per_shep = 1;
            }
            ret->dist_specific.stripes.extras =
                segment_count % qthread_num_shepherds();
            break;
        default:
            ret->dist_specific.dist_shep = NO_SHEPHERD;
    }
#ifdef QTHREAD_HAVE_MEM_AFFINITY
    switch (d) {
        case ALL_LOCAL:
        case ALL_RAND:
        case ALL_LEAST:
        case ALL_SAME:
        default:
            if (qthread_internal_shep_to_node(ret->dist_specific.dist_shep) ==
                QTHREAD_NO_NODE) {
                case DIST_STRIPES:
                case DIST_FIELDS:
                case DIST_RAND:
                case DIST_LEAST:
                case DIST:
                case FIXED_FIELDS:
                case FIXED_HASH:
                    ret->base_ptr =
                        (char *)qt_affinity_alloc(segment_count * ret->segment_bytes);
                    break;
            } else {
                ret->base_ptr =
                    (char *)qt_affinity_alloc_onnode(segment_count *
                                                     ret->segment_bytes,
                                                     qthread_internal_shep_to_node
                                                         (ret->dist_specific.dist_shep));
            }
            break;
    }
    if (ret->base_ptr == NULL) {
# ifdef QTHREAD_HAVE_LIBNUMA
        numa_error("allocating qarray body");
# endif
    }
#else /* ifdef QTHREAD_HAVE_MEM_AFFINITY */
      /* For speed, we want page-aligned memory, if we can get it */
    ret->base_ptr = qt_internal_aligned_alloc(segment_count * ret->segment_bytes, pagesize);
#endif  /* ifdef QTHREAD_HAVE_MEM_AFFINITY */
    qassert_goto((ret->base_ptr != NULL), badret_exit);

    /********************************************
    * Assign locations, maintain segment_count *
    ********************************************/
    {
        size_t                      segment, target_shep;
        const qthread_shepherd_id_t max_sheps = qthread_num_shepherds();

        qthread_debug(QARRAY_DETAILS, "qarray_create(): segment_count = %i\n",
                      (int)segment_count);
        for (segment = 0; segment < segment_count; segment++) {
            switch (d) {
                case FIXED_HASH:
                    target_shep = segment % qthread_num_shepherds();
                    break;
                case FIXED_FIELDS:
                    target_shep = qarray_internal_shepof_segidx(ret, segment);
                    break;
                case DIST:             /* assumed equivalent to DIST_RAND */
                case DIST_RAND:
                    target_shep = random() % max_sheps;
                    break;
                case DIST_FIELDS:
                {                      /* roughly copied from FIXED_FIELDS logic */
                    size_t segs_per_shep = segment_count / max_sheps;
                    size_t extras        = segment_count % max_sheps;
                    target_shep = segment / (segs_per_shep + 1);
                    if (target_shep >= extras) {
                        target_shep = (segment - extras) / segs_per_shep;
                    }
                    break;
                }
                case DIST_STRIPES:
                    target_shep =
                        (qthread_shepherd_id_t)(segment % max_sheps);
                    break;
                case DIST_LEAST:
                {
                    qthread_shepherd_id_t i;
                    target_shep = 0;
                    for (i = 1; i < max_sheps; i++) {
                        if (chunk_distribution_tracker[i] <
                            chunk_distribution_tracker[target_shep]) {
                            target_shep = i;
                        }
                    }
                    break;
                }
                default:
                    assert(ret->dist_type == ALL_SAME);
                    target_shep = ret->dist_specific.dist_shep;
            }
            if (ret->dist_type == DIST) {
                char *seghead =
                    qarray_elem_nomigrate(ret, segment * ret->segment_size);
                qarray_internal_segment_shep_write(ret, seghead, target_shep);
            }
            assert(target_shep < max_sheps);
            qthread_debug(QARRAY_DETAILS,
                          "qarray_create(): segment %i assigned to shep %i\n",
                          segment, target_shep);
#ifdef QTHREAD_HAVE_MEM_AFFINITY
            {
                /* make sure this shep has a node; if it does, put this segment there */
                unsigned int target_node =
                    qthread_internal_shep_to_node(target_shep);
                if (target_node != QTHREAD_NO_NODE) {
                    char *seghead = qarray_elem_nomigrate(ret,
                                                          segment *
                                                          ret->segment_size);
                    qt_affinity_mem_tonode(seghead, ret->segment_bytes,
                                           target_node);
                }
            }
#endif      /* ifdef QTHREAD_HAVE_MEM_AFFINITY */
            qthread_incr(&chunk_distribution_tracker[target_shep], 1);
        }
    }
#if defined(HAVE_MADVISE) && HAVE_DECL_MADV_ACCESS_LWP
    madvise(ret->base_ptr, segment_count * ret->segment_bytes, MADV_ACCESS_LWP);
#endif
    for (unsigned int i = 0; i < qthread_num_shepherds(); i++) {
        qthread_debug(QARRAY_DETAILS,
                      "qarray_create(): shep %i has %i segments\n", i,
                      chunk_distribution_tracker[i]);
    }
    qthread_debug(QARRAY_DETAILS,
                  "qarray_create(): done assigning segments, returning\n");
    return ret;

    qgoto(badret_exit);
    if (ret) {
        if (ret->base_ptr) {
            qt_free(ret->base_ptr);
        }
        FREE(ret, sizeof(qarray));
    }
    return NULL;
}                                      /*}}} */

qarray *qarray_create(const size_t count,
                      const size_t obj_size)
{                                      /*{{{ */
#if QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32 || \
    QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64
    return qarray_create_internal(count, obj_size, DIST_STRIPES, 0, 0);

#else
    return qarray_create_internal(count, obj_size, FIXED_HASH, 0, 0);
#endif
}                                      /*}}} */

qarray *qarray_create_tight(const size_t count,
                            const size_t obj_size)
{                                      /*{{{ */
#if QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32 || \
    QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64
    return qarray_create_internal(count, obj_size, DIST_STRIPES, 1, 0);

#else
    return qarray_create_internal(count, obj_size, FIXED_HASH, 1, 0);
#endif
}                                      /*}}} */

qarray *qarray_create_configured(const size_t         count,
                                 const size_t         obj_size,
                                 const distribution_t d,
                                 const char           tight,
                                 const int            seg_pages)
{                                      /*{{{ */
    return qarray_create_internal(count, obj_size, d, tight, seg_pages);
}                                      /*}}} */

void qarray_destroy(qarray *a)
{                                      /*{{{ */
    qassert_retvoid((a != NULL));
    qassert_retvoid((a->base_ptr != NULL));
    switch (a->dist_type) {
        case DIST:
        {
            size_t       segment;
            const size_t segment_count =
                a->count / a->segment_size +
                ((a->count % a->segment_size) ? 1 : 0);
            for (segment = 0; segment < segment_count; segment++) {
                char *segmenthead = qarray_elem_nomigrate(a,
                                                          segment *
                                                          a->segment_size);
                qthread_incr(&chunk_distribution_tracker
                             [qarray_internal_segment_shep_read(a, segmenthead)],
                             -1);
            }
            break;
        }
        default:
        case FIXED_FIELDS:
        case FIXED_HASH:
        {
            size_t       segment;
            const size_t segment_count =
                a->count / a->segment_size +
                ((a->count % a->segment_size) ? 1 : 0);
            for (segment = 0; segment < segment_count; segment++) {
                qthread_incr(&chunk_distribution_tracker
                             [qarray_internal_shepof_segidx(a, segment)], -1);
            }
            break;
        }
        case ALL_SAME:
            qthread_incr(&chunk_distribution_tracker
                         [a->dist_specific.dist_shep],
                         -1 * (a->count / a->segment_size +
                               ((a->count % a->segment_size) ? 1 : 0)));
            break;
    }
#ifdef QTHREAD_HAVE_MEM_AFFINITY
    qt_affinity_free(a->base_ptr,
                     a->segment_bytes * (a->count / a->segment_size +
                                         ((a->count % a->segment_size) ? 1 : 0)));
#else
    qt_internal_aligned_free(a->base_ptr, pagesize);
#endif
    FREE(a, sizeof(qarray));
}                                      /*}}} */

qthread_shepherd_id_t qarray_shepof(const qarray *a,
                                    const size_t  index)
{                                      /*{{{ */
    qassert_ret((a != NULL), NO_SHEPHERD);
    qassert_ret((index < a->count), NO_SHEPHERD);
    switch (a->dist_type) {
        case ALL_SAME:
            return a->dist_specific.dist_shep;

        case FIXED_FIELDS:
        case FIXED_HASH:
        case DIST:
        default:
        {
            const size_t                segment_num = index / a->segment_size; /* rounded down */
            const qthread_shepherd_id_t retval      =
                qarray_internal_shepof_segidx(a, segment_num);
            assert(retval < qthread_num_shepherds());
            return retval;
        }
    }
}                                      /*}}} */

void *qarray_elem_migrate(const qarray *a,
                          const size_t  index)
{                                      /*{{{ */
    void                 *ret;
    qthread_shepherd_id_t dest;

    qassert_ret((a != NULL), NULL);
    qassert_ret((index >= a->count), NULL);
    {
        const size_t segment_num  = index / a->segment_size;    /* rounded down */
        char        *segment_head = a->base_ptr + (segment_num + a->segment_bytes);

        ret =
            segment_head +
            ((index - segment_num * a->segment_size) * a->unit_size);
        dest = qarray_internal_shepof_ch(a, segment_head);
    }
    if (qthread_shep() != dest) {
        qthread_migrate_to(dest);
    }
    return ret;
}                                      /*}}} */

struct qarray_func_wrapper_args {
    union {
        qa_loop_f ql;
        qthread_f qt;
    } func;
    qarray      *a;
    void        *arg;
    aligned_t   *donecount;
    const size_t startat, stopat;
};
struct qarray_accumfunc_wrapper_args {
    union {
        qa_loopr_f ql;
    } func;
    qt_accum_f acc;
    qarray    *a;
    void      *arg;
    void      *ret;
    size_t     startat, stopat, retsize;
};
struct qarray_constfunc_wrapper_args {
    const union {
        qa_cloop_f ql;
        qthread_f  qt;
    } func;
    const qarray *a;
    void         *arg;
    aligned_t    *donecount;
    const size_t  startat, stopat;
};

static aligned_t qarray_strider(const struct qarray_func_wrapper_args *arg)
{                                      /*{{{ */
    const size_t                segment_size = arg->a->segment_size;
    const distribution_t        dist_type    = arg->a->dist_type;
    const qthread_shepherd_id_t shep         = qthread_shep();
    size_t                      max_count    = arg->stopat;
    size_t                      count        = arg->startat;

    /* all striders get the same count/max_count, so we have to find our own
     * starting point based on this thread's shep */
    switch (dist_type) {
        case ALL_SAME:
            if (shep != arg->a->dist_specific.dist_shep) {
                goto qarray_strider_exit;
            }
            break;
        case FIXED_FIELDS:
        {
            qthread_shepherd_id_t start_shep = qarray_shepof(arg->a, count);
            qthread_shepherd_id_t stop_shep  =
                qarray_shepof(arg->a, max_count - 1);
            if ((shep < start_shep) || (shep > stop_shep)) {
                goto qarray_strider_exit;
            }
            if (shep != start_shep) {
                /* count isn't *my* starting point, but I am within the
                 * range of interest, so find my starting point. */
                size_t extras        = arg->a->dist_specific.stripes.extras;
                size_t segs_per_shep =
                    arg->a->dist_specific.stripes.segs_per_shep;
                /* this relies on sheps being zero-indexed */
                if (shep < extras) {
                    count = shep * segment_size * (segs_per_shep + 1);
                } else {
                    count =
                        (extras * segment_size * (segs_per_shep + 1)) +
                        ((shep - extras) * segment_size * segs_per_shep);
                }
            }
            {
                size_t segs_on_this_shep =
                    arg->a->dist_specific.stripes.segs_per_shep;
                size_t last_count_on_this_shep;
                if (shep < arg->a->dist_specific.stripes.extras) {
                    segs_on_this_shep++;
                }
                last_count_on_this_shep =
                    count + ((segment_size * segs_on_this_shep) - 1);
                if (max_count > last_count_on_this_shep) {
                    max_count = last_count_on_this_shep + 1;
                }
            }
            break;
        }
        default:                       // use this when our starting point is somewhat unpredictable
            if ((count > 0) && (qarray_shepof(arg->a, count) != shep)) {
                /* jump to the next segment boundary */
                count += segment_size - (count % segment_size);
                if (count >= max_count) {
                    goto qarray_strider_exit;
                }
            }
            while (qarray_shepof(arg->a, count) != shep) {
                count += segment_size;
                if (count >= max_count) {
                    goto qarray_strider_exit;
                }
            }
            break;
    }
    /* at this point...
     * 1. cursor points to the first element of the array associated with this CPU
     * 2. count is the index of that element
     */
    while (1) {
        size_t       inpage_offset;
        const size_t max_offset =
            ((max_count - count) >
             segment_size) ? segment_size : (max_count - count);

        for (inpage_offset = 0; inpage_offset < max_offset; inpage_offset++) {
            void *ptr = qarray_elem_nomigrate(arg->a, count + inpage_offset);

            assert(ptr != NULL);       // aka internal error
            arg->func.qt(ptr);
        }
        switch (dist_type) {
            case FIXED_FIELDS:
            case ALL_SAME:
                count += segment_size;
                break;
            case FIXED_HASH:
            default:
                count += segment_size * qthread_num_shepherds();
                break;
            case DIST:                 /* XXX: this is awful - slow and bad for cache */
                count += segment_size;
                if (count >= max_count) {
                    goto qarray_strider_exit;
                }
                while (qarray_shepof(arg->a, count) != shep) {
                    count += segment_size;
                    if (count >= max_count) {
                        goto qarray_strider_exit;
                    }
                }
                break;
        }
        if (count >= max_count) {
            goto qarray_strider_exit;
        }
    }
qarray_strider_exit:
    qthread_incr(arg->donecount, 1);
    return 0;
}                                      /*}}} */

static aligned_t qarray_loop_strider(const struct qarray_func_wrapper_args *arg)
{                                      /*{{{ */
    const size_t                segment_size = arg->a->segment_size;
    const distribution_t        dist_type    = arg->a->dist_type;
    const qthread_shepherd_id_t shep         = qthread_shep();
    size_t                      max_count    = arg->stopat;
    size_t                      count        = arg->startat;
    const qa_loop_f             ql           = arg->func.ql;

    /* all striders get the same count/max_count, so we have to find our own
     * starting point based on this thread's shep */
    switch (dist_type) {
        case ALL_SAME:
            if (shep != arg->a->dist_specific.dist_shep) {
                goto qarray_loop_strider_exit;
            }
            break;
        case FIXED_FIELDS:
        {
            qthread_shepherd_id_t start_shep = qarray_shepof(arg->a, count);
            qthread_shepherd_id_t stop_shep  =
                qarray_shepof(arg->a, max_count - 1);
            if ((shep < start_shep) || (shep > stop_shep)) {
                goto qarray_loop_strider_exit;
            }
            if (shep != start_shep) {
                /* count isn't *my* starting point, but I am within the
                 * range of interest, so find my starting point. */
                /* this relies on sheps being zero-indexed */
                count =
                    shep * segment_size *
                    arg->a->dist_specific.stripes.segs_per_shep;
            }
            {
                size_t last_count_on_my_shep =
                    ((shep +
                      1) * segment_size *
                     arg->a->dist_specific.stripes.segs_per_shep) - 1;
                if (max_count > last_count_on_my_shep) {
                    max_count = last_count_on_my_shep;
                }
            }
            break;
        }
        default:
            if ((count > 0) && (qarray_shepof(arg->a, count) != shep)) {
                /* jump to the next segment boundary */
                count += segment_size - (count % segment_size);
                if (count >= max_count) {
                    goto qarray_loop_strider_exit;
                }
            }
            while (qarray_shepof(arg->a, count) != shep) {
                count += segment_size;
                if (count >= max_count) {
                    goto qarray_loop_strider_exit;
                }
            }
    }
    assert(count < max_count);
    /* at this point...
     * 1. cursor points to the first element of the array associated with this CPU
     * 2. count is the index of that element
     */
    switch (dist_type) {
        /* special case: all the work is contiguous, so we can hand it to the
         * loop function directly */
        case ALL_SAME:
        case FIXED_FIELDS:
            ql(count, max_count, arg->a, arg->arg);
            goto qarray_loop_strider_exit;
        default:                       /* aka NOT the special case */
            break;
    }
    while (1) {
        {
            const size_t max_offset =
                ((max_count - count) >
                 segment_size) ? segment_size : (max_count - count);
            ql(count, count + max_offset, arg->a, arg->arg);
        }
        switch (dist_type) {
            default:
                QTHREAD_TRAP();
                break;
            case FIXED_HASH:
                count += segment_size * qthread_num_shepherds();
                break;
            case DIST:                 /* XXX: this is awful - slow and bad for cache */
                count += segment_size;
                if (count >= max_count) {
                    goto qarray_loop_strider_exit;
                }
                while (qarray_shepof(arg->a, count) != shep) {
                    count += segment_size;
                    if (count >= max_count) {
                        goto qarray_loop_strider_exit;
                    }
                }
                break;
        }
        if (count >= max_count) {
            goto qarray_loop_strider_exit;
        }
    }
qarray_loop_strider_exit:
    qthread_incr(arg->donecount, 1);
    return 0;
}                                      /*}}} */

static aligned_t qarray_loopaccum_strider(const struct qarray_accumfunc_wrapper_args *arg)
{                                      /*{{{ */
    const size_t                segment_size = arg->a->segment_size;
    const distribution_t        dist_type    = arg->a->dist_type;
    const qthread_shepherd_id_t shep         = qthread_shep();
    const qt_accum_f            acc          = arg->acc;
    const qa_loopr_f            ql           = arg->func.ql;
    size_t                      max_count    = arg->stopat;
    size_t                      count        = arg->startat;
    char                       *myret        = arg->ret;
    char                       *tmpret       = NULL;
    char                        first        = 1;

    switch (dist_type) {
        case ALL_SAME:
            if (shep != arg->a->dist_specific.dist_shep) {
                goto qarray_loop_strider_exit;
            }
            break;
        case FIXED_FIELDS:
        {
            qthread_shepherd_id_t start_shep = qarray_shepof(arg->a, count);
            qthread_shepherd_id_t stop_shep  =
                qarray_shepof(arg->a, max_count - 1);
            if ((shep < start_shep) || (shep > stop_shep)) {
                goto qarray_loop_strider_exit;
            }
            if (shep != start_shep) {
                /* count isn't *my* starting point, but I am within the
                 * range of interest, so find my starting point. */
                /* this relies on sheps being zero-indexed */
                count =
                    shep * segment_size *
                    arg->a->dist_specific.stripes.segs_per_shep;
            }
            {
                size_t last_count_on_my_shep =
                    ((shep +
                      1) * segment_size *
                     arg->a->dist_specific.stripes.segs_per_shep) - 1;
                if (max_count > last_count_on_my_shep) {
                    max_count = last_count_on_my_shep;
                }
            }
            break;
        }
        default:
            if ((count > 0) && (qarray_shepof(arg->a, count) != shep)) {
                /* jump to the next segment boundary */
                count += segment_size - (count % segment_size);
                if (count >= max_count) {
                    goto qarray_loop_strider_exit;
                }
            }
            while (qarray_shepof(arg->a, count) != shep) {
                count += segment_size;
                if (count >= max_count) {
                    goto qarray_loop_strider_exit;
                }
            }
    }
    assert(count < max_count);
    /* at this point...
     * 1. cursor points to the first element of the array associated with this CPU
     * 2. count is the index of that element
     */
    switch (dist_type) {
        /* special case: all the work is contiguous, so we can hand it to the
         * loop function directly */
        case ALL_SAME:
        case FIXED_FIELDS:
            ql(count, max_count, arg->a, arg->arg, arg->ret);
            goto qarray_loop_strider_exit;
        default:                       /* aka NOT the special case */
            break;
    }
    tmpret = MALLOC(arg->retsize);
    assert(tmpret);
    while (1) {
        {
            const size_t max_offset =
                ((max_count - count) >
                 segment_size) ? segment_size : (max_count - count);
            if (first) {
                ql(count, count + max_offset, arg->a, arg->arg, myret);
                first = 0;
            } else {
                ql(count, count + max_offset, arg->a, arg->arg, tmpret);
                acc(myret, tmpret);
            }
        }
        switch (dist_type) {
            default:
                /* This should never happen, so deliberately cause a seg fault
                 * for corefile analysis */
                QTHREAD_TRAP();
                break;
            case FIXED_HASH:
                count += segment_size * qthread_num_shepherds();
                break;
            case DIST:                 /* XXX: this is awful - slow and bad for cache */
                count += segment_size;
                if (count >= max_count) {
                    goto qarray_loop_strider_exit;
                }
                while (qarray_shepof(arg->a, count) != shep) {
                    count += segment_size;
                    if (count >= max_count) {
                        goto qarray_loop_strider_exit;
                    }
                }
                break;
        }
        if (count >= max_count) {
            goto qarray_loop_strider_exit;
        }
    }
qarray_loop_strider_exit:
    if (tmpret != NULL) {
        FREE(tmpret, arg->retsize);
    }
    return 0;
}                                      /*}}} */

void qarray_iter(qarray      *a,
                 const size_t startat,
                 const size_t stopat,
                 qthread_f    func)
{                                      /*{{{ */
    aligned_t                       donecount = 0;
    struct qarray_func_wrapper_args qfwa      = { { NULL }, a, NULL, &donecount, startat, stopat };

    qassert_retvoid((a != NULL));
    qassert_retvoid((func != NULL));
    qassert_retvoid((startat <= stopat));
    qfwa.func.qt = func;
    switch (a->dist_type) {
        case ALL_SAME:
            qthread_fork_to((qthread_f)qarray_strider, &qfwa, NULL,
                            a->dist_specific.dist_shep);
            while (donecount == 0) {
                qthread_yield();
            }
            break;
        case FIXED_FIELDS:
        {
            qthread_shepherd_id_t start_shep = qarray_shepof(a, startat);
            qthread_shepherd_id_t stop_shep  = qarray_shepof(a, stopat - 1);
            size_t                num_spawns = 0;
            for (qthread_shepherd_id_t s = start_shep; s <= stop_shep; s++) {
                qthread_fork_to((qthread_f)qarray_strider, &qfwa, NULL, s);
                num_spawns++;
            }
            while (donecount < num_spawns) {
                qthread_yield();
            }
            break;
        }
        default:
            /* it'd be NICE to avoid spawning if not all sheps are represented
             * in the range (esp if the range is small), but because of
             * DIST_RAND, that's impossible to do without checking all the
             * ranges ahead of time. By spawning threads that check their own
             * ranges, we essentially parallelize the task of figuring out
             * which threads to spawn (bizarre way of thinking about it, I
             * know). */
            if (stopat - startat < a->segment_size) {
                qthread_fork_to((qthread_f)qarray_strider, &qfwa, NULL,
                                qarray_shepof(a, startat));
                while (donecount == 0) {
                    qthread_yield();
                }
            } else {
                qthread_shepherd_id_t       i;
                const qthread_shepherd_id_t maxsheps =
                    qthread_num_shepherds();

                for (i = 0; i < maxsheps; i++) {
                    qthread_fork_to((qthread_f)qarray_strider, &qfwa, NULL,
                                    i);
                }
                while (donecount < maxsheps) {
                    qthread_yield();
                }
            }
            break;
    }
}                                      /*}}} */

void qarray_iter_loop(qarray      *a,
                      const size_t startat,
                      const size_t stopat,
                      qa_loop_f    func,
                      void        *arg)
{                                      /*{{{ */
    aligned_t                       donecount = 0;
    struct qarray_func_wrapper_args qfwa      = { { func }, a, arg, &donecount, startat, stopat };

    qassert_retvoid((a != NULL));
    qassert_retvoid((func != NULL));
    qassert_retvoid((startat <= stopat));
    switch (a->dist_type) {
        case ALL_SAME:
            qthread_fork_to((qthread_f)qarray_loop_strider, &qfwa, NULL,
                            a->dist_specific.dist_shep);
            while (donecount == 0) {
                qthread_yield();
            }
            break;
        case FIXED_FIELDS:
        {
            qthread_shepherd_id_t start_shep = qarray_shepof(a, startat);
            qthread_shepherd_id_t stop_shep  = qarray_shepof(a, stopat - 1);
            size_t                num_spawns = 0;
            for (qthread_shepherd_id_t s = start_shep; s <= stop_shep; s++) {
                qthread_fork_to((qthread_f)qarray_loop_strider, &qfwa, NULL,
                                s);
                num_spawns++;
            }
            while (donecount < num_spawns) {
                qthread_yield();
            }
            break;
        }
        default:
            /* it'd be NICE to avoid spawning if not all sheps are represented
             * in the range (esp if the range is small), but because of
             * DIST_RAND, that's impossible to do without checking all the
             * ranges ahead of time. By spawning threads that check their own
             * ranges, we essentially parallelize the task of figuring out
             * which threads to spawn (bizarre way of thinking about it, I
             * know). */
            if (stopat - startat < a->segment_size) {
                qthread_fork_to((qthread_f)qarray_loop_strider, &qfwa, NULL,
                                qarray_shepof(a, startat));
                while (donecount == 0) {
                    qthread_yield();
                }
            } else {
                qthread_shepherd_id_t       i;
                const qthread_shepherd_id_t maxsheps =
                    qthread_num_shepherds();

                for (i = 0; i < maxsheps; i++) {
                    qthread_fork_to((qthread_f)qarray_loop_strider, &qfwa,
                                    NULL, i);
                }
                while (donecount < maxsheps) {
                    qthread_yield();
                }
            }
            break;
    }
}                                      /*}}} */

struct qarray_ilnb_args {
    qarray   *a;
    size_t    startat;
    size_t    stopat;
    qa_loop_f func;
    void     *arg;
};

static aligned_t qarray_ilnb_wrapper(void *_args)
{   /*{{{*/
    struct qarray_ilnb_args *a = (struct qarray_ilnb_args *)_args;

    qarray_iter_loop(a->a, a->startat, a->stopat, a->func, a->arg);
    FREE(_args, sizeof(struct qarray_ilnb_args));
    return 0;
} /*}}}*/

void qarray_iter_loop_nb(qarray      *a,
                         const size_t startat,
                         const size_t stopat,
                         qa_loop_f    func,
                         void        *arg,
                         aligned_t   *ret)
{   /*{{{*/
    struct qarray_ilnb_args *qargs = MALLOC(sizeof(struct qarray_ilnb_args));

    qargs->a       = a;
    qargs->startat = startat;
    qargs->stopat  = stopat;
    qargs->func    = func;
    qargs->arg     = arg;
    qthread_fork_to(qarray_ilnb_wrapper, qargs, ret, qthread_shep());
} /*}}}*/

void qarray_iter_constloop(const qarray *a,
                           const size_t  startat,
                           const size_t  stopat,
                           qa_cloop_f    func,
                           void         *arg)
{                                      /*{{{ */
    aligned_t                                  donecount = 0;
    const struct qarray_constfunc_wrapper_args qfwa      = { { func }, a, arg, &donecount, startat, stopat };

    qassert_retvoid((a != NULL));
    qassert_retvoid((func != NULL));
    qassert_retvoid((startat <= stopat));
    switch (a->dist_type) {
        case ALL_SAME:
            qthread_fork_to((qthread_f)qarray_loop_strider, &qfwa, NULL, a->dist_specific.dist_shep);
            while (donecount == 0) {
                qthread_yield();
            }
            break;
        case FIXED_FIELDS:
        {
            qthread_shepherd_id_t start_shep = qarray_shepof(a, startat);
            qthread_shepherd_id_t stop_shep  = qarray_shepof(a, stopat - 1);
            size_t                num_spawns = 0;
            for (qthread_shepherd_id_t s = start_shep; s <= stop_shep; s++) {
                qthread_fork_to((qthread_f)qarray_loop_strider, &qfwa, NULL,
                                s);
                num_spawns++;
            }
            while (donecount < num_spawns) {
                qthread_yield();
            }
            break;
        }
        default:
            /* it'd be NICE to avoid spawning if not all sheps are represented
             * in the range (esp if the range is small), but because of
             * DIST_RAND, that's impossible to do without checking all the
             * ranges ahead of time. By spawning threads that check their own
             * ranges, we essentially parallelize the task of figuring out
             * which threads to spawn (bizarre way of thinking about it, I
             * know). */
            if (stopat - startat < a->segment_size) {
                qthread_fork_to((qthread_f)qarray_loop_strider, &qfwa, NULL,
                                qarray_shepof(a, startat));
                while (donecount == 0) {
                    qthread_yield();
                }
            } else {
                qthread_shepherd_id_t       i;
                const qthread_shepherd_id_t maxsheps =
                    qthread_num_shepherds();

                for (i = 0; i < maxsheps; i++) {
                    qthread_fork_to((qthread_f)qarray_loop_strider, &qfwa,
                                    NULL, i);
                }
                while (donecount < maxsheps) {
                    qthread_yield();
                }
            }
            break;
    }
}                                      /*}}} */

void qarray_iter_loopaccum(qarray      *a,
                           const size_t startat,
                           const size_t stopat,
                           qa_loopr_f   func,
                           void        *arg,
                           void        *ret,
                           const size_t retsize,
                           qt_accum_f   acc)
{                                      /*{{{ */
    qassert_retvoid((a != NULL));
    qassert_retvoid((func != NULL));
    qassert_retvoid((startat <= stopat));
    switch (a->dist_type) {
        case ALL_SAME:
        {
            struct qarray_accumfunc_wrapper_args qfwa =
            { { func }, acc, a, arg, ret, startat, stopat, retsize };
            aligned_t r;
            qthread_fork_to((qthread_f)qarray_loopaccum_strider, &qfwa, &r,
                            a->dist_specific.dist_shep);
            qthread_readFF(&r, &r);
            break;
        }
        case FIXED_FIELDS:
        {
            qthread_shepherd_id_t                 start_shep = qarray_shepof(a, startat);
            qthread_shepherd_id_t                 stop_shep  = qarray_shepof(a, stopat - 1);
            const size_t                          num_spawns = (stop_shep - start_shep) + 1;
            struct qarray_accumfunc_wrapper_args *qfwa       =
                MALLOC(sizeof(struct qarray_accumfunc_wrapper_args) *
                       num_spawns);
            char        *rets = MALLOC(retsize * (num_spawns - 1));
            aligned_t   *rv   = MALLOC(sizeof(aligned_t) * num_spawns);
            unsigned int i;

            assert(qfwa);
            assert(rets);
            assert(rv);
            qfwa[0].ret = ret;
            for (qthread_shepherd_id_t s = start_shep; s <= stop_shep; s++) {
                i               = s - start_shep;
                qfwa[i].func.ql = func;
                qfwa[i].acc     = acc;
                qfwa[i].a       = a;
                qfwa[i].arg     = arg;
                qfwa[i].startat = startat;
                qfwa[i].stopat  = stopat;
                qfwa[i].retsize = retsize;
                if (s > start_shep) {
                    qfwa[i].ret = rets + ((i - 1) * retsize);
                }
                qthread_fork_to((qthread_f)qarray_loopaccum_strider,
                                &qfwa[i], &rv[i], s);
            }
            for (i = 0; i < num_spawns; i++) {
                qthread_readFF(NULL, &(rv[i]));
                if (i > 0) {
                    acc(ret, &rets[i - 1]);
                }
            }
            FREE(qfwa, sizeof(struct qarray_accumfunc_wrapper_args) * num_spawns);
            FREE(rets, retsize * (num_spawns - 1));
            FREE(rv, sizeof(aligned_t) * num_spawns);
            break;
        }
        default:
            /* it'd be NICE to avoid spawning if not all sheps are represented
             * in the range (esp if the range is small), but because of
             * DIST_RAND, that's impossible to do without checking all the
             * ranges ahead of time. By spawning threads that check their own
             * ranges, we essentially parallelize the task of figuring out
             * which threads to spawn (bizarre way of thinking about it, I
             * know). */
        {
            qthread_shepherd_id_t                 i;
            const size_t                          segsize  = a->segment_size;
            const qthread_shepherd_id_t           maxsheps = qthread_num_shepherds();
            struct qarray_accumfunc_wrapper_args *qfwa     =
                MALLOC(sizeof(struct qarray_accumfunc_wrapper_args) * maxsheps);
            char      *rets = qt_calloc(maxsheps - 1, retsize);
            aligned_t *rv   = MALLOC(sizeof(aligned_t) * maxsheps);

            assert(qfwa);
            assert(rets);
            assert(rv);
            qfwa[0].ret = ret;
            if ((QT_CEIL_RATIO(a->count, segsize) /*tot_segs*/ /
                 maxsheps) > /*range_segs*/ ((stopat - startat) / segsize)) {
                /* If we have a small(ish) range, try to figure out which
                 * shepherds need to be spawned to rather than spawning to
                 * everyone */
                memset(rv, 1, sizeof(aligned_t) * maxsheps);
                size_t count_marked = 0;
                size_t start        = (startat / segsize) * segsize;
                for (size_t idx = start; idx < stopat; idx += segsize) {
                    qthread_shepherd_id_t shepof = qarray_shepof(a, idx);
                    if (rv[shepof]) {
                        rv[shepof] = 0;     // mark to spawn
                        count_marked++;
                    }
                    if(count_marked == maxsheps) { break; }
                }
                for (i = 0; i < maxsheps; i++) {
                    if(rv[i] == 0) {
                        qfwa[i].func.ql = func;
                        qfwa[i].acc     = acc;
                        qfwa[i].a       = a;
                        qfwa[i].arg     = arg;
                        qfwa[i].startat = startat;
                        qfwa[i].stopat  = stopat;
                        qfwa[i].retsize = retsize;
                        if (i > 0) {
                            qfwa[i].ret = rets + ((i - 1) * retsize);
                        }
                        qthread_fork_to((qthread_f)qarray_loopaccum_strider,
                                        &qfwa[i], &rv[i], i);
                    }
                }
                {
                    int first = 1;
                    for (i = 0; i < maxsheps; i++) {
                        if (rv[i] == 0) {
                            qthread_readFF(NULL, &(rv[i]));
                            if (first) {
                                first = 0;
                                if (i > 0) {
                                    memcpy(ret, rets + ((i - 1) * retsize), retsize);
                                }
                            } else {
                                acc(ret, rets + ((i - 1) * retsize));
                            }
                        }
                    }
                }
            } else {
                /* spawn to everyone, and let them sort it out */
                for (i = 0; i < maxsheps; i++) {
                    qfwa[i].func.ql = func;
                    qfwa[i].acc     = acc;
                    qfwa[i].a       = a;
                    qfwa[i].arg     = arg;
                    qfwa[i].startat = startat;
                    qfwa[i].stopat  = stopat;
                    qfwa[i].retsize = retsize;
                    if (i > 0) {
                        qfwa[i].ret = rets + ((i - 1) * retsize);
                    }
                    qthread_fork_to((qthread_f)qarray_loopaccum_strider,
                                    &qfwa[i], &rv[i], i);
                }
                for (i = 0; i < maxsheps; i++) {
                    qthread_readFF(NULL, &(rv[i]));
                    if (i > 0) {
                        acc(ret, rets + ((i - 1) * retsize));
                    }
                }
            }
            FREE(qfwa, sizeof(struct qarray_accumfunc_wrapper_args) * maxsheps);
            FREE(rets, retsize * (maxsheps - 1));
            FREE(rv, sizeof(aligned_t) * maxsheps);
            break;
        }
    }
}                                      /*}}} */

void qarray_set_shepof(qarray               *a,
                       const size_t          i,
                       qthread_shepherd_id_t shep)
{                                      /*{{{ */
    if ((a == NULL) || (i > a->count)) {
        return;
    }
    switch (a->dist_type) {
        case FIXED_FIELDS:
        case FIXED_HASH:
            return;

        case ALL_SAME:
            if (a->dist_specific.dist_shep != shep) {
                size_t segment_count = (a->count / a->segment_size);
                segment_count += (a->count % a->segment_size) ? 1 : 0;
#ifdef QTHREAD_HAVE_MEM_AFFINITY
                unsigned int target_node =
                    qthread_internal_shep_to_node(shep);
                if (target_node != QTHREAD_NO_NODE) {
                    size_t num_segments = a->count / a->segment_size;
                    size_t array_size   = a->segment_bytes * num_segments;
                    qt_affinity_mem_tonode(a->base_ptr, array_size, target_node);
                }
#elif defined(HAVE_MADVISE) && HAVE_DECL_MADV_ACCESS_LWP
                madvise(a->base_ptr,
                        (a->count / a->segment_size) * a->segment_bytes, MADV_ACCESS_LWP);
#endif      /* ifdef QTHREAD_HAVE_MEM_AFFINITY */
                qthread_incr(&chunk_distribution_tracker[shep],
                             segment_count);
                qthread_incr(&chunk_distribution_tracker
                             [a->dist_specific.dist_shep],
                             -1 * segment_count);
                a->dist_specific.dist_shep = shep;
            }
            return;

        case DIST:
        {
            size_t segment = i / a->segment_size;
            char  *seghead =
                qarray_elem_nomigrate(a, segment * a->segment_size);
            qthread_shepherd_id_t cur_shep =
                qarray_internal_segment_shep_read(a, seghead);
            assert(cur_shep < qthread_num_shepherds());
            if (cur_shep != shep) {
#ifdef QTHREAD_HAVE_MEM_AFFINITY
                unsigned int target_node =
                    qthread_internal_shep_to_node(shep);
                if (target_node != QTHREAD_NO_NODE) {
                    qt_affinity_mem_tonode(a->base_ptr +
                                           (a->segment_bytes * segment),
                                           a->segment_bytes, target_node);
                }
#elif defined(HAVE_MADVISE) && HAVE_DECL_MADV_ACCESS_LWP
                madvise(a->base_ptr + (a->segment_bytes * (i / a->segment_size)),
                        a->segment_bytes, MADV_ACCESS_LWP);
#endif          /* ifdef QTHREAD_HAVE_MEM_AFFINITY */
                qthread_incr(&chunk_distribution_tracker[shep], 1);
                qthread_incr(&chunk_distribution_tracker[cur_shep], -1);
                qarray_internal_segment_shep_write(a, seghead, shep);
            }
        }
            return;

        default:                       /* should never happen; cause segfault for corefile analysis */
            QTHREAD_TRAP();
            return;
    }
}                                      /*}}} */

void qarray_dist_like(const qarray *ref,
                      qarray       *mod)
{                                      /*{{{ */
    qassert_retvoid(ref->count == mod->count);
    qassert_retvoid(ref->unit_size == mod->unit_size);
    if (ref->dist_type == DIST) {
        qassert_retvoid(mod->dist_type != ALL_SAME);
    }
    switch (ref->dist_type) {
        case ALL_SAME:
        {
            qthread_shepherd_id_t shep = qarray_shepof(ref, 0);
            size_t                i;
            qassert_retvoid(mod->dist_type != FIXED_HASH);
            qassert_retvoid(mod->dist_type != FIXED_FIELDS);
            switch (mod->dist_type) {
                case ALL_SAME:
                    qarray_set_shepof(mod, 0, shep);
                    break;
                case DIST:
                    for (i = 0; i < mod->count; i += mod->segment_size) {
                        qarray_set_shepof(mod, i, shep);
                    }
                    break;
                default:               /* should not happen *ever*, so trigger a segfault for corefile analysis */
                    QTHREAD_TRAP();
            }
            break;
        }
        case DIST:
            qassert_retvoid(ref->segment_size == mod->segment_size);
            qassert_retvoid(ref->segment_bytes == mod->segment_bytes);
            qassert_retvoid(mod->dist_type != FIXED_HASH);
            qassert_retvoid(mod->dist_type != FIXED_FIELDS);
            if (mod->dist_type == DIST) {
                size_t i;
                for (i = 0; i < mod->count; i += mod->segment_size) {
                    qarray_set_shepof(mod, i, qarray_shepof(ref, i));
                }
            } else {
                /* should not happen *ever*, so trigger a segfault for corefile analysis */
                QTHREAD_TRAP();
            }
            break;
        default:                       /* should not happen *ever*, so trigger a segfault for corefile analysis */
            QTHREAD_TRAP();
        case FIXED_HASH:
        case FIXED_FIELDS:
#if 0
            /* theoretically, this should be possible... but it could be
             * complicated, and nobody wants it yet */
            if (mod->dist_type == ref->dist_type) {
                return;
            }
            qassert_retvoid(mod->dist_type == FIXED_HASH ||
                            mod->dist_type == FIXED_FIELDS);
#else
            qassert_retvoid(ref->dist_type != FIXED_HASH &&
                            ref->dist_type != FIXED_FIELDS);
#endif
    }
}                                      /*}}} */

/* vim:set expandtab: */
