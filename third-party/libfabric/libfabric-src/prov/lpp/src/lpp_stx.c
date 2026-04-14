/*
 * Copyright (c) 2018-2024 GigaIO, Inc. All Rights Reserved.
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

#include "lpp.h"

static const struct fi_ops lpp_fi_stx_ops = {
	.size           = sizeof(struct fi_ops),
	.close          = lpp_fi_stx_close,
	.bind           = fi_no_bind,
	.control        = fi_no_control,
	.ops_open       = fi_no_ops_open,
};

static const struct fi_ops_ep lpp_stx_ops = {
	.size           = sizeof(struct fi_ops_ep),
	.cancel         = lpp_fi_cancel,
	.getopt         = fi_no_getopt,
	.setopt         = fi_no_setopt,
	.tx_ctx         = fi_no_tx_ctx,
	.rx_ctx         = fi_no_rx_ctx,
	.rx_size_left   = fi_no_rx_size_left,
	.tx_size_left   = fi_no_tx_size_left,
};

static const struct fi_tx_attr lpp_default_tx_attr = {
	.caps           = LPP_CAPS,
	.mode           = 0,
	.op_flags       = 0,
	.msg_order      = FI_ORDER_RAR | FI_ORDER_RAW | FI_ORDER_WAR | FI_ORDER_WAW | FI_ORDER_SAS,
	.comp_order     = 0,
	.inject_size    = 0,
	.size           = LPP_TX_CTX_MAX_COUNT,
	.iov_limit      = KLPP_MAX_SEND_IOVS,
	.rma_iov_limit  = KLPP_MAX_RMA_IOVS,
};

static size_t lpp_tx_max_eager(const struct klppioc_lf *klpp_devinfo)
{
	const struct klpp_mc_params *mc_params = &klpp_devinfo->mc_params;

	/* Check the number of cells -1 here to leave an extra cell for
	 * alignment purposes. For larger payloads, the payload is bumped to
	 * the start of the next cell for optimal memcpy performance. */
	return MIN(mc_params->srq_cell_cnt - 1, mc_params->dqp_cell_cnt - 1) *
	       KLPP_MQ_CELL_SIZE;
}

static size_t lpp_tx_inject_size(const struct klppioc_lf *klpp_devinfo)
{
	// TODO: return MIN(lpp_tx_max_eager(klpp_devinfo), lpp_max_eager_size);
	return LPP_MAX_INJECT;
}

static inline void lpp_tx_entry_dereg_mrs(struct lpp_tx_entry *tx_entry)
{
	for (int i = 0; i < tx_entry->iov_count; i++) {
		if (tx_entry->mrp[i])
			lpp_mr_close_internal(tx_entry->mrp[i]);
		tx_entry->mrp[i] = NULL;
	}

	tx_entry->lpp_flags &= ~LPP_TX_ENTRY_MR;
}

static inline int lpp_tx_entry_reg_mrs(struct lpp_domain *domain, struct lpp_tx_entry *tx_entry)
{
	int ret;

	assert(tx_entry->iov_count < KLPP_MAX_SEND_IOVS);
	for (int i = 0; i < tx_entry->iov_count; i++) {
		ret = lpp_mr_reg_internal(domain, tx_entry->msg_iov[i].iov_base,
					  tx_entry->msg_iov[i].iov_len,
					  FI_SEND, 0, tx_entry->hmem_iface,
					  NULL, &tx_entry->mrp[i]);
		if (ret)
			goto err_dereg;
	}
	tx_entry->lpp_flags |= LPP_TX_ENTRY_MR;
	return 0;

err_dereg:
	lpp_tx_entry_dereg_mrs(tx_entry);
	return ret;
}


struct fi_tx_attr lpp_tx_attrs(const struct klppioc_lf *klpp_devinfo)
{
	struct fi_tx_attr tmp = lpp_default_tx_attr;

	/* TODO what about for RMA inject? One option would be return EAGAIN if there are ongoing
	 * DMAs (to prevent ordering violations). That would simplify the kernel, certainly. The
	 * other option would be to actually allocate a tmp buffer and store the inject data until
	 * it's safe to TX. tmp.inject_size = klpp_devinfo->max_inject_size; */
	tmp.inject_size = lpp_tx_inject_size(klpp_devinfo);

	return tmp;
}

int lpp_verify_tx_attrs(const struct klppioc_lf *klpp_devinfo, struct fi_tx_attr *tx_attrs)
{

#define CHECK_ATTR(have, op, cmp)									\
	if ((have) op (cmp)) {										\
		FI_INFO(&lpp_prov, FI_LOG_FABRIC, "attr check failed: " #have " " #op " " #cmp " \n");	\
		return -FI_EINVAL;									\
	}

	CHECK_ATTR((tx_attrs->caps & ~LPP_CAPS),          !=,  0);
	CHECK_ATTR((tx_attrs->mode & LPP_MODE),           !=,  LPP_MODE);
	CHECK_ATTR((tx_attrs->op_flags & ~LPP_OP_FLAGS),  !=,  0);
	CHECK_ATTR((tx_attrs->msg_order & ~LPP_ORDER),    !=,  0);
	CHECK_ATTR(tx_attrs->comp_order,                  !=,  0);
	CHECK_ATTR(tx_attrs->inject_size,                 >,   lpp_tx_inject_size(klpp_devinfo));
	CHECK_ATTR(tx_attrs->size,                        >,   LPP_TX_CTX_MAX_COUNT);
	CHECK_ATTR(tx_attrs->iov_limit,                   >,   KLPP_MAX_SEND_IOVS);
	CHECK_ATTR(tx_attrs->rma_iov_limit,               >,   KLPP_MAX_RMA_IOVS);

#undef CHECK_ATTR

	return 0;
}

void lpp_tx_populate_defaults(const struct klppioc_lf *klpp_devinfo, struct fi_tx_attr *tx_attrs)
{
	if (tx_attrs->inject_size == 0)
		tx_attrs->inject_size = lpp_tx_inject_size(klpp_devinfo);

	if (tx_attrs->size == 0)
		tx_attrs->size = LPP_TX_CTX_MAX_COUNT;

	if (tx_attrs->iov_limit == 0)
		tx_attrs->iov_limit = KLPP_MAX_SEND_IOVS;

	if (tx_attrs->rma_iov_limit == 0)
		tx_attrs->rma_iov_limit = KLPP_MAX_RMA_IOVS;

	if ((tx_attrs->op_flags &
	     (FI_TRANSMIT_COMPLETE | FI_DELIVERY_COMPLETE)) == 0)
		tx_attrs->op_flags |= FI_DELIVERY_COMPLETE;
}

struct lpp_stx *lpp_stx_alloc(struct lpp_domain *lpp_domainp, struct fi_tx_attr *attr, void *context)
{
	struct lpp_stx *lpp_stxp = NULL;

	if (lpp_stxp = calloc(1, sizeof(struct lpp_stx)), lpp_stxp == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "failed to alloc stx\n");
		return NULL;
	}

	lpp_stxp->stx_fid.fid.fclass = FI_CLASS_STX_CTX;
	lpp_stxp->stx_fid.fid.context = context;
	lpp_stxp->stx_fid.fid.ops = (struct fi_ops *)&lpp_fi_stx_ops;

	lpp_stxp->stx_fid.ops = (struct fi_ops_ep *)&lpp_stx_ops;
	lpp_stxp->domain = lpp_domainp;

	if (attr == NULL)
		lpp_stxp->attr = lpp_tx_attrs(&lpp_domainp->devinfo);
	else
		lpp_stxp->attr = *attr;

	// Set some defaults for the user.
	lpp_tx_populate_defaults(&lpp_domainp->devinfo, &lpp_stxp->attr);

	if (lpp_max_eager_size > lpp_tx_max_eager(&lpp_domainp->devinfo)) {
		FI_WARN(&lpp_prov, FI_LOG_DOMAIN,
		    "requested max eager size %ld exceeds max possible %ld\n",
		    lpp_max_eager_size, lpp_tx_max_eager(&lpp_domainp->devinfo));
		lpp_max_eager_size = lpp_tx_max_eager(&lpp_domainp->devinfo);
	}

	dlist_init(&lpp_stxp->tx_entry_freelist);
	dlist_init(&lpp_stxp->tx_entry_unsent);
	dlist_init(&lpp_stxp->tx_entry_atomic_unsent);
	dlist_init(&lpp_stxp->tx_entry_dqp);
	dlist_init(&lpp_stxp->tx_entry_srq);
	lpp_stxp->tx_entries = calloc(lpp_stxp->attr.size,
	    sizeof(*lpp_stxp->tx_entries));
	if (lpp_stxp->tx_entries == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "failed to alloc tx_entries\n");
		goto err_free_stx;
	}
	for (size_t i = 0; i < lpp_stxp->attr.size; i++) {
		dlist_init(&lpp_stxp->tx_entries[i].dle);
		dlist_insert_tail(&lpp_stxp->tx_entries[i].dle,
		    &lpp_stxp->tx_entry_freelist);
	}

	// Create the context for this STX.
	if (lpp_ctx_create(lpp_domainp, &lpp_stxp->ctx, KLPP_CTX_STX, lpp_stxp->attr.size) != 0) {
		goto err_free_tx_entries;
	}

	lpp_stxp->id = lpp_stxp->ctx.id;
	ofi_atomic_initialize32(&lpp_stxp->ref_count, 0);
	ofi_atomic_inc32(&lpp_stxp->domain->util_domain.ref);

	return lpp_stxp;

err_free_tx_entries:
	free(lpp_stxp->tx_entries);
err_free_stx:
	free(lpp_stxp);
	return NULL;
}

int lpp_fi_stx_context(struct fid_domain *domain, struct fi_tx_attr *attr, struct fid_stx **stx, void *context)
{
	struct util_domain	*util_domain;
	struct lpp_domain	*lpp_domainp;
	struct lpp_stx		*lpp_stxp = NULL;

	if (domain == NULL)
		return -FI_EINVAL;

	util_domain = container_of(domain, struct util_domain, domain_fid);
	lpp_domainp = container_of(util_domain, struct lpp_domain, util_domain);

	if ((attr != NULL) && (lpp_verify_tx_attrs(&lpp_domainp->devinfo, attr) != 0))
		return -FI_EINVAL;

	if (lpp_stxp = lpp_stx_alloc(lpp_domainp, attr, context), lpp_stxp == NULL)
		return -FI_EINVAL;

	*stx = &lpp_stxp->stx_fid;

	return 0;
}

int lpp_stx_close(struct lpp_stx *lpp_stxp)
{
	int ret;

	// Check the reference count.  If we are not last, just return.
	if (ofi_atomic_get32(&lpp_stxp->ref_count) != 0)
		return 0;

	// Tell KLPP so we can free up KLPP resources.
	ret = klpp_ctx_close(&lpp_stxp->ctx);
	if (ret != 0) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA, "STX context close returned %d\n", ret);
	}

	// Close the CNTR.
	if (lpp_stxp->cntr != NULL) {
		ofi_atomic_dec32(&lpp_stxp->cntr->ref_count);
		lpp_stxp->cntr = NULL;
	}

	// Close the CQ.
	if (lpp_stxp->cq != NULL) {
		ofi_atomic_dec32(&lpp_stxp->cq->ref_count);
		lpp_stxp->cq = NULL;
	}

	ofi_atomic_dec32(&lpp_stxp->domain->util_domain.ref);
	free(lpp_stxp->tx_entries);
	free(lpp_stxp);
	return 0;
}

int lpp_fi_stx_close(struct fid *fid)
{
	struct lpp_stx *lpp_stxp = NULL;

	if (fid == NULL)
		return -FI_EINVAL;

	lpp_stxp = container_of(fid, struct lpp_stx, stx_fid);
	return lpp_stx_close(lpp_stxp);
}

static void lpp_tx_rdzv_cancel(struct lpp_ep *lpp_epp, struct lpp_tx_entry *tx_entry)
{
	struct lpp_stx *lpp_stxp = lpp_epp->stx;
	struct klpp_umc_u2k u2k;

	u2k.type = KLPP_U2K_RDZV_SEND_CANCEL;
	u2k.rdzv_send.token = lpp_tx_entry_to_token(lpp_stxp, tx_entry);
	lpp_umc_u2k_enqueue(lpp_epp, &u2k);
	tx_entry->lpp_flags &= ~LPP_TX_ENTRY_RDZV_PEND;
}

/* TODO: util_buf_alloc()
 * If threading == FI_THREAD_SAFE, must hold ep "op lock". */
struct lpp_tx_entry *lpp_tx_entry_get(struct lpp_stx *lpp_stxp)
{
	struct lpp_tx_entry *tx_entry;

	if (dlist_empty(&lpp_stxp->tx_entry_freelist)) {
		return NULL;
	} else {
		dlist_pop_front(&lpp_stxp->tx_entry_freelist,
		    struct lpp_tx_entry, tx_entry, dle);
		dlist_init(&tx_entry->dle);
		tx_entry->generation++;
		return tx_entry;
	}
}

static void lpp_tx_entry_put(struct lpp_ep *lpp_epp, struct lpp_tx_entry *tx_entry)
{
	struct lpp_stx *lpp_stxp = lpp_epp->stx;
	struct lpp_cq *lpp_cqp = lpp_epp->cq_transmit;
	struct lpp_umc_remote *rumc;
	struct lpp_fi_addr *dst_addr;

	if (tx_entry->lpp_flags & LPP_TX_ENTRY_RDZV_PEND)
		lpp_tx_rdzv_cancel(lpp_epp, tx_entry);

	if (tx_entry->lpp_flags & LPP_TX_ENTRY_ONLIST)
		dlist_remove_init(&tx_entry->dle);

	if (tx_entry->lpp_flags & LPP_TX_ENTRY_SRQ) {
		dst_addr = &tx_entry->dst_addr.fi_addr;
		rumc = lpp_rumc_get(lpp_epp, dst_addr->node_id,
		    dst_addr->umc_id);
		rumc->srq_refcount--;
		if (rumc->srq_refcount == 0) {
			dlist_remove(&rumc->dle);
		}
	}
	if (tx_entry->lpp_flags & LPP_TX_ENTRY_CQ_RSRV)
		lpp_cq_unreserve_slot(lpp_cqp);

	if (tx_entry->lpp_flags & LPP_TX_ENTRY_MR)
		lpp_tx_entry_dereg_mrs(tx_entry);

	if (tx_entry->gpu_stage_buf) {
		ofi_hmem_host_unregister(tx_entry->gpu_stage_buf);
		free(tx_entry->gpu_stage_buf);
		tx_entry->gpu_stage_buf = NULL;
	}

	tx_entry->flags = 0;
	tx_entry->lpp_flags = 0;
	tx_entry->hmem_iface = 0;
	dlist_insert_head(&tx_entry->dle, &lpp_stxp->tx_entry_freelist);
}

// TODO there is an ofi_key or something that does this some sort of thing
uint64_t lpp_tx_entry_to_token(struct lpp_stx *lpp_stxp,
			       struct lpp_tx_entry *tx_entry)
{
	uint64_t token;
	uint32_t idx;

	idx = (tx_entry - lpp_stxp->tx_entries);
	token = ((uint64_t)tx_entry->generation << 32) | idx;
	return token;
}

static struct lpp_tx_entry *lpp_token_to_tx_entry(struct lpp_stx *lpp_stxp,
						  uint64_t token)
{
	uint32_t idx;
	uint32_t generation;
	struct lpp_tx_entry *tx_entry;

	idx = token & 0xffffffff;
	generation = token >> 32;

	if (idx > lpp_stxp->attr.size) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
		    "Out of bounds idx: %u (max %ld)\n",
		    idx, lpp_stxp->attr.size);
		return NULL;
	}

	tx_entry = &lpp_stxp->tx_entries[idx];
	if (tx_entry->flags == 0) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
		    "tx_entry %u is not initialized\n",
		    idx);
		return NULL;
	}
	if (tx_entry->generation != generation) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
		    "token is stale (token gen %u != tx_entry gen %u)\n",
		    generation, tx_entry->generation);
		return NULL;
	}

	return tx_entry;
}

/* TODO: basically the same as process_deliv_comp, but the latter might
 * eventually have a potentially non-zero status.
 * If threading == FI_THREAD_SAFE, must hold ep "op lock". */
void lpp_tx_process_discard(struct lpp_ep *lpp_epp, struct klpp_msg_hdr *hdr)
{
	struct lpp_stx *lpp_stxp = lpp_epp->stx;
	struct lpp_tx_entry *tx_entry;

	tx_entry = lpp_token_to_tx_entry(lpp_stxp, hdr->token);
	if (tx_entry == NULL)
		return;

	lpp_completion_transmit(lpp_epp, lpp_epp->cntr_send, tx_entry->flags,
				tx_entry->msg_hdr.rcq_data, tx_entry->context, 0);
	lpp_tx_entry_put(lpp_epp, tx_entry);
}

void lpp_tx_process_atomic_deliv_comp(struct lpp_ep *lpp_epp, struct klpp_msg_hdr *hdr)
{
	struct lpp_stx *lpp_stxp = lpp_epp->stx;
	struct lpp_tx_entry *tx_entry;

	tx_entry = lpp_token_to_tx_entry(lpp_stxp, hdr->token);
	if (tx_entry == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
		       "deliv complete for invalid token %llx\n", hdr->token);
		return;
	}
	lpp_atomic_handle_cmpl(lpp_epp, tx_entry, hdr);
	lpp_tx_entry_put(lpp_epp, tx_entry);
}

// If threading == FI_THREAD_SAFE, must hold ep "op lock".
void lpp_tx_process_deliv_comp(struct lpp_ep *lpp_epp, struct klpp_msg_hdr *hdr)
{
	struct lpp_stx *lpp_stxp = lpp_epp->stx;
	struct lpp_tx_entry *tx_entry;

	tx_entry = lpp_token_to_tx_entry(lpp_stxp, hdr->token);
	if (tx_entry == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
		       "deliv complete for invalid token %llx\n", hdr->token);
		return;
	}
	lpp_completion_transmit(lpp_epp, lpp_epp->cntr_send, tx_entry->flags,
				tx_entry->msg_hdr.rcq_data, tx_entry->context,
				hdr->status);
	lpp_tx_entry_put(lpp_epp, tx_entry);
}

// If threading == FI_THREAD_SAFE, must hold ep "op lock".
void lpp_tx_process_invalid_dst(struct lpp_ep *lpp_epp, struct klpp_msg_hdr *hdr)
{
	FI_INFO(&lpp_prov, FI_LOG_EP_CTRL,
		"%u:%u path down due to INVALID_DST reply\n",
		hdr->src_ep.fi_addr.node_id,  hdr->src_ep.fi_addr.umc_id);
	lpp_stx_path_down(lpp_epp, hdr->src_ep.fi_addr.node_id,
			  hdr->src_ep.fi_addr.umc_id);
}

ssize_t lpp_tx_verify_flags(struct lpp_stx *lpp_stxp, uint64_t flags)
{
	if ((lpp_stxp->attr.caps & flags & LPP_CAPS_OPS) !=
	    (flags & LPP_CAPS_OPS)) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
			"TX attr caps (%lx) incorrect for requested op (%lx)\n",
			lpp_stxp->attr.caps, flags);
		return -FI_EINVAL;
	}
	return 0;
}

ssize_t lpp_tx_verify_rma_iov_count(struct lpp_stx *lpp_stxp, size_t iov_count)
{
	if (iov_count > lpp_stxp->attr.rma_iov_limit) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
			"iov_count out of range (%ld/%ld)\n", iov_count,
			lpp_stxp->attr.rma_iov_limit);
		return -FI_EINVAL;
	}
	return 0;
}

ssize_t lpp_tx_verify_iov_count(struct lpp_stx *lpp_stxp, size_t iov_count)
{
	if (iov_count > lpp_stxp->attr.iov_limit) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
			"iov_count out of range (%ld/%ld)\n", iov_count,
			lpp_stxp->attr.iov_limit);
		return -FI_EINVAL;
	}
	return 0;
}

static int lpp_stx_tx_common(struct lpp_ep *lpp_epp, struct lpp_tx_entry *tx_entry,
			     struct lpp_cntr *cntr)
{
	struct lpp_stx *lpp_stxp = lpp_epp->stx;
	struct lpp_fi_addr *dst_addr = &tx_entry->dst_addr.fi_addr;
	struct lpp_umc *umc = lpp_epp->umc;
	struct lpp_umc_remote *rumc;
	int ret;

	/* TODO: if the destination has unflushed completions, should this
	 * function return EAGAIN (and try to flush the completions?) Seems
	 * rather pathological to continue filling up the queue while starving
	 * out completions. Or is such a problem better solved by adjusting
	 * queue size to avoid any starvation issues? */
	rumc = lpp_rumc_get_insert(lpp_epp, dst_addr->node_id,
				   dst_addr->umc_id);
	if (rumc->dqp_idx != LPP_DQP_UNSET) {
		ret = lpp_umc_dqp_tx(lpp_epp, rumc, &tx_entry->msg_hdr,
				     tx_entry->msg_iov, tx_entry->iov_count);
		if (ret == 0) {
			if (tx_entry->msg_hdr.flags & KLPP_MSG_TOKEN) {
				dlist_insert_tail(&tx_entry->dle,
						  &lpp_stxp->tx_entry_dqp);
				tx_entry->lpp_flags |= LPP_TX_ENTRY_ONLIST;
			} else {
				lpp_completion_transmit(lpp_epp, cntr,
							tx_entry->flags,
							tx_entry->msg_hdr.rcq_data,
							tx_entry->context, ret);
				lpp_tx_entry_put(lpp_epp, tx_entry);
			}
		}
	} else {
		ret = klpp_kmc_send_one(lpp_epp, *dst_addr,
		    &tx_entry->msg_hdr, tx_entry->msg_iov, tx_entry->iov_count);
		if (ret == 0 && (tx_entry->msg_hdr.flags & KLPP_MSG_TOKEN)) {
			if (rumc->srq_refcount == 0) {
				/* Since we weren't keeping track of this rumc
				 * previously, reset rx_time. We want to start
				 * the keep alive timer starting now, not
				 * whenever we last cared about this rumc (i.e.,
				 * the old value of last_rx_time). */
				rumc->last_rx_time = lpp_get_time_ms();
				rumc->umc_gen = dst_addr->umc_gen;
				dlist_insert_tail(&rumc->dle,
						  &umc->keep_alive_rumcs);
			} else if (rumc->umc_gen != dst_addr->umc_gen) {
				/* TODO: Review this. There might be some
				 * subtlety here that's not being handled
				 * right.  E.g., what if the the msg this guy
				 * is trying to TX is targeting an old
				 * generation, not new? The path may be
				 * perfectly valid, just this particular msg
				 * has an old address. */
				FI_INFO(&lpp_prov, FI_LOG_EP_CTRL,
					"declaring path down, generations do not match %u != %u, srq_refcount %d nd%u.usr%u.umc%u.ep%u\n",
					rumc->umc_gen, dst_addr->umc_gen,
					rumc->srq_refcount, dst_addr->node_id,
					dst_addr->user_id, dst_addr->umc_id,
					dst_addr->ep_id);
				lpp_stx_path_down(lpp_epp, dst_addr->node_id, dst_addr->umc_id);
			}
			rumc->srq_refcount++;
			dlist_insert_tail(&tx_entry->dle,
					  &lpp_stxp->tx_entry_srq);
			tx_entry->lpp_flags |= (LPP_TX_ENTRY_SRQ | LPP_TX_ENTRY_ONLIST);
		} else if (ret == 0) {
			lpp_completion_transmit(lpp_epp, cntr, tx_entry->flags,
						tx_entry->msg_hdr.rcq_data,
						tx_entry->context, ret);
			lpp_tx_entry_put(lpp_epp, tx_entry);
		}
	}

	return ret;
}

// If threading == FI_THREAD_SAFE, must hold ep "op lock".
static int lpp_stx_flush_unsent(struct lpp_ep *lpp_epp,
				struct dlist_entry *unsent_list,
				struct lpp_cntr *cntr)
{
	struct lpp_tx_entry *tx_entry;
	int ret;

	/* TODO: should we store the unsents on the umc_remote? That way we can
	 * keep the streams independent and avoid HOL blocking. Having it on
	 * that list would also facilitate timing out msgs that have gotten
	 * EAGAIN for too long. But having a way to quickly iterate over all
	 * unsents is likely still needed.
	 * TODO: unsent items should timeout after a while */
	while (!dlist_empty(unsent_list)) {
		dlist_pop_front(unsent_list, struct lpp_tx_entry, tx_entry,
				dle);
		dlist_init(&tx_entry->dle);
		tx_entry->lpp_flags &= ~LPP_TX_ENTRY_ONLIST;
		ret = lpp_stx_tx_common(lpp_epp, tx_entry, cntr);
		if (ret == -FI_EAGAIN) {
			dlist_insert_head(&tx_entry->dle, unsent_list);
			tx_entry->lpp_flags |= LPP_TX_ENTRY_ONLIST;
			return -FI_EAGAIN;
		} else if (ret != 0) {
			lpp_completion_transmit(lpp_epp, lpp_epp->cntr_send,
						tx_entry->flags,
						tx_entry->msg_hdr.rcq_data,
						tx_entry->context, ret);
			lpp_tx_entry_put(lpp_epp, tx_entry);
		}
	}
	return 0;
}

static int lpp_stx_flush_msg_unsent(struct lpp_ep *lpp_epp)
{
	struct lpp_stx *lpp_stxp = lpp_epp->stx;

	return lpp_stx_flush_unsent(lpp_epp, &lpp_stxp->tx_entry_unsent,
				   lpp_epp->cntr_send);
}

static int lpp_stx_flush_atomic_unsent(struct lpp_ep *lpp_epp)
{
	struct lpp_stx *lpp_stxp = lpp_epp->stx;

	return lpp_stx_flush_unsent(lpp_epp, &lpp_stxp->tx_entry_atomic_unsent,
				   lpp_epp->cntr_write);
}

static void lpp_stx_copy_inject(struct lpp_tx_entry *tx_entry)
{
	uint8_t *buf = tx_entry->inject_data;

	if (buf == tx_entry->msg_iov[0].iov_base) // copy has already happened
		return;

	for (size_t i = 0; i < tx_entry->iov_count; i++) {
		memcpy(buf, tx_entry->msg_iov[i].iov_base,
		       tx_entry->msg_iov[i].iov_len);
		buf += tx_entry->msg_iov[i].iov_len;
	}
	tx_entry->msg_iov[0].iov_base = tx_entry->inject_data;
	tx_entry->msg_iov[0].iov_len = buf - tx_entry->inject_data;
	tx_entry->iov_count = 1;
}

static int lpp_stx_copy_inject_hmem(int iface, struct lpp_tx_entry *tx_entry)
{
	uint8_t *buf = tx_entry->inject_data;
	int ret;
	ret = ofi_copy_from_hmem(iface, -1, buf,
		tx_entry->msg_iov[0].iov_base,
		tx_entry->msg_iov[0].iov_len);
	if (ret) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA, "HMEM copy failed\n");
		return ret;
	}
	tx_entry->msg_iov[0].iov_base = tx_entry->inject_data;
	tx_entry->iov_count = 1;
	FI_DBG(&lpp_prov, FI_LOG_EP_DATA, "GPU data coppied to inject buf\n");
	return 0;
}

int lpp_stx_hmem_stage(struct lpp_ep *lpp_ep, struct lpp_tx_entry *tx_entry,
		       uint32_t data_length)
{
	tx_entry->gpu_stage_buf = malloc(data_length);
	if (!tx_entry->gpu_stage_buf)
		return -FI_ENOMEM;

	if (ofi_hmem_host_register(tx_entry->gpu_stage_buf, data_length)) {
		free(tx_entry->gpu_stage_buf);
		return -FI_EINVAL;
	}

	if (ofi_copy_from_hmem(lpp_ep->domain->enabled_iface,
				-1, tx_entry->gpu_stage_buf,
				tx_entry->msg_iov[0].iov_base,
				tx_entry->msg_iov[0].iov_len)) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
			"Failed to set up stage buffer\n");
		free(tx_entry->gpu_stage_buf);
		return -FI_EINVAL;
	}
	tx_entry->msg_iov[0].iov_base = tx_entry->gpu_stage_buf;
	FI_DBG(&lpp_prov, FI_LOG_EP_DATA, "Set up stage buf (%p, len: %d)\n",
			tx_entry->gpu_stage_buf, data_length);
	return 0;
}

static inline void lpp_tx_entry_clear_gpu(struct lpp_tx_entry *tx_entry)
{
	tx_entry->lpp_flags &= ~LPP_TX_ENTRY_GPU;
	tx_entry->hmem_iface = 0;
}

static inline bool lpp_tx_entry_gpu(struct lpp_tx_entry *tx_entry)
{

#if HAVE_CUDA || HAVE_ROCR
	return (tx_entry->lpp_flags & LPP_TX_ENTRY_GPU) != 0;
#else
	return false;
#endif
}

// If threading == FI_THREAD_SAFE, must hold ep "op lock".
ssize_t lpp_send_common(struct lpp_ep *lpp_epp, struct lpp_tx_entry *tx_entry)
{
	struct lpp_stx *lpp_stxp = lpp_epp->stx;
	struct lpp_cq *lpp_cqp = lpp_epp->cq_transmit;
	struct klpp_umc_u2k u2k;
	size_t gpu_cnt;
	ssize_t ret;

	tx_entry->msg_hdr.data_length = 0;
	gpu_cnt = 0;
	for (size_t i = 0; i < tx_entry->iov_count; i++) {
		tx_entry->msg_hdr.data_length += tx_entry->msg_iov[i].iov_len;
		ret = lpp_hmem_ptr(lpp_epp->domain, tx_entry->msg_iov[i].iov_base,
				   &tx_entry->hmem_iface);
		if (ret) {
			FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
					"Failed to determine tx src hmem");
			goto err_no_spool;
		}

		if (tx_entry->hmem_iface) {
			gpu_cnt++;
			if (tx_entry->msg_hdr.data_length)
				tx_entry->lpp_flags |= LPP_TX_ENTRY_GPU;
		}

		// TODO: fix
		if (gpu_cnt > 1) {
			FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
				"GPU buffers may only have 1 iov\n");
			ret = -FI_EINVAL;
			goto err_no_spool;
		}

		// TODO: fix
		if (i && gpu_cnt) {
			FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
				"mixed GPU and host iovs are illegal\n");
			ret = -FI_EINVAL;
			goto err_no_spool;
		}

	}

	ret = lpp_cq_reserve_slot(lpp_cqp, tx_entry->flags);
	if (ret > 0) {
		tx_entry->lpp_flags |= LPP_TX_ENTRY_CQ_RSRV;
	} else if (ret < 0) {
		FI_INFO(&lpp_prov, FI_LOG_EP_DATA,
		       "cannot start TX operation as it may overrun CQ\n");
		ret = -FI_EAGAIN;
		goto err_no_spool;
	}

	if (tx_entry->flags & FI_INJECT) {
		if (tx_entry->msg_hdr.data_length > lpp_stxp->attr.inject_size) {
			FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
				"length %u exceeds max inject size %ld\n",
				tx_entry->msg_hdr.data_length,
				lpp_stxp->attr.inject_size);
			ret = -FI_EINVAL;
			goto err_no_spool;
		}

		if (lpp_tx_entry_gpu(tx_entry)) {
			ret = lpp_stx_copy_inject_hmem(
				lpp_epp->domain->enabled_iface, tx_entry);
			if (ret)
				goto err_no_spool;
			lpp_tx_entry_clear_gpu(tx_entry);
		}

	}

	if (lpp_tx_entry_gpu(tx_entry) && lpp_disable_hmem_dma) {
		if (lpp_stx_hmem_stage(lpp_epp, tx_entry, tx_entry->msg_hdr.data_length)) {
			ret = -FI_EINVAL;
			goto err_no_spool;
		}
		lpp_tx_entry_clear_gpu(tx_entry);
	}

	FI_DBG(&lpp_prov, FI_LOG_EP_DATA, "buf: %p, len: %d, is_gpu: %d\n",
			tx_entry->msg_iov[0].iov_base, tx_entry->msg_hdr.data_length,
			lpp_tx_entry_gpu(tx_entry));

	if (tx_entry->msg_hdr.data_length <= lpp_max_eager_size &&
	    !lpp_tx_entry_gpu(tx_entry)) {
		tx_entry->msg_hdr.type = KLPP_MSG_EAGER_SEND;
		if (tx_entry->msg_hdr.data_length > 0) {
			tx_entry->msg_hdr.flags |= KLPP_MSG_DATA;
		}

		/* Attempt to flush unsent, as ordering requires them to go out
		 * first. */
		ret = lpp_stx_flush_msg_unsent(lpp_epp);
		if (ret)
			goto err;
		ret = lpp_stx_tx_common(lpp_epp, tx_entry, lpp_epp->cntr_send);
		if (ret)
			goto err;
	} else {
		if (tx_entry->flags & FI_INJECT) { // don't pass user buffer to kernel
			lpp_stx_copy_inject(tx_entry);
		}

		ret = lpp_tx_entry_reg_mrs(lpp_epp->domain, tx_entry);

		if (ret != 0) {
			FI_WARN(&lpp_prov, FI_LOG_EP_DATA, "failed to register MR\n");
			goto err_no_spool;
		}

		// TODO: Optimize for TRANSMIT_COMPLETE mode?
		tx_entry->msg_hdr.flags |= KLPP_MSG_TOKEN;

		u2k.type = KLPP_U2K_RDZV_SEND;
		u2k.rdzv_send.token = lpp_tx_entry_to_token(lpp_stxp, tx_entry);
		memcpy(&u2k.rdzv_send.iov, tx_entry->msg_iov,
		    sizeof(*tx_entry->msg_iov) * tx_entry->iov_count);
		u2k.rdzv_send.iov_cnt = tx_entry->iov_count;
		u2k.rdzv_send.send_addr = lpp_epp->addr.fi_addr;
		u2k.rdzv_send.recv_addr = tx_entry->dst_addr.fi_addr;

		lpp_umc_u2k_enqueue(lpp_epp, &u2k);

		tx_entry->lpp_flags |= LPP_TX_ENTRY_RDZV_PEND;
		tx_entry->msg_hdr.type = KLPP_MSG_RDZV_SEND;
		tx_entry->msg_hdr.flags |= KLPP_MSG_DATA_RDZV;

		FI_DBG(&lpp_prov, FI_LOG_EP_DATA, "len: %d, flags: 0x%lx "
				"lpp_flags: 0x%lx token: 0x%llx\n",
			tx_entry->msg_hdr.data_length, tx_entry->flags,
			tx_entry->lpp_flags, u2k.rdzv_send.token);
		// Attempt to flush unsent, as ordering requires them to go out first.
		ret = lpp_stx_flush_msg_unsent(lpp_epp);
		if (ret)
			goto err;
		ret = lpp_stx_tx_common(lpp_epp, tx_entry, lpp_epp->cntr_send);
		if (ret)
			goto err;
	}


	return 0;
err:
	if (ret == -FI_EAGAIN) {
		/* If this request isn't issuing a completion, or associalted with a
		 * counter, then the user isn't required to perform network progress.
		 * Placing the tx_entry on the unsent list can lead to deadlocks. */
		if (!(tx_entry->flags & FI_COMPLETION) && !lpp_epp->cntr_send) {
			lpp_tx_entry_put(lpp_epp, tx_entry);
			return -FI_EAGAIN;
		}
		if (tx_entry->flags & FI_INJECT)
			lpp_stx_copy_inject(tx_entry);

		dlist_insert_tail(&tx_entry->dle, &lpp_stxp->tx_entry_unsent);
		tx_entry->lpp_flags |= LPP_TX_ENTRY_ONLIST;
		return 0;
	}

err_no_spool:
	lpp_tx_entry_put(lpp_epp, tx_entry);
	return ret;
}

int lpp_stx_flush_unsent_all(struct lpp_ep *lpp_epp)
{
	int ret;

	ret = lpp_stx_flush_msg_unsent(lpp_epp);
	if (ret != 0)
		return ret;

	ret = lpp_stx_flush_atomic_unsent(lpp_epp);
	return ret;
}

// If threading == FI_THREAD_SAFE, must hold ep "op lock".
void lpp_stx_rdzv_done(struct lpp_ep *lpp_epp, struct klpp_umc_k2u *k2u)
{
	struct lpp_tx_entry *tx_entry;

	tx_entry = lpp_token_to_tx_entry(lpp_epp->stx,
	    k2u->rdzv_send_done.token);
	if (tx_entry == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
		       "got send done with invalid token %llx, status %d\n",
		       k2u->rdzv_send_done.token, k2u->rdzv_send_done.status);
		return;
	}
	// TODO: assert(tx_entry->lpp_flags & LPP_TX_ENTRY_RDZV_PEND); ?
	tx_entry->lpp_flags &= ~LPP_TX_ENTRY_RDZV_PEND;
	lpp_completion_transmit(lpp_epp, lpp_epp->cntr_send, tx_entry->flags,
				tx_entry->msg_hdr.rcq_data, tx_entry->context,
				k2u->rdzv_send_done.status);
	lpp_tx_entry_put(lpp_epp, tx_entry);
}

// If threading == FI_THREAD_SAFE, must hold ep "op lock".
void lpp_stx_path_down(struct lpp_ep *lpp_epp, int remote_node_id, int remote_umc_id)
{
	struct lpp_stx *lpp_stxp = lpp_epp->stx;
	struct lpp_fi_addr *addr;
	struct lpp_tx_entry *tx_entry;
	struct dlist_entry *tmp_entry;
	struct dlist_entry *lists[] = {
	    &lpp_stxp->tx_entry_dqp,
	    &lpp_stxp->tx_entry_srq,
	    &lpp_stxp->tx_entry_unsent,
	    &lpp_stxp->tx_entry_atomic_unsent,
	};

	/* TODO: maybe make a way to quickly find the entries associated with a
	 * particular remote guy. */
	for (unsigned long i = 0; i < ARRAY_SIZE(lists); i++) {
		dlist_foreach_container_safe(lists[i],
		    struct lpp_tx_entry, tx_entry, dle, tmp_entry) {
			addr = &tx_entry->dst_addr.fi_addr;
			if (addr->node_id == remote_node_id &&
			    addr->umc_id == remote_umc_id) {
				lpp_completion_transmit(lpp_epp,
				    lpp_epp->cntr_send, tx_entry->flags,
				    tx_entry->msg_hdr.rcq_data, tx_entry->context,
				    ESHUTDOWN);
				lpp_tx_entry_put(lpp_epp, tx_entry);
			}
		}
	}
}

// If threading == FI_THREAD_SAFE, must hold ep "op lock".
void lpp_stx_update_rx_time(struct lpp_ep *lpp_epp, struct lpp_fi_addr remote_addr)
{
	struct lpp_umc *umc = lpp_epp->umc;
	struct lpp_umc_remote *rumc;

	rumc = lpp_rumc_get(lpp_epp, remote_addr.node_id, remote_addr.umc_id);
	if (rumc != NULL && rumc->srq_refcount > 0 &&
	    rumc->last_rx_time != umc->prog_time) {
		rumc->last_rx_time = umc->prog_time;
		rumc->keep_alive_cnt = 0;
		// Move to tail of the list, since it now has theyoungest time of any rumc.
		dlist_remove(&rumc->dle);
		dlist_insert_tail(&rumc->dle, &umc->keep_alive_rumcs);
	}
}

// If threading == FI_THREAD_SAFE, must hold ep "op lock".
ssize_t lpp_atomic_common(struct lpp_ep *lpp_epp, struct lpp_tx_entry *tx_entry)
{
	struct lpp_stx *lpp_stxp = lpp_epp->stx;
	struct lpp_cq *lpp_cqp = lpp_epp->cq_transmit;
	ssize_t ret;
	enum fi_hmem_iface hmem_iface;

	for (size_t i = 0; i < tx_entry->iov_count; i++) {
		ret = lpp_hmem_ptr(lpp_epp->domain, tx_entry->msg_iov[i].iov_base,
				     &hmem_iface);
		if (ret) {
			FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
					"Unable to determine src hmem type");
			goto err_no_spool;
		}

		if (hmem_iface) {
			FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
				"GPU buffers are not allowed for atomic ops\n");
			ret = -FI_EINVAL;
			goto err_no_spool;
		}
	}

	ret = lpp_hmem_ptr(lpp_epp->domain, tx_entry->atomic_result.iov_base,
			     &hmem_iface);
	if (ret) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
				"Unable to determine atomic_result hmem type");
		goto err_no_spool;
	}

	if (tx_entry->atomic_hdr.mode != LPP_ATOMIC && hmem_iface) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
			"GPU buffers are not allowed for atomic ops\n");
		ret = -FI_EINVAL;
		goto err_no_spool;
	}

	tx_entry->msg_hdr.data_length =
		ofi_total_iov_len(tx_entry->msg_iov, tx_entry->iov_count);

	ret = lpp_cq_reserve_slot(lpp_cqp, tx_entry->flags);
	if (ret > 0) {
		tx_entry->lpp_flags |= LPP_TX_ENTRY_CQ_RSRV;
	} else if (ret < 0) {
		FI_INFO(&lpp_prov, FI_LOG_EP_DATA,
		       "cannot start TX operation as it may overrun CQ\n");
		ret = -FI_EAGAIN;
		goto err_no_spool;
	}

	if (tx_entry->flags & FI_INJECT) {
		if (tx_entry->msg_hdr.data_length > lpp_stxp->attr.inject_size) {
			FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
				"length %u exceeds max inject size %ld\n",
				tx_entry->msg_hdr.data_length, lpp_stxp->attr.inject_size);
			ret = -FI_EINVAL;
			goto err_no_spool;
		}
	}

	if (tx_entry->msg_hdr.data_length <= lpp_max_eager_size) {
		tx_entry->msg_hdr.type = KLPP_MSG_EAGER_ATOMIC;
		tx_entry->msg_hdr.flags |= KLPP_MSG_DATA;

		// Attempt to flush unsent, as ordering requires them to go out first.
		ret = lpp_stx_flush_atomic_unsent(lpp_epp);
		if (ret)
			goto err;
		ret = lpp_stx_tx_common(lpp_epp, tx_entry,
				lpp_epp->cntr_write);
		if (ret)
			goto err;
	} else {
		ret = -FI_EMSGSIZE;
		goto err_no_spool;
	}

	return 0;
err:
	if (ret == -FI_EAGAIN) {
		dlist_insert_tail(&tx_entry->dle,
				  &lpp_stxp->tx_entry_atomic_unsent);
		tx_entry->lpp_flags |= LPP_TX_ENTRY_ONLIST;
		if (tx_entry->flags & FI_INJECT)
			lpp_stx_copy_inject(tx_entry);

		return 0;
	}

err_no_spool:
	lpp_tx_entry_put(lpp_epp, tx_entry);
	return ret;
}
