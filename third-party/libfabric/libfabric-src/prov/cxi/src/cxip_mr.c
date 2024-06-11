/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2017 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2018,2020-2023 Hewlett Packard Enterprise Development LP
 */

#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <fasthash.h>
#include <ofi_util.h>

#include "cxip.h"

#define CXIP_DBG(...) _CXIP_DBG(FI_LOG_MR, __VA_ARGS__)
#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_MR, __VA_ARGS__)

static int cxip_mr_init(struct cxip_mr *mr, struct cxip_domain *dom,
			const struct fi_mr_attr *attr, uint64_t flags);
static void cxip_mr_fini(struct cxip_mr *mr);
static int cxip_mr_prov_cache_enable_std(struct cxip_mr *mr);

void cxip_mr_domain_fini(struct cxip_mr_domain *mr_domain)
{
	int i;

	/* Assumption is this is only called when a domain is freed and only a
	 * single thread should be freeing a domain. Thus, no lock is taken.
	 */
	for (i = 0; i < CXIP_MR_DOMAIN_HT_BUCKETS; i++) {
		if (!dlist_empty(&mr_domain->buckets[i]))
			CXIP_WARN("MR domain bucket %d is not empty\n", i);
	}

	ofi_spin_destroy(&mr_domain->lock);
}

void cxip_mr_domain_init(struct cxip_mr_domain *mr_domain)
{
	int i;

	ofi_spin_init(&mr_domain->lock);

	for (i = 0; i < CXIP_MR_DOMAIN_HT_BUCKETS; i++)
		dlist_init(&mr_domain->buckets[i]);
}

/*
 * cxip_ep_mr_insert() - Insert an MR key into the EP key space.
 *
 * Called during MR enable. The key space is a sparse 64 bits.
 */
static void cxip_ep_mr_insert(struct cxip_ep_obj *ep_obj, struct cxip_mr *mr)
{
	dlist_insert_tail(&mr->ep_entry, &ep_obj->ctrl.mr_list);
}

/*
 * cxip_ep_mr_remove() - Remove an MR key from the EP key space.
 */
static void cxip_ep_mr_remove(struct cxip_mr *mr)
{
	dlist_remove(&mr->ep_entry);
}

/*
 * cxip_mr_cb() - Process MR LE events.
 */
int cxip_mr_cb(struct cxip_ctrl_req *req, const union c_event *event)
{
	struct cxip_mr *mr = req->mr.mr;
	int evt_rc = cxi_event_rc(event);

	switch (event->hdr.event_type) {
	case C_EVENT_LINK:
		if (mr->optimized)
			assert(mr->mr_state == CXIP_MR_ENABLED);
		else
			assert(mr->mr_state == CXIP_MR_DISABLED);

		if (evt_rc == C_RC_OK) {
			mr->mr_state = CXIP_MR_LINKED;
			CXIP_DBG("MR PTE linked: %p\n", mr);
			break;
		}

		mr->mr_state = CXIP_MR_LINK_ERR;
		CXIP_WARN("MR PTE link: %p failed %d\n", mr, evt_rc);
		break;
	case C_EVENT_UNLINK:
		assert(evt_rc == C_RC_OK);

		assert(mr->mr_state == CXIP_MR_LINKED);
		mr->mr_state = CXIP_MR_UNLINKED;

		CXIP_DBG("MR PTE unlinked: %p\n", mr);
		break;
	case C_EVENT_MATCH:
		ofi_atomic_inc32(&mr->match_events);

		if (evt_rc != C_RC_OK)
			goto log_err;
		break;
	case C_EVENT_PUT:
	case C_EVENT_GET:
	case C_EVENT_ATOMIC:
	case C_EVENT_FETCH_ATOMIC:
		if (mr->count_events)
			ofi_atomic_inc32(&mr->access_events);

		if (evt_rc != C_RC_OK)
			goto log_err;

		/* TODO handle fi_writedata/fi_inject_writedata */
		break;
	default:
log_err:
		CXIP_WARN(CXIP_UNEXPECTED_EVENT,
			  cxi_event_to_str(event), cxi_rc_to_str(evt_rc));
	}

	return FI_SUCCESS;
}

static int cxip_mr_wait_append(struct cxip_ep_obj *ep_obj,
			       struct cxip_mr *mr)
{
	/* Wait for PTE LE append status update */
	do {
		sched_yield();
		cxip_ep_tgt_ctrl_progress_locked(ep_obj);
	} while (mr->mr_state != CXIP_MR_LINKED &&
		 mr->mr_state != CXIP_MR_LINK_ERR);

	if (mr->mr_state == CXIP_MR_LINK_ERR)
		return -FI_ENOSPC;

	return FI_SUCCESS;
}

/*
 * cxip_mr_enable_std() - Assign HW resources to the standard MR.
 *
 * Standard MRs are implemented by linking an LE describing the registered
 * buffer to a shared, matching PtlTE. The MR key is encoded in the LE match
 * bits. One PtlTE supports many standard MRs. The number of standard MR
 * supported is limited by the total number of NIC LEs. Because a matching LE
 * is used, unrestricted commands must be used to target standard MRs.
 *
 * Caller must hold mr->lock, mr->ep->ep_obj->lock.
 */
static int cxip_mr_enable_std(struct cxip_mr *mr)
{
	int ret;
	struct cxip_ep_obj *ep_obj = mr->ep->ep_obj;
	struct cxip_mr_key key = {
		.raw = mr->key,
	};
	uint32_t le_flags;

	mr->req.cb = cxip_mr_cb;

	le_flags = C_LE_UNRESTRICTED_BODY_RO;
	if (mr->attr.access & FI_REMOTE_WRITE)
		le_flags |= C_LE_OP_PUT;
	if (mr->attr.access & FI_REMOTE_READ)
		le_flags |= C_LE_OP_GET;
	if (mr->cntr)
		le_flags |= C_LE_EVENT_CT_COMM;

	/* TODO: to support fi_writedata(), we will want to leave
	 * success events enabled for mr->rma_events true too.
	 */
	if (!mr->count_events)
		le_flags |= C_LE_EVENT_SUCCESS_DISABLE;

	ret = cxip_pte_append(ep_obj->ctrl.pte,
			      mr->len ? CXI_VA_TO_IOVA(mr->md->md, mr->buf) : 0,
			      mr->len, mr->len ? mr->md->md->lac : 0,
			      C_PTL_LIST_PRIORITY, mr->req.req_id,
			      key.key, 0, CXI_MATCH_ID_ANY,
			      0, le_flags, mr->cntr, ep_obj->ctrl.tgq, true);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to write Append command: %d\n", ret);
		return ret;
	}

	ret = cxip_mr_wait_append(ep_obj, mr);
	if (ret)
		return ret;

	mr->enabled = true;

	CXIP_DBG("Standard MR enabled: %p (key: 0x%016lX)\n", mr, mr->key);

	return FI_SUCCESS;
}

/*
 * cxip_mr_disable_std() - Free HW resources from the standard MR.
 *
 * Caller must hold mr->lock, mr->ep->ep_obj->lock.
 */
static int cxip_mr_disable_std(struct cxip_mr *mr)
{
	int ret;
	struct cxip_ep_obj *ep_obj = mr->ep->ep_obj;

	/* TODO: Handle -FI_EAGAIN. */
	ret = cxip_pte_unlink(ep_obj->ctrl.pte, C_PTL_LIST_PRIORITY,
			      mr->req.req_id, ep_obj->ctrl.tgq);
	assert(ret == FI_SUCCESS);

	do {
		sched_yield();
		cxip_ep_tgt_ctrl_progress_locked(ep_obj);
	} while (mr->mr_state != CXIP_MR_UNLINKED);

	/* If MR event counts are recorded then we can check event counts
	 * to determine if invalidate can be skipped.
	 */
	if (!mr->count_events || ofi_atomic_get32(&mr->match_events) !=
	    ofi_atomic_get32(&mr->access_events)) {
		/* TODO: Temporary debug helper for DAOS to track if
		 * Match events detect a need to flush.
		 */
		if (mr->count_events)
			CXIP_WARN("Match events required pte LE invalidate\n");

		ret = cxil_invalidate_pte_le(ep_obj->ctrl.pte->pte, mr->key,
					     C_PTL_LIST_PRIORITY);
		if (ret)
			CXIP_WARN("MR %p key 0x%016lX invalidate failed %d\n",
				  mr, mr->key, ret);
	}

	mr->enabled = false;

	CXIP_DBG("Standard MR disabled: %p (key: 0x%016lX)\n", mr, mr->key);

	return FI_SUCCESS;
}

/*
 * cxip_mr_opt_pte_cb() - Process optimized MR state change events.
 */
void cxip_mr_opt_pte_cb(struct cxip_pte *pte, const union c_event *event)
{
	struct cxip_mr *mr = (struct cxip_mr *)pte->ctx;

	switch (pte->state) {
	case C_PTLTE_ENABLED:
		assert(mr->mr_state == CXIP_MR_DISABLED);
		mr->mr_state = CXIP_MR_ENABLED;

		CXIP_DBG("MR PTE enabled: %p\n", mr);
		break;
	default:
		CXIP_WARN("Unexpected state received: %u\n", pte->state);
	}
}

/*
 * cxip_mr_enable_opt() - Assign HW resources to the optimized MR.
 *
 * Optimized MRs are implemented by allocating a dedicated, non-matching PtlTE
 * and linking an LE describing the registered buffer. The MR key is used to
 * derive the PtlTE index. One PtlTE and one LE is required for each optimized
 * MR. Because a non-matching interface is used, optimized MRs can be targeted
 * with restricted commands. This may result in better performance.
 *
 * Caller must hold mr->lock, mr->ep->ep_obj->lock.
 */
static int cxip_mr_enable_opt(struct cxip_mr *mr)
{
	int ret;
	struct cxi_pt_alloc_opts opts = {};
	struct cxip_ep_obj *ep_obj = mr->ep->ep_obj;
	uint32_t le_flags;
	uint64_t ib = 0;
	int pid_idx;

	mr->req.cb = cxip_mr_cb;

	ret = cxip_pte_alloc_nomap(ep_obj->ptable, ep_obj->ctrl.tgt_evtq,
				   &opts, cxip_mr_opt_pte_cb, mr, &mr->pte);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to allocate PTE: %d\n", ret);
		return ret;
	}

	pid_idx = cxip_generic_mr_key_to_ptl_idx(mr->domain, mr->key, true);
	ret = cxip_pte_map(mr->pte, pid_idx, false);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to map write pid_idx %d to PTE: %d\n",
			  pid_idx, ret);
		goto err_pte_free;
	}

	pid_idx = cxip_generic_mr_key_to_ptl_idx(mr->domain, mr->key, false);
	ret = cxip_pte_map(mr->pte, pid_idx, false);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to map read pid_idx %d to PTE: %d\n",
			  pid_idx, ret);
		goto err_pte_free;
	}

	ret = cxip_pte_set_state(mr->pte, ep_obj->ctrl.tgq, C_PTLTE_ENABLED, 0);
	if (ret != FI_SUCCESS) {
		/* This is a bug, we have exclusive access to this CMDQ. */
		CXIP_WARN("Failed to enqueue command: %d\n", ret);
		goto err_pte_free;
	}

	le_flags = C_LE_EVENT_COMM_DISABLE | C_LE_EVENT_SUCCESS_DISABLE |
		   C_LE_UNRESTRICTED_BODY_RO;
	if (mr->attr.access & FI_REMOTE_WRITE)
		le_flags |= C_LE_OP_PUT;
	if (mr->attr.access & FI_REMOTE_READ)
		le_flags |= C_LE_OP_GET;
	if (mr->cntr)
		le_flags |= C_LE_EVENT_CT_COMM;

	/* When FI_FENCE is not requested, restricted operations can used PCIe
	 * relaxed ordering. Unrestricted operations PCIe relaxed ordering is
	 * controlled by an env for now.
	 */
	if (!(ep_obj->caps & FI_FENCE)) {
		ib = 1;

		if (cxip_env.enable_unrestricted_end_ro)
			le_flags |= C_LE_UNRESTRICTED_END_RO;
	}

	ret = cxip_pte_append(mr->pte,
			      mr->len ? CXI_VA_TO_IOVA(mr->md->md, mr->buf) : 0,
			      mr->len, mr->len ? mr->md->md->lac : 0,
			      C_PTL_LIST_PRIORITY, mr->req.req_id,
			      0, ib, CXI_MATCH_ID_ANY,
			      0, le_flags, mr->cntr, ep_obj->ctrl.tgq, true);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to write Append command: %d\n", ret);
		goto err_pte_free;
	}

	ret = cxip_mr_wait_append(ep_obj, mr);
	if (ret)
		goto err_pte_free;

	mr->enabled = true;

	CXIP_DBG("Optimized MR enabled: %p (key: 0x%016lX)\n", mr, mr->key);

	return FI_SUCCESS;

err_pte_free:
	cxip_pte_free(mr->pte);

	return ret;
}

/*
 * cxip_mr_disable_opt() - Free hardware resources for non-cached
 * optimized MR.
 *
 * Caller must hold mr->lock, mr->ep->ep_obj->lock.
 */
static int cxip_mr_disable_opt(struct cxip_mr *mr)
{
	int ret;
	struct cxip_ep_obj *ep_obj = mr->ep->ep_obj;

	ret = cxip_pte_unlink(mr->pte, C_PTL_LIST_PRIORITY,
			      mr->req.req_id, ep_obj->ctrl.tgq);
	if (ret) {
		CXIP_WARN("Failed to enqueue Unlink: %d\n", ret);
		goto cleanup;
	}

	do {
		sched_yield();
		cxip_ep_tgt_ctrl_progress_locked(ep_obj);
	} while (mr->mr_state != CXIP_MR_UNLINKED);

cleanup:
	cxip_pte_free(mr->pte);

	mr->enabled = false;

	CXIP_DBG("Optimized MR disabled: %p (key: 0x%016lX)\n", mr, mr->key);

	return FI_SUCCESS;
}

static void cxip_mr_prov_opt_to_std(struct cxip_mr *mr)
{
	struct cxip_mr_key key = {
		.raw = mr->mr_fid.key,
	};

	CXIP_WARN("Optimized MR unavailable, fallback to standard MR\n");

	key.opt = false;
	mr->mr_fid.key = key.raw;
	mr->optimized = false;
}

/*
 * cxip_mr_prov_enable_opt() - Enable a provider key optimized
 * MR, falling back to a standard MR if resources are not available.
 *
 * Caller must hold mr->lock, mr->ep->ep_obj->lock.
 */
static int cxip_mr_prov_enable_opt(struct cxip_mr *mr)
{
	int ret;

	ret = cxip_mr_enable_opt(mr);
	if (!ret)
		return ret;

	cxip_mr_prov_opt_to_std(mr);

	return cxip_mr_enable_std(mr);
}

/*
 * cxip_mr_prov_cache_enable_opt() - Enable a provider key optimized
 * MR configuring hardware if not already cached.
 *
 * Caller must hold mr->lock, mr->ep->ep_obj->lock.
 */
static int cxip_mr_prov_cache_enable_opt(struct cxip_mr *mr)
{
	int ret;
	int lac = mr->md->md->lac;
	struct cxi_pt_alloc_opts opts = {};
	struct cxip_ep_obj *ep_obj = mr->ep->ep_obj;
	struct cxip_mr_lac_cache *mr_cache;
	struct cxip_mr *_mr;
	uint32_t le_flags;
	uint64_t ib = 0;

	mr_cache = &ep_obj->ctrl.opt_mr_cache[lac];
	ofi_atomic_inc32(&mr_cache->ref);

	if (mr_cache->ctrl_req)
		goto done;

	mr_cache->ctrl_req = calloc(1, sizeof(struct cxip_ctrl_req));
	if (!mr_cache->ctrl_req) {
		ret = -FI_ENOMEM;
		goto err;
	}

	ret = cxip_domain_ctrl_id_alloc(ep_obj->domain, mr_cache->ctrl_req);
	if (ret) {
		CXIP_WARN("Failed to allocate MR buffer ID: %d\n", ret);
		goto err_free_req;
	}

	mr_cache->ctrl_req->ep_obj = ep_obj;
	mr_cache->ctrl_req->cb = cxip_mr_cb;

	/* Allocate a dummy MR used to maintain cache state for this
	 * LAC/enable RO state PTE.
	 */
	_mr = calloc(1, sizeof(struct cxip_mr));
	if (!_mr) {
		ret = -FI_ENOMEM;
		goto err_free_id;
	}

	mr_cache->ctrl_req->mr.mr = _mr;
	mr_cache->ctrl_req->mr.mr->domain = ep_obj->domain;
	mr_cache->ctrl_req->mr.mr->optimized = true;
	mr_cache->ctrl_req->mr.mr->mr_state = CXIP_MR_DISABLED;

	ret = cxip_pte_alloc_nomap(ep_obj->ptable, ep_obj->ctrl.tgt_evtq,
				   &opts, cxip_mr_opt_pte_cb,
				   _mr, &_mr->pte);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to allocate PTE: %d\n", ret);
		goto err_free_mr;
	}

	ret = cxip_pte_map(_mr->pte, CXIP_PTL_IDX_WRITE_PROV_CACHE_MR_OPT(lac),
			   false);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to map write PTE: %d\n", ret);
		goto err_pte_free;
	}

	ret = cxip_pte_map(_mr->pte, CXIP_PTL_IDX_READ_PROV_CACHE_MR_OPT(lac),
			   false);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to map write PTE: %d\n", ret);
		goto err_pte_free;
	}

	ret = cxip_pte_set_state(_mr->pte, ep_obj->ctrl.tgq,
				 C_PTLTE_ENABLED, 0);
	if (ret != FI_SUCCESS) {
		/* This is a bug, we have exclusive access to this CMDQ. */
		CXIP_WARN("Failed to enqueue command: %d\n", ret);
		goto err_pte_free;
	}

	le_flags = C_LE_EVENT_COMM_DISABLE | C_LE_EVENT_SUCCESS_DISABLE |
		   C_LE_UNRESTRICTED_BODY_RO | C_LE_OP_PUT | C_LE_OP_GET;

	/* When FI_FENCE is not requested, restricted operations can used PCIe
	 * relaxed ordering. Unrestricted operations PCIe relaxed ordering is
	 * controlled by an env for now.
	 */
	if (!(ep_obj->caps & FI_FENCE)) {
		ib = 1;

		if (cxip_env.enable_unrestricted_end_ro)
			le_flags |= C_LE_UNRESTRICTED_END_RO;
	}

	ret = cxip_pte_append(_mr->pte, 0, -1ULL, lac,
			      C_PTL_LIST_PRIORITY,
			      mr_cache->ctrl_req->req_id,
			      0, ib, CXI_MATCH_ID_ANY,
			      0, le_flags, NULL, ep_obj->ctrl.tgq, true);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to write Append command: %d\n", ret);
		goto err_pte_free;
	}

	ret = cxip_mr_wait_append(ep_obj, _mr);
	if (ret)
		goto err_pte_free;
done:
	mr->enabled = true;

	CXIP_DBG("Optimized MR enabled: %p (key: 0x%016lX)\n", mr, mr->key);

	return FI_SUCCESS;

err_pte_free:
	cxip_pte_free(_mr->pte);
err_free_mr:
	free(mr_cache->ctrl_req->mr.mr);
err_free_id:
	cxip_domain_ctrl_id_free(ep_obj->domain, mr_cache->ctrl_req);
err_free_req:
	free(mr_cache->ctrl_req);
	mr_cache->ctrl_req = NULL;
err:
	cxip_mr_prov_opt_to_std(mr);

	return cxip_mr_prov_cache_enable_std(mr);
}

/*
 * cxip_mr_prov_cache_disable_opt() - Disable a provider key
 * optimized MR.
 *
 * Caller must hold mr->lock, mr->ep->ep_obj->lock.
 */
static int cxip_mr_prov_cache_disable_opt(struct cxip_mr *mr)
{
	struct cxip_mr_key key = {
		.raw = mr->key,
	};
	int lac = key.lac;
	struct cxip_ep_obj *ep_obj = mr->ep->ep_obj;

	assert(key.opt);

	CXIP_DBG("Disable optimized cached MR: %p (key: 0x%016lX)\n",
		 mr, mr->key);

	if (ofi_atomic_get32(&ep_obj->ctrl.opt_mr_cache[lac].ref) <= 0) {
		CXIP_WARN("Cached optimized MR reference underflow\n");
		return -FI_EINVAL;
	}
	ofi_atomic_dec32(&ep_obj->ctrl.opt_mr_cache[lac].ref);
	mr->enabled = false;

	return FI_SUCCESS;
}

/*
 * cxip_mr_prov_cache_enable_std() - Enable a provider key standard
 * MR configuring hardware if not already cached.
 *
 * Caller must hold mr->lock, mr->ep->ep_obj->lock.
 */
static int cxip_mr_prov_cache_enable_std(struct cxip_mr *mr)
{
	int ret;
	int lac = mr->md->md->lac;
	struct cxip_ep_obj *ep_obj = mr->ep->ep_obj;
	struct cxip_mr_lac_cache *mr_cache;
	union cxip_match_bits mb;
	union cxip_match_bits ib;
	uint32_t le_flags;

	/* TODO: Handle enabling for each bound endpoint */
	mr_cache = &ep_obj->ctrl.std_mr_cache[lac];
	ofi_atomic_inc32(&mr_cache->ref);

	if (mr_cache->ctrl_req)
		goto done;

	mr_cache->ctrl_req = calloc(1, sizeof(struct cxip_ctrl_req));
	if (!mr_cache->ctrl_req) {
		ret = -FI_ENOMEM;
		goto err;
	}

	ret = cxip_domain_ctrl_id_alloc(ep_obj->domain, mr_cache->ctrl_req);
	if (ret) {
		CXIP_WARN("Failed to allocate MR buffer ID: %d\n", ret);
		goto err_free_req;
	}

	mr_cache->ctrl_req->ep_obj = ep_obj;
	mr_cache->ctrl_req->cb = cxip_mr_cb;

	/* Allocate a dummy MR used to maintain cache state transitions */
	mr_cache->ctrl_req->mr.mr = calloc(1, sizeof(struct cxip_mr));
	if (!mr_cache->ctrl_req->mr.mr) {
		ret = -FI_ENOMEM;
		goto err_free_id;
	}

	mr_cache->ctrl_req->mr.mr->domain = ep_obj->domain;
	mr_cache->ctrl_req->mr.mr->optimized = false;
	mr_cache->ctrl_req->mr.mr->mr_state = CXIP_MR_DISABLED;

	mb.raw = 0;
	mb.mr_lac = mr->md->md->lac;
	mb.mr_cached = 1;

	ib.raw = ~0;
	ib.mr_lac = 0;
	ib.mr_cached = 0;

	le_flags = C_LE_EVENT_SUCCESS_DISABLE | C_LE_UNRESTRICTED_BODY_RO |
		   C_LE_OP_PUT | C_LE_OP_GET;

	ret = cxip_pte_append(ep_obj->ctrl.pte, 0, -1ULL,
			      mb.mr_lac, C_PTL_LIST_PRIORITY,
			      mr_cache->ctrl_req->req_id,
			      mb.raw, ib.raw, CXI_MATCH_ID_ANY,
			      0, le_flags, NULL, ep_obj->ctrl.tgq, true);

	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to write Append command: %d\n", ret);
		goto err_free_mr;
	}

	ret = cxip_mr_wait_append(ep_obj, mr_cache->ctrl_req->mr.mr);
	if (ret)
		goto err_free_mr;

done:
	mr->enabled = true;

	CXIP_DBG("Enable cached standard MR: %p (key: 0x%016lX\n",
		 mr, mr->key);

	return FI_SUCCESS;

err_free_mr:
	free(mr_cache->ctrl_req->mr.mr);
err_free_id:
	cxip_domain_ctrl_id_free(ep_obj->domain, mr_cache->ctrl_req);
err_free_req:
	free(mr_cache->ctrl_req);
	mr_cache->ctrl_req = NULL;
err:
	ofi_atomic_dec32(&mr_cache->ref);

	return ret;
}

/*
 * cxip_mr_prov_cache_disable_std() - Disable a provider standard
 * cached MR.
 *
 * Caller must hold mr->lock, mr->ep->ep_obj->lock.
 */
static int cxip_mr_prov_cache_disable_std(struct cxip_mr *mr)
{
	struct cxip_mr_key key = {
	       .raw	= mr->key,
	};
	int lac = key.lac;
	struct cxip_ep_obj *ep_obj = mr->ep->ep_obj;

	CXIP_DBG("Disable standard cached MR: %p (key: 0x%016lX)\n",
		 mr, mr->key);
	if (ofi_atomic_get32(&ep_obj->ctrl.std_mr_cache[lac].ref) <= 0) {
		CXIP_WARN("Cached standard MR reference underflow\n");
		return -FI_EINVAL;
	}
	ofi_atomic_dec32(&ep_obj->ctrl.std_mr_cache[lac].ref);
	mr->enabled = false;

	return FI_SUCCESS;
}

/*
 * cxip_mr_domain_remove() - Remove client key from domain hash.
 */
static void cxip_mr_domain_remove(struct cxip_mr *mr)
{
	if (mr->domain->is_prov_key)
		return;

	/* Only remotely accessible MR were assigned an RKEY */
	if (!(mr->attr.access & (FI_REMOTE_READ | FI_REMOTE_WRITE)))
		return;

	ofi_spin_lock(&mr->domain->mr_domain.lock);
	dlist_remove(&mr->mr_domain_entry);
	ofi_spin_unlock(&mr->domain->mr_domain.lock);
}

/*
 * cxip_mr_domain_insert() - Validate uniqueness and insert
 * client key in the domain hash table.
 */
static int cxip_mr_domain_insert(struct cxip_mr *mr)
{
	struct cxip_mr_domain *mr_domain = &mr->domain->mr_domain;
	int bucket;
	struct cxip_mr *clash_mr;

	if (mr->domain->is_prov_key)
		return FI_SUCCESS;

	/* Only remotely accessible MR are assigned an RKEY */
	if (!(mr->attr.access & (FI_REMOTE_READ | FI_REMOTE_WRITE)))
		return FI_SUCCESS;

	mr->key = mr->attr.requested_key;

	if (!cxip_generic_is_valid_mr_key(mr->key))
		return -FI_EKEYREJECTED;

	bucket = fasthash64(&mr->key, sizeof(mr->key), 0) %
		CXIP_MR_DOMAIN_HT_BUCKETS;

	ofi_spin_lock(&mr_domain->lock);

	dlist_foreach_container(&mr_domain->buckets[bucket], struct cxip_mr,
				clash_mr, mr_domain_entry) {
		if (clash_mr->key == mr->key) {
			ofi_spin_unlock(&mr_domain->lock);
			return -FI_ENOKEY;
		}
	}

	dlist_insert_tail(&mr->mr_domain_entry, &mr_domain->buckets[bucket]);

	ofi_spin_unlock(&mr_domain->lock);

	return FI_SUCCESS;
}

static int cxip_init_mr_key(struct cxip_mr *mr, uint64_t req_key)
{
	mr->key = req_key;

	return FI_SUCCESS;
}

/*
 * cxip_prov_init_mr_key() - Generate a provider key for
 * a non-cached MR.
 */
static int cxip_prov_init_mr_key(struct cxip_mr *mr, uint64_t req_key)
{
	int ret;

	/* Non-cached FI_MR_PROV_KEY MR keys need to be unique. */
	ret = cxip_domain_prov_mr_id_alloc(mr->domain, mr);
	if (ret)
		return ret;

	CXIP_DBG("Init non-cached provider MR key 0x%016lX\n", mr->key);

	return FI_SUCCESS;
}

/*
 * cxip_prov_cache_init_mr_key() - Generate a provider key for
 * a cached MR.
 *
 * Note cached MR do not support counters or target events.
 */
static int cxip_prov_cache_init_mr_key(struct cxip_mr *mr,
				       uint64_t req_key)
{
	struct cxip_mr_key key = {};
	struct cxi_md *md = mr->md->md;

	/* If optimized enabled it is preferred for caching */
	key.opt = mr->domain->optimized_mrs;
	key.cached = true;
	key.is_prov = 1;
	key.lac = mr->len ? md->lac : 0;
	key.lac_off = mr->len ? CXI_VA_TO_IOVA(md, mr->buf) : 0;
	mr->key = key.raw;

	CXIP_DBG("Init cached MR key 0x%016lX, lac: %d, off:0x%016lX\n",
		 key.raw, key.lac, (uint64_t)key.lac_off);

	return FI_SUCCESS;
}

static bool cxip_is_valid_mr_key(uint64_t key)
{
	if (key & ~CXIP_MR_KEY_MASK)
		return false;

	return true;
}

static bool cxip_is_valid_prov_mr_key(uint64_t key)
{
	struct cxip_mr_key cxip_key = {
		.raw = key,
	};

	if (cxip_key.cached)
		return cxip_key.is_prov == 1;

	if (cxip_key.opt)
		return CXIP_MR_UNCACHED_KEY_TO_IDX(cxip_key.key) <
				CXIP_PTL_IDX_PROV_MR_OPT_CNT;

	if (cxip_key.key & ~CXIP_MR_PROV_KEY_MASK)
		return false;

	return true;
}

bool cxip_generic_is_valid_mr_key(uint64_t key)
{
	struct cxip_mr_key cxip_key = {
		.raw = key,
	};

	if (cxip_key.is_prov)
		return cxip_is_valid_prov_mr_key(key);

	return cxip_is_valid_mr_key(key);
}

static bool cxip_mr_key_opt(uint64_t key)
{
	/* Client key optimized MR controlled globally only */
	return cxip_env.optimized_mrs && key < CXIP_PTL_IDX_MR_OPT_CNT;
}

static bool cxip_prov_mr_key_opt(uint64_t key)
{
	struct cxip_mr_key cxip_key = {
		.raw = key,
	};

	if (cxip_key.opt)
		return true;

	return false;
}

bool cxip_generic_is_mr_key_opt(uint64_t key)
{
	struct cxip_mr_key cxip_key = {
		.raw = key,
	};

	if (cxip_key.is_prov)
		return cxip_prov_mr_key_opt(key);

	return cxip_mr_key_opt(key);
}

static bool cxip_prov_mr_key_events(uint64_t key)
{
	struct cxip_mr_key cxip_key = {
		.raw = key,
	};

	/* Cached keys can not be bound to counters or require RMA events,
	 * the "events" field is not defined.
	 */
	if (cxip_key.cached)
		return false;

	if (cxip_key.events)
		return true;

	return false;
}

/* If CAPs or MR Key indicate events are required at the target */
bool cxip_generic_is_mr_key_events(uint64_t caps, uint64_t key)
{
	struct cxip_mr_key cxip_key = {
		.raw = key,
	};

	if (cxip_key.is_prov)
		return cxip_prov_mr_key_events(key);

	/* Client keys cannot indicate if they require events and
	 * rely on FI_RMA_EVENT being set on source and target.
	 */
	return !!(caps & FI_RMA_EVENT);
}

/*
 * cxip_mr_key_to_ptl_idx() Maps a client generated key to the
 * PtlTE index.
 */
static int cxip_mr_key_to_ptl_idx(struct cxip_domain *dom,
				  uint64_t key, bool write)
{
	if (cxip_generic_is_mr_key_opt(key))
		return write ? CXIP_PTL_IDX_WRITE_MR_OPT(key) :
			CXIP_PTL_IDX_READ_MR_OPT(key);

	return write ? CXIP_PTL_IDX_WRITE_MR_STD : CXIP_PTL_IDX_READ_MR_STD;
}

/*
 * cxip_prov_mr_key_to_ptl_idx() - Maps a provider generated key
 * to the PtlTE index.
 */
static int cxip_prov_mr_key_to_ptl_idx(struct cxip_domain *dom,
				       uint64_t key, bool write)
{
	struct cxip_mr_key cxip_key = {
		.raw = key,
	};
	int idx;

	if (cxip_generic_is_mr_key_opt(key)) {
		idx = write ? CXIP_PTL_IDX_WRITE_MR_OPT_BASE :
			      CXIP_PTL_IDX_READ_MR_OPT_BASE;

		/* First 8 PTE are used for LAC cache entries */
		if (cxip_key.cached) {
			idx += cxip_key.lac;
			return idx;
		}

		/* Verify within non-cached optimized range */
		assert(CXIP_MR_UNCACHED_KEY_TO_IDX(cxip_key.key) <
				CXIP_PTL_IDX_PROV_MR_OPT_CNT);

		idx += CXIP_PTL_IDX_PROV_NUM_CACHE_IDX +
				CXIP_MR_UNCACHED_KEY_TO_IDX(cxip_key.key);
		return idx;
	}

	return write ? CXIP_PTL_IDX_WRITE_MR_STD : CXIP_PTL_IDX_READ_MR_STD;
}

/*
 * cxip_generic_mr_key_to_ptl_idx() - Maps a MR RKEY to the PtlTE index.
 */
int cxip_generic_mr_key_to_ptl_idx(struct cxip_domain *dom, uint64_t key,
				   bool write)
{
	struct cxip_mr_key cxip_key = {
		.raw = key,
	};

	if (cxip_key.is_prov)
		return cxip_prov_mr_key_to_ptl_idx(dom, key, write);

	return cxip_mr_key_to_ptl_idx(dom, key, write);
}

/* Caller should hold ep_obj->lock */
void cxip_ctrl_mr_cache_flush(struct cxip_ep_obj *ep_obj)
{
	int lac;
	struct cxip_mr_lac_cache *mr_cache;
	int ret;

	/* Flush standard MR resources hardware resources not in use */
	for (lac = 0; lac < CXIP_NUM_CACHED_KEY_LE; lac++) {
		mr_cache = &ep_obj->ctrl.std_mr_cache[lac];

		if (!mr_cache->ctrl_req ||
		    ofi_atomic_get32(&mr_cache->ref))
			continue;

		ret = cxip_pte_unlink(ep_obj->ctrl.pte, C_PTL_LIST_PRIORITY,
				      mr_cache->ctrl_req->req_id,
				      ep_obj->ctrl.tgq);
		assert(ret == FI_SUCCESS);

		do {
			sched_yield();
			cxip_ep_tgt_ctrl_progress_locked(ep_obj);
		} while (mr_cache->ctrl_req->mr.mr->mr_state !=
			 CXIP_MR_UNLINKED);

		ret = cxil_invalidate_pte_le(ep_obj->ctrl.pte->pte,
					     mr_cache->ctrl_req->req_id,
					     C_PTL_LIST_PRIORITY);
		if (ret)
			CXIP_WARN("Remote MR cache flush invalidate err: %d\n",
				  ret);

		free(mr_cache->ctrl_req->mr.mr);
		cxip_domain_ctrl_id_free(ep_obj->domain, mr_cache->ctrl_req);
		free(mr_cache->ctrl_req);
		mr_cache->ctrl_req = NULL;
	}

	/* Flush optimized MR resources hardware resources not in use */
	for (lac = 0; lac < CXIP_NUM_CACHED_KEY_LE; lac++) {
		mr_cache = &ep_obj->ctrl.opt_mr_cache[lac];

		if (!mr_cache->ctrl_req ||
		    ofi_atomic_get32(&mr_cache->ref))
			continue;

		ret = cxip_pte_unlink(mr_cache->ctrl_req->mr.mr->pte,
				      C_PTL_LIST_PRIORITY,
				      mr_cache->ctrl_req->req_id,
				      ep_obj->ctrl.tgq);
		if (ret) {
			CXIP_WARN("Failed to enqueue Unlink: %d\n", ret);
			goto cleanup;
		}

		do {
			sched_yield();
			cxip_ep_tgt_ctrl_progress_locked(ep_obj);
		} while (mr_cache->ctrl_req->mr.mr->mr_state !=
			 CXIP_MR_UNLINKED);

cleanup:
		cxip_pte_free(mr_cache->ctrl_req->mr.mr->pte);
		free(mr_cache->ctrl_req->mr.mr);
		cxip_domain_ctrl_id_free(ep_obj->domain, mr_cache->ctrl_req);
		free(mr_cache->ctrl_req);
		mr_cache->ctrl_req = NULL;
	}
}

struct cxip_mr_util_ops cxip_client_key_mr_util_ops = {
	.is_cached = false,
	.init_key = cxip_init_mr_key,
	.enable_opt = cxip_mr_enable_opt,
	.disable_opt = cxip_mr_disable_opt,
	.enable_std = cxip_mr_enable_std,
	.disable_std = cxip_mr_disable_std,
};

struct cxip_mr_util_ops cxip_prov_key_mr_util_ops = {
	.is_cached = false,
	.init_key = cxip_prov_init_mr_key,
	.enable_opt = cxip_mr_prov_enable_opt,
	.disable_opt = cxip_mr_disable_opt,
	.enable_std = cxip_mr_enable_std,
	.disable_std = cxip_mr_disable_std,
};

struct cxip_mr_util_ops cxip_prov_key_cache_mr_util_ops = {
	.is_cached = true,
	.init_key = cxip_prov_cache_init_mr_key,
	.enable_opt = cxip_mr_prov_cache_enable_opt,
	.disable_opt = cxip_mr_prov_cache_disable_opt,
	.enable_std = cxip_mr_prov_cache_enable_std,
	.disable_std = cxip_mr_prov_cache_disable_std,
};

int cxip_mr_enable(struct cxip_mr *mr)
{
	int ret;

	/* MR which require remote access require additional resources. Locally
	 * access MRs only do not. Thus, return FI_SUCCESS.
	 */
	if (mr->enabled ||
	    !(mr->attr.access & (FI_REMOTE_READ | FI_REMOTE_WRITE)))
		return FI_SUCCESS;

	/* Set MR operations based on key management and whether
	 * the MR is cache-able.
	 */
	if (!mr->domain->is_prov_key)
		mr->mr_util = &cxip_client_key_mr_util_ops;
	else if (mr->md && mr->md->cached && mr->domain->prov_key_cache &&
		 !mr->cntr && !mr->count_events && !mr->rma_events)
		mr->mr_util = &cxip_prov_key_cache_mr_util_ops;
	else
		mr->mr_util = &cxip_prov_key_mr_util_ops;

	/* Officially set MR key */
	if (mr->domain->is_prov_key) {
		ret = mr->mr_util->init_key(mr, mr->attr.requested_key);
		if (ret) {
			CXIP_WARN("Failed to initialize MR key: %d\n", ret);
			return ret;
		}
		mr->mr_fid.key = mr->key;
	}
	mr->optimized = cxip_generic_is_mr_key_opt(mr->key);

	ofi_genlock_lock(&mr->ep->ep_obj->lock);
	cxip_ep_mr_insert(mr->ep->ep_obj, mr);

	if (mr->optimized)
		ret = mr->mr_util->enable_opt(mr);
	else
		ret = mr->mr_util->enable_std(mr);
	ofi_genlock_unlock(&mr->ep->ep_obj->lock);

	if (ret != FI_SUCCESS)
		goto err_remove_mr;

	return FI_SUCCESS;

err_remove_mr:
	cxip_ep_mr_remove(mr);

	return ret;
}

int cxip_mr_disable(struct cxip_mr *mr)
{
	int ret;

	if (!mr->enabled ||
	    !(mr->attr.access & (FI_REMOTE_READ | FI_REMOTE_WRITE)))
		return FI_SUCCESS;

	ofi_genlock_lock(&mr->ep->ep_obj->lock);
	if (mr->optimized)
		ret = mr->mr_util->disable_opt(mr);
	else
		ret = mr->mr_util->disable_std(mr);

	cxip_ep_mr_remove(mr);
	ofi_genlock_unlock(&mr->ep->ep_obj->lock);

	return ret;
}

/*
 * cxip_mr_close() - fi_close implemented for MRs.
 */
static int cxip_mr_close(struct fid *fid)
{
	struct cxip_mr *mr;
	int ret;

	if (!fid)
		return -FI_EINVAL;

	mr = container_of(fid, struct cxip_mr, mr_fid.fid);

	ofi_spin_lock(&mr->lock);

	ret = cxip_mr_disable(mr);
	if (ret != FI_SUCCESS)
		CXIP_WARN("Failed to disable MR: %d\n", ret);

	if (mr->len)
		cxip_unmap(mr->md);

	cxip_mr_domain_remove(mr);

	if (mr->ep)
		ofi_atomic_dec32(&mr->ep->ep_obj->ref);

	if (mr->cntr)
		ofi_atomic_dec32(&mr->cntr->ref);

	cxip_mr_fini(mr);
	ofi_atomic_dec32(&mr->domain->ref);

	ofi_spin_unlock(&mr->lock);

	free(mr);

	return FI_SUCCESS;
}

/*
 * cxip_mr_bind() - fi_bind() implementation for MRs.
 */
static int cxip_mr_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct cxip_mr *mr;
	struct cxip_cntr *cntr;
	struct cxip_ep *ep;
	int ret = FI_SUCCESS;

	mr = container_of(fid, struct cxip_mr, mr_fid.fid);

	ofi_spin_lock(&mr->lock);

	switch (bfid->fclass) {
	case FI_CLASS_CNTR:
		cntr = container_of(bfid, struct cxip_cntr, cntr_fid.fid);
		if (mr->domain != cntr->domain || mr->enabled) {
			ret = -FI_EINVAL;
			break;
		}

		if (mr->cntr) {
			ret = -FI_EINVAL;
			break;
		}

		if (!(flags & FI_REMOTE_WRITE)) {
			ret = -FI_EINVAL;
			break;
		}

		mr->cntr = cntr;
		ofi_atomic_inc32(&cntr->ref);
		break;

	case FI_CLASS_EP:
		ep = container_of(bfid, struct cxip_ep, ep.fid);
		if (mr->domain != ep->ep_obj->domain || mr->enabled) {
			ret = -FI_EINVAL;
			break;
		}

		if (mr->ep || !ep->ep_obj->enabled) {
			ret = -FI_EINVAL;
			break;
		}

		if (mr->rma_events && !(ep->ep_obj->caps & FI_RMA_EVENT)) {
			CXIP_WARN("MR requires FI_RMA_EVENT EP cap\n");
			ret = -FI_EINVAL;
			break;
		}

		mr->ep = ep;
		ofi_atomic_inc32(&ep->ep_obj->ref);
		break;

	default:
		ret = -FI_EINVAL;
	}

	ofi_spin_unlock(&mr->lock);

	return ret;
}

/*
 * cxip_mr_control() - fi_control() implementation for MRs.
 */
static int cxip_mr_control(struct fid *fid, int command, void *arg)
{
	struct cxip_mr *mr;
	int ret;

	mr = container_of(fid, struct cxip_mr, mr_fid.fid);

	ofi_spin_lock(&mr->lock);

	switch (command) {
	case FI_ENABLE:
		/* An MR must be bound to an EP before being enabled. */
		if (!mr->ep) {
			ret = -FI_EINVAL;
			break;
		}

		ret = cxip_mr_enable(mr);
		if (ret != FI_SUCCESS)
			CXIP_WARN("Failed to enable MR: %d\n", ret);

		break;

	default:
		ret = -FI_EINVAL;
	}

	ofi_spin_unlock(&mr->lock);

	return ret;
}

static struct fi_ops cxip_mr_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = cxip_mr_close,
	.bind = cxip_mr_bind,
	.control = cxip_mr_control,
	.ops_open = fi_no_ops_open,
};

static void cxip_mr_fini(struct cxip_mr *mr)
{
	cxip_domain_ctrl_id_free(mr->domain, &mr->req);
	cxip_domain_prov_mr_id_free(mr->domain, mr);
}

static int cxip_mr_init(struct cxip_mr *mr, struct cxip_domain *dom,
			const struct fi_mr_attr *attr, uint64_t flags)
{
	int ret;

	ofi_spin_init(&mr->lock);

	mr->mr_fid.fid.fclass = FI_CLASS_MR;
	mr->mr_fid.fid.context = attr->context;
	mr->mr_fid.fid.ops = &cxip_mr_fi_ops;

	/* Generation of the key for FI_MR_PROV_KEY can not be done
	 * until the MR has been bound and enabled to at least one
	 * endpoint.
	 */
	mr->mr_fid.key = FI_KEY_NOTAVAIL;

	mr->domain = dom;
	mr->flags = flags;
	mr->attr = *attr;

	mr->count_events = dom->mr_match_events;
	ofi_atomic_initialize32(&mr->match_events, 0);
	ofi_atomic_initialize32(&mr->access_events, 0);
	mr->rma_events = flags & FI_RMA_EVENT;

	/* Support length 1 IOV only for now */
	mr->buf = mr->attr.mr_iov[0].iov_base;
	mr->len = mr->attr.mr_iov[0].iov_len;

	/* Allocate unique MR buffer ID if remote access MR */
	if (mr->attr.access & (FI_REMOTE_READ | FI_REMOTE_WRITE)) {
		ret = cxip_domain_ctrl_id_alloc(dom, &mr->req);
		if (ret) {
			CXIP_WARN("Failed to allocate MR buffer ID: %d\n", ret);
			ofi_spin_destroy(&mr->lock);
			return -FI_ENOSPC;
		}
	} else {
		mr->req.req_id = -1;
	}

	mr->mr_id = -1;
	mr->req.mr.mr = mr;
	mr->mr_fid.mem_desc = (void *)mr;
	mr->mr_state = CXIP_MR_DISABLED;

	return FI_SUCCESS;
}

/*
 * Libfabric MR creation APIs
 */

static int cxip_regattr(struct fid *fid, const struct fi_mr_attr *attr,
			uint64_t flags, struct fid_mr **mr)
{
	struct cxip_domain *dom;
	struct cxip_mr *_mr;
	int ret;

	if (fid->fclass != FI_CLASS_DOMAIN || !attr || attr->iov_count <= 0)
		return -FI_EINVAL;

	if (attr->iov_count != 1)
		return -FI_ENOSYS;

	if (flags & FI_DIRECTED_RECV) {
		CXIP_WARN("FI_DIRECTED_RECV and MRs not supported\n");
		return -FI_EINVAL;
	}

	dom = container_of(fid, struct cxip_domain, util_domain.domain_fid);

	_mr = calloc(1, sizeof(*_mr));
	if (!_mr)
		return -FI_ENOMEM;
	ret = cxip_mr_init(_mr, dom, attr, flags);
	if (ret)
		goto err_free_mr;

	ret = cxip_mr_domain_insert(_mr);
	if (ret)
		goto err_cleanup_mr;

	/* Client key can be set now and will be used to
	 * detect duplicate errors. Note only remote MR
	 * are assigned a RKEY.
	 */
	if (!_mr->domain->is_prov_key &&
	    _mr->attr.access & (FI_REMOTE_READ | FI_REMOTE_WRITE))
		_mr->mr_fid.key = _mr->key;

	if (_mr->len) {
		ret = cxip_map(_mr->domain, (void *)_mr->buf, _mr->len, 0,
			       &_mr->md);
		if (ret) {
			CXIP_WARN("Failed to map MR buffer: %d\n", ret);
			goto err_remove_mr;
		}
	}

	ofi_atomic_inc32(&dom->ref);
	*mr = &_mr->mr_fid;

	return FI_SUCCESS;

err_remove_mr:
	cxip_mr_domain_remove(_mr);

err_cleanup_mr:
	cxip_mr_fini(_mr);
err_free_mr:
	free(_mr);

	return ret;
}

static int cxip_regv(struct fid *fid, const struct iovec *iov, size_t count,
		     uint64_t access, uint64_t offset, uint64_t requested_key,
		     uint64_t flags, struct fid_mr **mr, void *context)
{
	struct fi_mr_attr attr;

	attr.mr_iov = iov;
	attr.iov_count = count;
	attr.access = access;
	attr.offset = offset;
	attr.requested_key = requested_key;
	attr.context = context;

	return cxip_regattr(fid, &attr, flags, mr);
}

static int cxip_reg(struct fid *fid, const void *buf, size_t len,
		    uint64_t access, uint64_t offset, uint64_t requested_key,
		    uint64_t flags, struct fid_mr **mr, void *context)
{
	struct iovec iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;

	return cxip_regv(fid, &iov, 1, access, offset, requested_key, flags, mr,
			 context);
}

struct fi_ops_mr cxip_dom_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = cxip_reg,
	.regv = cxip_regv,
	.regattr = cxip_regattr,
};
