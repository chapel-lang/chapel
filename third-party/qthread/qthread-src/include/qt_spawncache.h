#ifndef QT_SPAWNCACHE_H
#define QT_SPAWNCACHE_H

#include "qt_visibility.h"
#include "qt_qthread_t.h" /* for qthread_t */

/* Internal Structures */
typedef struct _qt_threadqueue_node qt_threadqueue_node_t;
typedef struct _qt_threadqueue_private {
    qt_threadqueue_node_t          *head, *tail;
    qt_threadqueue_node_t          *on_deck;
    long                            qlength;
    long                            qlength_stealable;
    struct _qt_threadqueue_private *next;
} qt_threadqueue_private_t;

#include "qt_threadqueues.h"

void INTERNAL                      qt_spawncache_init(void);
qt_threadqueue_private_t INTERNAL *qt_init_local_spawncache(void);
qt_threadqueue_private_t INTERNAL *qt_spawncache_get(void);
int INTERNAL                       qt_spawncache_yield(qthread_t *t);
int INTERNAL                       qt_spawncache_flush(qt_threadqueue_t *q);
void INTERNAL                      qt_spawncache_filter(qt_threadqueue_filter_f f);
int INTERNAL                       qt_spawncache_spawn(qthread_t        *t,
                                                       qt_threadqueue_t *q);

#endif // ifndef QT_SPAWNCACHE_H
/* vim:set expandtab: */
