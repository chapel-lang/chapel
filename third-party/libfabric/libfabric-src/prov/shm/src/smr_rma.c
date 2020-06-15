/*
 * Copyright (c) 2013-2018 Intel Corporation. All rights reserved
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


static void smr_format_rma_iov(struct smr_cmd *cmd, const struct fi_rma_iov *rma_iov,
			       size_t iov_count)
{
	cmd->rma.rma_count = iov_count;
	memcpy(cmd->rma.rma_iov, rma_iov, sizeof(*rma_iov) * iov_count);
}

static void smr_format_rma_resp(struct smr_cmd *cmd, fi_addr_t peer_id,
				const struct fi_rma_iov *rma_iov, size_t count,
				size_t total_len, uint32_t op, uint64_t op_flags)
{
	smr_generic_format(cmd, peer_id, op, 0, 0, op_flags);
	cmd->msg.hdr.size = total_len;
}

ssize_t smr_rma_fast(struct smr_region *peer_smr, struct smr_cmd *cmd,
		     const struct iovec *iov, size_t iov_count,
		     const struct fi_rma_iov *rma_iov, size_t rma_count,
		     void **desc, int peer_id, void *context, uint32_t op,
		     uint64_t op_flags)
{
	struct iovec rma_iovec[SMR_IOV_LIMIT];
	size_t total_len;
	int ret, i;

	for (i = 0; i < rma_count; i++) {
		rma_iovec[i].iov_base = (void *) rma_iov[i].addr;
		rma_iovec[i].iov_len = rma_iov[i].len;
	}

	total_len = ofi_total_iov_len(iov, iov_count);

	if (op == ofi_op_write) {
		ret = ofi_process_vm_writev(peer_smr->pid, iov, iov_count,
					    rma_iovec, rma_count, 0);
	} else {
		ret = ofi_process_vm_readv(peer_smr->pid, iov, iov_count,
					   rma_iovec, rma_count, 0);
	}

	if (ret != total_len) {
		if (ret < 0) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"CMA write error\n");
			ret = -errno;
		} else {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"unable to process tx completion\n");
			ret = -FI_EIO;
		}
		return ret;
	}

	smr_format_rma_resp(cmd, peer_id, rma_iov, rma_count, total_len,
			    (op == ofi_op_write) ? ofi_op_write_async :
			    ofi_op_read_async, op_flags);

	return 0;
}

ssize_t smr_generic_rma(struct smr_ep *ep, const struct iovec *iov,
	size_t iov_count, const struct fi_rma_iov *rma_iov, size_t rma_count,
	void **desc, fi_addr_t addr, void *context, uint32_t op, uint64_t data,
	uint64_t op_flags)
{
	struct smr_domain *domain;
	struct smr_region *peer_smr;
	struct smr_inject_buf *tx_buf;
	struct smr_sar_msg *sar;
	struct smr_resp *resp;
	struct smr_cmd *cmd;
	struct smr_tx_entry *pend;
	int id, peer_id, cmds, err = 0, comp = 1;
	uint16_t comp_flags;
	ssize_t ret = 0;
	size_t total_len;

	assert(iov_count <= SMR_IOV_LIMIT);
	assert(rma_count <= SMR_IOV_LIMIT);

	domain = container_of(ep->util_ep.domain, struct smr_domain, util_domain);

	id = (int) addr;
	peer_id = smr_peer_data(ep->region)[id].addr.addr;

	ret = smr_verify_peer(ep, id);
	if (ret)
		return ret;

	cmds = 1 + !(domain->fast_rma && !(op_flags &
		    (FI_REMOTE_CQ_DATA | FI_DELIVERY_COMPLETE)) &&
		     rma_count == 1 &&
		     ep->region->cma_cap == SMR_CMA_CAP_ON);

	peer_smr = smr_peer_region(ep->region, id);
	fastlock_acquire(&peer_smr->lock);
	if (peer_smr->cmd_cnt < cmds ||
	    smr_peer_data(ep->region)[id].sar_status) {
		ret = -FI_EAGAIN;
		goto unlock_region;
	}

	fastlock_acquire(&ep->util_ep.tx_cq->cq_lock);
	if (ofi_cirque_isfull(ep->util_ep.tx_cq->cirq)) {
		ret = -FI_EAGAIN;
		goto unlock_cq;
	}

	cmd = ofi_cirque_tail(smr_cmd_queue(peer_smr));

	if (cmds == 1) {
		err = smr_rma_fast(peer_smr, cmd, iov, iov_count, rma_iov,
				   rma_count, desc, peer_id,  context, op,
				   op_flags);
		comp_flags = cmd->msg.hdr.op_flags;
		goto commit_comp;
	}

	total_len = ofi_total_iov_len(iov, iov_count);

	smr_generic_format(cmd, peer_id, op, 0, data, op_flags);
	if (total_len <= SMR_MSG_DATA_LEN && op == ofi_op_write &&
	    !(op_flags & FI_DELIVERY_COMPLETE)) {
		smr_format_inline(cmd, iov, iov_count);
	} else if (total_len <= SMR_INJECT_SIZE &&
		   !(op_flags & FI_DELIVERY_COMPLETE)) {
		tx_buf = smr_freestack_pop(smr_inject_pool(peer_smr));
		smr_format_inject(cmd, iov, iov_count, peer_smr, tx_buf);
		if (op == ofi_op_read_req) {
			if (ofi_cirque_isfull(smr_resp_queue(ep->region))) {
				smr_freestack_push(smr_inject_pool(peer_smr), tx_buf);
				ret = -FI_EAGAIN;
				goto unlock_cq;
			}
			cmd->msg.hdr.op_flags |= SMR_RMA_REQ;
			resp = ofi_cirque_tail(smr_resp_queue(ep->region));
			pend = freestack_pop(ep->pend_fs);
			smr_format_pend_resp(pend, cmd, context, iov,
					     iov_count, id, resp);
			cmd->msg.hdr.data = smr_get_offset(ep->region, resp);
			ofi_cirque_commit(smr_resp_queue(ep->region));
			comp = 0;
		}
	} else {
		if (ofi_cirque_isfull(smr_resp_queue(ep->region))) {
			ret = -FI_EAGAIN;
			goto unlock_cq;
		}
		resp = ofi_cirque_tail(smr_resp_queue(ep->region));
		pend = freestack_pop(ep->pend_fs);
		if (ep->region->cma_cap == SMR_CMA_CAP_ON) {
			smr_format_iov(cmd, iov, iov_count, total_len, ep->region, resp);
		} else {
			if (total_len <= smr_env.sar_threshold) {
				if (!peer_smr->sar_cnt) {
					ret = -FI_EAGAIN;
				} else {
					sar = smr_freestack_pop(smr_sar_pool(peer_smr));
					smr_format_sar(cmd, iov, iov_count, total_len,
						       ep->region, peer_smr, sar,
						       pend, resp);
					peer_smr->sar_cnt--;
					smr_peer_data(ep->region)[id].sar_status = 1;
				}
			} else {
				ret = smr_format_mmap(ep, cmd, iov, iov_count,
						      total_len, pend, resp);
			}
			if (ret) {
				freestack_push(ep->pend_fs, pend);
				ret = -FI_EAGAIN;
				goto unlock_cq;
			}
		}
		smr_format_pend_resp(pend, cmd, context, iov, iov_count, id, resp);
		ofi_cirque_commit(smr_resp_queue(ep->region));
		comp = 0;
	}

	comp_flags = cmd->msg.hdr.op_flags;
	ofi_cirque_commit(smr_cmd_queue(peer_smr));
	peer_smr->cmd_cnt--;
	cmd = ofi_cirque_tail(smr_cmd_queue(peer_smr));
	smr_format_rma_iov(cmd, rma_iov, rma_count);

commit_comp:
	ofi_cirque_commit(smr_cmd_queue(peer_smr));
	peer_smr->cmd_cnt--;

	if (!comp)
		goto unlock_cq;

	ret = smr_complete_tx(ep, context, op, comp_flags, err);
	if (ret) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"unable to process tx completion\n");
	}

unlock_cq:
	fastlock_release(&ep->util_ep.tx_cq->cq_lock);
unlock_region:
	fastlock_release(&peer_smr->lock);
	return ret;
}

ssize_t smr_read(struct fid_ep *ep_fid, void *buf, size_t len, void *desc,
	fi_addr_t src_addr, uint64_t addr, uint64_t key, void *context)
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

ssize_t smr_readv(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
	size_t count, fi_addr_t src_addr, uint64_t addr, uint64_t key,
	void *context)
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

ssize_t smr_readmsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
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

ssize_t smr_write(struct fid_ep *ep_fid, const void *buf, size_t len, void *desc,
	fi_addr_t dest_addr, uint64_t addr, uint64_t key, void *context)
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

ssize_t smr_writev(struct fid_ep *ep_fid, const struct iovec *iov, void **desc,
		size_t count, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		void *context)
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


ssize_t smr_writemsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
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

ssize_t smr_generic_rma_inject(struct fid_ep *ep_fid, const void *buf,
	size_t len, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
	uint64_t data, uint64_t flags)
{
	struct smr_ep *ep;
	struct smr_domain *domain;
	struct smr_region *peer_smr;
	struct smr_inject_buf *tx_buf;
	struct smr_cmd *cmd;
	struct iovec iov;
	struct fi_rma_iov rma_iov;
	int id, peer_id, cmds;
	ssize_t ret = 0;

	assert(len <= SMR_INJECT_SIZE);
	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);
	domain = container_of(ep->util_ep.domain, struct smr_domain, util_domain);

	id = (int) dest_addr;
	peer_id = smr_peer_data(ep->region)[id].addr.addr;

	ret = smr_verify_peer(ep, id);
	if (ret)
		return ret;

	cmds = 1 + !(domain->fast_rma && !(flags & FI_REMOTE_CQ_DATA) &&
		     ep->region->cma_cap == SMR_CMA_CAP_ON);

	peer_smr = smr_peer_region(ep->region, id);
	fastlock_acquire(&peer_smr->lock);
	if (peer_smr->cmd_cnt < cmds ||
	    smr_peer_data(ep->region)[id].sar_status) {
		ret = -FI_EAGAIN;
		goto unlock_region;
	}

	iov.iov_base = (void *) buf;
	iov.iov_len = len;
	rma_iov.addr = addr;
	rma_iov.len = len;
	rma_iov.key = key;

	cmd = ofi_cirque_tail(smr_cmd_queue(peer_smr));

	if (cmds == 1) {
		ret = smr_rma_fast(peer_smr, cmd, &iov, 1, &rma_iov, 1, NULL,
				   peer_id, NULL, ofi_op_write, flags);
		if (ret)
			goto unlock_region;
		goto commit;
	}

	smr_generic_format(cmd, peer_id, ofi_op_write, 0, data, flags);
	if (len <= SMR_MSG_DATA_LEN) {
		smr_format_inline(cmd, &iov, 1);
	} else {
		tx_buf = smr_freestack_pop(smr_inject_pool(peer_smr));
		smr_format_inject(cmd, &iov, 1, peer_smr, tx_buf);
	}

	ofi_cirque_commit(smr_cmd_queue(peer_smr));
	peer_smr->cmd_cnt--;
	cmd = ofi_cirque_tail(smr_cmd_queue(peer_smr));
	smr_format_rma_iov(cmd, &rma_iov, 1);

commit:
	ofi_cirque_commit(smr_cmd_queue(peer_smr));
	peer_smr->cmd_cnt--;
	ofi_ep_tx_cntr_inc_func(&ep->util_ep, ofi_op_write);
unlock_region:
	fastlock_release(&peer_smr->lock);
	return ret;
}

ssize_t smr_writedata(struct fid_ep *ep_fid, const void *buf, size_t len,
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

ssize_t smr_rma_inject(struct fid_ep *ep_fid, const void *buf,
	size_t len, fi_addr_t dest_addr, uint64_t addr, uint64_t key)
{
	return smr_generic_rma_inject(ep_fid, buf, len, dest_addr, addr, key,
				      0, 0);
}

ssize_t smr_inject_writedata(struct fid_ep *ep_fid, const void *buf, size_t len,
			     uint64_t data, fi_addr_t dest_addr, uint64_t addr,
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
