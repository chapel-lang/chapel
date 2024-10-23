#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* System Headers */
#include <stdatomic.h>
#include <stdlib.h> /* for malloc/free/etc */
#include <unistd.h> /* for getpagesize() */

/* Public Headers */
#include "qthread/qthread.h" /* for qthread_incr() and qthread_cas() */

/* Internal Headers */
#include "qt_alloc.h"
#include "qt_asserts.h"
#include "qt_mpool.h"
#include "qt_subsystems.h"

/* The Internal API */
#include "qt_hash.h"

/*
 * The hash table in this file is based on the work by Ori Shalev and Nir Shavit
 * Much of the code is similar to the example code they published in their
 * paper "Split-Ordered Lists: Lock-Free Extensible Hash Tables", but has been
 * modified to support additional semantics.
 *
 * NOTE: I did not implement the Dynamic-Sized Array extension, because I don't
 * expect to support large numbers of entries in the table at the same time,
 * and I'm worried about state creep.
 */

#define MAX_LOAD 4
// #define USE_HASHWORD 1

/* internal types */
typedef uint64_t lkey_t;
typedef uint64_t so_key_t;
typedef uintptr_t marked_ptr_t;

#define MARK_OF(x) ((x) & 1)
#define PTR_MASK(x) ((x) & ~(marked_ptr_t)1)
#define PTR_OF(x) ((hash_entry *)PTR_MASK(x))
#define CONSTRUCT(mark, ptr) (marked_ptr_t)(PTR_MASK((uintptr_t)ptr) | (mark))
#define UNINITIALIZED ((marked_ptr_t)0)

size_t hard_max_buckets = 0;
qt_mpool hash_entry_pool = NULL;

typedef struct hash_entry_s {
  _Atomic so_key_t key;
  void *_Atomic value;
  _Atomic marked_ptr_t next;
} hash_entry;

struct qt_hash_s {
  _Atomic marked_ptr_t *B; // Buckets
  size_t volatile count;
  size_t volatile size;
};

#define ALLOC_HASH_ENTRY() qt_mpool_alloc(hash_entry_pool)
#define FREE_HASH_ENTRY(t) qt_mpool_free(hash_entry_pool, (t))

/* prototypes */
static void *qt_lf_list_find(_Atomic marked_ptr_t *head,
                             so_key_t key,
                             _Atomic marked_ptr_t **prev,
                             marked_ptr_t *cur,
                             marked_ptr_t *next);
static void initialize_bucket(qt_hash h, size_t bucket);

#define MSB (((uint64_t)1) << 63)
#define REVERSE_BYTE(x)                                                        \
  ((so_key_t)((((((uint32_t)(x)) * 0x0802LU & 0x22110LU) |                     \
                (((uint32_t)(x)) * 0x8020LU & 0x88440LU)) *                    \
                 0x10101LU >>                                                  \
               16) &                                                           \
              0xff))
#if 0
/* 32-bit reverse */
#define REVERSE(x)                                                             \
  (REVERSE_BYTE((((so_key_t)(x))) & 0xff) << 24) |                             \
    (REVERSE_BYTE((((so_key_t)(x)) >> 8) & 0xff) << 16) |                      \
    (REVERSE_BYTE((((so_key_t)(x)) >> 16) & 0xff) << 8) |                      \
    (REVERSE_BYTE((((so_key_t)(x)) >> 24) & 0xff))
#else
#define REVERSE(x)                                                             \
  ((REVERSE_BYTE((((so_key_t)(x))) & 0xff) << 56) |                            \
   (REVERSE_BYTE((((so_key_t)(x)) >> 8) & 0xff) << 48) |                       \
   (REVERSE_BYTE((((so_key_t)(x)) >> 16) & 0xff) << 40) |                      \
   (REVERSE_BYTE((((so_key_t)(x)) >> 24) & 0xff) << 32) |                      \
   (REVERSE_BYTE((((so_key_t)(x)) >> 32) & 0xff) << 24) |                      \
   (REVERSE_BYTE((((so_key_t)(x)) >> 40) & 0xff) << 16) |                      \
   (REVERSE_BYTE((((so_key_t)(x)) >> 48) & 0xff) << 8) |                       \
   (REVERSE_BYTE((((so_key_t)(x)) >> 56) & 0xff) << 0))
#endif /* if 0 */

#ifdef USE_HASHWORD
#define HASH_KEY(key) key = (qt_hash64(key) & (~MSB))
#else /* ifdef USE_HASHWORD */
#define HASH_KEY(key) key &= (~MSB)
#endif /* ifdef USE_HASHWORD */

static int qt_lf_list_insert(_Atomic marked_ptr_t *head,
                             hash_entry *node,
                             marked_ptr_t *ocur) { /*{{{*/
  so_key_t key = atomic_load_explicit(&node->key, memory_order_relaxed);

  while (1) {
    _Atomic marked_ptr_t *lprev;
    marked_ptr_t cur;

    if (qt_lf_list_find(head, key, &lprev, &cur, NULL) !=
        NULL) { // needs to set cur/prev
      if (ocur) { *ocur = cur; }
      return 0;
    }
    atomic_store_explicit(&node->next, CONSTRUCT(0, cur), memory_order_relaxed);
    if (qthread_cas((marked_ptr_t *)lprev,
                    atomic_load_explicit(&node->next, memory_order_relaxed),
                    CONSTRUCT(0, node)) == CONSTRUCT(0, cur)) {
      if (ocur) { *ocur = cur; }
      return 1;
    }
  }
} /*}}}*/

static int qt_lf_list_delete(_Atomic marked_ptr_t *head, so_key_t key) { /*{{{*/
  while (1) {
    _Atomic marked_ptr_t *lprev;
    marked_ptr_t lcur;
    marked_ptr_t lnext;

    if (qt_lf_list_find(head, key, &lprev, &lcur, &lnext) == NULL) { return 0; }
    if (qthread_cas_ptr((marked_ptr_t *)&PTR_OF(lcur)->next,
                        CONSTRUCT(0, lnext),
                        CONSTRUCT(1, lnext)) != (void *)CONSTRUCT(0, lnext)) {
      continue;
    }
    if (qthread_cas((marked_ptr_t *)lprev,
                    CONSTRUCT(0, lcur),
                    CONSTRUCT(0, lnext)) == CONSTRUCT(0, lcur)) {
      FREE_HASH_ENTRY(PTR_OF(lcur));
    } else {
      qt_lf_list_find(
        head, key, NULL, NULL, NULL); // needs to set cur/prev/next
    }
    return 1;
  }
} /*}}}*/

static void *qt_lf_list_find(_Atomic marked_ptr_t *head,
                             so_key_t key,
                             _Atomic marked_ptr_t **oprev,
                             marked_ptr_t *ocur,
                             marked_ptr_t *onext) { /*{{{*/
  so_key_t ckey;
  void *cval;
  _Atomic marked_ptr_t *prev = NULL;
  marked_ptr_t cur = UNINITIALIZED;
  marked_ptr_t next = UNINITIALIZED;

  while (1) {
    prev = head;
    cur = atomic_load_explicit(prev, memory_order_relaxed);
    while (1) {
      if (PTR_OF(cur) == NULL) {
        if (oprev) { *oprev = prev; }
        if (ocur) { *ocur = cur; }
        if (onext) { *onext = next; }
        return 0;
      }
      next = atomic_load_explicit(&PTR_OF(cur)->next, memory_order_relaxed);
      ckey = atomic_load_explicit(&PTR_OF(cur)->key, memory_order_relaxed);
      cval = atomic_load_explicit(&PTR_OF(cur)->value, memory_order_relaxed);
      if (atomic_load_explicit(prev, memory_order_relaxed) !=
          CONSTRUCT(0, cur)) {
        break; // this means someone mucked with the list; start over
      }
      if (!MARK_OF(next)) { // if next pointer is not marked
        if (ckey >= key) {  // if current key > key, the key isn't in the list;
                            // if current key == key, the key IS in the list
          if (oprev) { *oprev = prev; }
          if (ocur) { *ocur = cur; }
          if (onext) { *onext = next; }
          return (ckey == key) ? cval : NULL;
        }
        // but if current key < key, the we don't know yet, keep looking
        prev = &(PTR_OF(cur)->next);
      } else {
        if (qthread_cas((marked_ptr_t *)prev,
                        CONSTRUCT(0, cur),
                        CONSTRUCT(0, next)) == CONSTRUCT(0, cur)) {
          FREE_HASH_ENTRY(PTR_OF(cur));
        } else {
          break;
        }
      }
      cur = next;
    }
  }
} /*}}}*/

static inline so_key_t so_regularkey(lkey_t const key) {
  return REVERSE(key | MSB);
}

static inline so_key_t so_dummykey(lkey_t const key) { return REVERSE(key); }

static void qt_hash_subsystem_shutdown(void) {
  assert(hash_entry_pool != NULL);
  qt_mpool_destroy(hash_entry_pool);
  hash_entry_pool = NULL;
}

void INTERNAL qt_hash_initialize_subsystem(void) {
  assert(hash_entry_pool == NULL);
  hash_entry_pool = qt_mpool_create(sizeof(hash_entry));
  assert(hash_entry_pool != NULL);
  qthread_internal_cleanup_late(qt_hash_subsystem_shutdown);
}

int INTERNAL qt_hash_put(qt_hash h, qt_key_t key, void *value) {
  hash_entry *node = ALLOC_HASH_ENTRY();
  size_t bucket;
  lkey_t lkey = (uint64_t)(uintptr_t)key;

  HASH_KEY(lkey);
  bucket = lkey % h->size;

  assert(node);
  assert((lkey & MSB) == 0);
  atomic_store_explicit(&node->key, so_regularkey(lkey), memory_order_relaxed);
  atomic_store_explicit(&node->value, value, memory_order_relaxed);
  atomic_store_explicit(&node->next, UNINITIALIZED, memory_order_relaxed);

  if (atomic_load_explicit(&h->B[bucket], memory_order_relaxed) ==
      UNINITIALIZED) {
    initialize_bucket(h, bucket);
  }
  if (!qt_lf_list_insert(&(h->B[bucket]), node, NULL)) {
    FREE_HASH_ENTRY(node);
    return 0;
  }
  size_t csize = h->size;
  if (qthread_incr(&h->count, 1) / csize > MAX_LOAD) {
    if (2 * csize <= hard_max_buckets) { // this caps the size of the hash
      qthread_cas(&h->size, csize, 2 * csize);
    }
  }
  return 1;
}

void INTERNAL *qt_hash_get(qt_hash h, qt_key_t const key) {
  size_t bucket;
  lkey_t lkey = (uint64_t)(uintptr_t)key;

  HASH_KEY(lkey);
  bucket = lkey % h->size;

  if (atomic_load_explicit(&h->B[bucket], memory_order_relaxed) ==
      UNINITIALIZED) {
    // You'd think returning NULL at this point would be a good idea; but
    // if we do that, we risk losing key/value pairs (incorrectly reporting
    // them as absent) when the hash table resizes
    initialize_bucket(h, bucket);
  }
  return qt_lf_list_find(
    &(h->B[bucket]), so_regularkey(lkey), NULL, NULL, NULL);
}

int INTERNAL qt_hash_remove(qt_hash h, qt_key_t const key) {
  size_t bucket;
  lkey_t lkey = (uint64_t)(uintptr_t)key;

  HASH_KEY(lkey);
  bucket = lkey % h->size;

  if (atomic_load_explicit(&h->B[bucket], memory_order_relaxed) ==
      UNINITIALIZED) {
    initialize_bucket(h, bucket);
  }
  if (!qt_lf_list_delete(&(h->B[bucket]), so_regularkey(lkey))) { return 0; }
  qthread_incr(&h->count, -1);
  return 1;
}

static inline size_t GET_PARENT(uint64_t bucket) {
  uint64_t t = bucket;

  t |= t >> 1;
  t |= t >> 2;
  t |= t >> 4;
  t |= t >> 8;
  t |= t >> 16;
  t |= t >> 32; // creates a mask
  return bucket & (t >> 1);
}

static void initialize_bucket(qt_hash h, size_t bucket) {
  size_t parent = GET_PARENT(bucket);
  marked_ptr_t cur;

  if (atomic_load_explicit(&h->B[parent], memory_order_relaxed) ==
      UNINITIALIZED) {
    initialize_bucket(h, parent);
  }
  hash_entry *dummy = ALLOC_HASH_ENTRY();
  assert(dummy);
  atomic_store_explicit(
    &dummy->key, so_dummykey((lkey_t)bucket), memory_order_relaxed);
  atomic_store_explicit(&dummy->value, NULL, memory_order_relaxed);
  atomic_store_explicit(&dummy->next, UNINITIALIZED, memory_order_relaxed);
  if (!qt_lf_list_insert(&(h->B[parent]), dummy, &cur)) {
    FREE_HASH_ENTRY(dummy);
    dummy = PTR_OF(cur);
    while (atomic_load_explicit(&h->B[bucket], memory_order_relaxed) !=
           CONSTRUCT(0, dummy));
  } else {
    atomic_store_explicit(
      &h->B[bucket], CONSTRUCT(0, dummy), memory_order_relaxed);
  }
}

qt_hash INTERNAL qt_hash_create(int needSync) {
  qt_hash tmp = MALLOC(sizeof(struct qt_hash_s));

  assert(tmp);
  if (hard_max_buckets == 0) {
    hard_max_buckets = getpagesize() / sizeof(marked_ptr_t);
  }
  tmp->B = qt_calloc(hard_max_buckets, sizeof(marked_ptr_t));
  assert(atomic_load_explicit(&tmp->B, memory_order_relaxed));
  tmp->size = 2;
  tmp->count = 0;
  {
    hash_entry *dummy = ALLOC_HASH_ENTRY();
    assert(dummy);
    memset(dummy, 0, sizeof(hash_entry));
    atomic_store_explicit(
      &tmp->B[0], CONSTRUCT(0, dummy), memory_order_relaxed);
  }
  return tmp;
}

void INTERNAL qt_hash_destroy(qt_hash h) {
  marked_ptr_t cursor;

  assert(h);
  assert(h->B);
  cursor = atomic_load_explicit(&h->B[0], memory_order_relaxed);
  while (PTR_OF(cursor) != NULL) {
    hash_entry *tmp = PTR_OF(cursor);
    assert(MARK_OF(cursor) == 0);
    cursor = atomic_load_explicit(&PTR_OF(cursor)->next, memory_order_relaxed);
    FREE_HASH_ENTRY(tmp);
  }
  FREE(h->B, hard_max_buckets * sizeof(marked_ptr_t));
  FREE(h, sizeof(struct qt_hash_s));
}

void INTERNAL qt_hash_destroy_deallocate(qt_hash h, qt_hash_deallocator_fn f) {
  marked_ptr_t cursor;

  assert(h);
  assert(atomic_load_explicit(&h->B, memory_order_relaxed));
  cursor = atomic_load_explicit(&h->B[0], memory_order_relaxed);
  while (PTR_OF(cursor) != NULL) {
    hash_entry *he = PTR_OF(cursor);
    assert(MARK_OF(cursor) == 0);
    void *val = atomic_load_explicit(&he->value, memory_order_relaxed);
    if (val) { /* null pointers come from placeholders */
      f(val);
    }
    cursor = atomic_load_explicit(&he->next, memory_order_relaxed);
    FREE_HASH_ENTRY(he);
  }
  FREE(h->B, hard_max_buckets * sizeof(marked_ptr_t));
  FREE(h, sizeof(struct qt_hash_s));
}

size_t INTERNAL qt_hash_count(qt_hash h) {
  assert(h);
  return h->size;
}

void INTERNAL qt_hash_callback(qt_hash h, qt_hash_callback_fn f, void *arg) {
  marked_ptr_t cursor;

  assert(h);
  assert(h->B);
  cursor = atomic_load_explicit(&h->B[0], memory_order_relaxed);
  while (PTR_OF(cursor) != NULL) {
    so_key_t key =
      atomic_load_explicit(&PTR_OF(cursor)->key, memory_order_relaxed);
    if (MARK_OF(key)) {
      f((qt_key_t)(uintptr_t)REVERSE(key ^ 1),
        atomic_load_explicit(&PTR_OF(cursor)->value, memory_order_relaxed),
        arg);
    } else {
      // f((qt_key_t)REVERSE(key), PTR_OF(cursor)->value, (void *)1);
    }
    cursor = atomic_load_explicit(&PTR_OF(cursor)->next, memory_order_relaxed);
  }
}

/* vim:set expandtab: */
