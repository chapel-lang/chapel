#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <stdlib.h>
#include <unistd.h> // for getpagesize()
#include <string.h> // for memset()
#include <stdio.h>
#include <inttypes.h>

/* Installed Headers */
#include <qthread/dictionary.h>
#include <qthread/qthread.h>

/* Internal Headers */
#include "qt_asserts.h"
#include "qt_debug.h"
#ifdef EBUG
# define DEBUG(x) x
#else
# define DEBUG(x)
#endif

#define PUT_ALWAYS    0
#define PUT_IF_ABSENT 1
#define UNINITIALIZED (0)
/* external types */
typedef void *qt_key_t;
typedef struct qt_dictionary *qt_hash;
// typedef struct qt_hash_s *qt_hash;
typedef void (*qt_hash_callback_fn)(const qt_key_t,
                                    void *,
                                    void *);

#define MARK_OF(x)           ((x) & 1)
#define PTR_MASK(x)          ((x) & ~(marked_ptr_t)1)
#define PTR_OF(x)            ((hash_entry *)PTR_MASK(x))
#define CONSTRUCT(mark, ptr) (PTR_MASK((uintptr_t)ptr) | (mark))

#ifndef QTHREAD_VERSION
/* These are GCC builtins; other compilers may require inline assembly */
# define CAS(ADDR, OLDV, NEWV) __sync_val_compare_and_swap((ADDR), (OLDV), (NEWV))
# define INCR(ADDR, INCVAL)    __sync_fetch_and_add((ADDR), (INCVAL))
#else
# define CAS(ADDR, OLDV, NEWV) qthread_cas((ADDR), (OLDV), (NEWV))
# define INCR(ADDR, INCVAL)    qthread_incr((ADDR), (INCVAL))
#endif

#define SPINE_LENGTH       32
#define SPINE_BUCKET_WIDTH 5
#define BASE_SPINE_LENGTH  64
#define BASE_BUCKET_WIDTH  6
/*
 * typedef struct hash_entry_s {
 *  qt_key_t key;
 *  void    *value;
 *
 * } hash_entry;
 */

typedef struct list_entry hash_entry;

typedef union _spine_element {
    uint64_t    u;
    hash_entry *e;
    struct {
        uint32_t ctr;
        uint32_t id;
    } s;
} spine_element_t;

typedef struct _spine_s {
    spine_element_t *parent;
    spine_element_t  elements[SPINE_LENGTH];
} spine_t;

struct qt_dictionary {
    spine_element_t      base[BASE_SPINE_LENGTH];
    spine_t            **spines;
    size_t               count;
    size_t               maxspines;
    size_t               numspines;

    qt_dict_key_equals_f op_equals;
    qt_dict_hash_f       op_hash;
    qt_dict_cleanup_f    op_cleanup;
};

static void destroy_spine(qt_hash           h,
                          spine_t          *spine,
                          qt_dict_cleanup_f f);
static void destroy_element(hash_entry       *element,
                            qt_dict_cleanup_f f);

static inline qt_hash qt_hash_create(qt_dict_key_equals_f eq,
                                     qt_dict_hash_f       hash,
                                     qt_dict_cleanup_f    cleanup)
{
    qt_hash tmp = MALLOC(sizeof(qt_dictionary));

    tmp->op_equals  = eq;
    tmp->op_hash    = hash;
    tmp->op_cleanup = cleanup;

    assert(tmp);
    tmp->maxspines = getpagesize() / sizeof(spine_element_t *);
    tmp->spines    = (spine_t **) qt_calloc(tmp->maxspines,
                                            sizeof(spine_element_t *));

    return tmp;
}

qt_dictionary *qt_dictionary_create(qt_dict_key_equals_f eq,
                                    qt_dict_hash_f       hash,
                                    qt_dict_cleanup_f    cleanup)
{
    return qt_hash_create(eq, hash, cleanup);
}

#define SPINE_PTR_TEST(x)            ((x).u & 1)
#define SPINE_PTR_COUNT(x)           ((x).s.ctr >> 1)
#define SPINE_PTR_ID(x)              ((x).s.id)
#define SPINE_PTR(h, x)              (h->spines[SPINE_PTR_ID(x)])
#define BASE_SPINE_BUCKET(key)       (((uintptr_t)(key)) & (BASE_SPINE_LENGTH - 1))
#define SPINE_BUCKET(key, depth)     ((((uintptr_t)(key)) >> (BASE_BUCKET_WIDTH + (SPINE_BUCKET_WIDTH * ((depth) - 1)))) & (SPINE_LENGTH - 1))
#define SPINE_COUNT(x)               ((x << 1) | 1)
#define SPINE_PTR_UPDATE(ptr, count) ((spine_element_t)(uint64_t)(SPINE_PTR_ID(ptr) | SPINE_COUNT(count)))

#define INCREMENT_COUNT(bkt, oldval)    {                          \
        spine_element_t newcount;                                  \
        spine_element_t tmp   = oldval;                            \
        uint32_t        count = SPINE_PTR_COUNT(oldval) + 1;       \
                                                                   \
        assert(count < 0x7fffffff); /* something awful happened */ \
                                                                   \
        newcount.s.id  = child_val.s.id;                           \
        newcount.s.ctr = SPINE_COUNT(count);                       \
        tmp.u          = CAS(&(bkt->u), oldval.u, newcount.u);     \
        if (tmp.u != oldval.u) {                                   \
            oldval = tmp;                                          \
            continue;                                              \
        }                                                          \
}
#define DECREMENT_COUNT_BY(bkt, decrby) {                 \
        spine_element_t oldval = *bkt;                    \
        spine_element_t newcount;                         \
        spine_element_t tmp = oldval;                     \
        uint32_t        count;                            \
                                                          \
        do {                                              \
            oldval = tmp;                                 \
            count  = SPINE_PTR_COUNT(oldval) - decrby;    \
            if (count == 0) {                             \
                newcount.e = NULL;                        \
            } else {                                      \
                newcount.s.id  = oldval.s.id;             \
                newcount.s.ctr = SPINE_COUNT(count);      \
            }                                             \
            tmp.u = CAS(&(bkt->u), oldval.u, newcount.u); \
        } while (tmp.u != oldval.u);                      \
        if (count == 0) {                                 \
            deallocate_spine(h, oldval.s.id);             \
        }                                                 \
}
#define DECREMENT_COUNT(bkt)            DECREMENT_COUNT_BY(bkt, 1)

static void deallocate_spine(qt_hash h,
                             size_t  id)
{
    qt_free((void *)(h->spines[id])); // XXX should be to a memory pool
    h->spines[id] = NULL;
    INCR(&h->numspines, -1);
}

static size_t allocate_spine(qt_hash   h,
                             spine_t **realspine)
{
    size_t   newspine_ct = INCR(&h->numspines, 1) + 1;
    size_t   maxspines   = h->maxspines;
    spine_t *newspine;
    size_t   id;

    if (newspine_ct >= maxspines) {
        // Need to make more room in the spine-idx array
        spine_t **spines    = qt_calloc(maxspines * 2, sizeof(spine_t *));
        spine_t **oldspines = h->spines;
        spine_t **tmp;
        assert(spines);
        memcpy(spines, h->spines, sizeof(spine_t *) * maxspines);
        tmp = CAS(&h->spines, oldspines, spines);
        if (tmp != oldspines) { // someone else added more spines
            FREE(spines, maxspines * 2 * sizeof(spine_t *));
        } else {
            INCR(&h->maxspines, maxspines);
        }
        maxspines *= 2;
    }
    newspine = qt_calloc(1, sizeof(spine_t)); // XXX should be from a memory pool
    assert(newspine);
    for(id = 0; id < maxspines; ++id) {
        if (h->spines[id] == NULL) {
            spine_t *tmp = CAS(&h->spines[id], NULL, newspine);
            if (tmp == NULL) {
                break; // success!
            }
        }
    }
    assert(id < maxspines);
    if (realspine != NULL) {
        *realspine = (spine_t *)newspine;
    }

    return id;
}

static void destroy_spine(qt_dictionary    *h,
                          spine_t          *spine,
                          qt_dict_cleanup_f f)
{
    assert(spine);
    for (size_t i = 0; i < SPINE_LENGTH; ++i) {
        if (spine->elements[i].e != NULL) {
            if (SPINE_PTR_TEST(spine->elements[i])) { // spine
                destroy_spine(h, SPINE_PTR(h, spine->elements[i]), f);
            } else {
                destroy_element(spine->elements[i].e, f);
            }
        }
    }
}

static void destroy_element(hash_entry       *element,
                            qt_dict_cleanup_f f)
{
    assert(element);
    if (f) {
        f(element->key, element->value);
    }
    FREE(element, sizeof(hash_entry));
}

void qt_dictionary_destroy(qt_hash h)
{
    assert(h);
    assert(h->spines);
    for (size_t i = 0; i < BASE_SPINE_LENGTH; ++i) {
        if (h->base[i].e != NULL) {
            if (SPINE_PTR_TEST(h->base[i])) { // spine
                destroy_spine(h, SPINE_PTR(h, h->base[i]), h->op_cleanup);
            } else { // element
                destroy_element(h->base[i].e, h->op_cleanup);
            }
        }
    }
    if (h->numspines > 0) {
        for (size_t i = 0; i < h->maxspines; ++i) {
            if (h->spines[i] != NULL) {
                h->numspines--;
                qt_free((void *)(h->spines[i])); // XXX should be into a memory pool
                if (h->numspines == 0) { break; }
            }
        }
    }
    FREE(h->spines, h->maxspines * sizeof(spine_element_t *));
    FREE(h, sizeof(qt_dictionary));
}

/*
 * void qt_hash_destroy_deallocate(qt_hash                h,
 *                              qt_hash_deallocator_fn f)
 * {
 *  assert(h);
 *  assert(h->spines);
 *  for (size_t i = 0; i < BASE_SPINE_LENGTH; ++i) {
 *      if (h->base[i].e != NULL) {
 *          if (SPINE_PTR_TEST(h->base[i])) { // spine
 *              destroy_spine(h, SPINE_PTR(h, h->base[i]), f);
 *          } else { // element
 *              destroy_element(h->base[i].e, f);
 *          }
 *      }
 *  }
 *  for (size_t i = 0; i < h->numspines; ++i) {
 *      assert(h->spines[i] != NULL);
 *      free((void *)(h->spines[i]));
 *  }
 *  free(h->spines);
 *  free(h);
 * }
 */
#ifdef USE_HASHWORD
# define HASH_KEY(key) key = qt_hash64(key)
#else  /* ifdef USE_HASHWORD */
# define HASH_KEY(key)
#endif  /* ifdef USE_HASHWORD */

void *qt_hash_put_helper(qt_dictionary *h,
                         qt_key_t       key,
                         void          *value,
                         int            put_choice);

void *qt_hash_put_helper(qt_dictionary *h,
                         qt_key_t       key,
                         void          *value,
                         int            put_choice)
{
    uint64_t lkey = (uint64_t)(uintptr_t)(h->op_hash(key));

    HASH_KEY(lkey);
    assert(h);

    unsigned         bucket    = BASE_SPINE_BUCKET(lkey);
    hash_entry      *e         = qt_malloc(sizeof(hash_entry));   // XXX: should be from a memory pool
    spine_element_t *child_id  = &(h->base[bucket]);
    spine_element_t  child_val = h->base[bucket];
    spine_element_t *cur_id    = NULL;
    spine_t         *cur_spine = NULL;
    unsigned         depth     = 0;

    assert(e != NULL);
    e->hashed_key = lkey;
    e->key        = key;
    e->value      = value;
    e->next       = NULL;
    hash_entry *crt;
    do {
        if (child_val.e == NULL) {
            // place the entry in the hash
            if ((child_val.e = CAS(&(child_id->e), NULL, e)) == NULL) {
                // put success: no potential colliding element was present
                return value;
            }
        } else if (SPINE_PTR_TEST(child_val)) {
            INCREMENT_COUNT(child_id, child_val);
            if (cur_id) {
                DECREMENT_COUNT(cur_id);
            }
            cur_id    = child_id;
            cur_spine = SPINE_PTR(h, child_val);
            depth++;
            assert(depth <= 11); // otherwise, something has gone horribly wrong
            bucket    = SPINE_BUCKET(lkey, depth);
            child_id  = &cur_spine->elements[bucket];
            child_val = cur_spine->elements[bucket];
        } else if (child_val.e->hashed_key != lkey) {
            // upgrade to a spine
            spine_element_t newspine, cur;
            spine_t        *realspine;
            unsigned        bucket1 = SPINE_BUCKET(child_val.e->hashed_key, depth + 1);
            unsigned        bucket2 = SPINE_BUCKET(lkey, depth + 1);

            newspine.s.id                = allocate_spine(h, &realspine);
            realspine->parent            = cur_id;
            realspine->elements[bucket1] = child_val;

            if (bucket1 != bucket2) {            // both elements will be in the new spine
                newspine.s.ctr = SPINE_COUNT(2); // contains 2 elements

                realspine->elements[bucket2].e = e;

                if ((cur.e = CAS(&(child_id->e), child_val.e, newspine.e)) == child_val.e) {
                    // success!
                    if (cur_id) {
                        DECREMENT_COUNT(cur_id);
                    }
                    return value;
                } else {
                    child_val = cur;
                    deallocate_spine(h, newspine.s.id);
                }
            } else {                             // collision in the new spine (unusual; will use unnecessary CAS)
                newspine.s.ctr = SPINE_COUNT(1); // contains 1 element (oldval)

                if ((cur.e = CAS(&(child_id->e), child_val.e, newspine.e)) == child_val.e) {
                    // success
                    continue;
                } else {
                    child_val = cur;
                    deallocate_spine(h, newspine.s.id);
                }
            }
        } else {
            // use the real user-equals operation to differentiate subcases
            // it is possible that the element is there or it may not be there
            hash_entry *head;
            do {
                head    = child_id->e;
                e->next = head;
                crt     = head;
                // find the entry, if it is in the list
                while (crt) {
                    if (h->op_equals(crt->key, key)) {
                        // already exists

                        if (put_choice != PUT_IF_ABSENT) {
                            void **crt_val_adr = &(crt->value);
                            void  *crt_val     = crt->value;
                            while((qthread_cas_ptr(crt_val_adr, \
                                                   crt_val, value)) != crt_val ) {
                                crt_val = crt->value;
                            }
                        }

                        if (cur_id) { DECREMENT_COUNT(cur_id); }
                        return crt->value;
                    }
                    crt = crt->next;
                }
                // and try to insert it at the head of the list;
                // if the list changed, redu the work
            } while (qthread_cas_ptr(&(child_id->e), head, e) != head);
            // printf("IN put: (%s-%s)\n", child_id->e->key, child_id->e->value);
            // if (e->next !=NULL)
            //	printf("next key is %s and value %s\n", e->next->key, e->next->value);
            return e->value;
        }
    } while (1);
}

void *qt_dictionary_put(qt_dictionary *dict,
                        void          *key,
                        void          *value)
{
    return qt_hash_put_helper(dict, key, value, PUT_ALWAYS);
}

void *qt_dictionary_put_if_absent(qt_dictionary *dict,
                                  void          *key,
                                  void          *value)
{
    return qt_hash_put_helper(dict, key, value, PUT_IF_ABSENT);
}

int qt_dictionary_remove(qt_dictionary *h,
                         const qt_key_t key);

int qt_dictionary_remove(qt_dictionary *h,
                         const qt_key_t key)
{
    uint64_t lkey = (uint64_t)(uintptr_t)(h->op_hash(key));

    HASH_KEY(lkey);
    HASH_KEY(lkey);
    assert(h);

    unsigned         bucket    = BASE_SPINE_BUCKET(lkey);
    spine_element_t *child_id  = &(h->base[bucket]);
    spine_element_t  child_val = h->base[bucket];
    spine_element_t *cur_id    = NULL;
    spine_t         *cur_spine = NULL;
    unsigned         depth     = 0;

    // First, find and remove the item itself
    do {
        if (child_val.e == NULL) {
            // not found
            return 0;
        } else if (SPINE_PTR_TEST(child_val)) {
            cur_id    = child_id;
            cur_spine = SPINE_PTR(h, child_val);
            depth++;
            assert(depth <= 11);  // otherwise, something has gone horribly wrong
            bucket    = SPINE_BUCKET(lkey, depth);
            child_id  = &cur_spine->elements[bucket];
            child_val = cur_spine->elements[bucket];
        } else if (child_val.e->hashed_key == lkey) {
            hash_entry  *e    = child_id->e;
            hash_entry **prev = &(child_id->e);
            do {
                if (h->op_equals(e->key, key)) {
                    spine_element_t cur;
                    if ((cur.e = CAS(prev, e, e->next)) == e) {
                        if (h->op_cleanup != NULL) {
                            h->op_cleanup(child_val.e->key, NULL);
                        }
                        qt_free(child_val.e);                         // XXX should be into a mempool
                        // Second, walk back up the parent pointers, removing empty spines (if any)
                        // cur_id is the current spine pointer's location (if its null, we're in the base spine)
                        while (cur_id) {
                            spine_element_t oldval = *cur_id;
                            spine_element_t newcount;
                            spine_element_t tmp = oldval;
                            uint32_t        count;

                            do {
                                oldval = tmp;
                                count  = SPINE_PTR_COUNT(oldval) - 1;
                                if (count == 0) {
                                    newcount.e = NULL;
                                } else {
                                    newcount.s.id  = oldval.s.id;
                                    newcount.s.ctr = SPINE_COUNT(count);
                                }
                                tmp.u = CAS(&(cur_id->u), oldval.u, newcount.u);
                            } while (tmp.u != oldval.u);
                            if (count == 0) {
                                spine_t *s = SPINE_PTR(h, oldval);
                                assert(s);
                                cur_id = s->parent;
                                deallocate_spine(h, oldval.s.id);
                            } else {
                                break;
                            }
                        }
                        INCR(&(h->count), -1);
                        return 1;
                    } else {}
                }
                prev = &(e->next);
                e    = e->next;
            } while(e != NULL);
            return 0;
        } else {
            return 0;
        }
    } while (1);
}

void *qt_dictionary_delete(qt_dictionary *dict,
                           void          *key)
{
    void *val = qt_dictionary_get(dict, key);     // TODO : this is inefficient!
    int   ret = qt_dictionary_remove(dict, key);

    if(ret) { return val; } else { return NULL; }
}

void *qt_dictionary_get(qt_dictionary *h,
                        const qt_key_t key)
{
    uint64_t lkey = (uint64_t)(uintptr_t)(h->op_hash(key));

    HASH_KEY(lkey);
    assert(h);

    unsigned bucket = BASE_SPINE_BUCKET(lkey);
    // spine_element_t *child_id  = &(h->base[bucket]);
    spine_element_t child_val = h->base[bucket];
    // spine_element_t *cur_id    = NULL;
    spine_t *cur_spine = NULL;
    unsigned depth     = 0;

    do {
        if (child_val.e == NULL) {
            // not found
            return NULL;
        } else if (SPINE_PTR_TEST(child_val)) {
            // cur_id    = child_id;
            cur_spine = SPINE_PTR(h, child_val);
            depth++;
            assert(depth <= 11); // otherwise, something has gone horribly wrong
            bucket = SPINE_BUCKET(lkey, depth);
            // child_id  = &cur_spine->elements[bucket];
            child_val = cur_spine->elements[bucket];
        } else {
            // check that key == element->hashed_key
            if (child_val.e->hashed_key == lkey) {
                hash_entry *e = child_val.e;
                while(e) {
                    if (h->op_equals(e->key, key)) {
                        return e->value;
                    }
                    e = e->next;
                }
                return NULL;
            } else {
                return NULL;
            }
        }
    } while (1);
}

size_t qt_dictionary_count(qt_dictionary *h);

size_t qt_dictionary_count(qt_dictionary *h)
{
    assert(h);
    return h->count;
}

/*
 * static void spine_callback(qt_dictionary*             h,
 *                         spine_t            *spine,
 *                         qt_hash_callback_fn f,
 *                         void               *arg,
 *                         unsigned            depth)
 * {
 *  assert(spine);
 *  for (size_t i = 0; i < SPINE_LENGTH; ++i) {
 *      if (spine->elements[i].e != NULL) {
 *          if (SPINE_PTR_TEST(spine->elements[i])) {
 *              printf("subspine... (%li)\n", (intptr_t)arg);
 *              spine_callback(h,
 *                             SPINE_PTR(h, spine->elements[i]),
 *                             f,
 * #ifdef EBUG
 *                             (void *)(intptr_t)SPINE_PTR_COUNT(spine->elements[i]),
 * #else
 *                             arg,
 * #endif
 *                             depth + 1);
 *          } else {
 *              DEBUG(for (size_t j = 0; j < depth; ++j) printf("%li ", (intptr_t)arg); );
 *              f(spine->elements[i].e->hashed_key, spine->elements[i].e->value, arg);
 *          }
 *      }
 *  }
 * }
 * */
/*
 * void qt_hash_callback(qt_hash             h,
 *                    qt_hash_callback_fn f,
 *                    void               *arg)
 * {
 *  assert(h);
 *  assert(f);
 *
 *  for (size_t base_bucket = 0; base_bucket < BASE_SPINE_LENGTH; ++base_bucket) {
 *      if (h->base[base_bucket].e != NULL) {
 *          if (SPINE_PTR_TEST(h->base[base_bucket])) { // spine
 *              spine_callback(h,
 *                             SPINE_PTR(h, h->base[base_bucket]),
 *                             f,
 * #ifdef EBUG
 *                             (void *)(intptr_t)SPINE_PTR_COUNT(h->base[base_bucket]),
 * #else
 *                             arg,
 * #endif
 *                             1);
 *          } else { // element
 *              f(h->base[base_bucket].e->hashed_key, h->base[base_bucket].e->value, arg);
 *          }
 *      }
 *  }
 *  DEBUG(printf("============\n"));
 * }
 */

struct qt_dictionary_iterator {
    qt_dictionary *dict;
    list_entry    *crt; // =NULL if iterator is newly created or reached the end; =crt elem otherwise.
    int            bkt; // = -1 if iterator is newly created; =1 otherwise.
    int            spine_index;
    int            base_index;
};

qt_dictionary_iterator *qt_dictionary_iterator_create(qt_dictionary *dict)
{
    if(dict == NULL) {
        return ERROR;
    }
    qt_dictionary_iterator *it = (qt_dictionary_iterator *)MALLOC(sizeof(qt_dictionary_iterator));
    it->dict        = dict;
    it->crt         = NULL;
    it->bkt         = -1;
    it->spine_index = 0;
    it->base_index  = -1;
    return it;
}

void qt_dictionary_iterator_destroy(qt_dictionary_iterator *it)
{
    if(it == NULL) { return; }
    FREE(it, sizeof(qt_dictionary_iterator));
}

list_entry *qt_dictionary_iterator_next(qt_dictionary_iterator *it)
{
    if((it == NULL) || (it->dict == NULL)) {
        return ERROR;
    }

    if((it->crt != NULL) && (it->crt->next != NULL)) {
        it->crt = it->crt->next;
        return it->crt;
    }
    it->base_index++;
    int i;
    if(it->base_index < BASE_SPINE_LENGTH) {
        for(i = it->base_index; i < BASE_SPINE_LENGTH; i++)
            if((it->dict->base[i].e != NULL) && !SPINE_PTR_TEST(it->dict->base[i])) {
                it->base_index = i;
                it->crt        = it->dict->base[i].e;
                return it->crt;
            } else   {}
    }

    // first call
    if (it->bkt < 0) {
        it->bkt = 0;
    }

    while (it->bkt < it->dict->maxspines) {
        spine_t *local_e = it->dict->spines[it->bkt];
        if(local_e != NULL) {
            for(i = it->spine_index; i < SPINE_LENGTH; i++)
                if((local_e->elements[i].e != NULL) && !SPINE_PTR_TEST(local_e->elements[i])) {
                    it->spine_index = i + 1;
                    it->crt         = local_e->elements[i].e;
                    return it->crt;
                } else if( local_e->elements[i].e != NULL) {}
            it->spine_index = 0;
        }

        it->bkt++;
    }
    it->crt = NULL;
    return NULL;
}

list_entry *qt_dictionary_iterator_get(const qt_dictionary_iterator *it)
{
    if((it == NULL) || (it->dict == NULL)) {
        return ERROR;
    }
    return it->crt;
}

qt_dictionary_iterator *qt_dictionary_end(qt_dictionary *dict)
{
    qt_dictionary_iterator *ret = qt_dictionary_iterator_create(dict);

    ret->bkt = ret->dict->maxspines;
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
    ret->bkt         = b->bkt;
    ret->crt         = b->crt;
    ret->spine_index = b->spine_index;
    ret->base_index  = b->base_index;
    return ret;
}

void qt_dictionary_printbuckets(qt_dictionary *dict) {}

/* vim:set expandtab: */
