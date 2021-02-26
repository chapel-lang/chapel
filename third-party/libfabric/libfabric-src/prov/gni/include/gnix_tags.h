/*
 * Copyright (c) 2015-2016 Cray Inc. All rights reserved.
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

/**
 * Notes:
 *
 * This tag matching system currently implements a linked-list version of
 * a tag matcher.
 *
 * A hash list implementation was planned but will be not pursued due to the
 * constraints of the problem.
 *
 * As understood at the time of this writing, matching a tag involves matching
 * the bits of the tag against the bits of the tag to find, less the ignored
 * bits. When no bits are ignored, there is no subset of tags to match other
 * than the exact match. When some bits are ignored, there is a subset of tags
 * that are not exact matches, but are considered matches according to the
 * ignored bits. This problem represents a search over a k-space, where k is
 * the number of distinct ignore fields given by the tag format for the
 * provider. Each ignore field in the tag format distinct separates tags into
 * different 'bins' within the same field.
 *
 * A hash list implementation is not impossible, but can be computationally
 * impractical. This is due to the way a search must occur if bits are ignored.
 * The problem would be trivial if no bits were ignored, as the implementation
 * could simply go to the bucket where the tag was hashed and search there.
 * However, because ignored bits could hash to multiple, if not all, buckets,
 * the tag matcher must search all buckets. It takes more time to compute all
 * permutations of affected tags, than to just search each tag, so the default
 * behavior is to search all buckets if a non-zero ignore field is provided.
 *
 * In the event that the ignore field is always zero, a hash list
 * implementation is strictly better than a list implementation. However,
 * once ignore bits are considered, the problem becomes more complex.
 *
 * Consider a hash list implementation where tags are split into K buckets,
 * and tags are evenly distributed. If N tags are placed into the hash list in
 * an evenly distributed manner, we can compute a best, average and worst
 * case expectation for the hash list implementation.
 *
 * In the best case, the tag we are searching for is at the front of the list
 * and only one tag in the system could match the search parameters, then
 * we get the following best case analysis:
 *
 * When tags are evenly distributed, there should be ~(N/K) tags per bucket.
 *
 * Best(hash-list, middle list): ((N/K) * (K/ 2)) + 1 == (N/2) + 1
 * Best(hash-list, first list): 1
 *
 *   We can assume that on average, we'll have to search half the buckets in
 *   the best case, since the tags are evenly distributed. The very best case
 *   is that the tag would be in the first bucket, giving us O(1) instead.
 *   However, since tag location is determined on the hash, we have to assume
 *   that we will have to search half the buckets first (N/K) * (K/2). Since
 *   the element we are searching for is at the front of the list, it only
 *   takes one search on that list.
 *
 * Average(hash-list): (N/K) * (K/2) + (N/(2K)) == (1/2) * (N + N/K)
 *
 *   Similar to the best case, we can assume we'll have to search through half
 *   of the bucket when the ignore bits are set. Also, we can assume that the
 *   entry we are searching for lies in the middle of one of the hash lists.
 *   In this case, the average is not much worse than the best case.
 *
 * Worst(hash-list, last list): ((N/K) * K - 1) + (N/K) == N
 *
 *   The worst case is much more simple than the rest. It would be the last
 *   element in the last list. This would be no worse than a linear search
 *   through the list.
 *
 * All things considered, the hash list seems like it would be relatively
 * reasonable with a more likely chance of finding entries faster than
 * a standard linked list. However, a simple sanalysis shows that
 * that the best case is likely to be more expensive than the estimate.
 *
 * There is a bias in the algorithm. Since we always prefer the oldest, and
 * entries are ordered from oldest to newest, searching should
 * complete in less than N/2 operations on average. However, with the
 * hash-list, the bias has no influence due to the way that tags are
 * distributed. This causes the hash-list to perform much more slowly than
 * the list implementation.
 *
 * For the above listed reasons, a hash-list version of the tag matcher is
 * available, but should only be used under certain expectations. If the
 * frequency of searching with ignored bits is low, then a hash list may be
 * faster. If the ignore bits are used frequently and should match with
 * several requests, then that may also be preferred. However, in the event that
 * the user searches with few ignored bits that would match against very
 * few requests, then they would likely encounter average case behavior more
 * frequently than expected and thus would spend more time in the tag matcher.
 */

/*
 * Examples:
 *
 * Init:
 *
 * _gnix_posted_tag_storage_init(&ep->posted_recvs, NULL);
 * _gnix_unexpected_tag_storage_init(&ep->unexpected_recvs, NULL);
 *
 * On receipt of a message:
 *
 * fastlock_acquire(&ep->tag_lock);
 * req = _gnix_remove_by_tag(&ep->posted_recvs, msg->tag, 0);
 * if (!req)
 *     _gnix_insert_by_tag(&ep->unexpected_recvs, msg->tag, msg->req);
 * fastlock_release(&ep->tag_lock);
 *
 * On post of receive:
 *
 * fastlock_acquire(&ep->tag_lock);
 * tag_req = _gnix_remove_by_tag(&ep->unexpected_recvs,
 *           req->tag, req->ignore);
 * if (!tag_req)
 *     _gnix_insert_by_tag(&ep->posted_recvs, tag, req);
 * fastlock_release(&ep->tag_lock);
 *
 */

#ifndef PROV_GNI_SRC_GNIX_TAGS_H_
#define PROV_GNI_SRC_GNIX_TAGS_H_

#include <stdlib.h>
#include <ofi.h>
#include <ofi_list.h>

#include "gnix_util.h"

/* enumerations */

/**
 * Enumeration for determining the underlying data structure for a
 * tag storage.
 *
 * Using auto will choose one of list, hlist or kdtree based on mem_tag_format.
 */
enum {
	GNIX_TAG_AUTOSELECT = 0,//!< GNIX_TAG_AUTOSELECT
	GNIX_TAG_LIST,          //!< GNIX_TAG_LIST
	GNIX_TAG_HLIST,         //!< GNIX_TAG_HLIST
	GNIX_TAG_KDTREE,        //!< GNIX_TAG_KDTREE
	GNIX_TAG_MAXTYPES,      //!< GNIX_TAG_MAXTYPES
};

/**
 * Enumeration for the tag storage states
 */
enum {
	GNIX_TS_STATE_UNINITIALIZED = 0,//!< GNIX_TS_STATE_UNINITIALIZED
	GNIX_TS_STATE_INITIALIZED,      //!< GNIX_TS_STATE_INITIALIZED
	GNIX_TS_STATE_DESTROYED,        //!< GNIX_TS_STATE_DESTROYED
};

/* forward declarations */
struct gnix_tag_storage;
struct gnix_fab_req;
struct gnix_address;

/* structure declarations */
/**
 * @brief Function dispatch table for the different types of underlying structures
 * used in the tag storage.
 *
 * @var insert_tag    insert a request into the tag storage
 * @var remove_tag    remove a request from the tag storage
 * @var peek_tag      probe tag storage for a specific tag
 * @var init          performs specific initialization based on underlying
 *                     data structure
 * @var fini          performs specific finalization based on underlying
 *                     data structure
 */
struct gnix_tag_storage_ops {
	int (*insert_tag)(struct gnix_tag_storage *ts, uint64_t tag,
			struct gnix_fab_req *req);
	struct gnix_fab_req *(*remove_tag)(struct gnix_tag_storage *ts,
			uint64_t tag, uint64_t ignore,
			uint64_t flags, void *context,
			struct gnix_address *addr);
	struct gnix_fab_req *(*peek_tag)(struct gnix_tag_storage *ts,
			uint64_t tag, uint64_t ignore,
			uint64_t flags, void *context,
			struct gnix_address *addr);
	void (*remove_tag_by_req)(struct gnix_tag_storage *ts,
			struct gnix_fab_req *req);
	int (*init)(struct gnix_tag_storage *ts);
	int (*fini)(struct gnix_tag_storage *ts);
	struct gnix_fab_req *(*remove_req_by_context)(struct gnix_tag_storage *ts,
			void *context);
};

/**
 * @note The sequence and generation numbers will be used in the future for
 *       optimizing the search with branch and bound.
 */
struct gnix_tag_list_element {
	 /* entry to the next element in the list */
	struct dlist_entry free;
    /* has element been claimed with FI_CLAIM? */
	int claimed;
    /* associated fi_context with claimed element */
	void *context;
	/* sequence number */
	uint32_t seq;
	/* generation number */
	uint32_t gen;
};

/**
 * @note The type field is based on the GNIX_TAG_* enumerations listed above
 */
struct gnix_tag_storage_attr {
	/* one of 'auto', 'list', 'hlist' or 'kdtree' */
	int type;
	/* should the tag storage check addresses? */
	int use_src_addr_matching;
};

/**
 * @note Unused. This will be used in the future for the init heuristic when
 *         performing auto detection based on the mem_tag_format.
 */
struct gnix_tag_field {
	uint64_t mask;
	uint64_t len;
};

/**
 * @note Unused. This will be used in the future for the init heuristic when
 *         performing auto detection based on the mem_tag_format.
 */
struct gnix_tag_format {
	int field_cnt;
	struct gnix_tag_field *fields;
};

struct gnix_tag_list {
	struct dlist_entry list;
};

struct gnix_hlist_head {
	struct dlist_entry head;
	uint64_t oldest_tag_id;
	uint64_t oldest_gen;
};


struct gnix_tag_hlist {
	struct gnix_hlist_head *array;
	int elements;
	uint64_t last_inserted_id;
	uint64_t oldest_tag_id;
	uint64_t current_gen;
};

struct gnix_tag_kdtree {

};

/**
 * @brief gnix tag storage structure
 *
 * Used to store gnix_fab_requests by tag, and optionally, by address.
 *
 * @var seq         sequence counter for elements
 * @var state       state of the tag storage structure
 * @var gen         generation counter for elements
 * @var match_func  matching function for the tag storage, either posted or
 *                    unexpected
 * @var attr        tag storage attributes
 * @var ops         function dispatch table for underlying data structures
 * @var tag_format  unused. used during init for determining what type of
 *                  data structure to use for storing data
 */
struct gnix_tag_storage {
	ofi_atomic32_t seq;
	int state;
	int gen;
	int (*match_func)(struct dlist_entry *entry, const void *arg);
	struct gnix_tag_storage_attr attr;
	struct gnix_tag_storage_ops *ops;
	struct gnix_tag_format tag_format;
	union {
		struct gnix_tag_list list;
		struct gnix_tag_hlist hlist;
		struct gnix_tag_kdtree kdtree;
	};
};

/* function declarations */
/**
 * @brief generic matching function for posted and unexpected tag storages
 *
 * @param req                     gnix fabric request to match
 * @param tag                     tag to match
 * @param ignore                  bits to ignore in the tags
 * @param flags                   fi_tagged flags
 * @param context                 fi_context to match in request
 * @param uses_src_addr_matching  should we check addresses?
 * @param addr                    gnix address to match
 * @param matching_posted         is matching on a posted tag storage?
 * @return 1 if this request matches the parameters, 0 otherwise
 */
int _gnix_req_matches_params(
		struct gnix_fab_req *req,
		uint64_t tag,
		uint64_t ignore,
		uint64_t flags,
		void *context,
		int use_src_addr_matching,
		struct gnix_address *addr,
		int matching_posted);

/**
 * @brief matching function for unexpected tag storages
 *
 * @param entry  dlist entry pointing to the request to search
 * @param arg    search parameters as a gnix_tag_search_element
 * @return 1 if this request matches the parameters, 0 otherwise
 */
int _gnix_match_unexpected_tag(struct dlist_entry *entry, const void *arg);

/**
 * @brief matching function for posted tag storages
 *
 * @param entry  dlist entry pointing to the request to search
 * @param arg    search parameters as a gnix_tag_search_element
 * @return 1 if this request matches the parameters, 0 otherwise
 */
int _gnix_match_posted_tag(struct dlist_entry *entry, const void *arg);

/**
 * @brief base initialization function for tag storages
 * @note  This function should never be called directly. It is exposed for the
 *        purpose of allowing the test suite to reinitialize tag storages
 *        without knowing what type of tag storage is being reinitialized
 *
 * @param ts          tag storage pointer
 * @param attr        tag storage attributes
 * @param match_func  match function to be used on individual list elements
 * @return -FI_EINVAL, if any invalid parameters were given
 *         FI_SUCCESS, otherwise
 */
int _gnix_tag_storage_init(
		struct gnix_tag_storage *ts,
		struct gnix_tag_storage_attr *attr,
		int (*match_func)(struct dlist_entry *, const void *));

/**
 * @brief initialization function for posted tag storages
 *
 * @param ts          tag storage pointer
 * @param attr        tag storage attributes
 * @param match_func  match function to be used on individual list elements
 * @return -FI_EINVAL, if any invalid parameters were given
 *         FI_SUCCESS, otherwise
 */
static inline int _gnix_posted_tag_storage_init(
		struct gnix_tag_storage *ts,
		struct gnix_tag_storage_attr *attr)
{
	return _gnix_tag_storage_init(ts, attr, _gnix_match_posted_tag);
}

/**
 * @brief initialization function for unexpected tag storages
 *
 * @param ts          tag storage pointer
 * @param attr        tag storage attributes
 * @param match_func  match function to be used on individual list elements
 * @return -FI_EINVAL, if any invalid parameters were given
 *         FI_SUCCESS, otherwise
 */
static inline int _gnix_unexpected_tag_storage_init(
		struct gnix_tag_storage *ts,
		struct gnix_tag_storage_attr *attr)
{
	return _gnix_tag_storage_init(ts, attr, _gnix_match_unexpected_tag);
}

/**
 * @brief destroys a tag storage and releases any held memory
 *
 * @param ts
 * @return -FI_EINVAL, if the tag storage is in a bad state
 *         -FI_EAGAIN, if there are tags remaining in the tag storage
 *         FI_SUCCESS, otherwise
 */
int _gnix_tag_storage_destroy(struct gnix_tag_storage *ts);

/**
 * @brief inserts a gnix_fab_req into the tag storage
 *
 * @param ts           pointer to the tag storage
 * @param tag          tag associated with fab request
 * @param req          gnix fabric request
 * @param ignore       bits to ignore in tag (only applies to posted)
 * @param addr_ignore  bits to ignore in addr (only applies to posted)
 * @return
 *
 * @note if ts is a posted tag storage, 'req->ignore_bits' will be set to
 *         the value of 'ignore'.
 *
 * @note if ts is a posted tag storage and ts->attr.use_src_addr_matching
 *         is enabled, 'req->addr_ignore_bits' will be set to the value
 *         of 'addr_ignore'.
 */
int _gnix_insert_tag(
		struct gnix_tag_storage *ts,
		uint64_t tag,
		struct gnix_fab_req *req,
		uint64_t ignore);


/**
 * @brief matches at a request from the tag storage by tag and address
 *
 * @param ts           pointer to the tag storage
 * @param tag          tag to remove
 * @param ignore       bits to ignore in tag
 * @param flags        fi_tagged flags
 * @param context      fi_context associated with tag
 * @param addr         gnix_address associated with tag
 * @param addr_ignore  bits to ignore in address
 * @return NULL, if no entry found that matches parameters
 *         otherwise, a non-null value pointing to a gnix_fab_req
 *
 * @note ignore parameter is not used for posted tag storages
 * @note addr_ignore parameter is not used for posted tag storages
 * @note if FI_CLAIM is not provided in flags, the call is an implicit removal
 *       of the tag
 * @note When the FI_PEEK flag is not set, the request will be removed
 *       from the tag storage
 */
struct gnix_fab_req *_gnix_match_tag(
		struct gnix_tag_storage *ts,
		uint64_t tag,
		uint64_t ignore,
		uint64_t flags,
		void *context,
		struct gnix_address *addr);

struct gnix_fab_req *_gnix_remove_req_by_context(
		struct gnix_tag_storage *ts,
		void *context);

/**
 * @brief removes a gnix_fab_req from the tag storage list element
 *
 * @param ts           pointer to the tag storage
 * @param req          gnix fabric request
 * @param ignore       bits to ignore in tag (only applies to posted)
 * @return             none
 *
 * @note This is similar to _gnix_match_tag with the FI_PEEK flag not set
 *       but it does not need to search the list to remove the request
 */
void _gnix_remove_tag(
		struct gnix_tag_storage *ts,
		struct gnix_fab_req *req);

/* external symbols */



#endif /* PROV_GNI_SRC_GNIX_TAGS_H_ */
