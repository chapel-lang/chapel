#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Internal Headers */
#include "qt_asserts.h"
#include "qt_mpool.h"
#include "qthread/qpool.h"

qpool *qpool_create_aligned(size_t const isize, size_t alignment) { /*{{{ */
  return qt_mpool_create_aligned(isize, alignment);
} /*}}} */

qpool *qpool_create(size_t const item_size) { /*{{{ */
  return qpool_create_aligned(item_size, 0);
} /*}}} */

void *qpool_alloc(qpool *pool) { return qt_mpool_alloc(pool); }

void qpool_free(qpool *restrict pool, void *restrict mem) {
  qt_mpool_free(pool, mem);
}

void qpool_destroy(qpool *pool) { qt_mpool_destroy(pool); }

/* vim:set expandtab: */
