#ifndef QTHREAD_QARRAY_H
#define QTHREAD_QARRAY_H

#include <stdint.h>

#include "macros.h"
#include "qloop.h"

Q_STARTCXX /* */
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
    DIST_STRIPES,
    DIST_FIELDS,
    DIST_RAND,
    DIST_LEAST,
    /* types of ALL_SAME... only used for input to qarray_create() */
    ALL_LOCAL,
    ALL_RAND,
    ALL_LEAST
  } distribution_t;

typedef struct qarray_s {
  size_t unit_size;
  size_t count;
  size_t segment_size; /* units in a segment */
  size_t
    segment_bytes; /* bytes per segment (sometimes > unit_size*segment_size) */
  char *base_ptr;
  distribution_t dist_type;

  union {
    qthread_shepherd_id_t dist_shep; /* for ALL_SAME dist type */

    struct {
      size_t segs_per_shep; /* for FIXED_STRIPES dist type */
      size_t extras;
    } stripes;
  } dist_specific;
} qarray;

typedef void (*qa_loop_f)(size_t const startat,
                          size_t const stopat,
                          qarray *array,
                          void *arg);
typedef void (*qa_loopr_f)(size_t const startat,
                           size_t const stopat,
                           qarray *array,
                           void *arg,
                           void *ret);
typedef void (*qa_cloop_f)(size_t const startat,
                           size_t const stopat,
                           qarray const *array,
                           void *arg);

qarray *qarray_create(size_t const count, size_t const unit_size);
qarray *qarray_create_tight(size_t const count, size_t const unit_size);
qarray *qarray_create_configured(size_t const count,
                                 size_t const unit_size,
                                 distribution_t const d,
                                 char const tight,
                                 int const seg_pages);

void qarray_destroy(qarray *a);
void qarray_iter(qarray *a,
                 size_t const startat,
                 size_t const stopat,
                 qthread_f func);
void qarray_iter_loop(qarray *a,
                      size_t const startat,
                      size_t const stopat,
                      qa_loop_f func,
                      void *arg);
void qarray_iter_loop_nb(qarray *a,
                         size_t const startat,
                         size_t const stopat,
                         qa_loop_f func,
                         void *arg,
                         aligned_t *ret);
void qarray_iter_constloop(qarray const *a,
                           size_t const startat,
                           size_t const stopat,
                           qa_cloop_f func,
                           void *arg);
void qarray_iter_loopaccum(qarray *a,
                           size_t const startat,
                           size_t const stopat,
                           qa_loopr_f func,
                           void *arg,
                           void *ret,
                           size_t const retsize,
                           qt_accum_f acc);

void qarray_set_shepof(qarray *a, size_t const i, qthread_shepherd_id_t shep);
qthread_shepherd_id_t qarray_shepof(qarray const *a, size_t const index);
void qarray_dist_like(qarray const *ref, qarray *mod);

#define qarray_elem(a, i) qarray_elem_nomigrate(a, i)
void *qarray_elem_migrate(qarray const *a, size_t const index);

inline static void *qarray_elem_nomigrate(qarray const *a, size_t const index) {
  if ((a == NULL) || (index > a->count)) { return NULL; }

  {
    size_t const segment_num = index / a->segment_size; /* rounded down */

    return a->base_ptr +
           ((segment_num * a->segment_bytes) +
            ((index - segment_num * a->segment_size) * a->unit_size));
  }
}

Q_ENDCXX
#endif // ifndef QTHREAD_QARRAY_H
/* vim:set expandtab: */
