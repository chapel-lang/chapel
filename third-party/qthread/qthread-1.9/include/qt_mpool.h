#ifndef QT_MPOOL_H
#define QT_MPOOL_H

#include <stddef.h>                    /* for size_t (according to C89) */

typedef struct qt_mpool_s *qt_mpool;

void *qt_mpool_alloc(qt_mpool pool);

void qt_mpool_free(qt_mpool pool,
                   void    *mem);

#define qt_mpool_create(item_size) qt_mpool_create_aligned((item_size), 0)

qt_mpool qt_mpool_create_aligned(size_t       item_size,
                                 const size_t alignment);
void qt_mpool_destroy(qt_mpool pool);

void qt_mpool_subsystem_init(void);

#endif // ifndef QT_MPOOL_H
/* vim:set expandtab: */
