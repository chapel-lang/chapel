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
#include "ofi_hmem.h"
#include "smr.h"


static void smr_format_rma_ioc(struct smr_cmd *cmd, const struct fi_rma_ioc *rma_ioc,
			       size_t ioc_count)
{
	cmd->rma.rma_count = ioc_count;
	memcpy(cmd->rma.rma_ioc, rma_ioc, sizeof(*rma_ioc) * ioc_count);
}

static void smr_generic_atomic_format(struct smr_cmd *cmd, uint8_t datatype,
				      uint8_t atomic_op)
{
	cmd->msg.hdr.datatype = datatype;
	cmd->msg.hdr.atomic_op = atomic_op;
}

static void smr_format_inline_atomic(struct smr_cmd *cmd,
				     enum fi_hmem_iface iface, uint64_t device,
				     const struct iovec *iov, size_t count)
{
	cmd->msg.hdr.op_src = smr_src_inline;

	cmd->msg.hdr.size = ofi_copy_from_hmem_iov(cmd->msg.data.msg,
					SMR_MSG_DATA_LEN, iface, device,
					iov, count, 0);
}

static void smr_do_atomic_inline(struct smr_ep *ep, struct smr_region *peer_smr,
			int64_t id, int64_t peer_id, uint32_t op,
			uint64_t op_flags, enum fi_hmem_iface iface,
			uint64_t device, uint8_t datatype, uint8_t atomic_op,
			const struct iovec *iov, size_t iov_count,
			size_t total_len)
{
	struct smr_cmd *cmd;

	cmd = ofi_cirque_next(smr_cmd_queue(peer_smr));
	smr_generic_format(cmd, peer_id, op, 0, 0, op_flags);
	smr_generic_atomic_format(cmd, datatype, atomic_op);
	smr_format_inline_atomic(cmd, iface, device, iov, iov_count);

	ofi_cirque_commit(smr_cmd_queue(peer_smr));
	peer_smr->cmd_cnt--;
}

static void smr_format_inject_atomic(struct smr_cmd *cmd,
			enum fi_hmem_iface iface, uint64_t device,
			const struct iovec *iov, size_t count,
			const struct iovec *resultv, size_t result_count,
			const struct iovec *compv, size_t comp_count,
			struct smr_region *smr, struct smr_inject_buf *tx_buf)
{
	size_t comp_size;

	cmd->msg.hdr.op_src = smr_src_inject;
	cmd->msg.hdr.src_data = smr_get_offset(smr, tx_buf);

	switch (cmd->msg.hdr.op) {
	case ofi_op_atomic:
		cmd->msg.hdr.size = ofi_copy_from_hmem_iov(tx_buf->data,
					SMR_INJECT_SIZE, iface, device,
					iov, count, 0);
		break;
	case ofi_op_atomic_fetch:
		if (cmd->msg.hdr.atomic_op == FI_ATOMIC_READ)
			cmd->msg.hdr.size = ofi_total_iov_len(resultv, result_count);
		else
			cmd->msg.hdr.size = ofi_copy_from_hmem_iov(tx_buf->data,
						SMR_INJECT_SIZE, iface, device,
						iov, count, 0);
		break;
	case ofi_op_atomic_compare:
		cmd->msg.hdr.size = ofi_copy_from_hmem_iov(tx_buf->buf,
						SMR_COMP_INJECT_SIZE, iface, device,
						iov, count, 0);
		comp_size = ofi_copy_from_hmem_iov(tx_buf->comp, SMR_COMP_INJECT_SIZE,
					      iface, device, compv, comp_count, 0);
		if (comp_size != cmd->msg.hdr.size)
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"atomic and compare buffer size mismatch\n");
		break;
	default:
		break;
	}
}

static ssize_t smr_do_atomic_inject(struct smr_ep *ep, struct smr_region *peer_smr,
			int64_t id, int64_t peer_id, uint32_t op,
			uint64_t op_flags, enum fi_hmem_iface iface,
			uint64_t device, uint8_t datatype, uint8_t atomic_op,
			const struct iovec *iov, size_t iov_count,
			const struct iovec *resultv, size_t result_count,
			const struct iovec *compv, size_t comp_count,
			size_t total_len, void *context, uint16_t smr_flags)
{
	struct smr_cmd *cmd;
	struct smr_inject_buf *tx_buf;
	struct smr_tx_entry *pend;
	struct smr_resp *resp;

	cmd = ofi_cirque_next(smr_cmd_queue(peer_smr));
	tx_buf = smr_freestack_pop(smr_inject_pool(peer_smr));

	smr_generic_format(cmd, peer_id, op, 0, 0, op_flags);
	smr_generic_atomic_format(cmd, datatype, atomic_op);
	smr_format_inject_atomic(cmd, iface, device, iov, iov_count, 
				 resultv, result_count, compv, comp_count,
				 peer_smr, tx_buf);

	if (smr_flags & SMR_RMA_REQ || op_flags & FI_DELIVERY_COMPLETE) {
		if (ofi_cirque_isfull(smr_resp_queue(ep->region))) {
			smr_freestack_push(smr_inject_pool(peer_smr), tx_buf);
			return -FI_EAGAIN;
		}
		resp = ofi_cirque_next(smr_resp_queue(ep->region));
		pend = ofi_freestack_pop(ep->pend_fs);
		smr_format_pend_resp(pend, cmd, context, iface, device, resultv,
				     result_count, op_flags, id, resp);
		cmd->msg.hdr.data = smr_get_offset(ep->region, resp);
		ofi_cirque_commit(smr_resp_queue(ep->region));
	}

	cmd->msg.hdr.op_flags |= smr_flags;
	ofi_cirque_commit(smr_cmd_queue(peer_smr));
	peer_smr->cmd_cnt--;

	return FI_SUCCESS;
}

static int smr_select_atomic_proto(uint32_t op, uint64_t total_len,
				   uint64_t op_flags)
{
	if (op == ofi_op_atomic_compare || op == ofi_op_atomic_fetch ||
	    op_flags & FI_DELIVERY_COMPLETE || total_len > SMR_MSG_DATA_LEN)
		return smr_src_inject;

	return smr_src_inline;
}

static ssize_t smr_generic_atomic(struct smr_ep *ep,
			const struct fi_ioc *ioc, void **desc, size_t count,
			const struct fi_ioc *compare_ioc, void **compare_desc,
			size_t compare_count, struct fi_ioc *result_ioc,
			void **result_desc, size_t result_count,
			fi_addr_t addr, const struct fi_rma_ioc *rma_ioc,
			size_t rma_count, enum fi_datatype datatype,
			enum fi_op atomic_op, void *context, uint32_t op,
			uint64_t op_flags)
{
	struct smr_cmd *cmd;
	struct smr_region *peer_smr;
	struct iovec iov[SMR_IOV_LIMIT];
	struct iovec compare_iov[SMR_IOV_LIMIT];
	struct iovec result_iov[SMR_IOV_LIMIT];
	enum fi_hmem_iface iface;
	uint64_t device;
	uint16_t smr_flags = 0;
	int64_t id, peer_id;
	int proto;
	ssize_t ret = 0;
	size_t total_len;

	assert(count <= SMR_IOV_LIMIT);
	assert(result_count <= SMR_IOV_LIMIT);
	assert(compare_count <= SMR_IOV_LIMIT);
	assert(rma_count <= SMR_IOV_LIMIT);

	id = smr_verify_peer(ep, addr);
	if (id < 0)
		return -FI_EAGAIN;

	peer_id = smr_peer_data(ep->region)[id].addr.id;
	peer_smr = smr_peer_region(ep->region, id);

	pthread_spin_lock(&peer_smr->lock);
	if (peer_smr->cmd_cnt < 2 || smr_peer_data(ep->region)[id].sar_status) {
		ret = -FI_EAGAIN;
		goto unlock_region;
	}

	ofi_spin_lock(&ep->tx_lock);
	total_len = ofi_datatype_size(datatype) * ofi_total_ioc_cnt(ioc, count);

	switch (op) {
	case ofi_op_atomic_compare:
		assert(compare_ioc);
		ofi_ioc_to_iov(compare_ioc, compare_iov, compare_count,
			       ofi_datatype_size(datatype));
		total_len *= 2;
		/* fall through */
	case ofi_op_atomic_fetch:
		assert(result_ioc);
		ofi_ioc_to_iov(result_ioc, result_iov, result_count,
			       ofi_datatype_size(datatype));
		smr_flags = SMR_RMA_REQ;
		/* fall through */
	case ofi_op_atomic:
		if (atomic_op != FI_ATOMIC_READ) {
			assert(ioc);
			ofi_ioc_to_iov(ioc, iov, count, ofi_datatype_size(datatype));
		} else {
			count = 0;
		}
		break;
	default:
		break;
	}

	iface = smr_get_mr_hmem_iface(ep->util_ep.domain, desc, &device);

	proto = smr_select_atomic_proto(op, total_len, op_flags);

	if (proto == smr_src_inline) {
		smr_do_atomic_inline(ep, peer_smr, id, peer_id, ofi_op_atomic,
			 	     op_flags, iface, device, datatype, atomic_op,
				     iov, count, total_len);
	} else {
		ret = smr_do_atomic_inject(ep, peer_smr, id, peer_id, op,
				op_flags, iface, device, datatype, atomic_op,
				iov, count, result_iov, result_count,
				compare_iov, compare_count, total_len, context,
				smr_flags);
		if (ret)
			goto unlock_cq;
	}

	if (!(smr_flags & SMR_RMA_REQ) && !(op_flags & FI_DELIVERY_COMPLETE)) {
		ret = smr_complete_tx(ep, context, op, op_flags);
		if (ret) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"unable to process tx completion\n");
		}
	}

	cmd = ofi_cirque_next(smr_cmd_queue(peer_smr));
	smr_format_rma_ioc(cmd, rma_ioc, rma_count);
	ofi_cirque_commit(smr_cmd_queue(peer_smr));
	peer_smr->cmd_cnt--;
	smr_signal(peer_smr);
unlock_cq:
	ofi_spin_unlock(&ep->tx_lock);
unlock_region:
	pthread_spin_unlock(&peer_smr->lock);
	return ret;
}

static ssize_t smr_atomic_writemsg(struct fid_ep *ep_fid,
			const struct fi_msg_atomic *msg, uint64_t flags)
{
	struct smr_ep *ep;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	return smr_generic_atomic(ep, msg->msg_iov, msg->desc, msg->iov_count,
				  NULL, NULL, 0, NULL, NULL, 0, msg->addr,
				  msg->rma_iov, msg->rma_iov_count,
				  msg->datatype, msg->op, msg->context,
				  ofi_op_atomic, flags | ep->util_ep.tx_msg_flags);
}

static ssize_t smr_atomic_writev(struct fid_ep *ep_fid,
			const struct fi_ioc *iov, void **desc, size_t count,
			fi_addr_t dest_addr, uint64_t addr, uint64_t key,
			enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct smr_ep *ep;
	struct fi_rma_ioc rma_iov;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	rma_iov.addr = addr;
	rma_iov.count = ofi_total_ioc_cnt(iov, count);
	rma_iov.key = key;

	return smr_generic_atomic(ep, iov, desc, count, NULL, NULL, 0, NULL,
				  NULL, 0, dest_addr, &rma_iov, 1, datatype,
				  op, context, ofi_op_atomic, smr_ep_tx_flags(ep));
}

static ssize_t smr_atomic_write(struct fid_ep *ep_fid, const void *buf, size_t count,
			void *desc, fi_addr_t dest_addr, uint64_t addr,
			uint64_t key, enum fi_datatype datatype, enum fi_op op,
			void *context)
{
	struct smr_ep *ep;
	struct fi_ioc iov;
	struct fi_rma_ioc rma_iov;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	iov.addr = (void *) buf;
	iov.count = count;

	rma_iov.addr = addr;
	rma_iov.count = count;
	rma_iov.key = key;

	return smr_generic_atomic(ep, &iov, &desc, 1, NULL, NULL, 0, NULL, NULL, 0,
				  dest_addr, &rma_iov, 1, datatype, op, context,
				  ofi_op_atomic, smr_ep_tx_flags(ep));
}

static ssize_t smr_atomic_inject(struct fid_ep *ep_fid, const void *buf,
			size_t count, fi_addr_t dest_addr, uint64_t addr,
			uint64_t key, enum fi_datatype datatype, enum fi_op op)
{
	struct smr_cmd *cmd;
	struct smr_ep *ep;
	struct smr_region *peer_smr;
	struct iovec iov;
	struct fi_rma_ioc rma_ioc;
	int64_t id, peer_id;
	ssize_t ret = 0;
	size_t total_len;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	id = smr_verify_peer(ep, dest_addr);
	if (id < 0)
		return -FI_EAGAIN;

	peer_id = smr_peer_data(ep->region)[id].addr.id;
	peer_smr = smr_peer_region(ep->region, id);

	pthread_spin_lock(&peer_smr->lock);
	if (peer_smr->cmd_cnt < 2 || smr_peer_data(ep->region)[id].sar_status) {
		ret = -FI_EAGAIN;
		goto unlock_region;
	}

	total_len = count * ofi_datatype_size(datatype);
	assert(total_len <= SMR_INJECT_SIZE);

	iov.iov_base = (void *) buf;
	iov.iov_len = total_len;

	rma_ioc.addr = addr;
	rma_ioc.count = count;
	rma_ioc.key = key;

	if (total_len <= SMR_MSG_DATA_LEN) {
		smr_do_atomic_inline(ep, peer_smr, id, peer_id, ofi_op_atomic,
			 	     0, FI_HMEM_SYSTEM, 0, datatype, op,
				     &iov, 1, total_len);
	} else if (total_len <= SMR_INJECT_SIZE) {
		ret = smr_do_atomic_inject(ep, peer_smr, id, peer_id,
				ofi_op_atomic, 0, FI_HMEM_SYSTEM, 0, datatype,
				op, &iov, 1, NULL, 0, NULL, 0, total_len,
				NULL, 0);
		if (ret)
			goto unlock_region;
	}

	cmd = ofi_cirque_next(smr_cmd_queue(peer_smr));
	smr_format_rma_ioc(cmd, &rma_ioc, 1);
	ofi_cirque_commit(smr_cmd_queue(peer_smr));
	peer_smr->cmd_cnt--;
	smr_signal(peer_smr);

	ofi_ep_tx_cntr_inc_func(&ep->util_ep, ofi_op_atomic);
unlock_region:
	pthread_spin_unlock(&peer_smr->lock);
	return ret;
}

static ssize_t smr_atomic_readwritemsg(struct fid_ep *ep_fid,
			const struct fi_msg_atomic *msg, struct fi_ioc *resultv,
			void **result_desc, size_t result_count, uint64_t flags)
{
	struct smr_ep *ep;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	return smr_generic_atomic(ep, msg->msg_iov, msg->desc, msg->iov_count,
				  NULL, NULL, 0, resultv, result_desc,
				  result_count, msg->addr,
				  msg->rma_iov, msg->rma_iov_count,
				  msg->datatype, msg->op, msg->context,
				  ofi_op_atomic_fetch,
				  flags | ep->util_ep.tx_msg_flags);
}

static ssize_t smr_atomic_readwritev(struct fid_ep *ep_fid,
			const struct fi_ioc *iov, void **desc, size_t count,
			struct fi_ioc *resultv, void **result_desc,
			size_t result_count, fi_addr_t dest_addr, uint64_t addr,
			uint64_t key, enum fi_datatype datatype, enum fi_op op,
			void *context)
{
	struct smr_ep *ep;
	struct fi_rma_ioc rma_iov;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	rma_iov.addr = addr;
	rma_iov.count = ofi_total_ioc_cnt(iov, count);
	rma_iov.key = key;

	return smr_generic_atomic(ep, iov, desc, count, NULL, NULL, 0, resultv,
				  result_desc, result_count, dest_addr,
				  &rma_iov, 1, datatype, op, context,
				  ofi_op_atomic_fetch, smr_ep_tx_flags(ep));
}

static ssize_t smr_atomic_readwrite(struct fid_ep *ep_fid, const void *buf,
			size_t count, void *desc, void *result,
			void *result_desc, fi_addr_t dest_addr, uint64_t addr,
			uint64_t key, enum fi_datatype datatype, enum fi_op op,
			void *context)
{
	struct smr_ep *ep;
	struct fi_ioc iov, resultv;
	struct fi_rma_ioc rma_iov;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	iov.addr = (void *) buf;
	iov.count = count;

	resultv.addr = result;
	resultv.count = count;

	rma_iov.addr = addr;
	rma_iov.count = count;
	rma_iov.key = key;

	return smr_generic_atomic(ep, &iov, &desc, 1, NULL, NULL, 0, &resultv,
				  &result_desc, 1, dest_addr, &rma_iov, 1,
				  datatype, op, context, ofi_op_atomic_fetch,
				  smr_ep_tx_flags(ep));
}

static ssize_t smr_atomic_compwritemsg(struct fid_ep *ep_fid,
			const struct fi_msg_atomic *msg,
			const struct fi_ioc *comparev, void **compare_desc,
			size_t compare_count, struct fi_ioc *resultv,
			void **result_desc, size_t result_count, uint64_t flags)
{
	struct smr_ep *ep;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	return smr_generic_atomic(ep, msg->msg_iov, msg->desc, msg->iov_count,
				  comparev, compare_desc, compare_count,
				  resultv, result_desc,
				  result_count, msg->addr,
				  msg->rma_iov, msg->rma_iov_count,
				  msg->datatype, msg->op, msg->context,
				  ofi_op_atomic_compare,
				  flags | ep->util_ep.tx_msg_flags);
}

static ssize_t smr_atomic_compwritev(struct fid_ep *ep_fid,
			const struct fi_ioc *iov, void **desc, size_t count,
			const struct fi_ioc *comparev, void **compare_desc,
			size_t compare_count, struct fi_ioc *resultv,
			void **result_desc, size_t result_count,
			fi_addr_t dest_addr, uint64_t addr, uint64_t key,
			enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct smr_ep *ep;
	struct fi_rma_ioc rma_iov;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	rma_iov.addr = addr;
	rma_iov.count = ofi_total_ioc_cnt(iov, count);
	rma_iov.key = key;

	return smr_generic_atomic(ep, iov, desc, count, comparev, compare_desc,
				  compare_count, resultv, result_desc,
				  result_count, dest_addr, &rma_iov, 1,
				  datatype, op, context, ofi_op_atomic_compare,
				  smr_ep_tx_flags(ep));
}

static ssize_t smr_atomic_compwrite(struct fid_ep *ep_fid, const void *buf,
			size_t count, void *desc, const void *compare,
			void *compare_desc, void *result, void *result_desc,
			fi_addr_t dest_addr, uint64_t addr, uint64_t key,
			enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct smr_ep *ep;
	struct fi_ioc iov, resultv, comparev;
	struct fi_rma_ioc rma_iov;

	ep = container_of(ep_fid, struct smr_ep, util_ep.ep_fid.fid);

	iov.addr = (void *) buf;
	iov.count = count;

	resultv.addr = result;
	resultv.count = count;

	comparev.addr = (void *) compare;
	comparev.count = count;

	rma_iov.addr = addr;
	rma_iov.count = count;
	rma_iov.key = key;

	return smr_generic_atomic(ep, &iov, &desc, 1, &comparev, &compare_desc,
				  1, &resultv, &result_desc, 1, dest_addr,
				  &rma_iov, 1, datatype, op, context,
				  ofi_op_atomic_compare, smr_ep_tx_flags(ep));
}

int smr_query_atomic(struct fid_domain *domain, enum fi_datatype datatype,
		     enum fi_op op, struct fi_atomic_attr *attr, uint64_t flags)
{
	int ret;
	size_t total_size;

	if (flags & FI_TAGGED) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"tagged atomic op not supported\n");
		return -FI_EOPNOTSUPP;
	}

	if ((datatype == FI_INT128) || (datatype == FI_UINT128)) {
		FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
			"128-bit datatypes not supported\n");
		return -FI_EOPNOTSUPP;
	}

	ret = ofi_atomic_valid(&smr_prov, datatype, op, flags);
	if (ret || !attr)
		return ret;

	attr->size = ofi_datatype_size(datatype);

	total_size = (flags & FI_COMPARE_ATOMIC) ? SMR_COMP_INJECT_SIZE :
		      SMR_INJECT_SIZE;
	attr->count = total_size / attr->size;

	return ret;
}

static int smr_atomic_valid(struct fid_ep *ep, enum fi_datatype datatype,
			    enum fi_op op, size_t *count)
{
	struct fi_atomic_attr attr;
	int ret;

	ret = smr_query_atomic(NULL, datatype, op, &attr, 0);

	if (!ret)
		*count = attr.count;

	return ret;
}

static int smr_atomic_fetch_valid(struct fid_ep *ep, enum fi_datatype datatype,
				  enum fi_op op, size_t *count)
{
	struct fi_atomic_attr attr;
	int ret;

	ret = smr_query_atomic(NULL, datatype, op, &attr, FI_FETCH_ATOMIC);

	if (!ret)
		*count = attr.count;

	return ret;
}

static int smr_atomic_comp_valid(struct fid_ep *ep, enum fi_datatype datatype,
				 enum fi_op op, size_t *count)
{
	struct fi_atomic_attr attr;
	int ret;

	ret = smr_query_atomic(NULL, datatype, op, &attr, FI_COMPARE_ATOMIC);

	if (!ret)
		*count = attr.count;

	return ret;
}

struct fi_ops_atomic smr_atomic_ops = {
	.size = sizeof(struct fi_ops_atomic),
	.write = smr_atomic_write,
	.writev = smr_atomic_writev,
	.writemsg = smr_atomic_writemsg,
	.inject = smr_atomic_inject,
	.readwrite = smr_atomic_readwrite,
	.readwritev = smr_atomic_readwritev,
	.readwritemsg = smr_atomic_readwritemsg,
	.compwrite = smr_atomic_compwrite,
	.compwritev = smr_atomic_compwritev,
	.compwritemsg = smr_atomic_compwritemsg,
	.writevalid = smr_atomic_valid,
	.readwritevalid = smr_atomic_fetch_valid,
	.compwritevalid = smr_atomic_comp_valid,
};
