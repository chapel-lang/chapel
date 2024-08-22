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
#include "ofi_shm_p2p.h"
#include "smr.h"

static void smr_add_rma_cmd(struct smr_region *peer_smr,
		const struct fi_rma_iov *rma_iov, size_t iov_count,
		struct smr_cmd_entry *ce)
{
	ce->rma_cmd.rma.rma_count = iov_count;
	memcpy(ce->rma_cmd.rma.rma_iov, rma_iov, sizeof(*rma_iov) * iov_count);
}

static void smr_format_rma_resp(struct smr_cmd *cmd, fi_addr_t peer_id,
				const struct fi_rma_iov *rma_iov, size_t count,
				size_t total_len, uint32_t op, uint64_t op_flags)
{
	smr_generic_format(cmd, peer_id, op, 0, 0, op_flags);
	cmd->msg.hdr.size = total_len;
}

static ssize_t smr_rma_fast(struct smr_ep *ep, struct smr_region *peer_smr,
			const struct iovec *iov, size_t iov_count,
			const struct fi_rma_iov *rma_iov, size_t rma_count,
			void **desc, int peer_id, int id, void *context,
			uint32_t op, uint64_t op_flags)
{
	struct iovec vma_iovec[SMR_IOV_LIMIT], rma_iovec[SMR_IOV_LIMIT];
	struct xpmem_client *xpmem;
	struct smr_cmd_entry *ce;
	size_t total_len;
	int ret, i;
	int64_t pos;

	ret = smr_cmd_queue_next(smr_cmd_queue(peer_smr), &ce, &pos);
	if (ret == -FI_ENOENT)
		return -FI_EAGAIN;

	memcpy(vma_iovec, iov, sizeof(*iov) * iov_count);
	for (i = 0; i < rma_count; i++) {
		rma_iovec[i].iov_base = (void *) rma_iov[i].addr;
		rma_iovec[i].iov_len = rma_iov[i].len;
	}

	total_len = ofi_total_iov_len(iov, iov_count);

	xpmem = &smr_peer_data(ep->region)[id].xpmem;

	ret = ofi_shm_p2p_copy(ep->p2p_type, vma_iovec, iov_count,
			       rma_iovec, rma_count, total_len, peer_smr->pid,
			       op == ofi_op_write, xpmem);

	if (ret) {
		smr_cmd_queue_discard(ce, pos);
		return -FI_EAGAIN;
	}

	smr_format_rma_resp(&ce->cmd, peer_id, rma_iov, rma_count, total_len,
			    (op == ofi_op_write) ? ofi_op_write_async :
			    ofi_op_read_async, op_flags);
	smr_cmd_queue_commit(ce, pos);
	return FI_SUCCESS;
}

static ssize_t smr_generic_rma(struct smr_ep *ep, const struct iovec *iov,
	size_t iov_count, const struct fi_rma_iov *rma_iov, size_t rma_count,
	void **desc, fi_addr_t addr, void *context, uint32_t op, uint64_t data,
	uint64_t op_flags)
{
	struct smr_domain *domain;
	struct smr_region *peer_smr;
	int64_t id, peer_id;
	int cmds, err = 0, proto = smr_src_inline;
	ssize_t ret = 0;
	size_t total_len;
	struct smr_cmd_entry *ce;
	int64_t pos;

	assert(iov_count <= SMR_IOV_LIMIT);
	assert(rma_count <= SMR_IOV_LIMIT);
	assert(ofi_total_iov_len(iov, iov_count) ==
	       ofi_total_rma_iov_len(rma_iov, rma_count));

	domain = container_of(ep->util_ep.domain, struct smr_domain, util_domain);

	id = smr_verify_peer(ep, addr);
	if (id < 0)
		return -FI_EAGAIN;

	peer_id = smr_peer_data(ep->region)[id].addr.id;
	peer_smr = smr_peer_region(ep->region, id);

	cmds = 1 + !(domain->fast_rma && !(op_flags &
		    (FI_REMOTE_CQ_DATA | FI_DELIVERY_COMPLETE)) &&
		     rma_count == 1 && smr_vma_enabled(ep, peer_smr));

	if (smr_peer_data(ep->region)[id].sar_status)
		return -FI_EAGAIN;

	ofi_genlock_lock(&ep->util_ep.lock);

	if (cmds == 1) {
		err = smr_rma_fast(ep, peer_smr, iov, iov_count, rma_iov,
				   rma_count, desc, peer_id, id, context, op,
				   op_flags);
		if (err) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"error doing fast RMA\n");
			if (err == -FI_EAGAIN) {
				ret = -FI_EAGAIN;
				goto unlock;
			}

			ret = smr_write_err_comp(ep->util_ep.rx_cq, NULL,
						op_flags, 0, -err);
		} else {
			ret = smr_complete_tx(ep, context, op, op_flags);
		}

		if (ret) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"unable to process tx completion\n");
		}
		goto unlock;
	}

	ret = smr_cmd_queue_next(smr_cmd_queue(peer_smr), &ce, &pos);
	if (ret == -FI_ENOENT) {
		/* kick the peer to process any outstanding commands */
		ret = -FI_EAGAIN;
		goto unlock;
	}

	total_len = ofi_total_iov_len(iov, iov_count);
	assert(!(op_flags & FI_INJECT) || total_len <= SMR_INJECT_SIZE);

	proto = smr_select_proto(desc, iov_count, smr_vma_enabled(ep, peer_smr),
	                         op, total_len, op_flags);

	ret = smr_proto_ops[proto](ep, peer_smr, id, peer_id, op, 0, data,
				   op_flags, (struct ofi_mr **)desc, iov,
				   iov_count, total_len, context, &ce->cmd);
	if (ret) {
		smr_cmd_queue_discard(ce, pos);
		goto unlock;
	}

	smr_add_rma_cmd(peer_smr, rma_iov, rma_count, ce);
	smr_cmd_queue_commit(ce, pos);

	if (proto != smr_src_inline && proto != smr_src_inject)
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

static ssize_t smr_generic_rma_inject(struct fid_ep *ep_fid, const void *buf,
		size_t len, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		uint64_t data, uint64_t flags)
{
	struct smr_ep *ep;
	struct smr_domain *domain;
	struct smr_region *peer_smr;
	struct iovec iov;
	struct fi_rma_iov rma_iov;
	int64_t id, peer_id;
	int cmds, proto = smr_src_inline;
	ssize_t ret = 0;
	struct smr_cmd_entry *ce;
	int64_t pos;

	assert(len <= SMR_INJECT_SIZE);
	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);
	domain = container_of(ep->util_ep.domain, struct smr_domain, util_domain);

	id = smr_verify_peer(ep, dest_addr);
	if (id < 0)
		return -FI_EAGAIN;

	peer_id = smr_peer_data(ep->region)[id].addr.id;
	peer_smr = smr_peer_region(ep->region, id);

	cmds = 1 + !(domain->fast_rma && !(flags & FI_REMOTE_CQ_DATA) &&
		     smr_vma_enabled(ep, peer_smr));

	if (smr_peer_data(ep->region)[id].sar_status)
		return -FI_EAGAIN;

	iov.iov_base = (void *) buf;
	iov.iov_len = len;
	rma_iov.addr = addr;
	rma_iov.len = len;
	rma_iov.key = key;

	if (cmds == 1) {
		ret = smr_rma_fast(ep, peer_smr, &iov, 1, &rma_iov, 1, NULL,
				   peer_id, id, NULL, ofi_op_write, flags);
		goto out;
	}

	ret = smr_cmd_queue_next(smr_cmd_queue(peer_smr), &ce, &pos);
	if (ret == -FI_ENOENT)
		return -FI_EAGAIN;

	proto = len <= SMR_MSG_DATA_LEN ? smr_src_inline : smr_src_inject;
	ret = smr_proto_ops[proto](ep, peer_smr, id, peer_id, ofi_op_write, 0,
			data, flags, NULL, &iov, 1, len, NULL, &ce->cmd);
	if (ret) {
		smr_cmd_queue_discard(ce, pos);
		return -FI_EAGAIN;
	}
	smr_add_rma_cmd(peer_smr, &rma_iov, 1, ce);
	smr_cmd_queue_commit(ce, pos);

out:
	if (!ret)
		ofi_ep_peer_tx_cntr_inc(&ep->util_ep, ofi_op_write);
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

	return smr_generic_rma(ep, &iov, 1, &rma_iov, 1, &desc, dest_addr, context,
			       ofi_op_write, data,
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
			size_t len, uint64_t data, fi_addr_t dest_addr,
			uint64_t addr, uint64_t key)
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
