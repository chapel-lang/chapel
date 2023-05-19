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

/*
 * Once the linked lists cross the size limit, this function will enable tag
 * hashing and disable the non-hashing fastpath. We need to go back and insert
 * reqs into the hash tables where the hashing searches will look for them.
 */
void
psm3_mq_fastpath_disable(psm2_mq_t mq)
{
	psm2_mq_req_t *curp, cur;
	struct mqq *qp;
	unsigned hashvals[NUM_HASH_CONFIGS];
	int t = PSM2_ANYTAG_ANYSRC;

	mq->nohash_fastpath = 0;
	/* Everything in the unexpected_q needs to be duplicated into
	   each of the (three) unexpected hash tables. */
	qp = &mq->unexpected_q;
	for (curp = &qp->first; (cur = *curp) != NULL; curp = &cur->next[t]) {
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

	/* Everything in the expected_q needs to be moved into the
	   (single) correct expected hash table. */
	qp = &mq->expected_q;
	for (curp = &qp->first; (cur = *curp) != NULL; /*curp = &cur->next*/) {
		/* must read next ptr before remove */
		curp = &cur->next[PSM2_ANYTAG_ANYSRC];
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

		mq->expected_list_len--;
		mq->expected_hash_len++;
		mq_qq_remove_which(cur, PSM2_ANYTAG_ANYSRC);
	}
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
	      mq->expected_list_len == 0){
		mq->nohash_fastpath = 1;
	}
}

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
 */
static
psm2_mq_req_t
mq_req_match_with_tagsel(psm2_mq_t mq, psm2_epaddr_t src,
			 psm2_mq_tag_t *tag, psm2_mq_tag_t *tagsel, int remove)
{
	psm2_mq_req_t *curp;
	psm2_mq_req_t cur;
	unsigned hashval;
	int i, j = 0;
	struct mqq *qp;

	if_pt (mq->nohash_fastpath) {
		i = j = PSM2_ANYTAG_ANYSRC;
		qp = &mq->unexpected_q;
	} else if ((tagsel->tag[0] == 0xFFFFFFFF) &&
		   (tagsel->tag[1] == 0xFFFFFFFF)) {
		i = PSM2_TAG_SRC;
		hashval = hash_64(tag->tag64) % NUM_HASH_BUCKETS;
		qp = &mq->unexpected_htab[i][hashval];
	} else if (tagsel->tag[0] == 0xFFFFFFFF) {
		i = PSM2_TAG_ANYSRC;
		hashval = hash_32(tag->tag[0]) % NUM_HASH_BUCKETS;
		qp = &mq->unexpected_htab[i][hashval];
	} else if (tagsel->tag[1] == 0xFFFFFFFF) {
		i = PSM2_ANYTAG_SRC;
		hashval = hash_32(tag->tag[1]) % NUM_HASH_BUCKETS;
		qp = &mq->unexpected_htab[i][hashval];
	} else {
		/* unhashable tag */
		i = PSM2_ANYTAG_ANYSRC;
		qp = &mq->unexpected_q;
	}

	for (curp = &qp->first; (cur = *curp) != NULL; curp = &cur->next[i]) {
		psmi_assert(cur->req_data.peer != PSM2_MQ_ANY_ADDR);
		if ((src == PSM2_MQ_ANY_ADDR || src == cur->req_data.peer) &&
		    !((tag->tag[0] ^ cur->req_data.tag.tag[0]) & tagsel->tag[0]) &&
		    !((tag->tag[1] ^ cur->req_data.tag.tag[1]) & tagsel->tag[1]) &&
		    !((tag->tag[2] ^ cur->req_data.tag.tag[2]) & tagsel->tag[2])) {
			/* match! */
			if (remove) {
				if_pt (i == PSM2_ANYTAG_ANYSRC)
					mq->unexpected_list_len--;
				else
					mq->unexpected_hash_len--;
				for (; j < NUM_MQ_SUBLISTS; j++)
					mq_qq_remove_which(cur, j);
				psm3_mq_fastpath_try_reenable(mq);
			}
			return cur;
		}
	}
	return NULL;
}

static void mq_add_to_expected_hashes(psm2_mq_t mq, psm2_mq_req_t req)
{
	unsigned hashval;
	int i;

	req->timestamp = mq->timestamp++;
	if_pt (mq->nohash_fastpath) {
		mq_qq_append(&mq->expected_q, req);
		req->q[PSM2_ANYTAG_ANYSRC] = &mq->expected_q;
		mq->expected_list_len++;
		if_pf (mq->expected_list_len >= HASH_THRESHOLD)
			psm3_mq_fastpath_disable(mq);
	} else if ((req->req_data.tagsel.tag[0] == 0xFFFFFFFF) &&
		   (req->req_data.tagsel.tag[1] == 0xFFFFFFFF)) {
		i = PSM2_TAG_SRC;
		hashval = hash_64(req->req_data.tag.tag64) % NUM_HASH_BUCKETS;
		mq_qq_append_which(mq->expected_htab, i, hashval, req);
		mq->expected_hash_len++;
	} else if (req->req_data.tagsel.tag[0] == 0xFFFFFFFF) {
		i = PSM2_TAG_ANYSRC;
		hashval = hash_32(req->req_data.tag.tag[0]) % NUM_HASH_BUCKETS;
		mq_qq_append_which(mq->expected_htab, i, hashval, req);
		mq->expected_hash_len++;
	} else if (req->req_data.tagsel.tag[1] == 0xFFFFFFFF) {
		i = PSM2_ANYTAG_SRC;
		hashval = hash_32(req->req_data.tag.tag[1]) % NUM_HASH_BUCKETS;
		mq_qq_append_which(mq->expected_htab, i, hashval, req);
		mq->expected_hash_len++;
	} else {
		mq_qq_append(&mq->expected_q, req);
		req->q[PSM2_ANYTAG_ANYSRC] = &mq->expected_q;
		mq->expected_list_len++;
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
	psmi_assert((!!req->q[0] + !!req->q[1] + !!req->q[2] + !!req->q[3]) == 1);

	for (i = 0; i < NUM_MQ_SUBLISTS; i++)
		if (req->q[i]) /* found */
			break;
	switch (i) {
	case PSM2_ANYTAG_ANYSRC:
		mq->expected_list_len--;
		break;
	case PSM2_TAG_SRC:
	case PSM2_TAG_ANYSRC:
	case PSM2_ANYTAG_SRC:
		mq->expected_hash_len--;
		break;
	default:
		return 0;
	}

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

	PSMI_LOCK(mq->progress_lock);
	req = mq_req_match_with_tagsel(mq, src, tag, tagsel, remove_req);

	if (req != NULL) {
		PSMI_UNLOCK(mq->progress_lock);
		return req;
	}

	psm3_poll_internal(mq->ep, 1, 0);
	/* try again */
	req = mq_req_match_with_tagsel(mq, src, tag, tagsel, remove_req);

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
	rtag.tag[2] = 0;
#endif
	rtagsel.tag64 = tagsel;
	rtagsel.tag[2] = 0;

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
	rtag.tag[2] = 0;
#endif
	rtagsel.tag64 = tagsel;
	rtagsel.tag[2] = 0;

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
	tag.tag[2] = 0;

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
	tag.tag[2] = 0;

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
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
					req->is_buf_gpu_mem,
#endif
					buf, len, msglen);
			psm3_mq_sysbuf_free(mq, req->req_data.buf);
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
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
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
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
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
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

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		int gpu_mem = 0;
		void *gpu_user_buffer = NULL;

		if (len && PSMI_IS_GPU_ENABLED && PSMI_IS_GPU_MEM(buf)) {
			PSM3_MARK_BUF_SYNCHRONOUS(buf);

			gpu_mem = 1;
			gpu_user_buffer = buf;
		}
#endif

		/* First check unexpected Queue and remove req if found */
		recv_req = mq_req_match_with_tagsel(mq, addr, tag, tagsel, REMOVE_ENTRY);

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

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
			recv_req->is_buf_gpu_mem = gpu_mem;
			recv_req->user_gpu_buffer = gpu_user_buffer;
#endif

			mq_add_to_expected_hashes(mq, recv_req);
			_HFI_VDBG("buf=%p,len=%d,tag=%08x.%08x.%08x "
				  " tagsel=%08x.%08x.%08x req=%p\n",
				  buf, len, tag->tag[0], tag->tag[1], tag->tag[2],
				  tagsel->tag[0], tagsel->tag[1], tagsel->tag[2], recv_req);
		} else {
			_HFI_VDBG("unexpected buf=%p,len=%d,tag=%08x.%08x.%08x"
				  " tagsel=%08x.%08x.%08x req=%p\n", buf, len,
				  tag->tag[0], tag->tag[1], tag->tag[2],
				  tagsel->tag[0], tagsel->tag[1], tagsel->tag[2], recv_req);

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
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

#if defined(PSM_CUDA) || defined(PSM_ONEAPI) 
	int gpu_mem = 0;

	if (len && PSMI_IS_GPU_ENABLED && PSMI_IS_GPU_MEM(buf)) {
		PSM3_MARK_BUF_SYNCHRONOUS(buf);

		gpu_mem = 1;
	}
#endif

	PSM2_LOG_MSG("entering");
	PSMI_ASSERT_INITIALIZED();

	PSMI_LOCK(mq->progress_lock);

	/* First check unexpected Queue and remove req if found */
	req = mq_req_match_with_tagsel(mq, src, tag, tagsel, REMOVE_ENTRY);

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

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		req->is_buf_gpu_mem = gpu_mem;
		if (gpu_mem)
			req->user_gpu_buffer = buf;
		else
			req->user_gpu_buffer = NULL;
#endif

		mq_add_to_expected_hashes(mq, req);
		_HFI_VDBG("buf=%p,len=%d,tag=%08x.%08x.%08x "
			  " tagsel=%08x.%08x.%08x req=%p\n",
			  buf, len, tag->tag[0], tag->tag[1], tag->tag[2],
			  tagsel->tag[0], tagsel->tag[1], tagsel->tag[2], req);
	} else {
		_HFI_VDBG("unexpected buf=%p,len=%d,tag=%08x.%08x.%08x"
			  " tagsel=%08x.%08x.%08x req=%p\n", buf, len,
			  tag->tag[0], tag->tag[1], tag->tag[2],
			  tagsel->tag[0], tagsel->tag[1], tagsel->tag[2], req);
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
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
	rtag.tag[2] = 0;
#endif
	rtagsel.tag64 = tagsel;
	rtagsel.tag[2] = 0;
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

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		if (len && PSMI_IS_GPU_ENABLED && PSMI_IS_GPU_MEM(buf)) {
			PSM3_MARK_BUF_SYNCHRONOUS(buf);
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
			*((uint32_t *) value) = mq->hfi_thresh_rv;
		else {
			val32 = *((uint32_t *) value);
			mq->hfi_thresh_rv = val32;
		}
		_HFI_VDBG("RNDV_HFI_SZ = %d (%s)\n",
			  mq->hfi_thresh_rv, get ? "GET" : "SET");
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
	STAT(rx_sysbuf_bytes)		\
	STAT(comm_world_rank)

static
void
psm3_mq_print_stats(psm2_mq_t mq, FILE *perf_stats_fd)
{
	psm2_mq_stats_t stats;
	char msg_buffer[MSG_BUFFER_LEN];

	psm3_mq_get_stats(sizeof(stats), mq, &stats);

#define STAT(x) \
	snprintf(msg_buffer, MSG_BUFFER_LEN, "%*lu",TAB_SIZE, stats.x); \
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
	snprintf(msg_buffer, MSG_BUFFER_LEN, "%*s",TAB_SIZE, #x);\
	fwrite(msg_buffer, sizeof(char), strlen(msg_buffer), perf_stats_fd);

	STAT(delta_t)
	STATS

#undef STAT

	fwrite("\n", sizeof(char), 1, perf_stats_fd);

	/* Performance stats will be printed every $PSM3_MQ_PRINT_STATS seconds */
	do {
		snprintf(msg_buffer, MSG_BUFFER_LEN, "%*d",TAB_SIZE, delta_t);
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
	}
	PSM2_LOG_MSG("leaving");
}

psm2_error_t psm3_mq_initstats(psm2_mq_t mq, psm2_epid_t epid)
{
	 struct psmi_stats_entry entries[] = {
		PSMI_STATS_DECL("COMM_WORLD_Rank",
					MPSPAWN_STATS_REDUCTION_ALL, NULL,
					&mq->stats.comm_world_rank),
		PSMI_STATS_DECLU64("Total_count_sent", &mq->stats.tx_num),
		PSMI_STATS_DECLU64("Eager_count_sent", &mq->stats.tx_eager_num),
		PSMI_STATS_DECLU64("Eager_bytes_sent", &mq->stats.tx_eager_bytes),
		PSMI_STATS_DECLU64("Rendezvous_count_sent", &mq->stats.tx_rndv_num),
		PSMI_STATS_DECLU64("Rendezvous_bytes_sent", &mq->stats.tx_rndv_bytes),
		PSMI_STATS_DECLU64("Expected_count_recv", &mq->stats.rx_user_num),
		PSMI_STATS_DECLU64("Expected_bytes_recv", &mq->stats.rx_user_bytes),
		PSMI_STATS_DECLU64("Unexpected_count_recv", &mq->stats.rx_sys_num),
		PSMI_STATS_DECLU64("Unexpected_bytes_recv", &mq->stats.rx_sys_bytes),
		PSMI_STATS_DECLU64("shm_count_sent", &mq->stats.tx_shm_num),
		PSMI_STATS_DECLU64("shm_bytes_sent", &mq->stats.tx_shm_bytes),
		PSMI_STATS_DECLU64("shm_count_recv", &mq->stats.rx_shm_num),
		PSMI_STATS_DECLU64("shm_bytes_recv", &mq->stats.rx_shm_bytes),
#ifdef PSM_DSA
		PSMI_STATS_DECLU64("shm_count_dsa_copy_sent", &mq->stats.dsa_stats[0].dsa_copy),
		PSMI_STATS_DECLU64("shm_bytes_dsa_copy_sent", &mq->stats.dsa_stats[0].dsa_copy_bytes),
		PSMI_STATS_DECLU64("shm_dsa_wait_send_ns", &mq->stats.dsa_stats[0].dsa_wait_ns),
		PSMI_STATS_DECLU64("shm_dsa_no_wait_send", &mq->stats.dsa_stats[0].dsa_no_wait),
		PSMI_STATS_DECLU64("shm_dsa_page_fault_rd_send", &mq->stats.dsa_stats[0].dsa_page_fault_rd),
		PSMI_STATS_DECLU64("shm_dsa_page_fault_wr_send", &mq->stats.dsa_stats[0].dsa_page_fault_wr),
		PSMI_STATS_DECLU64("shm_dsa_error_send", &mq->stats.dsa_stats[0].dsa_error),

		PSMI_STATS_DECLU64("shm_count_dsa_copy_recv", &mq->stats.dsa_stats[1].dsa_copy),
		PSMI_STATS_DECLU64("shm_bytes_dsa_copy_recv", &mq->stats.dsa_stats[1].dsa_copy_bytes),
		PSMI_STATS_DECLU64("shm_dsa_wait_recv_ns", &mq->stats.dsa_stats[1].dsa_wait_ns),
		PSMI_STATS_DECLU64("shm_dsa_no_wait_recv_ns", &mq->stats.dsa_stats[1].dsa_no_wait),
		PSMI_STATS_DECLU64("shm_dsa_page_fault_rd_recv", &mq->stats.dsa_stats[1].dsa_page_fault_rd),
		PSMI_STATS_DECLU64("shm_dsa_page_fault_wr_recv", &mq->stats.dsa_stats[1].dsa_page_fault_wr),
		PSMI_STATS_DECLU64("shm_dsa_error_recv", &mq->stats.dsa_stats[1].dsa_error),
#endif
		PSMI_STATS_DECLU64("sysbuf_count_recv", &mq->stats.rx_sysbuf_num),
		PSMI_STATS_DECLU64("sysbuf_bytes_recv", &mq->stats.rx_sysbuf_bytes),
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		PSMI_STATS_DECLU64("Eager_cpu_count_sent", &mq->stats.tx_eager_cpu_num),
		PSMI_STATS_DECLU64("Eager_cpu_bytes_sent", &mq->stats.tx_eager_cpu_bytes),
		PSMI_STATS_DECLU64("Eager_gpu_count_sent", &mq->stats.tx_eager_gpu_num),
		PSMI_STATS_DECLU64("Eager_gpu_bytes_sent", &mq->stats.tx_eager_gpu_bytes),
		PSMI_STATS_DECLU64("sysbuf_cpu_count_recv", &mq->stats.rx_sysbuf_cpu_num),
		PSMI_STATS_DECLU64("sysbuf_cpu_bytes_recv", &mq->stats.rx_sysbuf_cpu_bytes),
		PSMI_STATS_DECLU64("sysbuf_gdrcopy_count_recv", &mq->stats.rx_sysbuf_gdrcopy_num),
		PSMI_STATS_DECLU64("sysbuf_gdrcopy_bytes_recv", &mq->stats.rx_sysbuf_gdrcopy_bytes),
		PSMI_STATS_DECLU64("sysbuf_cuCopy_count_recv", &mq->stats.rx_sysbuf_cuCopy_num),
		PSMI_STATS_DECLU64("sysbuf_cuCopy_bytes_recv", &mq->stats.rx_sysbuf_cuCopy_bytes),
#endif /* PSM_CUDA || PSM_ONEAPI */
	};

	return psm3_stats_register_type("MPI_Statistics_Summary",
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
	mq->shm_thresh_rv = MQ_SHM_THRESH_RNDV;

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
psm2_error_t psm3_mq_initialize_params(psm2_mq_t mq)
{
	union psmi_envvar_val env_hfitiny, env_rvwin, env_hfirv,
		env_shmrv, env_stats;

	// a limit of PSM_MQ_MAX_TINY btyes is hardcoded into the PSM protocol
	psm3_getenv("PSM3_MQ_TINY_NIC_LIMIT",
		    "NIC tiny packet limit (max 8, default 8)",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)mq->hfi_thresh_tiny, &env_hfitiny);
	mq->hfi_thresh_tiny = min(env_hfitiny.e_uint, PSM_MQ_NIC_MAX_TINY);

	psm3_getenv("PSM3_MQ_RNDV_NIC_THRESH",
		    "NIC eager-to-rendezvous switchover",
		    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)mq->hfi_thresh_rv, &env_hfirv);
	mq->hfi_thresh_rv = env_hfirv.e_uint;

	psm3_getenv("PSM3_MQ_RNDV_NIC_WINDOW",
		    "NIC rendezvous window size, max 4M",
		    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)mq->hfi_base_window_rv, &env_rvwin);
	mq->hfi_base_window_rv = min(PSM_MQ_NIC_MAX_RNDV_WINDOW, env_rvwin.e_uint);

	/* Re-evaluate this since it may have changed after initializing the shm
	 * device */
	mq->shm_thresh_rv = psm3_shm_mq_rv_thresh;
	psm3_getenv("PSM3_MQ_RNDV_SHM_THRESH",
		    "shm eager-to-rendezvous switchover",
		    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)mq->shm_thresh_rv, &env_shmrv);
	mq->shm_thresh_rv = env_shmrv.e_uint;

	psm3_getenv("PSM3_MQ_PRINT_STATS",
		    "Prints MQ performance stats every n seconds to file "
			"./psm3-perf-stat-ep-[epid]-pid-[pid] when set to -1 stats are "
			"printed only once during finalization",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val) 0, &env_stats);
	mq->print_stats = env_stats.e_uint;

	mq->nohash_fastpath = 1;
	return PSM2_OK;
}

psm2_error_t MOCKABLE(psm3_mq_free)(psm2_mq_t mq)
{
	psm3_mq_req_fini(mq);
	psm3_mq_sysbuf_fini(mq);
	psm3_stats_deregister_type(PSMI_STATSTYPE_MQ, mq);
	psmi_free(mq);
	return PSM2_OK;
}
MOCK_DEF_EPILOGUE(psm3_mq_free);
