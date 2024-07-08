/*
 * Copyright (c) 2017-2020 Intel Corporation. All rights reserved.
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include "rxm.h"


static ssize_t
rxm_ep_rma_reg_iov(struct rxm_ep *rxm_ep, const struct iovec *msg_iov,
		   void **desc, void **desc_storage, size_t iov_count,
		   uint64_t access, struct rxm_tx_buf *rma_buf)
{
	size_t i, ret;

	if (!rxm_ep->msg_mr_local)
		return FI_SUCCESS;

	if (!rxm_ep->rdm_mr_local) {
		ret = rxm_msg_mr_regv(rxm_ep, msg_iov, iov_count, SIZE_MAX,
				      access, rma_buf->rma.mr);
		if (OFI_UNLIKELY(ret))
			return ret;

		for (i = 0; i < iov_count; i++)
			desc_storage[i] = fi_mr_desc(rma_buf->rma.mr[i]);
		rma_buf->rma.count = (uint8_t) iov_count;
	} else {
		for (i = 0; i < iov_count; i++)
			desc_storage[i] =
				fi_mr_desc(((struct rxm_mr *) desc[i])->msg_mr);
	}
	return FI_SUCCESS;
}

static ssize_t
rxm_ep_rma_common(struct rxm_ep *rxm_ep, const struct fi_msg_rma *msg,
		  uint64_t flags, ssize_t (*rma_msg)(struct fid_ep *ep_fid,
		  const struct fi_msg_rma *msg, uint64_t flags),
		  uint64_t comp_flags)
{
	struct rxm_tx_buf *rma_buf;
	struct fi_msg_rma msg_rma = *msg;
	struct rxm_conn *rxm_conn;
	void *mr_desc[RXM_IOV_LIMIT] = { 0 };
	ssize_t ret;

	assert(msg->rma_iov_count <= rxm_ep->rxm_info->tx_attr->rma_iov_limit);

	ofi_genlock_lock(&rxm_ep->util_ep.lock);

	ret = rxm_get_conn(rxm_ep, msg->addr, &rxm_conn);
	if (ret)
		goto unlock;

	rma_buf = rxm_get_tx_buf(rxm_ep);
	if (!rma_buf) {
		ret = -FI_EAGAIN;
		goto unlock;
	}

	rma_buf->hdr.state = RXM_RMA;
	rma_buf->pkt.ctrl_hdr.type = rxm_ctrl_eager;
	rma_buf->app_context = msg->context;
	rma_buf->flags = flags;

	ret = rxm_ep_rma_reg_iov(rxm_ep, msg_rma.msg_iov, msg_rma.desc, mr_desc,
				 msg_rma.iov_count,
				 comp_flags & (FI_WRITE | FI_READ), rma_buf);
	if (OFI_UNLIKELY(ret))
		goto release;

	msg_rma.desc = mr_desc;
	msg_rma.context = rma_buf;

	ret = rma_msg(rxm_conn->msg_ep, &msg_rma, flags);
	if (!ret)
		goto unlock;

	if ((rxm_ep->msg_mr_local) && (!rxm_ep->rdm_mr_local))
		rxm_msg_mr_closev(rma_buf->rma.mr, rma_buf->rma.count);
release:
	rxm_free_tx_buf(rxm_ep, rma_buf);
unlock:
	ofi_genlock_unlock(&rxm_ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_ep_readmsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
	       uint64_t flags)
{
	struct rxm_ep *rxm_ep;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	return rxm_ep_rma_common(rxm_ep, msg, flags | rxm_ep->util_ep.tx_msg_flags,
				 fi_readmsg, FI_READ);
}

static ssize_t rxm_ep_readv(struct fid_ep *ep_fid, const struct iovec *iov,
			    void **desc, size_t count, fi_addr_t src_addr,
			    uint64_t addr, uint64_t key, void *context)
{
	struct rxm_ep *rxm_ep = container_of(ep_fid, struct rxm_ep,
					     util_ep.ep_fid.fid);
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.len = ofi_total_iov_len(iov, count),
		.key = key,
	};
	struct fi_msg_rma msg = {
		.msg_iov = iov,
		.desc = desc,
		.iov_count = count,
		.addr = src_addr,
		.rma_iov = &rma_iov,
		.rma_iov_count = 1,
		.context = context,
		.data = 0,
	};

	return rxm_ep_rma_common(rxm_ep, &msg, rxm_ep->util_ep.tx_op_flags,
				 fi_readmsg, FI_READ);
}

static ssize_t rxm_ep_read(struct fid_ep *ep_fid, void *buf, size_t len,
			   void *desc, fi_addr_t src_addr, uint64_t addr,
			   uint64_t key, void *context)
{
	struct iovec iov = {
		.iov_base = (void*)buf,
		.iov_len = len,
	};
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.len = len,
		.key = key,
	};
	struct fi_msg_rma msg = {
		.msg_iov = &iov,
		.desc = &desc,
		.iov_count = 1,
		.addr = src_addr,
		.rma_iov = &rma_iov,
		.rma_iov_count = 1,
		.context = context,
		.data = 0,
	};
	struct rxm_ep *rxm_ep;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	return rxm_ep_rma_common(rxm_ep, &msg, rxm_ep->util_ep.tx_op_flags,
				 fi_readmsg, FI_READ);
}

static void
rxm_ep_format_rma_msg(struct rxm_tx_buf *rma_buf,
		      const struct fi_msg_rma *orig_msg,
		      struct iovec *rxm_iov, struct fi_msg_rma *rxm_msg)
{
	ssize_t ret __attribute__((unused));
	rxm_msg->context = rma_buf;
	rxm_msg->addr = orig_msg->addr;
	rxm_msg->data = orig_msg->data;

	ret = rxm_copy_from_hmem_iov(orig_msg->desc, rma_buf->pkt.data,
				     rma_buf->pkt.hdr.size, orig_msg->msg_iov,
				     orig_msg->iov_count, 0);
	assert((size_t) ret == rma_buf->pkt.hdr.size);

	rxm_iov->iov_base = &rma_buf->pkt.data;
	rxm_iov->iov_len = rma_buf->pkt.hdr.size;
	rxm_msg->msg_iov = rxm_iov;
	rxm_msg->desc = &rma_buf->hdr.desc;
	rxm_msg->iov_count = 1;

	rxm_msg->rma_iov = orig_msg->rma_iov;
	rxm_msg->rma_iov_count = orig_msg->rma_iov_count;
}

static ssize_t
rxm_ep_rma_emulate_inject_msg(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn,
			      size_t total_size, const struct fi_msg_rma *msg,
			      uint64_t flags)
{
	struct rxm_tx_buf *rma_buf;
	ssize_t ret;
	struct iovec rxm_msg_iov = { 0 };
	struct fi_msg_rma rxm_rma_msg = { 0 };

	assert(msg->rma_iov_count <= rxm_ep->rxm_info->tx_attr->rma_iov_limit);

	rma_buf = rxm_get_tx_buf(rxm_ep);
	if (!rma_buf)
		return -FI_EAGAIN;

	rma_buf->hdr.state = RXM_RMA;
	rma_buf->pkt.ctrl_hdr.type = rxm_ctrl_eager;
	rma_buf->pkt.hdr.size = total_size;
	rma_buf->app_context = msg->context;
	rma_buf->flags = flags;
	rxm_ep_format_rma_msg(rma_buf, msg, &rxm_msg_iov, &rxm_rma_msg);

	flags = (flags & ~FI_INJECT) | FI_COMPLETION;

	ret = fi_writemsg(rxm_conn->msg_ep, &rxm_rma_msg, flags);
	if (ret) {
		if (ret == -FI_EAGAIN)
			rxm_ep_do_progress(&rxm_ep->util_ep);
		rxm_free_tx_buf(rxm_ep, rma_buf);
	}
	return ret;
}

static ssize_t
rxm_ep_rma_emulate_inject(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn,
			  const void *buf, size_t len, uint64_t data,
			  fi_addr_t dest_addr, uint64_t addr, uint64_t key,
			  uint64_t flags)
{
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.len = len,
		.key = key,
	};
	struct iovec iov = {
		.iov_base = (void*)buf,
		.iov_len = len,
	};
	struct fi_msg_rma msg = {
		.msg_iov = &iov,
		.desc = NULL,
		.iov_count = 1,
		.addr = dest_addr,
		.rma_iov = &rma_iov,
		.rma_iov_count = 1,
		.context = NULL,
		.data = data,
	};

	return rxm_ep_rma_emulate_inject_msg(rxm_ep, rxm_conn, len, &msg, flags);
}

static ssize_t
rxm_ep_rma_inject_common(struct rxm_ep *rxm_ep, const struct fi_msg_rma *msg,
			 uint64_t flags)
{
	struct rxm_conn *rxm_conn;
	size_t total_size = ofi_total_iov_len(msg->msg_iov, msg->iov_count);
	ssize_t ret;

	assert(total_size <= rxm_ep->rxm_info->tx_attr->inject_size);

	ofi_genlock_lock(&rxm_ep->util_ep.lock);

	ret = rxm_get_conn(rxm_ep, msg->addr, &rxm_conn);
	if (ret)
		goto unlock;

	if ((total_size > rxm_ep->rxm_info->tx_attr->inject_size) ||
	    rxm_ep->util_ep.cntrs[CNTR_WR] ||
	    (flags & FI_COMPLETION) || (msg->iov_count > 1) ||
	    (msg->rma_iov_count > 1)) {
		ret = rxm_ep_rma_emulate_inject_msg(rxm_ep, rxm_conn,
						    total_size, msg, flags);
		goto unlock;
	}

	if (flags & FI_REMOTE_CQ_DATA) {
		ret = fi_inject_writedata(rxm_conn->msg_ep,
					  msg->msg_iov->iov_base,
					  msg->msg_iov->iov_len, msg->data,
					  msg->addr, msg->rma_iov->addr,
					  msg->rma_iov->key);
	} else {
		ret = fi_inject_write(rxm_conn->msg_ep,
				      msg->msg_iov->iov_base,
				      msg->msg_iov->iov_len, msg->addr,
				      msg->rma_iov->addr,
				      msg->rma_iov->key);
	}
	if (ret == -FI_EAGAIN)
		rxm_ep_do_progress(&rxm_ep->util_ep);
	else if (ret)
		FI_WARN(&rxm_prov, FI_LOG_EP_DATA, "fi_inject_write* for"
			"MSG provider failed: %zd\n", ret);
unlock:
	ofi_genlock_unlock(&rxm_ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_ep_generic_writemsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
			uint64_t flags)
{
	struct rxm_ep *rxm_ep;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	if (flags & FI_INJECT)
		return rxm_ep_rma_inject_common(rxm_ep, msg, flags);
	else
		return rxm_ep_rma_common(rxm_ep, msg, flags,
					 fi_writemsg, FI_WRITE);
}

static ssize_t
rxm_ep_writemsg(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
		uint64_t flags)
{
	struct rxm_ep *rxm_ep;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	return rxm_ep_generic_writemsg(ep_fid, msg, flags |
				       rxm_ep->util_ep.tx_msg_flags);
}

static ssize_t rxm_ep_writev(struct fid_ep *ep_fid, const struct iovec *iov,
			     void **desc, size_t count, fi_addr_t dest_addr,
			     uint64_t addr, uint64_t key, void *context)
{
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.len = ofi_total_iov_len(iov, count),
		.key = key,
	};
	struct fi_msg_rma msg = {
		.msg_iov = iov,
		.desc = desc,
		.iov_count = count,
		.addr = dest_addr,
		.rma_iov = &rma_iov,
		.rma_iov_count = 1,
		.context = context,
		.data = 0,
	};
	struct rxm_ep *rxm_ep;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	return rxm_ep_generic_writemsg(ep_fid, &msg,
				       rxm_ep->util_ep.tx_op_flags);
}

static ssize_t rxm_ep_writedata(struct fid_ep *ep_fid, const void *buf,
				size_t len, void *desc, uint64_t data,
				fi_addr_t dest_addr, uint64_t addr,
				uint64_t key, void *context)
{
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.len = len,
		.key = key,
	};
	struct iovec iov = {
		.iov_base = (void*)buf,
		.iov_len = len,
	};
	struct fi_msg_rma msg = {
		.msg_iov = &iov,
		.desc = &desc,
		.iov_count = 1,
		.addr = dest_addr,
		.rma_iov = &rma_iov,
		.rma_iov_count = 1,
		.context = context,
		.data = data,
	};
	struct rxm_ep *rxm_ep;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	return rxm_ep_generic_writemsg(ep_fid, &msg, rxm_ep->util_ep.tx_op_flags |
				       FI_REMOTE_CQ_DATA);
}

static ssize_t rxm_ep_write(struct fid_ep *ep_fid, const void *buf,
			    size_t len, void *desc, fi_addr_t dest_addr,
			    uint64_t addr, uint64_t key, void *context)
{
	struct fi_rma_iov rma_iov = {
		.addr = addr,
		.len = len,
		.key = key,
	};
	struct iovec iov = {
		.iov_base = (void*)buf,
		.iov_len = len,
	};
	struct fi_msg_rma msg = {
		.msg_iov = &iov,
		.desc = &desc,
		.iov_count = 1,
		.addr = dest_addr,
		.rma_iov = &rma_iov,
		.rma_iov_count = 1,
		.context = context,
		.data = 0,
	};
	struct rxm_ep *rxm_ep;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	return rxm_ep_generic_writemsg(ep_fid, &msg, rxm_ep->util_ep.tx_op_flags);
}

static ssize_t rxm_ep_inject_write(struct fid_ep *ep_fid, const void *buf,
				   size_t len, fi_addr_t dest_addr,
				   uint64_t addr, uint64_t key)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&rxm_ep->util_ep.lock);

	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		goto unlock;

	if (len > rxm_ep->inject_limit || rxm_ep->util_ep.cntrs[CNTR_WR]) {
		ret = rxm_ep_rma_emulate_inject(rxm_ep, rxm_conn, buf, len, 0,
						dest_addr, addr, key,
						FI_INJECT);
		goto unlock;
	}

	ret = fi_inject_write(rxm_conn->msg_ep, buf, len, dest_addr, addr, key);
	if (ret == -FI_EAGAIN)
		rxm_ep_do_progress(&rxm_ep->util_ep);
	else if (ret)
		FI_WARN(&rxm_prov, FI_LOG_EP_DATA, "fi_inject_write for"
			" MSG provider failed: %zd\n", ret);
unlock:
	ofi_genlock_unlock(&rxm_ep->util_ep.lock);
	return ret;
}

static ssize_t rxm_ep_inject_writedata(struct fid_ep *ep_fid, const void *buf,
				       size_t len, uint64_t data,
				       fi_addr_t dest_addr, uint64_t addr,
				       uint64_t key)
{
	struct rxm_conn *rxm_conn;
	struct rxm_ep *rxm_ep;
	ssize_t ret;

	rxm_ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&rxm_ep->util_ep.lock);

	ret = rxm_get_conn(rxm_ep, dest_addr, &rxm_conn);
	if (ret)
		goto unlock;

	if (len > rxm_ep->inject_limit || rxm_ep->util_ep.cntrs[CNTR_WR]) {
		ret = rxm_ep_rma_emulate_inject(
			rxm_ep, rxm_conn, buf, len, data, dest_addr,
			addr, key, FI_REMOTE_CQ_DATA | FI_INJECT);
		goto unlock;
	}

	ret = fi_inject_writedata(rxm_conn->msg_ep, buf, len,
				  data, dest_addr, addr, key);
	if (ret == -FI_EAGAIN)
		rxm_ep_do_progress(&rxm_ep->util_ep);
	else if (ret)
		FI_WARN(&rxm_prov, FI_LOG_EP_DATA, "fi_inject_writedata"
			" for MSG provider failed: %zd\n", ret);
unlock:
	ofi_genlock_unlock(&rxm_ep->util_ep.lock);
	return ret;
}

struct fi_ops_rma rxm_rma_ops = {
	.size = sizeof (struct fi_ops_rma),
	.read = rxm_ep_read,
	.readv = rxm_ep_readv,
	.readmsg = rxm_ep_readmsg,
	.write = rxm_ep_write,
	.writev = rxm_ep_writev,
	.writemsg = rxm_ep_writemsg,
	.inject = rxm_ep_inject_write,
	.writedata = rxm_ep_writedata,
	.injectdata = rxm_ep_inject_writedata,
};


static ssize_t
rxm_read_thru(struct fid_ep *ep_fid, void *buf, size_t len,
	      void *desc, fi_addr_t src_addr, uint64_t addr,
	      uint64_t key, void *context)
{
	struct rxm_ep *ep;
	struct rxm_conn *conn;
	ssize_t ret;

	ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&ep->util_ep.lock);

	ret = rxm_get_conn(ep, src_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_read(conn->msg_ep, buf, len, desc, src_addr, addr,
		      key, context);
unlock:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_readv_thru(struct fid_ep *ep_fid, const struct iovec *iov,
	       void **desc, size_t count, fi_addr_t src_addr,
	       uint64_t addr, uint64_t key, void *context)
{
	struct rxm_ep *ep;
	struct rxm_conn *conn;
	ssize_t ret;

	ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&ep->util_ep.lock);

	ret = rxm_get_conn(ep, src_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_readv(conn->msg_ep, iov, desc, count, src_addr, addr,
		       key, context);
unlock:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_readmsg_thru(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
		 uint64_t flags)
{
	struct rxm_ep *ep;
	struct rxm_conn *conn;
	ssize_t ret;

	ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&ep->util_ep.lock);

	ret = rxm_get_conn(ep, msg->addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_readmsg(conn->msg_ep, msg, flags);
unlock:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_write_thru(struct fid_ep *ep_fid, const void *buf,
	       size_t len, void *desc, fi_addr_t dest_addr,
	       uint64_t addr, uint64_t key, void *context)
{
	struct rxm_ep *ep;
	struct rxm_conn *conn;
	ssize_t ret;

	ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&ep->util_ep.lock);

	ret = rxm_get_conn(ep, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_write(conn->msg_ep, buf, len, desc, dest_addr, addr,
		       key, context);
unlock:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_writev_thru(struct fid_ep *ep_fid, const struct iovec *iov,
		void **desc, size_t count, fi_addr_t dest_addr,
		uint64_t addr, uint64_t key, void *context)
{
	struct rxm_ep *ep;
	struct rxm_conn *conn;
	ssize_t ret;

	ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&ep->util_ep.lock);

	ret = rxm_get_conn(ep, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_writev(conn->msg_ep, iov, desc, count, dest_addr, addr,
			key, context);
unlock:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_writemsg_thru(struct fid_ep *ep_fid, const struct fi_msg_rma *msg,
		  uint64_t flags)
{
	struct rxm_ep *ep;
	struct rxm_conn *conn;
	ssize_t ret;

	ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&ep->util_ep.lock);

	ret = rxm_get_conn(ep, msg->addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_writemsg(conn->msg_ep, msg, flags);
unlock:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_inject_write_thru(struct fid_ep *ep_fid, const void *buf,
		      size_t len, fi_addr_t dest_addr,
		      uint64_t addr, uint64_t key)
{
	struct rxm_ep *ep;
	struct rxm_conn *conn;
	ssize_t ret;

	ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&ep->util_ep.lock);

	ret = rxm_get_conn(ep, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_inject_write(conn->msg_ep, buf, len, dest_addr, addr, key);
unlock:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_writedata_thru(struct fid_ep *ep_fid, const void *buf,
		   size_t len, void *desc, uint64_t data,
		   fi_addr_t dest_addr, uint64_t addr,
		   uint64_t key, void *context)
{
	struct rxm_ep *ep;
	struct rxm_conn *conn;
	ssize_t ret;

	ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&ep->util_ep.lock);

	ret = rxm_get_conn(ep, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_writedata(conn->msg_ep, buf, len, desc, data, dest_addr,
			   addr, key, context);
unlock:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t
rxm_inject_writedata_thru(struct fid_ep *ep_fid, const void *buf,
			  size_t len, uint64_t data, fi_addr_t dest_addr,
			  uint64_t addr, uint64_t key)
{
	struct rxm_ep *ep;
	struct rxm_conn *conn;
	ssize_t ret;

	ep = container_of(ep_fid, struct rxm_ep, util_ep.ep_fid.fid);
	ofi_genlock_lock(&ep->util_ep.lock);

	ret = rxm_get_conn(ep, dest_addr, &conn);
	if (ret)
		goto unlock;

	ret = fi_inject_writedata(conn->msg_ep, buf, len, data, dest_addr,
				  addr, key);
unlock:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

struct fi_ops_rma rxm_rma_thru_ops = {
	.size = sizeof (struct fi_ops_rma),
	.read = rxm_read_thru,
	.readv = rxm_readv_thru,
	.readmsg = rxm_readmsg_thru,
	.write = rxm_write_thru,
	.writev = rxm_writev_thru,
	.writemsg = rxm_writemsg_thru,
	.inject = rxm_inject_write_thru,
	.writedata = rxm_writedata_thru,
	.injectdata = rxm_inject_writedata_thru,
};
