#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <stdio.h>                     /* debugging */
#include "qt_mpool.h"
#include "qt_atomics.h"
#include "qthread_expect.h"
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
# define VALGRIND_CREATE_MEMPOOL(a, b, c)
# define VALGRIND_MEMPOOL_ALLOC(a, b, c)
# define VALGRIND_MEMPOOL_FREE(a, b)
#endif

#include <qthread/qthread-int.h>       /* for uintptr_t */
#include "qthread_asserts.h"
#include "qt_gcd.h"                    /* for qt_lcm() */

#ifdef HAVE_GETPAGESIZE
# include <unistd.h>
#else
static QINLINE int getpagesize()
{
    return 4096;
}

#endif

#ifdef QTHREAD_MUTEX_INCREMENT
# define ASM_ALLOWED(x)
#else
# define ASM_ALLOWED(x) x
#endif

struct qt_mpool_s {
    size_t         item_size;
    size_t         alloc_size;
    size_t         items_per_alloc;
    size_t         alignment;

    void *volatile QTHREAD_CASLOCK(reuse_pool);
    QTHREAD_FASTLOCK_TYPE pool_lock;
    char                 *alloc_block;
    char *volatile QTHREAD_CASLOCK(alloc_block_ptr);
    void                **alloc_list;
    size_t                alloc_list_pos;
};

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
            VALGRIND_MAKE_MEM_NOACCESS(ret, alloc_size);
            return ret;

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
#elif defined(HAVE_16ALIGNED_CALLOC)
            return calloc(1, alloc_size);

#elif defined(HAVE_16ALIGNED_MALLOC)
            ret = malloc(alloc_size);
#elif defined(HAVE_PAGE_ALIGNED_MALLOC)
            ret = malloc(alloc_size);
#else /* if defined(HAVE_MEMALIGN) */
            ret = valloc(alloc_size);  /* cross your fingers */
#endif /* if defined(HAVE_MEMALIGN) */
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
#ifndef QTHREAD_USE_ROSE_EXTENSIONS
    memset(ret, 0, alloc_size);
#endif
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
qt_mpool qt_mpool_create_aligned(size_t    item_size,
                                 size_t    alignment)
{                                      /*{{{ */
    qt_mpool pool = (qt_mpool)calloc(1, sizeof(struct qt_mpool_s));

    size_t alloc_size = 0;

    qassert_ret((pool != NULL), NULL);
    VALGRIND_CREATE_MEMPOOL(pool, 0, 0);
    if (pagesize == 0) {
        pagesize = getpagesize();
    }
    assert(pagesize > 0);
    /* first, we ensure that item_size is at least sizeof(void*), and also that
     * it is a multiple of sizeof(void*). The second condition technically
     * implies the first, but it's not a big deal. */
    if (item_size < sizeof(void *)) {
        item_size = sizeof(void *);
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
    pool->alloc_size = alloc_size;

    pool->items_per_alloc = alloc_size / item_size;

    QTHREAD_CASLOCK_INIT(pool->reuse_pool, NULL);
    QTHREAD_FASTLOCK_INIT(pool->pool_lock);
    pool->alloc_block = (char *)qt_mpool_internal_aligned_alloc(alloc_size,
                                                                alignment);
    assert(((unsigned long)(pool->alloc_block) & (alignment - 1)) == 0);
    qassert_goto((pool->alloc_block != NULL), errexit);
    *(void **)(pool->alloc_block) = NULL; // just in case aligned_alloc doesn't memset
    pool->alloc_block_ptr         = pool->alloc_block;
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
# define QPTR(x)        ((void *volatile *)(((uintptr_t)(x))& ~(uintptr_t)QCTR_MASK))
# define QCTR(x)        (((uintptr_t)(x))&QCTR_MASK)
# define QCOMPOSE(x, y) (void *)(((uintptr_t)QPTR(x)) | ((QCTR(y) + 1)&QCTR_MASK))
#else
# define QPTR(x)        (x)
# define QCTR(x)        0
# define QCOMPOSE(x, y) (x)
#endif

void *qt_mpool_alloc(qt_mpool pool)
{                                      /*{{{ */
    void **p = (void **)QTHREAD_CASLOCK_READ(pool->reuse_pool);

    qassert_ret((pool != NULL), NULL);
    if (QPTR(p) != NULL) {
        void *old, *new;

        /* note that this is only "safe" as long as there is no chance that the
         * pool has been destroyed. There's a small chance that p was allocated
         * and popped back into the queue so that the CAS works but "new" is
         * the wrong value... (also known among lock-free wonks as The ABA
         * Problem). Using the QPTR stuff reduces this chance to an acceptable
         * probability.
         */
        do {
            old = p;
            VALGRIND_MAKE_MEM_DEFINED(QPTR(p), pool->item_size);
            new = *(QPTR(p));
            p   = QT_CAS(pool->reuse_pool, old, QCOMPOSE(new, p));
        } while (p != old && QPTR(p) != NULL);
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
        if (QTHREAD_EXPECT(cur == max + pool->item_size, 0)) {
            while (pool->alloc_block_ptr == cur) ;
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
            ASM_ALLOWED(__asm__ __volatile__ ("" ::: "memory"));
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

void qt_mpool_free(qt_mpool pool,
                   void    *mem)
{                                      /*{{{ */
    void *p, *old, *new;

    qassert_retvoid((mem != NULL));
    qassert_retvoid((pool != NULL));
    do {
        old                    = (void *)QTHREAD_CASLOCK_READ(pool->reuse_pool); /* should be an atomic read */
        *(void *volatile *)mem = old;
        new                    = QCOMPOSE(mem, old);
        p                      = QT_CAS(pool->reuse_pool, old, new);
    } while (p != old);
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
    QTHREAD_CASLOCK_DESTROY(pool->reuse_pool);
    VALGRIND_DESTROY_MEMPOOL(pool);
    free(pool);
}                                      /*}}} */

/* vim:set expandtab: */
