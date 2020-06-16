/*
 * Copyright (c) 2015 Cray Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef GNIX_HASHTABLE_H_
#define GNIX_HASHTABLE_H_

#include <stdint.h>
#include <pthread.h>

#include <ofi.h>
#include <ofi_list.h>

#include "gnix_util.h"

typedef uint64_t gnix_ht_key_t;

typedef enum gnix_ht_state {
	GNIX_HT_STATE_UNINITIALIZED = 0,
	GNIX_HT_STATE_READY,
	GNIX_HT_STATE_DEAD,
} gnix_ht_state_e;

typedef struct gnix_ht_entry {
	struct dlist_entry entry;
	gnix_ht_key_t key;
	void *value;
} gnix_ht_entry_t;

typedef struct gnix_ht_lk_lh {
	rwlock_t lh_lock;
	struct dlist_entry head;
} gnix_ht_lk_lh_t;

typedef struct gnix_ht_lf_lh {
	struct dlist_entry head;
} gnix_ht_lf_lh_t;

enum gnix_ht_increase {
	GNIX_HT_INCREASE_ADD = 0,
	GNIX_HT_INCREASE_MULT
};

/**
 * Set of attributes that can be passed to the gnix_ht_init.
 *
 * @var ht_initial_size      initial number of buckets allocated
 * @var ht_maximum_size      maximum number of buckets to allocate on resize
 * @var ht_increase_step     additive or multiplicative factor to increase by.
 *                           If additive, the new_size = (old_size + increase)
 *                           If multiplicative, the new size = (old_size *
 *                           increase)
 * @var ht_increase_type     based on the gnix_ht_increase enum, this
 *                           influences whether the increase of the bucket
 *                           count is additive or multiplicative
 * @var ht_collision_thresh  threshold for resizing based on insertion
 *                           collisions. The threshold is based on the
 *                           average number of collisions per insertion,
 *                           multiplied by 100. If you want an average bucket
 *                           depth of 4, you would want to see 3-4 collisions
 *                           on average, so the appropriate threshold would be
 *                           ~400.
 * @var ht_hash_seed		 seed value that affects how items are hashed
 *                           internally. Using the same seed value and the same
 *                           insertion pattern will allow for repeatable
 *                           results.
 * @var ht_internal_locking  if non-zero, uses a version of the hash table with
 *                           internal locking implemented
 *
 * @var destructor           if non-NULL, will be called with value when
 *                           destroying the hash table
 */
typedef struct gnix_hashtable_attr {
	int ht_initial_size;
	int ht_maximum_size;
	int ht_increase_step;
	int ht_increase_type;
	int ht_collision_thresh;
	uint64_t ht_hash_seed;
	int ht_internal_locking;
	void  (*destructor)(void *);
} gnix_hashtable_attr_t;

struct gnix_hashtable;
struct gnix_hashtable_iter;

typedef struct gnix_hashtable_ops {
	int   (*init)(struct gnix_hashtable *);
	int   (*destroy)(struct gnix_hashtable *);
	int   (*insert)(struct gnix_hashtable *, gnix_ht_entry_t *, uint64_t *);
	int   (*remove)(struct gnix_hashtable *, gnix_ht_key_t);
	void *(*lookup)(struct gnix_hashtable *, gnix_ht_key_t);
	int   (*resize)(struct gnix_hashtable *, int, int);
	struct dlist_entry *(*retrieve_list)(struct gnix_hashtable *, int bucket);
	void *(*iter_next)(struct gnix_hashtable_iter *);
} gnix_hashtable_ops_t;

/**
 * Hashtable structure
 *
 * @var ht_lock        reader/writer lock for protecting internal structures
 *                     during a resize
 * @var ht_state       internal state mechanism for detecting valid state
 *                     transitions
 * @var ht_attr        attributes for the hash map to follow after init
 * @var ht_ops         function table for internal hash map calls
 * @var ht_elements    number of items in the hash map
 * @var ht_collisions  number of insertion collisions since the last resize
 * @var ht_insertions  number of insertions since the last resize
 * @var ht_size        number of hash buckets
 * @var ht_tbl         array of hash buckets
 */
typedef struct gnix_hashtable {
	pthread_rwlock_t ht_lock;
	gnix_ht_state_e ht_state;
	gnix_hashtable_attr_t ht_attr;
	gnix_hashtable_ops_t *ht_ops;
	ofi_atomic32_t ht_elements;
	ofi_atomic32_t ht_collisions;
	ofi_atomic32_t ht_insertions;
	int ht_size;
	union {
		gnix_ht_lf_lh_t *ht_lf_tbl;
		gnix_ht_lk_lh_t *ht_lk_tbl;
	};
} gnix_hashtable_t;

struct gnix_hashtable_iter {
	struct gnix_hashtable *ht;
	int cur_idx;
	gnix_ht_entry_t *cur_entry;
};

#define GNIX_HASHTABLE_ITERATOR(_ht, _iter)	\
	struct gnix_hashtable_iter _iter = {	\
		.ht = (_ht),			\
		.cur_idx = 0,			\
		.cur_entry = NULL		\
	}
#define GNIX_HASHTABLE_ITERATOR_KEY(_iter)	((_iter).cur_entry->key)

/**
 * Initializes the hash table with provided attributes, if any
 *
 * @param ht      pointer to the hash table structure
 * @param attr    pointer to the hash table attributes to initialize with
 * @return        0 on success, -FI_EINVAL on initialization error, or
 *                -FI_ENOMEM if allocation of the bucket array fails
 */
int _gnix_ht_init(gnix_hashtable_t *ht, gnix_hashtable_attr_t *attr);

/**
 * Destroys the hash table
 *
 * @param ht      pointer to the hash table structure
 * @return        0 on success, -FI_EINVAL upon passing an uninitialized
 *                or dead structure
 */
int _gnix_ht_destroy(gnix_hashtable_t *ht);

/**
 * Inserts an entry into the map with the provided key
 *
 * @param ht      pointer to the hash table structure
 * @param key     key used to hash the entry
 * @param entry   entry to be stored
 * @return        0 on success, -FI_ENOSPC when another entry with the same key
 *                exists in the hashtable, or -FI_EINVAL when called on a
 *                dead or uninitialized hash table
 */
int _gnix_ht_insert(gnix_hashtable_t *ht, gnix_ht_key_t key, void *entry);

/**
 * Removes an entry from the map with the provided key
 *
 * @param ht      pointer to the hash table structure
 * @param key     key used to hash the entry
 * @return        0 on success, -FI_ENOENT when the key doesn't exist in
 *                the hash table, or -FI_EINVAL when called on a dead or
 *                uninitialized hash table
 */
int _gnix_ht_remove(gnix_hashtable_t *ht, gnix_ht_key_t key);

/**
 * Looks up an entry in the hash table using key
 *
 * @param ht      pointer to the hash table structure
 * @param key     key used to hash the entry
 * @return        NULL if the key did not exist in the hash table, or the
 *                entry if the key exists in the hash table
 */
void *_gnix_ht_lookup(gnix_hashtable_t *ht, gnix_ht_key_t key);

/**
 * Tests to see if the hash table is empty
 *
 * @param ht      pointer to the hash table structure
 * @return        true if the hash table is empty, false if not
 */
int _gnix_ht_empty(gnix_hashtable_t *ht);

/**
 * Return next element in the hashtable
 *
 * @param iter    pointer to the hashtable iterator
 * @return        pointer to next element in the hashtable
 */
void *_gnix_ht_iterator_next(struct gnix_hashtable_iter *iter);

/* Hastable iteration macros */
#define ht_lf_for_each(ht, ht_entry)				\
	dlist_for_each(ht->ht_lf_tbl->head, ht_entry, entry)	\

#define ht_lk_for_each(ht, ht_entry)				\
	dlist_for_each(ht.ht_lk_tbl->head, ht_entry, entry)

#define ht_entry_value(ht_entry)		\
	ht_entry->value

#endif /* GNIX_HASHTABLE_H_ */
