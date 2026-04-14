/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "ofi.h"
#include <ofi_util.h>
#include <ofi_iov.h>

#include "efa.h"
#include "efa_av.h"
#include "efa_data_path_ops.h"
#include "efa_tp.h"

#include "efa_rdm_msg.h"
#include "efa_rdm_rma.h"
#include "efa_rdm_ope.h"
#include "efa_rdm_pke_cmd.h"
#include "efa_rdm_pke_rtm.h"
#include "efa_rdm_pke_nonreq.h"
#include "efa_rdm_pke_req.h"
#include "efa_rdm_tracepoint.h"
#include "efa_rdm_pke_print.h"

/**
 * @brief allocate a packet entry
 *
 * Allocate a packet entry from given packet packet pool
 * @param[in,out] ep end point
 * @param[in,out] pkt_pool packet pool
 * @param[in] alloc_type allocation type see `enum efa_rdm_pke_alloc_type`
 * @return on success return pointer of the allocated packet entry.
 *         on failure return NULL
 * @related efa_rdm_pke
 */
struct efa_rdm_pke *efa_rdm_pke_alloc(struct efa_rdm_ep *ep,
				      struct ofi_bufpool *pkt_pool,
				      enum efa_rdm_pke_alloc_type alloc_type)
{
	struct efa_rdm_pke *pkt_entry;
	void *mr = NULL;

	pkt_entry = ofi_buf_alloc_ex(pkt_pool, &mr);
	if (!pkt_entry)
		return NULL;

#ifdef ENABLE_EFA_POISONING
	efa_rdm_poison_mem_region(pkt_entry, pkt_pool->attr.size);
#endif
	dlist_init(&pkt_entry->entry);

#if ENABLE_DEBUG
	dlist_init(&pkt_entry->dbg_entry);
#endif
	/* Initialize necessary fields in pkt_entry.
	 * The memory region allocated by ofi_buf_alloc_ex is not initalized.
	 */
	pkt_entry->ep = ep;
	pkt_entry->mr = mr;
	/**
	 * Initialize pkt_entry->pkt_size to the allocated buf size of the
	 * bufpool. This is the data size posted to rdma-core, and MUST NOT
	 * exceed the memory registration size. Therefore pkt_entry->pkt_size
	 * should be adjusted according to the actual data size.
	 */
	pkt_entry->pkt_size = pkt_pool->attr.size - sizeof(struct efa_rdm_pke);
	pkt_entry->alloc_type = alloc_type;
	pkt_entry->flags = EFA_RDM_PKE_IN_USE;
	pkt_entry->next = NULL;
	pkt_entry->ope = NULL;
	pkt_entry->payload = NULL;
	pkt_entry->payload_size = 0;
	pkt_entry->payload_mr = NULL;
	pkt_entry->peer = NULL;

	switch (alloc_type) {
	case EFA_RDM_PKE_FROM_USER_RX_POOL:
	case EFA_RDM_PKE_FROM_READ_COPY_POOL:
		pkt_entry->flags |= EFA_RDM_PKE_HAS_NO_BASE_HDR;
		break;
	default:
		break;
	}
	return pkt_entry;
}

/**
 * @brief released packet entry
 *
 * @param[in] pkt_entry packet entry
 *
 * @related efa_rdm_pke
 */
void efa_rdm_pke_release(struct efa_rdm_pke *pkt_entry)
{
#ifdef ENABLE_EFA_POISONING
	efa_rdm_poison_mem_region(pkt_entry, ofi_buf_pool(pkt_entry)->attr.size);
#endif
	pkt_entry->flags = 0;
	ofi_buf_free(pkt_entry);
}

/**
 * @brief release a packet entry used by an TX operation
 *
 * TX operation include send/read_req/write_req/atomic_req
 *
 * @param[in,out] pkt_entry the pkt_entry to be released
 * @related efa_rdm_pke
 */
void efa_rdm_pke_release_tx(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ep *ep;
	struct efa_rdm_peer *peer;

	ep = pkt_entry->ep;
#if ENABLE_DEBUG
	dlist_remove(&pkt_entry->dbg_entry);
#endif
	/*
	 * Remove packet from double linked lists if it was inserted
	 */
	if (pkt_entry->flags & (EFA_RDM_PKE_IN_PEER_OUTSTANDING_TX_PKTS | EFA_RDM_PKE_IN_OPE_QUEUED_PKTS)) {
		dlist_remove(&pkt_entry->entry);
		pkt_entry->flags &= ~(EFA_RDM_PKE_IN_PEER_OUTSTANDING_TX_PKTS | EFA_RDM_PKE_IN_OPE_QUEUED_PKTS);
	}

	/*
	 * Decrement rnr_queued_pkts counter and reset backoff for this peer if
	 * we get a send completion for a retransmitted packet.
	 */
	if (OFI_UNLIKELY(pkt_entry->flags & EFA_RDM_PKE_RNR_RETRANSMIT)) {
		assert(ep->efa_rnr_queued_pkt_cnt);
		ep->efa_rnr_queued_pkt_cnt--;
		peer = pkt_entry->peer;
		assert(peer);
		peer->rnr_queued_pkt_cnt--;
		peer->rnr_backoff_wait_time = 0;
		if (peer->flags & EFA_RDM_PEER_IN_BACKOFF) {
			dlist_remove(&peer->rnr_backoff_entry);
			peer->flags &= ~EFA_RDM_PEER_IN_BACKOFF;
		}
		EFA_DBG(FI_LOG_EP_DATA,
			"reset backoff timer for peer fi_addr: %" PRIu64
			" implicit fi_addr: %" PRIu64 "\n",
			pkt_entry->peer->conn->fi_addr,
			pkt_entry->peer->conn->implicit_fi_addr);
	}

	efa_rdm_pke_release(pkt_entry);
}

/**
 * @brief release a packet entry used by a RX operation
 *
 * RX operation include receive/read_response/write_response/atomic_response
 * efa_rdm_pke_release_rx() release a rx packet entry.
 * It requires input pkt_entry to be unlinked.
 *
 * RX packet entry can be linked when medium message protocol
 * is used.
 *
 * In that case, caller is responsible to unlink the pkt_entry
 * can call this function on next packet entry.
 * @param[in,out] pkt_entry the pkt_entry to be released
 * @related efa_rdm_pke
 */
void efa_rdm_pke_release_rx(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ep *ep;

	assert(pkt_entry->next == NULL);
	ep = pkt_entry->ep;
	assert(ep);

	if (pkt_entry->alloc_type == EFA_RDM_PKE_FROM_EFA_RX_POOL) {
		ep->efa_rx_pkts_to_post++;
	} else if (pkt_entry->alloc_type == EFA_RDM_PKE_FROM_READ_COPY_POOL) {
		assert(ep->rx_readcopy_pkt_pool_used > 0);
		ep->rx_readcopy_pkt_pool_used--;
	}

#if ENABLE_DEBUG
	dlist_remove(&pkt_entry->dbg_entry);
#endif
	efa_rdm_pke_release(pkt_entry);
}

/**
 * @brief Release all rx packet entries that are linked
 * This can happen when medium / runting protocol is used.
 * This function will unlink the pkt entries in the list before
 * releasing each pkt entry as it is required by efa_rdm_pke_release_rx
 * (see above).
 * @param pkt_entry the head of the pkt entry linked list
 */
void efa_rdm_pke_release_rx_list(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_pke *curr, *next;

	curr = pkt_entry;
	while (curr) {
		next = curr->next;
		curr->next = NULL;
		efa_rdm_pke_release_rx(curr);
		curr = next;
	}
}

void efa_rdm_pke_copy(struct efa_rdm_pke *dest,
		      struct efa_rdm_pke *src)
{
	size_t src_pkt_offset;

	EFA_DBG(FI_LOG_EP_CTRL,
	       "Copying packet out of posted buffer! src_entry_alloc_type: %d desc_entry_alloc_type: %d\n",
		src->alloc_type, dest->alloc_type);
	dlist_init(&dest->entry);
#if ENABLE_DEBUG
	dlist_init(&dest->dbg_entry);
#endif
	/* dest->mr was set in efa_rdm_pke_alloc(), and
	 * is tied to the memory region, therefore should
	 * not be changed.
	 */
	dest->ope = src->ope;

	/* Pkt from read-copy pkt pool is only used for staging data
	 * that will be copied to application buffer via rdma-read,
	 * so it does not need to store anything other than application
	 * data in its wiredata. Therefore, we copy the actual data
	 * from src_pkt to the beginning of dest_pkt->wiredata.
	 */
	if (dest->alloc_type == EFA_RDM_PKE_FROM_READ_COPY_POOL) {
		assert(src->payload_size > 0);
		assert(src->payload);
		src_pkt_offset = src->payload - src->wiredata;
		dest->pkt_size = src->pkt_size - src_pkt_offset;
		dest->payload = dest->wiredata;
		dest->payload_size = dest->pkt_size;
		dest->payload_mr = dest->mr;
	} else {
		src_pkt_offset = 0;
		dest->pkt_size = src->pkt_size;
		if (src->payload_size > 0) {
			assert(src->payload);
			dest->payload = (src->payload - src->wiredata) + dest->wiredata;
			dest->payload_size = src->payload_size;
			dest->payload_mr = dest->mr;
		}
	}
	dest->peer = src->peer;
	dest->flags = EFA_RDM_PKE_IN_USE;
	dest->next = NULL;
	memcpy(dest->wiredata, src->wiredata + src_pkt_offset, dest->pkt_size);
}

/**
 * @brief create a copy of unexpected packet entry
 *
 * Handle copying or updating the metadata for an unexpected packet.
 *
 * Packets from the EFA RX pool will be copied into a separate buffer not
 * registered with the device (if this option is enabled) so that we can repost
 * the registered buffer again to keep the EFA RX queue full. Packets from the
 * SHM RX pool and peer srx ops will also be copied to reuse the unexpected message pool.
 *
 * @param[in,out] pkt_entry_ptr unexpected packet, if this packet is copied to
 *                a new memory region this pointer will be updated.
 *
 * @return	  struct efa_rdm_pke of the updated or copied packet, NULL on
 * 		  allocation failure.
 */
struct efa_rdm_pke *efa_rdm_pke_get_unexp(struct efa_rdm_pke **pkt_entry_ptr)
{
	struct efa_rdm_ep *ep;
	struct efa_rdm_pke *unexp_pkt_entry;
	enum efa_rdm_pke_alloc_type type;

	ep = (*pkt_entry_ptr)->ep;
	type = (*pkt_entry_ptr)->alloc_type;

	if (efa_env.rx_copy_unexp && (type == EFA_RDM_PKE_FROM_EFA_RX_POOL)) {
		unexp_pkt_entry = efa_rdm_pke_clone(*pkt_entry_ptr,
						    ep->rx_unexp_pkt_pool,
						    EFA_RDM_PKE_FROM_UNEXP_POOL);
		if (OFI_UNLIKELY(!unexp_pkt_entry)) {
			EFA_WARN(FI_LOG_EP_CTRL,
				"Unable to allocate rx_pkt_entry for unexp msg\n");
			return NULL;
		}
#if ENABLE_DEBUG
		/* unexp pkt is also rx pkt, insert it to rx pkt list so we can track it and clean up during ep close */
		dlist_insert_tail(&unexp_pkt_entry->dbg_entry, &ep->rx_pkt_list);
#endif
		efa_rdm_pke_release_rx(*pkt_entry_ptr);
		*pkt_entry_ptr = unexp_pkt_entry;
	} else {
		unexp_pkt_entry = *pkt_entry_ptr;
	}

	return unexp_pkt_entry;
}

void efa_rdm_pke_release_cloned(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_pke *next;

	while (pkt_entry) {
		assert(pkt_entry->alloc_type == EFA_RDM_PKE_FROM_OOO_POOL ||
		       pkt_entry->alloc_type == EFA_RDM_PKE_FROM_UNEXP_POOL);
		efa_rdm_pke_release(pkt_entry);
		next = pkt_entry->next;
		pkt_entry = next;
	}
}

/**
 * @brief clone a packet entry
 *
 * This function is used on receive side to make a copy of a packet whose memory is on bounce
 * buffer using other buffer pool, so the original packet can be released and posted to device.
 *
 * @param[in]			src		packet to be cloned
 * @param[in,out]		pkt_pool	the pool new packet to allocation from
 * @param[in]			alloc_type	pool type. Possible values are:
 * 						EFA_RDM_PKE_FROM_OOO_POOL,
 * 						EFA_RDM_PKE_FROM_UNEXP_POOL
 * 						EFA_RDM_PKE_FROM_READCOPY_POOL
 * @returns
 * A pointer to struct efa_rdm_pke.
 * If packet pool has been exhausted, return NULL.
 * @related efa_rdm_pke
 */
struct efa_rdm_pke *efa_rdm_pke_clone(struct efa_rdm_pke *src,
				      struct ofi_bufpool *pkt_pool,
				      enum efa_rdm_pke_alloc_type alloc_type)
{
	struct efa_rdm_ep *ep;
	struct efa_rdm_pke *root = NULL;
	struct efa_rdm_pke *dst;

	assert(src);
	assert(alloc_type == EFA_RDM_PKE_FROM_OOO_POOL ||
	       alloc_type == EFA_RDM_PKE_FROM_UNEXP_POOL ||
	       alloc_type == EFA_RDM_PKE_FROM_READ_COPY_POOL);

	ep = src->ep;
	assert(ep);

	dst = efa_rdm_pke_alloc(src->ep, pkt_pool, alloc_type);
	if (!dst)
		return NULL;

	if (alloc_type == EFA_RDM_PKE_FROM_READ_COPY_POOL) {
		assert(pkt_pool == ep->rx_readcopy_pkt_pool);
		ep->rx_readcopy_pkt_pool_used++;
		ep->rx_readcopy_pkt_pool_max_used = MAX(ep->rx_readcopy_pkt_pool_used,
							ep->rx_readcopy_pkt_pool_max_used);
	}

	efa_rdm_pke_copy(dst, src);
	root = dst;
	while (src->next) {
		dst->next = efa_rdm_pke_alloc(ep, pkt_pool, alloc_type);
		if (!dst->next) {
			efa_rdm_pke_release_cloned(root);
			return NULL;
		}

		efa_rdm_pke_copy(dst->next, src->next);
		src = src->next;
		dst = dst->next;
	}

	assert(dst && !dst->next);
	return root;
}

void efa_rdm_pke_append(struct efa_rdm_pke *dst,
			  struct efa_rdm_pke *src)
{
	assert(dst);

	while (dst->next)
		dst = dst->next;
	assert(dst && !dst->next);
	dst->next = src;
}

/**
 * @brief send data over wire using rdma-core API
 *
 * @param[in] pkt_entry_vec	an array of packet entries to be sent
 * @param[in] pkt_entry_cnt	number of packet entries to be sent
 * @param[in] flags		flags, currently only accept 0 or FI_MORE. When FI_MORE
 * is passed, it doesn't ring the doorbell (ibv_wr_complete).
 * @return		0 on success
 * 			On error, a negative value corresponding to fabric errno
 */
ssize_t efa_rdm_pke_sendv(struct efa_rdm_pke **pkt_entry_vec,
			  int pkt_entry_cnt, uint64_t flags)
{
	struct efa_qp *qp;
	struct efa_conn *conn;
	struct efa_rdm_ep *ep;
	struct efa_rdm_pke *pkt_entry;
	struct efa_rdm_peer *peer;
	struct ibv_sge sg_list[2];  /* efa device support up to 2 iov */
	struct ibv_data_buf inline_data_list[2];
	int ret = 0, pkt_idx, iov_cnt;

	assert(pkt_entry_cnt);
	ep = pkt_entry_vec[0]->ep;
	assert(ep);

	assert(pkt_entry_vec[0]->ope);
	peer = pkt_entry_vec[0]->ope->peer;
	assert(peer);
	if (peer->flags & EFA_RDM_PEER_IN_BACKOFF)
		return -FI_EAGAIN;

	conn = pkt_entry_vec[0]->peer->conn;
	assert(conn && conn->ep_addr);

	qp = ep->base_ep.qp;
	if (!ep->base_ep.is_wr_started) {
		efa_qp_wr_start(qp);
		ep->base_ep.is_wr_started = true;
	}
	for (pkt_idx = 0; pkt_idx < pkt_entry_cnt; ++pkt_idx) {
		pkt_entry = pkt_entry_vec[pkt_idx];
		assert(pkt_entry->peer == peer);

		qp->ibv_qp_ex->wr_id = (uintptr_t)pkt_entry;
		if ((pkt_entry->ope->fi_flags & FI_REMOTE_CQ_DATA) &&
		    (pkt_entry->flags & EFA_RDM_PKE_SEND_TO_USER_RECV_QP)) {
			/* Currently this is only expected for eager pkts */
			assert(pkt_entry_cnt == 1);
			efa_qp_wr_send_imm(qp, pkt_entry->ope->cq_entry.data);
		} else {
			efa_qp_wr_send(qp);
		}
		if (pkt_entry->pkt_size <= efa_rdm_ep_domain(ep)->device->efa_attr.inline_buf_size &&
	            !efa_mr_is_hmem((struct efa_mr *)pkt_entry->payload_mr)) {
			iov_cnt = 1;
			inline_data_list[0].addr = pkt_entry->wiredata;
			inline_data_list[0].length = pkt_entry->pkt_size - pkt_entry->payload_size;
			if (pkt_entry->payload) {
				iov_cnt = 2;
				inline_data_list[1].addr = pkt_entry->payload;
				inline_data_list[1].length = pkt_entry->payload_size;
			}

			efa_qp_wr_set_inline_data_list(qp, iov_cnt, inline_data_list);
		} else {
			iov_cnt = 1;
			sg_list[0].addr = (uintptr_t)pkt_entry->wiredata;
			sg_list[0].length = pkt_entry->pkt_size - pkt_entry->payload_size;
			sg_list[0].lkey = ((struct efa_mr *)pkt_entry->mr)->ibv_mr->lkey;
			if (pkt_entry->payload) {
				iov_cnt = 2;
				sg_list[1].addr = (uintptr_t)pkt_entry->payload;
				sg_list[1].length = pkt_entry->payload_size;
				sg_list[1].lkey = ((struct efa_mr *)pkt_entry->payload_mr)->ibv_mr->lkey;
			}

			efa_qp_wr_set_sge_list(qp, iov_cnt, sg_list);
		}

		if (pkt_entry->flags & EFA_RDM_PKE_SEND_TO_USER_RECV_QP) {
			assert(peer->extra_info[0] & EFA_RDM_EXTRA_FEATURE_REQUEST_USER_RECV_QP);
			efa_qp_wr_set_ud_addr(qp, conn->ah,
				   peer->user_recv_qp.qpn, peer->user_recv_qp.qkey);
		} else {
			efa_qp_wr_set_ud_addr(qp, conn->ah,
				   conn->ep_addr->qpn, conn->ep_addr->qkey);
		}

#if ENABLE_DEBUG
		dlist_insert_tail(&pkt_entry->dbg_entry, &ep->tx_pkt_list);
#ifdef ENABLE_EFA_RDM_PKE_DUMP
		efa_rdm_pke_print(pkt_entry, "Sent");
#endif
#endif

#if HAVE_LTTNG
		efa_rdm_tracepoint_wr_id_post_send((void *)pkt_entry);
#endif
	}

	if (!(flags & FI_MORE)) {
		ret = efa_qp_wr_complete(qp);
		ep->base_ep.is_wr_started = false;
	}

	if (OFI_UNLIKELY(ret)) {
		return (ret == ENOMEM) ? -FI_EAGAIN : -ret;
	}

	for (pkt_idx = 0; pkt_idx < pkt_entry_cnt; ++pkt_idx)
		efa_rdm_ep_record_tx_op_submitted(ep, pkt_entry_vec[pkt_idx]);
	return 0;
}

/**
 * @brief post one read request
 *
 * This function posts one read request.
 *
 * @param[in]		pkt_entry	read_entry that has information of the read request.
 * @param[in]		local_buf 	local buffer, where data will be copied to.
 * @param[in]		len		read size.
 * @param[in]		desc		memory descriptor of local buffer.
 * @param[in]		remote_buf	remote buffer, where data will be read from.
 * @param[in]		remote_key	memory key of remote buffer.
 * @return	On success, return 0
 * 		On failure, return a negative error code.
 */
int efa_rdm_pke_read(struct efa_rdm_pke *pkt_entry,
		     void *local_buf, size_t len, void *desc,
		     uint64_t remote_buf, size_t remote_key)
{
	struct efa_rdm_ep *ep;
	struct efa_qp *qp;
	struct efa_conn *conn;
	struct ibv_sge sge;
	struct efa_rdm_ope *txe;
	int err = 0;

	ep = pkt_entry->ep;
	assert(ep);
	txe = pkt_entry->ope;

	if (txe->peer == NULL)
		pkt_entry->flags |= EFA_RDM_PKE_LOCAL_READ;

	qp = ep->base_ep.qp;
	efa_qp_wr_start(qp);
	qp->ibv_qp_ex->wr_id = (uintptr_t)pkt_entry;
	efa_qp_wr_rdma_read(qp, remote_key, remote_buf);

	sge.addr = (uint64_t)local_buf;
	sge.length = len;
	sge.lkey = ((struct efa_mr *)desc)->ibv_mr->lkey;

	efa_qp_wr_set_sge_list(qp, 1, &sge);
	if (txe->peer == NULL) {
		efa_qp_wr_set_ud_addr(qp, ep->base_ep.self_ah,
				   qp->qp_num, qp->qkey);
	} else {
		conn = pkt_entry->peer->conn;
		assert(conn && conn->ep_addr);
		efa_qp_wr_set_ud_addr(qp, conn->ah,
				   conn->ep_addr->qpn, conn->ep_addr->qkey);
	}

#if ENABLE_DEBUG
#ifdef ENABLE_EFA_RDM_PKE_DUMP
	EFA_DBG(FI_LOG_EP_DATA,
		"Posted RDMA read length: %ld local buf: %ld local key: %d "
		"remote buf: %ld remote key: %ld\n",
		len, (uint64_t) local_buf,
		((struct efa_mr *) desc)->ibv_mr->lkey, remote_buf, remote_key);
#endif
#endif

#if HAVE_LTTNG
	efa_rdm_tracepoint_wr_id_post_read((void *)pkt_entry);
#endif

	err = efa_qp_wr_complete(qp);

	if (OFI_UNLIKELY(err))
		return (err == ENOMEM) ? -FI_EAGAIN : -err;

	efa_rdm_ep_record_tx_op_submitted(ep, pkt_entry);
	return 0;
}

/**
 * @brief post one write request
 *
 * This function posts one write request.
 *
 * @param[in]	pkt_entry	write_entry that has information of the write request.
 * @param[in]	flags		flags, currently only accept 0 or FI_MORE. When FI_MORE
 * is passed, it doesn't ring the doorbell (ibv_wr_complete).
 * @return	On success, return 0
 * 		On failure, return a negative error code.
 */
int efa_rdm_pke_write(struct efa_rdm_pke *pkt_entry)
{
	struct efa_rdm_ep *ep;
	struct efa_qp *qp;
	struct efa_conn *conn;
	struct ibv_sge sge;
	struct efa_rdm_rma_context_pkt *rma_context_pkt;
	struct efa_rdm_ope *txe;
	bool self_comm;
	void *local_buf;
	size_t len;
	void *desc;
	uint64_t remote_buf;
	size_t remote_key;
	int err = 0;

	ep = pkt_entry->ep;
	assert(ep);
	txe = pkt_entry->ope;

	rma_context_pkt = (struct efa_rdm_rma_context_pkt *)pkt_entry->wiredata;
	local_buf = rma_context_pkt->local_buf;
	len = rma_context_pkt->seg_size;
	desc = rma_context_pkt->desc;
	remote_buf = rma_context_pkt->remote_buf;
	remote_key = rma_context_pkt->remote_key;

	assert(((struct efa_mr *)desc)->ibv_mr);

	self_comm = (txe->peer == NULL);
	if (self_comm)
		pkt_entry->flags |= EFA_RDM_PKE_LOCAL_WRITE;

	qp = ep->base_ep.qp;
	if (!ep->base_ep.is_wr_started) {
		efa_qp_wr_start(qp);
		ep->base_ep.is_wr_started = true;
	}
	qp->ibv_qp_ex->wr_id = (uintptr_t)pkt_entry;

	if (txe->fi_flags & FI_REMOTE_CQ_DATA) {
		/* assert that we are sending the entire buffer as a
			   single IOV when immediate data is also included. */
		assert(len == txe->bytes_write_total_len);
		efa_qp_wr_rdma_write_imm(qp, remote_key, remote_buf,
				      txe->cq_entry.data);
	} else {
		efa_qp_wr_rdma_write(qp, remote_key, remote_buf);
	}

	sge.addr = (uint64_t)local_buf;
	sge.length = len;
	sge.lkey = ((struct efa_mr *)desc)->ibv_mr->lkey;

	/* As an optimization, we should consider implementing multiple-
		   iov writes using an IBV wr with multiple sge entries.
		   For now, each WR contains only one sge. */
	efa_qp_wr_set_sge_list(qp, 1, &sge);
	if (self_comm) {
		efa_qp_wr_set_ud_addr(qp, ep->base_ep.self_ah,
				   qp->qp_num, qp->qkey);
	} else {
		conn = pkt_entry->peer->conn;
		assert(conn && conn->ep_addr);
		efa_qp_wr_set_ud_addr(qp, conn->ah,
				   conn->ep_addr->qpn, conn->ep_addr->qkey);
	}

#if HAVE_LTTNG
	efa_rdm_tracepoint_wr_id_post_write((void *)pkt_entry);
#endif

	if (!(txe->fi_flags & FI_MORE)) {
		err = efa_qp_wr_complete(qp);
		ep->base_ep.is_wr_started = false;
	}

	if (OFI_UNLIKELY(err))
		return (err == ENOMEM) ? -FI_EAGAIN : -err;

	efa_rdm_ep_record_tx_op_submitted(ep, pkt_entry);
	return 0;
}

/**
 * @brief Post receive requests to EFA device
 *
 * @param[in] pke_vec	packet entries that contains information of receive buffer
 * @param[in] pke_cnt	Number of packet entries to post receive requests for
 * @return		0 on success
 * 			On error, a negative value corresponding to fabric errno
 */
ssize_t efa_rdm_pke_recvv(struct efa_rdm_pke **pke_vec,
			  int pke_cnt)
{
	struct efa_rdm_ep *ep;
	struct ibv_recv_wr *bad_wr;
	struct efa_recv_wr *recv_wr;
	int i, err;

	assert(pke_cnt);

	ep = pke_vec[0]->ep;
	assert(ep);

	for (i = 0; i < pke_cnt; ++i) {
		recv_wr = &ep->base_ep.efa_recv_wr_vec[i];
		recv_wr->wr.wr_id = (uintptr_t)pke_vec[i];
		recv_wr->wr.num_sge = 1;
		recv_wr->wr.sg_list = recv_wr->sge;
		recv_wr->wr.sg_list[0].length = pke_vec[i]->pkt_size;
		recv_wr->wr.sg_list[0].lkey = ((struct efa_mr *) pke_vec[i]->mr)->ibv_mr->lkey;
		recv_wr->wr.sg_list[0].addr = (uintptr_t)pke_vec[i]->wiredata;
		recv_wr->wr.next = NULL;
		if (i > 0)
			ep->base_ep.efa_recv_wr_vec[i-1].wr.next = &recv_wr->wr;
#if HAVE_LTTNG
		efa_rdm_tracepoint_wr_id_post_recv(pke_vec[i]);
#endif
	}

	err = efa_qp_post_recv(ep->base_ep.qp, &ep->base_ep.efa_recv_wr_vec[0].wr, &bad_wr);
	if (OFI_UNLIKELY(err))
		err = (err == ENOMEM) ? -FI_EAGAIN : -err;

	return err;
}

/**
 * @brief Post user receive requests to EFA device through user_recv_qp
 *
 * @param[in] pke_vec	packet entries that contains information of receive buffer
 * @param[in] pke_cnt	Number of packet entries to post receive requests for
 * @param[in] flags  	user supplied flags passed to fi_recv, support FI_MORE
 * @return		0 on success
 * 			On error, a negative value corresponding to fabric errno
 */
ssize_t efa_rdm_pke_user_recvv(struct efa_rdm_pke **pke_vec,
			  int pke_cnt, uint64_t flags)
{
	struct efa_rdm_ep *ep;
	struct ibv_recv_wr *bad_wr;
	struct efa_recv_wr *recv_wr;
	int i, err;
	size_t wr_index;

	assert(pke_cnt);

	ep = pke_vec[0]->ep;
	assert(ep);

	wr_index = ep->base_ep.recv_wr_index;
	assert(wr_index < ep->base_ep.info->rx_attr->size);

	for (i = 0; i < pke_cnt; ++i) {
		recv_wr = &ep->base_ep.user_recv_wr_vec[wr_index];
		recv_wr->wr.wr_id = (uintptr_t) pke_vec[i];
		recv_wr->wr.num_sge = 1;
		recv_wr->wr.sg_list = recv_wr->sge;
		recv_wr->wr.sg_list[0].addr = (uintptr_t) pke_vec[i]->payload;
		recv_wr->wr.sg_list[0].length = pke_vec[i]->payload_size;
		recv_wr->wr.sg_list[0].lkey = ((struct efa_mr *) pke_vec[i]->payload_mr)->ibv_mr->lkey;
		recv_wr->wr.next = NULL;
		if (wr_index > 0)
			ep->base_ep.user_recv_wr_vec[wr_index - 1].wr.next = &recv_wr->wr;
#if HAVE_LTTNG
		efa_rdm_tracepoint_wr_id_post_recv(pke_vec[i]);
#endif
		wr_index++;
	}

	ep->base_ep.recv_wr_index = wr_index;

	if (flags & FI_MORE)
		return 0;

	assert(ep->base_ep.user_recv_qp);
	err = efa_qp_post_recv(ep->base_ep.user_recv_qp, &ep->base_ep.user_recv_wr_vec[0].wr, &bad_wr);

	if (OFI_UNLIKELY(err))
		err = (err == ENOMEM) ? -FI_EAGAIN : -err;

	ep->base_ep.recv_wr_index = 0;

	return err;
}
