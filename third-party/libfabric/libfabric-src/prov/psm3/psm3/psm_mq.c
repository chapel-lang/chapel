/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2016 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2016 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2016 Intel Corporation. All rights reserved. */

#include <sched.h>

#include "psm_user.h"
#include "psm2_hal.h"
#include "psm_mq_internal.h"
#include "ips_proto_params.h"

/*
 * Functions to manipulate the expected queue in mq_ep.
 */

#ifdef LEARN_HASH_SELECTOR
// hash the req and then append to specified hash table in given q
PSMI_ALWAYS_INLINE(
void mq_qq_append_which_hash(psm2_mq_t mq,
				struct mqq q[NUM_HASH_CONFIGS][NUM_HASH_BUCKETS],
				int table, psm2_mq_req_t req))
{
	unsigned hashval = hash_src_tag_sel(&mq->table_sel[table],
										req->req_data.peer, &req->req_data.tag)
							% NUM_HASH_BUCKETS;
	mq_qq_append_which(q, table, hashval, req);
}

// find the table with matching table_sel. 
// If none found and have room for a new table, add a new empty table.
// if none found and didn't add a table, return NUM_HASH_CONFIGS and caller
// will need to use the simple linear list
PSMI_ALWAYS_INLINE(
int mq_qq_find_table(psm2_mq_t mq, psm2_epaddr_t src, psm2_mq_tag_t *tagsel))
{
	int t;
	uint8_t srcsel = src != PSM2_MQ_ANY_ADDR;

	// chose to have a loop instead of unrolling loop due to
	// possible better CPU instruction cache hit rate
	// search in the order the tables got created, 1st created may be used more
	for (t = NUM_HASH_CONFIGS-1; t >= mq->min_table; t--) {
		if (table_sel_cmp(&mq->table_sel[t], srcsel, tagsel))
			return t;
	}
	if (mq->min_table > 0) {
		// add a table
		psmi_assert(mq->min_table <= NUM_HASH_CONFIGS);
		_HFI_VDBG("add table_sel, srcsel=%u tagsel=%08x.%08x.%08x\n",
					srcsel, tagsel->tag[0], tagsel->tag[1], tagsel->tag[2]);
		t = --(mq->min_table);
		UPDATE_SUBQUEUE_COUNT(mq);
		build_table_sel(&mq->table_sel[t], srcsel, tagsel);
		return t;
	}
	// out of tables, must use simple linear list
	if (! mq->search_linear_expected) {
		mq->search_linear_expected = 1;
		UPDATE_SUBQUEUE_COUNT(mq);
	}
	return NUM_HASH_CONFIGS;
}
#endif /* LEARN_HASH_SELECTOR */

/*
 * Once the linked lists cross the size limit, this function will enable tag
 * hashing and disable the non-hashing fastpath. We need to go back and insert
 * reqs into the hash tables where the hashing searches will look for them.
 *
 * When called, either the expected or unexpected queue has crossed the
 * mq->hash_thresh.  However it's possible the other queue is empty.
 */
#ifdef LEARN_HASH_SELECTOR
/* If this is the first time we are transitioning out of nohash mode, and
 * the expected queue is currently empty, we will only adjust flags,
 * but won't actually have any table_sel values to use to build hashes until a
 * subsequent psm3_mq_* call provides a src/tagsel. At which point, that call
 * will then trigger the addition of a corresponding table_sel.
 */
#endif
void
psm3_mq_fastpath_disable(psm2_mq_t mq)
{
	psm2_mq_req_t next, cur;
	int t;
#ifndef LEARN_HASH_SELECTOR
	unsigned hashvals[NUM_HASH_CONFIGS];
#endif

	_HFI_VDBG("enable tag hashing: thresh %u unexp %u exp %u\n",
			mq->hash_thresh, mq->unexpected_list_len, mq->expected_list_len);

	mq->nohash_fastpath = 0;
#ifndef LEARN_HASH_SELECTOR
	mq->min_table = PSM2_TAG_SRC;
	psmi_assert(PSM2_ANYTAG_ANYSRC == NUM_HASH_CONFIGS);
#endif

	/* Everything in the expected_q needs to be moved into the
	   (single) correct expected hash table.
	   This will also establish the table_sel's we will use
	   for unexepected queue below */
	for (cur = mq->expected_q.first; cur != NULL; cur = next) {
		/* must read next ptr before remove */
		next = cur->next[NUM_HASH_CONFIGS];
#ifdef LEARN_HASH_SELECTOR
		t = mq_qq_find_table(mq, cur->req_data.peer, &cur->req_data.tagsel);
		if_pf (t == NUM_HASH_CONFIGS)
			continue; /* req must stay in simple linear list */
		mq_qq_append_which_hash(mq, mq->expected_htab, t, cur);
#else
		if ((cur->req_data.tagsel.tag[0] == 0xFFFFFFFF) &&
		    (cur->req_data.tagsel.tag[1] == 0xFFFFFFFF)) {
			/* hash tag0 and tag1 */
			t = PSM2_TAG_SRC;
			hashvals[t] = hash_64(cur->req_data.tag.tag64) % NUM_HASH_BUCKETS;
			mq_qq_append_which(mq->expected_htab,
					   t, hashvals[t], cur);
		} else if (cur->req_data.tagsel.tag[0] == 0xFFFFFFFF) {
			t = PSM2_TAG_ANYSRC;
			hashvals[t] = hash_32(cur->req_data.tag.tag[0]) % NUM_HASH_BUCKETS;
			mq_qq_append_which(mq->expected_htab,
					   t, hashvals[t], cur);
		} else if (cur->req_data.tagsel.tag[1] == 0xFFFFFFFF) {
			t = PSM2_ANYTAG_SRC;
			hashvals[t] = hash_32(cur->req_data.tag.tag[1]) % NUM_HASH_BUCKETS;
			mq_qq_append_which(mq->expected_htab,
					   t, hashvals[t], cur);
		} else
			continue; /* else, req must stay in ANY ANY */
#endif
		mq->expected_list_len--;
		mq->expected_hash_len++;
		mq_qq_remove_which(cur, NUM_HASH_CONFIGS);
	}

	/* Everything in the unexpected_q needs to be duplicated into
	   each of the unexpected hash tables. */
#ifdef LEARN_HASH_SELECTOR
	if (mq->min_table < NUM_HASH_CONFIGS) {
		for (cur = mq->unexpected_q.first; cur != NULL;
			 cur = cur->next[NUM_HASH_CONFIGS]) {
			mq->unexpected_hash_len++;
			for (t = mq->min_table; t < NUM_HASH_CONFIGS; t++) {
				mq_qq_append_which_hash(mq, mq->unexpected_htab, t, cur);
			}
		}
		psmi_assert(mq->unexpected_hash_len == mq->unexpected_list_len);
	}
#else
	for (cur = mq->unexpected_q.first; cur != NULL;
		 cur = cur->next[NUM_HASH_CONFIGS]) {
		mq->unexpected_hash_len++;
		hashvals[PSM2_TAG_SRC] =
			hash_64(cur->req_data.tag.tag64) % NUM_HASH_BUCKETS;
		hashvals[PSM2_TAG_ANYSRC] =
			hash_32(cur->req_data.tag.tag[0]) % NUM_HASH_BUCKETS;
		hashvals[PSM2_ANYTAG_SRC] =
			hash_32(cur->req_data.tag.tag[1]) % NUM_HASH_BUCKETS;
		for (t = PSM2_TAG_SRC; t < PSM2_ANYTAG_ANYSRC; t++)
			mq_qq_append_which(mq->unexpected_htab,
					   t, hashvals[t], cur);
	}
	psmi_assert(mq->unexpected_hash_len == mq->unexpected_list_len);
#endif

	UPDATE_SUBQUEUE_COUNT(mq);
	UPDATE_EXP_HASH_COUNT(mq);
	UPDATE_UNEXP_HASH_COUNT(mq);
}

/* easy threshold to re-enable: if |hash| == 0 && |list| < X
   aggressive threshold: if |hash| + |list| < X
   even easier: if |hash| + |list| == 0
   might be better approach to avoid constant bouncing between modes */
void psm3_mq_fastpath_try_reenable(psm2_mq_t mq)
{
	if_pf(mq->nohash_fastpath == 0 &&
	      mq->unexpected_hash_len == 0 &&
	      mq->expected_hash_len == 0 &&
	      mq->unexpected_list_len == 0 &&
	      mq->expected_list_len == 0 &&
	      mq->hash_thresh > 0) {
		// all unexpected on list and hashes, so total unexp is list (== hash)
		// all expected on list or 1 hash, so total exp is sum
		_HFI_VDBG("re-disable tag hashing: thresh %u unexp %u exp %u\n",
				mq->hash_thresh, mq->unexpected_list_len,
				mq->expected_list_len + mq->expected_hash_len);
		mq->nohash_fastpath = 1;
#if ! defined(LEARN_HASH_SELECTOR) || ! defined(RETAIN_PAST_TABLE_SEL)
		mq->min_table = NUM_HASH_CONFIGS;
#ifdef LEARN_HASH_SELECTOR
		mq->search_linear_expected = 0;
#endif
#endif
	}
}

#ifdef LEARN_HASH_SELECTOR
// populate a new unexpected hash table and while walking unexpected list
// also search for a matching req
// This assumes the real cost is memory references to walk the list
// as opposed to the cost of tag_cmp.
// If tag_cmp is the higher cost, it would be faster to search the appropriate
// unexpected hash table when done building it because the hash would
// reduce number of entries to tag_cmp.
//
// It would be possible to defer the cost of adding and populating the new
// unexpected hash table until we know the unexpected search will fail.
// However doing it now should allow future unexpected queue searches to
// be more efficient, which will be a win, especially if the searches
// need to go deeper into the unexpected queue.
PSMI_ALWAYS_INLINE(
psm2_mq_req_t populate_and_search(psm2_mq_t mq, int table, int srcsel,
								psm2_epaddr_t src,
								psm2_mq_tag_t *tagsel, psm2_mq_tag_t *tag))
{
#ifdef PSM_DEBUG
	unsigned cnt = 0;
#endif
	psm2_mq_req_t match = NULL;
	psm2_mq_req_t cur;
	unsigned k = 0;

	// walk the linear unexpected list to populate hash table and search
	for (cur = mq->unexpected_q.first; cur != NULL;
		 cur = cur->next[NUM_HASH_CONFIGS], k++) {
#ifdef PSM_DEBUG
		cnt++;
#endif
		mq_qq_append_which_hash(mq, mq->unexpected_htab, table, cur);
		if (tag_cmp_req(cur, srcsel, src, tagsel, tag)) {
			match = cur;
			cur = cur->next[NUM_HASH_CONFIGS];
			break;
		}
	}
	// continue populating, but done searching
	for (; cur != NULL; cur = cur->next[NUM_HASH_CONFIGS]) {
#ifdef PSM_DEBUG
		cnt++;
#endif
		mq_qq_append_which_hash(mq, mq->unexpected_htab, table, cur);
	}
#ifdef PSM_DEBUG
	if (table == NUM_HASH_CONFIGS-1) {	// first hash table learned
		psmi_assert(cnt == mq->unexpected_list_len);
		psmi_assert(! mq->unexpected_hash_len);
	}
#endif
	mq->unexpected_hash_len = mq->unexpected_list_len;
	UPDATE_UNEXP_SEARCH_LEN(mq, k);
	return match;
}
#endif /* LEARN_HASH_SELECTOR */

/*
 * ! @brief PSM exposed version to allow PTLs to match
 */

/*! @brief Try to match against the MQ using a tag and tagsel
 *
 * @param[in] mq Message Queue
 * @param[in] src Source (sender) epaddr, may be PSM2_MQ_ANY_ADDR.
 * @param[in] tag Input Tag
 * @param[in] tagsel Input Tag Selector
 * @param[in] remove Non-zero to remove the req from the queue
 *
 * @returns NULL if no match or an mq request if there is a match
 * When NULL is returned, *table will indicate expected hash table to add to
 */
static
psm2_mq_req_t
mq_req_match_with_tagsel(psm2_mq_t mq, psm2_epaddr_t src,
			 psm2_mq_tag_t *tag, psm2_mq_tag_t *tagsel, int remove, int *table)
{
	psm2_mq_req_t cur;
	unsigned hashval;
	int i, j;
	struct mqq *q;
	unsigned k = 0;
	int srcsel = src != PSM2_MQ_ANY_ADDR;
#ifndef LEARN_HASH_SELECTOR
	j = 0;
#endif

	mq->stats.num_unexp_search++;
	if_pt (mq->nohash_fastpath) {
		i = j = NUM_HASH_CONFIGS;
		q = &mq->unexpected_q;
#ifdef LEARN_HASH_SELECTOR
	} else {
		j = mq->min_table;	// 1st valid unexpected subqueue for remove
		psmi_assert(j <= NUM_HASH_CONFIGS && j >= 0);
		i = mq_qq_find_table(mq, src, tagsel);
		psmi_assert(i <= NUM_HASH_CONFIGS && i >= mq->min_table);
		if_pt (i < NUM_HASH_CONFIGS) {
			if_pf (i < j)	{ // added a new table
				/* Everything in the unexpected_q needs to be
				   duplicated into the new unexpected hash table. */
				psmi_assert(i == mq->min_table);
				cur = populate_and_search(mq, i, srcsel, src, tagsel, tag);
				if (cur) {
					j = mq->min_table;	// new min for remove
					goto found;
				}
				// hashval for add to expected table
				mq->hashvals[0] = hash_src_tag_sel(&mq->table_sel[i], src, tag)
							% NUM_HASH_BUCKETS;
				goto notfound;
			}
			// search an existing hash table
			hashval = hash_src_tag_sel(&mq->table_sel[i], src, tag)
							% NUM_HASH_BUCKETS;
			mq->hashvals[0] = hashval;	// for add to expected table
			q = &mq->unexpected_htab[i][hashval];
		} else {
			// couldn't add a table, search the simple linear list
			q = &mq->unexpected_q;
		}
	}
#else /* LEARN_HASH_SELECTOR */
	} else if ((tagsel->tag[0] == 0xFFFFFFFF) &&
		   (tagsel->tag[1] == 0xFFFFFFFF)) {
		i = PSM2_TAG_SRC;
		hashval = hash_64(tag->tag64) % NUM_HASH_BUCKETS;
		mq->hashvals[0] = hashval;	// for add to expected table
		q = &mq->unexpected_htab[i][hashval];
	} else if (tagsel->tag[0] == 0xFFFFFFFF) {
		i = PSM2_TAG_ANYSRC;
		hashval = hash_32(tag->tag[0]) % NUM_HASH_BUCKETS;
		mq->hashvals[0] = hashval;	// for add to expected table
		q = &mq->unexpected_htab[i][hashval];
	} else if (tagsel->tag[1] == 0xFFFFFFFF) {
		i = PSM2_ANYTAG_SRC;
		hashval = hash_32(tag->tag[1]) % NUM_HASH_BUCKETS;
		mq->hashvals[0] = hashval;	// for add to expected table
		q = &mq->unexpected_htab[i][hashval];
	} else {
		/* unhashable tag */
		i = PSM2_ANYTAG_ANYSRC;
		q = &mq->unexpected_q;
	}
#endif /* LEARN_HASH_SELECTOR */
	for (cur = q->first; cur != NULL; cur = cur->next[i], k++) {
		psmi_assert(cur->req_data.peer != PSM2_MQ_ANY_ADDR);
		if (tag_cmp_req(cur, srcsel, src, tagsel, tag))
			goto  found;
	}
#ifdef LEARN_HASH_SELECTOR
notfound:
#endif
	psmi_assert(i <= NUM_HASH_CONFIGS && i >= mq->min_table);
	*table = i;
	UPDATE_UNEXP_SEARCH_LEN(mq, k);
	return NULL;

found:
	if (remove) {
		psmi_assert(j <= NUM_HASH_CONFIGS && j >= mq->min_table);
		mq->unexpected_list_len--;
		if_pf (j < NUM_HASH_CONFIGS) {
			mq->unexpected_hash_len--;
			psmi_assert(mq->unexpected_hash_len == mq->unexpected_list_len);
		}
		for (; j < NUM_HASH_CONFIGS+1; j++)
			mq_qq_remove_which(cur, j);
		psm3_mq_fastpath_try_reenable(mq);
	}
	UPDATE_UNEXP_SEARCH_LEN(mq, k);
	return cur;
}

// must be immediately preceeded by mq_req_match_with_tagsel so
// mq->hashvals[0] is primed when using an expected hashtab
static void mq_add_to_expected_hashes(psm2_mq_t mq, int table, psm2_mq_req_t req)
{
	req->timestamp = mq->timestamp++;
	if_pt (mq->nohash_fastpath) {
		psmi_assert(table == NUM_HASH_CONFIGS);
		mq_qq_append(&mq->expected_q, req);
		mq->expected_list_len++;
		if_pf (mq->expected_list_len > mq->hash_thresh)
			psm3_mq_fastpath_disable(mq);
		UPDATE_EXP_LIST_COUNT(mq);
	} else if (table != NUM_HASH_CONFIGS) {
		mq_qq_append_which(mq->expected_htab, table, mq->hashvals[0], req);
		mq->expected_hash_len++;
		UPDATE_EXP_HASH_COUNT(mq);
	} else {
		// out of hashes, add all others to linear list
		mq_qq_append(&mq->expected_q, req);
		mq->expected_list_len++;
		UPDATE_EXP_LIST_COUNT(mq);
	}
}

/*! @brief Try to remove the req in the MQ
 *
 * @param[in] mq Message Queue
 * @param[in] req MQ request
 *
 * @returns 1 if successfully removed, or 0 if req cannot be found.
 */
static
int mq_req_remove_single(psm2_mq_t mq, psm2_mq_req_t req)
{
	int i;

	/* item should only exist in one expected queue at a time */
#ifdef PSM_DEBUG
	int count=0;
	for (i=mq->min_table; i < NUM_HASH_CONFIGS+1; i++) {
		if (req->q[i])
			count++;
	}
	psmi_assert(count == 1);
#endif

	if (req->q[NUM_HASH_CONFIGS]) {
		mq->expected_list_len--;
		i = NUM_HASH_CONFIGS;
	} else {
		for (i = mq->min_table; i < NUM_HASH_CONFIGS; i++) {
			if (req->q[i]) {
				mq->expected_hash_len--;
				goto found;
			}
		}
		return 0;	// should not happen, verified req is POSTED
	}
found:
	mq_qq_remove_which(req, i);
	psm3_mq_fastpath_try_reenable(mq);
	return 1;
}

PSMI_ALWAYS_INLINE(
psm2_mq_req_t
psm3_mq_iprobe_inner(psm2_mq_t mq, psm2_epaddr_t src,
		     psm2_mq_tag_t *tag,
		     psm2_mq_tag_t *tagsel, int remove_req))
{
	psm2_mq_req_t req;
	int table;

	PSMI_LOCK(mq->progress_lock);
	req = mq_req_match_with_tagsel(mq, src, tag, tagsel, remove_req, &table);

	if (req != NULL) {
		PSMI_UNLOCK(mq->progress_lock);
		return req;
	}

	psm3_poll_internal(mq->ep, 1, 0);
	/* try again */
	req = mq_req_match_with_tagsel(mq, src, tag, tagsel, remove_req, &table);

	PSMI_UNLOCK(mq->progress_lock);
	return req;
}

psm2_error_t
psm3_mq_iprobe2(psm2_mq_t mq, psm2_epaddr_t src,
		 psm2_mq_tag_t *tag, psm2_mq_tag_t *tagsel,
		 psm2_mq_status2_t *status)
{
	psm2_mq_req_t req;

	PSM2_LOG_MSG("entering");
	PSMI_ASSERT_INITIALIZED();

	req = psm3_mq_iprobe_inner(mq, src, tag, tagsel, 0);
	psmi_assert_req_not_internal(req);

	if (req != NULL) {
		if (status != NULL) {
			mq_status2_copy(req, status);
		}
		PSM2_LOG_MSG("leaving");
		return PSM2_OK;
	}
	PSM2_LOG_MSG("leaving");
	return PSM2_MQ_NO_COMPLETIONS;
}

psm2_error_t
psm3_mq_iprobe(psm2_mq_t mq, uint64_t tag, uint64_t tagsel,
		psm2_mq_status_t *status)
{
	psm2_mq_tag_t rtag;
	psm2_mq_tag_t rtagsel;
	psm2_mq_req_t req;

	PSM2_LOG_MSG("entering");
	PSMI_ASSERT_INITIALIZED();

	rtag.tag64 = tag;
#ifdef PSM_DEBUG
	rtag.rem32 = 0;
#endif
	rtagsel.tag64 = tagsel;
	rtagsel.rem32 = 0;

	req = psm3_mq_iprobe_inner(mq, PSM2_MQ_ANY_ADDR, &rtag, &rtagsel, 0);
	psmi_assert_req_not_internal(req);

	if (req != NULL) {
		if (status != NULL) {
			mq_status_copy(req, status);
		}
		PSM2_LOG_MSG("leaving");
		return PSM2_OK;
	}

	PSM2_LOG_MSG("leaving");

	return PSM2_MQ_NO_COMPLETIONS;
}

psm2_error_t
psm3_mq_improbe2(psm2_mq_t mq, psm2_epaddr_t src,
		  psm2_mq_tag_t *tag, psm2_mq_tag_t *tagsel,
		  psm2_mq_req_t *reqo, psm2_mq_status2_t *status)
{
	psm2_mq_req_t req;

	PSM2_LOG_MSG("entering");

	PSMI_ASSERT_INITIALIZED();

	req = psm3_mq_iprobe_inner(mq, src, tag, tagsel, 1);
	if (req != NULL) {
		if (status != NULL) {
			mq_status2_copy(req, status);
		}
		*reqo = req;
		PSM2_LOG_MSG("leaving");
		return PSM2_OK;
	}

	*reqo = NULL;
	PSM2_LOG_MSG("leaving");
	return PSM2_MQ_NO_COMPLETIONS;
}

psm2_error_t
psm3_mq_improbe(psm2_mq_t mq, uint64_t tag, uint64_t tagsel,
		 psm2_mq_req_t *reqo, psm2_mq_status_t *status)
{
	psm2_mq_tag_t rtag;
	psm2_mq_tag_t rtagsel;
	psm2_mq_req_t req;

	PSM2_LOG_MSG("entering");
	PSMI_ASSERT_INITIALIZED();

	rtag.tag64 = tag;
#ifdef PSM_DEBUG
	rtag.rem32 = 0;
#endif
	rtagsel.tag64 = tagsel;
	rtagsel.rem32 = 0;

	req = psm3_mq_iprobe_inner(mq, PSM2_MQ_ANY_ADDR, &rtag, &rtagsel, 1);
	if (req != NULL) {
		if (status != NULL) {
			mq_status_copy(req, status);
		}
		*reqo = req;
		PSM2_LOG_MSG("leaving");
		return PSM2_OK;
	}

	*reqo = NULL;
	PSM2_LOG_MSG("leaving");
	return PSM2_MQ_NO_COMPLETIONS;
}

psm2_error_t psm3_mq_cancel(psm2_mq_req_t *ireq)
{
	psm2_mq_req_t req = *ireq;
	psm2_mq_t mq;
	psm2_error_t err = PSM2_OK;

	PSM2_LOG_MSG("entering");
	PSMI_ASSERT_INITIALIZED();

	if (req == NULL) {
		PSM2_LOG_MSG("leaving");
		return PSM2_MQ_NO_COMPLETIONS;
	}

	/* Cancelling a send is a blocking operation, and expensive.
	 * We only allow cancellation of rendezvous sends, consider the eager sends
	 * as always unsuccessfully cancelled.
	 */
	mq = req->mq;
	PSMI_LOCK(mq->progress_lock);

	if (MQE_TYPE_IS_RECV(req->type)) {
		if (req->state == MQ_STATE_POSTED) {
			int rc;

			rc = mq_req_remove_single(mq, req);
			psmi_assert_always(rc);
			req->state = MQ_STATE_COMPLETE;
			mq_qq_append(&mq->completed_q, req);
			err = PSM2_OK;
		} else
			err = PSM2_MQ_NO_COMPLETIONS;
	} else {
		err = psm3_handle_error(mq->ep, PSM2_PARAM_ERR,
					"Cannot cancel send requests (req=%p)",
					req);
	}

	PSMI_UNLOCK(mq->progress_lock);

	PSM2_LOG_MSG("leaving");

	return err;
}

/* This is the only PSM function that blocks.
 * We handle it in a special manner since we don't know what the user's
 * execution environment is (threads, oversubscribing processes, etc).
 *
 * The status argument can be an instance of either type psm2_mq_status_t or
 * psm2_mq_status2_t.  Depending on the type, a corresponding status copy
 * routine should be passed in.
 */
PSMI_ALWAYS_INLINE(
psm2_error_t
psm3_mq_wait_inner(psm2_mq_req_t *ireq, void *status,
		   psmi_mq_status_copy_t status_copy,
		   int do_lock))
{
	psm2_error_t err = PSM2_OK;

	psm2_mq_req_t req = *ireq;
	if (req == PSM2_MQ_REQINVALID) {
		return PSM2_OK;
	}

	if (do_lock)
		PSMI_LOCK(req->mq->progress_lock);

	if (req->state != MQ_STATE_COMPLETE) {
		psm2_mq_t mq = req->mq;

		/* We'll be waiting on this req, mark it as so */
		req->type |= MQE_TYPE_WAITING;

		_HFI_VDBG("req=%p, buf=%p, len=%d, waiting\n",
			  req, req->req_data.buf, req->req_data.buf_len);

		if (req->testwait_callback) {
			err = req->testwait_callback(ireq);
			if (do_lock)
				PSMI_UNLOCK(req->mq->progress_lock);
			if (status != NULL) {
				status_copy(req, status);
			}
			return err;
		}

		PSMI_BLOCKUNTIL(mq->ep, err, req->state == MQ_STATE_COMPLETE);

		if (err > PSM2_OK_NO_PROGRESS)
			goto fail_with_lock;
		else
			err = PSM2_OK;
	}

	if(!psmi_is_req_internal(req))
		mq_qq_remove(&req->mq->completed_q, req);

	if (status != NULL) {
		status_copy(req, status);
	}

	_HFI_VDBG("req=%p complete, buf=%p, len=%d, err=%d\n",
		  req, req->req_data.buf, req->req_data.buf_len, req->req_data.error_code);

	psm3_mq_req_free_internal(req);
	*ireq = PSM2_MQ_REQINVALID;

fail_with_lock:
	if (do_lock)
		PSMI_UNLOCK(req->mq->progress_lock);
	return err;
}

psm2_error_t
psm3_mq_wait2(psm2_mq_req_t *ireq, psm2_mq_status2_t *status)
{
	psm2_error_t rv;
	PSM2_LOG_MSG("entering");
	PSMI_ASSERT_INITIALIZED();
	psmi_assert_req_not_internal(*ireq);

	rv = psm3_mq_wait_inner(ireq, status,
				  (psmi_mq_status_copy_t) mq_status2_copy, 1);
	PSM2_LOG_MSG("leaving");
	return rv;
}

psm2_error_t
psm3_mq_wait(psm2_mq_req_t *ireq, psm2_mq_status_t *status)
{
	psm2_error_t rv;
	PSM2_LOG_MSG("entering");
	PSMI_ASSERT_INITIALIZED();
	psmi_assert_req_not_internal(*ireq);

	rv = psm3_mq_wait_inner(ireq, status,
				  (psmi_mq_status_copy_t) mq_status_copy, 1);
	PSM2_LOG_MSG("leaving");
	return rv;
}

psm2_error_t psm3_mq_wait_internal(psm2_mq_req_t *ireq)
{
	return psm3_mq_wait_inner(ireq, NULL, NULL, 0);
}

/* The status argument can be an instance of either type psm2_mq_status_t or
 * psm2_mq_status2_t.  Depending on the type, a corresponding status copy
 * routine should be passed in.
 */
PSMI_ALWAYS_INLINE(
psm2_error_t
psm3_mq_test_inner(psm2_mq_req_t *ireq, void *status,
		   psmi_mq_status_copy_t status_copy))
{
	psm2_mq_req_t req = *ireq;
	psm2_error_t err = PSM2_OK;

	PSMI_ASSERT_INITIALIZED();

	if (req == PSM2_MQ_REQINVALID) {
		return PSM2_OK;
	}

	if (req->state != MQ_STATE_COMPLETE) {
		if (req->testwait_callback) {
			PSMI_LOCK(req->mq->progress_lock);
			err = req->testwait_callback(ireq);
			if (status != NULL) {
				status_copy(req, status);
			}
			PSMI_UNLOCK(req->mq->progress_lock);
			return err;
		} else
			return PSM2_MQ_NO_COMPLETIONS;
	}

	if (status != NULL)
		status_copy(req, status);

	_HFI_VDBG
	    ("req=%p complete, tag=%08x.%08x.%08x buf=%p, len=%d, err=%d\n",
	     req, req->req_data.tag.tag[0], req->req_data.tag.tag[1],
	     req->req_data.tag.tag[2], req->req_data.buf,
	     req->req_data.buf_len, req->req_data.error_code);

	PSMI_LOCK(req->mq->progress_lock);
	mq_qq_remove(&req->mq->completed_q, req);
	psm3_mq_req_free_internal(req);
	PSMI_UNLOCK(req->mq->progress_lock);

	*ireq = PSM2_MQ_REQINVALID;

	return err;
}

psm2_error_t
psm3_mq_test2(psm2_mq_req_t *ireq, psm2_mq_status2_t *status)
{
	psm2_error_t rv;
	PSM2_LOG_MSG("entering");
	rv = psm3_mq_test_inner(ireq, status,
				  (psmi_mq_status_copy_t) mq_status2_copy);
	PSM2_LOG_MSG("leaving");
	return rv;
}

psm2_error_t
psm3_mq_test(psm2_mq_req_t *ireq, psm2_mq_status_t *status)
{
	psm2_error_t rv;
	PSM2_LOG_MSG("entering");
	rv = psm3_mq_test_inner(ireq, status,
				  (psmi_mq_status_copy_t) mq_status_copy);
	PSM2_LOG_MSG("leaving");
	return rv;

}

psm2_error_t
psm3_mq_isend2(psm2_mq_t mq, psm2_epaddr_t dest, uint32_t flags,
		psm2_mq_tag_t *stag, const void *buf, uint32_t len,
		void *context, psm2_mq_req_t *req)
{
	psm2_error_t err;

	PSM2_LOG_MSG("entering");

	PSMI_ASSERT_INITIALIZED();
	psmi_assert(stag != NULL);

	PSMI_LOCK(mq->progress_lock);
	err =
		dest->ptlctl->mq_isend(mq, dest, flags, PSMI_REQ_FLAG_NORMAL,
				stag, buf, len, context, req);
	PSMI_UNLOCK(mq->progress_lock);

	psmi_assert(*req != NULL);
	psmi_assert_req_not_internal(*req);

	(*req)->req_data.peer = dest;

	PSM2_LOG_MSG("leaving");

	return err;
}

psm2_error_t
psm3_mq_isend(psm2_mq_t mq, psm2_epaddr_t dest, uint32_t flags, uint64_t stag,
	       const void *buf, uint32_t len, void *context, psm2_mq_req_t *req)
{
	psm2_error_t err;
	psm2_mq_tag_t tag;

	PSM2_LOG_MSG("entering");

	tag.tag64 = stag;
	tag.rem32 = 0;

	PSMI_ASSERT_INITIALIZED();

	PSMI_LOCK(mq->progress_lock);
	err = dest->ptlctl->mq_isend(mq, dest, flags, PSMI_REQ_FLAG_NORMAL,
				&tag, buf, len, context, req);
	PSMI_UNLOCK(mq->progress_lock);

	psmi_assert(*req != NULL);
	psmi_assert_req_not_internal(*req);

	(*req)->req_data.peer = dest;

	PSM2_LOG_MSG("leaving");
	return err;
}

psm2_error_t
psm3_mq_send2(psm2_mq_t mq, psm2_epaddr_t dest, uint32_t flags,
	       psm2_mq_tag_t *stag, const void *buf, uint32_t len)
{
	psm2_error_t err;

	PSM2_LOG_MSG("entering");
	PSMI_ASSERT_INITIALIZED();
	psmi_assert(stag != NULL);

	PSMI_LOCK(mq->progress_lock);
	err = dest->ptlctl->mq_send(mq, dest, flags, stag, buf, len);
	PSMI_UNLOCK(mq->progress_lock);
	PSM2_LOG_MSG("leaving");
	return err;
}

psm2_error_t
psm3_mq_send(psm2_mq_t mq, psm2_epaddr_t dest, uint32_t flags, uint64_t stag,
	      const void *buf, uint32_t len)
{
	psm2_error_t err;
	psm2_mq_tag_t tag;

	PSM2_LOG_MSG("entering stag: 0x%" PRIx64, stag);

	tag.tag64 = stag;
	tag.rem32 = 0;

	PSMI_ASSERT_INITIALIZED();

	PSMI_LOCK(mq->progress_lock);
	err = dest->ptlctl->mq_send(mq, dest, flags, &tag, buf, len);
	PSMI_UNLOCK(mq->progress_lock);
	PSM2_LOG_MSG("leaving");
	return err;
}

/*
 * Common subroutine to psm3_mq_irecv2 and psm3_mq_imrecv.  This code assumes
 * that the provided request has been matched, and begins copying message data
 * that has already arrived to the user's buffer.  Any remaining data is copied
 * by PSM polling until the message is complete.
 * Caller has initialized req->is_buf_gpu_mem and req->user_gpu_buffer
 * consistently with buf/len which represent the application buffer
 * but req->req_data.buf and req->req_data.len still point to the sysbuf
 * where data was landed.
 */
static psm2_error_t
psm3_mq_irecv_inner(psm2_mq_t mq, psm2_mq_req_t req, void *buf, uint32_t len)
{
	uint32_t msglen;

	PSM2_LOG_MSG("entering");
	psmi_assert(MQE_TYPE_IS_RECV(req->type));

	_HFI_VDBG("(req=%p) buf=%p len=%u req.state=%u\n", req, buf, len, req->state);

	switch (req->state) {
	case MQ_STATE_COMPLETE:
		if (req->req_data.buf != NULL) {	/* 0-byte messages don't alloc a sysbuf */
			msglen = mq_set_msglen(req, len, req->req_data.send_msglen);
			psm3_mq_recv_copy(mq, req,
#ifdef PSM_HAVE_GPU
					req->is_buf_gpu_mem,
#endif
					buf, len, msglen);
			psm3_mq_sysbuf_free(mq, req->req_data.buf);
#ifdef PSM_HAVE_GPU
		} else {
			mq->stats.rx_sysbuf_cpu_num++;
#endif
		}
		req->req_data.buf = buf;
		req->req_data.buf_len = len;
		mq_qq_append(&mq->completed_q, req);
		break;

	case MQ_STATE_UNEXP:	/* not done yet */
		msglen = mq_set_msglen(req, len, req->req_data.send_msglen);
		/* Copy What's been received so far and make sure we don't receive
		 * any more than copysz.  After that, swap system with user buffer
		 */
		req->recv_msgoff = min(req->recv_msgoff, msglen);
		psm3_mq_recv_copy(mq, req,
#ifdef PSM_HAVE_GPU
				req->is_buf_gpu_mem,
#endif
				buf, len, req->recv_msgoff);
		psm3_mq_sysbuf_free(mq, req->req_data.buf);

		req->state = MQ_STATE_MATCHED;
		req->req_data.buf = buf;
		req->req_data.buf_len = len;
		break;

	case MQ_STATE_UNEXP_RV:	/* rendez-vous ... */
		msglen = mq_set_msglen(req, len, req->req_data.send_msglen);
		/* Copy What's been received so far and make sure we don't receive
		 * any more than copysz.  After that, swap system with user buffer
		 */
		req->recv_msgoff = min(req->recv_msgoff, msglen);
		if (req->send_msgoff) {	// only have sysbuf if RTS w/payload
			psm3_mq_recv_copy(mq, req,
#ifdef PSM_HAVE_GPU
					req->is_buf_gpu_mem,
#endif
					buf, len, req->recv_msgoff);
			psm3_mq_sysbuf_free(mq, req->req_data.buf);
		}

		req->state = MQ_STATE_MATCHED;
		req->req_data.buf = buf;
		req->req_data.buf_len = len;
		req->rts_callback(req, 0);
		break;

	default:
		fprintf(stderr, "Unexpected state %d in req %p\n", req->state,
			req);
		fprintf(stderr, "type=%d, mq=%p, tag=%08x.%08x.%08x\n",
			req->type, req->mq, req->req_data.tag.tag[0], req->req_data.tag.tag[1],
			req->req_data.tag.tag[2]);
		abort();
	}
	PSM2_LOG_MSG("leaving");
	return PSM2_OK;
}

psm2_error_t
psm3_mq_fp_msg(psm2_ep_t ep, psm2_mq_t mq, psm2_epaddr_t addr, psm2_mq_tag_t *tag,
		psm2_mq_tag_t *tagsel, uint32_t flags, void *buf, uint32_t len,
		void *context, enum psm2_mq_fp_op fp_type, psm2_mq_req_t *req)
{
	psm2_error_t err = PSM2_OK;

	PSM2_LOG_MSG("entering");

	PSMI_ASSERT_INITIALIZED();

	PSMI_LOCK_ASSERT(mq->progress_lock);

	if (fp_type == PSM2_MQ_ISEND_FP) {
		psmi_assert(tag != NULL);
		err =
			addr->ptlctl->mq_isend(mq, addr, flags, PSMI_REQ_FLAG_FASTPATH,
					       tag, buf, len, context, req);

		psmi_assert(*req != NULL);
		psmi_assert_req_not_internal(*req);

		(*req)->req_data.peer = addr;
	} else if (fp_type == PSM2_MQ_IRECV_FP) {
		psm2_mq_req_t recv_req;
		int table;

#ifdef PSM_HAVE_GPU
		int gpu_mem = 0;
		void *gpu_user_buffer = NULL;

		if (len && PSM3_IS_GPU_MEM(buf)) {
			PSM3_GPU_MARK_BUF_SYNCHRONOUS(buf);

			gpu_mem = 1;
			gpu_user_buffer = buf;
		}
#endif

		/* First check unexpected Queue and remove req if found */
		recv_req = mq_req_match_with_tagsel(mq, addr, tag, tagsel, REMOVE_ENTRY, &table);

		if (recv_req == NULL) {
			/* prepost before arrival, add to expected q */
			recv_req = psm3_mq_req_alloc(mq, MQE_TYPE_RECV);
			if_pf(recv_req == NULL) {
				err = PSM2_NO_MEMORY;
				goto recv_ret;
			}

			recv_req->req_data.peer = addr;
			recv_req->req_data.tag = *tag;
			recv_req->req_data.tagsel = *tagsel;
			recv_req->state = MQ_STATE_POSTED;
			recv_req->req_data.buf = buf;
			recv_req->req_data.buf_len = len;
			recv_req->req_data.recv_msglen = len;
			recv_req->recv_msgoff = 0;
			recv_req->req_data.context = context;

#ifdef PSM_HAVE_GPU
			recv_req->is_buf_gpu_mem = gpu_mem;
			recv_req->user_gpu_buffer = gpu_user_buffer;
#endif

			mq_add_to_expected_hashes(mq, table, recv_req);
			_HFI_VDBG("buf=%p,len=%d,tag=%08x.%08x.%08x "
				  " tagsel=%08x.%08x.%08x req=%p\n",
				  buf, len, tag->tag[0], tag->tag[1], tag->tag[2],
				  tagsel->tag[0], tagsel->tag[1], tagsel->tag[2], recv_req);
		} else {
			_HFI_VDBG("unexpected buf=%p,len=%d,tag=%08x.%08x.%08x"
				  " tagsel=%08x.%08x.%08x req=%p\n", buf, len,
				  tag->tag[0], tag->tag[1], tag->tag[2],
				  tagsel->tag[0], tagsel->tag[1], tagsel->tag[2], recv_req);

#ifdef PSM_HAVE_GPU
			recv_req->is_buf_gpu_mem = gpu_mem;
			recv_req->user_gpu_buffer = gpu_user_buffer;
#endif

			recv_req->req_data.context = context;

			psm3_mq_irecv_inner(mq, recv_req, buf, len);
		}
recv_ret:
		psmi_assert_req_not_internal(recv_req);
		*req = recv_req;
	} else {
		err = PSM2_PARAM_ERR;
	}

	PSM2_LOG_MSG("leaving");

	return err;
}

psm2_error_t
psm3_mq_irecv2(psm2_mq_t mq, psm2_epaddr_t src,
		psm2_mq_tag_t *tag, psm2_mq_tag_t *tagsel,
		uint32_t flags, void *buf, uint32_t len, void *context,
		psm2_mq_req_t *reqo)
{
	psm2_error_t err = PSM2_OK;
	psm2_mq_req_t req;
	int table;

#ifdef PSM_HAVE_GPU
	int gpu_mem = 0;

	if (len && PSM3_IS_GPU_MEM(buf)) {
		PSM3_GPU_MARK_BUF_SYNCHRONOUS(buf);

		gpu_mem = 1;
	}
#endif

	PSM2_LOG_MSG("entering");
	PSMI_ASSERT_INITIALIZED();

	PSMI_LOCK(mq->progress_lock);

	/* First check unexpected Queue and remove req if found */
	req = mq_req_match_with_tagsel(mq, src, tag, tagsel, REMOVE_ENTRY, &table);

	if (req == NULL) {
		/* prepost before arrival, add to expected q */
		req = psm3_mq_req_alloc(mq, MQE_TYPE_RECV);
		if_pf(req == NULL) {
			err = PSM2_NO_MEMORY;
			goto ret;
		}

		req->req_data.peer = src;
		req->req_data.tag = *tag;
		req->req_data.tagsel = *tagsel;
		req->state = MQ_STATE_POSTED;
		req->req_data.buf = buf;
		req->req_data.buf_len = len;
		req->req_data.recv_msglen = len;
		req->recv_msgoff = 0;
		req->req_data.context = context;

#ifdef PSM_HAVE_GPU
		req->is_buf_gpu_mem = gpu_mem;
		if (gpu_mem)
			req->user_gpu_buffer = buf;
		else
			req->user_gpu_buffer = NULL;
#endif

		mq_add_to_expected_hashes(mq, table, req);
		_HFI_VDBG("buf=%p,len=%d,tag=%08x.%08x.%08x "
			  " tagsel=%08x.%08x.%08x req=%p\n",
			  buf, len, tag->tag[0], tag->tag[1], tag->tag[2],
			  tagsel->tag[0], tagsel->tag[1], tagsel->tag[2], req);
	} else {
		_HFI_VDBG("unexpected buf=%p,len=%d,tag=%08x.%08x.%08x"
			  " tagsel=%08x.%08x.%08x req=%p\n", buf, len,
			  tag->tag[0], tag->tag[1], tag->tag[2],
			  tagsel->tag[0], tagsel->tag[1], tagsel->tag[2], req);
#ifdef PSM_HAVE_GPU
		req->is_buf_gpu_mem = gpu_mem;
		if (gpu_mem)
			req->user_gpu_buffer = buf;
		else
			req->user_gpu_buffer = NULL;
#endif

		req->req_data.context = context;

		psm3_mq_irecv_inner(mq, req, buf, len);
	}

ret:
	PSMI_UNLOCK(mq->progress_lock);
	psmi_assert_req_not_internal(req);
	*reqo = req;
	PSM2_LOG_MSG("leaving");

	return err;
}

psm2_error_t
psm3_mq_irecv(psm2_mq_t mq, uint64_t tag, uint64_t tagsel, uint32_t flags,
	       void *buf, uint32_t len, void *context, psm2_mq_req_t *reqo)
{
	psm2_error_t rv;
	psm2_mq_tag_t rtag;
	psm2_mq_tag_t rtagsel;

	*reqo = NULL;

	PSM2_LOG_MSG("entering tag: 0x%" PRIx64, tag);

	rtag.tag64 = tag;
#ifdef PSM_DEBUG
	rtag.rem32 = 0;
#endif
	rtagsel.tag64 = tagsel;
	rtagsel.rem32 = 0;
	rv = psm3_mq_irecv2(mq, PSM2_MQ_ANY_ADDR, &rtag, &rtagsel,
			       flags, buf, len, context, reqo);

	psmi_assert_req_not_internal(*reqo);
	PSM2_LOG_MSG("leaving");

	return rv;
}

psm2_error_t
psm3_mq_imrecv(psm2_mq_t mq, uint32_t flags, void *buf, uint32_t len,
		void *context, psm2_mq_req_t *reqo)
{
	psm2_error_t err = PSM2_OK;
	psm2_mq_req_t req = *reqo;

	PSM2_LOG_MSG("entering");
	PSMI_ASSERT_INITIALIZED();

	if (req == PSM2_MQ_REQINVALID) {
		err = psm3_handle_error(mq->ep, PSM2_PARAM_ERR,
					"Invalid request (req=%p)", req);
	} else {
		/* Message is already matched -- begin delivering message data to the
		   user's buffer. */
		req->req_data.context = context;

#ifdef PSM_HAVE_GPU
		if (len && PSM3_IS_GPU_MEM(buf)) {
			PSM3_GPU_MARK_BUF_SYNCHRONOUS(buf);
			req->is_buf_gpu_mem = 1;
			req->user_gpu_buffer = buf;
		} else {
			req->is_buf_gpu_mem = 0;
			req->user_gpu_buffer = NULL;
		}
#endif

		PSMI_LOCK(mq->progress_lock);
		psm3_mq_irecv_inner(mq, req, buf, len);
		PSMI_UNLOCK(mq->progress_lock);
	}

	PSM2_LOG_MSG("leaving");

	return err;
}

/* The status argument can be an instance of either type psm2_mq_status_t or
 * psm2_mq_status2_t.  Depending on the type, a corresponding status copy
 * routine should be passed in.
 */
PSMI_ALWAYS_INLINE(
psm2_error_t
psm3_mq_ipeek_inner(psm2_mq_t mq, psm2_mq_req_t *oreq,
		    void *status,
		    psmi_mq_status_copy_t status_copy))
{
	psm2_mq_req_t req;

	PSMI_ASSERT_INITIALIZED();

	if ((req = mq->completed_q.first) == NULL) {
		PSMI_LOCK(mq->progress_lock);
		psm3_poll_internal(mq->ep, 1, 0);
		if ((req = mq->completed_q.first) == NULL) {
			PSMI_UNLOCK(mq->progress_lock);
			return PSM2_MQ_NO_COMPLETIONS;
		}
		PSMI_UNLOCK(mq->progress_lock);
	}
	/* something in the queue */
	*oreq = req;
	if (status != NULL)
		status_copy(req, status);

	return PSM2_OK;
}

psm2_error_t
psm3_mq_ipeek2(psm2_mq_t mq, psm2_mq_req_t *oreq, psm2_mq_status2_t *status)
{
	psm2_error_t rv;

	*oreq = NULL;

	PSM2_LOG_MSG("entering");
	rv = psm3_mq_ipeek_inner(mq, oreq, status,
				   (psmi_mq_status_copy_t) mq_status2_copy);

	psmi_assert_req_not_internal(*oreq);
	PSM2_LOG_MSG("leaving");
	return rv;
}

psm2_error_t
psm3_mq_ipeek(psm2_mq_t mq, psm2_mq_req_t *oreq, psm2_mq_status_t *status)
{
	psm2_error_t rv;

	*oreq = NULL;
	PSM2_LOG_MSG("entering");
	rv = psm3_mq_ipeek_inner(mq, oreq, status,
				   (psmi_mq_status_copy_t) mq_status_copy);

	psmi_assert_req_not_internal(*oreq);
	PSM2_LOG_MSG("leaving");
	return rv;
}

psm2_error_t psm3_mq_ipeek_dequeue_multi(psm2_mq_t mq, void *status_array,
		psmi_mq_status_copy_user_t status_copy, int *count)
{
	psm2_mq_req_t req;
	int read_count = *count;
	int ret = 0;

	PSMI_ASSERT_INITIALIZED();

	*count = 0;
	while (*count < read_count) {
		PSMI_LOCK(mq->progress_lock);

		if (mq->completed_q.first == NULL)
			psm3_poll_internal(mq->ep, 1, 0);

		if ((req = mq->completed_q.first) == NULL) {
			PSMI_UNLOCK(mq->progress_lock);
			return PSM2_MQ_NO_COMPLETIONS;
		}

		mq_qq_remove(&mq->completed_q, req);
		PSMI_UNLOCK(mq->progress_lock);

		ret = status_copy(&req->req_data, status_array, *count);
		psm3_mq_req_free(mq, req);

		if (unlikely(ret < 0)) {
			*count = ret;
			return PSM2_INTERNAL_ERR;
		} else if (ret == 0) {
			continue;
		}

		*count = *count + 1;

		if (ret > 1)
			break;
	}
	return PSM2_OK;
}

psm2_error_t psm3_mq_ipeek_dequeue(psm2_mq_t mq, psm2_mq_req_t *oreq)
{
	psm2_mq_req_t req;

	PSMI_ASSERT_INITIALIZED();
	PSMI_LOCK(mq->progress_lock);
	if (mq->completed_q.first == NULL)
		psm3_poll_internal(mq->ep, 1, 0);
	if ((req = mq->completed_q.first) == NULL) {
		PSMI_UNLOCK(mq->progress_lock);
		return PSM2_MQ_NO_COMPLETIONS;
	}
	mq_qq_remove(&mq->completed_q, req);
	PSMI_UNLOCK(mq->progress_lock);
	*oreq = req;
	return PSM2_OK;
}

psm2_error_t psm3_mq_req_free(psm2_mq_t mq, psm2_mq_req_t req)
{
	PSMI_ASSERT_INITIALIZED();
	if (req == NULL)
		return PSM2_OK;
	PSMI_LOCK(mq->progress_lock);
	psm3_mq_req_free_internal(req);
	PSMI_UNLOCK(mq->progress_lock);

	return PSM2_OK;
}

static
psm2_error_t psm3_mqopt_ctl(psm2_mq_t mq, uint32_t key, void *value, int get)
{
	psm2_error_t err = PSM2_OK;
	uint32_t val32;

	switch (key) {
	case PSM2_MQ_RNDV_HFI_SZ:
		if (get)
			*((uint32_t *) value) = mq->rndv_nic_thresh;
		else {
			val32 = *((uint32_t *) value);
			mq->rndv_nic_thresh = val32;
		}
		_HFI_VDBG("RNDV_HFI_SZ = %d (%s)\n",
			  mq->rndv_nic_thresh, get ? "GET" : "SET");
		break;

	case PSM2_MQ_RNDV_SHM_SZ:
		if (get)
			*((uint32_t *) value) = mq->shm_thresh_rv;
		else {
			val32 = *((uint32_t *) value);
			mq->shm_thresh_rv = val32;
		}
		_HFI_VDBG("RNDV_SHM_SZ = %d (%s)\n",
			  mq->shm_thresh_rv, get ? "GET" : "SET");
		break;
#ifdef PSM_HAVE_GPU
	case PSM2_MQ_GPU_RNDV_SHM_SZ:
		if (get)
			*((uint32_t *) value) = mq->shm_gpu_thresh_rv;
		else {
			val32 = *((uint32_t *) value);
			mq->shm_gpu_thresh_rv = val32;
		}
		_HFI_VDBG("RNDV_GPU_SHM_SZ = %d (%s)\n",
			  mq->shm_gpu_thresh_rv, get ? "GET" : "SET");
		break;
#endif
	case PSM2_MQ_MAX_SYSBUF_MBYTES:
		/* Deprecated: this option no longer does anything. */
		break;

	default:
		err =
		    psm3_handle_error(NULL, PSM2_PARAM_ERR,
				      "Unknown option key=%u", key);
		break;
	}
	return err;
}

psm2_error_t psm3_mq_getopt(psm2_mq_t mq, int key, void *value)
{
	psm2_error_t rv;
	PSM2_LOG_MSG("entering");
	PSMI_ERR_UNLESS_INITIALIZED(mq->ep);
	rv = psm3_mqopt_ctl(mq, key, value, 1);
	PSM2_LOG_MSG("leaving");
	return rv;
}

psm2_error_t psm3_mq_setopt(psm2_mq_t mq, int key, const void *value)
{
	psm2_error_t rv;
	PSM2_LOG_MSG("entering");
	PSMI_ERR_UNLESS_INITIALIZED(mq->ep);
	rv = psm3_mqopt_ctl(mq, key, (void *)value, 0);
	PSM2_LOG_MSG("leaving");
	return rv;
}

#define TAB_SIZE 16
#define STATS				\
	STAT(rx_user_bytes)		\
	STAT(rx_user_num)		\
	STAT(rx_sys_bytes)		\
	STAT(rx_sys_num)		\
	STAT(tx_num)			\
	STAT(tx_eager_num)		\
	STAT(tx_eager_bytes)		\
	STAT(tx_rndv_num)		\
	STAT(tx_rndv_bytes)		\
	STAT(tx_shm_num)		\
	STAT(rx_shm_num)		\
	STAT(rx_sysbuf_num)		\
	STAT(rx_sysbuf_bytes)	\
	STAT(comm_world_rank)
//	STAT(max_subqueues)
//	STAT(max_exp_list_len)
//	STAT(max_exp_hash_len)
//	STAT(num_exp_search)
//	STAT(tot_exp_search_cmp)
//	STAT(max_exp_search_cmp)
//	STAT(max_unexp_list_len)
//	STAT(max_unexp_hash_len)
//	STAT(num_unexp_search)
//	STAT(tot_unexp_search_cmp)
//	STAT(max_unexp_search__cmp)

static
void
psm3_mq_print_stats(psm2_mq_t mq, FILE *perf_stats_fd)
{
	psm2_mq_stats_t stats;
	char msg_buffer[MSG_BUFFER_LEN];

	psm3_mq_get_stats(sizeof(stats), mq, &stats);

#define STAT(x) \
	snprintf(msg_buffer, MSG_BUFFER_LEN, "%*lu ",TAB_SIZE-1, stats.x); \
	fwrite(msg_buffer, sizeof(char), strlen(msg_buffer), perf_stats_fd);

	STATS

#undef STAT

	fwrite("\n", sizeof(char), 1, perf_stats_fd);
}


static
void
*psm3_mq_print_stats_thread(void *_mq)
{
	psm2_mq_t mq = (psm2_mq_t)_mq;
	char perf_file_name[MSG_BUFFER_LEN];
	char msg_buffer[MSG_BUFFER_LEN];
	int delta_t = 0;

	snprintf(perf_file_name, MSG_BUFFER_LEN, "./psm3-perf-stat-ep-%s-pid-%d",
			psm3_epid_fmt_internal(mq->ep->epid, 0),
			getpid());
	FILE *perf_stats_fd = fopen(perf_file_name, "w+");

	if (!perf_stats_fd)
	{
		_HFI_ERROR("Failed to create fd for performance logging\n");
		goto end;
	}

#define STAT(x) \
	snprintf(msg_buffer, MSG_BUFFER_LEN, "%*s ",TAB_SIZE-1, #x);\
	fwrite(msg_buffer, sizeof(char), strlen(msg_buffer), perf_stats_fd);

	STAT(delta_t)
	STATS

#undef STAT

	fwrite("\n", sizeof(char), 1, perf_stats_fd);

	/* Performance stats will be printed every $PSM3_MQ_PRINT_STATS seconds */
	do {
		snprintf(msg_buffer, MSG_BUFFER_LEN, "%*d ",TAB_SIZE-1, delta_t);
		fwrite(msg_buffer, sizeof(char), strlen(msg_buffer), perf_stats_fd);
		psm3_mq_print_stats(mq, perf_stats_fd);
		fflush(perf_stats_fd);
		usleep(MICRO_SEC * mq->print_stats);
		delta_t += mq->print_stats;
	} while (mq->mq_perf_data.perf_print_stats);

	fclose(perf_stats_fd);
end:
	pthread_exit(NULL);
}

static
void
psm3_mq_print_stats_init(psm2_mq_t mq)
{
	mq->mq_perf_data.perf_print_stats = 1;
	if (pthread_create(&(mq->mq_perf_data.perf_print_thread), NULL,
				psm3_mq_print_stats_thread, (void*)mq))
	{
		mq->mq_perf_data.perf_print_stats = 0;
		_HFI_ERROR("Failed to create logging thread\n");
	}
}

static
void
psm3_mq_print_stats_finalize(psm2_mq_t mq)
{
	if (mq->mq_perf_data.perf_print_stats)
	{
		mq->mq_perf_data.perf_print_stats = 0;
		pthread_join(mq->mq_perf_data.perf_print_thread, NULL);
	}
}

/* parse a list of window_rv:limit values for
 * PSM3_RNDV_NIC_WINDOW and PSM3_GPU_RNDV_NIC_WINDOW
 * format is window:limit,window:limit,window
 * limit value must be increasing, limit for last entry is optional and
 * will be UINT32_MAX even if a value is specified.
 * 0 - successfully parsed, *list points to malloced list
 * -1 - str empty, *list unchanged
 * -2 - syntax error, *list unchanged
 */
static int psm3_mq_parse_window_rv(const char *str,
							size_t errstr_size, char errstr[],
							struct psm3_mq_window_rv_entry **list)
{
#define MAX_WINDOW_STR_LEN 1024
	char temp[MAX_WINDOW_STR_LEN+1];
	char *s;
	char *delim;
	struct psm3_mq_window_rv_entry *ret = NULL;
	int i;
	unsigned int win, limit;
	int skip_limit;

	if (!str || ! *str)
		return -1;

	strncpy(temp, str, MAX_WINDOW_STR_LEN);
	if (temp[MAX_WINDOW_STR_LEN-1] != 0) {
		// string too long
		if (errstr_size)
			snprintf(errstr, errstr_size,
				" Value too long, limit %u characters",
				MAX_WINDOW_STR_LEN-1);
		return -2;
	}

	s = temp;
	i = 0;
	do {
		if (! *s)	// trailing ',' on 2nd or later loop
			break;
		// find end of window field and put in \0 as needed
		delim = strpbrk(s, ":,");
		skip_limit = (!delim || *delim == ',');
		if (delim)
			*delim = '\0';
		// parse window
		if (psm3_parse_str_uint(s, &win, 1, PSM3_MQ_RNDV_NIC_WINDOW_MAX)) {
			if (errstr_size)
				snprintf(errstr, errstr_size, " Invalid window_rv: %s", s);
			goto fail;
		}
		// find next field
		if (delim)
			s = delim+1;
		if (skip_limit) {
			limit = UINT32_MAX;
		} else {
			delim = strpbrk(s, ",");
			if (delim)
				*delim = '\0';
			//parse limit
			if (!strcasecmp(s, "max") || !strcasecmp(s, "maximum")) {
				limit = UINT32_MAX;
			} else {
				if (psm3_parse_str_uint(s, &limit, 1, UINT32_MAX)) {
					if (errstr_size)
						snprintf(errstr, errstr_size, " Invalid limit: %s", s);
					goto fail;
				}
			}
			// find next field
			if (delim)
				s = delim+1;
		}
		if (i && ret[i-1].limit >= limit) {
			if (errstr_size)
				snprintf(errstr, errstr_size, " Limit not increasing: %u", limit);
			goto fail;
		}

		ret = (struct psm3_mq_window_rv_entry*)psmi_realloc(PSMI_EP_NONE,
				UNDEFINED, ret, sizeof(struct psm3_mq_window_rv_entry)*(i+1));
		if (! ret)	// keep scans happy
			return -2;
		ret[i].window_rv = ROUNDUP(win, PSMI_PAGESIZE);
		ret[i].limit = limit;
		i++;
	} while (delim);
	if (! i)
		return -1;
	// force last entry limit to UINT32_MAX so used for all remaining lengths
	ret[i-1].limit = UINT32_MAX;
	if (list)
		*list = ret;
	else
		psmi_free(ret);
	return 0;

fail:
	psmi_free(ret);
	return -2;
}

static int psm3_mq_parse_check_window_rv(int type,
										const union psmi_envvar_val val,
										void * ptr,
										size_t errstr_size, char errstr[])
{
	psmi_assert(type == PSMI_ENVVAR_TYPE_STR);
	return psm3_mq_parse_window_rv(val.e_str, errstr_size, errstr, NULL);
}

PSMI_ALWAYS_INLINE(uint32_t search_window(struct psm3_mq_window_rv_entry *e,
					uint32_t len))
{
	for (; len > e->limit; e++)
		;
	return e->window_rv;
}

// for CPU build, gpu argument ignored, but avoids needing ifdef in callers
uint32_t psm3_mq_max_window_rv(psm2_mq_t mq, int gpu)
{
	// must do search since window_rv may not be increasing (but usually is)
	uint32_t ret = 0;
	struct psm3_mq_window_rv_entry *e;
#ifdef PSM_HAVE_GPU
	if (gpu)
		e = mq->ips_gpu_window_rv;
	else
#endif
		e = mq->ips_cpu_window_rv;
	do {
		ret = max(ret, e->window_rv);
	} while ((e++)->limit < UINT32_MAX);
	return ret;
}

uint32_t psm3_mq_get_window_rv(psm2_mq_req_t req)
{
	if (! req->window_rv) {
#ifdef PSM_HAVE_GPU
		if (req->is_buf_gpu_mem) {
			req->window_rv = search_window(
						req->mq->ips_gpu_window_rv,
						req->req_data.send_msglen);
		} else
#endif	/* PSM_HAVE_GPU */
		req->window_rv = search_window(req->mq->ips_cpu_window_rv,
						req->req_data.send_msglen);
#ifdef PSM_HAVE_GPU
		_HFI_VDBG("Selected Window of %u for %u byte %s msg\n",
			req->window_rv,
			req->req_data.send_msglen,
			req->is_buf_gpu_mem?"GPU":"CPU");
#else
		_HFI_VDBG("Selected Window of %u for %u byte msg\n",
			req->window_rv, req->req_data.send_msglen);
#endif
	}
	return req->window_rv;
}

/*
 * This is the API for the user.  We actually allocate the MQ much earlier, but
 * the user can set options after obtaining an endpoint
 */
psm2_error_t
psm3_mq_init(psm2_ep_t ep, uint64_t ignored,
	      const struct psm2_optkey *opts, int numopts, psm2_mq_t *mqo)
{
	psm2_error_t err = PSM2_OK;

	if (ep == NULL) {
		err = PSM2_PARAM_ERR;
		goto fail;
	}

	psm2_mq_t mq = ep->mq;
	int i;

	PSM2_LOG_MSG("entering");

	PSMI_ERR_UNLESS_INITIALIZED(ep);

	psmi_assert_always(mq != NULL);
	psmi_assert_always(mq->ep != NULL);

	mq->stats.comm_world_rank = psm3_get_myrank();

	/* Process options */
	for (i = 0; err == PSM2_OK && i < numopts; i++)
		err = psm3_mqopt_ctl(mq, opts[i].key, opts[i].value, 0);
	if (err != PSM2_OK)	/* error already handled */
		goto fail;

	/* Initialize the unexpected system buffer allocator */
	psm3_mq_sysbuf_init(mq);
	char buf[128];
	psm3_mq_sysbuf_getinfo(mq, buf, sizeof buf);
	_HFI_VDBG("%s", buf);

	*mqo = mq;

	if (mq->print_stats > 0)
		psm3_mq_print_stats_init(mq);

fail:
	PSM2_LOG_MSG("leaving");
	return err;
}

psm2_error_t psm3_mq_finalize(psm2_mq_t mq)
{
	psm2_error_t rv = PSM2_OK;

	PSM2_LOG_MSG("entering");

	PSMI_ERR_UNLESS_INITIALIZED(mq->ep);

	if (mq->print_stats == -1)
	{
		mq->print_stats = 1;
		psm3_mq_print_stats_init(mq);
	}
	if (mq->print_stats != 0)
		psm3_mq_print_stats_finalize(mq);

	PSM2_LOG_MSG("leaving");
	return rv;
}

// This is intended for use by unit test programs at the psm2 API level only
// the sizeof psm2_mq_stats_t and hence valid values for len may change
// in future releases
void psm3_mq_get_stats(uint32_t len, psm2_mq_t mq, psm2_mq_stats_t *stats)
{
	PSM2_LOG_MSG("entering");
	if (len != sizeof(*stats)) {
		// this also catches older caller where 1st arg was a ptr
		_HFI_ERROR("Incorrect mq_stats_t size\n");
		memset(stats, 0, len);
	} else {
		memcpy(stats, &mq->stats, len);
#if ! defined(LEARN_HASH_SELECTOR) || ! defined(RETAIN_PAST_TABLE_SEL)
		stats->max_subqueues = NUM_HASH_CONFIGS+1 - mq->min_min_table;
#else
		stats->max_subqueues = NUM_HASH_CONFIGS+1
								- (mq->min_table + !mq->search_linear_expected);
#endif
	}
	PSM2_LOG_MSG("leaving");
}

static uint64_t total_bytes_sent(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	return (mq->stats.tx_eager_bytes + mq->stats.tx_rndv_bytes);
}

static uint64_t overall_avg_msg_size_sent(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.tx_num)
		return (mq->stats.tx_eager_bytes + mq->stats.tx_rndv_bytes)
			/ mq->stats.tx_num;
	else
		return 0;
}

static uint64_t eager_avg_msg_size_sent(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.tx_eager_num)
		return mq->stats.tx_eager_bytes / mq->stats.tx_eager_num;
	else
		return 0;
}

static uint64_t rndv_avg_msg_size_sent(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.tx_rndv_num)
		return mq->stats.tx_rndv_bytes / mq->stats.tx_rndv_num;
	else
		return 0;
}

static uint64_t total_count_recv(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	return (mq->stats.rx_user_num + mq->stats.rx_sys_num);
}

static uint64_t total_bytes_recv(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	return (mq->stats.rx_user_bytes + mq->stats.rx_sys_bytes);
}

static uint64_t overall_avg_msg_size_recv(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.rx_user_num + mq->stats.rx_sys_num)
		return (mq->stats.rx_user_bytes + mq->stats.rx_sys_bytes)
			/ (mq->stats.rx_user_num + mq->stats.rx_sys_num);
	else
		return 0;
}

static uint64_t expected_avg_msg_size_recv(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.rx_user_num)
		return mq->stats.rx_user_bytes / mq->stats.rx_user_num;
	else
		return 0;
}

static uint64_t num_subqueues(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->nohash_fastpath)
		return 1;
	else
#ifdef LEARN_HASH_SELECTOR
		return NUM_HASH_CONFIGS+1
					- (mq->min_table + !mq->search_linear_expected);
#else
		return NUM_HASH_CONFIGS+1 - mq->min_table;
#endif
}

static uint64_t max_subqueues(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
#if ! defined(LEARN_HASH_SELECTOR) || ! defined(RETAIN_PAST_TABLE_SEL)
	return NUM_HASH_CONFIGS+1 - mq->min_min_table;
#else
	return NUM_HASH_CONFIGS+1 - (mq->min_table + !mq->search_linear_expected);
#endif
}

static uint64_t expected_list_len(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	return mq->expected_list_len;
}

static uint64_t expected_hash_len(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	return mq->expected_hash_len;
}

static uint64_t avg_exp_search_cmp(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.num_exp_search)
		return (mq->stats.tot_exp_search_cmp / mq->stats.num_exp_search);
	else
		return 0;
}


static uint64_t unexpected_avg_msg_size_recv(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.rx_sys_num)
		return mq->stats.rx_sys_bytes / mq->stats.rx_sys_num;
	else
		return 0;
}

static uint64_t unexpected_list_len(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	return mq->unexpected_list_len;
}

static uint64_t unexpected_hash_len(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	return mq->unexpected_hash_len;
}

static uint64_t avg_unexp_search_cmp(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.num_unexp_search)
		return (mq->stats.tot_unexp_search_cmp / mq->stats.num_unexp_search);
	else
		return 0;
}

static uint64_t sysbuf_avg_size_recv(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.rx_sysbuf_num)
		return mq->stats.rx_sysbuf_bytes / mq->stats.rx_sysbuf_num;
	else
		return 0;
}

static uint64_t shm_avg_msg_size_sent(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.tx_shm_num)
		return mq->stats.tx_shm_bytes / mq->stats.tx_shm_num;
	else
		return 0;
}

static uint64_t shm_avg_msg_size_recv(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.rx_shm_num)
		return mq->stats.rx_shm_bytes / mq->stats.rx_shm_num;
	else
		return 0;
}

#ifdef PSM_DSA
static uint64_t shm_dsa_avg_copy_size_sent(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.dsa_stats[0].dsa_copy)
		return mq->stats.dsa_stats[0].dsa_copy_bytes
			/ mq->stats.dsa_stats[0].dsa_copy;
	else
		return 0;
}

static uint64_t shm_dsa_avg_copy_size_recv(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.dsa_stats[1].dsa_copy)
		return mq->stats.dsa_stats[1].dsa_copy_bytes
			/ mq->stats.dsa_stats[1].dsa_copy;
	else
		return 0;
}
#endif /* PSM_DSA */

#ifdef PSM_HAVE_GPU
static uint64_t gpu_ipc_hit_rate(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.gpu_ipc_cache_miss)	// all entries start with a miss, then get hits
		return((mq->stats.gpu_ipc_cache_hit*100)/(mq->stats.gpu_ipc_cache_miss+mq->stats.gpu_ipc_cache_hit));
	else
		return 0;
}

static uint64_t gpu_ipc_miss_rate(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.gpu_ipc_cache_miss)	// all entries start with a miss, then get hits
		return((mq->stats.gpu_ipc_cache_miss*100)/(mq->stats.gpu_ipc_cache_miss+mq->stats.gpu_ipc_cache_hit));
	else
		return 0;
}
#endif /* PSM_HAVE_GPU */


static uint64_t self_avg_msg_size_sent(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.tx_self_num)
		return mq->stats.tx_self_bytes / mq->stats.tx_self_num;
	else
		return 0;
}

#ifdef PSM_HAVE_GPU
static uint64_t eager_cpu_avg_msg_size_sent(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.tx_eager_cpu_num)
		return mq->stats.tx_eager_cpu_bytes
			/ mq->stats.tx_eager_cpu_num;
	else
		return 0;
}

static uint64_t eager_gpu_avg_msg_size_sent(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.tx_eager_gpu_num)
		return mq->stats.tx_eager_gpu_bytes
			/ mq->stats.tx_eager_gpu_num;
	else
		return 0;
}

static uint64_t sysbuf_cpu_avg_copy_size_recv(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.rx_sysbuf_cpu_num)
		return mq->stats.rx_sysbuf_cpu_bytes
			/ mq->stats.rx_sysbuf_cpu_num;
	else
		return 0;
}

static uint64_t sysbuf_gdrcopy_avg_size_recv(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.rx_sysbuf_gdrcopy_num)
		return mq->stats.rx_sysbuf_gdrcopy_bytes
			/ mq->stats.rx_sysbuf_gdrcopy_num;
	else
		return 0;
}

static uint64_t sysbuf_cuCopy_avg_size_recv(void *context)
{
	psm2_mq_t mq = (psm2_mq_t)context;
	if (mq->stats.rx_sysbuf_cuCopy_num)
		return mq->stats.rx_sysbuf_cuCopy_bytes
				/mq->stats.rx_sysbuf_cuCopy_num;
	else
		return 0;
}
#endif /* PSM_HAVE_GPU */

psm2_error_t psm3_mq_initstats(psm2_mq_t mq, psm2_epid_t epid)
{
	 struct psmi_stats_entry entries[] = {
		PSMI_STATS_DECL("COMM_WORLD_Rank",
				"Global Rank within job for this process",
					MPSPAWN_STATS_REDUCTION_ALL, NULL,
					&mq->stats.comm_world_rank),

		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("Overall message statistics across all "
			"PSM3 protocols (shm, nic, self)"),
		PSMI_STATS_DECLU64("Total_count_sent",
				"Total messages sent",
				&mq->stats.tx_num),
		PSMI_STATS_DECL_FUNC("Total_bytes_sent",
				"Total bytes sent",
				total_bytes_sent),
		PSMI_STATS_DECL_FUNC("Overall_avg_msg_size_sent",
				"Overall average message size sent",
				overall_avg_msg_size_sent),

		PSMI_STATS_DECLU64("Eager_count_sent",
				"Total messages sent using an Eager strategy",
				&mq->stats.tx_eager_num),
		PSMI_STATS_DECLU64("Eager_bytes_sent",
				"Total bytes sent using an Eager strategy",
				&mq->stats.tx_eager_bytes),
		PSMI_STATS_DECL_FUNC("Eager_avg_msg_size_sent",
				"Average message size sent using an Eager strategy",
				eager_avg_msg_size_sent),

		PSMI_STATS_DECLU64("Rendezvous_count_sent",
				"Total messages sent using Rendezvous strategy",
				&mq->stats.tx_rndv_num),
		PSMI_STATS_DECLU64("Rendezvous_bytes_sent",
				"Total bytes sent using Rendezvous strategy",
				&mq->stats.tx_rndv_bytes),
		PSMI_STATS_DECL_FUNC("Rendezvous_avg_msg_size_sent",
				"Average message size sent using Rendezvous strategy",
				rndv_avg_msg_size_sent),

		PSMI_STATS_DECL_FUNC("Total_count_recv",
				"Total messages received",
				total_count_recv),
		PSMI_STATS_DECL_FUNC("Total_bytes_recv",
				"Total bytes received",
				total_bytes_recv),
		PSMI_STATS_DECL_FUNC("Overall_avg_msg_size_recv",
				"Overall average message size received",
				overall_avg_msg_size_recv),

		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("Overall expected queue tag matching statistics "
			"across all PSM3 protocols (shm, nic, self). Newly arriving "
			"messages first search the expected queue and if no match, post to "
			"the unexpected queue. The expected queue has 1 or more subqueues. "
			"The subqueues consist of a list or a list and a set of hash "
			"tables. Each entry on the queue appears on the list or exactly 1 "
			"hash table."),
		PSMI_STATS_DECLU64("Expected_count_recv",
				"Total messages received where receive posted by application prior to arrival",
				&mq->stats.rx_user_num),
		PSMI_STATS_DECLU64("Expected_bytes_recv",
				"Total bytes received where receive posted by application prior to arrival",
				&mq->stats.rx_user_bytes),
		PSMI_STATS_DECL_FUNC("Expected_avg_msg_size_recv",
				"Average message size received where receive posted by application prior to arrival",
				expected_avg_msg_size_recv),
		PSMI_STATS_DECL_FUNC("Expected_subqueues",
				   "Current number of sub-queues used for expected message tag matching",
				   num_subqueues),
		PSMI_STATS_DECL_FUNC("Expected_max_subqueues",
				   "Maximum number of sub-queues used for expected message tag matching",
				   max_subqueues),
		PSMI_STATS_DECL_FUNC("Expected_list_len",
				   "Current number of items on list used for expected message tag matching",
				   expected_list_len),
		PSMI_STATS_DECLU64("Expected_max_list_len",
				   "Maximum number of items on list used for expected message tag matching",
				   &mq->stats.max_exp_list_len),
		PSMI_STATS_DECL_FUNC("Expected_hash_len",
				   "Current number of items on hash tables used for expected message tag matching",
				   expected_hash_len),
		PSMI_STATS_DECLU64("Expected_max_hash_len",
				   "Maximum number of items on hash tables used for expected message tag matching",
				   &mq->stats.max_exp_hash_len),
		PSMI_STATS_DECLU64("Expected_num_search",
				   "Total number of expected queue searches",
				   &mq->stats.num_exp_search),
		PSMI_STATS_DECLU64("Expected_total_search_cmp",
				   "Total entries compared during expected queue searches",
				   &mq->stats.tot_exp_search_cmp),
		PSMI_STATS_DECL_FUNC("Expected_avg_search_cmp",
				   "Average entries compared during expected queue searches",
				   avg_exp_search_cmp),
		PSMI_STATS_DECLU64("Expected_max_search_cmp",
				   "Maximum entries compared during an expected queue search",
				   &mq->stats.max_exp_search_cmp),

		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("Overall unexpected queue tag matching statistics "
			"across all PSM3 protocols (shm, nic, self).  Newly posted "
			"receives first search the unexpected queue and if no match, post "
			"to the expected queue. The unexpected queue has 1 or more "
			"subqueues.  The subqueues consist of a list or a list and a set "
			"of hash tables. Each entry on the queue appears on the list and "
			"all hash tables."),
		PSMI_STATS_DECLU64("Unexpected_count_recv",
				"Total messages received where receive posted by application after arrival",
				&mq->stats.rx_sys_num),
		PSMI_STATS_DECLU64("Unexpected_bytes_recv",
				"Total bytes received where receive posted by application after arrival",
				&mq->stats.rx_sys_bytes),
		PSMI_STATS_DECL_FUNC("Unexpected_avg_msg_size_recv",
				"Average message size received where receive posted by application after arrival",
				unexpected_avg_msg_size_recv),
		PSMI_STATS_DECL_FUNC("Unexpected_subqueues",
				   "Current number of sub-queues used for unexpected message tag matching",
				   num_subqueues),
		PSMI_STATS_DECL_FUNC("Unexpected_max_subqueues",
				   "Maximum number of sub-queues used for unexpected message tag matching",
				   max_subqueues),
		PSMI_STATS_DECL_FUNC("Unexpected_list_len",
				   "Current number of items on list used for unexpected message tag matching",
				   unexpected_list_len),
		PSMI_STATS_DECLU64("Unexpected_max_list_len",
				   "Maximum number of items on list used for unexpected message tag matching",
				   &mq->stats.max_unexp_list_len),
		PSMI_STATS_DECL_FUNC("Unexpected_hash_len",
				   "Current number of items on hash tables used for unexpected message tag matching",
				   unexpected_hash_len),
		PSMI_STATS_DECLU64("Unexpected_max_hash_len",
				   "Maximum number of items on hash tables used for unexpected message tag matching",
				   &mq->stats.max_unexp_hash_len),
		PSMI_STATS_DECLU64("Unexpected_num_search",
				   "Total number of unexpected queue searches",
				   &mq->stats.num_unexp_search),
		PSMI_STATS_DECLU64("Unexpected_total_search_cmp",
				   "Total entries compared during unexpected queue searches",
				   &mq->stats.tot_unexp_search_cmp),
		PSMI_STATS_DECL_FUNC("Unexpected_avg_search_cmp",
				   "Average entries compared during unexpected queue searches",
				   avg_unexp_search_cmp),
		PSMI_STATS_DECLU64("Unexpected_max_search_cmp",
				   "Maximum entries compared during an unexpected queue search",
				   &mq->stats.max_unexp_search_cmp),

		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("Overall receive bounce buffer statistics across "
			"all PSM3 protocols (shm, nic, self)"),
		PSMI_STATS_DECLU64("sysbuf_count_recv",
				"Total packets which used bounce buffers",
				&mq->stats.rx_sysbuf_num),
		PSMI_STATS_DECLU64("sysbuf_bytes_recv",
				"Total received bytes which used bounce buffers",
				&mq->stats.rx_sysbuf_bytes),
		PSMI_STATS_DECL_FUNC("sysbuf_avg_size_recv",
				"Average bounce buffer bytes used",
				sysbuf_avg_size_recv),

		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("Intra-node messages may use the PSM3 shm "
			"mechanism to transfer messages between processes on "
			"the same node. However sometimes layers above PSM3 "
			"may perform these transfers themselves without "
			"involving PSM3."),
		PSMI_STATS_DECLU64("shm_count_sent",
				"Total messages sent using PSM3 shm protocol",
				&mq->stats.tx_shm_num),
		PSMI_STATS_DECLU64("shm_bytes_sent",
				"Total bytes sent using PSM3 shm protocol",
				&mq->stats.tx_shm_bytes),
		PSMI_STATS_DECL_FUNC("shm_avg_msg_size_sent",
				"Average message size sent using PSM3 shm protocol",
				shm_avg_msg_size_sent),

		PSMI_STATS_DECLU64("shm_count_recv",
				"Total messages received using PSM3 shm protocol",
				&mq->stats.rx_shm_num),
		PSMI_STATS_DECLU64("shm_bytes_recv",
				"Total bytes received using PSM3 shm protocol",
				&mq->stats.rx_shm_bytes),
		PSMI_STATS_DECL_FUNC("shm_avg_msg_size_recv",
				"Average message size received using PSM3 shm protocol",
				shm_avg_msg_size_recv),

#ifdef PSM_DSA
		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("For some Intel(r) Xeon(r) processor "
			"models starting with the 4th Generation Intel(r) "
			"Xeon(r) Scalable Processors, the Data Streaming "
			"Accelerator (DSA) may be used to assist the PSM3 shm "
			"data copies between processes. When DSA is used, "
			"larger messages are transfered using multiple DSA "
			"copies"),
		// dsa_stats[0] is send, [1] is recv
		PSMI_STATS_DECLU64("shm_count_dsa_copy_sent",
				"Total DSA send copies",
				&mq->stats.dsa_stats[0].dsa_copy),
		PSMI_STATS_DECLU64("shm_bytes_dsa_copy_sent",
				"Total bytes sent using DSA",
				&mq->stats.dsa_stats[0].dsa_copy_bytes),
		PSMI_STATS_DECL_FUNC("shm_dsa_avg_copy_size_sent",
				"Average DSA send copy size",
				shm_dsa_avg_copy_size_sent),
		PSMI_STATS_DECLU64("shm_dsa_wait_swq_send_ns",
				"Total nanoseconds spent during send waiting for DSA SWQ enqcmd",
				&mq->stats.dsa_stats[0].dsa_swq_wait_ns),
		PSMI_STATS_DECLU64("shm_dsa_no_wait_swq_send",
				"Total DSA send copies with no wait for DSA SWQ enqcmd",
				&mq->stats.dsa_stats[0].dsa_swq_no_wait),
		PSMI_STATS_DECLU64("shm_dsa_wait_send_ns",
				"Total nanoseconds spent during send waiting for DSA completions",
				&mq->stats.dsa_stats[0].dsa_wait_ns),
		PSMI_STATS_DECLU64("shm_dsa_no_wait_send",
				"Total DSA send copies with no wait for DSA completions",
				&mq->stats.dsa_stats[0].dsa_no_wait),
		PSMI_STATS_DECLU64("shm_dsa_page_fault_rd_send",
				"Total memory read DSA page faults during send",
				&mq->stats.dsa_stats[0].dsa_page_fault_rd),
		PSMI_STATS_DECLU64("shm_dsa_page_fault_wr_send",
				"Total memory write DSA page faults during send",
				&mq->stats.dsa_stats[0].dsa_page_fault_wr),
		PSMI_STATS_DECLU64("shm_dsa_error_send",
				"Total DSA send copies which failured for non-page fault error",
				&mq->stats.dsa_stats[0].dsa_error),

		PSMI_STATS_DECLU64("shm_count_dsa_copy_recv",
				"Total DSA receive copies",
				&mq->stats.dsa_stats[1].dsa_copy),
		PSMI_STATS_DECLU64("shm_bytes_dsa_copy_recv",
				"Total bytes received using DSA",
				&mq->stats.dsa_stats[1].dsa_copy_bytes),
		PSMI_STATS_DECL_FUNC("shm_dsa_avg_copy_size_recv",
				"Average DSA receive copy size",
				shm_dsa_avg_copy_size_recv),
		PSMI_STATS_DECLU64("shm_dsa_wait_swq_recv_ns",
				"Total nanoseconds spent during receive waiting for DSA SWQ enqcmd",
				&mq->stats.dsa_stats[0].dsa_swq_wait_ns),
		PSMI_STATS_DECLU64("shm_dsa_no_wait_swq_recv",
				"Total DSA receive copies with no wait for DSA SWQ enqcmd",
				&mq->stats.dsa_stats[0].dsa_swq_no_wait),
		PSMI_STATS_DECLU64("shm_dsa_wait_recv_ns",
				"Total nanoseconds spent during receive waiting for DSA completions",
				&mq->stats.dsa_stats[1].dsa_wait_ns),
		PSMI_STATS_DECLU64("shm_dsa_no_wait_recv",
				"Total DSA receive copies with no wait for DSA completions",
				&mq->stats.dsa_stats[1].dsa_no_wait),
		PSMI_STATS_DECLU64("shm_dsa_page_fault_rd_recv",
				"Total memory read DSA page faults during receive",
				&mq->stats.dsa_stats[1].dsa_page_fault_rd),
		PSMI_STATS_DECLU64("shm_dsa_page_fault_wr_recv",
				"Total memory write DSA page faults during receive",
				&mq->stats.dsa_stats[1].dsa_page_fault_wr),
		PSMI_STATS_DECLU64("shm_dsa_error_recv",
				"Total DSA receive copiess which failured for non-page fault error",
				&mq->stats.dsa_stats[1].dsa_error),
#endif /* PSM_DSA */
#ifdef PSM_HAVE_GPU
		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("Intra-node GPU messages may use GPU IPC Handles "
			"to perform GPU to GPU rendezvous messages directly to and from "
			"GPU application buffers. This may then take advantage of "
			"optimized GPU to GPU HW mechanisms such as "
#ifdef PSM_CUDA
			"nvLink.\n"
#else
			"xeLink.\n"
#endif
			"To further optimize such transfers, a GPU IPC Handle cache is "
			"maintained on the receiver."),
		PSMI_STATS_DECLU64("gpu_ipc_cache_limit",
				"Max IPC Handles allowed in GPU cache",
				&mq->stats.gpu_ipc_cache_limit),
		PSMI_STATS_DECLU64("gpu_ipc_cache_nelems",
				"Current IPC Handles in GPU cache",
				&mq->stats.gpu_ipc_cache_nelems),
		PSMI_STATS_DECLU64("gpu_ipc_cache_max_nelems",
				"Max Observed IPC Handles in GPU cache",
				&mq->stats.gpu_ipc_cache_max_nelems),
		PSMI_STATS_DECLU64("gpu_ipc_hit",
				"Overall Number of hits",
				&mq->stats.gpu_ipc_cache_hit),
		PSMI_STATS_DECL("gpu_ipc_hit_%",
				"Overall Cache hit rate",
				MPSPAWN_STATS_REDUCTION_ALL,
				gpu_ipc_hit_rate, NULL),
		PSMI_STATS_DECLU64("gpu_ipc_miss",
				"Overall Number of misses",
				&mq->stats.gpu_ipc_cache_miss),
		PSMI_STATS_DECL("gpu_ipc_miss_%",
				"Overall Cache miss rate",
				MPSPAWN_STATS_REDUCTION_ALL,
				gpu_ipc_miss_rate, NULL),
		PSMI_STATS_DECLU64("gpu_ipc_evict",
				"Number of entries removed from cache due to no space",
				&mq->stats.gpu_ipc_cache_evict),
		PSMI_STATS_DECLU64("gpu_ipc_remove",
				"Number of entries removed from cache due to being stale",
				&mq->stats.gpu_ipc_cache_remove),
		PSMI_STATS_DECLU64("gpu_ipc_clear",
				"Number of times entire cache was cleared and reset due to error",
				&mq->stats.gpu_ipc_cache_clear),
#endif /* PSM_HAVE_GPU */

		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("The PSM3 self protocol is used in the "
			"rare occasions where a process sends a message to "
			"itself."),
		PSMI_STATS_DECLU64("self_count_sent",
				"Total messages sent using PSM3 self protocol",
				&mq->stats.tx_self_num),
		PSMI_STATS_DECLU64("self_bytes_sent",
				"Total bytes sent using PSM3 self protocol",
				&mq->stats.tx_self_bytes),
		PSMI_STATS_DECL_FUNC("self_avg_msg_size_sent",
				"Average message size sent using PSM3 self protocol",
				self_avg_msg_size_sent),

#ifdef PSM_HAVE_GPU
		// ------------------------------------------------------------
		PSMI_STATS_DECL_HELP("Eager messages may be sent from GPU or "
			"CPU application buffers.\n"
			"When PSM3 receive bounce buffers (sysbuf) are used, the data "
			"will eventually be copied to an application buffer "
			"which may be in CPU or GPU memory.  Copies to GPU "
			"memory may use direct GPU copies (GDR copy) or may "
			"use GPU APIs (cuCopy) to do the copy from the sysbuf "
			"to the application buffer in the GPU."),
		PSMI_STATS_DECLU64("Eager_cpu_count_sent",
				"Total messages sent from a CPU buffer using an Eager strategy",
				&mq->stats.tx_eager_cpu_num),
		PSMI_STATS_DECLU64("Eager_cpu_bytes_sent",
				"Total bytes sent from a CPU buffer using an Eager strategy",
				&mq->stats.tx_eager_cpu_bytes),
		PSMI_STATS_DECL_FUNC("Eager_cpu_avg_msg_size_sent",
				"Average message size sent from a CPU buffer using an Eager strategy",
				eager_cpu_avg_msg_size_sent),

		PSMI_STATS_DECLU64("Eager_gpu_count_sent",
				"Total messages sent from a GPU buffer using an Eager strategy",
				&mq->stats.tx_eager_gpu_num),
		PSMI_STATS_DECLU64("Eager_gpu_bytes_sent",
				"Total bytes sent from a GPU buffer using an Eager strategy",
				&mq->stats.tx_eager_gpu_bytes),
		PSMI_STATS_DECL_FUNC("Eager_gpu_avg_msg_size_sent",
				"Average message size sent from a GPU buffer using an Eager strategy",
				eager_gpu_avg_msg_size_sent),

		PSMI_STATS_DECLU64("sysbuf_cpu_count_recv",
				"Total copies from a receive bounce buffers to a CPU buffer",
				&mq->stats.rx_sysbuf_cpu_num),
		PSMI_STATS_DECLU64("sysbuf_cpu_bytes_recv",
				"Total bytes copied from a receive bounce buffers to a CPU buffer",
				&mq->stats.rx_sysbuf_cpu_bytes),
		PSMI_STATS_DECL_FUNC("sysbuf_cpu_avg_copy_size_recv",
				"Average copy size from a receive bounce buffer to a CPU buffer",
				sysbuf_cpu_avg_copy_size_recv),

		PSMI_STATS_DECLU64("sysbuf_gdrcopy_count_recv",
				"Total GDR copies from a receive bounce buffers to a GPU buffer",
				&mq->stats.rx_sysbuf_gdrcopy_num),
		PSMI_STATS_DECLU64("sysbuf_gdrcopy_bytes_recv",
				"Total GDR bytes copied from a receive bounce buffers to a GPU buffer",
				&mq->stats.rx_sysbuf_gdrcopy_bytes),
		PSMI_STATS_DECL_FUNC("sysbuf_gdrcopy_avg_size_recv",
				"Average GDR copy size from a receive bounce buffer to a GPU buffer",
				sysbuf_gdrcopy_avg_size_recv),


		PSMI_STATS_DECLU64("sysbuf_cuCopy_count_recv",
				"Total gpuCopy from a receive bounce buffers to a GPU buffer",
				&mq->stats.rx_sysbuf_cuCopy_num),
		PSMI_STATS_DECLU64("sysbuf_cuCopy_bytes_recv",
				"Total gpuCopy bytes from a receive bounce buffers to a GPU buffer",
				&mq->stats.rx_sysbuf_cuCopy_bytes),
		PSMI_STATS_DECL_FUNC("sysbuf_cuCopy_avg_size_recv",
				"Average gpuCopy size from a receive bounce buffer to a GPU buffer",
				sysbuf_cuCopy_avg_size_recv),
#endif /* PSM_HAVE_GPU */
	};

	return psm3_stats_register_type("MPI_Statistics_Summary",
		"High Level Message Send and Recv Statistics for an end point "
		"in the process.\n"
		"PSM3 uses various strategies to transfer messages.\n"
		"For smaller messages, Eager mechanisms are used where the "
		"sender immediately sends the message.\n"
		"For larger messages, and some synchronous messages, Rendezvous "
		"is used where the sender first issues a Request to Send (RTS) "
		"and once the receiver has identifed a matching application "
		"receive request, the receiver issues one or more Clear to "
		"Send (CTS). Upon receiving the CTS, the sender transfers the "
		"corresponding message data.\n"
		"If the application posts a receive before the matching message "
		"arrives, it is considered 'expected' and the receive request "
		"must be queued until a matching message arrives.\n"
		"If a message arrives before the receiving application has "
		"posted a receive, it is considered 'unexpected' and the "
		"message must be queued until the application posts a receive. "
		"If the received message was eager or an RTS containing "
		"payload, PSM3 receive bounce buffers (sysbuf) are used to hold the "
		"data as it arrives. For multi-packet messages it's possible "
		"to receive part of the message into bounce buffers and then "
		"when the application posts it's receive, the remainder may "
		"be received into the application buffer.\n",
					PSMI_STATSTYPE_MQ,
					entries,
					PSMI_HOWMANY(entries),
					psm3_epid_fmt_internal(epid, 0), mq, NULL);
}

psm2_error_t psm3_mq_malloc(psm2_mq_t *mqo)
{
	psm2_error_t err = PSM2_OK;

	psm2_mq_t mq =
	    (psm2_mq_t) psmi_calloc(NULL, UNDEFINED, 1, sizeof(struct psm2_mq));
	if (mq == NULL) {
		err = psm3_handle_error(NULL, PSM2_NO_MEMORY,
					"Couldn't allocate memory for mq endpoint");
		goto fail;
	}

	mq->ep = NULL;
	/*mq->unexpected_callback = NULL; */
	mq->memmode = psm3_parse_memmode();

	memset(mq->unexpected_htab, 0,
	       NUM_HASH_CONFIGS * NUM_HASH_BUCKETS * sizeof(struct mqq));
	memset(mq->expected_htab, 0,
	       NUM_HASH_CONFIGS * NUM_HASH_BUCKETS * sizeof(struct mqq));
	memset(&mq->expected_q, 0, sizeof(struct mqq));
	memset(&mq->unexpected_q, 0, sizeof(struct mqq));
	memset(&mq->completed_q, 0, sizeof(struct mqq));
	memset(&mq->outoforder_q, 0, sizeof(struct mqq));
	STAILQ_INIT(&mq->eager_q);


	// shm_thresh_rv is N/A to NIC and HAL, so we set this here and let
	// HAL set the rest of the defaults
	mq->shm_thresh_rv = PSM3_MQ_RNDV_SHM_THRESH;
#ifdef PSM_HAVE_GPU
	mq->shm_gpu_thresh_rv = psm3_gpu_mq_rndv_shm_gpu_thresh_default;
#endif

	psmi_hal_mq_init_defaults(mq);

	memset(&mq->stats, 0, sizeof(psm2_mq_stats_t));
	err = psm3_mq_req_init(mq);
	if (err)
		goto fail;

	*mqo = mq;

	return PSM2_OK;
fail:
	if (mq != NULL)
		psmi_free(mq);
	return err;
}

// parse MQ env variables
// called immediately after psm3_mq_malloc which has zeroed
// fields already
psm2_error_t psm3_mq_initialize_params(psm2_mq_t mq)
{
	union psmi_envvar_val env_hfitiny, env_rvwin, env_hfirv,
		env_shmrv, env_hash, env_stats;
#ifdef PSM_HAVE_GPU
	union psmi_envvar_val env_shmgpurv;
#endif

	// a limit of PSM_MQ_MAX_TINY btyes is hardcoded into the PSM protocol
	psm3_getenv("PSM3_MQ_TINY_NIC_LIMIT",
		    "NIC tiny packet limit (max 8, default 8)",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)mq->hfi_thresh_tiny, &env_hfitiny);
	mq->hfi_thresh_tiny = min(env_hfitiny.e_uint, PSM_MQ_NIC_MAX_TINY);

	psm3_getenv("PSM3_MQ_RNDV_NIC_THRESH",
		    "NIC eager-to-rendezvous switchover",
		    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)mq->rndv_nic_thresh, &env_hfirv);
	mq->rndv_nic_thresh = env_hfirv.e_uint;

#define WINDOW_SYNTAX "Specified as window_size:limit,window_size:limit, ...\nwhere limit is the largest message size the window_size is applicable to.\nThe last window_size in the list will be used for all remaining message\nsizes (eg. its limit is optional and ignored).\nwindow_size must be <= 4194304 and the limit in each entry must be larger\nthan the prior entry."

	// for loopback, no ips so no window_rv
	if (mq->ips_cpu_window_rv_str) {
		int got_depwin = 0;	// using deprecated PSM3_MQ_RNDV_NIC_WINDOW

		// PSM3_RNDV_NIC_WINDOW overrides deprecated PSM3_MQ_RNDV_NIC_WINDOW.
		// only parse PSM3_MQ_RNDV_NIC_WINDOW if used default for
		// PSM3_RNDV_NIC_WINDOW because it was not specified.
		if (psm3_getenv_range("PSM3_RNDV_NIC_WINDOW",
			"List of NIC rendezvous windows sizes for messges to and from a CPU buffer.",
			WINDOW_SYNTAX,
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR,
			(union psmi_envvar_val)(char*)(mq->ips_cpu_window_rv_str),
			(union psmi_envvar_val)NULL, (union psmi_envvar_val)NULL,
			psm3_mq_parse_check_window_rv, NULL, &env_rvwin) > 0) {
			// new syntax is superset of old
			got_depwin = (0 == psm3_getenv_range("PSM3_MQ_RNDV_NIC_WINDOW",
					"[Deprecated, use PSM3_RNDV_NIC_WINDOW and PSM3_GPU_RNDV_NIC_WINDOW]",
					"NIC rendezvous window size, max 4194304",
					PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_STR,
					(union psmi_envvar_val)(char*)(mq->ips_cpu_window_rv_str),
					(union psmi_envvar_val)NULL, (union psmi_envvar_val)NULL,
					psm3_mq_parse_check_window_rv, NULL, &env_rvwin));
		}
		if (psm3_mq_parse_window_rv(env_rvwin.e_str, 0, NULL,
								 &mq->ips_cpu_window_rv) < 0) {
			// already checked, shouldn't get parse errors nor empty strings
			psmi_assert(0);
		}
#ifdef PSM_HAVE_GPU
		if (mq->ips_gpu_window_rv_str) {
			union psmi_envvar_val env_gpurvwin;
			char *env;

			env =  psm3_env_get("PSM3_GPU_RNDV_NIC_WINDOW");
			if (env && *env)
				got_depwin = 0;	// use new default as default
			// PSM3_GPU_RNDV_NIC_WINDOW overrides deprecated
			// PSM3_MQ_RNDV_NIC_WINDOW.
			// If PSM3_GPU_RNDV_NIC_WINDOW not specified and user specified
			// PSM3_MQ_RNDV_NIC_WINDOW, use it for GPU too.
			(void)psm3_getenv_range("PSM3_GPU_RNDV_NIC_WINDOW",
					"List of NIC rendezvous windows sizes for messages to or from a GPU buffer.",
					WINDOW_SYNTAX,
					PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR,
					got_depwin?env_rvwin:
					  (union psmi_envvar_val)(char*)(mq->ips_gpu_window_rv_str),
					(union psmi_envvar_val)NULL, (union psmi_envvar_val)NULL,
					psm3_mq_parse_check_window_rv, NULL, &env_gpurvwin);
			if (psm3_mq_parse_window_rv(env_gpurvwin.e_str, 0, NULL,
								 &mq->ips_gpu_window_rv)< 0) {
				// already checked, shouldn't get parse errors nor empty strings
				psmi_assert(0);
			}
		}
#else
		(void)got_depwin;	// keep compiler happy
#endif /* PSM_HAVE_GPU */
	}

	psm3_getenv("PSM3_MQ_RNDV_SHM_THRESH",
		    "shm eager-to-rendezvous switchover",
		    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)mq->shm_thresh_rv, &env_shmrv);
	mq->shm_thresh_rv = env_shmrv.e_uint;

#ifdef PSM_HAVE_GPU
	if (PSM3_GPU_IS_ENABLED) {
		psm3_getenv("PSM3_MQ_RNDV_SHM_GPU_THRESH",
			"shm eager-to-rendezvous switchover for GPU send",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
			(union psmi_envvar_val)mq->shm_gpu_thresh_rv, &env_shmgpurv);
		mq->shm_gpu_thresh_rv = env_shmgpurv.e_uint;
	}
#endif

	psm3_getenv("PSM3_MQ_HASH_THRESH",
		    "linear list to hash tag matching switchover",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)DEFAULT_HASH_THRESH, &env_hash);
	mq->hash_thresh = env_hash.e_uint;

	psm3_getenv("PSM3_MQ_PRINT_STATS",
		    "Prints MQ performance stats every n seconds to file "
			"./psm3-perf-stat-ep-[epid]-pid-[pid] when set to -1 stats are "
			"printed only once during finalization",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val) 0, &env_stats);
	mq->print_stats = env_stats.e_uint;

	mq->nohash_fastpath = 1;
	mq->min_table = NUM_HASH_CONFIGS;
#ifdef LEARN_HASH_SELECTOR
	mq->search_linear_expected = 0;
#endif
#if ! defined(LEARN_HASH_SELECTOR) || ! defined(RETAIN_PAST_TABLE_SEL)
	//mq->min_min_table = mq->min_table + !mq->search_linear_expected;
	mq->min_min_table = NUM_HASH_CONFIGS + 1;
#endif
	return PSM2_OK;
}

psm2_error_t MOCKABLE(psm3_mq_free)(psm2_mq_t mq)
{
	psm3_mq_req_fini(mq);
	psm3_mq_sysbuf_fini(mq);
	psm3_stats_deregister_type(PSMI_STATSTYPE_MQ, mq);
#ifdef PSM_HAVE_GPU
	psmi_free(mq->ips_gpu_window_rv);
#endif
	psmi_free(mq->ips_cpu_window_rv);
	psmi_free(mq);
	return PSM2_OK;
}
MOCK_DEF_EPILOGUE(psm3_mq_free);
