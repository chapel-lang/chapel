/*
 * Copyright (c) 2016-2017 Cray Inc. All rights reserved.
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

#include <gnix_mr_cache.h>
#include <gnix_smrn.h>
#include <gnix_mr_notifier.h>
#include <gnix.h>

typedef unsigned long long int cache_entry_state_t;
/* These are used for entry state and should be unique */
#define GNIX_CES_INUSE       (1ULL << 8)    /* in use */
#define GNIX_CES_STALE       (2ULL << 8)    /* cached for possible reuse */
#define GNIX_CES_STATE_MASK  (0xFULL << 8)

typedef unsigned long long int cache_entry_flag_t;
/* One or more of these can be combined with the above */
#define GNIX_CE_RETIRED     (1ULL << 61)   /* in use, but not to be reused */
#define GNIX_CE_MERGED      (1ULL << 62)   /* merged entry, i.e., not
					    * an original request from
					    * fi_mr_reg */
#define GNIX_CE_UNMAPPED    (1ULL << 63)   /* at least 1 page of the
					    * entry has been unmapped
					    * by the OS */

/**
 * @brief structure for containing the fields relevant to the memory cache key
 *
 * @var   address  base address of the memory region
 * @var   address  length of the memory region
 */
typedef struct gnix_mr_cache_key {
	uint64_t address;
	uint64_t length;
} gnix_mr_cache_key_t;

/**
 * @brief gnix memory registration cache entry
 *
 * @var   state      state of the memory registration cache entry
 * @var   mr         gnix memory registration descriptor
 * @var   mem_hndl   gni memory handle for the memory registration
 * @var   key        gnix memory registration cache key
 * @var   domain     gnix domain associated with the memory registration
 * @var   nic        gnix nic associated with the memory registration
 * @var   ref_cnt    reference counting for the cache
 * @var   lru_entry  lru list entry
 * @var   siblings   list of sibling entries
 * @var   children   list of subsumed child entries
 */
typedef struct gnix_mr_cache_entry {
	struct gnix_smrn_context context;
	cache_entry_state_t state;
	gnix_mr_cache_key_t key;
	ofi_atomic32_t ref_cnt;
	struct dlist_entry lru_entry;
	struct dlist_entry siblings;
	struct dlist_entry children;
	uint64_t mr[0];
} gnix_mr_cache_entry_t;

/* forward declarations */
int _gnix_mr_cache_register(
		gnix_mr_cache_t             *cache,
		uint64_t                    address,
		uint64_t                    length,
		struct _gnix_fi_reg_context *fi_reg_context,
		void                        **handle);

int _gnix_mr_cache_deregister(
		gnix_mr_cache_t *cache,
		void            *handle);

static inline int __mr_cache_entry_put(
		gnix_mr_cache_t       *cache,
		gnix_mr_cache_entry_t *entry);

static inline int __mr_cache_entry_get(
		gnix_mr_cache_t       *cache,
		gnix_mr_cache_entry_t *entry);

static inline int __mr_cache_entry_destroy(gnix_mr_cache_t *cache,
					   gnix_mr_cache_entry_t *entry);

static int __mr_cache_create_registration(
		gnix_mr_cache_t             *cache,
		uint64_t                    address,
		uint64_t                    length,
		gnix_mr_cache_entry_t       **entry,
		gnix_mr_cache_key_t         *key,
		struct _gnix_fi_reg_context *fi_reg_context);


/* global declarations */

/* default attributes for new caches */
gnix_mr_cache_attr_t __default_mr_cache_attr = {
		.soft_reg_limit      = 4096,
		.hard_reg_limit      = -1,
		.hard_stale_limit    = 128,
#if HAVE_KDREG
		.lazy_deregistration = 1,
#else
		.lazy_deregistration = 0,
#endif
};

/* Functions for using and manipulating cache entry state */
static inline cache_entry_state_t __entry_get_state(gnix_mr_cache_entry_t *e)
{
	return e->state & GNIX_CES_STATE_MASK;
}

static inline void __entry_set_state(gnix_mr_cache_entry_t *e,
				     cache_entry_state_t s)
{
	e->state = (e->state & ~GNIX_CES_STATE_MASK) |
		(s & GNIX_CES_STATE_MASK);
}

static inline void __entry_reset_state(gnix_mr_cache_entry_t *e)
{
	e->state = 0ULL;
}

static inline bool __entry_is_flag(gnix_mr_cache_entry_t *e,
				   cache_entry_flag_t f)
{
	return (e->state & f) != 0;
}

static inline void __entry_set_flag(gnix_mr_cache_entry_t *e,
				    cache_entry_flag_t f)
{
	e->state = e->state | f;
}

static inline bool __entry_is_retired(gnix_mr_cache_entry_t *e)
{
	return __entry_is_flag(e, GNIX_CE_RETIRED);
}

static inline bool __entry_is_merged(gnix_mr_cache_entry_t *e)
{
	return __entry_is_flag(e, GNIX_CE_MERGED);
}

static inline bool __entry_is_unmapped(gnix_mr_cache_entry_t *e)
{
	return __entry_is_flag(e, GNIX_CE_UNMAPPED);
}

static inline void __entry_set_retired(gnix_mr_cache_entry_t *e)
{
	__entry_set_flag(e, GNIX_CE_RETIRED);
}

static inline void __entry_set_merged(gnix_mr_cache_entry_t *e)
{
	__entry_set_flag(e, GNIX_CE_MERGED);
}

static inline void __entry_set_unmapped(gnix_mr_cache_entry_t *e)
{
	__entry_set_flag(e, GNIX_CE_UNMAPPED);
}

/**
 * Key comparison function for finding overlapping gnix memory
 * registration cache entries
 *
 * @param[in] x key to be inserted or found
 * @param[in] y key to be compared against
 *
 * @return    -1 if it should be positioned at the left, 0 if it overlaps,
 *             1 otherwise
 */
static int __find_overlapping_addr(
		void *x,
		void *y)
{
	gnix_mr_cache_key_t *to_find  = (gnix_mr_cache_key_t *) x;
	gnix_mr_cache_key_t *to_compare = (gnix_mr_cache_key_t *) y;
	uint64_t to_find_end = to_find->address + to_find->length - 1;
	uint64_t to_compare_end = to_compare->address + to_compare->length - 1;

	/* format: (x_addr,  x_len) - (y_addr,  y_len) truth_value
	 *
	 * case 1: (0x1000, 0x1000) - (0x1400, 0x0800) true
	 * case 2: (0x1000, 0x1000) - (0x0C00, 0x0800) true
	 * case 3: (0x1000, 0x1000) - (0x1C00, 0x0800) true
	 * case 4: (0x1000, 0x1000) - (0x0C00, 0x2000) true
	 * case 5: (0x1000, 0x1000) - (0x0400, 0x0400) false
	 * case 6: (0x1000, 0x1000) - (0x2400, 0x0400) false
	 */
	if (!(to_find_end < to_compare->address ||
			to_compare_end < to_find->address))
		return 0;

	/* left */
	if (to_find->address < to_compare->address)
		return -1;

	return 1;
}

/**
 * Key comparison function for gnix memory registration caches
 *
 * @param[in] x key to be inserted or found
 * @param[in] y key to be compared against
 *
 * @return    -1 if it should be positioned at the left, 0 if the same,
 *             1 otherwise
 */
static inline int __mr_cache_key_comp(
		void *x,
		void *y)
{
	gnix_mr_cache_key_t *to_insert  = (gnix_mr_cache_key_t *) x;
	gnix_mr_cache_key_t *to_compare = (gnix_mr_cache_key_t *) y;

	if (to_compare->address == to_insert->address)
		return 0;

	/* to the left */
	if (to_insert->address < to_compare->address)
		return -1;

	/* to the right */
	return 1;
}

/**
 * Helper function for matching the exact key entry
 *
 * @param entry     memory registration cache key
 * @param to_match  memory registration cache key
 * @return 1 if match, otherwise 0
 */
static inline int __match_exact_key(
		gnix_mr_cache_key_t *entry,
		gnix_mr_cache_key_t *to_match)
{
	return entry->address == to_match->address &&
			entry->length == to_match->length;
}

/**
 * dlist search function for matching the exact memory registration key
 *
 * @param entry memory registration cache entry
 * @param match memory registration cache key
 * @return 1 if match, otherwise 0
 */
static inline int __mr_exact_key(struct dlist_entry *entry,
		const void *match)
{
	gnix_mr_cache_entry_t *x = container_of(entry,
							gnix_mr_cache_entry_t,
							siblings);

	gnix_mr_cache_key_t *y = (gnix_mr_cache_key_t *) match;

	return __match_exact_key(&x->key, y);
}


/**
 * Helper function to determine if one key subsumes another
 *
 * @param x  gnix_mr_cache_key
 * @param y  gnix_mr_cache_key
 * @return 1 if x subsumes y, 0 otherwise
 */
static inline int __can_subsume(
		gnix_mr_cache_key_t *x,
		gnix_mr_cache_key_t *y)
{
	return (x->address <= y->address) &&
			((x->address + x->length) >=
					(y->address + y->length));
}

static inline void __attach_retired_entries_to_registration(
		gnix_mr_cache_t *cache,
		struct dlist_entry *retired_entries,
		gnix_mr_cache_entry_t *parent)
{
	gnix_mr_cache_entry_t *entry, *tmp;

	dlist_for_each_safe(retired_entries, entry, tmp, siblings) {
		dlist_remove(&entry->siblings);
		dlist_insert_tail(&entry->siblings,
				&parent->children);
		if (!dlist_empty(&entry->children)) {
			/* move the entry's children to the sibling tree
			 * and decrement the reference count */
			dlist_splice_tail(&parent->children,
					&entry->children);
			__mr_cache_entry_put(cache, entry);
		}
	}

	if (!dlist_empty(retired_entries)) {
		GNIX_FATAL(FI_LOG_MR, "retired_entries not empty\n");
	}

	__mr_cache_entry_get(cache, parent);
}

static inline void __remove_sibling_entries_from_tree(
		gnix_mr_cache_t *cache,
		struct dlist_entry *list,
		RbtHandle tree)
{
	RbtStatus rc;
	RbtIterator iter;
	gnix_mr_cache_entry_t *entry;

	dlist_for_each(list, entry, siblings)
	{
		GNIX_DEBUG(FI_LOG_MR,
			   "removing key from tree, key=%llx:%llx\n",
			   entry->key.address, entry->key.length);
		iter = rbtFind(tree, &entry->key);
		if (OFI_UNLIKELY(!iter)) {
			GNIX_FATAL(FI_LOG_MR, "key not found\n");
		}

		rc = rbtErase(tree, iter);
		if (OFI_UNLIKELY(rc != RBT_STATUS_OK)) {
			GNIX_FATAL(FI_LOG_MR,
				   "could not remove entry from tree\n");
		}
	}
}

/**
 * Pushes an entry into the LRU cache. No limits are maintained here as
 *   the hard_stale_limit attr value will directly limit the lru size
 *
 * @param[in] cache  a memory registration cache object
 * @param[in] entry  a memory registration cache entry
 *
 * @return           FI_SUCCESS, always
 */
static inline int __mr_cache_lru_enqueue(
		gnix_mr_cache_t       *cache,
		gnix_mr_cache_entry_t *entry)
{
	dlist_insert_tail(&entry->lru_entry, &cache->lru_head);

	return FI_SUCCESS;
}

/**
 * Pops an registration cache entry from the lru cache.
 *
 * @param[in] cache  a memory registration cache
 * @param[in] entry  a memory registration cache entry
 *
 * @return           FI_SUCCESS, on success
 * @return           -FI_ENOENT, on empty LRU
 */
static inline int __mr_cache_lru_dequeue(
		gnix_mr_cache_t       *cache,
		gnix_mr_cache_entry_t **entry)
{
	gnix_mr_cache_entry_t *ret;

	ret = dlist_first_entry(&cache->lru_head,
			gnix_mr_cache_entry_t, lru_entry);
	if (OFI_UNLIKELY(!ret)) { /* we check list_empty before calling */
		*entry = NULL;
		return -FI_ENOENT;
	}

	/* remove entry from the list */
	*entry = ret;
	dlist_remove(&ret->lru_entry);

	return FI_SUCCESS;
}

/**
 * Removes a particular registration cache entry from the lru cache.
 *
 * @param[in] cache  a memory registration cache
 * @param[in] entry  a memory registration cache entry
 *
 * @return           FI_SUCCESS, on success
 * @return           -FI_ENOENT, on empty LRU
 */
static inline int __mr_cache_lru_remove(
		gnix_mr_cache_t       *cache,
		gnix_mr_cache_entry_t *entry)
{
	/* Could do some error checking to see if in cache */

	dlist_remove(&entry->lru_entry);

	return FI_SUCCESS;
}

/**
 * Remove entries that have been unmapped as indicated by the notifer
 *
 * @param[in] cache  a memory registration cache
 *
 * @return           nothing
 */
static bool __notifier_warned = false;
static void
__clear_notifier_events(gnix_mr_cache_t *cache)
{
	int ret;
	gnix_mr_cache_entry_t *entry;
	struct gnix_smrn_context *context;
	RbtIterator iter;

	if (!cache->attr.smrn) {
		return;
	}

	if (!cache->attr.lazy_deregistration) {
		return;
	}

	ret = _gnix_smrn_get_event(cache->attr.smrn,
				&cache->rq, &context);
	while (ret == FI_SUCCESS) {
		entry = container_of(context,
					struct gnix_mr_cache_entry, context);
		switch (__entry_get_state(entry)) {
		case GNIX_CES_INUSE:
			/* First, warn that this might be a
			 * problem.*/
			if ((__notifier_warned == false) &&
			    !__entry_is_merged(entry)) {
				GNIX_WARN(FI_LOG_MR,
					  "Registered memory region"
					  " includes unmapped pages."
					  "  Have you freed memory"
					  " without closing the memory"
					  " region?\n");
				__notifier_warned = true;
			}

			GNIX_DEBUG(FI_LOG_MR,
				   "Marking unmapped entry (%p)"
				   " as retired %llx:%llx\n", entry,
				   entry->key.address,
				   entry->key.length);

			__entry_set_unmapped(entry);

			if (__entry_is_retired(entry)) {
				/* Nothing to do */
				break;
			}

			/* Retire this entry (remove from
			 * inuse tree) */

			__entry_set_retired(entry);
			iter = rbtFind(cache->inuse.rb_tree,
				       &entry->key);
			if (OFI_LIKELY(iter != NULL)) {
				ret = rbtErase(cache->inuse.rb_tree,
					       iter);
				if (ret != RBT_STATUS_OK) {
					GNIX_FATAL(FI_LOG_MR,
						   "Unmapped entry"
						   " could not be"
						   " removed from"
						   " in usetree.\n");
				}
			} else {
				/*  The only way we should get
				 *  here is if we're in the
				 *  middle of retiring this
				 *  entry.  Not sure if this
				 *  is worth a separate
				 *  warning from the one
				 *  above. */
			}

			break;
		case GNIX_CES_STALE:
			__entry_set_unmapped(entry);
			iter = rbtFind(cache->stale.rb_tree,
				       &entry->key);
			if (!iter) {
				break;
			}

			ret = rbtErase(cache->stale.rb_tree, iter);
			if (ret != RBT_STATUS_OK) {
				GNIX_FATAL(FI_LOG_MR,
					   "Unmapped entry could"
					   " not be removed "
					   " from stale tree.\n");
			}

			GNIX_DEBUG(FI_LOG_MR, "Removed unmapped entry"
				   " (%p) from stale tree %llx:%llx\n",
				   entry, entry->key.address,
				   entry->key.length);

			if (__mr_cache_lru_remove(cache, entry) == FI_SUCCESS) {
				GNIX_DEBUG(FI_LOG_MR, "Removed"
					   " unmapped entry (%p)"
					   " from lru list %llx:%llx\n",
					   entry, entry->key.address,
					   entry->key.length);

				ofi_atomic_dec32(&cache->stale.elements);

			} else {
				GNIX_WARN(FI_LOG_MR, "Failed to remove"
					  " unmapped entry"
					  " from lru list (%p) %p\n",
					  entry, iter);
			}

			__mr_cache_entry_destroy(cache, entry);

			break;
		default:
			GNIX_FATAL(FI_LOG_MR,
				   "Unmapped entry (%p) in incorrect"
				   " state: %d\n",
				   entry, entry->state);
		}

		ret = _gnix_smrn_get_event(cache->attr.smrn,
					&cache->rq, &context);
	}
	if (ret != -FI_EAGAIN) {
		/* Should we do something else here? */
		GNIX_WARN(FI_LOG_MR,
			  "_gnix_smrn_get_event returned error: %s\n",
			  fi_strerror(-ret));
	}

	return;
}

/**
 * Start monitoring a memory region
 *
 * @param[in] cache  a memory registration cache
 * @param[in] entry  a memory registration entry
 *
 * @return           return code from _gnix_smrn_monitor
 */
static int
__notifier_monitor(gnix_mr_cache_t *cache,
		   gnix_mr_cache_entry_t *entry)
{

	if (!cache->attr.lazy_deregistration) {
		return FI_SUCCESS;
	}

	if (cache->attr.smrn == NULL) {
		return FI_SUCCESS;
	}

	GNIX_DEBUG(FI_LOG_MR, "monitoring entry=%p %llx:%llx\n", entry,
		   entry->key.address, entry->key.length);

	return  _gnix_smrn_monitor(cache->attr.smrn,
						  &cache->rq,
					      (void *) entry->key.address,
					      entry->key.length,
					      (uint64_t) &entry->context,
						  &entry->context);
}

/**
 * Stop monitoring a memory region
 *
 * @param[in] cache  a memory registration cache
 * @param[in] entry  a memory registration entry
 *
 * @return           nothing
 */
static void
__notifier_unmonitor(gnix_mr_cache_t *cache,
		     gnix_mr_cache_entry_t *entry)
{
	int rc;

	if (!cache->attr.lazy_deregistration) {
		return;
	}

	if (cache->attr.smrn == NULL) {
		return;
	}

	__clear_notifier_events(cache);

	if (!__entry_is_unmapped(entry)) {
		GNIX_DEBUG(FI_LOG_MR, "unmonitoring entry=%p (state=%d)\n",
			   entry, entry->state);
		rc = _gnix_smrn_unmonitor(cache->attr.smrn,
						  (uint64_t) &entry->context,
					      &entry->context);
		if (rc != FI_SUCCESS) {
			/* This probably is okay (ESRCH), because the
			 * memory could have been unmapped in the
			 * interim, so clear the notifier events
			 * again */
			GNIX_DEBUG(FI_LOG_MR,
				   "failed to unmonitor memory (entry=%p),"
				   " so clear notifier events again\n",
				   entry, fi_strerror(-rc));

			__clear_notifier_events(cache);
		}
	}
}

/**
 * Destroys the memory registration cache entry and deregisters the memory
 *   region with uGNI
 *
 * @param[in] entry  a memory registration cache entry
 *
 * @return           return code from callbacks
 */
static inline int __mr_cache_entry_destroy(gnix_mr_cache_t *cache,
		gnix_mr_cache_entry_t *entry)
{
	int rc;

	rc = cache->attr.dereg_callback(entry->mr,
			cache->attr.destruct_context);
	if (!rc) {
		/* Should we bother with this check?  If we don't, the
		 * only difference it that __clear_notifier_events
		 * will be called one additional time. */
		if (!__entry_is_unmapped(entry)) {
			__notifier_unmonitor(cache, entry);
		}
		__entry_reset_state(entry);

		rc = cache->attr.destruct_callback(cache->attr.dereg_context);
		if (!rc)
			free(entry);
	} else {
		GNIX_INFO(FI_LOG_MR, "failed to deregister memory"
			  " region with callback, "
			  "cache_entry=%p ret=%i\n", entry, rc);
	}

	return rc;
}

static inline int __insert_entry_into_stale(
		gnix_mr_cache_t *cache,
		gnix_mr_cache_entry_t *entry)
{
	RbtStatus rc;
	int ret = 0;

	if (__entry_is_unmapped(entry)) {
		GNIX_DEBUG(FI_LOG_MR, "entry (%p) unmapped, not inserting"
			   " into stale %llx:%llx", entry,
			   entry->key.address, entry->key.length);
		/* Should we return some other value? */
		return ret;
	}

	rc = rbtInsert(cache->stale.rb_tree,
			&entry->key,
			entry);
	if (rc != RBT_STATUS_OK) {
		GNIX_ERR(FI_LOG_MR,
				"could not insert into stale rb tree,"
				" rc=%d key.address=%llx key.length=%llx entry=%p",
				rc,
				entry->key.address,
				entry->key.length,
				entry);

		ret = __mr_cache_entry_destroy(cache, entry);
	} else {
		GNIX_DEBUG(FI_LOG_MR,
			   "inserted key=%llx:%llx into stale\n",
			   entry->key.address, entry->key.length);

		__mr_cache_lru_enqueue(cache, entry);
		ofi_atomic_inc32(&cache->stale.elements);
		switch (__entry_get_state(entry)) {
		case  GNIX_CES_INUSE:
			__entry_set_state(entry, GNIX_CES_STALE);
			break;
		default:
			GNIX_FATAL(FI_LOG_MR,
				   "stale entry (%p) %llx:%llx in bad"
				   " state (%d)\n", entry,
				   entry->key.address, entry->key.length,
				   entry->state);
		}
	}

	return ret;
}

static inline void __resolve_stale_entry_collision(
		gnix_mr_cache_t *cache,
		RbtIterator found,
		gnix_mr_cache_entry_t *entry)
{
	RbtStatus __attribute__((unused)) rc;
	gnix_mr_cache_entry_t *c_entry, *tmp;
	gnix_mr_cache_key_t *c_key;
	int ret;
	DLIST_HEAD(to_destroy);
	RbtIterator iter = found;
	int add_new_entry = 1, cmp;

	GNIX_TRACE(FI_LOG_MR, "\n");

	GNIX_DEBUG(FI_LOG_MR,
		   "resolving collisions with entry (%p) %llx:%llx\n",
		   entry, entry->key.address, entry->key.length);

	while (iter) {
		rbtKeyValue(cache->stale.rb_tree, iter, (void **) &c_key,
				(void **) &c_entry);

		cmp = __find_overlapping_addr(&entry->key, c_key);
		if (cmp != 0)
			break;

		if (__can_subsume(&entry->key, c_key) ||
				(entry->key.length > c_key->length)) {
			GNIX_DEBUG(FI_LOG_MR,
				   "adding stale entry (%p) to destroy list,"
				   "  key=%llx:%llx\n", c_entry,
				   c_key->address, c_key->length);
			dlist_insert_tail(&c_entry->siblings, &to_destroy);
		} else {
			add_new_entry = 0;
		}

		iter = rbtNext(cache->stale.rb_tree, iter);
	}

	/* TODO I can probably do this in a single sweep, avoiding a second
	 * pass and incurring n lg n removal time
	 */
	dlist_for_each_safe(&to_destroy, c_entry, tmp, siblings)
	{
		GNIX_DEBUG(FI_LOG_MR, "removing key from tree, entry %p"
			   " key=%llx:%llx\n", c_entry,
			   c_entry->key.address, c_entry->key.length);
		iter = rbtFind(cache->stale.rb_tree, &c_entry->key);
		if (OFI_UNLIKELY(!iter)) {
			GNIX_FATAL(FI_LOG_MR, "key not found\n");
		}

		rc = rbtErase(cache->stale.rb_tree,
						iter);
		if (OFI_UNLIKELY(rc != RBT_STATUS_OK)) {
			GNIX_FATAL(FI_LOG_MR,
				   "could not remove entry from tree\n");
		}

		if (__mr_cache_lru_remove(cache, c_entry) != FI_SUCCESS) {
			GNIX_WARN(FI_LOG_MR, "Failed to remove entry"
				  " from lru list (%p)\n",
				  c_entry);
		}
		ofi_atomic_dec32(&cache->stale.elements);
		dlist_remove(&c_entry->siblings);
		__mr_cache_entry_destroy(cache, c_entry);
	}
	if (OFI_UNLIKELY(!dlist_empty(&to_destroy))) {
		GNIX_FATAL(FI_LOG_MR, "to_destroy not empty\n");
	}

	if (add_new_entry) {
		ret = __insert_entry_into_stale(cache, entry);
		if (ret) {
			GNIX_FATAL(FI_LOG_MR,
				   "Failed to insert subsumed MR "
				   " entry (%p) into stale list\n",
				   entry);
		}
	} else {
		/* stale entry is larger than this one
		 * so lets just toss this entry out
		 */
		GNIX_DEBUG(FI_LOG_MR,
			   "larger entry already exists, "
			   "to_destroy=%llx:%llx\n",
			   entry->key.address, entry->key.length);

		ret = __mr_cache_entry_destroy(cache, entry);
		if (ret) {
			GNIX_ERR(FI_LOG_MR,
					"failed to destroy a "
					"registration, entry=%p grc=%d\n",
					c_entry, ret);
		}
	}
}

/**
 * Increments the reference count on a memory registration cache entry
 *
 * @param[in] cache  gnix memory registration cache
 * @param[in] entry  a memory registration cache entry
 *
 * @return           reference count for the registration
 */
static inline int __mr_cache_entry_get(
		gnix_mr_cache_t       *cache,
		gnix_mr_cache_entry_t *entry)
{
	GNIX_TRACE(FI_LOG_MR, "\n");

	GNIX_DEBUG(FI_LOG_MR,
		   "Up ref cnt on entry %p\n", entry);
	return ofi_atomic_inc32(&entry->ref_cnt);
}

/**
 * Decrements the reference count on a memory registration cache entry
 *
 * @param[in] cache  gnix memory registration cache
 * @param[in] entry  a memory registration cache entry
 *
 * @return           return code from dereg callback
 */
static inline int __mr_cache_entry_put(
		gnix_mr_cache_t       *cache,
		gnix_mr_cache_entry_t *entry)
{
	RbtIterator iter;
	int rc;
	gni_return_t grc = GNI_RC_SUCCESS;
	RbtIterator found;
	gnix_mr_cache_entry_t *parent = NULL;
	struct dlist_entry *next;

	GNIX_TRACE(FI_LOG_MR, "\n");

	if (cache->attr.lazy_deregistration) {
		__clear_notifier_events(cache);
	}

	GNIX_DEBUG(FI_LOG_MR,
		   "Decrease ref cnt on entry %p\n", entry);
	if (ofi_atomic_dec32(&entry->ref_cnt) == 0) {
		next = entry->siblings.next;
		dlist_remove(&entry->children);
		dlist_remove(&entry->siblings);

		/* if this is the last child to deallocate,
		 * release the reference to the parent
		 */
		if (next != &entry->siblings && dlist_empty(next)) {
			parent = container_of(next, gnix_mr_cache_entry_t,
					children);
			grc = __mr_cache_entry_put(cache, parent);
			if (OFI_UNLIKELY(grc != GNI_RC_SUCCESS)) {
				GNIX_ERR(FI_LOG_MR,
						"failed to release reference to parent, "
						"parent=%p refs=%d\n",
						parent,
						ofi_atomic_get32(&parent->ref_cnt));
			}
		}

		ofi_atomic_dec32(&cache->inuse.elements);

		if (!__entry_is_retired(entry)) {
			iter = rbtFind(cache->inuse.rb_tree, &entry->key);
			if (OFI_UNLIKELY(!iter)) {
				GNIX_ERR(FI_LOG_MR,
						"failed to find entry in the inuse cache\n");
			} else {
				rc = rbtErase(cache->inuse.rb_tree, iter);
				if (OFI_UNLIKELY(rc != RBT_STATUS_OK)) {
					GNIX_ERR(FI_LOG_MR,
						 "failed to erase lru entry"
						 " from stale tree\n");
				}
			}
		}

		/* if we are doing lazy dereg and the entry
		 * isn't retired, put it in the stale cache
		 */
		if (cache->attr.lazy_deregistration &&
		    !(__entry_is_retired(entry))) {
			GNIX_DEBUG(FI_LOG_MR,
				   "moving key %llx:%llx to stale\n",
				   entry->key.address, entry->key.length);

			found = rbtFindLeftmost(cache->stale.rb_tree,
					&entry->key, __find_overlapping_addr);
			if (found) {
				/* one or more stale entries would overlap with this
				 * new entry. We need to resolve these collisions by dropping
				 * registrations
				 */
				__resolve_stale_entry_collision(cache, found, entry);
			} else {
				/* if not found, ... */
				grc = __insert_entry_into_stale(cache, entry);
			}
		} else {
			/* if retired or not using lazy registration */
			GNIX_DEBUG(FI_LOG_MR,
				   "destroying entry, key=%llx:%llx\n",
				   entry->key.address, entry->key.length);

			grc = __mr_cache_entry_destroy(cache, entry);
		}

		if (OFI_UNLIKELY(grc != GNI_RC_SUCCESS)) {
			GNIX_INFO(FI_LOG_MR, "dereg callback returned '%s'\n",
				  gni_err_str[grc]);
		}
	}


	return grc;
}

/**
 * Checks the sanity of cache attributes
 *
 * @param[in]   attr  attributes structure to be checked
 * @return      FI_SUCCESS if the attributes are valid
 *              -FI_EINVAL if the attributes are invalid
 */
static inline int __check_mr_cache_attr_sanity(gnix_mr_cache_attr_t *attr)
{
	/* 0 < attr->hard_reg_limit < attr->soft_reg_limit */
	if (attr->hard_reg_limit > 0 &&
			attr->hard_reg_limit < attr->soft_reg_limit)
		return -FI_EINVAL;

	/* callbacks must be provided */
	if (!attr->reg_callback || !attr->dereg_callback ||
			!attr->destruct_callback)
		return -FI_EINVAL;

	/* valid otherwise */
	return FI_SUCCESS;
}

int _gnix_mr_cache_init(
		gnix_mr_cache_t         **cache,
		gnix_mr_cache_attr_t    *attr)
{
	gnix_mr_cache_attr_t *cache_attr = &__default_mr_cache_attr;
	gnix_mr_cache_t *cache_p;
	int rc;

	GNIX_TRACE(FI_LOG_MR, "\n");

	/* if the provider asks us to use their attributes, are they sane? */
	if (attr) {
		if (__check_mr_cache_attr_sanity(attr) != FI_SUCCESS)
			return -FI_EINVAL;

		cache_attr = attr;
	}

	cache_p = (gnix_mr_cache_t *) calloc(1, sizeof(*cache_p));
	if (!cache_p)
		return -FI_ENOMEM;

	/* save the attribute values */
	memcpy(&cache_p->attr, cache_attr, sizeof(*cache_attr));

	/* list is used because entries can be removed from the stale list if
	 *   a user might call register on a stale entry's memory region
	 */
	dlist_init(&cache_p->lru_head);

	/* set up inuse tree */
	cache_p->inuse.rb_tree = rbtNew(__mr_cache_key_comp);
	if (!cache_p->inuse.rb_tree) {
		rc = -FI_ENOMEM;
		goto err_inuse;
	}

	/* if using lazy deregistration, set up stale tree */
	if (cache_p->attr.lazy_deregistration) {
		cache_p->stale.rb_tree = rbtNew(__mr_cache_key_comp);
		if (!cache_p->stale.rb_tree) {
			rc = -FI_ENOMEM;
			goto err_stale;
		}
	}

	/* initialize the element counts. If we are reinitializing a dead cache,
	 *   destroy will have already set the element counts
	 */
	if (cache_p->state == GNIX_MRC_STATE_UNINITIALIZED) {
		ofi_atomic_initialize32(&cache_p->inuse.elements, 0);
		ofi_atomic_initialize32(&cache_p->stale.elements, 0);
	}

	cache_p->hits = 0;
	cache_p->misses = 0;

	cache_p->state = GNIX_MRC_STATE_READY;

	dlist_init(&cache_p->rq.list);
	dlist_init(&cache_p->rq.entry);
	fastlock_init(&cache_p->rq.lock);

	*cache = cache_p;

	return FI_SUCCESS;

err_stale:
	rbtDelete(cache_p->inuse.rb_tree);
	cache_p->inuse.rb_tree = NULL;
err_inuse:
	free(cache_p);

	return rc;
}

int _gnix_mr_cache_destroy(gnix_mr_cache_t *cache)
{
	if (cache->state != GNIX_MRC_STATE_READY)
		return -FI_EINVAL;

	GNIX_TRACE(FI_LOG_MR, "\n");

	/*
	 * Remove all of the stale entries from the cache
	 */
	_gnix_mr_cache_flush(cache);

	/*
	 * if there are still elements in the cache after the flush,
	 *   then someone forgot to deregister memory.
	 *   We probably shouldn't destroy the cache at this point.
	 */
	if (ofi_atomic_get32(&cache->inuse.elements) != 0) {
		return -FI_EAGAIN;
	}

	/* destroy the tree */
	rbtDelete(cache->inuse.rb_tree);
	cache->inuse.rb_tree = NULL;

	/* stale will been flushed already, so just destroy the tree */
	if (cache->attr.lazy_deregistration) {
		rbtDelete(cache->stale.rb_tree);
		cache->stale.rb_tree = NULL;
	}

	cache->state = GNIX_MRC_STATE_DEAD;
	free(cache);

	return FI_SUCCESS;
}

int __mr_cache_flush(gnix_mr_cache_t *cache, int flush_count) {
	int rc;
	RbtIterator iter;
	gnix_mr_cache_entry_t *entry;
	int destroyed = 0;

	GNIX_TRACE(FI_LOG_MR, "\n");

	GNIX_DEBUG(FI_LOG_MR, "starting flush on memory registration cache\n");

	/* flushes are unnecessary for caches without lazy deregistration */
	if (!cache->attr.lazy_deregistration)
		return FI_SUCCESS;

	while (!dlist_empty(&cache->lru_head)) {

		if (flush_count >= 0 && flush_count == destroyed)
			break;

		rc = __mr_cache_lru_dequeue(cache, &entry);
		if (OFI_UNLIKELY(rc != FI_SUCCESS)) {
			GNIX_ERR(FI_LOG_MR,
					"list may be corrupt, no entries from lru pop\n");
			break;
		}

		GNIX_DEBUG(FI_LOG_MR, "attempting to flush key %llx:%llx\n",
			   entry->key.address, entry->key.length);
		iter = rbtFind(cache->stale.rb_tree, &entry->key);
		if (OFI_UNLIKELY(!iter)) {
			GNIX_ERR(FI_LOG_MR,
				 "lru entries MUST be present in the cache,"
				 " could not find entry (%p) in stale tree"
				 " %llx:%llx\n",
				 entry, entry->key.address, entry->key.length);
			break;
		}

		rc = rbtErase(cache->stale.rb_tree, iter);
		if (OFI_UNLIKELY(rc != RBT_STATUS_OK)) {
			GNIX_ERR(FI_LOG_MR,
					"failed to erase lru entry from stale tree\n");
			break;
		}

		__mr_cache_entry_destroy(cache, entry);
		entry = NULL;
		++destroyed;
	}

	GNIX_DEBUG(FI_LOG_MR, "flushed %i of %i entries from memory "
		   "registration cache\n", destroyed,
		   ofi_atomic_get32(&cache->stale.elements));

	if (destroyed > 0) {
		ofi_atomic_sub32(&cache->stale.elements, destroyed);
	}

	return FI_SUCCESS;
}

int _gnix_mr_cache_flush(gnix_mr_cache_t *cache)
{

	if (OFI_UNLIKELY(cache->state != GNIX_MRC_STATE_READY))
		return -FI_EINVAL;

	__mr_cache_flush(cache, cache->attr.hard_reg_limit);

	return FI_SUCCESS;
}

static int __mr_cache_search_inuse(
		gnix_mr_cache_t             *cache,
		uint64_t                    address,
		uint64_t                    length,
		gnix_mr_cache_entry_t       **entry,
		gnix_mr_cache_key_t         *key,
		struct _gnix_fi_reg_context *fi_reg_context)
{
	int ret = FI_SUCCESS, cmp;
	RbtIterator iter;
	gnix_mr_cache_key_t *found_key, new_key;
	gnix_mr_cache_entry_t *found_entry;
	uint64_t new_end, found_end;
	DLIST_HEAD(retired_entries);

	if (cache->attr.lazy_deregistration) {
		__clear_notifier_events(cache);
	}

	/* first we need to find an entry that overlaps with this one.
	 * care should be taken to find the left most entry that overlaps
	 * with this entry since the entry we are searching for might overlap
	 * many entries and the tree may be left or right balanced
	 * at the head
	 */
	iter = rbtFindLeftmost(cache->inuse.rb_tree, (void *) key,
			__find_overlapping_addr);
	if (!iter) {
		GNIX_DEBUG(FI_LOG_MR,
			   "could not find key in inuse, key=%llx:%llx\n",
			   key->address, key->length);
		return -FI_ENOENT;
	}

	rbtKeyValue(cache->inuse.rb_tree, iter, (void **) &found_key,
			(void **) &found_entry);

	GNIX_DEBUG(FI_LOG_MR,
		   "found a key that matches the search criteria, "
		   "found=%llx:%llx key=%llx:%llx\n",
		   found_key->address, found_key->length,
		   key->address, key->length);

	/* if the entry that we've found completely subsumes
	 * the requested entry, just return a reference to
	 * that existing registration
	 */
	if (__can_subsume(found_key, key)) {
		GNIX_DEBUG(FI_LOG_MR,
			   "found an entry that subsumes the request, "
			   "existing=%llx:%llx key=%llx:%llx entry %p\n",
			   found_key->address, found_key->length,
			   key->address, key->length, found_entry);
		*entry = found_entry;
		__mr_cache_entry_get(cache, found_entry);

		cache->hits++;
		return FI_SUCCESS;
	}

	/* otherwise, iterate from the existing entry until we can no longer
	 * find an entry that overlaps with the new registration and remove
	 * and retire each of those entries.
	 */
	new_key.address = MIN(found_key->address, key->address);
	new_end = key->address + key->length;
	while (iter) {
		rbtKeyValue(cache->inuse.rb_tree, iter, (void **) &found_key,
				(void **) &found_entry);


		cmp = __find_overlapping_addr(found_key, key);
		GNIX_DEBUG(FI_LOG_MR,
			   "candidate: key=%llx:%llx result=%d\n",
			   found_key->address,
			   found_key->length, cmp);
		if (cmp != 0)
			break;

		/* compute new ending address */
		found_end = found_key->address + found_key->length;

		/* mark the entry as retired */
		GNIX_DEBUG(FI_LOG_MR, "retiring entry, key=%llx:%llx entry %p\n",
			   found_key->address, found_key->length, found_entry);
		__entry_set_retired(found_entry);
		dlist_insert_tail(&found_entry->siblings, &retired_entries);

		iter = rbtNext(cache->inuse.rb_tree, iter);
	}
	/* Since our new key might fully overlap every other entry in the tree,
	 * we need to take the maximum of the last entry and the new entry
	 */
	new_key.length = MAX(found_end, new_end) - new_key.address;


	/* remove retired entries from tree */
	GNIX_DEBUG(FI_LOG_MR, "removing retired entries from inuse tree\n");
	__remove_sibling_entries_from_tree(cache,
			&retired_entries, cache->inuse.rb_tree);

	/* create new registration */
	GNIX_DEBUG(FI_LOG_MR,
		   "creating a new merged registration, key=%llx:%llx\n",
		   new_key.address, new_key.length);
	ret = __mr_cache_create_registration(cache,
			new_key.address, new_key.length,
			entry, &new_key, fi_reg_context);
	if (ret) {
		/* If we get here, one of two things have happened.
		 * Either some part of the new merged registration was
		 * unmapped (i.e., freed by user) or the merged
		 * registration failed for some other reason (probably
		 * GNI_RC_ERROR_RESOURCE).  The first case is a user
		 * error (which they should have been warned about by
		 * the notifier), and the second case is always
		 * possible.  Neither case is a problem.  The entries
		 * above have been retired, and here we return the
		 * error */
		GNIX_DEBUG(FI_LOG_MR,
			   "failed to create merged registration, key=",
			   new_key.address, new_key.length);
		return ret;
	}
	GNIX_DEBUG(FI_LOG_MR,
		   "created a new merged registration, key=%llx:%llx entry %p\n",
		   new_key.address, new_key.length, *entry);

	__entry_set_merged(*entry);

	/* move retired entries to the head of the new entry's child list */
	if (!dlist_empty(&retired_entries)) {
		__attach_retired_entries_to_registration(cache,
				&retired_entries, *entry);
	}

	cache->misses++;

	return ret;
}

static int __mr_cache_search_stale(
		gnix_mr_cache_t             *cache,
		uint64_t                    address,
		uint64_t                    length,
		gnix_mr_cache_entry_t       **entry,
		gnix_mr_cache_key_t         *key,
		struct _gnix_fi_reg_context *fi_reg_context)
{
	int ret;
	RbtStatus rc;
	RbtIterator iter;
	gnix_mr_cache_key_t *mr_key;
	gnix_mr_cache_entry_t *mr_entry, *tmp;

	if (cache->attr.lazy_deregistration) {
		__clear_notifier_events(cache);
	}

	GNIX_DEBUG(FI_LOG_MR, "searching for stale entry, key=%llx:%llx\n",
		   key->address, key->length);

	iter = rbtFindLeftmost(cache->stale.rb_tree, (void *) key,
			__find_overlapping_addr);
	if (!iter)
		return -FI_ENOENT;

	rbtKeyValue(cache->stale.rb_tree, iter, (void **) &mr_key,
			(void **) &mr_entry);

	GNIX_DEBUG(FI_LOG_MR,
		   "found a matching entry, found=%llx:%llx key=%llx:%llx\n",
		   mr_key->address, mr_key->length,
		   key->address, key->length);


	/* if the entry that we've found completely subsumes
	 * the requested entry, just return a reference to
	 * that existing registration
	 */
	if (__can_subsume(mr_key, key)) {
		ret = __mr_cache_search_inuse(cache, address, length,
				&tmp, mr_key, fi_reg_context);
		if (ret == FI_SUCCESS) {
			/* if we found an entry in the inuse tree
			 * in this manner, it means that there was
			 * an entry either overlapping or contiguous
			 * with the stale entry in the inuse tree, and
			 * a new entry has been made and saved to tmp.
			 * The old entry (mr_entry) should be destroyed
			 * now as it is no longer needed.
			 */
			GNIX_DEBUG(FI_LOG_MR,
				   "removing entry from stale key=%llx:%llx\n",
				   mr_key->address, mr_key->length);

			rc = rbtErase(cache->stale.rb_tree, iter);
			if (OFI_UNLIKELY(rc != RBT_STATUS_OK)) {
				GNIX_ERR(FI_LOG_MR,
						"failed to erase entry from stale tree\n");
			} else {
				if (__mr_cache_lru_remove(cache, mr_entry)
				    == FI_SUCCESS) {
					ofi_atomic_dec32(&cache->stale.elements);
				} else {
					GNIX_WARN(FI_LOG_MR, "Failed to remove"
						  " entry (%p) from lru list\n",
						  mr_entry);
				}
				__mr_cache_entry_destroy(cache, mr_entry);
			}

			*entry = tmp;
		} else {
			GNIX_DEBUG(FI_LOG_MR,
				   "removing entry (%p) from stale and"
				   " migrating to inuse, key=%llx:%llx\n",
				   mr_entry, mr_key->address, mr_key->length);
			rc = rbtErase(cache->stale.rb_tree, iter);
			if (OFI_UNLIKELY(rc != RBT_STATUS_OK)) {
				GNIX_FATAL(FI_LOG_MR,
					   "failed to erase entry (%p) from "
					   " stale tree\n", mr_entry);
			}

			if (__mr_cache_lru_remove(cache, mr_entry)
			    != FI_SUCCESS) {
				GNIX_WARN(FI_LOG_MR, "Failed to remove"
					  " entry (%p) from lru list\n",
					  mr_entry);
			}

			ofi_atomic_dec32(&cache->stale.elements);

			/* if we made it to this point, there weren't
			 * any entries in the inuse tree that would
			 * have overlapped with this entry
			 */
			rc = rbtInsert(cache->inuse.rb_tree,
					&mr_entry->key, mr_entry);
			if (OFI_UNLIKELY(rc != RBT_STATUS_OK)) {
				GNIX_FATAL(FI_LOG_MR,
					   "failed to insert entry into"
					   "inuse tree\n");
			}

			ofi_atomic_set32(&mr_entry->ref_cnt, 1);
			ofi_atomic_inc32(&cache->inuse.elements);

			*entry = mr_entry;
		}

		return FI_SUCCESS;
	}

	GNIX_DEBUG(FI_LOG_MR,
		   "could not use matching entry, "
		   "found=%llx:%llx\n",
		   mr_key->address, mr_key->length);

	return -FI_ENOENT;
}

static int __mr_cache_create_registration(
		gnix_mr_cache_t             *cache,
		uint64_t                    address,
		uint64_t                    length,
		gnix_mr_cache_entry_t       **entry,
		gnix_mr_cache_key_t         *key,
		struct _gnix_fi_reg_context *fi_reg_context)
{
	int rc;
	gnix_mr_cache_entry_t *current_entry;
	void *handle;

	/* if we made it here, we didn't find the entry at all */
	current_entry = calloc(1, sizeof(*current_entry) + cache->attr.elem_size);
	if (!current_entry)
		return -FI_ENOMEM;

	handle = (void *) current_entry->mr;

	dlist_init(&current_entry->lru_entry);
	dlist_init(&current_entry->children);
	dlist_init(&current_entry->siblings);

	handle = cache->attr.reg_callback(handle, (void *) address, length,
			fi_reg_context, cache->attr.reg_context);
	if (OFI_UNLIKELY(!handle)) {
		GNIX_INFO(FI_LOG_MR,
			  "failed to register memory with callback\n");
		goto err;
	}

	__entry_reset_state(current_entry);

	/* set up the entry's key */
	current_entry->key.address = address;
	current_entry->key.length = length;

	rc = __notifier_monitor(cache, current_entry);
	if (OFI_UNLIKELY(rc != FI_SUCCESS)) {
		GNIX_INFO(FI_LOG_MR,
			  "failed to monitor memory with notifier\n");
		goto err_dereg;
	}

	rc = rbtInsert(cache->inuse.rb_tree, &current_entry->key,
			current_entry);
	if (OFI_UNLIKELY(rc != RBT_STATUS_OK)) {
		GNIX_ERR(FI_LOG_MR, "failed to insert registration "
				"into cache, ret=%i\n", rc);

		goto err_dereg;
	}

	GNIX_DEBUG(FI_LOG_MR, "inserted key %llx:%llx into inuse %p\n",
		   current_entry->key.address, current_entry->key.length, current_entry);


	ofi_atomic_inc32(&cache->inuse.elements);
	ofi_atomic_initialize32(&current_entry->ref_cnt, 1);

	*entry = current_entry;

	return FI_SUCCESS;

err_dereg:
	rc = cache->attr.dereg_callback(current_entry->mr,
					cache->attr.dereg_context);
	if (OFI_UNLIKELY(rc)) {
		GNIX_INFO(FI_LOG_MR,
			  "failed to deregister memory with "
			  "callback, ret=%d\n", rc);
	}
err:
	free(current_entry);
	return -FI_ENOMEM;
}

/**
 * Function to register memory with the cache
 *
 * @param[in] cache           gnix memory registration cache pointer
 * @param[in] mr              gnix memory region descriptor pointer
 * @param[in] address         base address of the memory region to be
 *                            registered
 * @param[in] length          length of the memory region to be registered
 * @param[in] fi_reg_context  fi_reg_mr API call parameters
 * @param[in,out] mem_hndl    gni memory handle pointer to written to and
 *                            returned
 */
int _gnix_mr_cache_register(
		gnix_mr_cache_t             *cache,
		uint64_t                    address,
		uint64_t                    length,
		struct _gnix_fi_reg_context *fi_reg_context,
		void                        **handle)
{
	int ret;
	gnix_mr_cache_key_t key = {
			.address = address,
			.length = length,
	};
	gnix_mr_cache_entry_t *entry;

	GNIX_TRACE(FI_LOG_MR, "\n");

	/* fastpath inuse */
	ret = __mr_cache_search_inuse(cache, address, length,
			&entry, &key, fi_reg_context);
	if (ret == FI_SUCCESS)
		goto success;

	/* if we shouldn't introduce any new elements, return -FI_ENOSPC */
	if (OFI_UNLIKELY(cache->attr.hard_reg_limit > 0 &&
			 (ofi_atomic_get32(&cache->inuse.elements) >=
			  cache->attr.hard_reg_limit))) {
		ret = -FI_ENOSPC;
		goto err;
	}

	if (cache->attr.lazy_deregistration) {
		__clear_notifier_events(cache);

		/* if lazy deregistration is in use, we can check the
		 *   stale tree
		 */
		ret = __mr_cache_search_stale(cache, address, length,
				&entry, &key, fi_reg_context);
		if (ret == FI_SUCCESS) {
			cache->hits++;
			goto success;
		}
	}

	/* If the cache is full, then flush one of the stale entries to make
	 *   room for the new entry. This works because we check above to see if
	 *   the number of inuse entries exceeds the hard reg limit
	 */
	if ((ofi_atomic_get32(&cache->inuse.elements) +
			ofi_atomic_get32(&cache->stale.elements)) == cache->attr.hard_reg_limit)
		__mr_cache_flush(cache, 1);

	ret = __mr_cache_create_registration(cache, address, length,
			&entry, &key, fi_reg_context);
	if (ret) {
		goto err;
	}

	cache->misses++;

success:
	__entry_set_state(entry, GNIX_CES_INUSE);
	*handle = (void *) entry->mr;

	return FI_SUCCESS;

err:
	return ret;
}

/**
 * Function to deregister memory in the cache
 *
 * @param[in]  mr     gnix memory registration descriptor pointer
 *
 * @return     FI_SUCCESS on success
 *             -FI_ENOENT if there isn't an active memory registration
 *               associated with the mr
 *             return codes associated with dereg callback
 */
int _gnix_mr_cache_deregister(
		gnix_mr_cache_t *cache,
		void            *handle)
{
	gnix_mr_cache_entry_t *entry;
	gni_return_t grc;

	GNIX_TRACE(FI_LOG_MR, "\n");

	/* check to see if we can find the entry so that we can drop the
	 *   held reference
	 */

	entry = container_of(handle, gnix_mr_cache_entry_t, mr);
	if (__entry_get_state(entry) != GNIX_CES_INUSE) {
		GNIX_WARN(FI_LOG_MR, "entry (%p) in incorrect state (%d)\n",
			  entry, entry->state);
		return -FI_EINVAL;
	}

	GNIX_DEBUG(FI_LOG_MR, "entry found, entry=%p refs=%d\n",
		   entry, ofi_atomic_get32(&entry->ref_cnt));

	grc = __mr_cache_entry_put(cache, entry);

	/* Since we check this on each deregistration, the amount of elements
	 * over the limit should always be 1
	 */
	if (ofi_atomic_get32(&cache->stale.elements) > cache->attr.hard_stale_limit)
		__mr_cache_flush(cache, 1);

	return gnixu_to_fi_errno(grc);
}

