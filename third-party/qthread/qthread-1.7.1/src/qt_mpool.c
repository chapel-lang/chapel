#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Includes */
#include <pthread.h>

#include <stddef.h>                    /* for size_t (according to C89) */
#include <stdlib.h>                    /* for calloc() and malloc() */
#if (HAVE_MEMALIGN && HAVE_MALLOC_H)
# include <malloc.h>                   /* for memalign() */
#endif
#include <string.h>

#ifdef QTHREAD_USE_VALGRIND
# include <valgrind/memcheck.h>
#else
# define VALGRIND_DESTROY_MEMPOOL(a)
# define VALGRIND_MAKE_MEM_NOACCESS(a, b)
# define VALGRIND_MAKE_MEM_DEFINED(a, b)
# define VALGRIND_MAKE_MEM_UNDEFINED(a, b)
# define VALGRIND_CREATE_MEMPOOL(a, b, c)
# define VALGRIND_MEMPOOL_ALLOC(a, b, c)
# define VALGRIND_MEMPOOL_FREE(a, b)
#endif

/* Internal Includes */
#include <qthread/qthread-int.h>       /* for uintptr_t */
#include "qt_mpool.h"
#include "qt_atomics.h"
#include "qthread_expect.h"
#include "qthread_asserts.h"
#include "qt_debug.h"
#include "qt_gcd.h"                    /* for qt_lcm() */

#ifdef HAVE_GETPAGESIZE
# include <unistd.h>
#else
static QINLINE int getpagesize()
{
    return 4096;
}

#endif

struct qt_mpool_s {
    size_t                item_size;
    size_t                alloc_size;
    size_t                items_per_alloc;
    size_t                alignment;

    pthread_key_t         threadlocal_cache;

    QTHREAD_FASTLOCK_TYPE reuse_lock;
    void                 *reuse_pool;

    char                 *alloc_block;
    char                 *QTHREAD_CASLOCK(alloc_block_ptr);

    QTHREAD_FASTLOCK_TYPE pool_lock;
    void                **alloc_list;
    size_t                alloc_list_pos;
};

typedef struct qt_mpool_cache_entry_s {
    struct qt_mpool_cache_entry_s *next;
    struct qt_mpool_cache_entry_s *block_tail;
    uint8_t                        data[];
} qt_mpool_cache_t;

typedef struct threadlocal_cache_s {
    qt_mpool_cache_t *cache;
    size_t count;
} qt_mpool_threadlocal_cache_t;

/* local constants */
static size_t pagesize = 0;

/* local funcs */
static QINLINE void *qt_mpool_internal_aligned_alloc(size_t alloc_size,
                                                     size_t alignment)
{                                      /*{{{ */
    void *ret;

    switch (alignment) {
        case 0:
            ret = calloc(1, alloc_size);
            goto return_clean;
        case 16:
        case 8:
        case 4:
        case 2:
#if defined(HAVE_MEMALIGN)
            ret = memalign(16, alloc_size);
#elif defined(HAVE_POSIX_MEMALIGN)
            posix_memalign(&(ret), 16, alloc_size);
#elif defined(HAVE_WORKING_VALLOC)
            ret = valloc(alloc_size);
#elif defined(HAVE_16ALIGNED_MALLOC) || defined(HAVE_PAGE_ALIGNED_MALLOC)
            ret = malloc(alloc_size);
#else                                 /* if defined(HAVE_MEMALIGN) */
            ret = valloc(alloc_size); /* cross your fingers */
#endif  /* if defined(HAVE_MEMALIGN) */
            break;
        default:
#ifdef HAVE_MEMALIGN
            ret = memalign(alignment, alloc_size);
#elif defined(HAVE_POSIX_MEMALIGN)
            posix_memalign(&(ret), alignment, alloc_size);
#elif defined(HAVE_WORKING_VALLOC)
            ret = valloc(alloc_size);
#elif defined(HAVE_PAGE_ALIGNED_MALLOC)
            ret = malloc(alloc_size);
#else
            ret = valloc(alloc_size);  /* cross your fingers */
#endif
    }
return_clean:
    VALGRIND_MAKE_MEM_NOACCESS(ret, alloc_size);
    return ret;
}                                      /*}}} */

static QINLINE void qt_mpool_internal_aligned_free(void        *freeme,
                                                   /*const size_t alloc_size, */
                                                   const size_t alignment)
{                                      /*{{{ */
    switch (alignment) {
        case 0:
            free(freeme);
            return;

        default:
#if defined(HAVE_16_ALIGNED_CALLOC) ||           \
            defined(HAVE_16_ALIGNED_MALLOC) ||   \
            defined(HAVE_MEMALIGN) ||            \
            defined(HAVE_PAGE_ALIGNED_MALLOC) || \
            defined(HAVE_POSIX_MEMALIGN) ||      \
            defined(HAVE_WORKING_VALLOC)
            free(freeme);
#endif
            return;
    }
}                                      /*}}} */

// sync means lock-protected
// item_size is how many bytes to return
// ...memory is always allocated in multiples of getpagesize()
qt_mpool qt_mpool_create_aligned(size_t item_size,
                                 size_t alignment)
{                                      /*{{{ */
    qt_mpool pool = (qt_mpool)calloc(1, sizeof(struct qt_mpool_s));

    size_t alloc_size = 0;

    qassert_ret((pool != NULL), NULL);
    VALGRIND_CREATE_MEMPOOL(pool, 0, 0);
    if (pagesize == 0) {
        pagesize = getpagesize();
    }
    assert(pagesize > 0);
    /* first, we ensure that item_size is at least sizeof(qt_mpool_cache_t), and also that
     * it is a multiple of sizeof(void*). */
    if (item_size < sizeof(qt_mpool_cache_t)) {
        item_size = sizeof(qt_mpool_cache_t);
    }
    if (item_size % sizeof(void *)) {
        item_size += (sizeof(void *)) - (item_size % sizeof(void *));
    }
    if (alignment <= 16) {
        alignment = 16;
    }
    if (item_size % alignment) {
        item_size += alignment - (item_size % alignment);
    }
    pool->item_size = item_size;
    pool->alignment = alignment;
    /* next, we find the least-common-multiple in sizes between item_size and
     * pagesize. If this is less than ten items (an arbitrary number), we
     * increase the alloc_size until it is at least that big. This guarantees
     * that the allocation size will be a multiple of pagesize (fast!
     * efficient!). */
    alloc_size = qt_lcm(item_size, pagesize);
    if (alloc_size == 0) {             /* degenerative case */
        if (item_size > pagesize) {
            alloc_size = item_size;
        } else {
            alloc_size = pagesize;
        }
    } else {
        while (alloc_size / item_size < 128) {
            alloc_size *= 2;
        }
        while (alloc_size < pagesize * 16) {
            alloc_size *= 2;
        }
    }
    pool->alloc_size      = alloc_size;
    pool->items_per_alloc = alloc_size / item_size;
    pool->reuse_pool      = NULL;
    QTHREAD_FASTLOCK_INIT(pool->reuse_lock);
    QTHREAD_FASTLOCK_INIT(pool->pool_lock);
    pthread_key_create(&pool->threadlocal_cache, free);
    pool->alloc_block = (char *)qt_mpool_internal_aligned_alloc(alloc_size,
                                                                alignment);
    assert(((unsigned long)(pool->alloc_block) & (alignment - 1)) == 0);
    qassert_goto((pool->alloc_block != NULL), errexit);
    VALGRIND_MAKE_MEM_DEFINED(pool->alloc_block, sizeof(void **));
    *(void **)(pool->alloc_block) = NULL; // just in case aligned_alloc doesn't memset
    VALGRIND_MAKE_MEM_NOACCESS(pool->alloc_block, sizeof(void **));
    QTHREAD_CASLOCK_INIT(pool->alloc_block_ptr, pool->alloc_block);
    /* this assumes that pagesize is a multiple of sizeof(void*) */
    pool->alloc_list = calloc(1, pagesize);
    qassert_goto((pool->alloc_list != NULL), errexit);
    pool->alloc_list[0]  = pool->alloc_block;
    pool->alloc_list_pos = 1;
    return pool;

    qgoto(errexit);
    if (pool) {
        if (pool->alloc_block) {
            qt_mpool_internal_aligned_free(pool->alloc_block, /* alloc_size, */ alignment);
        }
        free(pool);
    }
    return NULL;
}                                      /*}}} */

qt_mpool qt_mpool_create(size_t item_size)
{                                      /*{{{ */
    return qt_mpool_create_aligned(item_size, 0);
}                                      /*}}} */

/* to avoid ABA reinsertion trouble, each pointer in the pool needs to have a
 * monotonically increasing counter associated with it. The counter doesn't
 * need to be huge, just big enough to make the APA problem sufficiently
 * unlikely. We'll just claim 4, to be conservative. Thus, an allocated block
 * of memory must be aligned to 16 bytes. */
#ifndef QTHREAD_USE_VALGRIND
# define QCTR_MASK (15)
# define QPTR(x)        ((void **)(((uintptr_t)(x)) & ~(uintptr_t)QCTR_MASK))
# define QCTR(x)        (((uintptr_t)(x)) &QCTR_MASK)
# define QCOMPOSE(x, y) (void *)(((uintptr_t)QPTR(x)) | ((QCTR(y) + 1) &QCTR_MASK))
#else
# define QPTR(x)        (x)
# define QCTR(x)        0
# define QCOMPOSE(x, y) (x)
#endif

void *qt_mpool_cached_alloc(qt_mpool pool)
{
    qt_mpool_threadlocal_cache_t *tc;
    qt_mpool_cache_t *cache = NULL;
    size_t cnt;

    qassert_ret((pool != NULL), NULL);

    tc = pthread_getspecific(pool->threadlocal_cache);
    if (NULL == tc) {
        tc = calloc(1, sizeof(qt_mpool_threadlocal_cache_t));
        assert(tc);
        pthread_setspecific(pool->threadlocal_cache, tc);
    }
    cache = tc->cache;
    qthread_debug(MPOOL_CALLS, "->cache:%p cnt:%u\n", cache, (unsigned int)tc->count);
    if (cache) {
        void *ret = (void *)cache;
        cache = cache->next;
        qthread_debug(MPOOL_BEHAVIOR, "->...cached count:%zu\n", (size_t)tc->count - 1);
        tc->cache = cache;
        --tc->count;
        return ret;
    } else {
        const size_t items_per_alloc = pool->items_per_alloc;
        cnt   = tc->count;
        /* cache is empty; need to fill it */
        assert(cnt == 0);
        if (pool->reuse_pool) { // global cache
            qthread_debug(MPOOL_DETAILS, "->...pull from reuse\n");
            QTHREAD_FASTLOCK_LOCK(&pool->reuse_lock);
            if (pool->reuse_pool) {
                cache                   = pool->reuse_pool;
                pool->reuse_pool        = cache->block_tail->next;
                cache->block_tail->next = NULL;
            }
            QTHREAD_FASTLOCK_UNLOCK(&pool->reuse_lock);
            cnt = items_per_alloc;
        }
        if (NULL == cache) {
            uint8_t *p;
            const size_t item_size = pool->item_size;

            /* need to allocate a new block and record that I did so in the central pool */
            qthread_debug(MPOOL_DETAILS, "->...allocating new block\n");
            p = qt_mpool_internal_aligned_alloc(pool->alloc_size,
                                                pool->alignment);
            qassert_ret((p != NULL), NULL);
            assert(pool->alignment == 0 ||
                   (((uintptr_t)p) & (pool->alignment - 1)) == 0);
            QTHREAD_FASTLOCK_LOCK(&pool->pool_lock);
            if (pool->alloc_list_pos == (pagesize / sizeof(void *) - 1)) {
                void **tmp = calloc(1, pagesize);
                qassert_ret((tmp != NULL), NULL);
                tmp[pagesize / sizeof(void *) - 1] = pool->alloc_list;
                pool->alloc_list                   = tmp;
                pool->alloc_list_pos               = 0;
            }
            pool->alloc_list[pool->alloc_list_pos] = p;
            pool->alloc_list_pos++;
            QTHREAD_FASTLOCK_UNLOCK(&pool->pool_lock);
            /* set all the pointers for this block of memory */
            {
                qt_mpool_cache_t *const block_tail = (qt_mpool_cache_t *)&p[(items_per_alloc - 1) * item_size];
                for (size_t i = 1; i < items_per_alloc; ++i) {
                    cache = (qt_mpool_cache_t *)&p[i * item_size];
                    cache->next = (qt_mpool_cache_t *)&p[(i + 1) * item_size];
                    cache->block_tail = block_tail;
                }
                cache->next = NULL; // last one (this logic eliminates a branch in the loop)
                // now for a quick sanity check
                cache = (qt_mpool_cache_t *)&p[item_size];
                assert(cache);
                assert(cache->next);
                assert(cache->block_tail);
                assert(cache->block_tail->next == NULL);
            }
            qthread_debug(MPOOL_BEHAVIOR, "->...new_malloc count:%zu (alloc'd:%zu)\n", (size_t)(items_per_alloc - 1), items_per_alloc);
            tc->cache = (qt_mpool_cache_t *)&p[item_size];
            tc->count = items_per_alloc - 1;
            return p;
        } else {
            qthread_debug(MPOOL_BEHAVIOR, "->...from_global_pool count:%zu\n", (size_t)(cnt - 1));
            tc->cache = cache->next;
            tc->count = cnt - 1;
            cache->next       = NULL; // unnecessary
            cache->block_tail = NULL; // unnecessary
            return cache;
        }
    }
}

void *qt_mpool_alloc(qt_mpool pool)
{                                      /*{{{ */
    void **p = NULL;

    qassert_ret((pool != NULL), NULL);
    if (pool->reuse_pool) {
        QTHREAD_FASTLOCK_LOCK(&pool->reuse_lock);
        if (pool->reuse_pool) {
            p                = pool->reuse_pool;
            pool->reuse_pool = *(uint64_t **)p;
        } else { p = NULL; }
        QTHREAD_FASTLOCK_UNLOCK(&pool->reuse_lock);
    }

    if (QPTR(p) == NULL) {             /* this is not an else on purpose */
        char *base, *max, *cur;
start:
        QTHREAD_FASTLOCK_LOCK(&pool->pool_lock);
        base = pool->alloc_block;
        max  = base + (pool->item_size * pool->items_per_alloc);
        cur  = pool->alloc_block_ptr;
        QTHREAD_FASTLOCK_UNLOCK(&pool->pool_lock);
        while (cur <= max && cur >= base) {
            char *cur2 = QT_CAS(pool->alloc_block_ptr, cur, cur + pool->item_size);
            if (cur2 == cur) { break; }
            cur = cur2;
            if (base != pool->alloc_block) { goto start; }
        }
        if (QTHREAD_EXPECT(cur > max, 0)) {
            goto start;
        } else if (QTHREAD_EXPECT(cur == max, 0)) {
            /* realloc */
            if (pool->alloc_list_pos == (pagesize / sizeof(void *) - 1)) {
                void **tmp = calloc(1, pagesize);

                qassert_goto((tmp != NULL), alloc_exit);
                tmp[pagesize / sizeof(void *) - 1] = pool->alloc_list;
                pool->alloc_list                   = tmp;
                pool->alloc_list_pos               = 0;
            }
            p = qt_mpool_internal_aligned_alloc(pool->alloc_size,
                                                pool->alignment);
            qassert_ret((p != NULL), NULL);
            qassert_ret((QCTR(p) == 0), NULL);
            assert(pool->alignment == 0 ||
                   (((unsigned long)p) & (pool->alignment - 1)) == 0);
            QTHREAD_FASTLOCK_LOCK(&pool->pool_lock);
            pool->alloc_block                      = (void *)p;
            pool->alloc_list[pool->alloc_list_pos] = pool->alloc_block;
            pool->alloc_list_pos++;
            VALGRIND_MAKE_MEM_UNDEFINED(p, pool->item_size);
            MACHINE_FENCE;
            pool->alloc_block_ptr = ((char *)p) + pool->item_size;
            QTHREAD_FASTLOCK_UNLOCK(&pool->pool_lock);
        } else {
            p = (void **)cur;
        }
    }
    qgoto(alloc_exit);
    VALGRIND_MEMPOOL_ALLOC(pool, QPTR(p), pool->item_size);
    return (void *)QPTR(p);
}                                      /*}}} */

void qt_mpool_cached_free(qt_mpool pool,
                          void    *mem)
{
    qt_mpool_threadlocal_cache_t *tc;
    qt_mpool_cache_t *cache = NULL;
    qt_mpool_cache_t *n     = (qt_mpool_cache_t *)mem;
    uintptr_t         cnt;

    qassert_retvoid((mem != NULL));
    qassert_retvoid((pool != NULL));
    tc = pthread_getspecific(pool->threadlocal_cache);
    if (NULL == tc) {
        tc = calloc(1, sizeof(qt_mpool_threadlocal_cache_t));
        assert(tc);
        pthread_setspecific(pool->threadlocal_cache, tc);
    }
    cache = tc->cache;
    cnt   = tc->count;
    qthread_debug(MPOOL_CALLS, "->cache:%p cnt:%u\n", cache, (unsigned int)cnt);
    if (cache) {
        assert(cnt != 0);
        n->next       = cache;
        n->block_tail = cache->block_tail;
    } else {
        assert(cnt == 0);
        n->next       = NULL;
        n->block_tail = n;
    }
    cnt++;
    if (cnt >= (pool->items_per_alloc * 2)) {
        qt_mpool_cache_t *toglobal;
        /* push to global */
        qthread_debug(MPOOL_DETAILS, "->push to global! cnt:%u\n", (unsigned)cnt);
        assert(n);
        assert(n->block_tail);
        toglobal            = n->block_tail->next;
        n->block_tail->next = NULL;
        assert(toglobal);
        assert(toglobal->block_tail);
        QTHREAD_FASTLOCK_LOCK(&pool->reuse_lock);
        toglobal->block_tail->next = pool->reuse_pool;
        pool->reuse_pool           = toglobal;
        QTHREAD_FASTLOCK_UNLOCK(&pool->reuse_lock);
        cnt -= pool->items_per_alloc;
    }
    if (cnt == pool->items_per_alloc + 1) {
        qthread_debug(MPOOL_DETAILS, "->chop_block\n");
        n->block_tail = n;
    }
    tc->cache = n;
    tc->count = cnt;
    qthread_debug(MPOOL_BEHAVIOR, "->free count = %zu\n", (size_t)cnt);
    VALGRIND_MEMPOOL_FREE(pool, mem);
}

void qt_mpool_free(qt_mpool pool,
                   void    *mem)
{                                      /*{{{ */
    qassert_retvoid((mem != NULL));
    qassert_retvoid((pool != NULL));
    QTHREAD_FASTLOCK_LOCK(&pool->reuse_lock);
    *(void **)mem    = pool->reuse_pool;
    pool->reuse_pool = mem;
    QTHREAD_FASTLOCK_UNLOCK(&pool->reuse_lock);
    VALGRIND_MEMPOOL_FREE(pool, mem);
}                                      /*}}} */

void qt_mpool_destroy(qt_mpool pool)
{                                      /*{{{ */
    qassert_retvoid((pool != NULL));
    while (pool->alloc_list) {
        unsigned int i = 0;

        void *p = pool->alloc_list[0];

        while (p && i < (pagesize / sizeof(void *) - 1)) {
            qt_mpool_internal_aligned_free((void *)QPTR(p),     /* pool->alloc_size, */
                                           pool->alignment);
            i++;
            p = pool->alloc_list[i];
        }
        p                = pool->alloc_list;
        pool->alloc_list = pool->alloc_list[pagesize / sizeof(void *) - 1];
        free(p);
    }
    pthread_key_delete(pool->threadlocal_cache);
    QTHREAD_FASTLOCK_DESTROY(pool->pool_lock);
    QTHREAD_FASTLOCK_DESTROY(pool->reuse_lock);
    VALGRIND_DESTROY_MEMPOOL(pool);
    free(pool);
}                                      /*}}} */

/* vim:set expandtab: */
