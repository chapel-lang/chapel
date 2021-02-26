/*
 * Copyright (c) 2019-2020 Amazon.com, Inc. or its affiliates.
 * All rights reserved.
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

#include <ofi_atomic.h>
#include "rxr.h"
#include "rxr_rma.h"
#include "rxr_cntr.h"
#include "rxr_atomic.h"
#include "rxr_pkt_cmd.h"

static void rxr_atomic_copy_shm_msg(struct fi_msg_atomic *shm_msg,
				    const struct fi_msg_atomic *msg,
				    struct fi_rma_ioc *rma_iov)
{
	int i;

	assert(msg->rma_iov_count <= RXR_IOV_LIMIT);
	memcpy(shm_msg, msg, sizeof(*msg));
	if (!(shm_info->domain_attr->mr_mode & FI_MR_VIRT_ADDR)) {
		memcpy(rma_iov, msg->rma_iov,
		       sizeof(*msg->rma_iov) * msg->rma_iov_count);
		for (i = 0; i < msg->rma_iov_count; i++)
			rma_iov[i].addr = 0;
		shm_msg->rma_iov = rma_iov;
	}
}

static
struct rxr_tx_entry *
rxr_atomic_alloc_tx_entry(struct rxr_ep *rxr_ep,
			  const struct fi_msg_atomic *msg_atomic,
			  const struct rxr_atomic_ex *atomic_ex,
			  uint32_t op, uint64_t flags)
{
	struct rxr_tx_entry *tx_entry;
	struct fi_msg msg;
	struct iovec iov[RXR_IOV_LIMIT];
	size_t datatype_size = ofi_datatype_size(msg_atomic->datatype);

	tx_entry = ofi_buf_alloc(rxr_ep->tx_entry_pool);
	if (OFI_UNLIKELY(!tx_entry)) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL, "TX entries exhausted.\n");
		return NULL;
	}

#if ENABLE_DEBUG
	dlist_insert_tail(&tx_entry->tx_entry_entry, &rxr_ep->tx_entry_list);
#endif
	ofi_ioc_to_iov(msg_atomic->msg_iov, iov, msg_atomic->iov_count, datatype_size);
	msg.addr = msg_atomic->addr;
	msg.msg_iov = iov;
	msg.context = msg_atomic->context;
	msg.iov_count = msg_atomic->iov_count;
	msg.data = msg_atomic->data;
	msg.desc = msg_atomic->desc;
	rxr_tx_entry_init(rxr_ep, tx_entry, &msg, op, flags);

	assert(msg_atomic->rma_iov_count > 0);
	assert(msg_atomic->rma_iov);
	tx_entry->rma_iov_count = msg_atomic->rma_iov_count;
	ofi_rma_ioc_to_iov(msg_atomic->rma_iov,
			   tx_entry->rma_iov,
			   msg_atomic->rma_iov_count,
			   datatype_size);

	tx_entry->atomic_hdr.atomic_op = msg_atomic->op;
	tx_entry->atomic_hdr.datatype = msg_atomic->datatype;

	if (op == ofi_op_atomic_fetch || op == ofi_op_atomic_compare) {
		assert(atomic_ex);
		memcpy(&tx_entry->atomic_ex, atomic_ex, sizeof(struct rxr_atomic_ex));
	}

	return tx_entry;
}

static
ssize_t rxr_atomic_generic_efa(struct rxr_ep *rxr_ep,
			       const struct fi_msg_atomic *msg,
			       const struct rxr_atomic_ex *atomic_ex,
			       uint32_t op, uint64_t flags)
{
	struct rxr_tx_entry *tx_entry;
	struct rxr_peer *peer;
	ssize_t err;
	static int req_pkt_type_list[] = {
		[ofi_op_atomic] = RXR_WRITE_RTA_PKT,
		[ofi_op_atomic_fetch] = RXR_FETCH_RTA_PKT,
		[ofi_op_atomic_compare] = RXR_COMPARE_RTA_PKT
	};

	assert(msg->iov_count <= rxr_ep->tx_iov_limit);
	rxr_perfset_start(rxr_ep, perf_rxr_tx);
	fastlock_acquire(&rxr_ep->util_ep.lock);

	if (OFI_UNLIKELY(is_tx_res_full(rxr_ep))) {
		err = -FI_EAGAIN;
		goto out;
	}

	peer = rxr_ep_get_peer(rxr_ep, msg->addr);

	tx_entry = rxr_atomic_alloc_tx_entry(rxr_ep, msg, atomic_ex, op, flags);
	if (OFI_UNLIKELY(!tx_entry)) {
		err = -FI_EAGAIN;
		rxr_ep_progress_internal(rxr_ep);
		goto out;
	}

	tx_entry->msg_id = (peer->next_msg_id != ~0) ?
			    peer->next_msg_id++ : ++peer->next_msg_id;

	err = rxr_pkt_post_ctrl_or_queue(rxr_ep, RXR_TX_ENTRY,
					tx_entry, req_pkt_type_list[op],
					0);

	if (OFI_UNLIKELY(err)) {
		rxr_release_tx_entry(rxr_ep, tx_entry);
		peer->next_msg_id--;
	}

out:
	fastlock_release(&rxr_ep->util_ep.lock);
	rxr_perfset_end(rxr_ep, perf_rxr_tx);
	return err;
}

static ssize_t
rxr_atomic_inject(struct fid_ep *ep,
		  const void *buf, size_t count,
		  fi_addr_t dest_addr, uint64_t remote_addr, uint64_t remote_key,
		  enum fi_datatype datatype, enum fi_op op)
{
	struct fi_ioc iov;
	struct fi_rma_ioc rma_iov;
	struct fi_msg_atomic msg;

	struct rxr_ep *rxr_ep;
	struct rxr_peer *peer;

	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);
	peer = rxr_ep_get_peer(rxr_ep, dest_addr);
	if (peer->is_local) {
		assert(rxr_ep->use_shm);
		if (!(shm_info->domain_attr->mr_mode & FI_MR_VIRT_ADDR))
			remote_addr = 0;
		return fi_inject_atomic(rxr_ep->shm_ep, buf, count, peer->shm_fiaddr,
					remote_addr, remote_key, datatype, op);
	}

	iov.addr = (void *)buf;
	iov.count = count;

	rma_iov.addr = remote_addr;
	rma_iov.count = count;
	rma_iov.key = remote_key;

	msg.msg_iov = &iov;
	msg.iov_count = 1;
	msg.desc = NULL;
	msg.addr = dest_addr;
	msg.rma_iov = &rma_iov;
	msg.rma_iov_count = 1;
	msg.datatype = datatype;
	msg.op = op;
	msg.context = NULL;
	msg.data = 0;

	return rxr_atomic_generic_efa(rxr_ep, &msg, NULL, ofi_op_atomic,
				      FI_INJECT | RXR_NO_COMPLETION);
}

static ssize_t
rxr_atomic_writemsg(struct fid_ep *ep,
		    const struct fi_msg_atomic *msg,
		    uint64_t flags)
{
	struct fi_msg_atomic shm_msg;
	struct rxr_ep *rxr_ep;
	struct rxr_peer *peer;
	struct fi_rma_ioc rma_iov[RXR_IOV_LIMIT];

	FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
	       "%s: iov_len: %lu flags: %lx\n",
	       __func__, ofi_total_ioc_cnt(msg->msg_iov, msg->iov_count), flags);

	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);
	peer = rxr_ep_get_peer(rxr_ep, msg->addr);
	if (peer->is_local) {
		assert(rxr_ep->use_shm);
		rxr_atomic_copy_shm_msg(&shm_msg, msg, rma_iov);
		shm_msg.addr = peer->shm_fiaddr;
		return fi_atomicmsg(rxr_ep->shm_ep, &shm_msg, flags);
	}

	return rxr_atomic_generic_efa(rxr_ep, msg, NULL, ofi_op_atomic, flags);
}

static ssize_t
rxr_atomic_writev(struct fid_ep *ep,
		  const struct fi_ioc *iov, void **desc, size_t count,
		  fi_addr_t dest_addr, uint64_t remote_addr, uint64_t remote_key,
		  enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct fi_msg_atomic msg;
	struct fi_rma_ioc rma_ioc;

	rma_ioc.addr = remote_addr;
	rma_ioc.count = ofi_total_ioc_cnt(iov, count);
	rma_ioc.key = remote_key;

	msg.msg_iov = iov;
	msg.iov_count = count;
	msg.desc = desc;
	msg.addr = dest_addr;
	msg.rma_iov = &rma_ioc;
	msg.rma_iov_count = 1;
	msg.datatype = datatype;
	msg.op = op;
	msg.context = context;
	msg.data = 0;

	FI_DBG(&rxr_prov, FI_LOG_EP_DATA, "%s total_count=%ld atomic_op=%d\n", __func__,
	       ofi_total_ioc_cnt(iov, count), msg.op);

	return rxr_atomic_writemsg(ep, &msg, 0);
}

static ssize_t
rxr_atomic_write(struct fid_ep *ep,
		 const void *buf, size_t count, void *desc,
		 fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		 enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct fi_ioc ioc;

	ioc.addr = (void *)buf;
	ioc.count = count;
	return rxr_atomic_writev(ep, &ioc, &desc, 1,
				 dest_addr, addr, key,
				 datatype, op, context);
}

static ssize_t
rxr_atomic_readwritemsg(struct fid_ep *ep,
			const struct fi_msg_atomic *msg,
			struct fi_ioc *resultv, void **result_desc, size_t result_count,
			uint64_t flags)
{
	struct rxr_ep *rxr_ep;
	struct rxr_peer *peer;
	struct fi_msg_atomic shm_msg;
	struct fi_rma_ioc rma_iov[RXR_IOV_LIMIT];
	struct rxr_atomic_ex atomic_ex;
	size_t datatype_size = ofi_datatype_size(msg->datatype);

	FI_DBG(&rxr_prov, FI_LOG_EP_DATA, "%s total_len=%ld atomic_op=%d\n", __func__,
	       ofi_total_ioc_cnt(msg->msg_iov, msg->iov_count), msg->op);

	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);
	peer = rxr_ep_get_peer(rxr_ep, msg->addr);
	if (peer->is_local) {
		assert(rxr_ep->use_shm);
		rxr_atomic_copy_shm_msg(&shm_msg, msg, rma_iov);
		shm_msg.addr = peer->shm_fiaddr;
		return fi_fetch_atomicmsg(rxr_ep->shm_ep, &shm_msg,
					  resultv, result_desc, result_count,
					  flags);
	}

	ofi_ioc_to_iov(resultv, atomic_ex.resp_iov, result_count, datatype_size);
	atomic_ex.resp_iov_count = result_count;
	return rxr_atomic_generic_efa(rxr_ep, msg, &atomic_ex, ofi_op_atomic_fetch, flags);
}

static ssize_t
rxr_atomic_readwritev(struct fid_ep *ep,
		      const struct fi_ioc *iov, void **desc, size_t count,
		      struct fi_ioc *resultv, void **result_desc, size_t result_count,
		      fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		      enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct fi_msg_atomic msg;
	struct fi_rma_ioc rma_ioc;

	rma_ioc.addr = addr;
	rma_ioc.count = ofi_total_ioc_cnt(iov, count);
	rma_ioc.key = key;

	msg.msg_iov = iov;
	msg.iov_count = count;
	msg.desc = desc;
	msg.addr = dest_addr;
	msg.rma_iov = &rma_ioc;
	msg.rma_iov_count = 1;
	msg.datatype = datatype;
	msg.op = op;
	msg.context = context;
	msg.data = 0;

	return rxr_atomic_readwritemsg(ep, &msg, resultv, result_desc, result_count, 0);
}

static ssize_t
rxr_atomic_readwrite(struct fid_ep *ep,
		     const void *buf, size_t count, void *desc,
		     void *result, void *result_desc,
		     fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		     enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct fi_ioc ioc, resp_ioc;

	ioc.addr = (void *)buf;
	ioc.count = count;
	resp_ioc.addr = result;
	resp_ioc.count = count;

	return rxr_atomic_readwritev(ep, &ioc, &desc, 1,
				     &resp_ioc, &result_desc, 1,
				     dest_addr, addr, key,
				     datatype, op, context);
}

static ssize_t
rxr_atomic_compwritemsg(struct fid_ep *ep,
			const struct fi_msg_atomic *msg,
			const struct fi_ioc *comparev, void **compare_desc, size_t compare_count,
			struct fi_ioc *resultv, void **result_desc, size_t result_count,
			uint64_t flags)
{
	struct rxr_ep *rxr_ep;
	struct rxr_peer *peer;
	struct fi_msg_atomic shm_msg;
	struct fi_rma_ioc rma_iov[RXR_IOV_LIMIT];
	struct rxr_atomic_ex atomic_ex;
	size_t datatype_size = ofi_datatype_size(msg->datatype);

	FI_DBG(&rxr_prov, FI_LOG_EP_DATA,
	       "%s: iov_len: %lu flags: %lx\n",
	       __func__, ofi_total_ioc_cnt(msg->msg_iov, msg->iov_count), flags);

	rxr_ep = container_of(ep, struct rxr_ep, util_ep.ep_fid.fid);
	peer = rxr_ep_get_peer(rxr_ep, msg->addr);
	if (peer->is_local) {
		assert(rxr_ep->use_shm);
		rxr_atomic_copy_shm_msg(&shm_msg, msg, rma_iov);
		shm_msg.addr = peer->shm_fiaddr;
		return fi_compare_atomicmsg(rxr_ep->shm_ep, &shm_msg,
					    comparev, compare_desc, compare_count,
					    resultv, result_desc, result_count,
					    flags);
	}

	ofi_ioc_to_iov(resultv, atomic_ex.resp_iov, result_count, datatype_size);
	atomic_ex.resp_iov_count = result_count;

	ofi_ioc_to_iov(comparev, atomic_ex.comp_iov, compare_count, datatype_size);
	atomic_ex.comp_iov_count = compare_count;

	return rxr_atomic_generic_efa(rxr_ep, msg, &atomic_ex, ofi_op_atomic_compare, flags);
}

static ssize_t
rxr_atomic_compwritev(struct fid_ep *ep,
		      const struct fi_ioc *iov, void **desc, size_t count,
		      const struct fi_ioc *comparev, void **compare_desc, size_t compare_count,
		      struct fi_ioc *resultv, void **result_desc, size_t result_count,
		      fi_addr_t dest_addr, uint64_t rma_addr, uint64_t rma_key,
		      enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct fi_msg_atomic msg;
	struct fi_rma_ioc rma_ioc;

	rma_ioc.addr = rma_addr;
	rma_ioc.count = ofi_total_ioc_cnt(iov, count);
	rma_ioc.key = rma_key;

	msg.msg_iov = iov;
	msg.iov_count = count; 
	msg.desc = desc;
	msg.addr = dest_addr;
	msg.rma_iov = &rma_ioc;
	msg.rma_iov_count = 1;
	msg.datatype = datatype;
	msg.op = op;
	msg.context = context;
	msg.data = 0;

	return rxr_atomic_compwritemsg(ep, &msg,
				       comparev, compare_desc, compare_count,
				       resultv, result_desc, result_count,
				       0);
}

static ssize_t
rxr_atomic_compwrite(struct fid_ep *ep,
		     const void *buf, size_t count, void *desc,
		     const void *compare, void *compare_desc,
		     void *result, void *result_desc,
		     fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		     enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct fi_ioc ioc, resp_ioc, comp_ioc;

	ioc.addr = (void *)buf;
	ioc.count = count;
	resp_ioc.addr = result;
	resp_ioc.count = count;
	comp_ioc.addr = (void *)compare;
	comp_ioc.count = count;

	return rxr_atomic_compwritev(ep, &ioc, &desc, 1,
				     &comp_ioc, &compare_desc, 1,
				     &resp_ioc, &result_desc, 1,
				     dest_addr, addr, key,
				     datatype, op, context);
}

int rxr_query_atomic(struct fid_domain *domain,
		     enum fi_datatype datatype, enum fi_op op,
		     struct fi_atomic_attr *attr, uint64_t flags)
{
	struct rxr_domain *rxr_domain;
	int ret;
	size_t max_atomic_size;

	if (flags & FI_TAGGED) {
		FI_WARN(&rxr_prov, FI_LOG_EP_CTRL,
			"tagged atomic op not supported\n");
		return -FI_EINVAL;
	}

	ret = ofi_atomic_valid(&rxr_prov, datatype, op, flags);
	if (ret || !attr)
		return ret;

	rxr_domain = container_of(domain, struct rxr_domain,
				  util_domain.domain_fid);

	max_atomic_size = rxr_domain->mtu_size - sizeof(struct rxr_rta_hdr)
			  - rxr_domain->addrlen
			  - RXR_IOV_LIMIT * sizeof(struct fi_rma_iov);

	if (flags & FI_COMPARE_ATOMIC)
		max_atomic_size /= 2;

	attr->size = ofi_datatype_size(datatype);
	attr->count = max_atomic_size / attr->size;
	return 0;
}

static int rxr_atomic_valid(struct fid_ep *ep_fid, enum fi_datatype datatype,
			    enum fi_op op, uint64_t flags, size_t *count)
{
	struct util_ep *ep;
	struct fi_atomic_attr attr;
	int ret;

	ep = container_of(ep_fid, struct util_ep, ep_fid);
	ret = rxr_query_atomic(&ep->domain->domain_fid,
			       datatype, op, &attr, flags);
	if (!ret)
		*count = attr.count;

	return ret;
}

static int rxr_atomic_write_valid(struct fid_ep *ep, enum fi_datatype datatype,
				  enum fi_op op, size_t *count)
{
	return rxr_atomic_valid(ep, datatype, op, 0, count);
}

static int rxr_atomic_readwrite_valid(struct fid_ep *ep,
				      enum fi_datatype datatype, enum fi_op op,
				      size_t *count)
{
	return rxr_atomic_valid(ep, datatype, op, FI_FETCH_ATOMIC, count);
}

static int rxr_atomic_compwrite_valid(struct fid_ep *ep,
				      enum fi_datatype datatype, enum fi_op op,
				      size_t *count)
{
	return rxr_atomic_valid(ep, datatype, op, FI_COMPARE_ATOMIC, count);
}

struct fi_ops_atomic rxr_ops_atomic = {
	.size = sizeof(struct fi_ops_atomic),
	.write = rxr_atomic_write,
	.writev = rxr_atomic_writev,
	.writemsg = rxr_atomic_writemsg,
	.inject = rxr_atomic_inject,
	.readwrite = rxr_atomic_readwrite,
	.readwritev = rxr_atomic_readwritev,
	.readwritemsg = rxr_atomic_readwritemsg,
	.compwrite = rxr_atomic_compwrite,
	.compwritev = rxr_atomic_compwritev,
	.compwritemsg = rxr_atomic_compwritemsg,
	.writevalid = rxr_atomic_write_valid,
	.readwritevalid = rxr_atomic_readwrite_valid,
	.compwritevalid = rxr_atomic_compwrite_valid,
};

