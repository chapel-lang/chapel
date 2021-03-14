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
#include "rxd.h"

static struct rxd_x_entry *rxd_tx_entry_init_atomic(struct rxd_ep *ep, fi_addr_t addr,
			uint32_t op, const struct iovec *iov, size_t iov_count,
			uint64_t data, uint32_t flags, void *context,
			const struct fi_rma_iov *rma_iov, size_t rma_count,
			const struct iovec *res_iov, size_t res_count,
			const struct iovec *comp_iov, size_t comp_count,
			enum fi_datatype datatype, enum fi_op atomic_op)
{
	struct rxd_x_entry *tx_entry;
	struct rxd_domain *rxd_domain = rxd_ep_domain(ep);
	struct rxd_base_hdr *base_hdr;
	size_t max_inline, len;
	void *ptr;

	OFI_UNUSED(len);

	tx_entry = rxd_tx_entry_init_common(ep, addr, op, iov, iov_count, 0,
					    data, flags, context, &base_hdr, &ptr);
	if (!tx_entry)
		return NULL;

	if (res_count) {
		tx_entry->res_count = res_count;
		memcpy(&tx_entry->res_iov[0], res_iov, sizeof(*res_iov) * res_count);
	}

	max_inline = rxd_domain->max_inline_msg;
	rxd_check_init_cq_data(&ptr, tx_entry, &max_inline);

	if (rma_count > 1 || tx_entry->cq_entry.flags & FI_READ) {
		max_inline -= sizeof(struct rxd_sar_hdr);
		rxd_init_sar_hdr(&ptr, tx_entry, rma_count);
	} else {
		tx_entry->flags |= RXD_INLINE;
		base_hdr->flags = tx_entry->flags;
		tx_entry->num_segs = 1;
	}

	rxd_init_rma_hdr(&ptr, rma_iov, rma_count);
	rxd_init_atom_hdr(&ptr, datatype, atomic_op);
	max_inline -= (sizeof(struct ofi_rma_iov) * rma_count) +
		      sizeof(struct rxd_atom_hdr) ;

	assert(tx_entry->cq_entry.len < max_inline);
	if (atomic_op != FI_ATOMIC_READ) {
		tx_entry->bytes_done = rxd_init_msg(&ptr, tx_entry->iov,
				tx_entry->iov_count, tx_entry->cq_entry.len,
				max_inline);
		if (tx_entry->op == RXD_ATOMIC_COMPARE) {
			max_inline /= 2;
			assert(tx_entry->cq_entry.len <= max_inline);
			len = rxd_init_msg(&ptr, comp_iov, comp_count,
					tx_entry->cq_entry.len,
					max_inline);
			assert(len == tx_entry->bytes_done);
		}
	}

	tx_entry->pkt->pkt_size = rxd_pkt_size(ep, base_hdr, ptr);

	return tx_entry;
}

static ssize_t rxd_generic_atomic(struct rxd_ep *rxd_ep,
			const struct fi_ioc *ioc, void **desc, size_t count,
			const struct fi_ioc *compare_ioc, void **compare_desc,
			size_t compare_count, struct fi_ioc *result_ioc,
			void **result_desc, size_t result_count,
			fi_addr_t addr, const struct fi_rma_ioc *rma_ioc,
			size_t rma_count, uint64_t data, enum fi_datatype datatype,
			enum fi_op atomic_op, void *context, uint32_t op,
			uint32_t rxd_flags)
{
	struct rxd_x_entry *tx_entry;
	struct iovec iov[RXD_IOV_LIMIT], res_iov[RXD_IOV_LIMIT], comp_iov[RXD_IOV_LIMIT];
	struct fi_rma_iov rma_iov[RXD_IOV_LIMIT]; 
	fi_addr_t rxd_addr;
	ssize_t ret = -FI_EAGAIN;

	assert(count <= RXD_IOV_LIMIT);
	assert(rma_count <= RXD_IOV_LIMIT);

	ofi_ioc_to_iov(ioc, iov, count, ofi_datatype_size(datatype));

	assert(ofi_total_iov_len(iov, count) <= (op == RXD_ATOMIC_COMPARE) ?
	       rxd_ep_domain(rxd_ep)->max_inline_atom / 2 :
	       rxd_ep_domain(rxd_ep)->max_inline_atom);

	ofi_ioc_to_iov(result_ioc, res_iov, result_count, ofi_datatype_size(datatype));
	ofi_ioc_to_iov(compare_ioc, comp_iov, compare_count, ofi_datatype_size(datatype));
	ofi_rma_ioc_to_iov(rma_ioc, rma_iov, rma_count, ofi_datatype_size(datatype));

	fastlock_acquire(&rxd_ep->util_ep.lock);

	if (ofi_cirque_isfull(rxd_ep->util_ep.tx_cq->cirq))
		goto out;

	rxd_addr = rxd_ep_av(rxd_ep)->fi_addr_table[addr];
	ret = rxd_send_rts_if_needed(rxd_ep, rxd_addr);
	if (ret)
		goto out;

	tx_entry = rxd_tx_entry_init_atomic(rxd_ep, rxd_addr, op, iov, count,
			data, rxd_flags, context, rma_iov, rma_count, res_iov,
			result_count, comp_iov, compare_count, datatype, atomic_op);
	if (!tx_entry)
		goto out;

	if (rxd_ep->peers[rxd_addr].peer_addr != FI_ADDR_UNSPEC)
		(void) rxd_start_xfer(rxd_ep, tx_entry);

out:
	fastlock_release(&rxd_ep->util_ep.lock);
	return ret;
}

static ssize_t rxd_atomic_writemsg(struct fid_ep *ep_fid,
			const struct fi_msg_atomic *msg, uint64_t flags)
{
	struct rxd_ep *ep;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	return rxd_generic_atomic(ep, msg->msg_iov, msg->desc, msg->iov_count,
				  NULL, NULL, 0, NULL, NULL, 0, msg->addr,
				  msg->rma_iov, msg->rma_iov_count, msg->data,
				  msg->datatype, msg->op, msg->context,
				  RXD_ATOMIC, rxd_tx_flags(flags |
				  ep->util_ep.tx_msg_flags));
}

static ssize_t rxd_atomic_writev(struct fid_ep *ep_fid,
			const struct fi_ioc *iov, void **desc, size_t count,
			fi_addr_t dest_addr, uint64_t addr, uint64_t key,
			enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct rxd_ep *ep;
	struct fi_rma_ioc rma_iov;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	rma_iov.addr = addr;
	rma_iov.count = ofi_total_ioc_cnt(iov, count);
	rma_iov.key = key;

	return rxd_generic_atomic(ep, iov, desc, count, NULL, NULL, 0, NULL,
				  NULL, 0, dest_addr, &rma_iov, 1, 0, datatype,
				  op, context, RXD_ATOMIC,
				  ep->tx_flags);
}

static ssize_t rxd_atomic_write(struct fid_ep *ep_fid, const void *buf, size_t count,
			void *desc, fi_addr_t dest_addr, uint64_t addr,
			uint64_t key, enum fi_datatype datatype, enum fi_op op,
			void *context)
{
	struct fi_ioc iov;
	struct fi_rma_ioc rma_iov;
	struct rxd_ep *ep;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	iov.addr = (void *) buf;
	iov.count = count;

	rma_iov.addr = addr;
	rma_iov.count = count;
	rma_iov.key = key;

	return rxd_generic_atomic(ep, &iov, &desc, 1, NULL, NULL, 0, NULL, NULL, 0,
				  dest_addr, &rma_iov, 1, 0, datatype, op, context,
				  RXD_ATOMIC, ep->tx_flags);
}

static ssize_t rxd_atomic_inject(struct fid_ep *ep_fid, const void *buf,
			size_t count, fi_addr_t dest_addr, uint64_t addr,
			uint64_t key, enum fi_datatype datatype, enum fi_op op)
{
	struct rxd_ep *rxd_ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);
	struct rxd_x_entry *tx_entry;
	struct iovec iov;
	struct fi_rma_iov rma_iov; 
	fi_addr_t rxd_addr;
	ssize_t ret = -FI_EAGAIN;

	iov.iov_base = (void *) buf;
	iov.iov_len = count * ofi_datatype_size(datatype);
	assert(iov.iov_len <= rxd_ep_domain(rxd_ep)->max_inline_atom);

	rma_iov.addr = addr;
	rma_iov.len = count * ofi_datatype_size(datatype);
	rma_iov.key = key;

	fastlock_acquire(&rxd_ep->util_ep.lock);

	if (ofi_cirque_isfull(rxd_ep->util_ep.tx_cq->cirq))
		goto out;

	rxd_addr = rxd_ep_av(rxd_ep)->fi_addr_table[dest_addr];
	ret = rxd_send_rts_if_needed(rxd_ep, rxd_addr);
	if (ret)
		goto out;

	tx_entry = rxd_tx_entry_init_atomic(rxd_ep, rxd_addr, RXD_ATOMIC, &iov, 1,
			0, RXD_INJECT | RXD_NO_TX_COMP, NULL, &rma_iov, 1, NULL,
			0, NULL, 0, datatype, op);
	if (!tx_entry)
		goto out;

	if (rxd_ep->peers[rxd_addr].peer_addr == FI_ADDR_UNSPEC)
		goto out;

	(void) rxd_start_xfer(rxd_ep, tx_entry);
out:
	fastlock_release(&rxd_ep->util_ep.lock);
	return ret;
}

static ssize_t rxd_atomic_readwritemsg(struct fid_ep *ep_fid,
			const struct fi_msg_atomic *msg, struct fi_ioc *resultv,
			void **result_desc, size_t result_count, uint64_t flags)
{
	struct rxd_ep *ep;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	return rxd_generic_atomic(ep, msg->msg_iov, msg->desc, msg->iov_count,
				  NULL, NULL, 0, resultv, result_desc,
				  result_count, msg->addr,
				  msg->rma_iov, msg->rma_iov_count, msg->data,
				  msg->datatype, msg->op, msg->context,
				  RXD_ATOMIC_FETCH, rxd_tx_flags(flags |
				  ep->util_ep.tx_msg_flags));
}

static ssize_t rxd_atomic_readwritev(struct fid_ep *ep_fid,
			const struct fi_ioc *iov, void **desc, size_t count,
			struct fi_ioc *resultv, void **result_desc,
			size_t result_count, fi_addr_t dest_addr, uint64_t addr,
			uint64_t key, enum fi_datatype datatype, enum fi_op op,
			void *context)
{
	struct fi_rma_ioc rma_iov;
	struct rxd_ep *ep;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	rma_iov.addr = addr;
	rma_iov.count = ofi_total_ioc_cnt(iov, count);
	rma_iov.key = key;

	return rxd_generic_atomic(ep, iov, desc, count, NULL, NULL, 0, resultv,
				  result_desc, result_count, dest_addr,
				  &rma_iov, 1, 0, datatype, op, context,
				  RXD_ATOMIC_FETCH, ep->tx_flags);
}

static ssize_t rxd_atomic_readwrite(struct fid_ep *ep_fid, const void *buf,
			size_t count, void *desc, void *result,
			void *result_desc, fi_addr_t dest_addr, uint64_t addr,
			uint64_t key, enum fi_datatype datatype, enum fi_op op,
			void *context)
{
	struct fi_ioc iov, resultv;
	struct fi_rma_ioc rma_iov;
	struct rxd_ep *ep;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	iov.addr = (void *) buf;
	iov.count = count;

	resultv.addr = result;
	resultv.count = count;

	rma_iov.addr = addr;
	rma_iov.count = count;
	rma_iov.key = key;

	return rxd_generic_atomic(ep, &iov, &desc, 1, NULL, NULL, 0, &resultv,
				  &result_desc, 1, dest_addr, &rma_iov, 1, 0,
				  datatype, op, context, RXD_ATOMIC_FETCH,
				  ep->tx_flags);
}

static ssize_t rxd_atomic_compwritemsg(struct fid_ep *ep_fid,
			const struct fi_msg_atomic *msg,
			const struct fi_ioc *comparev, void **compare_desc,
			size_t compare_count, struct fi_ioc *resultv,
			void **result_desc, size_t result_count, uint64_t flags)
{
	struct rxd_ep *ep;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	return rxd_generic_atomic(ep, msg->msg_iov, msg->desc, msg->iov_count,
				  comparev, compare_desc, compare_count,
				  resultv, result_desc,
				  result_count, msg->addr,
				  msg->rma_iov, msg->rma_iov_count, msg->data,
				  msg->datatype, msg->op, msg->context,
				  RXD_ATOMIC_COMPARE, rxd_tx_flags(flags |
				  ep->util_ep.tx_msg_flags));
}

static ssize_t rxd_atomic_compwritev(struct fid_ep *ep_fid,
			const struct fi_ioc *iov, void **desc, size_t count,
			const struct fi_ioc *comparev, void **compare_desc,
			size_t compare_count, struct fi_ioc *resultv,
			void **result_desc, size_t result_count,
			fi_addr_t dest_addr, uint64_t addr, uint64_t key,
			enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct fi_rma_ioc rma_iov;
	struct rxd_ep *ep;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	rma_iov.addr = addr;
	rma_iov.count = ofi_total_ioc_cnt(iov, count);
	rma_iov.key = key;

	return rxd_generic_atomic(ep, iov, desc, count, comparev, compare_desc,
				  compare_count, resultv, result_desc,
				  result_count, dest_addr, &rma_iov, 1, 0,
				  datatype, op, context, RXD_ATOMIC_COMPARE,
				  ep->tx_flags);
}

static ssize_t rxd_atomic_compwrite(struct fid_ep *ep_fid, const void *buf,
			size_t count, void *desc, const void *compare,
			void *compare_desc, void *result, void *result_desc,
			fi_addr_t dest_addr, uint64_t addr, uint64_t key,
			enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct fi_ioc iov, resultv, comparev;
	struct fi_rma_ioc rma_iov;
	struct rxd_ep *ep;

	ep = container_of(ep_fid, struct rxd_ep, util_ep.ep_fid.fid);

	iov.addr = (void *) buf;
	iov.count = count;

	resultv.addr = result;
	resultv.count = count;

	comparev.addr = (void *) compare;
	comparev.count = count;

	rma_iov.addr = addr;
	rma_iov.count = count;
	rma_iov.key = key;

	return rxd_generic_atomic(ep, &iov, &desc, 1, &comparev, &compare_desc,
				  1, &resultv, &result_desc, 1, dest_addr,
				  &rma_iov, 1, 0, datatype, op, context,
				  RXD_ATOMIC_COMPARE, ep->tx_flags);
}

int rxd_query_atomic(struct fid_domain *domain, enum fi_datatype datatype,
		     enum fi_op op, struct fi_atomic_attr *attr, uint64_t flags)
{
	struct rxd_domain *rxd_domain;
	int ret;
	size_t total_size;

	if (flags & FI_TAGGED) {
		FI_WARN(&rxd_prov, FI_LOG_EP_CTRL,
			"tagged atomic op not supported\n");
		return -FI_EINVAL;
	}

	ret = ofi_atomic_valid(&rxd_prov, datatype, op, flags);
	if (ret || !attr)
		return ret;

	rxd_domain = container_of(domain, struct rxd_domain,
				  util_domain.domain_fid);
	attr->size = ofi_datatype_size(datatype);

	total_size = (flags & FI_COMPARE_ATOMIC) ?  rxd_domain->max_inline_atom / 2 :
		      rxd_domain->max_inline_atom;
	attr->count = total_size / attr->size;

	return ret;
}

static int rxd_atomic_valid(struct fid_ep *ep, enum fi_datatype datatype,
			    enum fi_op op, size_t *count)
{
	struct fi_atomic_attr attr;
	int ret;

	ret = rxd_query_atomic(&(container_of(ep,
			struct util_ep, ep_fid))->domain->domain_fid,
			datatype, op, &attr, 0);
	if (!ret)
		*count = attr.count;

	return ret;
}

static int rxd_atomic_fetch_valid(struct fid_ep *ep, enum fi_datatype datatype,
				  enum fi_op op, size_t *count)
{
	struct fi_atomic_attr attr;
	int ret;

	ret = rxd_query_atomic(&(container_of(ep,
			struct util_ep, ep_fid))->domain->domain_fid,
			datatype, op, &attr, FI_FETCH_ATOMIC);
	if (!ret)
		*count = attr.count;

	return ret;
}

static int rxd_atomic_comp_valid(struct fid_ep *ep, enum fi_datatype datatype,
				 enum fi_op op, size_t *count)
{
	struct fi_atomic_attr attr;
	int ret;

	ret = rxd_query_atomic(&(container_of(ep,
			struct util_ep, ep_fid))->domain->domain_fid,
			datatype, op, &attr, FI_COMPARE_ATOMIC);
	if (!ret)
		*count = attr.count;

	return ret;
}

struct fi_ops_atomic rxd_ops_atomic = {
	.size = sizeof(struct fi_ops_atomic),
	.write = rxd_atomic_write,
	.writev = rxd_atomic_writev,
	.writemsg = rxd_atomic_writemsg,
	.inject = rxd_atomic_inject,
	.readwrite = rxd_atomic_readwrite,
	.readwritev = rxd_atomic_readwritev,
	.readwritemsg = rxd_atomic_readwritemsg,
	.compwrite = rxd_atomic_compwrite,
	.compwritev = rxd_atomic_compwritev,
	.compwritemsg = rxd_atomic_compwritemsg,
	.writevalid = rxd_atomic_valid,
	.readwritevalid = rxd_atomic_fetch_valid,
	.compwritevalid = rxd_atomic_comp_valid,
};
