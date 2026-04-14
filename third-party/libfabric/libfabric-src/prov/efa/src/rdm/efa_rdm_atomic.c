/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include <ofi_iov.h>
#include <ofi_atomic.h>
#include "efa.h"
#include "efa_rdm_rma.h"
#include "efa_cntr.h"
#include "efa_rdm_atomic.h"
#include "efa_rdm_pke_cmd.h"

static void efa_rdm_atomic_init_shm_msg(struct efa_rdm_ep *ep, struct fi_msg_atomic *shm_msg,
				    const struct fi_msg_atomic *msg,
				    struct fi_rma_ioc *rma_iov,
				    void **shm_desc)
{
	int i;

	assert(msg->rma_iov_count <= EFA_RDM_IOV_LIMIT);
	memcpy(shm_msg, msg, sizeof(*msg));
	if (!(ep->shm_info->domain_attr->mr_mode & FI_MR_VIRT_ADDR)) {
		memcpy(rma_iov, msg->rma_iov,
		       sizeof(*msg->rma_iov) * msg->rma_iov_count);
		for (i = 0; i < msg->rma_iov_count; i++)
			rma_iov[i].addr = 0;
		shm_msg->rma_iov = rma_iov;
	}

	if (msg->desc) {
		efa_rdm_get_desc_for_shm(msg->iov_count, msg->desc, shm_desc);
		shm_msg->desc = shm_desc;
	} else {
		shm_msg->desc = NULL;
	}
}

static
struct efa_rdm_ope *
efa_rdm_atomic_alloc_txe(struct efa_rdm_ep *efa_rdm_ep,
		      	 struct efa_rdm_peer *peer,
			 const struct fi_msg_atomic *msg_atomic,
			 const struct efa_rdm_atomic_ex *atomic_ex,
			 uint32_t op, uint64_t flags)
{
	struct efa_rdm_ope *txe;
	struct fi_msg msg;
	struct iovec iov[EFA_RDM_IOV_LIMIT];
	size_t datatype_size;

	datatype_size = ofi_datatype_size(msg_atomic->datatype);
	if (OFI_UNLIKELY(!datatype_size)) {
		return NULL;
	}

	txe = ofi_buf_alloc(efa_rdm_ep->ope_pool);
	if (OFI_UNLIKELY(!txe)) {
		EFA_DBG(FI_LOG_EP_CTRL, "TX entries exhausted.\n");
		return NULL;
	}

	dlist_insert_tail(&txe->ep_entry, &efa_rdm_ep->txe_list);

	ofi_ioc_to_iov(msg_atomic->msg_iov, iov, msg_atomic->iov_count, datatype_size);
	msg.addr = msg_atomic->addr;
	msg.msg_iov = iov;
	msg.context = msg_atomic->context;
	msg.iov_count = msg_atomic->iov_count;
	msg.data = msg_atomic->data;
	msg.desc = msg_atomic->desc;
	efa_rdm_txe_construct(txe, efa_rdm_ep, peer, &msg, op, flags);

	assert(msg_atomic->rma_iov_count > 0);
	assert(msg_atomic->rma_iov);
	txe->rma_iov_count = msg_atomic->rma_iov_count;
	ofi_rma_ioc_to_iov(msg_atomic->rma_iov,
			   txe->rma_iov,
			   msg_atomic->rma_iov_count,
			   datatype_size);

	txe->atomic_hdr.atomic_op = msg_atomic->op;
	txe->atomic_hdr.datatype = msg_atomic->datatype;

	if (op == ofi_op_atomic_fetch || op == ofi_op_atomic_compare) {
		assert(atomic_ex);
		memcpy(&txe->atomic_ex, atomic_ex, sizeof(struct efa_rdm_atomic_ex));
	}

	return txe;
}

/**
 * @brief Post atomic operations from give ep and tx entry
 *
 * @param efa_rdm_ep efa rdm ep
 * @param txe tx entry
 * @return ssize_t 0 on success, negative integer on failure
 */
ssize_t efa_rdm_atomic_post_atomic(struct efa_rdm_ep *efa_rdm_ep, struct efa_rdm_ope *txe)
{
	bool delivery_complete_requested;
	static int req_pkt_type_list[] = {
		[ofi_op_atomic] = EFA_RDM_WRITE_RTA_PKT,
		[ofi_op_atomic_fetch] = EFA_RDM_FETCH_RTA_PKT,
		[ofi_op_atomic_compare] = EFA_RDM_COMPARE_RTA_PKT
	};

	delivery_complete_requested = txe->fi_flags & FI_DELIVERY_COMPLETE;
	if (delivery_complete_requested && !(txe->peer->is_local)) {
		/*
		 * Because delivery complete is defined as an extra
		 * feature, the receiver might not support it.
		 * The sender cannot send with FI_DELIVERY_COMPLETE
		 * if the peer is not able to handle it.
		 * If the sender does not know whether the peer
		 * can handle it, it needs to trigger
		 * a handshake packet from the peer.
		 * The handshake packet contains
		 * the information whether the peer
		 * support it or not.
		 */
		if (!efa_rdm_ep->homogeneous_peers) {
			if (!(txe->peer->flags & EFA_RDM_PEER_HANDSHAKE_RECEIVED))
				return efa_rdm_ep_enforce_handshake_for_txe(efa_rdm_ep, txe);

			if (!(txe->peer->is_self) && !efa_rdm_peer_support_delivery_complete(txe->peer))
				return -FI_EOPNOTSUPP;
		}
	}

	if (delivery_complete_requested && txe->op == ofi_op_atomic) {
		return efa_rdm_ope_post_send(txe, EFA_RDM_DC_WRITE_RTA_PKT);
	} else {
		/*
		 * Fetch atomic and compare atomic
		 * support DELIVERY_COMPLETE
		 * by nature
		 */
		return efa_rdm_ope_post_send(txe, req_pkt_type_list[txe->op]);
	}
}

static
ssize_t efa_rdm_atomic_generic_efa(struct efa_rdm_ep *efa_rdm_ep,
			       const struct fi_msg_atomic *msg, struct efa_rdm_peer *peer,
			       const struct efa_rdm_atomic_ex *atomic_ex,
			       uint32_t op, uint64_t flags)
{
	struct efa_rdm_ope *txe;
	ssize_t err;
	struct util_srx_ctx *srx_ctx;

	assert(msg->iov_count <= efa_rdm_ep->base_ep.info->tx_attr->iov_limit);
	efa_perfset_start(efa_rdm_ep, perf_efa_tx);

	srx_ctx = efa_rdm_ep_get_peer_srx_ctx(efa_rdm_ep);

	ofi_genlock_lock(srx_ctx->lock);

	assert(peer);

	if (peer->flags & EFA_RDM_PEER_IN_BACKOFF) {
		err = -FI_EAGAIN;
		goto out;
	}

	txe = efa_rdm_atomic_alloc_txe(efa_rdm_ep, peer, msg, atomic_ex, op, flags);
	if (OFI_UNLIKELY(!txe)) {
		err = -FI_EAGAIN;
		goto out;
	}

	txe->msg_id = (peer->next_msg_id != ~0) ?
			    peer->next_msg_id++ : ++peer->next_msg_id;

	err = efa_rdm_atomic_post_atomic(efa_rdm_ep, txe);

	if (OFI_UNLIKELY(err)) {
		efa_rdm_txe_release(txe);
		peer->next_msg_id--;
	}

out:
	ofi_genlock_unlock(srx_ctx->lock);
	efa_perfset_end(efa_rdm_ep, perf_efa_tx);
	return err;
}

static ssize_t
efa_rdm_atomic_inject(struct fid_ep *ep,
		  const void *buf, size_t count,
		  fi_addr_t dest_addr, uint64_t remote_addr, uint64_t remote_key,
		  enum fi_datatype datatype, enum fi_op op)
{
	struct fi_ioc iov;
	struct fi_rma_ioc rma_iov;
	struct fi_msg_atomic msg;

	struct efa_rdm_ep *efa_rdm_ep;
	struct efa_rdm_peer *peer;
	int err;

	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	err = efa_rdm_ep_cap_check_atomic(efa_rdm_ep);
	if (err)
		return err;
	peer = efa_rdm_ep_get_peer(efa_rdm_ep, dest_addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		if (!(efa_rdm_ep->shm_info->domain_attr->mr_mode & FI_MR_VIRT_ADDR))
			remote_addr = 0;

		return fi_inject_atomic(efa_rdm_ep->shm_ep, buf, count, peer->conn->shm_fi_addr,
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

	return efa_rdm_atomic_generic_efa(efa_rdm_ep, &msg, peer, NULL, ofi_op_atomic,
				      FI_INJECT | EFA_RDM_TXE_NO_COMPLETION);
}

static ssize_t
efa_rdm_atomic_writemsg(struct fid_ep *ep,
		    const struct fi_msg_atomic *msg,
		    uint64_t flags)
{
	struct fi_msg_atomic shm_msg;
	struct efa_rdm_ep *efa_rdm_ep;
	struct efa_rdm_peer *peer;
	struct fi_rma_ioc rma_iov[EFA_RDM_IOV_LIMIT];
	void *shm_desc[EFA_RDM_IOV_LIMIT] = {NULL};
	int err;

	EFA_DBG(FI_LOG_EP_DATA, "iov_len: %lu flags: %lx\n",
		ofi_total_ioc_cnt(msg->msg_iov, msg->iov_count), flags);

	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	err = efa_rdm_ep_cap_check_atomic(efa_rdm_ep);
	if (err)
		return err;

	err = efa_rdm_attempt_to_sync_memops_iov(efa_rdm_ep, (struct iovec *)msg->msg_iov, msg->desc, msg->iov_count);
	if (err)
		return err;

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, msg->addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		efa_rdm_atomic_init_shm_msg(efa_rdm_ep, &shm_msg, msg, rma_iov, shm_desc);
		shm_msg.addr = peer->conn->shm_fi_addr;
		return fi_atomicmsg(efa_rdm_ep->shm_ep, &shm_msg, flags);
	}

	return efa_rdm_atomic_generic_efa(efa_rdm_ep, msg, peer, NULL, ofi_op_atomic, flags);
}

static ssize_t
efa_rdm_atomic_writev(struct fid_ep *ep,
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

	EFA_DBG(FI_LOG_EP_DATA, "total_count=%ld atomic_op=%d\n",
		ofi_total_ioc_cnt(iov, count), msg.op);

	return efa_rdm_atomic_writemsg(ep, &msg, 0);
}

static ssize_t
efa_rdm_atomic_write(struct fid_ep *ep,
		 const void *buf, size_t count, void *desc,
		 fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		 enum fi_datatype datatype, enum fi_op op, void *context)
{
	struct fi_ioc ioc;

	ioc.addr = (void *)buf;
	ioc.count = count;
	return efa_rdm_atomic_writev(ep, &ioc, &desc, 1,
				 dest_addr, addr, key,
				 datatype, op, context);
}

static ssize_t
efa_rdm_atomic_readwritemsg(struct fid_ep *ep,
			const struct fi_msg_atomic *msg,
			struct fi_ioc *resultv, void **result_desc, size_t result_count,
			uint64_t flags)
{
	struct efa_rdm_ep *efa_rdm_ep;
	struct efa_rdm_peer *peer;
	struct fi_msg_atomic shm_msg;
	struct fi_rma_ioc shm_rma_iov[EFA_RDM_IOV_LIMIT];
	void *shm_desc[EFA_RDM_IOV_LIMIT] = {NULL};
	void *shm_res_desc[EFA_RDM_IOV_LIMIT] = {NULL};
	struct efa_rdm_atomic_ex atomic_ex = {
		.resp_iov_count = result_count,
	};
	size_t datatype_size;
	int err;

	datatype_size = ofi_datatype_size(msg->datatype);
	if (OFI_UNLIKELY(!datatype_size)) {
		return -errno;
	}

	EFA_DBG(FI_LOG_EP_DATA, "total_len=%ld atomic_op=%d\n",
		ofi_total_ioc_cnt(msg->msg_iov, msg->iov_count), msg->op);

	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	err = efa_rdm_ep_cap_check_atomic(efa_rdm_ep);
	if (err)
		return err;

	err = efa_rdm_attempt_to_sync_memops_iov(efa_rdm_ep, (struct iovec *)msg->msg_iov, msg->desc, msg->iov_count);
	if (err)
		return err;

	err = efa_rdm_attempt_to_sync_memops_ioc(efa_rdm_ep, (struct fi_ioc *)resultv, result_desc, result_count);
	if (err)
		return err;

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, msg->addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		efa_rdm_atomic_init_shm_msg(efa_rdm_ep, &shm_msg, msg, shm_rma_iov, shm_desc);
		shm_msg.addr = peer->conn->shm_fi_addr;
		efa_rdm_get_desc_for_shm(result_count, result_desc, shm_res_desc);
		return fi_fetch_atomicmsg(efa_rdm_ep->shm_ep, &shm_msg,
					  resultv, shm_res_desc, result_count,
					  flags);
	}

	ofi_ioc_to_iov(resultv, atomic_ex.resp_iov, result_count, datatype_size);
	memcpy(atomic_ex.result_desc, result_desc, sizeof(void*) * result_count);

	return efa_rdm_atomic_generic_efa(efa_rdm_ep, msg, peer, &atomic_ex, ofi_op_atomic_fetch, flags);
}

static ssize_t
efa_rdm_atomic_readwritev(struct fid_ep *ep,
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

	return efa_rdm_atomic_readwritemsg(ep, &msg, resultv, result_desc, result_count, 0);
}

static ssize_t
efa_rdm_atomic_readwrite(struct fid_ep *ep,
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

	return efa_rdm_atomic_readwritev(ep, &ioc, &desc, 1,
				     &resp_ioc, &result_desc, 1,
				     dest_addr, addr, key,
				     datatype, op, context);
}

static ssize_t
efa_rdm_atomic_compwritemsg(struct fid_ep *ep,
			const struct fi_msg_atomic *msg,
			const struct fi_ioc *comparev, void **compare_desc, size_t compare_count,
			struct fi_ioc *resultv, void **result_desc, size_t result_count,
			uint64_t flags)
{
	struct efa_rdm_ep *efa_rdm_ep;
	struct efa_rdm_peer *peer;
	struct fi_msg_atomic shm_msg;
	struct fi_rma_ioc shm_rma_iov[EFA_RDM_IOV_LIMIT];
	void *shm_desc[EFA_RDM_IOV_LIMIT] = {NULL};
	void *shm_res_desc[EFA_RDM_IOV_LIMIT] = {NULL};
	void *shm_comp_desc[EFA_RDM_IOV_LIMIT] = {NULL};
	struct efa_rdm_atomic_ex atomic_ex = {
		.resp_iov_count = result_count,
		.comp_iov_count = compare_count,
		.compare_desc = compare_desc,
	};
	size_t datatype_size;
	int err;

	datatype_size = ofi_datatype_size(msg->datatype);
	if (OFI_UNLIKELY(!datatype_size)) {
		return -errno;
	}

	EFA_DBG(FI_LOG_EP_DATA, "iov_len: %lu flags: %lx\n",
		ofi_total_ioc_cnt(msg->msg_iov, msg->iov_count), flags);

	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	err = efa_rdm_ep_cap_check_atomic(efa_rdm_ep);
	if (err)
		return err;

	err = efa_rdm_attempt_to_sync_memops_iov(efa_rdm_ep, (struct iovec *)msg->msg_iov, msg->desc, msg->iov_count);
	if (err)
		return err;

	err = efa_rdm_attempt_to_sync_memops_ioc(efa_rdm_ep, (struct fi_ioc *)comparev, compare_desc, compare_count);
	if (err)
		return err;

	err = efa_rdm_attempt_to_sync_memops_ioc(efa_rdm_ep, (struct fi_ioc *)resultv, result_desc, result_count);
	if (err)
		return err;

	peer = efa_rdm_ep_get_peer(efa_rdm_ep, msg->addr);
	assert(peer);
	if (peer->is_local && efa_rdm_ep->shm_ep) {
		efa_rdm_atomic_init_shm_msg(efa_rdm_ep, &shm_msg, msg, shm_rma_iov, shm_desc);
		shm_msg.addr = peer->conn->shm_fi_addr;
		efa_rdm_get_desc_for_shm(result_count, result_desc, shm_res_desc);
		efa_rdm_get_desc_for_shm(compare_count, compare_desc, shm_comp_desc);
		return fi_compare_atomicmsg(efa_rdm_ep->shm_ep, &shm_msg,
					    comparev, shm_comp_desc, compare_count,
					    resultv, shm_res_desc, result_count,
					    flags);
	}

	ofi_ioc_to_iov(resultv, atomic_ex.resp_iov, result_count, datatype_size);
	ofi_ioc_to_iov(comparev, atomic_ex.comp_iov, compare_count, datatype_size);
	memcpy(atomic_ex.result_desc, result_desc, sizeof(void*) * result_count);

	return efa_rdm_atomic_generic_efa(efa_rdm_ep, msg, peer, &atomic_ex, ofi_op_atomic_compare, flags);
}

static ssize_t
efa_rdm_atomic_compwritev(struct fid_ep *ep,
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

	return efa_rdm_atomic_compwritemsg(ep, &msg,
				       comparev, compare_desc, compare_count,
				       resultv, result_desc, result_count,
				       0);
}

static ssize_t
efa_rdm_atomic_compwrite(struct fid_ep *ep,
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

	return efa_rdm_atomic_compwritev(ep, &ioc, &desc, 1,
				     &comp_ioc, &compare_desc, 1,
				     &resp_ioc, &result_desc, 1,
				     dest_addr, addr, key,
				     datatype, op, context);
}

int efa_rdm_atomic_query(struct fid_domain *domain,
		         enum fi_datatype datatype, enum fi_op op,
			 struct fi_atomic_attr *attr, uint64_t flags)
{
	struct efa_domain *efa_domain;
	int ret;
	size_t max_atomic_size;

	if (flags & FI_TAGGED) {
		EFA_WARN(FI_LOG_EP_CTRL,
			"tagged atomic op not supported\n");
		return -FI_EINVAL;
	}

	if ((datatype == FI_INT128) || (datatype == FI_UINT128)) {
		EFA_WARN(FI_LOG_EP_CTRL,
			"128-bit atomic integers not supported\n");
		return -FI_EOPNOTSUPP;
	}

	ret = ofi_atomic_valid(&efa_prov, datatype, op, flags);
	if (ret || !attr)
		return ret;

	efa_domain = container_of(domain, struct efa_domain,
				  util_domain.domain_fid);

	max_atomic_size = efa_domain->mtu_size - sizeof(struct efa_rdm_rta_hdr)
			  - efa_domain->addrlen
			  - EFA_RDM_IOV_LIMIT * sizeof(struct fi_rma_iov);

	if (flags & FI_COMPARE_ATOMIC)
		max_atomic_size /= 2;

	attr->size = ofi_datatype_size(datatype);
	if (OFI_UNLIKELY(!attr->size)) {
		return -errno;
	}
	attr->count = max_atomic_size / attr->size;
	return 0;
}

static int efa_rdm_atomic_valid(struct fid_ep *ep_fid, enum fi_datatype datatype,
			    enum fi_op op, uint64_t flags, size_t *count)
{
	struct util_ep *ep;
	struct efa_rdm_ep *efa_rdm_ep;
	struct fi_atomic_attr attr;
	int ret;

	ep = container_of(ep_fid, struct util_ep, ep_fid);
	efa_rdm_ep = container_of(ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);

	ret = efa_rdm_ep_cap_check_atomic(efa_rdm_ep);
	if (ret)
		return ret;

	ret = efa_rdm_atomic_query(&ep->domain->domain_fid,
				   datatype, op, &attr, flags);
	if (!ret)
		*count = attr.count;

	return ret;
}

static int efa_rdm_atomic_write_valid(struct fid_ep *ep, enum fi_datatype datatype,
				  enum fi_op op, size_t *count)
{
	return efa_rdm_atomic_valid(ep, datatype, op, 0, count);
}

static int efa_rdm_atomic_readwrite_valid(struct fid_ep *ep,
				      enum fi_datatype datatype, enum fi_op op,
				      size_t *count)
{
	return efa_rdm_atomic_valid(ep, datatype, op, FI_FETCH_ATOMIC, count);
}

static int efa_rdm_atomic_compwrite_valid(struct fid_ep *ep,
				      enum fi_datatype datatype, enum fi_op op,
				      size_t *count)
{
	return efa_rdm_atomic_valid(ep, datatype, op, FI_COMPARE_ATOMIC, count);
}

struct fi_ops_atomic efa_rdm_atomic_ops = {
	.size = sizeof(struct fi_ops_atomic),
	.write = efa_rdm_atomic_write,
	.writev = efa_rdm_atomic_writev,
	.writemsg = efa_rdm_atomic_writemsg,
	.inject = efa_rdm_atomic_inject,
	.readwrite = efa_rdm_atomic_readwrite,
	.readwritev = efa_rdm_atomic_readwritev,
	.readwritemsg = efa_rdm_atomic_readwritemsg,
	.compwrite = efa_rdm_atomic_compwrite,
	.compwritev = efa_rdm_atomic_compwritev,
	.compwritemsg = efa_rdm_atomic_compwritemsg,
	.writevalid = efa_rdm_atomic_write_valid,
	.readwritevalid = efa_rdm_atomic_readwrite_valid,
	.compwritevalid = efa_rdm_atomic_compwrite_valid,
};

