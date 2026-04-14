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

static const struct fi_ops_ep lpp_rx_ops = {
	.size		= sizeof(struct fi_ops_ep),
	.cancel		= lpp_fi_cancel,
	.getopt		= fi_no_getopt,
	.setopt		= lpp_fi_setopt,
	.tx_ctx		= fi_no_tx_ctx,
	.rx_ctx		= fi_no_rx_ctx,
	.rx_size_left	= fi_no_rx_size_left,
	.tx_size_left	= fi_no_tx_size_left,
};

static const struct fi_rx_attr lpp_default_rx_attr = {
	.caps			= LPP_CAPS,
	.mode			= 0,
	.op_flags		= 0,
	.msg_order		= FI_ORDER_RAR | FI_ORDER_RAW | FI_ORDER_WAR | FI_ORDER_WAW | FI_ORDER_SAS,
	.comp_order		= 0,
	.total_buffered_recv	= 0,
	.size			= LPP_RX_CTX_MAX_COUNT,
	.iov_limit		= KLPP_MAX_RECV_IOVS,
};

struct fi_rx_attr
lpp_rx_attrs(const struct klppioc_lf *klpp_devinfo)
{
	return lpp_default_rx_attr;
}

int lpp_verify_rx_attrs(const struct klppioc_lf *klpp_devinfo, struct fi_rx_attr *rx_attrs)
{

#define CHECK_ATTR(have, op, cmp)									\
	if ((have) op (cmp)) {										\
		FI_WARN(&lpp_prov, FI_LOG_FABRIC, "attr check failed: " #have " " #op " " #cmp "\n");	\
		return -FI_EINVAL;									\
	}

	CHECK_ATTR((rx_attrs->caps & ~LPP_CAPS),          !=,  0);
	CHECK_ATTR((rx_attrs->mode & LPP_MODE),           !=,  LPP_MODE);
	CHECK_ATTR((rx_attrs->op_flags & ~LPP_OP_FLAGS),  !=,  0);
	CHECK_ATTR((rx_attrs->msg_order & ~LPP_ORDER),    !=,  0);
	CHECK_ATTR(rx_attrs->comp_order,                  !=,  0);
	CHECK_ATTR(rx_attrs->total_buffered_recv,         !=,  0);
	CHECK_ATTR(rx_attrs->size,                        >,   LPP_RX_CTX_MAX_COUNT);
	CHECK_ATTR(rx_attrs->iov_limit,                   >,   KLPP_MAX_RECV_IOVS);

#undef CHECK_ATTR

	return 0;
}

struct lpp_rx *lpp_rx_context(struct lpp_ep *lpp_epp, struct fi_rx_attr *attr)
{
	struct lpp_rx *lpp_rxp = NULL;
	struct lpp_rx_entry *rx_entry;

	// Allocate the RX.
	if (lpp_rxp = calloc(1, sizeof(struct lpp_rx)), lpp_rxp == NULL)
		return NULL;

	lpp_rxp->domain = lpp_epp->domain;

	lpp_rxp->rx_fid.fid.fclass = FI_CLASS_RX_CTX;
	lpp_rxp->rx_fid.fid.context = NULL;
	lpp_rxp->rx_fid.fid.ops = NULL;

	lpp_rxp->rx_fid.ops = (struct fi_ops_ep *)&lpp_rx_ops;

	lpp_rxp->domain = lpp_epp->domain;
	lpp_rxp->is_enabled = 0;
	lpp_rxp->min_multi_recv = LPP_MIN_MULTI_RECV;

	if (attr == NULL)
		lpp_rxp->attr = lpp_rx_attrs(&lpp_epp->domain->devinfo);
	else
		lpp_rxp->attr = *attr;

	dlist_init(&lpp_rxp->rx_entry_freelist);
	dlist_init(&lpp_rxp->posted_rxs);
	dlist_init(&lpp_rxp->unexp_msgs);
	dlist_init(&lpp_rxp->unexp_claimed_msgs);

	// TODO: can attr.size == 0?
	rx_entry = calloc(lpp_rxp->attr.size, sizeof(*rx_entry));
	if (rx_entry == NULL)
		goto err_free_rx;

	lpp_rxp->rx_entries = rx_entry;
	for (int i = 0; i < lpp_rxp->attr.size; i++) {
		dlist_init(&rx_entry[i].pending_ops);
		dlist_init(&rx_entry[i].dle);
		dlist_insert_tail(&rx_entry[i].dle, &lpp_rxp->rx_entry_freelist);
	}

	// Create the context for this RX.
	if (lpp_ctx_create(lpp_rxp->domain, &lpp_rxp->ctx, KLPP_CTX_RX, lpp_rxp->attr.size) != 0)
		goto err_free_rx_entry;

	lpp_rxp->id = lpp_rxp->ctx.id;
	ofi_atomic_initialize32(&lpp_rxp->ref_count, 0);
	ofi_atomic_inc32(&lpp_rxp->domain->util_domain.ref);
	return lpp_rxp;

err_free_rx_entry:
	free(rx_entry);
err_free_rx:
	free(lpp_rxp);
	return NULL;
}

static void lpp_free_unexp(struct lpp_rx *lpp_rxp)
{
	struct lpp_rx_unexp_msg *unexp;
	struct dlist_entry *tmp_entry;
	struct dlist_entry *lists[] = { &lpp_rxp->unexp_msgs,
					&lpp_rxp->unexp_claimed_msgs };

	for (int i = 0; i < ARRAY_SIZE(lists); i++) {
		dlist_foreach_container_safe(lists[i], struct lpp_rx_unexp_msg,
					     unexp, dle, tmp_entry) {
			dlist_remove(&unexp->dle);
			free(unexp);
		}
	}
}

static void lpp_free_rx_op(struct lpp_rx *lpp_rxp)
{
	struct lpp_rx_entry *rx_entry;
	struct lpp_rx_op *rx_op;
	struct dlist_entry *tmp_entry;

	for (int i = 0; i < lpp_rxp->attr.size; i++) {
		rx_entry = &lpp_rxp->rx_entries[i];
		dlist_foreach_container_safe(&rx_entry->pending_ops,
					     struct lpp_rx_op, rx_op, dle,
					     tmp_entry) {
			if (rx_op->lpp_flags & LPP_RX_OP_MUSTFREE) {
				dlist_remove(&rx_op->dle);
				free(rx_op);
			}
		}
	}
}

int lpp_rx_close(struct lpp_rx *lpp_rxp)
{
	// TODO: We don't support shared RX. It's one-to-one with the endpoint.
	if (ofi_atomic_get32(&lpp_rxp->ref_count) != 0)
		return 0;

	if (klpp_ctx_close(&lpp_rxp->ctx) != 0)
		return -FI_ENODATA;

	if (lpp_rxp->cntr != NULL) {
		ofi_atomic_dec32(&lpp_rxp->cntr->ref_count);
		lpp_rxp->cntr = NULL;
	}

	if (lpp_rxp->cq != NULL) {
		ofi_atomic_dec32(&lpp_rxp->cq->ref_count);
		lpp_rxp->cq = NULL;
	}

	ofi_atomic_dec32(&lpp_rxp->domain->util_domain.ref);

	lpp_free_unexp(lpp_rxp);
	lpp_free_rx_op(lpp_rxp);
	free(lpp_rxp->rx_entries);
	free(lpp_rxp);
	return 0;
}

// TODO: util_buf_alloc(), ofi_mem stuff
struct lpp_rx_entry *lpp_rx_entry_get(struct lpp_rx *lpp_rxp)
{
	struct lpp_rx_entry *rx_entry;

	if (dlist_empty(&lpp_rxp->rx_entry_freelist))
		return NULL;

	dlist_pop_front(&lpp_rxp->rx_entry_freelist,
			struct lpp_rx_entry, rx_entry, dle);
	dlist_init(&rx_entry->dle);
	return rx_entry;
}

static void lpp_rx_entry_put(struct lpp_ep *lpp_epp, struct lpp_rx_entry *rx_entry)
{
	struct lpp_rx *lpp_rxp = lpp_epp->rx;
	struct lpp_cq *lpp_cqp = lpp_epp->cq_recv;

	if (rx_entry->lpp_flags & LPP_RX_ENTRY_CQ_RSRV)
		lpp_cq_unreserve_slot(lpp_cqp);

	if (rx_entry->lpp_flags & LPP_RX_ENTRY_MR) {
		lpp_mr_close_internal(rx_entry->mrp);
		rx_entry->mrp = NULL;
	}
	rx_entry->lpp_flags = 0;
	rx_entry->flags = 0;
	rx_entry->hmem_iface = 0;
	dlist_insert_head(&rx_entry->dle, &lpp_rxp->rx_entry_freelist);
}

static inline bool lpp_rx_entry_gpu(struct lpp_rx_entry *rx_entry)
{
#if HAVE_CUDA || HAVE_ROCR
	return (rx_entry->lpp_flags & LPP_RX_ENTRY_GPU) != 0;
#else
	return false;
#endif
}


static bool match_msg(struct lpp_rx *lpp_rxp, struct klpp_msg_hdr *hdr,
		      struct lpp_rx_entry *rx_entry)
{
	int tag = ((hdr->flags & KLPP_MSG_TAG) != 0) +
	    ((rx_entry->flags & FI_TAGGED) != 0);

	if ((tag == 2 &&
	    ((hdr->tag & ~rx_entry->ignore) != (rx_entry->tag & ~rx_entry->ignore))) ||
	    tag == 1) {
		return false;
	}

	if (lpp_rxp->attr.caps & FI_DIRECTED_RECV) {
		if (rx_entry->addr != FI_ADDR_UNSPEC &&
		    rx_entry->addr != hdr->src_ep.raw)
			return false;
	}

	return true;
}

static size_t rx_entry_bytes_left(struct lpp_rx_entry *rx_entry)
{
	return (rx_entry->total_length - rx_entry->offset);
}

static void rx_entry_force_consumed(struct lpp_rx_entry *rx_entry)
{
	rx_entry->offset = rx_entry->total_length;
}

static bool rx_entry_consumed(struct lpp_rx *lpp_rxp, struct lpp_rx_entry *rx_entry)
{
	size_t bytes_left;

	if (rx_entry->flags & FI_MULTI_RECV) {
		bytes_left = rx_entry_bytes_left(rx_entry);
		return (bytes_left < lpp_rxp->min_multi_recv ||
			bytes_left <= 0);
	} else {
		return true;
	}
}

/* TODO: better name?
 * Return true if the rx_entry can accept more messages, false if it is consumed. */
static bool recv_fit_msg(struct lpp_rx *lpp_rxp, struct klpp_msg_hdr *hdr,
			 struct lpp_rx_entry *rx_entry, struct lpp_rx_op *rx_op)
{
	size_t rx_bytes_left = rx_entry_bytes_left(rx_entry);

	if (hdr->data_length > rx_bytes_left) {
		rx_op->overflow_length = hdr->data_length - rx_bytes_left;
		rx_op->length = rx_bytes_left;
	} else {
		rx_op->overflow_length = 0;
		rx_op->length = hdr->data_length;
	}
	rx_op->tag = hdr->tag;
	rx_op->offset = rx_entry->offset;
	rx_entry->offset += rx_op->length;

	if (rx_entry_consumed(lpp_rxp, rx_entry)) {
		dlist_remove(&rx_entry->dle);
		return false;
	} else {
		return true;
	}
}

static void rx_entry_complete(struct lpp_ep *lpp_epp, struct lpp_rx_entry *rx_entry,
			      uint64_t tag, void *buf, size_t len,
			      size_t olen, fi_addr_t src_addr, int status)
{
	uint64_t flags = rx_entry->flags;

	flags &= ~FI_MULTI_RECV;

	lpp_completion_recv(lpp_epp, flags, rx_entry->data, rx_entry->context,
			    rx_entry->tag, buf, len, olen, src_addr, status);
}

// If threading == FI_THREAD_SAFE, must hold ep "op lock".
static void lpp_rx_op_comp(struct lpp_ep *lpp_epp, struct lpp_rx_op *rx_op,
			   int status)
{
	struct lpp_rx_entry *rx_entry = rx_op->rx_entry;

	rx_entry_complete(lpp_epp, rx_entry, rx_op->tag, rx_op->buf,
			  rx_op->length, rx_op->overflow_length,
			  rx_op->src_addr.raw, status);

	if (rx_op->lpp_flags & LPP_RX_OP_ONLIST)
		dlist_remove(&rx_op->dle);

	if (rx_entry_consumed(lpp_epp->rx, rx_entry) &&
	    dlist_empty(&rx_entry->pending_ops)) {
		if (rx_entry->flags & FI_MULTI_RECV) {
			lpp_completion_multirecv(lpp_epp, rx_entry->context);
		}
		lpp_rx_entry_put(lpp_epp, rx_entry);
	}
	if (rx_op->lpp_flags & LPP_RX_OP_MUSTFREE)
		free(rx_op);
}

// If threading == FI_THREAD_SAFE, must hold ep "op lock".
static void lpp_rx_op_pending(struct lpp_rx_op *rx_op)
{
	struct lpp_rx_entry *rx_entry = rx_op->rx_entry;

	dlist_init(&rx_op->dle);
	dlist_insert_tail(&rx_op->dle, &rx_entry->pending_ops);
	rx_op->lpp_flags |= LPP_RX_OP_ONLIST;
}

static void rx_op_comp_spool_cb(struct lpp_ep *lpp_epp, void *data, int status)
{
	struct lpp_rx_op *rx_op = data;

	lpp_rx_op_comp(lpp_epp, rx_op, status);
}

/* rx_op is presumed to be on the stack (and will be copied if it needs to live
 * beyond this function). */
static void rx_comp(struct lpp_ep *lpp_epp, struct klpp_msg_hdr *hdr,
		    struct lpp_rx_op *rx_op, int status)
{
	struct klpp_msg_hdr cmpl_hdr;
	struct lpp_rx_op *rx_op_heap;
	int ret;

	/* Remote side indicates its desire for completion notice by providing
	 * us with a token. If so, we need to withhold our local completion
	 * until the delivery complete notice has been flushed to the other
	 * side (this may happen immediately, or it may need to be delayed, due
	 * to, e.g., a full remote RX queue). If we did not withhold the local
	 * completion, the local side may believe it is safe to exit and
	 * inadvertently discard the spooled delivery complete notice. */
	if (hdr->flags & KLPP_MSG_TOKEN) {
		cmpl_hdr.type = KLPP_MSG_SEND_DELIV_COMP;
		cmpl_hdr.flags = KLPP_MSG_TOKEN;
		cmpl_hdr.token = hdr->token;
		cmpl_hdr.status = status;
		ret = lpp_umc_tx_cmpl(lpp_epp, hdr->src_ep.fi_addr, &cmpl_hdr,
				      NULL, 0, rx_op_comp_spool_cb,
				      sizeof(*rx_op), (void **)&rx_op_heap);
		if (ret == -FI_EAGAIN) {
			/* If the TX queues have space, no heap allocations are
			 * made, and the rx_op on the stack is the only one
			 * ever needed. However, if the TX queues are backed
			 * up, we need to clone the struct onto the heap so
			 * it'll be accessible for retries later.
			 * lpp_umc_tx_cmpl() includes space for this in its
			 * malloc() call, since we specified how much is
			 * needed at call time. */
			memcpy(rx_op_heap, rx_op, sizeof(*rx_op));
			lpp_rx_op_pending(rx_op_heap);
		} else {
			/* We may have failed to tell the other side that the
			 * operation is done (if ret != 0), but we'll still
			 * fire the local completion as usual. Remote side will
			 * presumably time out. */
			lpp_rx_op_comp(lpp_epp, rx_op, status);
		}
	} else {
		lpp_rx_op_comp(lpp_epp, rx_op, status);
	}
}

static void recv_eager(struct lpp_ep *lpp_epp, struct lpp_rx_entry *rx_entry,
		       struct klpp_msg_hdr *hdr, struct lpp_rx_op *rx_op)
{
	int rc;
	uint8_t *srcbuf;
	void *dstbuf;
	struct iovec *iov;
	size_t bytes_left;
	size_t offset;

	if (hdr->flags & KLPP_MSG_DATA) {
		srcbuf = (uint8_t *)klpp_msg_data_offset(hdr, hdr->data_length);
		bytes_left = rx_op->length;
		offset = rx_op->offset;

		rx_op->buf = rx_entry->msg_iov[0].iov_base;
		for (size_t i = 0; i < rx_entry->iov_count && bytes_left > 0; i++) {
			iov = &rx_entry->msg_iov[i];
			if (offset > iov->iov_len) {
				offset -= iov->iov_len;
				continue;
			} else if (offset > 0) {
				rx_op->buf = (uint8_t*)iov->iov_base + offset;
				dstbuf = rx_op->buf;
				offset = 0;
			} else {
				dstbuf = iov->iov_base;
			}
			if(lpp_rx_entry_gpu(rx_entry)) {
				rc = ofi_copy_to_hmem(rx_op->hmem_iface, -1,
						 dstbuf, srcbuf,
						 MIN(iov->iov_len, bytes_left));
				if(rc)
					FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
						"ofi_copy_to_hmem error (%d)",
						rc);
			} else {
				memcpy(dstbuf, srcbuf, MIN(iov->iov_len, bytes_left));
			}

			srcbuf += iov->iov_len;
			bytes_left -= iov->iov_len;
		}
		// Make sure data visible before generating completion. GPU sync if hmem?
		compiler_barrier();
	}

	rx_comp(lpp_epp, hdr, rx_op, 0);
}

static void recv_rdzv(struct lpp_ep *lpp_epp, struct lpp_rx_entry *rx_entry,
		      struct klpp_msg_hdr *hdr, struct lpp_rx_op *rx_op)
{
	struct klpp_umc_u2k u2k;
	int ret;

	if (!(rx_entry->lpp_flags & LPP_RX_ENTRY_MR)) {
		ret = lpp_mr_reg_internal(lpp_epp->domain,
					  rx_entry->msg_iov[0].iov_base,
					  rx_entry->msg_iov[0].iov_len, FI_RECV,
					  0, rx_entry->hmem_iface, NULL,
					  &rx_entry->mrp);

		if (ret) {
			FI_WARN(&lpp_prov, FI_LOG_EP_DATA, "failed to reg MR\n");
			rx_comp(lpp_epp, hdr, rx_op, -FI_ENOMEM);
			return;
		}
		rx_entry->lpp_flags |= LPP_RX_ENTRY_MR;
	}

	u2k.type = KLPP_U2K_RDZV_RECV;
	u2k.rdzv_recv.rx_op_ptr = (__u64)rx_op;
	u2k.rdzv_recv.token = hdr->token;
	u2k.rdzv_recv.base_uaddr = (__u64)rx_entry->msg_iov[0].iov_base;
	u2k.rdzv_recv.base_length = rx_entry->msg_iov[0].iov_len;
	u2k.rdzv_recv.offset = rx_op->offset;
	u2k.rdzv_recv.length = rx_op->length;
	u2k.rdzv_recv.send_addr = hdr->src_ep.fi_addr;
	u2k.rdzv_recv.recv_addr = lpp_epp->addr.fi_addr;

	lpp_umc_u2k_enqueue(lpp_epp, &u2k);
}

static void do_recv(struct lpp_ep *lpp_epp, struct klpp_msg_hdr *hdr,
		    struct lpp_rx_entry *rx_entry, bool *more)
{
	FI_DBG(&lpp_prov, FI_LOG_EP_DATA,
		"len: %u, hdr->TAG:%d (0x%llx) hdr->TOKEN:%d (0x%llx) hdr->RCQ_DATA:%d (0x%llx)\n",
		hdr->data_length,
		!!(hdr->flags & KLPP_MSG_TAG), hdr->tag,
		!!(hdr->flags & KLPP_MSG_TOKEN), hdr->token,
		!!(hdr->flags & KLPP_MSG_RCQ_DATA), hdr->rcq_data
	);

	if (hdr->flags & KLPP_MSG_RCQ_DATA) {
		rx_entry->flags |= FI_REMOTE_CQ_DATA;
		rx_entry->data = hdr->rcq_data;
	}

	if (hdr->type == KLPP_MSG_EAGER_SEND) {
		struct lpp_rx_op rx_op = { .rx_entry = rx_entry,
					   .src_addr = hdr->src_ep,
					   .hmem_iface = rx_entry->hmem_iface };

		*more = recv_fit_msg(lpp_epp->rx, hdr, rx_entry, &rx_op);

		recv_eager(lpp_epp, rx_entry, hdr, &rx_op);
	} else {
		struct lpp_rx_op *rx_op;

		// TODO util_buf_alloc
		rx_op = malloc(sizeof(*rx_op));
		rx_op->lpp_flags = LPP_RX_OP_MUSTFREE;
		rx_op->rx_entry = rx_entry;
		rx_op->src_addr = hdr->src_ep;
		lpp_rx_op_pending(rx_op);
		*more = recv_fit_msg(lpp_epp->rx, hdr, rx_entry, rx_op);
		recv_rdzv(lpp_epp, rx_entry, hdr, rx_op);
	}
}

ssize_t lpp_rx_verify_iov_count(struct lpp_rx *lpp_rxp, size_t iov_count)
{
	if (iov_count > lpp_rxp->attr.iov_limit) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
		    "iov_count out of range (%ld/%ld)\n",
		    iov_count, lpp_rxp->attr.iov_limit);
		return -FI_EINVAL;
	}
	return 0;
}

ssize_t lpp_rx_verify_flags(struct lpp_rx *lpp_rxp, uint64_t flags)
{
	if ((lpp_rxp->attr.caps & flags & LPP_CAPS_OPS) !=
	    (flags & LPP_CAPS_OPS)) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
		    "RX attr caps (%lx) incorrect for requested op (%lx)\n",
		    lpp_rxp->attr.caps, flags);
		return -FI_EINVAL;
	}
	return 0;
}

static int lpp_rx_send_discard(struct lpp_ep *lpp_epp,
			       struct klpp_msg_hdr *discard_hdr)
{
	struct klpp_msg_hdr hdr = {
	    .type = KLPP_MSG_DISCARD_SEND,
	    .token = discard_hdr->token,
	    .flags = KLPP_MSG_TOKEN,
	};

	return lpp_umc_tx_simple(lpp_epp, discard_hdr->src_ep.fi_addr, &hdr,
	    NULL, 0);
}

// If threading == FI_THREAD_SAFE, must hold ep "op lock".
ssize_t lpp_rx_common(struct lpp_ep *lpp_epp, struct lpp_rx_entry *rx_entry)
{
	struct lpp_rx *lpp_rxp = lpp_epp->rx;
	struct lpp_cq *lpp_cqp = lpp_epp->cq_recv;
	struct lpp_rx_unexp_msg *unexp;
	struct dlist_entry *search_q;
	struct dlist_entry *tmp_entry;
	bool more;
	bool matched_msg = false;
	bool claiming;
	size_t gpu_cnt;
	ssize_t ret;

	rx_entry->offset = 0;
	rx_entry->total_length = 0;
	gpu_cnt = 0;
	for (size_t i = 0; i < rx_entry->iov_count; i++) {
		rx_entry->total_length += rx_entry->msg_iov[i].iov_len;
		ret = lpp_hmem_ptr(lpp_epp->domain, rx_entry->msg_iov[i].iov_base,
				   &rx_entry->hmem_iface);
		if (ret) {
			FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
				"failed to determine hmem iface\n");
			goto err;
		}
		if (rx_entry->hmem_iface) {
			rx_entry->lpp_flags |= LPP_RX_ENTRY_GPU;
			if (OFI_UNLIKELY(gpu_cnt != i)) {
				FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
					"mixed CPU and host iovs are illegal\n");
				goto err;
			}
			gpu_cnt++;
			// TODO: fix
			if (gpu_cnt > 1) {
				FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
					"GPU buffers may only have 1 iov\n");
				goto err;
			}
		}
	}

	FI_DBG(&lpp_prov, FI_LOG_EP_DATA, "len: %ld, flags: 0x%lx lpp_flags: 0x%lx\n",
		rx_entry->total_length, rx_entry->flags, rx_entry->lpp_flags);

	/* TODO: do we really need this check?
	 * Check that the count of IOVs is in range. Zero length recvs are
	 * acceptable only in the case that no data will actually be received;
	 * i.e., we are performing a PEEK/CLAIM/DISCARD operation that does not
	 * recv the data.
	 * pcd_bits = rx_entry->flags & (FI_PEEK | FI_CLAIM | FI_DISCARD);
	 * if (rx_entry->iov_count == 0 && (pcd_bits == 0 || pcd_bits == FI_CLAIM)) {
	 *	FI_DBG(&lpp_prov, FI_LOG_EP_DATA, "iov_count is zero for recv\n");
	 *	ret = -FI_EINVAL;
	 *	goto err;
	 * } */

	ret = lpp_cq_reserve_slot(lpp_cqp, rx_entry->flags);
	if (ret > 0) {
		rx_entry->lpp_flags |= LPP_RX_ENTRY_CQ_RSRV;
	} else if (ret < 0) {
		FI_INFO(&lpp_prov, FI_LOG_EP_DATA,
			"cannot start RX operation as it may overrun CQ\n");
		ret = -FI_EAGAIN;
		goto err;
	}

	// Previously claimed messages will have been transferred to a separate queue.
	if ((rx_entry->flags & (FI_PEEK | FI_CLAIM)) == FI_CLAIM) {
		search_q = &lpp_rxp->unexp_claimed_msgs;
		claiming = true;
	} else {
		search_q = &lpp_rxp->unexp_msgs;
		claiming = false;
	}

	/* If we're here just to peek/claim/discard, we won't actually post to
	 * the RX queue, but rather will complete immediately after searching
	 * the queue below. */
	if ((rx_entry->flags & (FI_PEEK | FI_CLAIM | FI_DISCARD)) == 0)
		dlist_insert_tail(&rx_entry->dle, &lpp_rxp->posted_rxs);

	dlist_foreach_container_safe(search_q, struct lpp_rx_unexp_msg, unexp,
				     dle, tmp_entry) {
		if ((claiming && unexp->claim_context == rx_entry->context) ||
		    match_msg(lpp_rxp, unexp->hdr, rx_entry)) {
			matched_msg = true;
			switch (rx_entry->flags &
			    (FI_PEEK | FI_CLAIM | FI_DISCARD)) {
			case (FI_PEEK | FI_CLAIM):
				dlist_remove(&unexp->dle);
				dlist_insert_tail(&unexp->dle,
				    &lpp_rxp->unexp_claimed_msgs);
				unexp->claim_context = rx_entry->context;
				// Fallthrough
			case FI_PEEK:
				rx_entry_complete(lpp_epp, rx_entry,
						  unexp->hdr->tag, NULL,
						  unexp->hdr->data_length, 0,
						  unexp->hdr->src_ep.raw, 0);
				lpp_rx_entry_put(lpp_epp, rx_entry);
				goto out;
				break;
			 case (FI_PEEK | FI_DISCARD):
			 case (FI_CLAIM | FI_DISCARD):
				ret = lpp_rx_send_discard(lpp_epp, unexp->hdr);
				if (ret != 0)
					goto err;

				rx_entry_complete(lpp_epp, rx_entry,
						  unexp->hdr->tag, NULL,
						  unexp->hdr->data_length, 0,
						  unexp->hdr->src_ep.raw, 0);
				dlist_remove(&unexp->dle);
				free(unexp);
				lpp_rx_entry_put(lpp_epp, rx_entry);
				goto out;
				break;
			case FI_CLAIM:
			default:
				// Fallthrough to normal rx processing below.
				break;
			}
			do_recv(lpp_epp, unexp->hdr, rx_entry, &more);
			dlist_remove(&unexp->dle);
			free(unexp);
			if (!more) {
				break;
			}
		}
	}

	/* TODO: Does this belong here? The usual rules of FI_PROGRESS_MANUAL
	 * are that progress is made when you read the cntr/cq.
	 * lpp_umc_kservice(lpp_epp);
	 * PEEK/CLAIM recvs never wait on the posted queue. If no matching cmds
	 * were found, we fail immediately. */
	if ((rx_entry->flags & (FI_PEEK | FI_CLAIM)) && !matched_msg) {
		rx_entry_complete(lpp_epp, rx_entry, 0, NULL, 0, 0,
				  FI_ADDR_UNSPEC, ENOMSG);
		lpp_rx_entry_put(lpp_epp, rx_entry);
	}
out:
	return 0;

err:
	lpp_rx_entry_put(lpp_epp, rx_entry);
	return ret;
}

// If threading == FI_THREAD_SAFE, must hold ep "op lock".
int lpp_rx_process_msg(struct lpp_ep *lpp_epp, struct klpp_msg_hdr *hdr)
{
	struct lpp_rx *lpp_rxp = lpp_epp->rx;
	struct lpp_rx_entry *rx_entry;
	struct dlist_entry *tmp_entry;
	struct lpp_rx_unexp_msg *unexp;
	size_t unexp_size;
	bool more;

	dlist_foreach_container_safe(&lpp_rxp->posted_rxs,
	    struct lpp_rx_entry, rx_entry, dle, tmp_entry) {
		if (match_msg(lpp_rxp, hdr, rx_entry)) {
			do_recv(lpp_epp, hdr, rx_entry, &more);
			goto out;
		}
	}

	unexp_size = sizeof(*unexp) + klpp_msg_total_bytes(hdr);
	unexp = malloc(unexp_size);
	if (unexp == NULL) {
		perror("TODO, failed to alloc unexp msg\n");
		goto out;
	}
	memcpy(unexp->hdr, hdr, klpp_msg_total_bytes(hdr));
	dlist_insert_tail(&unexp->dle, &lpp_rxp->unexp_msgs);

	FI_DBG(&lpp_prov, FI_LOG_EP_DATA,"add to unexp hdr->TAG:%d (0x%llx) hdr->RCQ_DATA:%d (0x%llx)\n",
		!!(hdr->flags & KLPP_MSG_TAG), hdr->tag,
		!!(hdr->flags & KLPP_MSG_RCQ_DATA), hdr->rcq_data
	);
out:
	return 0;
}

// If threading == FI_THREAD_SAFE, must hold ep "op lock".
void lpp_rx_rdzv_done(struct lpp_ep *lpp_epp, struct klpp_umc_k2u *k2u)
{
	struct lpp_rx_op *rx_op;
	struct lpp_rx_entry *rx_entry;
	struct iovec *iov;
	size_t offset;

	rx_op = (struct lpp_rx_op*)k2u->rdzv_recv_done.rx_op_ptr;
	rx_entry = rx_op->rx_entry;

	/* TODO is this a redundant computation? Kernel needs to do something
	 * like this right? */
	rx_op->buf = rx_entry->msg_iov[0].iov_base;
	offset = rx_op->offset;
	for (int i = 0; offset > 0; i++) {
		iov = &rx_entry->msg_iov[i];
		if (offset > iov->iov_len) {
			offset -= iov->iov_len;
		} else {
			rx_op->buf = (uint8_t*)iov->iov_base + offset;
			break;
		}
	}

	/* TODO: if status != 0, should we send the remote guy a DELIV_COMP?
	 * Is that okay if the kernel already sent a failure msg via
	 * ntb_transport (should be; if the other side already dropped the
	 * associated tx_entry, it should see the incorrect generation and drop
	 * our DELIV_COMP msg harmlessly). We need the sender's address and
	 * token to do this (rx_op has to store that?). */
	lpp_rx_op_comp(lpp_epp, rx_op, k2u->rdzv_recv_done.status);
}

// If threading == FI_THREAD_SAFE, must hold ep "op lock".
int lpp_rx_cancel(struct lpp_ep *lpp_epp, void *context)
{
	struct lpp_rx *lpp_rxp = lpp_epp->rx;
	struct lpp_rx_entry *rx_entry;
	struct dlist_entry *tmp_entry;

	dlist_foreach_container_safe(&lpp_rxp->posted_rxs,
	    struct lpp_rx_entry, rx_entry, dle, tmp_entry) {
		if (rx_entry->context == context) {
			rx_entry_complete(lpp_epp, rx_entry, 0, NULL, 0, 0,
					  FI_ADDR_UNSPEC, ECANCELED);
			/* TODO: review this behavior w/r/t upper layer requirements.
			 * If there are pending ops, should the cancel fail? */
			dlist_remove(&rx_entry->dle);
			rx_entry_force_consumed(rx_entry);
			if (dlist_empty(&rx_entry->pending_ops))
				lpp_rx_entry_put(lpp_epp, rx_entry);

			break;
		}
	}
	return 0;
}
