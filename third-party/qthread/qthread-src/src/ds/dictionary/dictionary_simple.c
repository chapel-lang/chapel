#include <qthread/dictionary.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>          // using CHAR_BIT
#include <qthread/qthread.h> // using CAS_ptr, qthread_worker_unique and qthread_num_workers
#include <56reader-rwlock.h> // using rwlock_*

#include "qt_asserts.h"
#include "qt_debug.h"

struct qt_dictionary {
    qt_dict_key_equals_f op_equals;
    qt_dict_hash_f       op_hash;
    qt_dict_cleanup_f    op_cleanup;
    list_entry         **content;
#ifdef DELETE_SUPPORT
    struct tlrw_lock    *lock;
#endif
};

struct qt_dictionary_iterator {
    qt_dictionary *dict;
    list_entry    *crt;
    int            bkt;
};

/* Prototype should NOT go in header, we don't want it public*/
void *qt_dictionary_put_helper(qt_dictionary *dict,
                               void          *key,
                               void          *value,
                               char           put_type);

#ifdef DELETE_SUPPORT

// Handle the case when variable does not exist;
// Locks become nops when qthread is no longer initialized
# ifdef QTHREAD_NO_ASSERTS
static int qthread_library_initialized = 1;
# endif

# define rlock(a)                                                                       \
    {                                                                                   \
        if(qthread_library_initialized == 1) {                                          \
            int id = qthread_worker_unique(NULL);                                       \
            /* this if protects against crash when using after qthreads was finalized*/ \
            /* if qthreads is finalized, use sequential semantics */                    \
            if (id < MAX_READERS) {                                                     \
                rwlock_rdlock(a, id); }                                                 \
        }                                                                               \
    }

# define runlock(a)                               \
    {                                             \
        if(qthread_library_initialized == 1) {    \
            int id = qthread_worker_unique(NULL); \
            if (id < MAX_READERS) {               \
                rwlock_rdunlock(a, id); }         \
        }                                         \
    }
# define wlock(a)                                 \
    {                                             \
        if(qthread_library_initialized == 1) {    \
            int id = qthread_worker_unique(NULL); \
            if (id < MAX_READERS) {               \
                rwlock_wrlock(a, id); }           \
        }                                         \
    }

# define wunlock(a)                               \
    {                                             \
        if(qthread_library_initialized == 1) {    \
            int id = qthread_worker_unique(NULL); \
            if (id < MAX_READERS) {               \
                rwlock_wrunlock(a); }             \
        }                                         \
    }
# define rwlinit(a)                                                                  \
    {                                                                                \
        /* fail instead of silently accepting more readers than we can handle*/      \
        /* setting the number of workers dynamically to be larger than MAX_READERS*/ \
        /* is not checked, so don't do it! */                                        \
        assert(qthread_num_workers() < MAX_READERS);                                 \
        a = (rwlock_t *)MALLOC(sizeof(rwlock_t));                                    \
        rwlock_init(a);                                                              \
    }
# define rwlfree(a)                \
    {                              \
        FREE(a, sizeof(rwlock_t)); \
    }
#else /* ifdef DELETE_SUPPORT */
# define rlock(a)
# define runlock(a)
# define wlock(a)
# define wunlock(a)
# define rwlinit(a)
# define rwlfree(a)
#endif /* ifdef DELETE_SUPPORT */

// #define DICT_ABS(v) ((+1 | (v>>(sizeof(int)*CHAR_BIT-1)))*v)
#define DICT_ABS(v) ((v > 0) ? v : (-v))

#define GET_BUCKET(hash) ((DICT_ABS(hash)) & ((1 << BKT_POW) - 1))

#ifndef QTHREAD_NO_ASSERTS
extern int qthread_library_initialized;
#endif

qt_dictionary *qt_dictionary_create(qt_dict_key_equals_f eq,
                                    qt_dict_hash_f       hash,
                                    qt_dict_cleanup_f    cleanup)
{
    assert(qthread_library_initialized && "Need to initialize qthreads before using the dictionary");
    qt_dictionary *ret = (qt_dictionary *)MALLOC(sizeof(qt_dictionary));
    ret->op_equals  = eq;
    ret->op_hash    = hash;
    ret->op_cleanup = cleanup;
    ret->content    = (list_entry **)MALLOC(NO_BUCKETS * sizeof(list_entry *));

    int i;
    for (i = 0; i < NO_BUCKETS; i++) {
        ret->content[i] = NULL;
    }

    rwlinit(ret->lock);
    return ret;
}

void qt_dictionary_destroy(qt_dictionary *d)
{
    int i;

    for (i = 0; i < NO_BUCKETS; i++) {
        list_entry *tmp, *top = d->content[i];
        while (top != NULL) {
            tmp = top;
            top = top->next;
            if (d->op_cleanup) {
                d->op_cleanup(tmp->key, tmp->value);
            }
            FREE(tmp, sizeof(list_entry));
        }
    }
    FREE(d->content, NO_BUCKETS * sizeof(list_entry *));
    rwlfree(d->lock);
    FREE(d, sizeof(qt_dictionary));
}

#define PUT_ALWAYS    0
#define PUT_IF_ABSENT 1

#define DICTIONARY_ADD_TO_HEAD

#ifdef DICTIONARY_ADD_TO_HEAD
void *qt_dictionary_put_helper(qt_dictionary *dict,
                               void          *key,
                               void          *value,
                               char           put_type)
{
    int hash = dict->op_hash(key);

    int bucket = GET_BUCKET(hash);

    rlock(dict->lock);
    list_entry **const crt = &(dict->content[bucket]);
    assert(!(crt == NULL || dict->content == NULL));
    list_entry *walk = *crt, *toadd = NULL;
    list_entry *head = walk, *stop = NULL;
    while(1) {
        while(walk != stop && walk != NULL) {
            if((walk->hashed_key == hash) && (dict->op_equals(walk->key, key))) {
                if(toadd != NULL) { FREE(toadd, sizeof(list_entry)); }

                if(put_type == PUT_ALWAYS) {
                    void **crt_val_adr = &(walk->value);
                    void  *crt_val     = walk->value;
                    while((qthread_cas_ptr(crt_val_adr, \
                                           crt_val, value)) != crt_val ) {
                        // try until succeeding to add
                        crt_val = walk->value;
                    }
                }
                runlock(dict->lock);
                return walk->value;
            }
            walk = walk->next;
        }
        // if new entry not found and not created, create it.
        if(toadd == NULL) {
            toadd = (list_entry *)MALLOC(sizeof(list_entry));
            if(toadd == NULL) {
                runlock(dict->lock);
                return NULL;
            }
            toadd->key        = key;
            toadd->value      = value;
            toadd->next       = head;
            toadd->hashed_key = hash;
        }
        void *code = qthread_cas_ptr(crt, head, toadd);
        if(code == head) {        // succeeded adding
            runlock(dict->lock);
            return value;
        }
        stop        = head;
        head        = *crt;
        walk        = head;
        toadd->next = head;
    }

    runlock(dict->lock);
    return NULL;
}

#else /* ifdef DICTIONARY_ADD_TO_HEAD */
void *qt_dictionary_put_helper(qt_dictionary *dict,
                               void          *key,
                               void          *value,
                               char           put_type)
{
    int hash = dict->op_hash(key);

    int bucket = GET_BUCKET(hash);

    rlock(dict->lock);
    list_entry **crt = &(dict->content[bucket]);
    assert(!(crt == NULL || dict->content == NULL));
    list_entry *walk = *crt, *toadd = NULL;
    while(1) {
        while(walk != NULL) {
            if((walk->hashed_key == hash) && (dict->op_equals(walk->key, key))) {
                if(toadd != NULL) { FREE(toadd, sizeof(list_entry)); }

                if(put_type == PUT_ALWAYS) {
                    void **crt_val_adr = &(walk->value);
                    void  *crt_val     = walk->value;
                    while((qthread_cas_ptr(crt_val_adr, \
                                           crt_val, value)) != crt_val ) {
                        crt_val = walk->value;
                    }
                }
                runlock(dict->lock);
                return walk->value;
            }
            crt  = &(walk->next);
            walk = walk->next;
        }
        if(toadd == NULL) {
            toadd = (list_entry *)MALLOC(sizeof(list_entry));
            if(toadd == NULL) {
                runlock(dict->lock);
                return NULL;
            }
            toadd->key        = key;
            toadd->value      = value;
            toadd->next       = NULL;
            toadd->hashed_key = hash;
        }
        void *code = qthread_cas_ptr(crt, NULL, toadd);
        if(code == NULL) {
            runlock(dict->lock);
            return value;
        }
        walk = *crt;
    }

    runlock(dict->lock);
    return NULL;
}
#endif /* ifdef DICTIONARY_ADD_TO_HEAD */

void *qt_dictionary_put(qt_dictionary *dict,
                        void          *key,
                        void          *value)
{
    return qt_dictionary_put_helper(dict, key, value, PUT_ALWAYS);
}

void *qt_dictionary_put_if_absent(qt_dictionary *dict,
                                  void          *key,
                                  void          *value)
{
    return qt_dictionary_put_helper(dict, key, value, PUT_IF_ABSENT);
}

void *qt_dictionary_get(qt_dictionary *dict,
                        void          *key)
{
    int hash   = dict->op_hash(key);
    int bucket = GET_BUCKET(hash);

    rlock(dict->lock);

    list_entry *walk = dict->content[bucket];
    while(walk != NULL) {
        if ((walk->hashed_key == hash) && (dict->op_equals(walk->key, key))) {
            runlock(dict->lock);
            return walk->value;
        }
        walk = walk->next;
    }

    runlock(dict->lock);
    return NULL;
}

void *qt_dictionary_delete(qt_dictionary *dict,
                           void          *key)
{
    void       *to_ret  = NULL;
    list_entry *to_free = NULL;
    int         hash    = dict->op_hash(key);
    int         bucket  = GET_BUCKET(hash);

    wlock(dict->lock);

    list_entry *walk = dict->content[bucket];
    list_entry *head = walk;
    /*
     * if(walk == NULL) assert(0);//cannot remove an element not present in hash
     * else if((walk -> hashed_key == hash) && (dict -> op_equals(walk -> key, key))) {
     *      //remove list head
     *      to_free = walk;
     *      to_ret = walk -> value;
     *      dict -> content[bucket] = walk -> next;
     *      if (dict->op_cleanup != NULL)
     *              dict->op_cleanup(to_free->key);
     *      free(to_free);
     * }
     * else while(walk -> next != NULL){
     *      if ((walk -> hashed_key == hash) && (dict -> op_equals(walk -> next -> key, key))) {
     *              to_free = walk -> next;
     *              to_ret = walk -> next -> value;
     *              walk -> next = walk -> next -> next;
     *              if (dict->op_cleanup != NULL)
     *                      dict->op_cleanup(to_free->key);
     *              free(to_free);
     *              break;
     *      }
     *      walk = walk -> next;
     * }
     *
     */
    list_entry *prev = NULL;
    while(walk != NULL) {
        if ((walk->hashed_key == hash) && (dict->op_equals(walk->key, key))) {
            if (walk == head) {
                dict->content[bucket] = walk->next;
            } else {
                prev->next = walk->next;
            }
            to_free = walk;
            to_ret  = walk->value;
            if (dict->op_cleanup != NULL) {
                dict->op_cleanup(to_free->key, NULL);
            }
            FREE(to_free, sizeof(list_entry));

            break;
        }
        prev = walk;
        walk = walk->next;
    }

    wunlock(dict->lock);
    return to_ret;
}

qt_dictionary_iterator *qt_dictionary_iterator_create(qt_dictionary *dict)
{
    if((dict == NULL) || (dict->content == NULL)) {
        return ERROR;
    }
    qt_dictionary_iterator *it = (qt_dictionary_iterator *)MALLOC(sizeof(qt_dictionary_iterator));
    if(it == NULL) {
        return ERROR;         // out of memory
    }
    it->dict = dict;
    it->bkt  = -1;
    it->crt  = NULL;
    return it;
}

void qt_dictionary_iterator_destroy(qt_dictionary_iterator *it)
{
    if(it == NULL) { return; }
    FREE(it, sizeof(qt_dictionary_iterator));
}

list_entry *qt_dictionary_iterator_next(qt_dictionary_iterator *it)
{
    if((it == NULL) || (it->dict == NULL) || (it->dict->content == NULL)) {
        return ERROR;
    }
    // First call to next: search for the first non-empty bucket
    if(it->bkt == -1) {
        int i;
        for(i = 0; i < NO_BUCKETS; i++)
            if(it->dict->content[i] != NULL) {
                it->bkt = i;
                it->crt = it->dict->content[i];
                return it->crt;
            }
    } else {
        // start searching for next element starting with the last returned element
        list_entry *walk = it->crt;
        // if item was deleted or there are no more elements in the list, return NULL
        if(walk == NULL) {
            it->bkt = -1;
            return ERROR;
        }

        walk = walk->next;

        // search all buckets in dictionary
        while (it->bkt < NO_BUCKETS) {
            if(walk != NULL) {
                it->crt = walk;
                return it->crt;
            }
            it->bkt++;
            if(it->bkt < NO_BUCKETS) {
                walk = it->dict->content[it->bkt];
            }
        }
        // if dictionary has no more elements return NULL
        it->crt = NULL;
        return NULL;
    }
    assert(0);
    return NULL;
}

list_entry *qt_dictionary_iterator_get(const qt_dictionary_iterator *it)
{
    if((it == NULL) || (it->dict == NULL) || (it->dict->content == NULL)) {
        printf(" Inside dictionary get, found NULL, will return ERROR\n");
        return ERROR;
    }

    return it->crt;
}

qt_dictionary_iterator *qt_dictionary_end(qt_dictionary *dict)
{
    if((dict == NULL) || (dict->content == NULL)) {
        return NULL;
    }
    qt_dictionary_iterator *it = qt_dictionary_iterator_create(dict);
    it->crt  = NULL;
    it->bkt  = NO_BUCKETS;
    it->dict = dict;
    return it;
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
    int total        = 0;
    int used_buckets = 0;

    for(int bucket = 0; bucket < NO_BUCKETS; bucket++) {
        int          no_el = 0;
        list_entry **crt   = &(dict->content[bucket]);
        if ((crt != NULL) && (dict->content != NULL)) {
            list_entry *walk = *crt;
            while(walk != NULL) {
                no_el++;
                crt  = &(walk->next);
                walk = walk->next;
            }
            if (no_el > 0) {
                printf("Bucket %d has %d elements.\n", bucket, no_el);
                used_buckets++;
            }
            total += no_el;

            // assert(no_el < 2);
        }
    }
    printf("used_buckets = %d; total elements = %d;\n", used_buckets, total);
}

/* vim:set expandtab: */
