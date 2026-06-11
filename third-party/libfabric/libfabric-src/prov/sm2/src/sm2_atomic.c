/*
 * Copyright (c) Intel Corporation. All rights reserved.
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
#include "sm2_fifo.h"

static inline void
sm2_atomic_format(struct sm2_xfer_entry *xfer_entry, uint8_t datatype,
		  uint8_t atomic_op, const struct fi_rma_ioc *rma_ioc,
		  size_t rma_ioc_count, const struct iovec *resultv,
		  size_t result_count, const struct iovec *iov,
		  size_t iov_count, const struct iovec *compare_iov,
		  size_t compare_count)
{
	struct sm2_atomic_entry *atomic_entry =
		(struct sm2_atomic_entry *) xfer_entry->user_data;
	size_t comp_size;

	atomic_entry->atomic_hdr.datatype = datatype;
	atomic_entry->atomic_hdr.atomic_op = atomic_op;

	atomic_entry->atomic_hdr.rma_ioc_count = rma_ioc_count;
	memcpy(atomic_entry->atomic_hdr.rma_ioc, rma_ioc,
	       sizeof(*rma_ioc) * rma_ioc_count);

	if (xfer_entry->hdr.op == ofi_op_atomic_fetch ||
	    xfer_entry->hdr.op == ofi_op_atomic_compare) {
		atomic_entry->atomic_hdr.result_iov_count = result_count;
		memcpy(atomic_entry->atomic_hdr.result_iov, resultv,
		       sizeof(*resultv) * result_count);
	}

	switch (xfer_entry->hdr.op) {
	case ofi_op_atomic:
		xfer_entry->hdr.size = ofi_copy_from_iov(
			atomic_entry->atomic_data.data, SM2_ATOMIC_INJECT_SIZE,
			iov, iov_count, 0);
		break;
	case ofi_op_atomic_fetch:
		if (xfer_entry->hdr.op != FI_ATOMIC_READ)
			xfer_entry->hdr.size = ofi_copy_from_iov(
				atomic_entry->atomic_data.data,
				SM2_ATOMIC_INJECT_SIZE, iov, iov_count, 0);
		break;
	case ofi_op_atomic_compare:
		xfer_entry->hdr.size = ofi_copy_from_iov(
			atomic_entry->atomic_data.buf,
			SM2_ATOMIC_COMP_INJECT_SIZE, iov, iov_count, 0);
		comp_size = ofi_copy_from_iov(atomic_entry->atomic_data.comp,
					      SM2_ATOMIC_COMP_INJECT_SIZE,
					      compare_iov, compare_count, 0);
		if (comp_size != xfer_entry->hdr.size)
			FI_WARN(&sm2_prov, FI_LOG_EP_CTRL,
				"atomic and compare buffer size mismatch\n");
		break;
	default:
		assert(0);
		break;
	}
}

static inline ssize_t
sm2_do_atomic_inject(struct sm2_ep *ep, int64_t peer_gid, uint32_t op,
		     uint64_t op_flags, uint8_t datatype, uint8_t atomic_op,
		     const struct iovec *iov, size_t iov_count,
		     const struct fi_rma_ioc *rma_ioc, size_t rma_ioc_count,
		     const struct iovec *compare_iov, size_t compare_count,
		     const struct iovec *result_iov, size_t result_count,
		     size_t total_len, void *context, uint16_t proto_flags)
{
	struct sm2_xfer_entry *xfer_entry;
	size_t ret;

	ret = sm2_pop_xfer_entry(ep, &xfer_entry);
	if (ret)
		return ret;

	xfer_entry->hdr.proto = sm2_proto_inject;

	sm2_generic_format(xfer_entry, ep->gid, op, 0, 0, op_flags, context);
	xfer_entry->hdr.proto_flags |= proto_flags;
	sm2_atomic_format(xfer_entry, datatype, atomic_op, rma_ioc,
			  rma_ioc_count, result_iov, result_count, iov,
			  iov_count, compare_iov, compare_count);
	sm2_fifo_write(ep, peer_gid, xfer_entry);

	return FI_SUCCESS;
}

static inline ssize_t sm2_generic_atomic(
	struct sm2_ep *ep, const struct fi_ioc *ioc, void **desc,
	size_t iov_count, const struct fi_ioc *compare_ioc, void **compare_desc,
	size_t compare_count, struct fi_ioc *result_ioc, void **result_desc,
	size_t result_count, fi_addr_t addr, const struct fi_rma_ioc *rma_ioc,
	size_t rma_ioc_count, enum fi_datatype datatype, enum fi_op atomic_op,
	void *context, uint32_t op, uint64_t op_flags)
{
	struct iovec iov[SM2_IOV_LIMIT];
	struct iovec compare_iov[SM2_IOV_LIMIT];
	struct iovec result_iov[SM2_IOV_LIMIT];
	sm2_gid_t peer_gid;
	ssize_t ret = 0;
	uint16_t proto_flags = 0;
	size_t total_len;

	assert(iov_count <= SM2_IOV_LIMIT);
	assert(result_count <= SM2_IOV_LIMIT);
	assert(compare_count <= SM2_IOV_LIMIT);
	assert(rma_ioc_count <= SM2_IOV_LIMIT);

	ret = sm2_verify_peer(ep, addr, &peer_gid);
	if (ret < 0)
		return ret;

	total_len =
		ofi_datatype_size(datatype) * ofi_total_ioc_cnt(ioc, iov_count);

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
		proto_flags |= SM2_RMA_REQ;
		/* fall through */
	case ofi_op_atomic:
		if (atomic_op != FI_ATOMIC_READ) {
			assert(ioc);
			ofi_ioc_to_iov(ioc, iov, iov_count,
				       ofi_datatype_size(datatype));
		} else {
			iov_count = 0;
		}
		break;
	default:
		FI_WARN(&sm2_prov, FI_LOG_EP_CTRL, "Unrecognized atomic op\n");
		return -FI_ENOSYS;
	}

	ofi_genlock_lock(&ep->util_ep.lock);

	ret = sm2_do_atomic_inject(
		ep, peer_gid, op, op_flags, datatype, atomic_op, iov, iov_count,
		rma_ioc, rma_ioc_count, compare_iov, compare_count, result_iov,
		result_count, total_len, context, proto_flags);

	if (ret) {
		FI_WARN(&sm2_prov, FI_LOG_EP_CTRL, "Atomic operation failed\n");
		goto out;
	}

	/* Generate immediate send completion for ofi_op_atomic unless
	 * FI_DELIVERY_COMPLETE are set */
	if (op == ofi_op_atomic) {
		if (!(op_flags & FI_DELIVERY_COMPLETE)) {
			ret = sm2_complete_tx(ep, context, op, op_flags);
			if (ret)
				FI_WARN(&sm2_prov, FI_LOG_EP_CTRL,
					"unable to process tx completion\n");
		}
	}

out:
	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t sm2_atomic_writemsg(struct fid_ep *ep_fid,
				   const struct fi_msg_atomic *msg,
				   uint64_t flags)
{
	struct sm2_ep *ep;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	return sm2_generic_atomic(
		ep, msg->msg_iov, msg->desc, msg->iov_count, NULL, NULL, 0,
		NULL, NULL, 0, msg->addr, msg->rma_iov, msg->rma_iov_count,
		msg->datatype, msg->op, msg->context, ofi_op_atomic,
		flags | ep->util_ep.tx_msg_flags);
}

static ssize_t sm2_atomic_writev(struct fid_ep *ep_fid,
				 const struct fi_ioc *iov, void **desc,
				 size_t count, fi_addr_t dest_addr,
				 uint64_t addr, uint64_t key,
				 enum fi_datatype datatype, enum fi_op op,
				 void *context)
{
	struct sm2_ep *ep;
	struct fi_rma_ioc rma_iov;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	rma_iov.addr = addr;
	rma_iov.count = ofi_total_ioc_cnt(iov, count);
	rma_iov.key = key;

	return sm2_generic_atomic(ep, iov, desc, count, NULL, NULL, 0, NULL,
				  NULL, 0, dest_addr, &rma_iov, 1, datatype, op,
				  context, ofi_op_atomic, sm2_ep_tx_flags(ep));
}

static ssize_t sm2_atomic_write(struct fid_ep *ep_fid, const void *buf,
				size_t count, void *desc, fi_addr_t dest_addr,
				uint64_t addr, uint64_t key,
				enum fi_datatype datatype, enum fi_op op,
				void *context)
{
	struct sm2_ep *ep;
	struct fi_ioc iov;
	struct fi_rma_ioc rma_iov;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	iov.addr = (void *) buf;
	iov.count = count;

	rma_iov.addr = addr;
	rma_iov.count = count;
	rma_iov.key = key;

	return sm2_generic_atomic(ep, &iov, &desc, 1, NULL, NULL, 0, NULL, NULL,
				  0, dest_addr, &rma_iov, 1, datatype, op,
				  context, ofi_op_atomic, sm2_ep_tx_flags(ep));
}

static ssize_t sm2_atomic_inject(struct fid_ep *ep_fid, const void *buf,
				 size_t count, fi_addr_t dest_addr,
				 uint64_t addr, uint64_t key,
				 enum fi_datatype datatype,
				 enum fi_op atomic_op)
{
	struct sm2_ep *ep =
		container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);
	struct iovec iov;
	struct fi_rma_ioc rma_ioc;
	sm2_gid_t peer_gid;
	ssize_t ret = 0;
	size_t total_len;

	ret = sm2_verify_peer(ep, dest_addr, &peer_gid);
	if (ret < 0)
		return ret;

	total_len = count * ofi_datatype_size(datatype);
	assert(total_len <= SM2_ATOMIC_INJECT_SIZE);

	iov.iov_base = (void *) buf;
	iov.iov_len = total_len;

	rma_ioc.addr = addr;
	rma_ioc.count = count;
	rma_ioc.key = key;

	ofi_genlock_lock(&ep->util_ep.lock);

	ret = sm2_do_atomic_inject(ep, peer_gid, ofi_op_atomic, 0, datatype,
				   atomic_op, &iov, 1, &rma_ioc, 1, NULL, 0,
				   NULL, 0, total_len, 0, 0);

	if (!ret)
		ofi_ep_peer_tx_cntr_inc(&ep->util_ep, ofi_op_atomic);

	ofi_genlock_unlock(&ep->util_ep.lock);
	return ret;
}

static ssize_t sm2_atomic_readwritemsg(struct fid_ep *ep_fid,
				       const struct fi_msg_atomic *msg,
				       struct fi_ioc *resultv,
				       void **result_desc, size_t result_count,
				       uint64_t flags)
{
	struct sm2_ep *ep;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	return sm2_generic_atomic(
		ep, msg->msg_iov, msg->desc, msg->iov_count, NULL, NULL, 0,
		resultv, result_desc, result_count, msg->addr, msg->rma_iov,
		msg->rma_iov_count, msg->datatype, msg->op, msg->context,
		ofi_op_atomic_fetch, flags | ep->util_ep.tx_msg_flags);
}

static ssize_t sm2_atomic_readwritev(struct fid_ep *ep_fid,
				     const struct fi_ioc *iov, void **desc,
				     size_t count, struct fi_ioc *resultv,
				     void **result_desc, size_t result_count,
				     fi_addr_t dest_addr, uint64_t addr,
				     uint64_t key, enum fi_datatype datatype,
				     enum fi_op op, void *context)
{
	struct sm2_ep *ep;
	struct fi_rma_ioc rma_iov;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	rma_iov.addr = addr;
	rma_iov.count = ofi_total_ioc_cnt(iov, count);
	rma_iov.key = key;

	return sm2_generic_atomic(ep, iov, desc, count, NULL, NULL, 0, resultv,
				  result_desc, result_count, dest_addr,
				  &rma_iov, 1, datatype, op, context,
				  ofi_op_atomic_fetch, sm2_ep_tx_flags(ep));
}

static ssize_t sm2_atomic_readwrite(struct fid_ep *ep_fid, const void *buf,
				    size_t count, void *desc, void *result,
				    void *result_desc, fi_addr_t dest_addr,
				    uint64_t addr, uint64_t key,
				    enum fi_datatype datatype, enum fi_op op,
				    void *context)
{
	struct sm2_ep *ep;
	struct fi_ioc iov, resultv;
	struct fi_rma_ioc rma_iov;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	iov.addr = (void *) buf;
	iov.count = count;

	resultv.addr = result;
	resultv.count = count;

	rma_iov.addr = addr;
	rma_iov.count = count;
	rma_iov.key = key;

	return sm2_generic_atomic(ep, &iov, &desc, 1, NULL, NULL, 0, &resultv,
				  &result_desc, 1, dest_addr, &rma_iov, 1,
				  datatype, op, context, ofi_op_atomic_fetch,
				  sm2_ep_tx_flags(ep));
}

static ssize_t
sm2_atomic_compwritemsg(struct fid_ep *ep_fid, const struct fi_msg_atomic *msg,
			const struct fi_ioc *comparev, void **compare_desc,
			size_t compare_count, struct fi_ioc *resultv,
			void **result_desc, size_t result_count, uint64_t flags)
{
	struct sm2_ep *ep;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	return sm2_generic_atomic(
		ep, msg->msg_iov, msg->desc, msg->iov_count, comparev,
		compare_desc, compare_count, resultv, result_desc, result_count,
		msg->addr, msg->rma_iov, msg->rma_iov_count, msg->datatype,
		msg->op, msg->context, ofi_op_atomic_compare,
		flags | ep->util_ep.tx_msg_flags);
}

static ssize_t sm2_atomic_compwritev(
	struct fid_ep *ep_fid, const struct fi_ioc *iov, void **desc,
	size_t count, const struct fi_ioc *comparev, void **compare_desc,
	size_t compare_count, struct fi_ioc *resultv, void **result_desc,
	size_t result_count, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
	enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct sm2_ep *ep;
	struct fi_rma_ioc rma_iov;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	rma_iov.addr = addr;
	rma_iov.count = ofi_total_ioc_cnt(iov, count);
	rma_iov.key = key;

	return sm2_generic_atomic(ep, iov, desc, count, comparev, compare_desc,
				  compare_count, resultv, result_desc,
				  result_count, dest_addr, &rma_iov, 1,
				  datatype, op, context, ofi_op_atomic_compare,
				  sm2_ep_tx_flags(ep));
}

static ssize_t sm2_atomic_compwrite(struct fid_ep *ep_fid, const void *buf,
				    size_t count, void *desc,
				    const void *compare, void *compare_desc,
				    void *result, void *result_desc,
				    fi_addr_t dest_addr, uint64_t addr,
				    uint64_t key, enum fi_datatype datatype,
				    enum fi_op op, void *context)
{
	struct sm2_ep *ep;
	struct fi_ioc iov, resultv, comparev;
	struct fi_rma_ioc rma_iov;

	ep = container_of(ep_fid, struct sm2_ep, util_ep.ep_fid.fid);

	iov.addr = (void *) buf;
	iov.count = count;

	resultv.addr = result;
	resultv.count = count;

	comparev.addr = (void *) compare;
	comparev.count = count;

	rma_iov.addr = addr;
	rma_iov.count = count;
	rma_iov.key = key;

	return sm2_generic_atomic(ep, &iov, &desc, 1, &comparev, &compare_desc,
				  1, &resultv, &result_desc, 1, dest_addr,
				  &rma_iov, 1, datatype, op, context,
				  ofi_op_atomic_compare, sm2_ep_tx_flags(ep));
}

int sm2_query_atomic(struct fid_domain *domain, enum fi_datatype datatype,
		     enum fi_op op, struct fi_atomic_attr *attr, uint64_t flags)
{
	int ret;
	size_t total_size;

	if (flags & FI_TAGGED) {
		FI_WARN(&sm2_prov, FI_LOG_EP_CTRL,
			"tagged atomic op not supported\n");
		return -FI_EOPNOTSUPP;
	}

	if ((datatype == FI_INT128) || (datatype == FI_UINT128)) {
		FI_WARN(&sm2_prov, FI_LOG_EP_CTRL,
			"128-bit datatypes not supported\n");
		return -FI_EOPNOTSUPP;
	}

	ret = ofi_atomic_valid(&sm2_prov, datatype, op, flags);
	if (ret || !attr)
		return ret;

	attr->size = ofi_datatype_size(datatype);

	total_size = (flags & FI_COMPARE_ATOMIC) ? SM2_ATOMIC_COMP_INJECT_SIZE :
						   SM2_ATOMIC_INJECT_SIZE;
	attr->count = total_size / attr->size;

	return ret;
}

static int sm2_atomic_valid(struct fid_ep *ep, enum fi_datatype datatype,
			    enum fi_op op, size_t *count)
{
	struct fi_atomic_attr attr;
	int ret;

	ret = sm2_query_atomic(NULL, datatype, op, &attr, 0);

	if (!ret)
		*count = attr.count;

	return ret;
}

static int sm2_atomic_fetch_valid(struct fid_ep *ep, enum fi_datatype datatype,
				  enum fi_op op, size_t *count)
{
	struct fi_atomic_attr attr;
	int ret;

	ret = sm2_query_atomic(NULL, datatype, op, &attr, FI_FETCH_ATOMIC);

	if (!ret)
		*count = attr.count;

	return ret;
}

static int sm2_atomic_comp_valid(struct fid_ep *ep, enum fi_datatype datatype,
				 enum fi_op op, size_t *count)
{
	struct fi_atomic_attr attr;
	int ret;

	ret = sm2_query_atomic(NULL, datatype, op, &attr, FI_COMPARE_ATOMIC);

	if (!ret)
		*count = attr.count;

	return ret;
}

struct fi_ops_atomic sm2_atomic_ops = {
	.size = sizeof(struct fi_ops_atomic),
	.write = sm2_atomic_write,
	.writev = sm2_atomic_writev,
	.writemsg = sm2_atomic_writemsg,
	.inject = sm2_atomic_inject,
	.readwrite = sm2_atomic_readwrite,
	.readwritev = sm2_atomic_readwritev,
	.readwritemsg = sm2_atomic_readwritemsg,
	.compwrite = sm2_atomic_compwrite,
	.compwritev = sm2_atomic_compwritev,
	.compwritemsg = sm2_atomic_compwritemsg,
	.writevalid = sm2_atomic_valid,
	.readwritevalid = sm2_atomic_fetch_valid,
	.compwritevalid = sm2_atomic_comp_valid,
};
