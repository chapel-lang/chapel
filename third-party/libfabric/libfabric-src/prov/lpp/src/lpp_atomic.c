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

#include <stdlib.h>

#include "lpp.h"

struct lpp_atomic_spool_cbarg {
	void *result_buf;
};

void lpp_atomic_handle_cmpl(struct lpp_ep *lpp_epp, struct lpp_tx_entry *tx_entry,
			    struct klpp_msg_hdr *hdr)
{
	struct lpp_atomic_hdr *atomic_hdr;
	void *msg_data;

	atomic_hdr = &tx_entry->atomic_hdr;

	switch (atomic_hdr->mode) {
	case LPP_ATOMIC:
		lpp_completion_transmit(lpp_epp, lpp_epp->cntr_write,
					tx_entry->flags,
					tx_entry->msg_hdr.rcq_data,
					tx_entry->context, hdr->status);
		break;
	case LPP_ATOMIC_FETCH:
	case LPP_ATOMIC_CMP:
		/* "status" and "data_length" are a union. If data is present,
		 * we assume success and process the data. Otherwise, treat it
		 * as status. */
		if (!(hdr->flags & KLPP_MSG_DATA)) {
			lpp_completion_transmit(lpp_epp, lpp_epp->cntr_write,
						tx_entry->flags,
						tx_entry->msg_hdr.rcq_data,
						tx_entry->context, hdr->status);
		} else {
			msg_data = klpp_msg_data_offset(hdr, hdr->data_length);
			memcpy(tx_entry->atomic_result.iov_base, msg_data,
			       MIN(hdr->data_length,
				   tx_entry->atomic_result.iov_len));
			lpp_completion_transmit(lpp_epp, lpp_epp->cntr_write,
						tx_entry->flags,
						tx_entry->msg_hdr.rcq_data,
						tx_entry->context, 0);
		}
		break;
	default:
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA, "invalid atomic mode %u\n",
			atomic_hdr->mode);
	}
}

static void atomic_comp_spool_cb(struct lpp_ep *lpp_epp, void *data, int status)
{
	struct lpp_atomic_spool_cbarg *cbarg = data;

	free(cbarg->result_buf);
}

static void *atomic_res_buf_alloc(size_t size)
{
	void *buf;

	/* TODO: buffer pool? */
	if (posix_memalign(&buf, KLPP_MQ_CELL_SIZE, size) != 0) {
		return NULL;
	} else {
		return buf;
	}
}

void lpp_atomic_process_msg(struct lpp_ep *lpp_epp, struct klpp_msg_hdr *hdr)
{
	struct lpp_atomic_hdr *atomic_hdr;
	struct lpp_atomic_spool_cbarg *cbarg;
	struct klpp_msg_hdr cmpl_hdr;
	void *atomic_data;
	void *atomic_cmp_data;
	void *local_data;
	void *local_end;
	size_t typesize;
	size_t count;
	struct lpp_mr *lpp_mrp = NULL;
	uint64_t want_access;
	uint64_t valid_flags;
	size_t payload_length;
	size_t op_length;
	void *res_data;
	int ret;

	atomic_hdr = (struct lpp_atomic_hdr *)klpp_msg_data_offset(
		hdr, hdr->data_length);
	atomic_data = (atomic_hdr + 1);
	typesize = ofi_datatype_size(atomic_hdr->datatype);
	count = atomic_hdr->count;
	op_length = count * typesize;

	cmpl_hdr.type = KLPP_MSG_ATOMIC_DELIV_COMP;
	cmpl_hdr.flags = KLPP_MSG_TOKEN;
	cmpl_hdr.token = hdr->token;

	switch (atomic_hdr->mode) {
	case LPP_ATOMIC:
		valid_flags = 0;
		want_access = FI_REMOTE_WRITE;
		payload_length = op_length;
		break;
	case LPP_ATOMIC_FETCH:
		valid_flags = FI_FETCH_ATOMIC;
		payload_length = op_length;
		if (atomic_hdr->op == FI_ATOMIC_READ) {
			want_access = FI_REMOTE_READ;
		} else {
			want_access = FI_REMOTE_WRITE | FI_REMOTE_READ;
		}
		if (!(hdr->flags & KLPP_MSG_TOKEN)) {
			FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
				"atomic fetch missing required token\n");
			// No way to reply; all we can do is ignore the message.
			return;
		}
		break;
	case LPP_ATOMIC_CMP:
		valid_flags = FI_COMPARE_ATOMIC;
		payload_length = op_length * 2;
		want_access = FI_REMOTE_WRITE | FI_REMOTE_READ;
		if (!(hdr->flags & KLPP_MSG_TOKEN)) {
			FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
				"atomic fetch missing required token\n");
			// No way to reply; all we can do is ignore the message.
			return;
		}
		break;
	default:
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA, "invalid atomic mode %u\n",
			atomic_hdr->mode);
		cmpl_hdr.status = -FI_EINVAL;
		goto err_tx_cmpl;
	}

	if (atomic_hdr->op != FI_ATOMIC_READ &&
	    hdr->data_length - sizeof(*atomic_hdr) != payload_length) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
			"data length mismatch, data_length %zu payload_length %zu\n",
			hdr->data_length - sizeof(*atomic_hdr), payload_length);
		cmpl_hdr.status = -FI_EINVAL;
		goto err_tx_cmpl;
	}

	ret = ofi_atomic_valid(&lpp_prov, atomic_hdr->datatype, atomic_hdr->op,
			       valid_flags);
	if (ret != 0) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
			"atomic datatype %u op %u flags %lx is invalid\n",
			atomic_hdr->datatype, atomic_hdr->op, valid_flags);
		cmpl_hdr.status = -FI_EINVAL;
		goto err_tx_cmpl;
	}

	lpp_mrp = lpp_mr_cache_find(lpp_epp->domain, (void *) atomic_hdr->addr,
				    op_length, atomic_hdr->key);

	if (lpp_mrp == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA, "no MR for key %lx\n",
			atomic_hdr->key);
		cmpl_hdr.status = -FI_ENOENT;
		goto err_tx_cmpl;
	}

	if ((lpp_mrp->attr.access & want_access) != want_access) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
			"invalid access for atomic write dest: have %lx, want %lx\n",
			lpp_mrp->attr.access, want_access);
		cmpl_hdr.status = -FI_EACCES;
		goto err_tx_cmpl;
	}

	local_data = (void *)atomic_hdr->addr;
	local_end = (char *)local_data + op_length;
	if (local_data < lpp_mrp->attr.mr_iov[0].iov_base ||
	    local_end > (void *)((char *)lpp_mrp->attr.mr_iov[0].iov_base +
					lpp_mrp->attr.mr_iov[0].iov_len)) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
			"local buffer with key %lx is out of range, have [%p,%p] want [%p,%p]\n",
			atomic_hdr->key, lpp_mrp->attr.mr_iov[0].iov_base,
			(char *)lpp_mrp->attr.mr_iov[0].iov_base +
				lpp_mrp->attr.mr_iov[0].iov_len,
			local_data, local_end);
		cmpl_hdr.status = -FI_EINVAL;
		goto err_tx_cmpl;
	}

	switch (atomic_hdr->mode) {
	case LPP_ATOMIC:
		ofi_atomic_write_handlers[atomic_hdr->op][atomic_hdr->datatype](
			local_data, atomic_data, count);

		if (hdr->flags & KLPP_MSG_TOKEN) {
			cmpl_hdr.status = 0;
			lpp_umc_tx_cmpl(lpp_epp, hdr->src_ep.fi_addr, &cmpl_hdr,
					NULL, 0, NULL, 0, NULL);
		}
		break;
	case LPP_ATOMIC_FETCH:
		res_data = atomic_res_buf_alloc(op_length);
		if (res_data == NULL) {
			cmpl_hdr.status = -FI_ENOMEM;
			goto err_tx_cmpl;
		}

		ofi_atomic_readwrite_handlers[atomic_hdr->op]
					     [atomic_hdr->datatype](local_data,
								    atomic_data,
								    res_data,
								    count);

		cmpl_hdr.flags |= KLPP_MSG_DATA;
		cmpl_hdr.data_length = op_length;
		ret = lpp_umc_tx_cmpl(lpp_epp, hdr->src_ep.fi_addr, &cmpl_hdr,
				      res_data, cmpl_hdr.data_length,
				      atomic_comp_spool_cb, sizeof(*cbarg),
				      (void **)&cbarg);
		if (ret == -FI_EAGAIN) {
			cbarg->result_buf = res_data;
		} else {
			free(res_data);
		}

		break;
	case LPP_ATOMIC_CMP:
		atomic_cmp_data = (char *)atomic_data + op_length;

		res_data = atomic_res_buf_alloc(op_length);
		if (res_data == NULL) {
			cmpl_hdr.status = -FI_ENOMEM;
			goto err_tx_cmpl;
		}

		ofi_atomic_swap_handlers[atomic_hdr->op - OFI_SWAP_OP_START]
					[atomic_hdr->datatype](local_data,
							       atomic_data,
							       atomic_cmp_data,
							       res_data, count);

		cmpl_hdr.flags |= KLPP_MSG_DATA;
		cmpl_hdr.data_length = op_length;
		ret = lpp_umc_tx_cmpl(lpp_epp, hdr->src_ep.fi_addr, &cmpl_hdr,
				      res_data, cmpl_hdr.data_length,
				      atomic_comp_spool_cb, sizeof(*cbarg),
				      (void **)&cbarg);
		if (ret == -FI_EAGAIN) {
			cbarg->result_buf = res_data;
		} else {
			free(res_data);
		}
		break;
	}

	lpp_mr_cache_drop(lpp_epp->domain, lpp_mrp);

	return;

err_tx_cmpl:
	if (hdr->flags & KLPP_MSG_TOKEN) {
		lpp_umc_tx_cmpl(lpp_epp, hdr->src_ep.fi_addr, &cmpl_hdr, NULL,
				0, NULL, 0, NULL);
	}
	if (lpp_mrp)
		lpp_mr_cache_drop(lpp_epp->domain, lpp_mrp);
}

static ssize_t lpp_fi_atomic(struct fid_ep *ep, const void *buf, size_t count,
			     void *desc, fi_addr_t dest_addr, uint64_t addr,
			     uint64_t key, enum fi_datatype datatype,
			     enum fi_op op, void *context)
{
	struct lpp_tx_entry *tx_entry;
	struct lpp_ep *lpp_epp;
	struct lpp_stx *lpp_stxp;
	uint64_t comp_flag;
	size_t typesize;
	ssize_t ret;

	lpp_epp = container_of(ep, struct lpp_ep, ep_fid);
	lpp_stxp = lpp_epp->stx;

	ret = ofi_atomic_valid(&lpp_prov, datatype, op, 0);
	if (ret != 0) {
		return ret;
	}

	comp_flag = lpp_cq_comp_flag(lpp_epp->transmit_bind_flags,
				     lpp_stxp->attr.op_flags);

	lpp_ep_op_lock(lpp_epp);

	tx_entry = lpp_tx_entry_get(lpp_stxp);
	if (tx_entry == NULL) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	typesize = ofi_datatype_size(datatype);
	tx_entry->msg_iov[0].iov_base = &tx_entry->atomic_hdr;
	tx_entry->msg_iov[0].iov_len = sizeof(tx_entry->atomic_hdr);
	tx_entry->msg_iov[1].iov_base = (void *)buf;
	tx_entry->msg_iov[1].iov_len = count * typesize;
	tx_entry->iov_count = 2;
	tx_entry->flags =
		FI_ATOMIC | comp_flag | (lpp_stxp->attr.op_flags & FI_INJECT);
	tx_entry->context = context;
	tx_entry->dst_addr.raw = dest_addr;
	tx_entry->msg_hdr.flags = lpp_stx_msg_hdr_flags(0, 0, lpp_stxp);
	tx_entry->msg_hdr.token = lpp_tx_entry_to_token(lpp_stxp, tx_entry);

	tx_entry->atomic_hdr.mode = LPP_ATOMIC;
	tx_entry->atomic_hdr.op = op;
	tx_entry->atomic_hdr.datatype = datatype;
	tx_entry->atomic_hdr.key = key;
	tx_entry->atomic_hdr.addr = addr;
	tx_entry->atomic_hdr.count = count;

	ret = lpp_atomic_common(lpp_epp, tx_entry);

unlock:
	lpp_ep_op_unlock(lpp_epp);
	return ret;
}

static ssize_t lpp_fi_atomicmsg(struct fid_ep *ep,
				const struct fi_msg_atomic *msg, uint64_t flags)
{
	struct lpp_tx_entry *tx_entry;
	struct lpp_ep *lpp_epp;
	struct lpp_stx *lpp_stxp;
	uint64_t comp_flag;
	size_t typesize;
	ssize_t ret;

	lpp_epp = container_of(ep, struct lpp_ep, ep_fid);
	lpp_stxp = lpp_epp->stx;

	if (flags & FI_TAGGED)
		return -FI_EBADFLAGS;

	ret = ofi_atomic_valid(&lpp_prov, msg->datatype, msg->op, 0);
	if (ret != 0)
		return ret;

	ret = lpp_tx_verify_rma_iov_count(lpp_stxp, msg->iov_count);
	if (ret != 0)
		return ret;

	ret = lpp_tx_verify_rma_iov_count(lpp_stxp, msg->rma_iov_count);
	if (ret != 0)
		return ret;

	ret = lpp_tx_verify_flags(lpp_stxp, flags);
	if (ret != 0)
		return ret;

	if (msg->rma_iov[0].count < msg->msg_iov[0].count)
		return -FI_EINVAL;

	comp_flag = lpp_cq_comp_flag(lpp_epp->transmit_bind_flags, flags);

	lpp_ep_op_lock(lpp_epp);

	tx_entry = lpp_tx_entry_get(lpp_stxp);
	if (tx_entry == NULL) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	typesize = ofi_datatype_size(msg->datatype);
	tx_entry->msg_iov[0].iov_base = &tx_entry->atomic_hdr;
	tx_entry->msg_iov[0].iov_len = sizeof(tx_entry->atomic_hdr);
	tx_entry->msg_iov[1].iov_base = msg->msg_iov[0].addr;
	tx_entry->msg_iov[1].iov_len = msg->msg_iov[0].count * typesize;
	tx_entry->iov_count = 2;
	tx_entry->flags = FI_ATOMIC | comp_flag | (flags & FI_INJECT);
	tx_entry->context = msg->context;
	tx_entry->dst_addr.raw = msg->addr;
	tx_entry->msg_hdr.flags = lpp_stx_msg_hdr_flags(flags, 0, lpp_stxp);
	tx_entry->msg_hdr.token = lpp_tx_entry_to_token(lpp_stxp, tx_entry);

	tx_entry->atomic_hdr.mode = LPP_ATOMIC;
	tx_entry->atomic_hdr.op = msg->op;
	tx_entry->atomic_hdr.datatype = msg->datatype;
	tx_entry->atomic_hdr.key = msg->rma_iov[0].key;
	tx_entry->atomic_hdr.addr = msg->rma_iov[0].addr;
	tx_entry->atomic_hdr.count = msg->msg_iov[0].count;

	ret = lpp_atomic_common(lpp_epp, tx_entry);

unlock:
	lpp_ep_op_unlock(lpp_epp);
	return ret;
}

static ssize_t lpp_fi_fetch_atomic(struct fid_ep *ep, const void *buf,
				   size_t count, void *desc, void *result,
				   void *result_desc, fi_addr_t dest_addr,
				   uint64_t addr, uint64_t key,
				   enum fi_datatype datatype, enum fi_op op,
				   void *context)
{
	struct lpp_tx_entry *tx_entry;
	struct lpp_ep *lpp_epp;
	struct lpp_stx *lpp_stxp;
	uint64_t comp_flag;
	size_t typesize;
	ssize_t ret;

	lpp_epp = container_of(ep, struct lpp_ep, ep_fid);
	lpp_stxp = lpp_epp->stx;

	ret = ofi_atomic_valid(&lpp_prov, datatype, op, FI_FETCH_ATOMIC);
	if (ret != 0)
		return ret;

	comp_flag = lpp_cq_comp_flag(lpp_epp->transmit_bind_flags,
				     lpp_stxp->attr.op_flags);

	lpp_ep_op_lock(lpp_epp);

	tx_entry = lpp_tx_entry_get(lpp_stxp);
	if (tx_entry == NULL) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	typesize = ofi_datatype_size(datatype);
	tx_entry->msg_iov[0].iov_base = &tx_entry->atomic_hdr;
	tx_entry->msg_iov[0].iov_len = sizeof(tx_entry->atomic_hdr);
	if (op != FI_ATOMIC_READ) {
		tx_entry->msg_iov[1].iov_base = (void *)buf;
		tx_entry->msg_iov[1].iov_len = count * typesize;
		tx_entry->iov_count = 2;
	} else {
		tx_entry->iov_count = 1;
	}
	tx_entry->flags =
		FI_ATOMIC | comp_flag | (lpp_stxp->attr.op_flags & FI_INJECT);
	tx_entry->context = context;
	tx_entry->dst_addr.raw = dest_addr;
	/* Atomic types with reply data (fetch/compare) must always have a
	 * token for replies, so KLPP_MSG_TOKEN is passed as a required flag. */
	tx_entry->msg_hdr.flags = lpp_stx_msg_hdr_flags(0, KLPP_MSG_TOKEN, lpp_stxp);
	tx_entry->msg_hdr.token = lpp_tx_entry_to_token(lpp_stxp, tx_entry);

	tx_entry->atomic_hdr.mode = LPP_ATOMIC_FETCH;
	tx_entry->atomic_hdr.op = op;
	tx_entry->atomic_hdr.datatype = datatype;
	tx_entry->atomic_hdr.key = key;
	tx_entry->atomic_hdr.addr = addr;
	tx_entry->atomic_hdr.count = count;
	tx_entry->atomic_result.iov_base = result;
	tx_entry->atomic_result.iov_len = count * typesize;

	ret = lpp_atomic_common(lpp_epp, tx_entry);

unlock:
	lpp_ep_op_unlock(lpp_epp);
	return ret;
}

static ssize_t lpp_fi_fetch_atomicmsg(struct fid_ep *ep,
				      const struct fi_msg_atomic *msg,
				      struct fi_ioc *resultv,
				      void **result_desc, size_t result_count,
				      uint64_t flags)
{
	struct lpp_tx_entry *tx_entry;
	struct lpp_ep *lpp_epp;
	struct lpp_stx *lpp_stxp;
	uint64_t comp_flag;
	size_t typesize;
	size_t count;
	ssize_t ret;

	lpp_epp = container_of(ep, struct lpp_ep, ep_fid);
	lpp_stxp = lpp_epp->stx;

	if (flags & FI_TAGGED) {
		return -FI_EBADFLAGS;
	}

	ret = ofi_atomic_valid(&lpp_prov, msg->datatype, msg->op,
			       FI_FETCH_ATOMIC);
	if (ret != 0) {
		return ret;
	}
	ret = lpp_tx_verify_rma_iov_count(lpp_stxp, msg->iov_count);
	if (ret != 0) {
		return ret;
	}
	ret = lpp_tx_verify_rma_iov_count(lpp_stxp, msg->rma_iov_count);
	if (ret != 0) {
		return ret;
	}
	ret = lpp_tx_verify_rma_iov_count(lpp_stxp, result_count);
	if (ret != 0) {
		return ret;
	}
	ret = lpp_tx_verify_flags(lpp_stxp, flags);
	if (ret != 0) {
		return ret;
	}
	if (msg->op != FI_ATOMIC_READ) {
		if (msg->rma_iov[0].count < msg->msg_iov[0].count) {
			return -FI_EINVAL;
		}
		if (resultv[0].count < msg->msg_iov[0].count) {
			return -FI_EINVAL;
		}
	}
	if (resultv[0].count < msg->rma_iov[0].count) {
		return -FI_EINVAL;
	}

	comp_flag = lpp_cq_comp_flag(lpp_epp->transmit_bind_flags, flags);

	lpp_ep_op_lock(lpp_epp);

	tx_entry = lpp_tx_entry_get(lpp_stxp);
	if (tx_entry == NULL) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	typesize = ofi_datatype_size(msg->datatype);
	tx_entry->msg_iov[0].iov_base = &tx_entry->atomic_hdr;
	tx_entry->msg_iov[0].iov_len = sizeof(tx_entry->atomic_hdr);
	if (msg->op != FI_ATOMIC_READ) {
		tx_entry->msg_iov[1].iov_base = msg->msg_iov[0].addr;
		tx_entry->msg_iov[1].iov_len = msg->msg_iov[0].count * typesize;
		tx_entry->iov_count = 2;
		count = msg->msg_iov[0].count;
	} else {
		tx_entry->iov_count = 1;
		count = msg->rma_iov[0].count;
	}
	tx_entry->flags = FI_ATOMIC | comp_flag | (flags & FI_INJECT);
	tx_entry->context = msg->context;
	tx_entry->dst_addr.raw = msg->addr;
	/* Atomic types with reply data (fetch/compare) must always have a
	 * token for replies, so KLPP_MSG_TOKEN is passed as a required flag. */
	tx_entry->msg_hdr.flags =
		lpp_stx_msg_hdr_flags(flags, KLPP_MSG_TOKEN, lpp_stxp);
	tx_entry->msg_hdr.token = lpp_tx_entry_to_token(lpp_stxp, tx_entry);

	tx_entry->atomic_hdr.mode = LPP_ATOMIC_FETCH;
	tx_entry->atomic_hdr.op = msg->op;
	tx_entry->atomic_hdr.datatype = msg->datatype;
	tx_entry->atomic_hdr.key = msg->rma_iov[0].key;
	tx_entry->atomic_hdr.addr = msg->rma_iov[0].addr;
	tx_entry->atomic_hdr.count = count;
	tx_entry->atomic_result.iov_base = resultv[0].addr;
	tx_entry->atomic_result.iov_len = resultv[0].count * typesize;

	ret = lpp_atomic_common(lpp_epp, tx_entry);

unlock:
	lpp_ep_op_unlock(lpp_epp);
	return ret;
}

static ssize_t lpp_fi_compare_atomic(struct fid_ep *ep, const void *buf,
				     size_t count, void *desc,
				     const void *compare, void *compare_desc,
				     void *result, void *result_desc,
				     fi_addr_t dest_addr, uint64_t addr,
				     uint64_t key, enum fi_datatype datatype,
				     enum fi_op op, void *context)
{
	struct lpp_tx_entry *tx_entry;
	struct lpp_ep *lpp_epp;
	struct lpp_stx *lpp_stxp;
	uint64_t comp_flag;
	size_t typesize;
	ssize_t ret;

	lpp_epp = container_of(ep, struct lpp_ep, ep_fid);
	lpp_stxp = lpp_epp->stx;

	ret = ofi_atomic_valid(&lpp_prov, datatype, op, FI_COMPARE_ATOMIC);
	if (ret != 0) {
		return ret;
	}

	comp_flag = lpp_cq_comp_flag(lpp_epp->transmit_bind_flags,
				     lpp_stxp->attr.op_flags);

	lpp_ep_op_lock(lpp_epp);

	tx_entry = lpp_tx_entry_get(lpp_stxp);
	if (tx_entry == NULL) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	typesize = ofi_datatype_size(datatype);
	tx_entry->msg_iov[0].iov_base = &tx_entry->atomic_hdr;
	tx_entry->msg_iov[0].iov_len = sizeof(tx_entry->atomic_hdr);
	tx_entry->msg_iov[1].iov_base = (void *)buf;
	tx_entry->msg_iov[1].iov_len = count * typesize;
	tx_entry->msg_iov[2].iov_base = (void *)compare;
	tx_entry->msg_iov[2].iov_len = count * typesize;
	tx_entry->iov_count = 3;
	tx_entry->flags =
		FI_ATOMIC | comp_flag | (lpp_stxp->attr.op_flags & FI_INJECT);
	tx_entry->context = context;
	tx_entry->dst_addr.raw = dest_addr;
	/* Atomic types with reply data (fetch/compare) must always have a
	 * token for replies, so KLPP_MSG_TOKEN is passed as a required flag. */
	tx_entry->msg_hdr.flags =
		lpp_stx_msg_hdr_flags(0, KLPP_MSG_TOKEN, lpp_stxp);
	tx_entry->msg_hdr.token = lpp_tx_entry_to_token(lpp_stxp, tx_entry);

	tx_entry->atomic_hdr.mode = LPP_ATOMIC_CMP;
	tx_entry->atomic_hdr.op = op;
	tx_entry->atomic_hdr.datatype = datatype;
	tx_entry->atomic_hdr.key = key;
	tx_entry->atomic_hdr.addr = addr;
	tx_entry->atomic_hdr.count = count;
	tx_entry->atomic_result.iov_base = result;
	tx_entry->atomic_result.iov_len = count * typesize;

	ret = lpp_atomic_common(lpp_epp, tx_entry);

unlock:
	lpp_ep_op_unlock(lpp_epp);
	return ret;
}

static ssize_t
lpp_fi_compare_atomicmsg(struct fid_ep *ep, const struct fi_msg_atomic *msg,
			 const struct fi_ioc *comparev, void **compare_desc,
			 size_t compare_count, struct fi_ioc *resultv,
			 void **result_desc, size_t result_count,
			 uint64_t flags)
{
	struct lpp_tx_entry *tx_entry;
	struct lpp_ep *lpp_epp;
	struct lpp_stx *lpp_stxp;
	uint64_t comp_flag;
	size_t typesize;
	ssize_t ret;

	lpp_epp = container_of(ep, struct lpp_ep, ep_fid);
	lpp_stxp = lpp_epp->stx;

	if (flags & FI_TAGGED) {
		return -FI_EBADFLAGS;
	}

	ret = ofi_atomic_valid(&lpp_prov, msg->datatype, msg->op,
			       FI_COMPARE_ATOMIC);
	if (ret != 0)
		return ret;

	ret = lpp_tx_verify_rma_iov_count(lpp_stxp, msg->iov_count);
	if (ret != 0)
		return ret;

	ret = lpp_tx_verify_rma_iov_count(lpp_stxp, msg->rma_iov_count);
	if (ret != 0)
		return ret;

	ret = lpp_tx_verify_rma_iov_count(lpp_stxp, result_count);
	if (ret != 0)
		return ret;

	ret = lpp_tx_verify_rma_iov_count(lpp_stxp, compare_count);
	if (ret != 0)
		return ret;

	ret = lpp_tx_verify_flags(lpp_stxp, flags);
	if (ret != 0)
		return ret;

	if (msg->rma_iov[0].count < msg->msg_iov[0].count)
		return -FI_EINVAL;

	if (resultv[0].count < msg->msg_iov[0].count)
		return -FI_EINVAL;

	if (comparev[0].count != msg->msg_iov[0].count)
		return -FI_EINVAL;


	comp_flag = lpp_cq_comp_flag(lpp_epp->transmit_bind_flags, flags);

	lpp_ep_op_lock(lpp_epp);

	tx_entry = lpp_tx_entry_get(lpp_stxp);
	if (tx_entry == NULL) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	typesize = ofi_datatype_size(msg->datatype);
	tx_entry->msg_iov[0].iov_base = &tx_entry->atomic_hdr;
	tx_entry->msg_iov[0].iov_len = sizeof(tx_entry->atomic_hdr);
	tx_entry->msg_iov[1].iov_base = msg->msg_iov[0].addr;
	tx_entry->msg_iov[1].iov_len = msg->msg_iov[0].count * typesize;
	tx_entry->msg_iov[2].iov_base = comparev[0].addr;
	tx_entry->msg_iov[2].iov_len = comparev[0].count * typesize;
	tx_entry->iov_count = 3;
	tx_entry->flags = FI_ATOMIC | comp_flag | (flags & FI_INJECT);
	tx_entry->context = msg->context;
	tx_entry->dst_addr.raw = msg->addr;
	/* Atomic types with reply data (fetch/compare) must always have a
	 * token for replies, so KLPP_MSG_TOKEN is passed as a required flag. */
	tx_entry->msg_hdr.flags =
		lpp_stx_msg_hdr_flags(flags, KLPP_MSG_TOKEN, lpp_stxp);
	tx_entry->msg_hdr.token = lpp_tx_entry_to_token(lpp_stxp, tx_entry);

	tx_entry->atomic_hdr.mode = LPP_ATOMIC_CMP;
	tx_entry->atomic_hdr.op = msg->op;
	tx_entry->atomic_hdr.datatype = msg->datatype;
	tx_entry->atomic_hdr.key = msg->rma_iov[0].key;
	tx_entry->atomic_hdr.addr = msg->rma_iov[0].addr;
	tx_entry->atomic_hdr.count = msg->msg_iov[0].count;
	tx_entry->atomic_result.iov_base = resultv[0].addr;
	tx_entry->atomic_result.iov_len = resultv[0].count * typesize;

	ret = lpp_atomic_common(lpp_epp, tx_entry);

unlock:
	lpp_ep_op_unlock(lpp_epp);
	return ret;
}

static int lpp_fi_atomicvalid(struct fid_ep *ep, enum fi_datatype datatype,
			      enum fi_op op, size_t *count)
{
	struct lpp_ep *lpp_epp;
	struct fi_atomic_attr attr;
	int ret;

	lpp_epp = container_of(ep, struct lpp_ep, ep_fid);
	ret = lpp_fi_query_atomic(&lpp_epp->domain->util_domain.domain_fid,
				  datatype, op, &attr, 0);
	if (ret != 0)
		return ret;

	*count = attr.count;
	return 0;
}

static int lpp_fi_fetch_atomicvalid(struct fid_ep *ep,
				    enum fi_datatype datatype, enum fi_op op,
				    size_t *count)
{
	struct lpp_ep *lpp_epp;
	struct fi_atomic_attr attr;
	int ret;

	lpp_epp = container_of(ep, struct lpp_ep, ep_fid);
	ret = lpp_fi_query_atomic(&lpp_epp->domain->util_domain.domain_fid,
			datatype, op, &attr, FI_FETCH_ATOMIC);
	if (ret != 0)
		return ret;

	*count = attr.count;
	return 0;
}

static int lpp_fi_compare_atomicvalid(struct fid_ep *ep,
				      enum fi_datatype datatype, enum fi_op op,
				      size_t *count)
{
	return -FI_EOPNOTSUPP;
	struct lpp_ep *lpp_epp;
	struct fi_atomic_attr attr;
	int ret;

	lpp_epp = container_of(ep, struct lpp_ep, ep_fid);
	ret = lpp_fi_query_atomic(&lpp_epp->domain->util_domain.domain_fid,
				  datatype, op, &attr, FI_COMPARE_ATOMIC);
	if (ret != 0)
		return ret;

	*count = attr.count;
	return 0;
}

int lpp_fi_query_atomic(struct fid_domain *domain, enum fi_datatype datatype,
			enum fi_op op, struct fi_atomic_attr *attr,
			uint64_t flags)
{
	size_t total_size;
	int ret;

	if (flags & FI_TAGGED) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
			"tagged atomic ops not supported\n");
		return -FI_EINVAL;
	}

	ret = ofi_atomic_valid(&lpp_prov, datatype, op, flags);
	if (ret != 0) {
		return ret;
	}

	/* TODO: If rdzv atomics are implemented, this size should be limit by
	 * the max RMA size instead of the max eager size. */
	if (flags & FI_COMPARE_ATOMIC)
		total_size = (lpp_max_eager_size - sizeof(struct lpp_atomic_hdr)) / 2;
	else
		total_size = lpp_max_eager_size - sizeof(struct lpp_atomic_hdr);

	attr->size = ofi_datatype_size(datatype);
	attr->count = (total_size / attr->size);

	return 0;
}

const struct fi_ops_atomic lpp_ep_atomic = {
	.size = sizeof(struct fi_ops_atomic),
	.write = lpp_fi_atomic,
	.writev = fi_no_atomic_writev,
	.writemsg = lpp_fi_atomicmsg,
	.inject = fi_no_atomic_inject,
	.readwrite = lpp_fi_fetch_atomic,
	.readwritev = fi_no_atomic_readwritev,
	.readwritemsg = lpp_fi_fetch_atomicmsg,
	.compwrite = lpp_fi_compare_atomic,
	.compwritev = fi_no_atomic_compwritev,
	.compwritemsg = lpp_fi_compare_atomicmsg,
	.writevalid = lpp_fi_atomicvalid,
	.readwritevalid = lpp_fi_fetch_atomicvalid,
	.compwritevalid = lpp_fi_compare_atomicvalid,
};
