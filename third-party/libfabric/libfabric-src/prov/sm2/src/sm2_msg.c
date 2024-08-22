/*
 * Copyright (c) Intel Corporation. All rights reserved
 * Copyright (c) Amazon.com, Inc. or its affiliates. All rights reserved.
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
#include "sm2.h"

static inline int sm2_select_proto(void **desc, size_t iov_count,
				   uint64_t op_flags, uint64_t total_len)
{
	struct ofi_mr *sm2_desc;
	enum fi_hmem_iface iface;

	if (desc && *desc) {
		sm2_desc = (struct ofi_mr *) *desc;
		iface = sm2_desc->iface;
		if (iface == FI_HMEM_CUDA) {
			if ((sm2_desc->flags & OFI_HMEM_DATA_DEV_REG_HANDLE) &&
			    (total_len <= SM2_MAX_GDRCOPY_SIZE)) {
				assert(sm2_desc->hmem_data);
				return sm2_proto_inject;
			}
			/* Do not inject if IPC is available so device to device
			 * transfer may occur if possible. */
			/* TODO - multi IOV support */
			if (iov_count == 1 && desc && desc[0]) {
				if (ofi_hmem_is_ipc_enabled(iface) &&
				    sm2_desc->flags & FI_HMEM_DEVICE_ONLY &&
				    !(op_flags & FI_INJECT)) {
					return sm2_proto_ipc;
				}
			}
		}
	}

	if (total_len <= SM2_INJECT_SIZE)
		return sm2_proto_inject;

	return sm2_proto_cma;
}

static ssize_t sm2_recvmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
			   uint64_t flags)
{
	struct sm2_ep *ep;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);
	return util_srx_generic_recv(ep->srx, msg->msg_iov, msg->desc,
				     msg->iov_count, msg->addr, msg->context,
				     flags | ep->util_ep.rx_msg_flags);
}

static ssize_t sm2_recvv(struct fid_ep *ep_fid, const struct iovec *iov,
			 void **desc, size_t count, fi_addr_t src_addr,
			 void *context)
{
	struct sm2_ep *ep;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);
	return util_srx_generic_recv(ep->srx, iov, desc, count, src_addr,
				     context, sm2_ep_rx_flags(ep));
}

static ssize_t sm2_recv(struct fid_ep *ep_fid, void *buf, size_t len,
			void *desc, fi_addr_t src_addr, void *context)
{
	struct iovec iov;
	struct sm2_ep *ep;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	iov.iov_base = buf;
	iov.iov_len = len;
	return util_srx_generic_recv(ep->srx, &iov, &desc, 1, src_addr, context,
				     sm2_ep_rx_flags(ep));
}

static ssize_t sm2_generic_sendmsg(struct sm2_ep *ep, const struct iovec *iov,
				   void **desc, size_t iov_count,
				   fi_addr_t addr, uint64_t tag, uint64_t data,
				   void *context, uint32_t op,
				   uint64_t op_flags)
{
	struct sm2_region *peer_smr;
	sm2_gid_t peer_gid;
	ssize_t ret = 0;
	size_t total_len;
	struct ofi_mr **mr = (struct ofi_mr **) desc;
	int proto;

	assert(iov_count <= SM2_IOV_LIMIT);

	ret = sm2_verify_peer(ep, addr, &peer_gid);
	if (ret < 0)
		return ret;

	peer_smr = sm2_peer_region(ep, peer_gid);

	ofi_genlock_lock(&ep->util_ep.lock);

	total_len = ofi_total_iov_len(iov, iov_count);
	assert(!(op_flags & FI_INJECT) || total_len <= SM2_INJECT_SIZE);

	proto = sm2_select_proto(desc, iov_count, op_flags, total_len);
	ret = sm2_proto_ops[proto](ep, peer_smr, peer_gid, op, tag, data,
				   op_flags, mr, iov, iov_count, total_len,
				   context);
	if (ret)
		goto unlock_cq;

	if (sm2_proto_imm_send_comp(proto)) {
		ret = sm2_complete_tx(ep, context, op, op_flags);
		if (ret) {
			FI_WARN(&sm2_prov, FI_LOG_EP_CTRL,
				"Unable to process tx completion\n");
			goto unlock_cq;
		}
	}

unlock_cq:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t sm2_send(struct fid_ep *ep_fid, const void *buf, size_t len,
			void *desc, fi_addr_t dest_addr, void *context)
{
	struct sm2_ep *ep;
	struct iovec msg_iov;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	msg_iov.iov_base = (void *) buf;
	msg_iov.iov_len = len;

	return sm2_generic_sendmsg(ep, &msg_iov, &desc, 1, dest_addr, 0, 0,
				   context, ofi_op_msg, sm2_ep_tx_flags(ep));
}

static ssize_t sm2_sendv(struct fid_ep *ep_fid, const struct iovec *iov,
			 void **desc, size_t count, fi_addr_t dest_addr,
			 void *context)
{
	struct sm2_ep *ep;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	return sm2_generic_sendmsg(ep, iov, desc, count, dest_addr, 0, 0,
				   context, ofi_op_msg, sm2_ep_tx_flags(ep));
}

static ssize_t sm2_sendmsg(struct fid_ep *ep_fid, const struct fi_msg *msg,
			   uint64_t flags)
{
	struct sm2_ep *ep;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	return sm2_generic_sendmsg(ep, msg->msg_iov, msg->desc, msg->iov_count,
				   msg->addr, 0, msg->data, msg->context,
				   ofi_op_msg,
				   flags | ep->util_ep.tx_msg_flags);
}

static ssize_t sm2_generic_inject(struct fid_ep *ep_fid, const void *buf,
				  size_t len, fi_addr_t dest_addr, uint64_t tag,
				  uint64_t data, uint32_t op, uint64_t op_flags)
{
	struct sm2_ep *ep;
	struct sm2_region *peer_smr;
	sm2_gid_t peer_gid;
	ssize_t ret = 0;
	struct iovec msg_iov;

	assert(len <= SM2_INJECT_SIZE);

	msg_iov.iov_base = (void *) buf;
	msg_iov.iov_len = len;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	ret = sm2_verify_peer(ep, dest_addr, &peer_gid);
	if (ret < 0)
		return ret;

	peer_smr = sm2_peer_region(ep, peer_gid);

	ofi_genlock_lock(&ep->util_ep.lock);
	ret = sm2_proto_ops[sm2_proto_inject](ep, peer_smr, peer_gid, op, tag,
					      data, op_flags, NULL, &msg_iov, 1,
					      len, NULL);

	if (!ret)
		ofi_ep_peer_tx_cntr_inc(&ep->util_ep, op);

	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t sm2_inject(struct fid_ep *ep_fid, const void *buf, size_t len,
			  fi_addr_t dest_addr)
{
	return sm2_generic_inject(ep_fid, buf, len, dest_addr, 0, 0, ofi_op_msg,
				  0);
}

static ssize_t sm2_senddata(struct fid_ep *ep_fid, const void *buf, size_t len,
			    void *desc, uint64_t data, fi_addr_t dest_addr,
			    void *context)
{
	struct sm2_ep *ep;
	struct iovec iov;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	iov.iov_base = (void *) buf;
	iov.iov_len = len;

	return sm2_generic_sendmsg(ep, &iov, &desc, 1, dest_addr, 0, data,
				   context, ofi_op_msg,
				   FI_REMOTE_CQ_DATA | sm2_ep_tx_flags(ep));
}

static ssize_t sm2_injectdata(struct fid_ep *ep_fid, const void *buf,
			      size_t len, uint64_t data, fi_addr_t dest_addr)
{
	return sm2_generic_inject(ep_fid, buf, len, dest_addr, 0, data,
				  ofi_op_msg, FI_REMOTE_CQ_DATA);
}

struct fi_ops_msg sm2_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = sm2_recv,
	.recvv = sm2_recvv,
	.recvmsg = sm2_recvmsg,
	.send = sm2_send,
	.sendv = sm2_sendv,
	.sendmsg = sm2_sendmsg,
	.inject = sm2_inject,
	.senddata = sm2_senddata,
	.injectdata = sm2_injectdata,
};

struct fi_ops_msg sm2_no_recv_msg_ops = {
	.size = sizeof(struct fi_ops_msg),
	.recv = fi_no_msg_recv,
	.recvv = fi_no_msg_recvv,
	.recvmsg = fi_no_msg_recvmsg,
	.send = sm2_send,
	.sendv = sm2_sendv,
	.sendmsg = sm2_sendmsg,
	.inject = sm2_inject,
	.senddata = sm2_senddata,
	.injectdata = sm2_injectdata,
};

static ssize_t sm2_trecv(struct fid_ep *ep_fid, void *buf, size_t len,
			 void *desc, fi_addr_t src_addr, uint64_t tag,
			 uint64_t ignore, void *context)
{
	struct iovec iov;
	struct sm2_ep *ep;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	iov.iov_base = buf;
	iov.iov_len = len;

	return util_srx_generic_trecv(ep->srx, &iov, &desc, 1, src_addr,
				      context, tag, ignore,
				      sm2_ep_rx_flags(ep));
}

static ssize_t sm2_trecvv(struct fid_ep *ep_fid, const struct iovec *iov,
			  void **desc, size_t count, fi_addr_t src_addr,
			  uint64_t tag, uint64_t ignore, void *context)
{
	struct sm2_ep *ep;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	return util_srx_generic_trecv(ep->srx, iov, desc, count, src_addr,
				      context, tag, ignore,
				      sm2_ep_rx_flags(ep));
}

static ssize_t sm2_trecvmsg(struct fid_ep *ep_fid,
			    const struct fi_msg_tagged *msg, uint64_t flags)
{
	struct sm2_ep *ep;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	return util_srx_generic_trecv(ep->srx, msg->msg_iov, msg->desc,
				      msg->iov_count, msg->addr, msg->context,
				      msg->tag, msg->ignore,
				      flags | ep->util_ep.rx_msg_flags);
}

static ssize_t sm2_tsend(struct fid_ep *ep_fid, const void *buf, size_t len,
			 void *desc, fi_addr_t dest_addr, uint64_t tag,
			 void *context)
{
	struct sm2_ep *ep;
	struct iovec msg_iov;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	msg_iov.iov_base = (void *) buf;
	msg_iov.iov_len = len;

	return sm2_generic_sendmsg(ep, &msg_iov, &desc, 1, dest_addr, tag, 0,
				   context, ofi_op_tagged, sm2_ep_tx_flags(ep));
}

static ssize_t sm2_tsendv(struct fid_ep *ep_fid, const struct iovec *iov,
			  void **desc, size_t count, fi_addr_t dest_addr,
			  uint64_t tag, void *context)
{
	struct sm2_ep *ep;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	return sm2_generic_sendmsg(ep, iov, desc, count, dest_addr, tag, 0,
				   context, ofi_op_tagged, sm2_ep_tx_flags(ep));
}

static ssize_t sm2_tsendmsg(struct fid_ep *ep_fid,
			    const struct fi_msg_tagged *msg, uint64_t flags)
{
	struct sm2_ep *ep;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	return sm2_generic_sendmsg(ep, msg->msg_iov, msg->desc, msg->iov_count,
				   msg->addr, msg->tag, msg->data, msg->context,
				   ofi_op_tagged,
				   flags | ep->util_ep.tx_msg_flags);
}

static ssize_t sm2_tinject(struct fid_ep *ep_fid, const void *buf, size_t len,
			   fi_addr_t dest_addr, uint64_t tag)
{
	return sm2_generic_inject(ep_fid, buf, len, dest_addr, tag, 0,
				  ofi_op_tagged, 0);
}

static ssize_t sm2_tsenddata(struct fid_ep *ep_fid, const void *buf, size_t len,
			     void *desc, uint64_t data, fi_addr_t dest_addr,
			     uint64_t tag, void *context)
{
	struct sm2_ep *ep;
	struct iovec iov;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	iov.iov_base = (void *) buf;
	iov.iov_len = len;

	return sm2_generic_sendmsg(ep, &iov, &desc, 1, dest_addr, tag, data,
				   context, ofi_op_tagged,
				   FI_REMOTE_CQ_DATA | sm2_ep_tx_flags(ep));
}

static ssize_t sm2_tinjectdata(struct fid_ep *ep_fid, const void *buf,
			       size_t len, uint64_t data, fi_addr_t dest_addr,
			       uint64_t tag)
{
	return sm2_generic_inject(ep_fid, buf, len, dest_addr, tag, data,
				  ofi_op_tagged, FI_REMOTE_CQ_DATA);
}

struct fi_ops_tagged sm2_tag_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = sm2_trecv,
	.recvv = sm2_trecvv,
	.recvmsg = sm2_trecvmsg,
	.send = sm2_tsend,
	.sendv = sm2_tsendv,
	.sendmsg = sm2_tsendmsg,
	.inject = sm2_tinject,
	.senddata = sm2_tsenddata,
	.injectdata = sm2_tinjectdata,
};

struct fi_ops_tagged sm2_no_recv_tag_ops = {
	.size = sizeof(struct fi_ops_tagged),
	.recv = fi_no_tagged_recv,
	.recvv = fi_no_tagged_recvv,
	.recvmsg = fi_no_tagged_recvmsg,
	.send = sm2_tsend,
	.sendv = sm2_tsendv,
	.sendmsg = sm2_tsendmsg,
	.inject = sm2_tinject,
	.senddata = sm2_tsenddata,
	.injectdata = sm2_tinjectdata,
};
