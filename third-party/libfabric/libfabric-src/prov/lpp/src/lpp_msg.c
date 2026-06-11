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

ssize_t lpp_fi_recvmsg(struct fid_ep *ep, const struct fi_msg *msg,
				 uint64_t flags)
{
	struct lpp_ep *lpp_epp;
	struct lpp_rx *lpp_rxp;
	struct lpp_rx_entry *rx_entry;
	uint64_t comp_flag;
	ssize_t ret;

	lpp_epp = container_of(ep, struct lpp_ep, ep_fid);
	lpp_rxp = lpp_epp->rx;

	ret = lpp_rx_verify_iov_count(lpp_rxp, msg->iov_count);
	if (ret != 0)
		return ret;

	ret = lpp_rx_verify_flags(lpp_rxp, flags);
	if (ret != 0)
		return ret;

	comp_flag = lpp_cq_comp_flag(lpp_epp->recv_bind_flags, flags);

	lpp_ep_op_lock(lpp_epp);

	rx_entry = lpp_rx_entry_get(lpp_rxp);
	if (rx_entry == NULL) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	rx_entry->flags = FI_MSG | FI_RECV | comp_flag | flags;
	memcpy(rx_entry->msg_iov, msg->msg_iov,
	    sizeof(rx_entry->msg_iov[0]) * msg->iov_count);
	rx_entry->iov_count = msg->iov_count;
	rx_entry->addr = msg->addr;
	rx_entry->context = msg->context;
	//rx_entry->data = msg->data;

	ret = lpp_rx_common(lpp_epp, rx_entry);

unlock:
	lpp_ep_op_unlock(lpp_epp);
	return ret;
}

ssize_t lpp_fi_recv(struct fid_ep *ep, void *buf, size_t len,
			      void *desc, fi_addr_t src_addr, void *context)
{
	struct lpp_ep *lpp_epp;
	struct lpp_rx *lpp_rxp;
	struct lpp_rx_entry *rx_entry;
	uint64_t comp_flag;
	int ret;

	lpp_epp = container_of(ep, struct lpp_ep, ep_fid);
	lpp_rxp = lpp_epp->rx;

	comp_flag = lpp_cq_comp_flag(lpp_epp->recv_bind_flags,
	    lpp_rxp->attr.op_flags);

	lpp_ep_op_lock(lpp_epp);

	rx_entry = lpp_rx_entry_get(lpp_rxp);
	if (rx_entry == NULL) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	rx_entry->flags = FI_MSG | FI_RECV | comp_flag | lpp_rxp->attr.op_flags;
	rx_entry->msg_iov[0].iov_base = buf;
	rx_entry->msg_iov[0].iov_len = len;
	rx_entry->iov_count = 1;
	rx_entry->addr = src_addr;
	rx_entry->context = context;

	ret = lpp_rx_common(lpp_epp, rx_entry);

unlock:
	lpp_ep_op_unlock(lpp_epp);
	return ret;
}

ssize_t lpp_fi_sendmsg(struct fid_ep *ep, const struct fi_msg *msg,
				 uint64_t flags)
{
	struct lpp_tx_entry *tx_entry;
	struct lpp_ep *lpp_epp;
	struct lpp_stx *lpp_stxp;
	uint64_t comp_flag;
	ssize_t ret;

	lpp_epp = container_of(ep, struct lpp_ep, ep_fid);
	lpp_stxp = lpp_epp->stx;

	ret = lpp_tx_verify_iov_count(lpp_stxp, msg->iov_count);
	if (ret != 0)
		return ret;

	ret = lpp_tx_verify_flags(lpp_stxp, flags);
	if (ret != 0)
		return ret;

	comp_flag = lpp_cq_comp_flag(lpp_epp->transmit_bind_flags, flags);

	lpp_ep_op_lock(lpp_epp);

	tx_entry = lpp_tx_entry_get(lpp_stxp);
	if (tx_entry == NULL) {
		ret = -FI_EAGAIN;
		goto unlock;
	}
	memcpy(&tx_entry->msg_iov, msg->msg_iov,
	       sizeof(tx_entry->msg_iov[0]) * msg->iov_count);
	tx_entry->iov_count = msg->iov_count;
	tx_entry->flags = FI_MSG | FI_SEND | comp_flag | flags;
	tx_entry->context = msg->context;
	tx_entry->dst_addr.raw = msg->addr;
	tx_entry->msg_hdr.flags = lpp_stx_msg_hdr_flags(flags, 0, lpp_stxp);
	tx_entry->msg_hdr.token = lpp_tx_entry_to_token(lpp_stxp, tx_entry);

	ret = lpp_send_common(lpp_epp, tx_entry);

unlock:
	lpp_ep_op_unlock(lpp_epp);
	return ret;
}

ssize_t lpp_fi_sendv(struct fid_ep *ep, const struct iovec *iov,
			       void **desc, size_t count, fi_addr_t dest_addr,
			       void *context)
{
	struct lpp_tx_entry *tx_entry;
	struct lpp_ep *lpp_epp;
	struct lpp_stx *lpp_stxp;
	uint64_t comp_flag;
	ssize_t ret;

	lpp_epp = container_of(ep, struct lpp_ep, ep_fid);
	lpp_stxp = lpp_epp->stx;

	ret = lpp_tx_verify_iov_count(lpp_stxp, count);
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
	memcpy(&tx_entry->msg_iov, iov, sizeof(tx_entry->msg_iov[0]) * count);
	tx_entry->iov_count = count;
	tx_entry->flags = FI_MSG | FI_SEND | comp_flag |
			  (lpp_stxp->attr.op_flags & FI_INJECT);
	tx_entry->context = context;
	tx_entry->dst_addr.raw = dest_addr;
	tx_entry->msg_hdr.flags = lpp_stx_msg_hdr_flags(0, 0, lpp_stxp);
	tx_entry->msg_hdr.token = lpp_tx_entry_to_token(lpp_stxp, tx_entry);

	ret = lpp_send_common(lpp_epp, tx_entry);

unlock:
	lpp_ep_op_unlock(lpp_epp);
	return ret;
}

ssize_t lpp_fi_send(struct fid_ep *ep, const void *buf, size_t len,
			      void *desc, fi_addr_t dest_addr, void *context)
{
	struct lpp_tx_entry *tx_entry;
	struct lpp_ep *lpp_epp;
	struct lpp_stx *lpp_stxp;
	uint64_t comp_flag;
	ssize_t ret;

	lpp_epp = container_of(ep, struct lpp_ep, ep_fid);
	lpp_stxp = lpp_epp->stx;

	comp_flag = lpp_cq_comp_flag(lpp_epp->transmit_bind_flags,
				     lpp_stxp->attr.op_flags);

	lpp_ep_op_lock(lpp_epp);

	tx_entry = lpp_tx_entry_get(lpp_stxp);
	if (tx_entry == NULL) {
		ret = -FI_EAGAIN;
		goto unlock;
	}
	tx_entry->msg_iov[0].iov_base = (void*)buf;
	tx_entry->msg_iov[0].iov_len = len;
	tx_entry->iov_count = 1;
	tx_entry->flags = FI_MSG | FI_SEND | comp_flag |
			  (lpp_stxp->attr.op_flags & FI_INJECT);
	tx_entry->context = context;
	tx_entry->dst_addr.raw = dest_addr;
	tx_entry->msg_hdr.flags = lpp_stx_msg_hdr_flags(0, 0, lpp_stxp);
	tx_entry->msg_hdr.token = lpp_tx_entry_to_token(lpp_stxp, tx_entry);

	ret = lpp_send_common(lpp_epp, tx_entry);

unlock:
	lpp_ep_op_unlock(lpp_epp);
	return ret;
}

ssize_t lpp_fi_inject(struct fid_ep *ep, const void *buf, size_t len,
				fi_addr_t dest_addr)
{
	struct lpp_tx_entry *tx_entry;
	struct lpp_ep *lpp_epp;
	struct lpp_stx *lpp_stxp;
	ssize_t ret;

	lpp_epp = container_of(ep, struct lpp_ep, ep_fid);
	lpp_stxp = lpp_epp->stx;

	lpp_ep_op_lock(lpp_epp);

	tx_entry = lpp_tx_entry_get(lpp_stxp);
	if (tx_entry == NULL) {
		ret = -FI_EAGAIN;
		goto unlock;
	}
	tx_entry->msg_iov[0].iov_base = (void*)buf;
	tx_entry->msg_iov[0].iov_len = len;
	tx_entry->iov_count = 1;
	tx_entry->flags = FI_MSG | FI_SEND | FI_INJECT;
	tx_entry->context = NULL;
	tx_entry->dst_addr.raw = dest_addr;
	tx_entry->msg_hdr.flags = lpp_stx_msg_hdr_flags(0, 0, lpp_stxp);
	tx_entry->msg_hdr.token = lpp_tx_entry_to_token(lpp_stxp, tx_entry);

	ret = lpp_send_common(lpp_epp, tx_entry);

unlock:
	lpp_ep_op_unlock(lpp_epp);
	return ret;
}
