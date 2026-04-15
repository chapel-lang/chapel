/*
 * Copyright (c) Intel Corporation. All rights reserved
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

#include "smr.h"

static void smr_add_rma_cmd(struct smr_region *peer_smr,
			    const struct fi_rma_iov *rma_iov, size_t iov_count,
			    struct smr_cmd *cmd)
{
	cmd->rma.rma_count = iov_count;
	memcpy(cmd->rma.rma_iov, rma_iov, sizeof(*rma_iov) * iov_count);
}

static void smr_format_rma_resp(struct smr_cmd *cmd, int64_t peer_id,
				const struct fi_rma_iov *rma_iov, size_t count,
				size_t total_len, uint32_t op,
				uint64_t op_flags)
{
	smr_generic_format(cmd, 0, peer_id, op, 0, 0, op_flags);
	cmd->hdr.size = total_len;
}

static ssize_t smr_rma_fast(struct smr_ep *ep, struct smr_region *peer_smr,
			    const struct iovec *iov, size_t iov_count,
			    const struct fi_rma_iov *rma_iov, size_t rma_count,
			    void **desc, int rx_id, int tx_id, void *context,
			    uint32_t op, uint64_t op_flags)
{
	struct iovec vma_iovec[SMR_IOV_LIMIT], rma_iovec[SMR_IOV_LIMIT];
	struct ofi_xpmem_client *xpmem;
	struct smr_cmd_entry *ce;
	size_t total_len;
	int ret, i;
	int64_t pos;

	ret = smr_cmd_queue_next(smr_cmd_queue(peer_smr), &ce, &pos);
	if (ret == -FI_ENOENT)
		return -FI_EAGAIN;

	memcpy(vma_iovec, iov, sizeof(*iov) * iov_count);
	for (i = 0; i < rma_count; i++) {
		rma_iovec[i].iov_base = (void *)(uintptr_t) rma_iov[i].addr;
		rma_iovec[i].iov_len = rma_iov[i].len;
	}

	total_len = ofi_total_iov_len(iov, iov_count);

	xpmem = &smr_peer_data(ep->region)[tx_id].xpmem;

	ret = ofi_shm_p2p_copy(ep->p2p_type, vma_iovec, iov_count,
			       rma_iovec, rma_count, total_len, peer_smr->pid,
			       op == ofi_op_write, xpmem);

	if (ret) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL, "error doing fast RMA\n");
		ret = smr_write_err_comp(ep->util_ep.rx_cq, NULL, op_flags, 0,
					 ret);
		smr_cmd_queue_discard(ce, pos);
		return -FI_EAGAIN;
	}

	smr_format_rma_resp(&ce->cmd, rx_id, rma_iov, rma_count, total_len,
			    (op == ofi_op_write) ? ofi_op_write_async :
			    ofi_op_read_async, op_flags);

	smr_cmd_queue_commit(ce, pos);

	ret = smr_complete_tx(ep, context, op, op_flags);
	if (ret) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"unable to process tx completion\n");
	}

	return FI_SUCCESS;
}

static inline bool smr_do_fast_rma(struct smr_ep *ep, uint64_t op_flags,
				   size_t rma_count, size_t total_len,
				   struct smr_region *peer_smr)
{
	struct smr_domain *domain;

	domain = container_of(ep->util_ep.domain, struct smr_domain,
			      util_domain);

	return domain->fast_rma && !(op_flags &
		    (FI_REMOTE_CQ_DATA | FI_DELIVERY_COMPLETE)) &&
		     rma_count == 1 && smr_vma_enabled(ep, peer_smr) &&
		     total_len > SMR_INJECT_SIZE;

}

static ssize_t smr_generic_rma(
		struct smr_ep *ep, const struct iovec *iov,
		size_t iov_count, const struct fi_rma_iov *rma_iov,
		size_t rma_count, void **desc, fi_addr_t addr, void *context,
		uint32_t op, uint64_t data, uint64_t op_flags)
{
	struct smr_region *peer_smr;
	int64_t tx_id, rx_id;
	int proto = smr_proto_inline;
	ssize_t ret = -FI_EAGAIN;
	size_t total_len;
	struct smr_cmd_entry *ce;
	struct smr_cmd *cmd;
	int64_t pos;

	assert(iov_count <= SMR_IOV_LIMIT);
	assert(rma_count <= SMR_IOV_LIMIT);
	assert(ofi_total_iov_len(iov, iov_count) ==
	       ofi_total_rma_iov_len(rma_iov, rma_count));

	tx_id = smr_verify_peer(ep, addr);
	if (tx_id < 0)
		return -FI_EAGAIN;

	rx_id = smr_peer_data(ep->region)[tx_id].id;
	peer_smr = smr_peer_region(ep, tx_id);

	ofi_genlock_lock(&ep->util_ep.lock);
	if (smr_peer_data(ep->region)[tx_id].sar_status)
		goto unlock;

	total_len = ofi_total_iov_len(iov, iov_count);
	if (smr_do_fast_rma(ep, op_flags, rma_count, total_len, peer_smr)) {
		ret = smr_rma_fast(ep, peer_smr, iov, iov_count, rma_iov,
				   rma_count, desc, rx_id, tx_id, context, op,
				   op_flags);
		goto unlock;
	}

	ret = smr_cmd_queue_next(smr_cmd_queue(peer_smr), &ce, &pos);
	if (ret == -FI_ENOENT) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	assert(!(op_flags & FI_INJECT) || total_len <= SMR_INJECT_SIZE);

	proto = smr_select_proto(desc, iov_count, smr_vma_enabled(ep, peer_smr),
	                         smr_ipc_valid(ep, peer_smr, tx_id, rx_id), op,
				 total_len, op_flags);
	if (proto != smr_proto_inline) {
		if (smr_freestack_isempty(smr_cmd_stack(ep->region))) {
			smr_cmd_queue_discard(ce, pos);
			ret = -FI_EAGAIN;
			goto unlock;
		}
		cmd = smr_freestack_pop(smr_cmd_stack(ep->region));
		assert(cmd);
		ce->ptr = smr_local_to_peer(ep, peer_smr, tx_id, rx_id,
					    (uintptr_t) cmd);
	} else {
		cmd = &ce->cmd;
	}
	ret = smr_send_ops[proto](ep, peer_smr, tx_id, rx_id, op, 0, data,
				  op_flags, (struct ofi_mr **)desc, iov,
				  iov_count, total_len, context, cmd);
	if (ret) {
		if (proto != smr_proto_inline)
			smr_freestack_push(smr_cmd_stack(ep->region), cmd);
		smr_cmd_queue_discard(ce, pos);
		goto unlock;
	}

	smr_add_rma_cmd(peer_smr, rma_iov, rma_count, cmd);
	smr_cmd_queue_commit(ce, pos);

	if (proto != smr_proto_inline || op == ofi_op_read_req)
		goto unlock;

	ret = smr_complete_tx(ep, context, op, op_flags);
	if (ret) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"unable to process tx completion\n");
	}

unlock:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t smr_read(struct fid_ep *ep_fid, void *buf, size_t len,
			void *desc, fi_addr_t src_addr, uint64_t addr,
			uint64_t key, void *context)
{
	struct smr_ep *ep;
	struct iovec msg_iov;
	struct fi_rma_iov rma_iov;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	msg_iov.iov_base = (void *) buf;
	msg_iov.iov_len = len;
	rma_iov.addr = addr;
	rma_iov.len = len;
	rma_iov.key = key;

	return smr_generic_rma(ep, &msg_iov, 1, &rma_iov, 1, &desc,
			       src_addr, context, ofi_op_read_req, 0,
			       smr_ep_tx_flags(ep));
}

static ssize_t smr_readv(struct fid_ep *ep_fid, const struct iovec *iov,
			 void **desc, size_t count, fi_addr_t src_addr,
			 uint64_t addr, uint64_t key, void *context)
{
	struct smr_ep *ep;
	struct fi_rma_iov rma_iov;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	rma_iov.addr = addr;
	rma_iov.len  = ofi_total_iov_len(iov, count);
	rma_iov.key = key;

	return smr_generic_rma(ep, iov, count, &rma_iov, 1, desc,
			       src_addr, context, ofi_op_read_req, 0,
			       smr_ep_tx_flags(ep));
}

static ssize_t smr_readmsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
			   uint64_t flags)
{
	struct smr_ep *ep;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	return smr_generic_rma(ep, msg->msg_iov, msg->iov_count,
			       msg->rma_iov, msg->rma_iov_count,
			       msg->desc, msg->addr, msg->context,
			       ofi_op_read_req, 0,
			       flags | ep->util_ep.tx_msg_flags);
}

static ssize_t smr_write(struct fid_ep *ep_fid, const void *buf, size_t len,
			 void *desc, fi_addr_t dest_addr, uint64_t addr,
			 uint64_t key, void *context)
{
	struct smr_ep *ep;
	struct iovec msg_iov;
	struct fi_rma_iov rma_iov;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	msg_iov.iov_base = (void *) buf;
	msg_iov.iov_len = len;
	rma_iov.addr = addr;
	rma_iov.len = len;
	rma_iov.key = key;

	return smr_generic_rma(ep, &msg_iov, 1, &rma_iov, 1, &desc,
			       dest_addr, context, ofi_op_write, 0,
			       smr_ep_tx_flags(ep));
}

static ssize_t smr_writev(struct fid_ep *ep_fid, const struct iovec *iov,
			  void **desc, size_t count, fi_addr_t dest_addr,
			  uint64_t addr, uint64_t key, void *context)
{
	struct smr_ep *ep;
	struct fi_rma_iov rma_iov;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	rma_iov.addr = addr;
	rma_iov.len  = ofi_total_iov_len(iov, count);
	rma_iov.key = key;

	return smr_generic_rma(ep, iov, count, &rma_iov, 1, desc,
			       dest_addr, context, ofi_op_write, 0,
			       smr_ep_tx_flags(ep));
}


static ssize_t smr_writemsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
			    uint64_t flags)
{
	struct smr_ep *ep;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	return smr_generic_rma(ep, msg->msg_iov, msg->iov_count,
			       msg->rma_iov, msg->rma_iov_count,
			       msg->desc, msg->addr, msg->context,
			       ofi_op_write, msg->data,
			       flags | ep->util_ep.tx_msg_flags);
}

static ssize_t smr_generic_rma_inject(
		struct fid_ep *ep_fid, const void *buf, size_t len,
		fi_addr_t dest_addr, uint64_t addr, uint64_t key, uint64_t data,
		uint64_t flags)
{
	struct smr_ep *ep;
	struct smr_region *peer_smr;
	struct iovec iov;
	struct fi_rma_iov rma_iov;
	int64_t tx_id, rx_id;
	int proto = smr_proto_inline;
	ssize_t ret = -FI_EAGAIN;
	struct smr_cmd *cmd;
	struct smr_cmd_entry *ce;
	int64_t pos;

	assert(len <= SMR_INJECT_SIZE);
	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	tx_id = smr_verify_peer(ep, dest_addr);
	if (tx_id < 0)
		return -FI_EAGAIN;

	rx_id = smr_peer_data(ep->region)[tx_id].id;
	peer_smr = smr_peer_region(ep, tx_id);

	ofi_genlock_lock(&ep->util_ep.lock);
	if (smr_peer_data(ep->region)[tx_id].sar_status)
		goto unlock;

	iov.iov_base = (void *) buf;
	iov.iov_len = len;
	rma_iov.addr = addr;
	rma_iov.len = len;
	rma_iov.key = key;

	ret = smr_cmd_queue_next(smr_cmd_queue(peer_smr), &ce, &pos);
	if (ret == -FI_ENOENT) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	if (len <= SMR_MSG_DATA_LEN) {
		proto = smr_proto_inline;
		cmd = &ce->cmd;
	} else {
		proto = smr_proto_inject;
		if (smr_freestack_isempty(smr_cmd_stack(ep->region))) {
			smr_cmd_queue_discard(ce, pos);
			ret = -FI_EAGAIN;
			goto unlock;
		}

		cmd = smr_freestack_pop(smr_cmd_stack(ep->region));
		assert(cmd);
		ce->ptr = smr_local_to_peer(ep, peer_smr, tx_id, rx_id,
					    (uintptr_t) cmd);
	}

	ret = smr_send_ops[proto](ep, peer_smr, tx_id, rx_id, ofi_op_write, 0,
				  data, flags, NULL, &iov, 1, len, NULL, cmd);
	if (ret) {
		if (proto != smr_proto_inline)
			smr_freestack_push(smr_cmd_stack(ep->region), cmd);
		smr_cmd_queue_discard(ce, pos);
		goto unlock;
	}
	smr_add_rma_cmd(peer_smr, &rma_iov, 1, cmd);
	smr_cmd_queue_commit(ce, pos);

	if (proto == smr_proto_inline)
		ofi_ep_peer_tx_cntr_inc(&ep->util_ep, ofi_op_write);
unlock:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t smr_writedata(struct fid_ep *ep_fid, const void *buf, size_t len,
			     void *desc, uint64_t data, fi_addr_t dest_addr,
			     uint64_t addr, uint64_t key, void *context)
{
	struct smr_ep *ep;
	struct iovec iov;
	struct fi_rma_iov rma_iov;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	iov.iov_base = (void *) buf;
	iov.iov_len = len;
	rma_iov.addr = addr;
	rma_iov.len = len;
	rma_iov.key = key;

	return smr_generic_rma(ep, &iov, 1, &rma_iov, 1, &desc, dest_addr,
			       context, ofi_op_write, data,
			       FI_REMOTE_CQ_DATA | smr_ep_tx_flags(ep));
}

static ssize_t smr_rma_inject(struct fid_ep *ep_fid, const void *buf,
			      size_t len, fi_addr_t dest_addr, uint64_t addr,
			      uint64_t key)
{
	return smr_generic_rma_inject(ep_fid, buf, len, dest_addr, addr, key,
				      0, 0);
}

static ssize_t smr_inject_writedata(struct fid_ep *ep_fid, const void *buf,
				    size_t len, uint64_t data,
				    fi_addr_t dest_addr, uint64_t addr,
				    uint64_t key)
{
	return smr_generic_rma_inject(ep_fid, buf, len, dest_addr, addr, key,
				      data, FI_REMOTE_CQ_DATA);
}

struct fi_ops_rma smr_rma_ops = {
	.size = sizeof(struct fi_ops_rma),
	.read = smr_read,
	.readv = smr_readv,
	.readmsg = smr_readmsg,
	.write = smr_write,
	.writev = smr_writev,
	.writemsg = smr_writemsg,
	.inject = smr_rma_inject,
	.writedata = smr_writedata,
	.injectdata = smr_inject_writedata,
};