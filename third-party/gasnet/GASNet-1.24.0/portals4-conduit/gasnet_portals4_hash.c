/*   $Source: bitbucket.org:berkeleylab/gasnet.git/portals4-conduit/gasnet_portals4_hash.c $
 * Description: GASNet header for portals4-conduit lock-free hash table
 * Copyright 2012, Sandia National Laboratories
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_handler.h>
#include <gasnet_core_internal.h>

/* System Headers */
#include <stdlib.h> /* for malloc/free/etc */
#include <unistd.h> /* for getpagesize() */
#include <errno.h>
#include <unistd.h>
#include <signal.h>

/* Internal Headers */
#include <gasnet_portals4.h>

/* The Internal API */
#include "gasnet_portals4_hash.h"

/*
 * The hash table in this file is based on the work by Ori Shalev and Nir Shavit
 * Much of the code is similar to the example code they published in their
 * paper "Split-Ordered Lists: Lock-Free Extensible Hash Tables", but has been
 * modified to support additional semantics.
 *
 * NOTE: I did not implement the Dynamic-Sized Array extension, because I don't
 * expect to support large numbers of entries in the table at the same time,
 * and I'm worried about state creep.   -Kyle Wheeler
 */

#define MAX_LOAD     4
#define USE_HASHWORD 1

/* internal types */
typedef uint64_t lkey_t;
typedef uint64_t so_key_t;
typedef uintptr_t marked_ptr_t;

#define MARK_OF(x)           ((x) & 1)
#define PTR_MASK(x)          ((x) & ~(marked_ptr_t)1)
#define PTR_OF(x)            ((hash_entry *)PTR_MASK(x))
#define CONSTRUCT0(ptr)      PTR_MASK((marked_ptr_t)ptr)
#define CONSTRUCT1(ptr)      ((marked_ptr_t)ptr|1)
#define UNINITIALIZED ((marked_ptr_t)0)

/* This is more or less a NO-OP on x86-64, but MIGHT be required on other arches */
/* TODO: can we weaken this? */
#define CAS_MB (GASNETI_ATOMIC_REL | GASNETI_ATOMIC_ACQ)

#if PLATFORM_ARCH_32
# ifdef GASNETI_ATOMIC32_NOT_SIGNALSAFE
#  error "this code requires native 32-bit atomics"
# endif
# define CAS_marked_ptr(_p,_o,_n) gasneti_weakatomic32_compare_and_swap((gasneti_weakatomic32_t*)(_p),(_o),(_n),CAS_MB)
#elif PLATFORM_ARCH_64
# ifdef GASNETI_ATOMIC64_NOT_SIGNALSAFE
#  error "this code requires native 64-bit atomics"
# endif
# define CAS_marked_ptr(_p,_o,_n) gasneti_weakatomic64_compare_and_swap((gasneti_weakatomic64_t*)(_p),(_o),(_n),CAS_MB)
#else
# error "Unknown pointer width"
#endif

#if !defined(GASNETI_HAVE_WEAKATOMIC_ADD_SUB) || !defined(GASNETI_HAVE_WEAKATOMIC_CAS)
# error "this code requires atomic additon and compare-and-swap"
#endif

static size_t   hard_max_buckets = 0;

typedef struct hash_entry_s {
    so_key_t     key;
    void        *value;
    marked_ptr_t next;
} hash_entry;

struct gasnetc_hash_s {
    marked_ptr_t   *B;  /* Buckets */
    gasneti_weakatomic_t count;
    /* 'size' is always a power-of-two.
     * So, we replace it by mask = (size-1) to allowing transforming of '%' to '&' */
    gasneti_weakatomic_t mask;
};

/* A lock-free hash table deserves lock-free allocation */
#if 1
  static gasneti_lifo_head_t entry_pool = GASNETI_LIFO_INITIALIZER;
  GASNETI_ALWAYS_INLINE(alloc_hash_entry)
  hash_entry *alloc_hash_entry(void)
  {
    hash_entry *entry = gasneti_lifo_pop(&entry_pool);
    if_pf (NULL == entry) {
      gasneti_leak(entry = gasneti_malloc(sizeof(hash_entry)));
    }
    return entry;
  }
# define ALLOC_HASH_ENTRY() alloc_hash_entry()
# define FREE_HASH_ENTRY(t) gasneti_lifo_push(&entry_pool, (t))
#else
# define ALLOC_HASH_ENTRY() gasneti_malloc(sizeof(hash_entry))
# define FREE_HASH_ENTRY(t) gasneti_free(t)
#endif

/* prototypes */
static void *gasnetc_lf_list_find(marked_ptr_t  *head,
                             so_key_t       key,
                             marked_ptr_t **prev,
                             marked_ptr_t  *cur,
                             marked_ptr_t  *next);
static void initialize_bucket(gasnetc_hash h,
                              size_t  bucket);

#define REVERSE_BYTE(x) ((so_key_t)((((((uint32_t)(x)) * 0x0802LU & 0x22110LU) | (((uint32_t)(x)) * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16) & 0xff))
#if PLATFORM_ARCH_32
# define MSB (((uint64_t)1) << 31)
# define REVERSE(x) (REVERSE_BYTE((((so_key_t)(x))) & 0xff) << 24) | \
    (REVERSE_BYTE((((so_key_t)(x)) >> 8) & 0xff) << 16) |            \
    (REVERSE_BYTE((((so_key_t)(x)) >> 16) & 0xff) << 8) |            \
    (REVERSE_BYTE((((so_key_t)(x)) >> 24) & 0xff))
#elif PLATFORM_ARCH_64
# define MSB (((uint64_t)1) << 63)
# define REVERSE(x) ((REVERSE_BYTE((((so_key_t)(x))) & 0xff) << 56) |       \
                     (REVERSE_BYTE((((so_key_t)(x)) >> 8) & 0xff) << 48) |  \
                     (REVERSE_BYTE((((so_key_t)(x)) >> 16) & 0xff) << 40) | \
                     (REVERSE_BYTE((((so_key_t)(x)) >> 24) & 0xff) << 32) | \
                     (REVERSE_BYTE((((so_key_t)(x)) >> 32) & 0xff) << 24) | \
                     (REVERSE_BYTE((((so_key_t)(x)) >> 40) & 0xff) << 16) | \
                     (REVERSE_BYTE((((so_key_t)(x)) >> 48) & 0xff) << 8) |  \
                     (REVERSE_BYTE((((so_key_t)(x)) >> 56) & 0xff) << 0))
#else
# error "Unknown wordsize"
#endif

/* TODO: This hash is probably waaay to expensive for the way the table is being
 * used right now (perhaps a dozen keys?), but is probably a good choice for
 * more general-purpose use.  So, what should be done here?   -PHH 2013.04.15
 */
#ifdef USE_HASHWORD
# define HASH_KEY(key) key = gasnetc_hashword(key)
/* this function based on http://burtleburtle.net/bob/hash/evahash.html */
/* TODO: inline asm for CPUs w/ rotate instructions? */
# define rot(x, k) (((x) << (k)) | ((x) >> (32 - (k))))
static uint64_t gasnetc_hashword(uint64_t key)
{   /*{{{*/
    uint32_t a, b, c;

    const union {
        uint64_t key;
        uint8_t  b[sizeof(uint64_t)];
    } k = {
        key
    };

    a  = b = c = 0x32533d0c + sizeof(uint64_t); /* an arbitrary value, randomly selected */
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
    return ((uint64_t)c + (((uint64_t)b) << 32)) & (~MSB);
} /*}}}*/

#else /* ifdef USE_HASHWORD */
# define HASH_KEY(key)
#endif /* ifdef USE_HASHWORD */

static int gasnetc_lf_list_insert(marked_ptr_t *head,
                             hash_entry   *node,
                             marked_ptr_t *ocur)
{   /*{{{*/
    so_key_t key = node->key;

    while (1) {
        marked_ptr_t *lprev;
        marked_ptr_t  cur;

        if (gasnetc_lf_list_find(head, key, &lprev, &cur, NULL) != NULL) { /* needs to set cur/prev */
            if (ocur) { *ocur = cur; }
            return 0;
        }
        node->next = CONSTRUCT0(cur);
        if (CAS_marked_ptr(lprev, node->next, CONSTRUCT0(node))) {
            if (ocur) { *ocur = cur; }
            return 1;
        }
    }
} /*}}}*/

static int gasnetc_lf_list_delete(marked_ptr_t *head,
                             so_key_t      key)
{   /*{{{*/
    while (1) {
        marked_ptr_t *lprev;
        marked_ptr_t  lcur;
        marked_ptr_t  lnext;

        if (gasnetc_lf_list_find(head, key, &lprev, &lcur, &lnext) == NULL) { return 0; }
        if (!CAS_marked_ptr(&PTR_OF(lcur)->next, CONSTRUCT0(lnext), CONSTRUCT1(lnext))) { continue; }
        if (CAS_marked_ptr(lprev, CONSTRUCT0(lcur), CONSTRUCT0(lnext))) {
            FREE_HASH_ENTRY(PTR_OF(lcur));
        } else {
            gasnetc_lf_list_find(head, key, NULL, NULL, NULL); /* needs to set cur/prev/next */
        }
        return 1;
    }
} /*}}}*/

static void *gasnetc_lf_list_find(marked_ptr_t  *head,
                             so_key_t       key,
                             marked_ptr_t **oprev,
                             marked_ptr_t  *ocur,
                             marked_ptr_t  *onext)
{   /*{{{*/
    so_key_t      ckey;
    void         *cval;
    marked_ptr_t *prev = NULL;
    marked_ptr_t  cur  = UNINITIALIZED;
    marked_ptr_t  next = UNINITIALIZED;

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
            next = PTR_OF(cur)->next;
            ckey = PTR_OF(cur)->key;
            cval = PTR_OF(cur)->value;
            if (*prev != CONSTRUCT0(cur)) {
                break; /* this means someone mucked with the list; start over */
            }
            if (!MARK_OF(next)) {  /* if next pointer is not marked */
                if (ckey >= key) { /* if current key > key, the key isn't in the list; if current key == key, the key IS in the list */
                    if (oprev) { *oprev = prev; }
                    if (ocur) { *ocur = cur; }
                    if (onext) { *onext = next; }
                    return (ckey == key) ? cval : NULL;
                }
                /* but if current key < key, the we don't know yet, keep looking */
                prev = &(PTR_OF(cur)->next);
            } else {
                if (CAS_marked_ptr(prev, CONSTRUCT0(cur), CONSTRUCT0(next))) {
                    FREE_HASH_ENTRY(PTR_OF(cur));
                } else {
                    break;
                }
            }
            cur = next;
        }
    }
} /*}}}*/

GASNETI_ALWAYS_INLINE(so_regularkey) GASNETI_CONST
so_key_t so_regularkey(const lkey_t key)
{
    return REVERSE(key | MSB);
}
GASNETI_CONSTP(so_regularkey)

GASNETI_ALWAYS_INLINE(so_dummykey) GASNETI_CONST
so_key_t so_dummykey(const lkey_t key)
{
    return REVERSE(key);
}
GASNETI_CONSTP(so_dummykey)

void  gasnetc_hash_initialize_subsystem(void)
{
}

int  gasnetc_hash_put(gasnetc_hash  h,
                         gasnetc_key_t key,
                         void    *value)
{
    hash_entry *node = ALLOC_HASH_ENTRY();
    size_t      bucket;
    lkey_t      lkey = (lkey_t)(uintptr_t)key;
    size_t csize;

    HASH_KEY(lkey);
    bucket = lkey & gasneti_weakatomic_read(&h->mask, 0);

    gasneti_assert((lkey & MSB) == 0);
    node->key   = so_regularkey(lkey);
    node->value = value;
    node->next  = UNINITIALIZED;

    if (h->B[bucket] == UNINITIALIZED) {
        initialize_bucket(h, bucket);
    }
    if (!gasnetc_lf_list_insert(&(h->B[bucket]), node, NULL)) {
        FREE_HASH_ENTRY(node);
        return 0;
    }
    csize = 1 + gasneti_weakatomic_read(&h->mask, 0);
    /* gasneti_weakatomic_add() is add-and-fetch, so we must offset */
    if ((gasneti_weakatomic_add(&h->count, 1, 0) - 1) / csize > MAX_LOAD) {
        if (2 * csize <= hard_max_buckets) { /* this caps the size of the hash */
            (void) gasneti_weakatomic_compare_and_swap(&h->mask, csize-1, (2 * csize) - 1, 0);
            gasneti_assert(GASNETI_POWEROFTWO(csize));
        }
    }
    return 1;
}

/* PHH: _put extended to return the pre-existing value on failure */
void *gasnetc_hash_put_find(gasnetc_hash  h,
                         gasnetc_key_t key,
                         void    *value)
{
    marked_ptr_t cur;
    hash_entry *node = ALLOC_HASH_ENTRY();
    size_t      bucket;
    lkey_t      lkey = (lkey_t)(uintptr_t)key;
    size_t csize;

    HASH_KEY(lkey);
    bucket = lkey & gasneti_weakatomic_read(&h->mask, 0);

    gasneti_assert((lkey & MSB) == 0);
    node->key   = so_regularkey(lkey);
    node->value = value;
    node->next  = UNINITIALIZED;

    if (h->B[bucket] == UNINITIALIZED) {
        initialize_bucket(h, bucket);
    }
    if (!gasnetc_lf_list_insert(&(h->B[bucket]), node, &cur)) {
        FREE_HASH_ENTRY(node);
        return PTR_OF(cur)->value;
    }
    csize = 1 + gasneti_weakatomic_read(&h->mask, 0);
    /* gasneti_weakatomic_add() is add-and-fetch, so we must offset */
    if ((gasneti_weakatomic_add(&h->count, 1, 0) - 1) / csize > MAX_LOAD) {
        if (2 * csize <= hard_max_buckets) { /* this caps the size of the hash */
            (void) gasneti_weakatomic_compare_and_swap(&h->mask, csize-1, (2 * csize) - 1, 0);
            gasneti_assert(GASNETI_POWEROFTWO(csize));
        }
    }
    return NULL;
}

void  *gasnetc_hash_get(gasnetc_hash        h,
                           const gasnetc_key_t key)
{
    size_t bucket;
    lkey_t lkey = (lkey_t)(uintptr_t)key;

    HASH_KEY(lkey);
    bucket = lkey & gasneti_weakatomic_read(&h->mask, 0);

    if (h->B[bucket] == UNINITIALIZED) {
        /* You'd think returning NULL at this point would be a good idea; but
           if we do that, we risk losing key/value pairs (incorrectly reporting
           them as absent) when the hash table resizes */
        initialize_bucket(h, bucket);
    }
    return gasnetc_lf_list_find(&(h->B[bucket]), so_regularkey(lkey), NULL, NULL, NULL);
}

int  gasnetc_hash_remove(gasnetc_hash        h,
                            const gasnetc_key_t key)
{
    size_t bucket;
    lkey_t lkey = (lkey_t)(uintptr_t)key;

    HASH_KEY(lkey);
    bucket = lkey & gasneti_weakatomic_read(&h->mask, 0);

    if (h->B[bucket] == UNINITIALIZED) {
        initialize_bucket(h, bucket);
    }
    if (!gasnetc_lf_list_delete(&(h->B[bucket]), so_regularkey(lkey))) {
        return 0;
    }
    gasneti_weakatomic_decrement(&h->count, 0);
    return 1;
}

GASNETI_ALWAYS_INLINE(GET_PARENT) GASNETI_CONST
size_t GET_PARENT(uint64_t bucket)
{
    uint64_t t = bucket;

    t |= t >> 1;
    t |= t >> 2;
    t |= t >> 4;
    t |= t >> 8;
    t |= t >> 16;
    t |= t >> 32;     /* creates a mask */
    return bucket & (t >> 1);
}
GASNETI_CONSTP(GET_PARENT)

static void initialize_bucket(gasnetc_hash h,
                              size_t  bucket)
{
    size_t       parent = GET_PARENT(bucket);
    marked_ptr_t cur;
    hash_entry *dummy;
    if (h->B[parent] == UNINITIALIZED) {
        initialize_bucket(h, parent);
    }
    dummy = ALLOC_HASH_ENTRY();
    dummy->key   = so_dummykey((lkey_t)bucket);
    dummy->value = NULL;
    dummy->next  = UNINITIALIZED;
    if (!gasnetc_lf_list_insert(&(h->B[parent]), dummy, &cur)) {
        FREE_HASH_ENTRY(dummy);
        dummy = PTR_OF(cur);
        while (h->B[bucket] != CONSTRUCT0(dummy)) ;
    } else {
        h->B[bucket] = CONSTRUCT0(dummy);
    }
}

gasnetc_hash  gasnetc_hash_create()
{
    gasnetc_hash tmp = gasneti_malloc(sizeof(struct gasnetc_hash_s));

    if (hard_max_buckets == 0) {
        hard_max_buckets = getpagesize() / sizeof(marked_ptr_t);
        gasneti_assert(GASNETI_POWEROFTWO(hard_max_buckets));
        gasneti_assert(hard_max_buckets <= GASNETI_ATOMIC_MAX);
    }
    tmp->B = gasneti_calloc(hard_max_buckets, sizeof(marked_ptr_t));
    gasneti_weakatomic_set(&tmp->mask, 1, 0); /* corresponds to size=2 */
    gasneti_weakatomic_set(&tmp->count, 0, 0);
    {
        hash_entry *dummy = ALLOC_HASH_ENTRY();
        memset(dummy, 0, sizeof(hash_entry));
        tmp->B[0] = CONSTRUCT0(dummy);
    }
    return tmp;
}

void  gasnetc_hash_destroy(gasnetc_hash h)
{
    marked_ptr_t cursor;

    gasneti_assert(h);
    gasneti_assert(h->B);
    cursor = h->B[0];
    while (PTR_OF(cursor) != NULL) {
        hash_entry *tmp = PTR_OF(cursor);
        gasneti_assert(MARK_OF(cursor) == 0);
        cursor = PTR_OF(cursor)->next;
        FREE_HASH_ENTRY(tmp);
    }
    gasneti_free(h->B);
    gasneti_free(h);
}

void  gasnetc_hash_destroy_deallocate(gasnetc_hash                h,
                                         gasnetc_hash_deallocator_fn f)
{
    marked_ptr_t cursor;

    gasneti_assert(h);
    gasneti_assert(h->B);
    cursor = h->B[0];
    while (PTR_OF(cursor) != NULL) {
        hash_entry *he = PTR_OF(cursor);
        gasneti_assert(MARK_OF(cursor) == 0);
        if (he->value) { /* null pointers come from placeholders */
            f(he->value);
        }
        cursor = he->next;
        FREE_HASH_ENTRY(he);
    }
    gasneti_free(h->B);
    gasneti_free(h);
}

/* TODO: this is the table size, not the number of entries */
size_t  gasnetc_hash_count(gasnetc_hash h)
{
    gasneti_assert(h);
    return 1 + gasneti_weakatomic_read(&h->mask, 0);
}

void  gasnetc_hash_callback(gasnetc_hash             h,
                               gasnetc_hash_callback_fn f,
                               void               *arg)
{
    marked_ptr_t cursor;

    gasneti_assert(h);
    gasneti_assert(h->B);
    cursor = h->B[0];
    while (PTR_OF(cursor) != NULL) {
        so_key_t key = PTR_OF(cursor)->key;
        gasneti_assert(MARK_OF(cursor) == 0);
        if (key & 1) {
            f((gasnetc_key_t)(uintptr_t)REVERSE(key ^ 1), PTR_OF(cursor)->value, arg);
        } else {
            /* f((gasnetc_key_t)REVERSE(key), PTR_OF(cursor)->value, (void *)1); */
        }
        cursor = PTR_OF(cursor)->next;
    }
}

/* vim:set expandtab: */
