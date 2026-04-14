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

#define DIV_ROUND_UP(n, d)	(((n) + (d) - 1) / (d))
#define wmb()			asm volatile("sfence" ::: "memory")
#define unlikely(x)		__builtin_expect((x), 0)
#define likely(x)		__builtin_expect((x), 1)

static void lpp_umc_rx_progress(struct lpp_ep *lpp_epp);
static void lpp_umc_norx_progress(struct lpp_ep *lpp_epp);
static void lpp_umc_dqp_read(struct lpp_ep *lpp_epp, struct lpp_umc_dqp *dqp);

struct klpp_mq *lpp_remote_srq(struct lpp_ep *lpp_epp, int remote_host_index, __u64 remote_srq_offset)
{
	struct lpp_domain *domain = lpp_epp->domain;
	struct klpp_rmr_region_info *rmr_region_info = &domain->devinfo.rmr_region_info;
	__u64 remote_node_window;

	if (remote_srq_offset == (-1UL) || !lpp_write_only)
		return NULL;

	remote_node_window = remote_host_index * rmr_region_info->per_node_bytes;

	return (struct klpp_mq *)((char *)domain->rmr_srq + remote_node_window + remote_srq_offset);
}

int lpp_umc_acquire(struct lpp_ep *lpp_epp)
{
	struct lpp_domain *domain = lpp_epp->domain;
	struct klpp_mc_params *mc_params = &lpp_epp->domain->devinfo.mc_params;
	struct lpp_umc *umc;
	size_t rx_size;
	size_t tx_size;
	size_t k2u_size;
	size_t u2k_size;
	__u64 remote_srq_offset;
	int ret;

	umc = calloc(1, sizeof(*umc));
	if (umc == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "failed to alloc UMC\n");
		return -FI_ENOMEM;
	}
	umc->srqs = calloc(mc_params->n_srq, sizeof(*umc->srqs));
	if (umc->srqs == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "failed to alloc UMC SRQs\n");
		ret = -FI_ENOMEM;
		goto err_free_umc;
	}
	umc->dqps = calloc(mc_params->n_dqp, sizeof(*umc->dqps));
	if (umc->dqps == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "failed to alloc UMC DQPs\n");
		ret = -FI_ENOMEM;
		goto err_free_srqs;
	}

	rx_size = klpp_umc_rx_bytes(mc_params);
	umc->rx_base = klpp_mmap(domain, rx_size, PROT_READ | PROT_WRITE);
	if (umc->rx_base == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "failed to map UMC RX queues\n");
		ret = -FI_ENOMEM;
		goto err_free_dqps;
	}

	tx_size = klpp_umc_tx_bytes(mc_params);
	umc->tx_base = klpp_mmap(domain, tx_size, PROT_READ | PROT_WRITE);
	if (umc->tx_base == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "failed to map UMC TX queues\n");
		ret = -FI_ENOMEM;
		goto err_unmap_rx;
	}

	k2u_size = klpp_k2u_bytes(mc_params);
	umc->k2u_base = klpp_mmap(domain, k2u_size, PROT_READ | PROT_WRITE);
	if (umc->k2u_base == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "failed to map UMC K2U queue\n");
		ret = -FI_ENOMEM;
		goto err_unmap_tx;
	}
	umc->k2u = (struct klpp_ringbuf_k2u *)umc->k2u_base;

	u2k_size = klpp_u2k_bytes(mc_params);
	umc->u2k_base = klpp_mmap(domain, u2k_size, PROT_READ | PROT_WRITE);
	if (umc->u2k_base == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "failed to map UMC U2K queue\n");
		ret = -FI_ENOMEM;
		goto err_unmap_k2u;
	}
	umc->u2k = (struct klpp_ringbuf_u2k *)umc->u2k_base;

	ret = klpp_umc_acquire(domain, &umc->id, &lpp_epp->umc_src_id,
			       &umc->generation, umc->rx_base, umc->tx_base,
			       umc->k2u_base, umc->u2k_base,
			       lpp_epp->addr.fi_addr, &lpp_epp->port);
	if (ret != 0) {
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "failed to acquire UMC from kernel\n");
		goto err_unmap_u2k;
	}

	for (int i = 0; i < mc_params->n_srq; i++) {
		umc->srqs[i].mq = klpp_srq_offset(umc->rx_base, mc_params, i);
		remote_srq_offset = umc->srqs[i].mq->spad.remote_srq_offset;
		if (!lpp_write_only)
			umc->srqs[i].remote_mq = NULL;
		else if (remote_srq_offset == (-1ULL))
			umc->srqs[i].remote_mq = umc->srqs[lpp_epp->domain->devinfo.host_index].mq;
		else
			umc->srqs[i].remote_mq = lpp_remote_srq(lpp_epp, i, remote_srq_offset);
	}
	for (int i = 0; i < mc_params->n_dqp; i++) {
		umc->dqps[i].rx = klpp_dqp_offset(umc->rx_base, mc_params, i);
		umc->dqps[i].tx = klpp_dqp_offset(umc->tx_base, mc_params, i);
	}

	/* TODO: If atomics are enabled, on the STX, need to use the regular
	 * rx_progress. The other way to solve that problem would be to share
	 * UMCs for shared fi contexts, such that there isn't an "RMA specific"
	 * context like MPICH creates. */
	if (lpp_epp->rx)
		umc->progfn = lpp_umc_rx_progress;
	else
		umc->progfn = lpp_umc_norx_progress;

	dlist_init(&umc->keep_alive_rumcs);
	dlist_init(&umc->cmpl_spool_dqp);
	ofi_mutex_init(&umc->lock);

	lpp_epp->umc = umc;

	return 0;

err_unmap_u2k:
	klpp_munmap(umc->u2k_base, u2k_size);
err_unmap_k2u:
	klpp_munmap(umc->k2u_base, k2u_size);
err_unmap_tx:
	klpp_munmap(umc->tx_base, tx_size);
err_unmap_rx:
	klpp_munmap(umc->rx_base, rx_size);
err_free_dqps:
	free(umc->dqps);
err_free_srqs:
	free(umc->srqs);
err_free_umc:
	free(umc);
	return ret;
}

static void lpp_umc_free_dqp_spool(struct lpp_ep *lpp_epp)
{
	struct lpp_spooled_msg_dqp *dmsg;
	struct dlist_entry *tmp_entry;

	dlist_foreach_container_safe(&lpp_epp->umc->cmpl_spool_dqp,
				     struct lpp_spooled_msg_dqp, dmsg, dle,
				     tmp_entry) {
		if (dmsg->callback != NULL)
			dmsg->callback(lpp_epp, dmsg + 1, -FI_ESHUTDOWN);

		dlist_remove(&dmsg->dle);
		free(dmsg);
	}
}

static void lpp_umc_free_srq_spool(struct lpp_ep *lpp_epp)
{
	struct lpp_cmpl_spool_srq *spool = &lpp_epp->umc->cmpl_spool_srq;
	struct lpp_spooled_msg_srq *smsg;
	struct klpp_ioc_kmc_send *ioc;

	for (ioc = spool->head; ioc != NULL; ioc = spool->head) {
		smsg = container_of(ioc, struct lpp_spooled_msg_srq, ioc);
		spool->head = (struct klpp_ioc_kmc_send *)ioc->next_ptr;
		if (smsg->callback != NULL)
			smsg->callback(lpp_epp, smsg + 1, -FI_ESHUTDOWN);

		free(smsg);
	}
}

static void lpp_umc_free_rumcs(struct lpp_ep *lpp_epp)
{
	struct index_map *idm = &lpp_epp->umc->rumc_idm;
	struct lpp_umc_remote *rumc_arr;
	int i;

	for (i = 0; i < OFI_IDX_MAX_CHUNKS; i++) {
		rumc_arr = ofi_idm_lookup(idm, i);
		if (rumc_arr)
			free(rumc_arr);
	}
	ofi_idm_reset(idm, NULL);
}

void lpp_umc_release(struct lpp_ep *lpp_epp, struct lpp_umc *umc)
{
	struct klpp_mc_params *mc_params = &lpp_epp->domain->devinfo.mc_params;
	size_t rx_size;
	size_t tx_size;
	size_t k2u_size;
	size_t u2k_size;

	lpp_umc_free_dqp_spool(lpp_epp);
	lpp_umc_free_srq_spool(lpp_epp);

	rx_size = klpp_umc_rx_bytes(mc_params);
	tx_size = klpp_umc_tx_bytes(mc_params);
	k2u_size = klpp_k2u_bytes(mc_params);
	u2k_size = klpp_u2k_bytes(mc_params);
	klpp_munmap(umc->rx_base, rx_size);
	klpp_munmap(umc->tx_base, tx_size);
	klpp_munmap(umc->k2u_base, k2u_size);
	klpp_munmap(umc->u2k_base, u2k_size);
	klpp_umc_release(lpp_epp->domain, umc->id);
	free(umc->srqs);
	lpp_umc_free_rumcs(lpp_epp);
	free(umc->dqps);
	free(umc);
}

static struct lpp_umc_remote *
__lpp_rumc_get_insert(struct lpp_ep *lpp_epp, int node_id, int umc_id, bool insert)
{
	struct klpp_mc_params *mc_params = &lpp_epp->domain->devinfo.mc_params;
	struct lpp_umc *umc = lpp_epp->umc;
	struct lpp_umc_remote *rumc_arr;

	rumc_arr = ofi_idm_lookup(&umc->rumc_idm, node_id);
	if (OFI_UNLIKELY(rumc_arr == NULL && insert)) {
		rumc_arr = malloc(mc_params->n_umc * sizeof(*rumc_arr));
		if (rumc_arr == NULL) {
			FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
			       "failed to alloc remote umcs array\n");
			return NULL;
		}
		for (int i = 0; i < mc_params->n_umc; i++) {
			struct lpp_umc_remote *rumc = &rumc_arr[i];
			dlist_init(&rumc->dle);
			rumc->node_id = node_id;
			rumc->umc_id = i;
			rumc->dqp_idx = LPP_DQP_UNSET;
			rumc->keep_alive_cnt = 0;
			rumc->srq_refcount = 0;
			rumc->last_rx_time = 0;
			rumc->flags = 0;
		}
		ofi_idm_set(&umc->rumc_idm, node_id, rumc_arr);
	}
	return rumc_arr ? &rumc_arr[umc_id] : NULL;
}

struct lpp_umc_remote *lpp_rumc_get(struct lpp_ep *lpp_epp, int node_id, int umc_id)
{
	return __lpp_rumc_get_insert(lpp_epp, node_id, umc_id, false);
}

struct lpp_umc_remote *lpp_rumc_get_insert(struct lpp_ep *lpp_epp, int node_id,
					   int umc_id)
{
	return __lpp_rumc_get_insert(lpp_epp, node_id, umc_id, true);
}

/* TODO: if supporting multiple FI_THREAD_SAFE, this needs to be protected with
 * a lock. (basically, progress path needs a lock, and then this one part of
 * the tx path needs a lock and/or needs to be lock-free. The rest should be
 * fine without additional locks.) */
static int dqp_reserve_space(struct lpp_umc_dqp *dqp,
			     struct klpp_mc_params *mc_params,
			     size_t bytes_needed, size_t *nop_bytes,
			     size_t *tx_prod_idx)
{
	uint64_t *p = &dqp->tx_prod_idx;
	__u64 *c = &dqp->rx->spad.cons_idx;
	size_t cells_used;
	size_t cells_left;
	size_t cells_until_wrap;
	size_t bytes_left;
	size_t bytes_until_wrap;

	cells_used = *p - *c;
	cells_left = mc_params->dqp_cell_cnt - cells_used;
	cells_until_wrap = mc_params->dqp_cell_cnt -
	    *p % mc_params->dqp_cell_cnt;
	bytes_until_wrap = cells_until_wrap * KLPP_MQ_CELL_SIZE;
	bytes_left = cells_left * KLPP_MQ_CELL_SIZE;

	if (bytes_needed > bytes_left ||
	    (bytes_needed > bytes_until_wrap &&
	    bytes_needed > bytes_left - bytes_until_wrap)) {
		return -FI_EAGAIN;
	} else {
		*nop_bytes = bytes_needed > bytes_until_wrap ?
		    bytes_until_wrap : 0;
		*tx_prod_idx = *p % mc_params->dqp_cell_cnt;
		*p += DIV_ROUND_UP(bytes_needed + *nop_bytes, KLPP_MQ_CELL_SIZE);
		return 0;
	}
}

/* This function exists merely to make it easier to see when an assignment
 * involves remote memory. We don't actually need to do anything special to
 * write to the NT BAR. */
static inline void io_wr64(__u64 *src, __u64 *dst)
{
	*dst = *src;
}

int lpp_umc_dqp_tx(struct lpp_ep *lpp_epp, struct lpp_umc_remote *rumc,
		   struct klpp_msg_hdr *hdr, const struct iovec *iov,
		   size_t iov_count)
{
	struct klpp_mc_params *mc_params = &lpp_epp->domain->devinfo.mc_params;
	struct lpp_umc *umc = lpp_epp->umc;
	struct lpp_umc_dqp *dqp = &umc->dqps[rumc->dqp_idx];
	struct klpp_mq *tx_dqp = dqp->tx;
	struct klpp_msg_hdr *remote_hdr;
	size_t nop_bytes;
	size_t bytes_needed;
	size_t p;
	uint8_t *remote_data_buf;
	int ret;

	hdr->src_id = lpp_epp->umc_src_id;
	bytes_needed = klpp_msg_total_bytes(hdr);

	ret = dqp_reserve_space(dqp, mc_params, bytes_needed, &nop_bytes, &p);
	if (ret != 0)
		return ret;

	remote_hdr = (struct klpp_msg_hdr*)&tx_dqp->cells[p];

	if (nop_bytes > 0) {
		struct klpp_msg_hdr nop_msg_hdr = {
			.type = KLPP_MSG_NOP,
			.src_id = hdr->src_id,
			.flags = KLPP_MSG_DATA,
			.data_length = klpp_msg_nop_data_length(nop_bytes),
		};
		io_wr64(&nop_msg_hdr.valid, &remote_hdr->valid);
		remote_hdr = (struct klpp_msg_hdr*)&tx_dqp->cells[0];
	}

	if (hdr->flags & KLPP_MSG_DATA) {
		remote_data_buf = (uint8_t *)klpp_msg_data_offset(
			remote_hdr, hdr->data_length);
		for (size_t i = 0; i < iov_count; i++) {
			lpp_io_memcpy(remote_data_buf, iov[i].iov_base,
				      iov[i].iov_len);
			remote_data_buf += iov[i].iov_len;
		}
	}
	if (hdr->flags & KLPP_MSG_TOKEN)
		io_wr64(&hdr->token, &remote_hdr->token);

	if (hdr->flags & KLPP_MSG_TAG) {
		if ((rumc->flags & LPP_UMCR_PERST_TAG) &&
		    hdr->tag == rumc->persistent_tag) {
			hdr->flags |= KLPP_MSG_TAG_PERST;
		} else {
			rumc->flags |= LPP_UMCR_PERST_TAG;
			rumc->persistent_tag = hdr->tag;
			io_wr64(&hdr->tag, &remote_hdr->tag);
		}
	}
	if (hdr->flags & KLPP_MSG_RCQ_DATA)
			io_wr64(&hdr->rcq_data, &remote_hdr->rcq_data);


	/* TODO: If the buffer is WC, this first wmb() needs to be a hardware
	 * fence. If not, a compiler barrier is sufficient. */
	compiler_barrier();
	io_wr64(&hdr->valid, &remote_hdr->valid);
	wmb();

	return 0;
}

int lpp_umc_tx_simple(struct lpp_ep *lpp_epp, struct lpp_fi_addr dst_addr,
		      struct klpp_msg_hdr *hdr, const struct iovec *iov,
		      size_t iov_count)
{
	struct lpp_umc_remote *rumc;

	rumc = lpp_rumc_get(lpp_epp, dst_addr.node_id, dst_addr.umc_id);
	if (rumc != NULL && rumc->dqp_idx != LPP_DQP_UNSET)
		return lpp_umc_dqp_tx(lpp_epp, rumc, hdr, iov, iov_count);
	else
		return klpp_kmc_send_one(lpp_epp, dst_addr, hdr, iov, iov_count);
}

static void lpp_umc_kservice(struct lpp_ep *lpp_epp)
{
	struct lpp_umc *umc = lpp_epp->umc;

	if (umc->flags & LPP_UMC_KSERVICE) {
		umc->flags &= ~LPP_UMC_KSERVICE;
		klpp_progress(lpp_epp->domain, lpp_epp->umc->id);
	}
}

void lpp_umc_u2k_enqueue(struct lpp_ep *lpp_epp, struct klpp_umc_u2k *u2k)
{
	struct lpp_umc *umc = lpp_epp->umc;
	struct klpp_mc_params *mc_params = &lpp_epp->domain->devinfo.mc_params;
	int ret;

	do {
		ret = klpp_ringbuf_enqueue_u2k(umc->u2k, mc_params->u2k_entry_cnt, u2k);
		umc->flags |= LPP_UMC_KSERVICE;
		/* Servicing the kernel will clear the queue, opening up a slot
		 * for us to try again. */
		if (ret != 0)
			lpp_umc_kservice(lpp_epp);

	} while (ret != 0);
}

static int lpp_umc_send_pong(struct lpp_ep *lpp_epp, struct lpp_fi_addr dst_addr)
{
	struct klpp_msg_hdr hdr = { 0 };

	hdr.type = KLPP_MSG_PONG;
	return lpp_umc_tx_simple(lpp_epp, dst_addr, &hdr, NULL, 0);
}

static void lpp_cmpl_srq_add(struct lpp_ep *lpp_epp, struct klpp_ioc_kmc_send *ioc)
{
	struct lpp_umc *umc = lpp_epp->umc;
	struct lpp_cmpl_spool_srq *spool = &umc->cmpl_spool_srq;

	/* TODO: should there be a max imposed on the number of spooled
	 * completions? If the max is reached, we go to the kernel immediately
	 * to attempt to flush the spool. */

	ioc->errval = -FI_ENODATA;
	ioc->next_ptr = 0;

	if (spool->tail == NULL) {
		spool->head = ioc;
		spool->tail = ioc;
	} else {
		spool->tail->next_ptr = (__u64)ioc;
		spool->tail = ioc;
	}
	spool->size++;
}

static void lpp_cmpl_srq_flush(struct lpp_ep *lpp_epp)
{
	struct lpp_spooled_msg_srq *smsg;
	struct lpp_cmpl_spool_srq *spool = &lpp_epp->umc->cmpl_spool_srq;
	struct klpp_ioc_kmc_send *ioc;
	int ret;

	if (likely(spool->head == NULL))
		return;

	ret = klpp_kmc_send_list(lpp_epp, spool->head);
	if (ret)
		// TODO: this error needs to be propagated
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "failed trigger list send\n");

	for (ioc = spool->head; ioc != NULL; ioc = spool->head) {
		if (ioc->errval == -FI_ENODATA) {
			FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
				"cmpl cmd not executed\n");
			break;
		} else if (ioc->errval == -FI_EAGAIN) {
			ioc->errval = -FI_ENODATA;
			break;
		} else {
			smsg = container_of(ioc, struct lpp_spooled_msg_srq,
					    ioc);
			spool->size--;
			if (ioc->errval != 0) {
				FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
					"failed to srq send completion %lld\n",
					ioc->errval);
			}
			spool->head = (struct klpp_ioc_kmc_send *)ioc->next_ptr;
			if (spool->head == NULL)
				spool->tail = NULL;

			if (smsg->callback != NULL)
				smsg->callback(lpp_epp, smsg + 1, 0);

			free(smsg);
		}
	}
}

static void lpp_cmpl_dqp_flush(struct lpp_ep *lpp_epp)
{
	struct lpp_umc *umc = lpp_epp->umc;
	struct lpp_spooled_msg_dqp *dmsg;
	struct dlist_entry *tmp_entry;
	struct lpp_umc_remote *rumc;
	int ret;

	dlist_foreach_container_safe(&umc->cmpl_spool_dqp,
				     struct lpp_spooled_msg_dqp, dmsg, dle,
				     tmp_entry)
	{
		rumc = lpp_rumc_get(lpp_epp, dmsg->addr.node_id,
				    dmsg->addr.umc_id);
		if (unlikely(rumc->dqp_idx == LPP_DQP_UNSET)) {
			/* TODO: If we support "soft DQP detach", in which the
			 * remote peer is still there but we stop using the
			 * DQP, this needs to be reevaluated. */
			ret = -FI_ESHUTDOWN;
		} else {
			ret = lpp_umc_dqp_tx(lpp_epp, rumc, &dmsg->msg_hdr,
					     &dmsg->iov, dmsg->iov_count);
		}
		if (ret != -FI_EAGAIN) {
			if (ret != 0)
				FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
					"failed to dqp send completion %d\n", ret);

			if (dmsg->callback != NULL)
				dmsg->callback(lpp_epp, dmsg + 1, 0);

			dlist_remove(&dmsg->dle);
			free(dmsg);
		}
	}
}

static void lpp_cmpl_flush(struct lpp_ep *lpp_epp)
{
	lpp_cmpl_dqp_flush(lpp_epp);
	lpp_cmpl_srq_flush(lpp_epp);
}

/* This function has two goals:
 * - Spool up completions if we get an EAGAIN when we try to transmit them, so
 *   that we can try again later. Spooling completions is necessary to avoid a
 *   deadlock where both sides spin trying to transmit a completion without
 *   spending any cycles to clear their own (full) RX queue.
 * - Spool up SRQ completions and transmit them in batch. This reduces trips to
 *   the kernel.
 *
 * If the return is -FI_EAGAIN, an allocation will be made to spool the
 * completion. An extra cb_data_len bytes are alloced for the caller's use,
 * with the address returned in cb_data. */
int lpp_umc_tx_cmpl(struct lpp_ep *lpp_epp, struct lpp_fi_addr addr,
		    struct klpp_msg_hdr *hdr, void *data, size_t data_len,
		    lpp_spool_cb callback, size_t cb_data_len, void **cb_data)
{
	struct lpp_umc *umc = lpp_epp->umc;
	struct lpp_umc_remote *rumc;
	struct lpp_spooled_msg_dqp *dmsg;
	struct lpp_spooled_msg_srq *smsg;
	struct klpp_ioc_kmc_send *ioc;
	struct iovec iov;
	size_t iov_count;
	int ret;

	if (data != NULL) {
		iov.iov_base = data;
		iov.iov_len = data_len;
		iov_count = 1;
	} else {
		iov_count = 0;
	}

	rumc = lpp_rumc_get(lpp_epp, addr.node_id, addr.umc_id);
	if (rumc != NULL && rumc->dqp_idx != LPP_DQP_UNSET) {
		ret = lpp_umc_dqp_tx(lpp_epp, rumc, hdr, &iov, iov_count);
		if (ret == -FI_EAGAIN) {
			// TODO: util_buf_alloc?
			dmsg = malloc(sizeof(*dmsg) + cb_data_len);
			if (cb_data != NULL)
				*cb_data = dmsg + 1;

			dmsg->addr = addr;
			dmsg->callback = callback;
			dmsg->iov = iov;
			dmsg->iov_count = iov_count;
			memcpy(&dmsg->msg_hdr, hdr, sizeof(*hdr));
			dlist_insert_tail(&dmsg->dle, &umc->cmpl_spool_dqp);
		} else if (ret) {
			FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
				"failed to send deliv comp: %d\n", ret);
		}
	} else {
		// TODO util_buf_alloc?
		smsg = malloc(sizeof(*smsg) + cb_data_len);
		if (cb_data != NULL)
			*cb_data = smsg + 1;

		smsg->callback = callback;
		smsg->iov = iov;
		ioc = &smsg->ioc;
		ioc->dst_addr = addr;
		ioc->kmc_flags = lpp_disable_dqp ? KLPPIOCF_KMC_NODQP : 0;
		ioc->iov_ptr = (__u64)&smsg->iov;
		ioc->iov_cnt = iov_count;
		hdr->src_ep = lpp_epp->addr;
		ioc->msg_hdr = *hdr;
		lpp_cmpl_srq_add(lpp_epp, ioc);
		ret = -FI_EAGAIN;
	}

	return ret;
}

static void process_msg(struct lpp_ep *lpp_epp, struct klpp_msg_hdr *hdr)
{
	FI_DBG(&lpp_prov, FI_LOG_EP_DATA,"hdr->type: %d, hdr->TAG:%d (0x%llx) hdr->RCQ_DATA:%d (0x%llx)\n",
		hdr->type,
		!!(hdr->flags & KLPP_MSG_TAG), hdr->tag,
		!!(hdr->flags & KLPP_MSG_RCQ_DATA), hdr->rcq_data);

	switch(hdr->type) {
	case KLPP_MSG_EAGER_SEND:
	case KLPP_MSG_RDZV_SEND:
		lpp_rx_process_msg(lpp_epp, hdr);
		break;
	case KLPP_MSG_EAGER_ATOMIC:
	case KLPP_MSG_RDZV_ATOMIC:
		lpp_atomic_process_msg(lpp_epp, hdr);
		break;
	case KLPP_MSG_ATOMIC_DELIV_COMP:
		lpp_tx_process_atomic_deliv_comp(lpp_epp, hdr);
		break;
	case KLPP_MSG_SEND_DELIV_COMP:
		lpp_tx_process_deliv_comp(lpp_epp, hdr);
		break;
	case KLPP_MSG_DISCARD_SEND:
		lpp_tx_process_discard(lpp_epp, hdr);
		break;
	case KLPP_MSG_INVALID_DST:
		lpp_tx_process_invalid_dst(lpp_epp, hdr);
		// This is an abnormal response; don't update rx_time.
		return;
		break;
	case KLPP_MSG_PING:
		lpp_umc_send_pong(lpp_epp, hdr->src_ep.fi_addr);
		break;
	case KLPP_MSG_PONG:
	case KLPP_MSG_NOP:
		// Nothing to do.
		break;
	default:
		assert(0);
		break;
	}

	lpp_stx_update_rx_time(lpp_epp, hdr->src_ep.fi_addr);
}

static inline struct klpp_msg_hdr *get_hdr(struct klpp_mq *mq, uint64_t cell_idx)
{
	return (struct klpp_msg_hdr *)&mq->cells[cell_idx];
}

static inline void lpp_srq_check_kservice(struct lpp_umc *umc,
					  struct klpp_mq *mq)
{
	if (unlikely(mq->spad.kservice)) {
		/* TODO: build a list of node_ids that have their kservice
		 * flags set. When we go to the kernel, we will scan only the
		 * associated ntb_transport queues. When that's done, we also
		 * want to skip clearing the kservice flag here. We'll clear it
		 * in the kernel when we've actually serviced that node
		 * (reduces spurious kernel transitions). */
		umc->flags |= LPP_UMC_KSERVICE;
		mq->spad.kservice = 0;
	}
}

static void lpp_umc_srq_read(struct lpp_ep *lpp_epp, struct lpp_umc_srq *srq)
{
	struct lpp_umc *umc = lpp_epp->umc;
	struct klpp_mc_params *mc_params = &lpp_epp->domain->devinfo.mc_params;
	struct klpp_mq *mq = srq->mq;
	struct klpp_mq *remote_mq = srq->remote_mq;
	__u64 *cons_idx = &mq->spad.cons_idx;
	struct klpp_msg_hdr *hdr;
	uint64_t cell_idx;
	size_t cmd_bytes;
	size_t cmd_cells;

	lpp_srq_check_kservice(umc, mq);

	/* This indicates the queue may contain stale data from a previous
	 * generation of the UMC. */
	if (unlikely(mq->spad.rx_gen != mq->spad.tx_gen))
		return;

	cell_idx = *cons_idx % mc_params->srq_cell_cnt;
	for (hdr = get_hdr(mq, cell_idx); hdr->type != KLPP_MSG_INVALID;
	    hdr = get_hdr(mq, cell_idx)) {
		process_msg(lpp_epp, hdr);

		cmd_bytes = klpp_msg_total_bytes(hdr);
		cmd_cells = DIV_ROUND_UP(cmd_bytes, KLPP_MQ_CELL_SIZE);
		for (uint64_t i = cell_idx; i < cell_idx + cmd_cells; i++) {
			hdr = get_hdr(mq, i);
			hdr->type = KLPP_MSG_INVALID;
		}
		/* Ensure we're done touching the cells before making them
		 * appear free to the remote side (which may read our cons_idx
		 * asynchronously). */
		compiler_barrier();

		*cons_idx += cmd_cells;
		cell_idx = *cons_idx % mc_params->srq_cell_cnt;
	}

	if (lpp_write_only) {
		if (remote_mq)
			io_wr64(cons_idx, &remote_mq->spad.remote_cons_idx);
		else
			FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
				"no update of remote_cons_idx (umc %d) (remote_mq NULL)\n",
				umc->id);
	}
}

static void lpp_umc_scan_srq_kservice(struct lpp_ep *lpp_epp)
{
	struct lpp_domain *domain = lpp_epp->domain;
	struct lpp_umc *umc = lpp_epp->umc;
	struct klpp_mc_params *mc_params = &domain->devinfo.mc_params;
	struct lpp_umc_srq *srq;
	struct klpp_mq *mq;

	/* TODO: kernel should be able to tell us how many nodes we're actually
	 * talking to (<= mc_params->n_srq). If it's far less than n_srq, we
	 * get an advantage in that we don't have to scan very many queues. */
	for (int i = 0; i < mc_params->n_srq; i++) {
		srq = &umc->srqs[i];
		mq = srq->mq;
		lpp_srq_check_kservice(umc, mq);
	}
}

static void lpp_umc_scan_srq(struct lpp_ep *lpp_epp)
{
	struct lpp_domain *domain = lpp_epp->domain;
	struct lpp_umc *umc = lpp_epp->umc;
	struct klpp_mc_params *mc_params = &domain->devinfo.mc_params;
	struct lpp_umc_srq *srq;

	/* TODO: kernel should be able to tell us how many nodes we're actually
	 * talking to (<= mc_params->n_srq). If it's far less than n_srq, we
	 * get an advantage in that we don't have to scan very many queues. */
	for (int i = 0; i < mc_params->n_srq; i++) {
		srq = &umc->srqs[i];
		lpp_umc_srq_read(lpp_epp, srq);
	}
}

static void lpp_umc_dqp_ready(struct lpp_ep *lpp_epp, struct klpp_umc_k2u *k2u)
{
	struct lpp_umc *umc = lpp_epp->umc;
	struct lpp_umc_dqp *dqp = &umc->dqps[k2u->dqp.local_dqp_id];
	struct klpp_msg_hdr sync_hdr = { 0 };
	struct lpp_umc_remote *rumc;

	rumc = lpp_rumc_get_insert(lpp_epp, k2u->dqp.remote_node_id,
				   k2u->dqp.remote_umc_id);
	rumc->dqp_idx = k2u->dqp.local_dqp_id;
	memcpy(dqp->remote_src_ids, k2u->dqp.ep_addrs, sizeof(dqp->remote_src_ids));
	dqp->flags |= (LPP_DQP_ENABLED | LPP_DQP_NEW);

	sync_hdr.type = KLPP_MSG_DQP_SYNC;
	if (lpp_umc_dqp_tx(lpp_epp, rumc, &sync_hdr, NULL, 0) != 0)
		FI_INFO(&lpp_prov, FI_LOG_EP_CTRL,
			"LPP: failed to send DQP_SYNC\n");
	else
		FI_INFO(&lpp_prov, FI_LOG_EP_CTRL,
			"local umc:dqp %u:%u mapped to remote %u:%u:%u:%u\n", umc->id,
			k2u->dqp.local_dqp_id, k2u->dqp.remote_node_id,
			k2u->dqp.remote_umc_id, dqp->remote_src_ids[0].user_id,
			dqp->remote_src_ids[0].ep_id);
}

void lpp_umc_dqp_teardown(struct lpp_ep *lpp_epp, struct klpp_umc_k2u *k2u)
{
	struct lpp_umc *umc = lpp_epp->umc;
	struct lpp_umc_remote *rumc;
	struct klpp_umc_u2k u2k;
	struct lpp_umc_dqp *dqp;

	rumc = lpp_rumc_get(lpp_epp, k2u->dqp.remote_node_id,
	    k2u->dqp.remote_umc_id);
	dqp = &umc->dqps[k2u->dqp.local_dqp_id];

	/* Ensure all remaining messages have been scooped up before we trash
	 * this DQP. */
	lpp_umc_dqp_read(lpp_epp, dqp);

	rumc->dqp_idx = LPP_DQP_UNSET;
	rumc->flags = 0;
	memset(dqp->remote_src_ids, 0, sizeof(dqp->remote_src_ids));
	dqp->flags &= ~(LPP_DQP_ENABLED | LPP_DQP_NEW);
	dqp->rx_cons_idx = 0;
	dqp->tx_prod_idx = 0;
	// dqp->rx_last_synced_cons_idx = 0;

	FI_INFO(&lpp_prov, FI_LOG_EP_CTRL,
		"%u:%u path down due to DQP teardown notification\n",
		k2u->dqp.remote_node_id, k2u->dqp.remote_umc_id);

	lpp_stx_path_down(lpp_epp, k2u->dqp.remote_node_id,
			  k2u->dqp.remote_umc_id);

	u2k.type = KLPP_U2K_DQP_TEARDOWN_ACK;
	u2k.teardown_ack.local_dqp_id = k2u->dqp.local_dqp_id;
	lpp_umc_u2k_enqueue(lpp_epp, &u2k);
}

static void lpp_umc_read_k2u(struct lpp_ep *lpp_epp)
{
	struct lpp_domain *domain = lpp_epp->domain;
	struct klpp_mc_params *mc_params = &domain->devinfo.mc_params;
	struct lpp_umc *umc = lpp_epp->umc;
	struct klpp_umc_k2u k2u;

	/* TODO: in a shared-UMC world, we need the RDZV send/recv msgs to
	 * identify the endpoint they pertain to. (It's not necessarily the one
	 * that is currently running this function). Or is it enough that we
	 * can identify the tx_entry/rx_entry (how will that work if there is
	 * inter-process sharing of the UMC?) */
	while (klpp_ringbuf_dequeue_k2u(umc->k2u, mc_params->k2u_entry_cnt, &k2u) == 0) {
		switch(k2u.type) {
		case KLPP_K2U_DQP_READY:
			lpp_umc_dqp_ready(lpp_epp, &k2u);
			break;
		case KLPP_K2U_DQP_TEARDOWN:
			lpp_umc_dqp_teardown(lpp_epp, &k2u);
			break;
		case KLPP_K2U_RDZV_SEND_DONE:
			lpp_stx_rdzv_done(lpp_epp, &k2u);
			break;
		case KLPP_K2U_RDZV_RECV_DONE:
			lpp_rx_rdzv_done(lpp_epp, &k2u);
			break;
		case KLPP_K2U_KSERVICE:
			umc->flags |= LPP_UMC_KSERVICE;
			break;
		default:
			break;
		}
	}
}

static void lpp_umc_dqp_sync(struct lpp_ep *lpp_epp, struct lpp_umc_dqp *dqp)
{
	struct lpp_umc *umc = lpp_epp->umc;
	struct lpp_umc_srq *srq;
	int node_id;

	/* If a DQP was just activated, we must first clear out any remaining
	 * messages still in the SRQ, otherwise we risk misordering incoming
	 * messages. That could happen if the other node transmitted messages
	 * to the SRQ, then switched to the DQP, but we scan the DQP first. */
	node_id = dqp->remote_src_ids[0].node_id;
	srq = &umc->srqs[node_id];
	lpp_umc_srq_read(lpp_epp, srq);
	dqp->flags &= ~LPP_DQP_NEW;
}

static void lpp_umc_dqp_read(struct lpp_ep *lpp_epp, struct lpp_umc_dqp *dqp)
{
	struct klpp_mc_params *mc_params = &lpp_epp->domain->devinfo.mc_params;
	struct klpp_mq *mq = dqp->rx;
	uint64_t *cons_idx = &dqp->rx_cons_idx;
	__u64 *remote_cons_idx = &dqp->tx->spad.cons_idx;
	//uint64_t *last_cons_idx = &dqp->rx_last_synced_cons_idx;
	struct klpp_msg_hdr *hdr;
	uint64_t cell_idx;
	size_t cmd_bytes;
	size_t cmd_cells;

	/* TODO: very similar to SRQ processing. See about factoring out the
	 * common bits. */
	cell_idx = *cons_idx % mc_params->dqp_cell_cnt;
	for (hdr = get_hdr(mq, cell_idx); hdr->type != KLPP_MSG_INVALID;
	     hdr = get_hdr(mq, cell_idx)) {
		if (unlikely(hdr->src_id > KLPP_UMC_MAX_EP ||
		    dqp->remote_src_ids[hdr->src_id].user_id == 0)) {
			FI_INFO(&lpp_prov, FI_LOG_EP_CTRL,
				"invalid src_id in message src_id: %u\n",
				hdr->src_id);
		} else {
			hdr->src_ep.fi_addr = dqp->remote_src_ids[hdr->src_id];
			if (unlikely(hdr->type == KLPP_MSG_DQP_SYNC)) {
				lpp_umc_dqp_sync(lpp_epp, dqp);
			} else {
				if (hdr->flags & KLPP_MSG_TAG_PERST) {
					hdr->tag = dqp->persistent_tag;
				} else if (hdr->flags & KLPP_MSG_TAG) {
					dqp->persistent_tag = hdr->tag;
				}
				process_msg(lpp_epp, hdr);
			}
		}

		cmd_bytes = klpp_msg_total_bytes(hdr);
		cmd_cells = DIV_ROUND_UP(cmd_bytes, KLPP_MQ_CELL_SIZE);
		for (uint64_t i = cell_idx; i < cell_idx + cmd_cells; i++) {
			hdr = get_hdr(mq, i);
			hdr->type = KLPP_MSG_INVALID;
		}

		*cons_idx += cmd_cells;
		cell_idx = *cons_idx % mc_params->dqp_cell_cnt;

		/* XXX: This isn't quite right. We need to make sure the remote
		 * side can always fit a max message size payload into the
		 * queue. So either max message size must be (mq_size / 2), or
		 * this needs to send consumer updates more frequently. Also
		 * remember the NOP message mechanism. Even though there may be
		 * enough cells for a given message, the sender may see EAGAIN
		 * because it would require splitting the message. So we need
		 * to send a cons_idx update whenever the number of free,
		 * contiguous cells is < max_msg_size.
		 * To reduce traffic on the wire, don't update the remote
		 * consumer index every time it changes locally.
		 * if (cell_idx - *last_cons_idx >= (mc_params->dqp_cell_cnt / 2)) {
		 *	compiler_barrier();
		 *	io_wr64((__u64*)cons_idx, remote_cons_idx);
		 *	*last_cons_idx = *cons_idx;
		 * } */
		io_wr64((__u64*)cons_idx, remote_cons_idx);
	}
}

static void lpp_umc_scan_dqp(struct lpp_ep *lpp_epp)
{
	struct lpp_domain *domain = lpp_epp->domain;
	struct lpp_umc *umc = lpp_epp->umc;
	struct klpp_mc_params *mc_params = &domain->devinfo.mc_params;
	struct lpp_umc_dqp *dqp;

	for (int i = 0; i < mc_params->n_dqp; i++) {
		dqp = &umc->dqps[i];
		if (dqp->flags & LPP_DQP_ENABLED) {
			lpp_umc_dqp_read(lpp_epp, dqp);
		}
	}
}

static int lpp_umc_send_ping(struct lpp_ep *lpp_epp, uint32_t node_id,
			     uint32_t umc_id, uint8_t umc_gen)
{
	struct klpp_msg_hdr hdr = { 0 };
	struct lpp_fi_addr dst_addr = { 0 };

	dst_addr.node_id = node_id;
	dst_addr.umc_id = umc_id;
	dst_addr.umc_gen = umc_gen;
	hdr.type = KLPP_MSG_PING;

	return lpp_umc_tx_simple(lpp_epp, dst_addr, &hdr, NULL, 0);
}

static void lpp_umc_check_keep_alive(struct lpp_ep *lpp_epp)
{
	struct lpp_umc *umc = lpp_epp->umc;
	struct lpp_umc_remote *rumc;
	struct dlist_entry *tmp_entry;
	int ret;

	if (unlikely(lpp_keep_alive_msec == 0))
		return;

	dlist_foreach_container_safe(&umc->keep_alive_rumcs,
	    struct lpp_umc_remote, rumc, dle, tmp_entry) {
		/* TODO: put a backoff on keep alives? That would be backoff on
		 * /successful/ PONG replies, not failures. If there is a
		 * remote guy with an unexp msg sitting in his queue for a long
		 * duration (and no other traffic), we might keep PINGing him
		 * every 3 seconds. In such a case, it might be better to back
		 * off and ping him less frequently.
		 *
		 * TODO: another keep alive option would be to use the spad.
		 * The scanning process could do something like place a
		 * monotonically increasing value in the spad, and if we want
		 * to make sure it's still alive, we can go read the spad and
		 * see if the value has increased. The benefit of using the
		 * spad is that we don't take up a slot in the queue, and more
		 * importantly, we don't have to deal with the possible issue
		 * where the queue is full each time we try to TX a ping, even
		 * though the other guy is alive and processing the queue. */
		if ((rumc->keep_alive_cnt == 0 &&
		     umc->prog_time - rumc->last_rx_time >
			     lpp_keep_alive_msec) ||
		    (rumc->keep_alive_cnt > 0 &&
		     umc->prog_time - rumc->last_ka_time >
			     lpp_keep_alive_msec)) {
			if (rumc->keep_alive_cnt >= lpp_keep_alive_retries) {
				FI_INFO(&lpp_prov, FI_LOG_EP_CTRL,
					"%u:%u path down; failed to reply to %ld keep alives\n",
					rumc->node_id, rumc->umc_id,
					lpp_keep_alive_retries);
				lpp_stx_path_down(lpp_epp, rumc->node_id,
						  rumc->umc_id);
				 /* Marking the path down should kill all
				  * tx_entries referencing it, effectively
				  * cleaning up this rumc. */
				assert(rumc->srq_refcount == 0);
			} else {
				rumc->keep_alive_cnt++;
				rumc->last_ka_time = umc->prog_time;
				ret = lpp_umc_send_ping(lpp_epp, rumc->node_id,
							rumc->umc_id,
							rumc->umc_gen);
				if (ret != 0) {
					FI_WARN(&lpp_prov, FI_LOG_EP_CTRL,
						"failed to send keep alive: %d\n",
						ret);
				}
			}
		} else {
			break;
		}
	}
}

/* XXX: This is a workaround for lost kservice events, which is a kernel-side
 * issue. When that is fully resolved, there will be no need for this function. */
static void lpp_umc_poll_cnt(struct lpp_ep *lpp_epp)
{
	struct lpp_umc *umc = lpp_epp->umc;

	umc->xxx_poll_cnt++;
	if (OFI_UNLIKELY(umc->xxx_poll_cnt % 128 == 0))
		umc->flags |= LPP_UMC_KSERVICE;
}

static void lpp_umc_rx_progress(struct lpp_ep *lpp_epp)
{
	lpp_epp->umc->prog_time = lpp_get_time_ms();
	lpp_umc_poll_cnt(lpp_epp);
	lpp_umc_read_k2u(lpp_epp);
	lpp_umc_scan_srq(lpp_epp);
	lpp_umc_scan_dqp(lpp_epp);
	/* TODO: If there are SRQ cmpls to flush, the head ptr could be
	 * provided in the usual kernel progres ioctl, saving a trip to the
	 * kernel. */
	lpp_cmpl_flush(lpp_epp);
	/* TODO: should kservice be handled up one level? I.e., the calling
	 * function would accumulate kernel work from all UMCs, then do one
	 * kernel transition? */
	lpp_umc_kservice(lpp_epp);
	lpp_umc_check_keep_alive(lpp_epp);
}

static void lpp_umc_norx_progress(struct lpp_ep *lpp_epp)
{
	lpp_umc_poll_cnt(lpp_epp);
	lpp_umc_read_k2u(lpp_epp);
	lpp_umc_scan_srq_kservice(lpp_epp);
	lpp_umc_kservice(lpp_epp);
}
