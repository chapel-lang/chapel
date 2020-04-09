#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <stdlib.h> /* for malloc/free/etc */
#include <stdio.h>  /* for printf() */
#include <unistd.h> /* for getpagesize() */

/* Qthreads Headers */
#include <qthread/qthread.h> /* for qthread_incr() and qthread_cas() */
#include <qthread/qpool.h>
#include <qthread/dictionary.h>

/* Internal Headers */
#include "qt_asserts.h"
#include "qt_debug.h"
#include "qt_atomics.h"
#include "qt_alloc.h"

/*
 * The hash table in this file is based on the work by Ori Shalev and Nir Shavit
 * Much of the code is similar to the example code they published in their
 * paper "Split-Ordered Lists: Lock-Free Extensible Hash Tables", but has been
 * modified to support additional semantics.
 *
 * NOTE: I did not implement the Dynamic-Sized Array extension, because I don't
 * expect to support large numbers of entries in the table at the same time,
 * and I'm worried about state creep. All that means is that for extremely
 * large hash tables, we're less efficient than we could be.
 */

#define MAX_LOAD 4
// #define USE_HASHWORD 1

#ifdef USE_HASHWORD
# include <qthread/hash.h>
#endif

#define key_t mykey_t

/* external types */
typedef void *qt_key_t;
typedef struct qt_dictionary *qt_hash;
typedef void (*qt_hash_callback_fn)(const qt_key_t,
                                    void *,
                                    void *);
typedef void (*qt_hash_deallocator_fn)(void *);

/* internal types */
typedef uint64_t key_t;
typedef uint64_t so_key_t;
typedef uintptr_t marked_ptr_t;

#define MARK_OF(x)           ((x) & 1)
#define PTR_MASK(x)          (((marked_ptr_t)x) & ~(marked_ptr_t)1)
#define PTR_OF(x)            ((hash_entry *)PTR_MASK(x))
#define CONSTRUCT(mark, ptr) (PTR_MASK((uintptr_t)ptr) | (mark))
#define UNINITIALIZED ((marked_ptr_t)0)

/* Internal Globals */
static size_t hard_max_buckets = NO_BUCKETS;

/* ... pools */
static qpool *hash_entry_pool = NULL;

typedef struct list_entry hash_entry;
// So: list_entry* = hash_entry*
// (other typedef found in dictionary.h)

struct qt_dictionary {
    marked_ptr_t        *B;      // Buckets
    size_t               count;  // Crt number of elements in hash
    size_t               size;   // Crt number of buckets (doubling if load per bucket too much)
    qt_dict_key_equals_f op_equals;
    qt_dict_hash_f       op_hash;
    qt_dict_cleanup_f    op_cleanup;
};
// So: qt_dictionary* = qt_hash
// (typedef found in dictionary.h)

/* prototypes */
static void *qt_lf_list_find(marked_ptr_t        *head,
                             so_key_t             hashed_key,
                             qt_key_t             key,
                             marked_ptr_t       **prev,
                             marked_ptr_t        *cur,
                             marked_ptr_t        *next,
                             qt_dict_key_equals_f op_equals);
static void initialize_bucket(qt_hash h,
                              size_t  bucket);

#define MSB (((uint64_t)1) << 63)
#define REVERSE_BYTE(x) ((so_key_t)((((((uint32_t)(x)) * 0x0802LU & 0x22110LU) | (((uint32_t)(x)) * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16) & 0xff))
#if 0
/* 32-bit reverse */
# define REVERSE(x) (REVERSE_BYTE((((so_key_t)(x))) & 0xff) << 24) | \
    (REVERSE_BYTE((((so_key_t)(x)) >> 8) & 0xff) << 16) |            \
    (REVERSE_BYTE((((so_key_t)(x)) >> 16) & 0xff) << 8) |            \
    (REVERSE_BYTE((((so_key_t)(x)) >> 24) & 0xff))
#else
# define REVERSE(x) ((REVERSE_BYTE((((so_key_t)(x))) & 0xff) << 56) |       \
                     (REVERSE_BYTE((((so_key_t)(x)) >> 8) & 0xff) << 48) |  \
                     (REVERSE_BYTE((((so_key_t)(x)) >> 16) & 0xff) << 40) | \
                     (REVERSE_BYTE((((so_key_t)(x)) >> 24) & 0xff) << 32) | \
                     (REVERSE_BYTE((((so_key_t)(x)) >> 32) & 0xff) << 24) | \
                     (REVERSE_BYTE((((so_key_t)(x)) >> 40) & 0xff) << 16) | \
                     (REVERSE_BYTE((((so_key_t)(x)) >> 48) & 0xff) << 8) |  \
                     (REVERSE_BYTE((((so_key_t)(x)) >> 56) & 0xff) << 0))
#endif /* if 0 */

#ifdef USE_HASHWORD
# define HASH_KEY(key) key = (qt_hash64(key) & (~MSB))
#else /* ifdef USE_HASHWORD */
# define HASH_KEY(key) key &= (~MSB)
#endif /* ifdef USE_HASHWORD */

/**
 * Insert node "node" in the list starting at position "head".
 * If item exists replace it (force insert)
 */
static void qt_lf_force_list_insert(marked_ptr_t        *head,
                                    hash_entry          *node,
                                    qt_dict_key_equals_f op_equals)
{
    so_key_t hashed_key = node->hashed_key;
    qt_key_t key        = node->key;

    while (1) {
        marked_ptr_t *lprev;
        marked_ptr_t  cur;
        marked_ptr_t  lnext;

        if (qt_lf_list_find(head, hashed_key, key, &lprev, &cur, &lnext, op_equals) != NULL) {                       // needs to set cur/prev/next
            node->next = (hash_entry *)CONSTRUCT(0, lnext);
        } else {
            node->next = (hash_entry *)CONSTRUCT(0, cur);
        }
        if (qthread_cas(lprev, CONSTRUCT(0, cur), CONSTRUCT(0, node)) == CONSTRUCT(0, cur)) {
            return;
        }
    }
}

/**
 * Insert node "node" in the list starting at position "head".
 * Set "ocur" to be the item with the same key if it exists
 *      or the item which follows if the node was added successfully.
 * Set crt_node to point to the current node with key = "node->key"
 * Return 0 if item existed and 1 if it was inserted
 */
static int qt_lf_list_insert(marked_ptr_t        *head,
                             hash_entry          *node,
                             marked_ptr_t        *ocur,
                             hash_entry         **crt_node,
                             qt_dict_key_equals_f op_equals)
{
    so_key_t hashed_key = node->hashed_key;
    qt_key_t key        = node->key;

    while (1) {
        marked_ptr_t *lprev;
        marked_ptr_t  cur;

        if (qt_lf_list_find(head, hashed_key, key, &lprev, &cur, NULL, op_equals) != NULL) {                       // needs to set cur/prev
            if (ocur) { *ocur = cur; }
            if (crt_node) { *crt_node = PTR_OF(cur); }
            return 0;
        }
        node->next = (hash_entry *)CONSTRUCT(0, cur);
        if (qthread_cas(lprev, (marked_ptr_t)(uintptr_t)node->next, CONSTRUCT(0, node)) == CONSTRUCT(0, cur)) {
            if (ocur) { *ocur = cur; }
            if (crt_node) { *crt_node = node; }
            return 1;
        }
    }
}

static int qt_lf_list_delete(marked_ptr_t        *head,
                             so_key_t             hashed_key,
                             qt_key_t             key,
                             qt_dict_key_equals_f op_equals,
                             qt_dict_cleanup_f    cleanup)
{
    while (1) {
        marked_ptr_t *lprev;
        marked_ptr_t  lcur;
        marked_ptr_t  lnext;
        if (qt_lf_list_find(head, hashed_key, key, &lprev, &lcur, &lnext, op_equals) == NULL) {
            qthread_debug(ALWAYS_OUTPUT, "### inside delete - return 0\n");
            return 0;
        }
        if (qthread_cas_ptr(&PTR_OF(lcur)->next, (void*)CONSTRUCT(0, lnext), (void*)CONSTRUCT(1, lnext)) != (void *)CONSTRUCT(0, lnext)) {
            qthread_debug(ALWAYS_OUTPUT, "### inside delete - cas failed continue\n");
            continue;
        }
        if (qthread_cas(lprev, CONSTRUCT(0, lcur), CONSTRUCT(0, lnext)) == CONSTRUCT(0, lcur)) {
            if (cleanup != NULL) {
                cleanup(PTR_OF(lcur)->key, NULL);
            }
            qpool_free(hash_entry_pool, PTR_OF(lcur));
        } else {
            qt_lf_list_find(head, hashed_key, key, NULL, NULL, NULL, op_equals);                                   // needs to set cur/prev/next
        }
        return 1;
    }
}

static void *qt_lf_list_find(marked_ptr_t        *head,
                             so_key_t             hashed_key,
                             qt_key_t             key,
                             marked_ptr_t       **oprev,
                             marked_ptr_t        *ocur,
                             marked_ptr_t        *onext,
                             qt_dict_key_equals_f op_equals)
{
    so_key_t      ckey;
    qt_key_t      okey;
    void         *cval;
    marked_ptr_t *prev = NULL;
    marked_ptr_t  cur  = UNINITIALIZED;
    marked_ptr_t  next = UNINITIALIZED;

    // int foundInsertPos = 0;
    while (1) {
        prev = head;
        cur  = *prev;
        while (1) {
            if (PTR_OF(cur) == NULL) {
                if (oprev) { *oprev = prev; }
                if (ocur) { *ocur = cur; }
                if (onext) { *onext = next; }
                return 0;
            }
            next = (marked_ptr_t)(PTR_OF(cur)->next);
            ckey = PTR_OF(cur)->hashed_key;
            cval = PTR_OF(cur)->value;
            okey = PTR_OF(cur)->key;
            if (*prev != CONSTRUCT(0, cur)) {
                break; // this means someone mucked with the list; start over
            }
            if (!MARK_OF(next)) {  // if next pointer is not marked
                if (ckey >= hashed_key) {
                    // if current key > hashed_key, the key isn't in the list; if current key == hashed_key, the key IS in the list
                    // if (foundInsertPos == 0) {
                    if (oprev) { *oprev = prev; }
                    if (ocur) { *ocur = cur; }
                    if (onext) { *onext = next; }
                    // foundInsertPos = 1;
                    // }

                    if(ckey == hashed_key) {
                        if((okey != NULL) && op_equals(okey, key)) {
                            /*
                             * if (oprev) { *oprev = prev; }
                             * if (ocur) { *ocur = cur; }
                             * if (onext) { *onext = next; }
                             */
                            return cval;
                        }                        // else, keep looking
                    } else { return NULL; }
                }
                // but if current key < hashed_key, the we don't know yet, keep looking
                prev = (marked_ptr_t *)&(PTR_OF(cur)->next);
            } else {
                if (qthread_cas(prev, CONSTRUCT(0, cur), CONSTRUCT(0, next)) == CONSTRUCT(0, cur)) {
                    qpool_free(hash_entry_pool, PTR_OF(cur));
                } else {
                    break;
                }
            }
            cur = next;
        }
    }
}

static inline so_key_t so_regularkey(const key_t key)
{
    return REVERSE(key | MSB);
}

static inline so_key_t so_dummykey(const key_t key)
{
    return REVERSE(key);
}

#define PUT_ALWAYS    0
#define PUT_IF_ABSENT 1

// old public method; added last param to distinguish between put and put if absent
static inline hash_entry *qt_hash_put(qt_hash  h,
                                      qt_key_t key,
                                      void    *value,
                                      int      put_choice)
{
    hash_entry *node = qpool_alloc(hash_entry_pool);
    hash_entry *ret  = node;
    size_t      bucket;
    uint64_t    lkey = (uint64_t)(uintptr_t)(h->op_hash(key));

    HASH_KEY(lkey);
    bucket = lkey % h->size;

    assert(node);
    assert((lkey & MSB) == 0);
    node->hashed_key = so_regularkey(lkey);
    node->key        = key; // Also store original key!
    node->value      = value;
    node->next       = (hash_entry*)UNINITIALIZED;

    if (h->B[bucket] == UNINITIALIZED) {
        initialize_bucket(h, bucket);
    }

    if(put_choice == PUT_IF_ABSENT) {
        if (!qt_lf_list_insert(&(h->B[bucket]), node, NULL, &ret, h->op_equals)) {
            qpool_free(hash_entry_pool, node);
            return ret->value;
        }
    } else {
        qt_lf_force_list_insert(&(h->B[bucket]), node, h->op_equals);
    }

    size_t csize = h->size;
    if (qthread_incr(&h->count, 1) / csize > MAX_LOAD) {
        if (2 * csize <= hard_max_buckets) { // this caps the size of the hash
            qthread_cas(&h->size, csize, 2 * csize);
        }
    }
    return ret->value;
}

void *qt_dictionary_put(qt_dictionary *dict,
                        void          *key,
                        void          *value)
{
    return qt_hash_put(dict, key, value, PUT_ALWAYS);
}

void *qt_dictionary_put_if_absent(qt_dictionary *dict,
                                  void          *key,
                                  void          *value)
{
    return qt_hash_put(dict, key, value, PUT_IF_ABSENT);
}

// old public method
static inline void *qt_hash_get(qt_hash        h,
                                const qt_key_t key)
{
    size_t   bucket;
    uint64_t lkey = (uint64_t)(uintptr_t)(h->op_hash(key));

    HASH_KEY(lkey);
    bucket = lkey % h->size;

    if (h->B[bucket] == UNINITIALIZED) {
        // You'd think returning NULL at this point would be a good idea; but
        // if we do that, we risk losing key/value pairs (incorrectly reporting
        // them as absent) when the hash table resizes
        initialize_bucket(h, bucket);
    }
    return qt_lf_list_find(&(h->B[bucket]), so_regularkey(lkey), key, NULL, NULL, NULL, h->op_equals);
}

void *qt_dictionary_get(qt_dictionary *dict,
                        void          *key)
{
    return qt_hash_get(dict, key);
}

// old public method
static inline int qt_hash_remove(qt_hash        h,
                                 const qt_key_t key)
{
    size_t   bucket;
    uint64_t lkey = (uint64_t)(uintptr_t)(h->op_hash(key));

    HASH_KEY(lkey);
    bucket = lkey % h->size;

    if (h->B[bucket] == UNINITIALIZED) {
        initialize_bucket(h, bucket);
    }
    if (!qt_lf_list_delete(&(h->B[bucket]), so_regularkey(lkey), key, h->op_equals, h->op_cleanup)) {
        return 0;
    }
    qthread_incr(&h->count, -1);
    return 1;
}

void *qt_dictionary_delete(qt_dictionary *dict,
                           void          *key)
{
    void *val = qt_hash_get(dict, key);     // TODO : this is inefficient!
    int   ret = qt_hash_remove(dict, key);

    if(ret) { return val; } else { return NULL; }
}

static inline size_t GET_PARENT(uint64_t bucket)
{
    uint64_t t = bucket;

    t |= t >> 1;
    t |= t >> 2;
    t |= t >> 4;
    t |= t >> 8;
    t |= t >> 16;
    t |= t >> 32;     // creates a mask
    return bucket & (t >> 1);
}

static void initialize_bucket(qt_hash h,
                              size_t  bucket)
{
    size_t       parent = GET_PARENT(bucket);
    marked_ptr_t cur;

    if (h->B[parent] == UNINITIALIZED) {
        initialize_bucket(h, parent);
    }
    hash_entry *dummy = qpool_alloc(hash_entry_pool);
    assert(dummy);
    dummy->hashed_key = so_dummykey(bucket);
    dummy->key        = NULL;
    dummy->value      = NULL;
    dummy->next       = (hash_entry*)UNINITIALIZED;
    if (!qt_lf_list_insert(&(h->B[parent]), dummy, &cur, NULL, h->op_equals)) {
        qpool_free(hash_entry_pool, dummy);
        dummy = PTR_OF(cur);
        while (h->B[bucket] != CONSTRUCT(0, dummy)) ;
    } else {
        h->B[bucket] = CONSTRUCT(0, dummy);
    }
}

// old public method
static inline qt_hash qt_hash_create(qt_dict_key_equals_f eq,
                                     qt_dict_hash_f       hash,
                                     qt_dict_cleanup_f    cleanup)
{
    qt_hash tmp;

    if (hash_entry_pool == NULL) {
        if (qthread_cas_ptr(&hash_entry_pool, NULL, (void *)1) == NULL) {
            hash_entry_pool = qpool_create(sizeof(hash_entry));
        } else {
            while (hash_entry_pool == (void *)1) SPINLOCK_BODY();
        }
    }

    tmp = MALLOC(sizeof(qt_dictionary));
    assert(tmp);
    tmp->op_equals  = eq;
    tmp->op_hash    = hash;
    tmp->op_cleanup = cleanup;

    assert(tmp);
    if (hard_max_buckets == 0) {
        hard_max_buckets = pagesize / sizeof(marked_ptr_t);
    }
    tmp->B = qt_calloc(hard_max_buckets, sizeof(marked_ptr_t));
    assert(tmp->B);
    tmp->size  = 2;
    tmp->count = 0;
    {
        hash_entry *dummy = qpool_alloc(hash_entry_pool);
        assert(dummy);
        memset(dummy, 0, sizeof(hash_entry));
        tmp->B[0] = CONSTRUCT(0, dummy);
    }
    return tmp;
}

qt_dictionary *qt_dictionary_create(qt_dict_key_equals_f eq,
                                    qt_dict_hash_f       hash,
                                    qt_dict_cleanup_f    cleanup)
{
    return qt_hash_create(eq, hash, cleanup);
}

// old public method
static inline void qt_hash_destroy(qt_hash h)
{
    marked_ptr_t cursor;

    assert(h);
    assert(h->B);
    cursor = h->B[0];
    while (PTR_OF(cursor) != NULL) {
        marked_ptr_t tmp = cursor;
        assert(MARK_OF(tmp) == 0);
        cursor = (marked_ptr_t)((PTR_OF(cursor)->next));
        if (h->op_cleanup && ((PTR_OF(tmp)->key || PTR_OF(tmp)->value))) {
            // only call the cleanup function on non-place-holders
            h->op_cleanup(PTR_OF(tmp)->key, PTR_OF(tmp)->value);
        }
        qpool_free(hash_entry_pool, PTR_OF(tmp));
    }
    FREE(h->B, hard_max_buckets * sizeof(marked_ptr_t));
    FREE(h, sizeof(qt_hash));
}

void qt_dictionary_destroy(qt_dictionary *d)
{
    qt_hash_destroy(d);
}

/*
 * void qt_hash_destroy_deallocate(qt_hash                h,
 *                              qt_hash_deallocator_fn f)
 * {
 *  marked_ptr_t cursor;
 *
 *  assert(h);
 *  assert(h->B);
 *  cursor = h->B[0];
 *  while (PTR_OF(cursor) != NULL) {
 *      marked_ptr_t tmp = cursor;
 *      assert(MARK_OF(tmp) == 0);
 *      cursor = PTR_OF(cursor)->next;
 *      f(PTR_OF(cursor));
 *      free(PTR_OF(tmp));
 *  }
 *  free(h->B);
 *  free(h);
 * }
 *
 * size_t qt_hash_count(qt_hash h)
 * {
 *  assert(h);
 *  return h->size;
 * }
 *
 * void qt_hash_callback(qt_hash             h,
 *                    qt_hash_callback_fn f,
 *                    void               *arg)
 * {
 *  marked_ptr_t cursor;
 *
 *  assert(h);
 *  assert(h->B);
 *  cursor = h->B[0];
 *  while (PTR_OF(cursor) != NULL) {
 *      marked_ptr_t tmp = cursor;
 *      so_key_t     key = PTR_OF(cursor)->hashed_key;
 *      assert(MARK_OF(tmp) == 0);
 *      if (key & 1) {
 *          f((qt_key_t)(uintptr_t)REVERSE(key ^ 1), PTR_OF(cursor)->value, arg);
 *      } else {
 *          // f((qt_key_t)REVERSE(key), PTR_OF(cursor)->value, (void *)1);
 *      }
 *      cursor = PTR_OF(cursor)->next;
 *  }
 * }
 */

struct qt_dictionary_iterator {
    qt_dictionary *dict;
    list_entry    *crt; // =NULL if iterator is newly created or reached the end; =crt elem otherwise.
    int            bkt; // = -1 if iterator is newly created; =1 otherwise.
};

qt_dictionary_iterator *qt_dictionary_iterator_create(qt_dictionary *dict)
{
    if(dict == NULL) {
        return ERROR;
    }
    qt_dictionary_iterator *it = (qt_dictionary_iterator *)MALLOC(sizeof(qt_dictionary_iterator));
    it->dict = dict;
    it->crt  = NULL;
    it->bkt  = -1;
    return it;
}

void qt_dictionary_iterator_destroy(qt_dictionary_iterator *it)
{
    if(it == NULL) { return; }
    FREE(it, sizeof(qt_dictionary_iterator));
}

static list_entry *qt_dictionary_iterator_next_element(qt_dictionary_iterator *it)
{
    if((it == NULL) || (it->dict == NULL)) {
        return ERROR;
    }
    qt_dictionary *h = it->dict;
    if(h->count == 0) {
        it->bkt = 1;
        return NULL;
    }
    if(it->bkt == -1) {
        int csize = h->size, bucket;
        for(bucket = 0; bucket < csize; bucket++)
            if (h->B[bucket] != UNINITIALIZED) {
                it->bkt = 1;
                it->crt = PTR_OF(h->B[bucket]);
                return it->crt;
            }
    }
    if(it->crt == UNINITIALIZED) {
        return NULL;
    }
    it->crt = PTR_OF(it->crt->next);
    return it->crt;
}

list_entry *qt_dictionary_iterator_next(qt_dictionary_iterator *it)
{
    list_entry *ret = qt_dictionary_iterator_next_element(it);

    while(ret != ERROR && ret != NULL && ret->key == NULL) {
        ret = qt_dictionary_iterator_next_element(it);
    }
    return ret;
}

list_entry *qt_dictionary_iterator_get(const qt_dictionary_iterator *it)
{
    if((it == NULL) || (it->dict == NULL)) {
        return ERROR;
    }
    qt_dictionary *h = it->dict;
    if((h->count == 0) || (it->crt == UNINITIALIZED)) {
        return NULL;
    }
    return it->crt;
}

qt_dictionary_iterator *qt_dictionary_end(qt_dictionary *dict)
{
    qt_dictionary_iterator *ret = qt_dictionary_iterator_create(dict);

    ret->bkt = 1;
    return ret;
}

int qt_dictionary_iterator_equals(qt_dictionary_iterator *a,
                                  qt_dictionary_iterator *b)
{
    if ((a == NULL) || (b == NULL)) {
        return a == b;
    }
    return (a->crt == b->crt) && (a->dict == b->dict) && (a->bkt == b->bkt);
}

qt_dictionary_iterator *qt_dictionary_iterator_copy(qt_dictionary_iterator *b)
{
    if(b == NULL) {
        return NULL;
    }
    qt_dictionary_iterator *ret = qt_dictionary_iterator_create(b->dict);
    if((ret == NULL) || (ret == ERROR)) {
        return NULL;
    }
    ret->bkt = b->bkt;
    ret->crt = b->crt;
    return ret;
}

void qt_dictionary_printbuckets(qt_dictionary *dict)
{
    /*int csize = dict->size, bucket;
     * int used_buckets = 0, total = dict->count;
     * for(bucket=0; bucket<csize-1; bucket++){
     *      if (dict->B[bucket] != UNINITIALIZED){
     *              list_entry* head = dict->B[bucket];
     *              list_entry* tail = dict->B[bucket+1];
     *              while(head != tail && head != NULL){
     *                      ...
     *              }
     *      }
     * }*/
    printf("allocated_buckets = %d; total elements = %d;\n", (int)dict->size, (int)dict->count);
}

/* vim:set expandtab: */
