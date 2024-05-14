/*
 * Copyright (c) 2013-2021 Intel Corporation. All rights reserved
 * (C) Copyright 2021 Amazon.com, Inc. or its affiliates.
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
#include <string.h>
#include <sys/uio.h>

#include "ofi_iov.h"
#include "smr.h"

static ssize_t smr_recvmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
			   uint64_t flags)
{
	struct smr_ep *ep;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	return util_srx_generic_recv(ep->srx, msg->msg_iov, msg->desc,
				     msg->iov_count, msg->addr, msg->context,
				     flags | ep->util_ep.rx_msg_flags);
}

static ssize_t smr_recvv(struct fid_ep *ep_fid, const struct iovec *iov,
			 void **desc, size_t count, fi_addr_t src_addr,
			 void *context)
{
	struct smr_ep *ep;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	return util_srx_generic_recv(ep->srx, iov, desc, count, src_addr,
				     context, smr_ep_rx_flags(ep));
}

static ssize_t smr_recv(struct fid_ep *ep_fid, void *buf, size_t len,
			void *desc, fi_addr_t src_addr, void *context)
{
	struct iovec iov;
	struct smr_ep *ep;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	iov.iov_base = buf;
	iov.iov_len = len;

	return util_srx_generic_recv(ep->srx, &iov, &desc, 1, src_addr, context,
				     smr_ep_rx_flags(ep));
}

static ssize_t smr_generic_sendmsg(struct smr_ep *ep, const struct iovec *iov,
				   void **desc, size_t iov_count, fi_addr_t addr,
				   uint64_t tag, uint64_t data, void *context,
				   uint32_t op, uint64_t op_flags)
{
	struct smr_region *peer_smr;
	int64_t id, peer_id;
	ssize_t ret = 0;
	size_t total_len;
	int proto;
	struct smr_cmd_entry *ce;
	int64_t pos;

	assert(iov_count <= SMR_IOV_LIMIT);

	id = smr_verify_peer(ep, addr);
	if (id < 0)
		return -FI_EAGAIN;

	peer_id = smr_peer_data(ep->region)[id].addr.id;
	peer_smr = smr_peer_region(ep->region, id);

	if (smr_peer_data(ep->region)[id].sar_status)
		return -FI_EAGAIN;

	ret = smr_cmd_queue_next(smr_cmd_queue(peer_smr), &ce, &pos);
	if (ret == -FI_ENOENT)
		return -FI_EAGAIN;

	ofi_genlock_lock(&ep->util_ep.lock);

	total_len = ofi_total_iov_len(iov, iov_count);
	assert(!(op_flags & FI_INJECT) || total_len <= SMR_INJECT_SIZE);

	proto = smr_select_proto(desc, iov_count, smr_vma_enabled(ep, peer_smr),
	                         op, total_len, op_flags);

	ret = smr_proto_ops[proto](ep, peer_smr, id, peer_id, op, tag, data, op_flags,
				   (struct ofi_mr **)desc, iov, iov_count, total_len,
				   context, &ce->cmd);
	if (ret) {
		smr_cmd_queue_discard(ce, pos);
		goto unlock;
	}
	smr_cmd_queue_commit(ce, pos);

	if (proto != smr_src_inline && proto != smr_src_inject)
		goto unlock;

	ret = smr_complete_tx(ep, context, op, op_flags);
	if (ret) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"unable to process tx completion\n");
		goto unlock;
	}

unlock:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t smr_send(struct fid_ep *ep_fid, const void *buf, size_t len,
		void *desc, fi_addr_t dest_addr, void *context)
{
	struct smr_ep *ep;
	struct iovec msg_iov;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	msg_iov.iov_base = (void *) buf;
	msg_iov.iov_len = len;

	return smr_generic_sendmsg(ep, &msg_iov, &desc, 1, dest_addr, 0,
				   0, context, ofi_op_msg, smr_ep_tx_flags(ep));
}

static ssize_t smr_sendv(struct fid_ep *ep_fid, const struct iovec *iov,
		void **desc, size_t count, fi_addr_t dest_addr, void *context)
{
	struct smr_ep *ep;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	return smr_generic_sendmsg(ep, iov, desc, count, dest_addr, 0,
				   0, context, ofi_op_msg, smr_ep_tx_flags(ep));
}

static ssize_t smr_sendmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
			   uint64_t flags)
{
	struct smr_ep *ep;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	return smr_generic_sendmsg(ep, msg->msg_iov, msg->desc, msg->iov_count,
				   msg->addr, 0, msg->data, msg->context,
				   ofi_op_msg, flags | ep->util_ep.tx_msg_flags);
}

static ssize_t smr_generic_inject(struct fid_ep *ep_fid, const void *buf,
				  size_t len, fi_addr_t dest_addr, uint64_t tag,
				  uint64_t data, uint32_t op, uint64_t op_flags)
{
	struct smr_ep *ep;
	struct smr_region *peer_smr;
	int64_t id, peer_id;
	ssize_t ret = 0;
	struct iovec msg_iov;
	int proto;
	struct smr_cmd_entry *ce;
	int64_t pos;

	assert(len <= SMR_INJECT_SIZE);

	msg_iov.iov_base = (void *) buf;
	msg_iov.iov_len = len;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	id = smr_verify_peer(ep, dest_addr);
	if (id < 0)
		return -FI_EAGAIN;

	peer_id = smr_peer_data(ep->region)[id].addr.id;
	peer_smr = smr_peer_region(ep->region, id);

	if (smr_peer_data(ep->region)[id].sar_status)
		return -FI_EAGAIN;

	ret = smr_cmd_queue_next(smr_cmd_queue(peer_smr), &ce, &pos);
	if (ret == -FI_ENOENT)
		return -FI_EAGAIN;

	proto = len <= SMR_MSG_DATA_LEN ? smr_src_inline : smr_src_inject;
	ret = smr_proto_ops[proto](ep, peer_smr, id, peer_id, op, tag, data,
			op_flags, NULL, &msg_iov, 1, len, NULL, &ce->cmd);
	if (ret) {
		smr_cmd_queue_discard(ce, pos);
		return -FI_EAGAIN;
	}
	smr_cmd_queue_commit(ce, pos);
	ofi_ep_peer_tx_cntr_inc(&ep->util_ep, op);

	return FI_SUCCESS;
}

static ssize_t smr_inject(struct fid_ep *ep_fid, const void *buf, size_t len,
			  fi_addr_t dest_addr)
{
	return smr_generic_inject(ep_fid, buf, len, dest_addr, 0, 0,
				  ofi_op_msg, 0);
}

static ssize_t smr_senddata(struct fid_ep *ep_fid, const void *buf, size_t len,
			    void *desc, uint64_t data, fi_addr_t dest_addr,
			    void *context)
{
	struct smr_ep *ep;
	struct iovec iov;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	iov.iov_base = (void *) buf;
	iov.iov_len = len;

	return smr_generic_sendmsg(ep, &iov, &desc, 1, dest_addr, 0, data,
				   context, ofi_op_msg,
				   FI_REMOTE_CQ_DATA | smr_ep_tx_flags(ep));
}

static ssize_t smr_injectdata(struct fid_ep *ep_fid, const void *buf,
			      size_t len, uint64_t data, fi_addr_t dest_addr)
{
	return smr_generic_inject(ep_fid, buf, len, dest_addr, 0, data,
				  ofi_op_msg, FI_REMOTE_CQ_DATA);
}

struct fi_ops_msg smr_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = smr_recv,
	.recvv = smr_recvv,
	.recvmsg = smr_recvmsg,
	.send = smr_send,
	.sendv = smr_sendv,
	.sendmsg = smr_sendmsg,
	.inject = smr_inject,
	.senddata = smr_senddata,
	.injectdata = smr_injectdata,
};

struct fi_ops_msg smr_no_recv_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = fi_no_msg_recv,
	.recvv = fi_no_msg_recvv,
	.recvmsg = fi_no_msg_recvmsg,
	.send = smr_send,
	.sendv = smr_sendv,
	.sendmsg = smr_sendmsg,
	.inject = smr_inject,
	.senddata = smr_senddata,
	.injectdata = smr_injectdata,
};

static ssize_t smr_trecv(struct fid_ep *ep_fid, void *buf, size_t len,
			 void *desc, fi_addr_t src_addr, uint64_t tag,
			 uint64_t ignore, void *context)
{
	struct iovec iov;
	struct smr_ep *ep;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	iov.iov_base = buf;
	iov.iov_len = len;

	return util_srx_generic_trecv(ep->srx, &iov, &desc, 1, src_addr, context,
				     tag, ignore, smr_ep_rx_flags(ep));
}

static ssize_t smr_trecvv(struct fid_ep *ep_fid, const struct iovec *iov,
			  void **desc, size_t count, fi_addr_t src_addr,
			  uint64_t tag, uint64_t ignore, void *context)
{
	struct smr_ep *ep;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	return util_srx_generic_trecv(ep->srx, iov, desc, count, src_addr,
				     context, tag, ignore, smr_ep_rx_flags(ep));
}

static ssize_t smr_trecvmsg(struct fid_ep *ep_fid,
			    const struct fi_msg_tagged *msg, uint64_t flags)
{
	struct smr_ep *ep;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	return util_srx_generic_trecv(ep->srx, msg->msg_iov, msg->desc,
				     msg->iov_count, msg->addr, msg->context,
				     msg->tag, msg->ignore,
				     flags | ep->util_ep.rx_msg_flags);
}

static ssize_t smr_tsend(struct fid_ep *ep_fid, const void *buf, size_t len,
	void *desc, fi_addr_t dest_addr, uint64_t tag, void *context)
{
	struct smr_ep *ep;
	struct iovec msg_iov;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	msg_iov.iov_base = (void *) buf;
	msg_iov.iov_len = len;

	return smr_generic_sendmsg(ep, &msg_iov, &desc, 1, dest_addr, tag,
				   0, context, ofi_op_tagged,
				   smr_ep_tx_flags(ep));
}

static ssize_t smr_tsendv(struct fid_ep *ep_fid, const struct iovec *iov,
	void **desc, size_t count, fi_addr_t dest_addr, uint64_t tag,
	void *context)
{
	struct smr_ep *ep;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	return smr_generic_sendmsg(ep, iov, desc, count, dest_addr, tag,
				   0, context, ofi_op_tagged,
				   smr_ep_tx_flags(ep));
}

static ssize_t smr_tsendmsg(struct fid_ep *ep_fid,
			    const struct fi_msg_tagged *msg, uint64_t flags)
{
	struct smr_ep *ep;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	return smr_generic_sendmsg(ep, msg->msg_iov, msg->desc, msg->iov_count,
				   msg->addr, msg->tag, msg->data, msg->context,
				   ofi_op_tagged,
				   flags | ep->util_ep.tx_msg_flags);
}

static ssize_t smr_tinject(struct fid_ep *ep_fid, const void *buf, size_t len,
			   fi_addr_t dest_addr, uint64_t tag)
{
	return smr_generic_inject(ep_fid, buf, len, dest_addr, tag, 0,
				  ofi_op_tagged, 0);
}

static ssize_t smr_tsenddata(struct fid_ep *ep_fid, const void *buf, size_t len,
			     void *desc, uint64_t data, fi_addr_t dest_addr,
			     uint64_t tag, void *context)
{
	struct smr_ep *ep;
	struct iovec iov;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	iov.iov_base = (void *) buf;
	iov.iov_len = len;

	return smr_generic_sendmsg(ep, &iov, &desc, 1, dest_addr, tag, data,
				   context, ofi_op_tagged,
				   FI_REMOTE_CQ_DATA | smr_ep_tx_flags(ep));
}

static ssize_t smr_tinjectdata(struct fid_ep *ep_fid, const void *buf,
			       size_t len, uint64_t data, fi_addr_t dest_addr,
			       uint64_t tag)
{
	return smr_generic_inject(ep_fid, buf, len, dest_addr, tag, data,
				  ofi_op_tagged, FI_REMOTE_CQ_DATA);
}

struct fi_ops_tagged smr_tag_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = smr_trecv,
	.recvv = smr_trecvv,
	.recvmsg = smr_trecvmsg,
	.send = smr_tsend,
	.sendv = smr_tsendv,
	.sendmsg = smr_tsendmsg,
	.inject = smr_tinject,
	.senddata = smr_tsenddata,
	.injectdata = smr_tinjectdata,
};

struct fi_ops_tagged smr_no_recv_tag_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = fi_no_tagged_recv,
	.recvv = fi_no_tagged_recvv,
	.recvmsg = fi_no_tagged_recvmsg,
	.send = smr_tsend,
	.sendv = smr_tsendv,
	.sendmsg = smr_tsendmsg,
	.inject = smr_tinject,
	.senddata = smr_tsenddata,
	.injectdata = smr_tinjectdata,
};
