/*
 * Copyright (c) 2022-2023 Hewlett Packard Enterprise Development LP
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 */
#include "config.h"
#include "cxip.h"

#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_EP_CTRL, __VA_ARGS__)

static void cxip_rdzv_pte_cb(struct cxip_pte *pte, const union c_event *event)
{
	switch (pte->state) {
	case C_PTLTE_ENABLED:
		break;
	default:
		CXIP_FATAL("Unexpected state received: %u\n", pte->state);
	}
}

static bool cxip_rdzv_pte_append_done(struct cxip_rdzv_pte *pte,
				      int expected_success_count)
{
	if (ofi_atomic_get32(&pte->le_linked_success_count) ==
	    expected_success_count)
		return true;

	if (ofi_atomic_get32(&pte->le_linked_failure_count) != 0)
		return true;

	return false;
}

static int cxip_rdzv_pte_wait_append(struct cxip_rdzv_pte *pte,
				     uint32_t expected_count)
{
	int ret = FI_SUCCESS;

	/* Poll until the LE is linked or a failure occurs. */
	do {
		cxip_evtq_progress(&pte->txc->base.tx_evtq);
		sched_yield();
	} while (!cxip_rdzv_pte_append_done(pte, expected_count));

	if (ofi_atomic_get32(&pte->le_linked_failure_count)) {
		ret = -FI_EIO;
		CXIP_WARN("Failed to append zero byte put LE: %d:%s\n", ret,
			  fi_strerror(-ret));
	}

	return ret;
}

static void cxip_rdzv_pte_src_reqs_free(struct cxip_rdzv_match_pte *pte)
{
	int i;

	/* The corresponding LE is not freed using an unlink command. Instead,
	 * this logic relies on the freeing of the hardware PtlTE to release the
	 * LEs.
	 */
	for (i = 0; i < RDZV_SRC_LES; i++) {
		if (pte->src_reqs[i])
			cxip_evtq_req_free(pte->src_reqs[i]);
	}
}

/* caller should hold ep_obj->lock */
int cxip_rdzv_pte_src_req_alloc(struct cxip_rdzv_match_pte *pte, int lac)
{
	int ret;
	union cxip_match_bits mb;
	union cxip_match_bits ib;
	uint32_t le_flags;
	int expected_success_count;
	struct cxip_req *req;
	struct cxip_rdzv_pte *base = &pte->base_pte;

	/* Reuse a previously allocated request whenever possible. */
	if (pte->src_reqs[lac])
		return FI_SUCCESS;

	mb.raw = 0;
	mb.rdzv_lac = lac;
	ib.raw = ~0;
	ib.rdzv_lac = 0;
	le_flags = C_LE_UNRESTRICTED_BODY_RO | C_LE_UNRESTRICTED_END_RO |
		C_LE_OP_GET | C_LE_EVENT_UNLINK_DISABLE;

	req = cxip_evtq_req_alloc(&base->txc->base.tx_evtq, 1, base);
	if (!req) {
		ret = -FI_EAGAIN;
		CXIP_WARN("Failed to allocate %d rendezvous source request: %d:%s\n",
			  lac, ret, fi_strerror(-ret));
		return ret;
	}
	req->cb = cxip_rdzv_pte_src_cb;

	expected_success_count =
		ofi_atomic_get32(&base->le_linked_success_count) + 1;

	ret = cxip_pte_append(base->pte, 0, -1ULL, lac, C_PTL_LIST_PRIORITY,
			      req->req_id, mb.raw, ib.raw,
			      CXI_MATCH_ID_ANY, 0, le_flags, NULL,
			      base->txc->rx_cmdq, true);
	if (ret) {
		CXIP_WARN("Failed to issue %d rendezvous source request LE append: %d:%s\n",
			  lac, ret, fi_strerror(-ret));
		goto err_free_req;
	}

	ret = cxip_rdzv_pte_wait_append(base, expected_success_count);
	if (ret != FI_SUCCESS)
		goto err_free_req;

	pte->src_reqs[lac] = req;

	return FI_SUCCESS;

err_free_req:
	cxip_evtq_req_free(req);

	return ret;
}

static void cxip_rdzv_pte_zbp_req_free(struct cxip_rdzv_match_pte *pte)
{
	/* The corresponding LE is not freed using an unlink command. Instead,
	 * this logic relies on the freeing of the hardware PtlTE to release the
	 * LEs.
	 */
	cxip_evtq_req_free(pte->zbp_req);
}

static int cxip_rdzv_pte_zbp_req_alloc(struct cxip_rdzv_match_pte *pte)
{
	uint32_t le_flags = C_LE_UNRESTRICTED_BODY_RO |
		C_LE_UNRESTRICTED_END_RO | C_LE_OP_PUT |
		C_LE_EVENT_UNLINK_DISABLE;
	union cxip_match_bits mb = {
		.le_type = CXIP_LE_TYPE_ZBP,
	};
	union cxip_match_bits ib = {
		.tag = ~0,
		.tx_id = ~0,
		.cq_data = 1,
		.tagged = 1,
		.match_comp = 1,
		.rdzv_done = 1,
	};
	struct cxip_rdzv_pte *base = &pte->base_pte;
	int ret;
	int expected_success_count;

	pte->zbp_req = cxip_evtq_req_alloc(&base->txc->base.tx_evtq, 1, pte);
	if (!pte->zbp_req) {
		ret = -FI_ENOMEM;
		CXIP_WARN("Failed to allocate zero byte put request: %d:%s\n",
			  ret, fi_strerror(-ret));
		return ret;
	}

	pte->zbp_req->cb = cxip_rdzv_pte_zbp_cb;

	expected_success_count =
		ofi_atomic_get32(&base->le_linked_success_count) + 1;

	ret = cxip_pte_append(base->pte, 0, 0, 0, C_PTL_LIST_PRIORITY,
			      pte->zbp_req->req_id, mb.raw, ib.raw,
			      CXI_MATCH_ID_ANY, 0, le_flags, NULL,
			      base->txc->rx_cmdq, true);
	if (ret) {
		CXIP_WARN("Failed to issue zero byte put LE append: %d:%s\n",
			  ret, fi_strerror(-ret));
		goto err_free_req;
	}

	ret = cxip_rdzv_pte_wait_append(base, expected_success_count);
	if (ret != FI_SUCCESS)
		goto err_free_req;

	return FI_SUCCESS;

err_free_req:
	cxip_evtq_req_free(pte->zbp_req);

	return ret;
}

static void cxip_rdzv_pte_free(struct cxip_rdzv_pte *pte)
{
	/* Freeing the PtlTE causes the PtlTE to be reset and all LEs to be
	 * freed. Thus, no need to issue disable and/or unlink commands.
	 */
	cxip_pte_free(pte->pte);

	/* Flush the CQ to ensure any events referencing the rendezvous requests
	 * are processed.
	 */
	cxip_evtq_progress(&pte->txc->base.tx_evtq);
}

void cxip_rdzv_match_pte_free(struct cxip_rdzv_match_pte *pte)
{
	cxip_rdzv_pte_free(&pte->base_pte);

	/* Release all the rendezvous requests. */
	cxip_rdzv_pte_src_reqs_free(pte);
	cxip_rdzv_pte_zbp_req_free(pte);

	free(pte);
}

void cxip_rdzv_nomatch_pte_free(struct cxip_rdzv_nomatch_pte *pte)
{
	cxip_rdzv_pte_free(&pte->base_pte);
	cxip_evtq_req_free(pte->le_req);

	free(pte);
}

static int cxip_rdzv_base_pte_alloc(struct cxip_txc_hpc *txc,
				    uint32_t write_pid_idx, bool write,
				    uint32_t read_pid_idx, bool read,
				    bool matching,
				    struct cxip_rdzv_pte *base_pte)
{
	int ret;
	struct cxi_pt_alloc_opts pt_opts = {
		.is_matching = matching,
	};

	base_pte->txc = txc;
	ofi_atomic_initialize32(&base_pte->le_linked_success_count, 0);
	ofi_atomic_initialize32(&base_pte->le_linked_failure_count, 0);

	if (matching && txc->base.ep_obj->av->symmetric)
		pt_opts.use_logical = 1;

	/* Reserve the Rendezvous Send PTE */
	ret = cxip_pte_alloc_nomap(txc->base.ep_obj->ptable,
				   txc->base.tx_evtq.eq,
				   &pt_opts, cxip_rdzv_pte_cb, txc,
				   &base_pte->pte);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to alloc base rendezvous PtlTE: %d:%s\n",
			  ret, fi_strerror(-ret));
		return ret;
	}

	if (write) {
		ret = cxip_pte_map(base_pte->pte, write_pid_idx, false);
		if (ret != FI_SUCCESS) {
			CXIP_WARN("Failed to map write PTE: %d\n", ret);
			goto err_free_rdzv_pte;
		}
	}

	if (read) {
		ret = cxip_pte_map(base_pte->pte, read_pid_idx, false);
		if (ret != FI_SUCCESS) {
			CXIP_WARN("Failed to map read PTE: %d\n", ret);
			goto err_free_rdzv_pte;
		}
	}

	/* Set to enable, event will be processed on link */
	ret = cxip_pte_set_state(base_pte->pte, txc->rx_cmdq,
				 C_PTLTE_ENABLED, 0);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to enqueue enable command: %d:%s\n", ret,
			  fi_strerror(-ret));
		goto err_free_rdzv_pte;
	}

	return FI_SUCCESS;

err_free_rdzv_pte:
	cxip_pte_free(base_pte->pte);
	base_pte->pte = NULL;

	return ret;
}

/* ep_obj->lock should be held by caller */
int cxip_rdzv_match_pte_alloc(struct cxip_txc_hpc *txc,
			      struct cxip_rdzv_match_pte **rdzv_pte)
{
	int ret;
	struct cxip_rdzv_match_pte *match_pte;
	uint32_t pid_idx = txc->base.domain->iface->dev->info.rdzv_get_idx;
	struct cxip_rdzv_pte *base;

	match_pte = calloc(1, sizeof(*match_pte));
	if (!match_pte) {
		ret = -ENOMEM;
		CXIP_WARN("Rendzvous matching PtlTE allocation error: %d:%s\n",
			  ret, fi_strerror(-ret));
		return ret;
	}

	ret = cxip_rdzv_base_pte_alloc(txc, pid_idx, true,
				       CXIP_PTL_IDX_RDZV_DEST, true,
				       true, &match_pte->base_pte);
	if (ret != FI_SUCCESS)
		goto err_free_rdzv_pte_mem;

	/* Matching specific initialization */
	base = &match_pte->base_pte;

	ret = cxip_rdzv_pte_zbp_req_alloc(match_pte);
	if (ret) {
		CXIP_WARN("Failed to allocate zero byte put request: %d:%s\n",
			  ret, fi_strerror(-ret));
		goto err_free_rdzv_pte;
	}
	*rdzv_pte = match_pte;

	return FI_SUCCESS;

err_free_rdzv_pte:
	cxip_pte_free(base->pte);
err_free_rdzv_pte_mem:
	free(match_pte);

	return ret;
}

/* ep_obj->lock should be held by caller */
int cxip_rdzv_nomatch_pte_alloc(struct cxip_txc_hpc *txc, int lac,
				struct cxip_rdzv_nomatch_pte **rdzv_pte)
{
	int ret;
	struct cxip_rdzv_nomatch_pte *nomatch_pte;
	struct cxip_rdzv_pte *base;
	uint32_t le_flags;
	uint32_t expected_success_count;
	uint64_t ib = 0;
	uint32_t pid_idx = CXIP_PTL_IDX_RDZV_RESTRICTED(lac);

#if ENABLE_DEBUG
	/* Enable testing of fallback to default rendezvous protocol
	 * if unable to allocate required non-matching PTE/LE resources.
	 */
	if (txc->force_err & CXIP_TXC_FORCE_ERR_ALT_READ_PROTO_ALLOC)
		return -FI_ENOSPC;
#endif
	nomatch_pte = calloc(1, sizeof(*nomatch_pte));
	if (!nomatch_pte) {
		ret = -ENOMEM;
		CXIP_WARN("Rdzv restricted PtlTE allocation error: %d:%s\n",
			  ret, fi_strerror(-ret));
		return ret;
	}

	ret = cxip_rdzv_base_pte_alloc(txc, 0, false, pid_idx, true,
				       false, &nomatch_pte->base_pte);
	if (ret != FI_SUCCESS)
		goto err_free_rdzv_pte_mem;

	/* Non-matching specific initialization */
	base = &nomatch_pte->base_pte;

	nomatch_pte->le_req = cxip_evtq_req_alloc(&txc->base.tx_evtq, 1,
						  nomatch_pte);
	if (!nomatch_pte->le_req) {
		ret = -FI_EAGAIN;
		CXIP_WARN("Rdzv PtlTE LAC %d request allocation error: %d:%s\n",
			  lac, ret, fi_strerror(-ret));
		goto err_free_rdzv_pte;
	}

	nomatch_pte->le_req->cb = cxip_rdzv_pte_src_cb;

	le_flags = C_LE_EVENT_COMM_DISABLE | C_LE_EVENT_SUCCESS_DISABLE |
		   C_LE_UNRESTRICTED_BODY_RO | C_LE_OP_GET;
	ib = 1;
	expected_success_count =
		ofi_atomic_get32(&base->le_linked_success_count) + 1;

	ret = cxip_pte_append(base->pte, 0, -1ULL, lac, C_PTL_LIST_PRIORITY,
			      nomatch_pte->le_req->req_id, 0, ib,
			      CXI_MATCH_ID_ANY, 0, le_flags, NULL,
			      txc->rx_cmdq, true);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to enqueue append cmd: %d:%s\n",
			  ret, fi_strerror(-ret));
		goto err_free_rdzv_pte;
	}

	ret = cxip_rdzv_pte_wait_append(base, expected_success_count);
	if (ret != FI_SUCCESS)
		goto err_free_req;

	*rdzv_pte = nomatch_pte;
	return FI_SUCCESS;

err_free_req:
	cxip_evtq_req_free(nomatch_pte->le_req);
err_free_rdzv_pte:
	cxip_pte_free(nomatch_pte->base_pte.pte);
err_free_rdzv_pte_mem:
	free(nomatch_pte);

	return ret;
}
