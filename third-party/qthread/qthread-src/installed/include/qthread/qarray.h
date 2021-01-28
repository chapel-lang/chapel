#ifndef QTHREAD_QARRAY_H
#define QTHREAD_QARRAY_H

#include "qthread-int.h"
#include "macros.h"
#include "qloop.h"

Q_STARTCXX                             /* */
typedef enum {
    /* the default, used both as input and after creation */
    FIXED_HASH = 0,
    /* these are also used both as input and after creation */
    FIXED_FIELDS,
    /* these are used only after it's created */
    ALL_SAME /* if used as input to qarray_create(), treated as ALL_LOCAL */,
    DIST /* if used as input to qarray_create(), treated as DIST_RAND */,
    /* types of DIST... only used for input to qarray_create();
     * note that these only establish an initial condition. */
    DIST_STRIPES, DIST_FIELDS, DIST_RAND, DIST_LEAST,
    /* types of ALL_SAME... only used for input to qarray_create() */
    ALL_LOCAL, ALL_RAND, ALL_LEAST
} distribution_t;
typedef struct qarray_s {
    size_t         unit_size;
    size_t         count;
    size_t         segment_size;  /* units in a segment */
    size_t         segment_bytes; /* bytes per segment (sometimes > unit_size*segment_size) */
    char          *base_ptr;
    distribution_t dist_type;
    union {
        qthread_shepherd_id_t dist_shep;        /* for ALL_SAME dist type */
        struct {
            size_t segs_per_shep;                       /* for FIXED_STRIPES dist type */
            size_t extras;
        } stripes;
    } dist_specific;
} qarray;

typedef void (*qa_loop_f)(const size_t startat,
                          const size_t stopat,
                          qarray      *array,
                          void        *arg);
typedef void (*qa_loopr_f)(const size_t startat,
                           const size_t stopat,
                           qarray      *array,
                           void        *arg,
                           void        *ret);
typedef void (*qa_cloop_f)(const size_t  startat,
                           const size_t  stopat,
                           const qarray *array,
                           void         *arg);

qarray *qarray_create(const size_t count,
                      const size_t unit_size);
qarray *qarray_create_tight(const size_t count,
                            const size_t unit_size);
qarray *qarray_create_configured(const size_t         count,
                                 const size_t         unit_size,
                                 const distribution_t d,
                                 const char           tight,
                                 const int            seg_pages);

void qarray_destroy(qarray *a);
void qarray_iter(qarray      *a,
                 const size_t startat,
                 const size_t stopat,
                 qthread_f    func);
void qarray_iter_loop(qarray      *a,
                      const size_t startat,
                      const size_t stopat,
                      qa_loop_f    func,
                      void        *arg);
void qarray_iter_loop_nb(qarray      *a,
                         const size_t startat,
                         const size_t stopat,
                         qa_loop_f    func,
                         void        *arg,
                         aligned_t   *ret);
void qarray_iter_constloop(const qarray *a,
                           const size_t  startat,
                           const size_t  stopat,
                           qa_cloop_f    func,
                           void         *arg);
void qarray_iter_loopaccum(qarray      *a,
                           const size_t startat,
                           const size_t stopat,
                           qa_loopr_f   func,
                           void        *arg,
                           void        *ret,
                           const size_t retsize,
                           qt_accum_f   acc);

void qarray_set_shepof(qarray               *a,
                       const size_t          i,
                       qthread_shepherd_id_t shep);
qthread_shepherd_id_t qarray_shepof(const qarray *a,
                                    const size_t  index);
void qarray_dist_like(const qarray *ref,
                      qarray       *mod);

#define qarray_elem(a, i) qarray_elem_nomigrate(a, i)
void *qarray_elem_migrate(const qarray *a,
                          const size_t  index);
QINLINE static void *qarray_elem_nomigrate(const qarray *a,
                                           const size_t  index)
{
    if ((a == NULL) || (index > a->count)) {
        return NULL;
    }

    {
        const size_t segment_num = index / a->segment_size;     /* rounded down */

        return a->base_ptr + ((segment_num * a->segment_bytes) + ((index - segment_num * a->segment_size) * a->unit_size));
    }
}

Q_ENDCXX
#endif // ifndef QTHREAD_QARRAY_H
/* vim:set expandtab: */
