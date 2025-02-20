/* Internal Headers */
#include "qthread/qpool.h"
#include "qt_asserts.h"
#include "qt_mpool.h"
#include "qt_visibility.h"

API_FUNC qpool *qpool_create_aligned(size_t const isize, size_t alignment) {
  return qt_mpool_create_aligned(isize, alignment);
}

API_FUNC qpool *qpool_create(size_t const item_size) {
  return qpool_create_aligned(item_size, 0);
}

API_FUNC void *qpool_alloc(qpool *pool) { return qt_mpool_alloc(pool); }

API_FUNC void qpool_free(qpool *restrict pool, void *restrict mem) {
  qt_mpool_free(pool, mem);
}

API_FUNC void qpool_destroy(qpool *pool) { qt_mpool_destroy(pool); }

/* vim:set expandtab: */
