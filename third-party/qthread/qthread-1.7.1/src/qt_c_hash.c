#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include "qt_visibility.h"
#include "qt_hash.h"
#include "qthread_asserts.h"
#include "qthread_prefetch.h"
#include "qt_atomics.h"
#include "qthread/cacheline.h"
#include <stdlib.h>
#include <unistd.h>                    /* for getpagesize() */
#if (HAVE_MEMALIGN && HAVE_MALLOC_H)
# include <malloc.h>
#endif

#ifdef HAVE_GETPAGESIZE
# include <unistd.h>
#else
static QINLINE int getpagesize(void)
{
    return 4096;
}

#endif

#ifndef QT_HASH_CAST
# define QT_HASH_CAST qt_key_t
#endif

typedef struct {
    qt_key_t key;
    void    *value;
} hash_entry;

struct qt_hash_s {
    QTHREAD_FASTLOCK_TYPE *lock;
    hash_entry            *entries;
    uint64_t               mask;
    size_t                 num_entries;
    size_t                 population, deletes;
    size_t                 grow_size, shrink_size, tidy_up_size; // cache for speed
    void                  *value[2];                             // handle out-of-bound values
    short                  has_key[2];
};

static size_t linesize = 0;
static size_t bucketsize;
static size_t bucketmask;
#define KEY_NULL    ((qt_key_t)0)
#define KEY_DELETED ((qt_key_t)1)

static inline size_t encompassing_power_of_two(size_t k)
{   /*{{{*/
    size_t z = 1;

    while (z < k) z <<= 1;
    assert((z >> 1) <= k && z >= k);
    return z;
} /*}}}*/

static inline void qt_hash_internal_create(qt_hash ret,
                                           size_t  entries)
{   /*{{{*/
    size_t min_entries = 2 * getpagesize() / sizeof(hash_entry);

    /* meta data */
    if (entries % min_entries != 0) {
        entries += min_entries - (entries % min_entries);
    }
    entries          = encompassing_power_of_two(entries); // needs to be a power of two to make the masking work right (which is key to speed)
    ret->num_entries = entries;
    ret->mask        = (entries - 1) & ~bucketmask;
    /* triggers */
    ret->grow_size    = (entries * 0.65f) - 1;
    ret->tidy_up_size = (entries * 0.8f) - 1;
    if (ret->num_entries == min_entries) {
        ret->shrink_size = 0;
    } else {
        ret->shrink_size = entries * 0.030f;
    }
    /* data storage */
#if defined(HAVE_MEMALIGN)
    ret->entries = memalign(linesize, sizeof(hash_entry) * entries);
#elif defined(HAVE_POSIX_MEMALIGN)
    posix_memalign((void **)&(ret->entries), linesize,
                   sizeof(hash_entry) * entries);
#elif defined(HAVE_WORKING_VALLOC)
    ret->entries = valloc(sizeof(hash_entry) * entries);
#elif defined(HAVE_PAGE_ALIGNED_MALLOC)
    ret->entries = malloc(sizeof(hash_entry) * entries);
#else
    ret->entries = valloc(sizeof(hash_entry) * entries);        /* cross your fingers */
#endif
    if (ret->entries) {
        memset(ret->entries, 0, sizeof(hash_entry) * entries);
    } else {
        free(ret);
        ret = NULL;
    }
} /*}}}*/

/* this function based on http://burtleburtle.net/bob/hash/evahash.html */
#define rot(x, k) (((x) << (k)) | ((x) >> (32 - (k))))
static uint64_t qt_hashword(uint64_t key)
{   /*{{{*/
    uint32_t a, b, c;

    const union {
        uint64_t key;
        uint8_t  b[sizeof(uint64_t)];
    } k = {
        key
    };

    a  = b = c = 0xdeadbeef + sizeof(uint64_t);
    c += 47;

    b += k.b[7] << 24;
    b += k.b[6] << 16;
    b += k.b[5] << 8;
    b += k.b[4];
    a += k.b[3] << 24;
    a += k.b[2] << 16;
    a += k.b[1] << 8;
    a += k.b[0];

    c ^= b;
    c -= rot(b, 14);
    a ^= c;
    a -= rot(c, 11);
    b ^= a;
    b -= rot(a, 25);
    c ^= b;
    c -= rot(b, 16);
    a ^= c;
    a -= rot(c, 4);
    b ^= a;
    b -= rot(a, 14);
    c ^= b;
    c -= rot(b, 24);
    return ((uint64_t)c + ((uint64_t)b << 32));
} /*}}}*/

static inline void **qt_hash_internal_find(qt_hash  h,
                                           qt_key_t key)
{   /*{{{*/
    const hash_entry *z      = h->entries;
    const uint64_t    mask   = h->mask;
    const uint64_t    hashed = qt_hashword((uint64_t)(uintptr_t)key);

    uint64_t bucket = hashed & mask;
    uint64_t step, quit;

    if (key == KEY_DELETED || key == KEY_NULL) {
        if (h->has_key[(uintptr_t)key]) {
            return &(h->value[(uintptr_t)key]);
        } else {
            return NULL;
        }
    }
    for (unsigned i = 0; i < bucketsize; ++i) {
        Q_PREFETCH(&z[bucket + i + 1].key, 0, 1);
        //Q_PREFETCH(&z[bucket + i + 1].value, 1, 1);
        const qt_key_t zkey = z[bucket + i].key;
        if (zkey == key) {
            return (void *)&z[bucket + i].value;
        }
        if (zkey == KEY_NULL) {
            return NULL;
        }
    }

    step = (((hashed >> 16) | (hashed << 16)) & mask) | bucketsize;
    quit = bucket;
    do {
        unsigned i;
        bucket = (bucket + step) & mask;
        for (i = 0; i < bucketsize; ++i) {
            Q_PREFETCH(&z[bucket + i + 1].key, 0, 1);
            //Q_PREFETCH(&z[bucket + i + 1].value, 1, 1);
            const qt_key_t zkey = z[bucket + i].key;
            if (zkey == key) {
                return (void *)&z[bucket + i].value;
            }
            if (zkey == KEY_NULL) {
                return NULL;
            }
        }
    } while (bucket != quit);
    return NULL;
} /*}}}*/

qt_hash INTERNAL qt_hash_create(int needSync)
{   /*{{{*/
    qt_hash ret;

    if (linesize == 0) {
        linesize   = qthread_cacheline();
        bucketsize = linesize / sizeof(hash_entry);
        bucketmask = bucketsize - 1;
    }
    ret = calloc(1, sizeof(struct qt_hash_s));
    if (ret) {
        if (needSync) {
            ret->lock = malloc(sizeof(QTHREAD_FASTLOCK_TYPE));
            QTHREAD_FASTLOCK_INIT_PTR(ret->lock);
        } else {
            ret->lock = NULL;
        }
        qt_hash_internal_create(ret, 100);
    }
    return ret;
} /*}}}*/

void INTERNAL qt_hash_destroy(qt_hash h)
{   /*{{{*/
    assert(h);
    if (h->lock) {
        QTHREAD_FASTLOCK_DESTROY_PTR(h->lock);
        free((void *)h->lock);
    }
    assert(h->entries);
    free(h->entries);
    free(h);
} /*}}}*/

/* This function destroys the hash and applies the given deallocator function
 * to each value stored in the hash */
void INTERNAL qt_hash_destroy_deallocate(qt_hash                h,
                                         qt_hash_deallocator_fn f)
{   /*{{{*/
    size_t visited = 0;

    assert(h);
    if (h->lock) {
        QTHREAD_FASTLOCK_LOCK(h->lock);
    }
    if (h->has_key[0] == 1) {
        ++visited;
        f(h->value[0]);
    }
    if (h->has_key[1] == 1) {
        ++visited;
        f(h->value[1]);
    }
    if (visited < h->population) {
        size_t i;
        for (i = 0; i < h->num_entries; ++i) {
            if (h->entries[i].key > KEY_DELETED) {
                ++visited;
                f(h->entries[i].value);
                if (visited == h->population) {
                    break;
                }
            }
        }
    }
    assert(visited == h->population);
    if (h->lock) {
        QTHREAD_FASTLOCK_UNLOCK(h->lock);
    }
    qt_hash_destroy(h);
} /*}}}*/

int INTERNAL qt_hash_put(qt_hash  h,
                         qt_key_t key,
                         void    *value)
{   /*{{{*/
    int ret;

    assert(h);
    if (h->lock) {
        QTHREAD_FASTLOCK_LOCK(h->lock);
    }
    ret = qt_hash_put_locked(h, key, value);
    if (h->lock) {
        QTHREAD_FASTLOCK_UNLOCK(h->lock);
    }
    return ret;
} /*}}}*/

static void brehash(qt_hash h,
                    size_t  len)
{   /*{{{*/
    qt_hash d = calloc(1, sizeof(struct qt_hash_s));
    size_t  i, copied;

    assert(d);
    qt_hash_internal_create(d, len);
    memcpy(d->has_key, h->has_key, sizeof(short) * 2);
    memcpy(d->value, h->value, sizeof(void *) * 2);
    copied = h->has_key[0] + h->has_key[1];
    if (copied < h->population) {
        for (i = 0; i < h->num_entries; ++i) {
            if (h->entries[i].key > KEY_DELETED) {
                qassertnot(qt_hash_put_locked(d, h->entries[i].key, h->entries[i].value), 0);
                ++copied;
                if (copied == h->population) {
                    break;
                }
            }
        }
    }
    assert(h->population == d->population);
    free(h->entries);
    h->entries      = d->entries;
    h->mask         = d->mask;
    h->num_entries  = d->num_entries;
    h->population   = d->population;
    h->deletes      = d->deletes;
    h->grow_size    = d->grow_size;
    h->tidy_up_size = d->tidy_up_size;
    h->value[0]     = d->value[0];
    h->value[1]     = d->value[1];
    h->has_key[0]   = d->has_key[0];
    h->has_key[1]   = d->has_key[1];
    free(d);
} /*}}}*/

int INTERNAL qt_hash_put_locked(qt_hash  h,
                                qt_key_t key,
                                void    *value)
{                     /*{{{*/
    hash_entry *z;    // for speed, to avoid extra ptr dereferences
    uint64_t    mask; // for speed, to avoid extra ptr dereferences
    int         done;
    ssize_t     f;
    size_t      i;
    uint64_t    hw, bucket, step;

    assert(h);
    if (key == KEY_DELETED || key == KEY_NULL) {
        h->has_key[(uintptr_t)key] = 1;
        h->value[(uintptr_t)key] = value;
        return 1;
    }

restart:
    z    = h->entries;
    mask = h->mask;
    done = 0;
    f    = -1;

    hw     = qt_hashword((uint64_t)(uintptr_t)key);
    bucket = hw & mask;

    /* find the key in the bucket
     * - otherwise insert in the first DELETED bucket
     * - otherwise insert in the first NULL bucket
     * - otherwise the bucket is full (i.e. we're not done) */
    for (i = 0; i < bucketsize; ++i) {
        if (z[bucket + i].key == key) {
            z[bucket + i].value = value;
            return 1;
        } else if (z[bucket + i].key == KEY_DELETED) {
            if (f == -1) {
                f = bucket + i;
            }
        } else if (z[bucket + i].key == KEY_NULL) {
            if (f == -1) {
                f = bucket + i;
            }
            done = 1;
        }
    }
    if (!done) {
        uint64_t quit = bucket;
        step = (((hw >> 16) | (hw << 16)) & mask) | bucketsize;
        do {
            bucket = (bucket + step) & mask;
            for (i = 0; i < bucketsize; ++i) {
                /* must search the entire cacheline (because otherwise we'd
                 * have to do more movement when deleting things from the
                 * cacheline)... should be cheap, though */
                if (z[bucket + i].key == key) {
                    z[bucket + i].value = value;
                    return 1;
                } else if (z[bucket + i].key == KEY_DELETED) {
                    if (f == -1) {
                        f = bucket + i;
                    }
                } else if (z[bucket + i].key == KEY_NULL) {
                    if (f == -1) {
                        f = bucket + i;
                    }
                    done = 1;
                }
            }
        } while (!done && bucket != quit);
    }
    assert(f != -1);                                         // we MUST have found a place for it (otherwise the hash should have been resized bigger)
    assert(z[f].key == KEY_NULL || z[f].key == KEY_DELETED); // sanity: the spot is empty

    if (z[f].key == KEY_NULL) {
        if (h->population >= h->grow_size) {
            brehash(h, h->num_entries * 2);
            goto restart;
        } else if (h->population + h->deletes > h->tidy_up_size) {
            brehash(h, h->num_entries);
            goto restart;
        }
    } else if (z[f].key == KEY_DELETED) {
        --h->deletes;
    }
    z[f].key   = key;
    z[f].value = value;
    ++h->population;
    return 1;
} /*}}}*/

int INTERNAL qt_hash_remove(qt_hash        h,
                            const qt_key_t key)
{   /*{{{*/
    int ret;

    assert(h);
    if (h->lock) {
        QTHREAD_FASTLOCK_LOCK(h->lock);
    }
    ret = qt_hash_remove_locked(h, key);
    if (h->lock) {
        QTHREAD_FASTLOCK_UNLOCK(h->lock);
    }
    return ret;
} /*}}}*/

int INTERNAL qt_hash_remove_locked(qt_hash        h,
                                   const qt_key_t key)
{   /*{{{*/
    hash_entry *p;
    void      **value;

    assert(h);
    if (key == KEY_DELETED || key == KEY_NULL) {
        if (h->has_key[(uintptr_t)key] == 0) {
            return 0;
        } else {
            h->has_key[(uintptr_t)key] = 0;
            h->value[(uintptr_t)key] = NULL;
        }
        return 1;
    }
    value = qt_hash_internal_find(h, key);
    if (value == NULL) {
        return 0;
    }
    p      = (hash_entry *)(value - 1); // sneaky way to recover the hash_entry ptr
    p->key = KEY_DELETED;
    ++h->deletes;
    --h->population;
    if (h->population + h->deletes >= h->tidy_up_size) {
        brehash(h, h->num_entries);
    } else if (h->population < h->shrink_size) {
        brehash(h, h->num_entries / 2);
    }
    return 1;
} /*}}}*/

void INTERNAL *qt_hash_get(qt_hash        h,
                           const qt_key_t key)
{   /*{{{*/
    void *ret;

    assert(h);
    if (h->lock) {
        QTHREAD_FASTLOCK_LOCK(h->lock);
    }
    ret = qt_hash_get_locked(h, key);
    if (h->lock) {
        QTHREAD_FASTLOCK_UNLOCK(h->lock);
    }
    return (void *)ret;
} /*}}}*/

void INTERNAL *qt_hash_get_locked(qt_hash        h,
                                  const qt_key_t key)
{   /*{{{*/
    void **value;

    assert(h);
    value = qt_hash_internal_find(h, key);
    if (value == NULL) {
        return NULL;
    } else {
        return *value;
    }
} /*}}}*/

void INTERNAL qt_hash_callback(qt_hash             h,
                               qt_hash_callback_fn f,
                               void               *arg)
{   /*{{{*/
    size_t visited = 0;

    assert(h);
    if (h->lock) {
        QTHREAD_FASTLOCK_LOCK(h->lock);
    }
    if (h->has_key[0] == 1) {
        ++visited;
        f(KEY_NULL, h->value[0], arg);
    }
    if (h->has_key[1] == 1) {
        ++visited;
        f(KEY_DELETED, h->value[1], arg);
    }
    if (visited < h->population) {
        size_t i;
        for (i = 0; i < h->num_entries; ++i) {
            if (h->entries[i].key > KEY_DELETED) {
                ++visited;
                f(h->entries[i].key, h->entries[i].value, arg);
                if (visited == h->population) {
                    break;
                }
            }
        }
    }
    if (h->lock) {
        QTHREAD_FASTLOCK_UNLOCK(h->lock);
    }
} /*}}}*/

size_t INTERNAL qt_hash_count(qt_hash h)
{   /*{{{*/
    size_t ct;

    assert(h);
    if (h->lock) {
        QTHREAD_FASTLOCK_LOCK(h->lock);
    }
    ct = h->population + h->has_key[0] + h->has_key[1];
    if (h->lock) {
        QTHREAD_FASTLOCK_UNLOCK(h->lock);
    }
    return ct;
} /*}}}*/

void INTERNAL qt_hash_lock(qt_hash h)
{   /*{{{*/
    assert(h);
    if (h->lock) {
        QTHREAD_FASTLOCK_LOCK(h->lock);
    }
} /*}}}*/

void INTERNAL qt_hash_unlock(qt_hash h)
{   /*{{{*/
    assert(h);
    if (h->lock) {
        QTHREAD_FASTLOCK_UNLOCK(h->lock);
    }
} /*}}}*/

/* vim:set expandtab: */
