#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <pthread.h> /* for pthread_key_*() */
#include <string.h>  /* for memset() */

/* Internal Headers */
#include "qt_spawncache.h"
#include "qt_visibility.h"
#include "qt_debug.h"
#include "qthread_asserts.h"
#include "qthread_innards.h"
#include "qthread/cacheline.h"
#include "qt_macros.h"
#include "qt_aligned_alloc.h"

/* Globals */
TLS_DECL_INIT(qt_threadqueue_private_t *, spawn_cache);

/* Static Functions */
static void qt_spawncache_shutdown(void)
{
    qthread_debug(CORE_DETAILS, "destroy thread-local task queue\n");
    void *freeme = TLS_GET(spawn_cache);
    qthread_internal_aligned_free(freeme, qthread_cacheline());
    TLS_DELETE(spawn_cache);
}

#ifndef TLS
static void qt_threadqueue_private_destroy(void *q)
{
    assert(((qt_threadqueue_private_t *)q)->head == NULL &&
           ((qt_threadqueue_private_t *)q)->tail == NULL &&
           ((qt_threadqueue_private_t *)q)->qlength == 0 &&
           ((qt_threadqueue_private_t *)q)->qlength_stealable == 0);

    qthread_internal_aligned_free(q, qthread_cacheline());
}

#endif /* ifndef TLS */

/* Internal-only Functions */
void INTERNAL qt_spawncache_init(void)
{
    TLS_INIT2(spawn_cache, qt_threadqueue_private_destroy);
    qthread_internal_cleanup(qt_spawncache_shutdown);
}

qt_threadqueue_private_t INTERNAL *qt_init_local_spawncache(void)
{
    void *const ret = qthread_internal_aligned_alloc(sizeof(qt_threadqueue_private_t), qthread_cacheline());

    assert(ret);
    memset(ret, 0, sizeof(qt_threadqueue_private_t));

    TLS_SET(spawn_cache, ret);
    return (qt_threadqueue_private_t *)ret;
}

qt_threadqueue_private_t INTERNAL *qt_spawncache_get()
{
    return TLS_GET(spawn_cache);
}

int INTERNAL qt_spawncache_spawn(qthread_t        *t,
                                 qt_threadqueue_t *q)
{
    qt_threadqueue_private_t *cache = TLS_GET(spawn_cache);

    if (cache) {
        int ret = qt_threadqueue_private_enqueue(cache, q, t);
        if( !ret) {
            return ret;
        }
        return ret;
    } else {
        return 0;
    }
}

int INTERNAL qt_spawncache_yield(qthread_t *t)
{
    qt_threadqueue_private_t *cache = TLS_GET(spawn_cache);

    if (cache) {
        return qt_threadqueue_private_enqueue_yielded(cache, t);
    } else {
        return 0;
    }
}

/* vim:set expandtab: */
