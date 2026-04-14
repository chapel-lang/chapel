/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All
 * rights reserved. */

/**
 * @file efa_data_path_direct_entry.h
 * @brief EFA Direct Data Path Entry Point Functions
 *
 * This header file contains the entry point functions for EFA's direct
 * data path operations. These functions provide the high-level
 * interface for posting work requests and processing completions using
 * direct hardware access.
 *
 * Key Components:
 * - Work request posting functions (send, receive, RDMA operations)
 * - Completion polling and processing functions
 * - Work completion attribute extraction functions
 * - Session management for batched operations
 *
 * All functions are marked as static inline for optimal
 * performance and are designed to be called directly from the libfabric
 * provider's fast path operations.
 *
 * The functions in this file implement the core logic for:
 * - Direct hardware queue manipulation
 * - Work request validation and posting
 * - Completion queue polling and entry processing
 * - Work completion status and attribute extraction
 *
 * @note This file is only compiled when HAVE_EFA_DATA_PATH_DIRECT is defined
 */

#ifndef _EFA_DATA_PATH_DIRECT_ENTRY_H
#define _EFA_DATA_PATH_DIRECT_ENTRY_H

#include "config.h"

#if HAVE_EFA_DATA_PATH_DIRECT

#include <rdma/ib_user_verbs.h>

#include "efa_data_path_direct_internal.h"
#include "efa_data_path_direct_structs.h"

/**
 * @brief Post receive work requests directly to hardware
 *
 * Posts receive work requests directly to the hardware receive queue,
 * bypassing the standard rdma-core posting mechanism. This provides
 * optimized performance for receive operations.
 *
 * @param qp Pointer to the EFA queue pair
 * @param wr Pointer to the first receive work request in a linked list
 * @param bad Pointer to store the failing work request on error
 * @return 0 on success, error code on failure
 *
 * @note This function writes directly to write-combining memory which
 *       may be sub-optimal compared to rdma-core's batched approach
 */
static inline int efa_data_path_direct_post_recv(struct efa_qp *qp,
						 struct ibv_recv_wr *wr,
						 struct ibv_recv_wr **bad)
{
	/* see rdma-core/providers/efa/verbs.c: efa_post_recv */
	uintptr_t addr;
	struct efa_io_rx_desc *rx_buf;
	struct efa_data_path_direct_wq *wq = &qp->data_path_direct_qp.rq.wq;
	uint32_t rq_desc_offset;
	uint32_t i;
	uint16_t req_id;
	uint32_t lkey_ctrl = 0;
	int err = 0;

	while (wr) {
		err = efa_post_recv_validate(qp, wr);
		if (err) {
			*bad = wr;
			goto ring_db;
		}

		req_id = efa_wq_get_next_wrid_idx(wq, wr->wr_id);
		wq->wqe_posted++;

		/* Default init of the rx buffer */
		EFA_SET(&lkey_ctrl, EFA_IO_RX_DESC_FIRST, 1);
		EFA_SET(&lkey_ctrl, EFA_IO_RX_DESC_LAST, 0);

		for (i = 0; i < wr->num_sge; i++) {
			rq_desc_offset = (wq->pc & wq->desc_mask) * sizeof(*rx_buf);
			rx_buf = (struct efa_io_rx_desc *)(qp->data_path_direct_qp.rq.buf +
							   rq_desc_offset);
			rx_buf->req_id = req_id;
			rx_buf->lkey_ctrl = lkey_ctrl;
			/* Set last indication if need) */
			if (i == wr->num_sge - 1)
				EFA_SET(&rx_buf->lkey_ctrl, EFA_IO_RX_DESC_LAST, 1);

			addr = wr->sg_list[i].addr;

			/* Set RX buffer desc from SGE */
			rx_buf->length = MIN(wr->sg_list[i].length, UINT16_MAX);
			EFA_SET(&rx_buf->lkey_ctrl, EFA_IO_RX_DESC_LKEY,
			wr->sg_list[i].lkey);
			rx_buf->buf_addr_lo = addr;
			rx_buf->buf_addr_hi = (uint64_t)addr >> 32;
			/* Wrap rx descriptor index */
			wq->pc++;
			if (!(wq->pc & wq->desc_mask))
				wq->phase++;
		}
		wr = wr->next;
	}

ring_db:
	efa_data_path_direct_rq_ring_doorbell(&qp->data_path_direct_qp.rq, wq->pc);
	return err;
}

/**
 * @brief Complete a work request session
 *
 * Finalizes the current work request session by posting any pending
 * work queue entries to hardware and handling error conditions.
 * This function mirrors efa_send_wr_complete functionality.
 *
 * @param qp Pointer to the EFA queue pair
 * @return 0 on success, error code if session had errors
 */
static inline int efa_data_path_direct_wr_complete(struct efa_qp *qp)
{
	/* See: efa_send_wr_complete. */

	struct efa_data_path_direct_sq *sq = &qp->data_path_direct_qp.sq;

	if (OFI_UNLIKELY(qp->data_path_direct_qp.wr_session_err)) {
		sq->wq.wqe_posted -= sq->num_wqe_pending;
		sq->wq.pc -= sq->num_wqe_pending;
		goto out;
	}
	/* it should not be possible to get here with sq->num_wqe_pending==0 */
	assert(sq->num_wqe_pending);

	efa_data_path_direct_send_wr_post_working(sq, true);

out:
	return qp->data_path_direct_qp.wr_session_err;
}

/**
 * @brief Prepare an RDMA read work request
 *
 * Sets up a work queue entry for an RDMA read operation, configuring
 * the remote memory key and address for the read operation.
 *
 * @param efaqp Pointer to the EFA queue pair
 * @param rkey Remote memory key for the target buffer
 * @param remote_addr Remote memory address to read from
 */
static inline void efa_data_path_direct_wr_rdma_read(struct efa_qp *efaqp,
						     uint32_t rkey,
						     uint64_t remote_addr)
{
	struct efa_io_tx_wqe *tx_wqe;

	tx_wqe = efa_data_path_direct_send_wr_common(efaqp, EFA_IO_RDMA_READ);
	if (OFI_UNLIKELY(!tx_wqe))
		return;

	efa_send_wr_set_rdma_addr(tx_wqe, rkey, remote_addr);
}

/**
 * @brief Prepare an RDMA write work request
 *
 * Sets up a work queue entry for an RDMA write operation, configuring
 * the remote memory key and address for the write operation.
 *
 * @param efaqp Pointer to the EFA queue pair
 * @param rkey Remote memory key for the target buffer
 * @param remote_addr Remote memory address to write to
 */
static inline void efa_data_path_direct_wr_rdma_write(struct efa_qp *efaqp,
						      uint32_t rkey,
						      uint64_t remote_addr)
{
	struct efa_io_tx_wqe *tx_wqe;

	tx_wqe = efa_data_path_direct_send_wr_common(efaqp, EFA_IO_RDMA_WRITE);
	if (OFI_UNLIKELY(!tx_wqe))
		return;

	efa_send_wr_set_rdma_addr(tx_wqe, rkey, remote_addr);
}

/**
 * @brief Prepare an RDMA write with immediate data work request
 *
 * Sets up a work queue entry for an RDMA write operation with immediate
 * data, configuring the remote memory key, address, and immediate data.
 *
 * @param efaqp Pointer to the EFA queue pair
 * @param rkey Remote memory key for the target buffer
 * @param remote_addr Remote memory address to write to
 * @param imm_data Immediate data to include with the write
 */
static inline void efa_data_path_direct_wr_rdma_write_imm(struct efa_qp *efaqp,
							  uint32_t rkey,
							  uint64_t remote_addr,
							  __be32 imm_data)
{
	struct efa_io_tx_wqe *tx_wqe;

	tx_wqe = efa_data_path_direct_send_wr_common(efaqp, EFA_IO_RDMA_WRITE);
	if (OFI_UNLIKELY(!tx_wqe))
		return;

	efa_send_wr_set_rdma_addr(tx_wqe, rkey, remote_addr);
	efa_send_wr_set_imm_data(tx_wqe, imm_data);
}

/**
 * @brief Prepare a send work request
 *
 * Sets up a work queue entry for a basic send operation.
 *
 * @param efaqp Pointer to the EFA queue pair
 */
static inline void efa_data_path_direct_wr_send(struct efa_qp *efaqp)
{
	efa_data_path_direct_send_wr_common(efaqp, EFA_IO_SEND);
}

/**
 * @brief Prepare a send with immediate data work request
 *
 * Sets up a work queue entry for a send operation with immediate data.
 *
 * @param efaqp Pointer to the EFA queue pair
 * @param imm_data Immediate data to include with the send
 */
static inline void efa_data_path_direct_wr_send_imm(struct efa_qp *efaqp, __be32 imm_data)
{
	struct efa_io_tx_wqe *tx_wqe;

	tx_wqe = efa_data_path_direct_send_wr_common(efaqp, EFA_IO_SEND);
	if (OFI_UNLIKELY(!tx_wqe))
		return;

	efa_send_wr_set_imm_data(tx_wqe, imm_data);
}

/**
 * @brief Set inline data for the current work request
 *
 * Copies data from the provided buffer list directly into the work queue
 * entry for inline transmission. This avoids the need for separate DMA
 * operations for small data transfers.
 *
 * @param efa_qp Pointer to the EFA queue pair
 * @param num_buf Number of buffers in the buffer list
 * @param buf_list Array of data buffers to copy inline
 */
static inline void
efa_data_path_direct_wr_set_inline_data_list(struct efa_qp *efa_qp,
					     size_t num_buf,
					     const struct ibv_data_buf *buf_list)
{
	struct efa_data_path_direct_qp *qp = &efa_qp->data_path_direct_qp;
	struct efa_io_tx_wqe *tx_wqe = &qp->sq.curr_tx_wqe;
	uint32_t total_length = 0;
	uint32_t length;
	size_t i;

	if (OFI_UNLIKELY(qp->wr_session_err))
		return;

	for (i = 0; i < num_buf; i++) {
		length = buf_list[i].length;

		memcpy(tx_wqe->data.inline_data + total_length,
		       buf_list[i].addr, length);
		total_length += length;
	}

	EFA_SET(&tx_wqe->meta.ctrl1, EFA_IO_TX_META_DESC_INLINE_MSG, 1);
	tx_wqe->meta.length = total_length;
}

/**
 * @brief Set scatter-gather list for the current work request
 *
 * Configures the scatter-gather elements for the current work request,
 * handling different operation types (SEND, RDMA_READ, RDMA_WRITE) with
 * appropriate buffer descriptor setup.
 *
 * @param efa_qp Pointer to the EFA queue pair
 * @param num_sge Number of scatter-gather elements
 * @param sg_list Array of scatter-gather elements
 */
static inline void efa_data_path_direct_wr_set_sge_list(struct efa_qp *efa_qp,
							size_t num_sge,
							const struct ibv_sge *sg_list)
{
	struct efa_data_path_direct_qp *qp = &efa_qp->data_path_direct_qp;
	struct efa_io_rdma_req *rdma_req;
	struct efa_io_tx_wqe *tx_wqe;
	struct efa_data_path_direct_sq *sq = &qp->sq;
	uint8_t op_type;

	if (OFI_UNLIKELY(qp->wr_session_err))
		return;

	tx_wqe = &sq->curr_tx_wqe;
	op_type = EFA_GET(&tx_wqe->meta.ctrl1, EFA_IO_TX_META_DESC_OP_TYPE);
	switch (op_type) {
	case EFA_IO_SEND:
		efa_post_send_sgl(tx_wqe->data.sgl, sg_list, num_sge);
		break;
	case EFA_IO_RDMA_READ:
	case EFA_IO_RDMA_WRITE:
		if (OFI_UNLIKELY(num_sge > EFA_IO_TX_DESC_NUM_RDMA_BUFS)) {
			EFA_WARN(FI_LOG_EP_DATA, "EFA device doesn't support > %d iov for rdma operations\n", EFA_IO_TX_DESC_NUM_RDMA_BUFS);
			qp->wr_session_err = EINVAL;
			return;
		}
		rdma_req = &tx_wqe->data.rdma_req;
		rdma_req->remote_mem.length =
			efa_sge_total_bytes(sg_list, num_sge);
		efa_post_send_sgl(rdma_req->local_mem, sg_list, num_sge);
		break;
	default:
		return;
	}

	tx_wqe->meta.length = num_sge;
}

/**
 * @brief Set unreliable datagram addressing information
 *
 * Configures the addressing information for unreliable datagram operations,
 * including the address handle, remote queue pair number, and queue key.
 *
 * @param efaqp Pointer to the EFA queue pair
 * @param ah Pointer to the address handle
 * @param remote_qpn Remote queue pair number
 * @param remote_qkey Remote queue key
 */
static inline void efa_data_path_direct_wr_set_ud_addr(struct efa_qp *efaqp,
						       struct efa_ah *ah,
						       uint32_t remote_qpn,
						       uint32_t remote_qkey)
{
	struct efa_io_tx_wqe *tx_wqe;

	if (OFI_UNLIKELY(efaqp->data_path_direct_qp.wr_session_err))
		return;

	tx_wqe = &efaqp->data_path_direct_qp.sq.curr_tx_wqe;

	tx_wqe->meta.dest_qp_num = remote_qpn;
	tx_wqe->meta.ah = ah->ahn;
	tx_wqe->meta.qkey = remote_qkey;
}

/**
 * @brief Start a new work request session
 *
 * Initializes a new work request session by clearing error state and
 * starting write-combining memory operations for optimal performance.
 *
 * @param qp Pointer to the EFA queue pair
 */
static inline void efa_data_path_direct_wr_start(struct efa_qp *qp)
{
	qp->data_path_direct_qp.wr_session_err = 0;
	qp->data_path_direct_qp.sq.num_wqe_pending = 0;
	mmio_wc_start();
}

/**
 * @brief Start polling for completion queue entries
 *
 * Initiates completion queue polling by retrieving the next available
 * completion entry and setting up the polling context. This function
 * prepares the completion queue for subsequent completion processing.
 *
 * @param ibv_cq Pointer to the IBV completion queue
 * @param attr Polling attributes (currently unused)
 * @return 0 on success, ENOENT if no completions available, EINVAL on error
 */
static inline int efa_data_path_direct_start_poll(struct efa_ibv_cq *ibv_cq,
						   struct ibv_poll_cq_attr *attr)
{
	uint32_t qpn;
	struct efa_domain *efa_domain;
	struct efa_cq *efa_cq;
	struct efa_data_path_direct_cq *data_path_direct = &ibv_cq->data_path_direct;

	efa_cq = container_of(ibv_cq, struct efa_cq, ibv_cq);

	efa_domain = container_of(efa_cq->util_cq.domain, struct efa_domain,
			  util_domain);

	data_path_direct->cur_cqe =
		efa_data_path_direct_next_device_cqe_get(data_path_direct);
	if (!data_path_direct->cur_cqe)
		return ENOENT;

	qpn = data_path_direct->cur_cqe->qp_num;
	data_path_direct->cur_qp =
		efa_domain->qp_table[qpn & efa_domain->qp_table_sz_m1];

	if (!data_path_direct->cur_qp) {
		data_path_direct->cur_wq = NULL;
		EFA_DBG(FI_LOG_CQ, "QP[%u] does not exist in QP table\n", qpn);
		return EINVAL;
	}

	efa_data_path_direct_process_ex_cqe(ibv_cq, data_path_direct->cur_qp);
	data_path_direct->cc++;
	return 0;
}

static inline uint32_t efa_data_path_direct_wc_read_qp_num(struct efa_ibv_cq *ibv_cq)
{
	return ibv_cq->data_path_direct.cur_cqe->qp_num;
}

static inline enum ibv_wc_opcode
efa_data_path_direct_wc_read_opcode(struct efa_ibv_cq *ibv_cq)
{
	enum efa_io_send_op_type op_type;
	struct efa_io_cdesc_common *cqe;

	cqe = ibv_cq->data_path_direct.cur_cqe;
	op_type = EFA_GET(&cqe->flags, EFA_IO_CDESC_COMMON_OP_TYPE);

	if (EFA_GET(&cqe->flags, EFA_IO_CDESC_COMMON_Q_TYPE) ==
	    EFA_IO_SEND_QUEUE) {
		if (op_type == EFA_IO_RDMA_WRITE)
			return IBV_WC_RDMA_WRITE;
		return IBV_WC_SEND;
	}

	if (op_type == EFA_IO_RDMA_WRITE)
		return IBV_WC_RECV_RDMA_WITH_IMM;

	return IBV_WC_RECV;
}

static inline int efa_data_path_direct_next_poll(struct efa_ibv_cq *ibv_cq)
{
	struct efa_io_cdesc_common *cqe = ibv_cq->data_path_direct.cur_cqe;

	if (ibv_cq->data_path_direct.cur_wq)
		efa_wq_put_wrid_idx(ibv_cq->data_path_direct.cur_wq, cqe->req_id);
	return efa_data_path_direct_start_poll(ibv_cq, NULL);
}

#if HAVE_EFADV_CQ_ATTR_DB
static inline void
efa_update_cq_doorbell(struct efa_data_path_direct_cq *data_path_direct,
		       bool arm)
{
	uint32_t db = 0;

	EFA_SET(&db, EFA_IO_REGS_CQ_DB_CONSUMER_INDEX, data_path_direct->cc);
	EFA_SET(&db, EFA_IO_REGS_CQ_DB_CMD_SN, data_path_direct->cmd_sn & 0x3);
	EFA_SET(&db, EFA_IO_REGS_CQ_DB_ARM, arm);

	mmio_write32(data_path_direct->db, db);
}
#endif /* HAVE_EFADV_CQ_ATTR_DB */

static inline void efa_data_path_direct_end_poll(struct efa_ibv_cq *ibv_cq)
{
	struct efa_io_cdesc_common *cqe = ibv_cq->data_path_direct.cur_cqe;

	if (cqe) {
		if (ibv_cq->data_path_direct.cur_wq)
			efa_wq_put_wrid_idx(ibv_cq->data_path_direct.cur_wq,
				    cqe->req_id);
#if HAVE_EFADV_CQ_ATTR_DB
		if (ibv_cq->data_path_direct.db)
			efa_update_cq_doorbell(&ibv_cq->data_path_direct, false);
#endif
	}
}

static inline uint32_t efa_data_path_direct_wc_read_vendor_err(struct efa_ibv_cq *ibv_cq)
{
	return ibv_cq->data_path_direct.cur_cqe->status;
}

static inline uint32_t efa_data_path_direct_wc_read_byte_len(struct efa_ibv_cq *ibv_cq)
{
	struct efa_io_cdesc_common *cqe;
	struct efa_io_rx_cdesc_ex *rcqe;
	uint32_t length;

	cqe = ibv_cq->data_path_direct.cur_cqe;

	if (EFA_GET(&cqe->flags, EFA_IO_CDESC_COMMON_Q_TYPE) !=
	    EFA_IO_RECV_QUEUE)
		return 0;

	rcqe = container_of(cqe, struct efa_io_rx_cdesc_ex, base.common);

	length = rcqe->base.length;
	if (EFA_GET(&cqe->flags, EFA_IO_CDESC_COMMON_OP_TYPE) ==
	    EFA_IO_RDMA_WRITE)
		length |= ((uint32_t) rcqe->u.rdma_write.length_hi << 16);

	return length;
}

static inline unsigned int efa_data_path_direct_wc_read_wc_flags(struct efa_ibv_cq *ibv_cq)
{
	unsigned int wc_flags = 0;

	if (EFA_GET(&ibv_cq->data_path_direct.cur_cqe->flags,
		    EFA_IO_CDESC_COMMON_HAS_IMM))
		wc_flags |= IBV_WC_WITH_IMM;

	return wc_flags;
}

static inline __be32 efa_data_path_direct_wc_read_imm_data(struct efa_ibv_cq *ibv_cq)
{
	struct efa_io_rx_cdesc *rcqe;

	rcqe = container_of(ibv_cq->data_path_direct.cur_cqe, struct efa_io_rx_cdesc,
		    common);

	return htobe32(rcqe->imm);
}

static inline uint32_t efa_data_path_direct_wc_read_src_qp(struct efa_ibv_cq *ibv_cq)
{
	struct efa_io_rx_cdesc *rcqe;

	rcqe = container_of(ibv_cq->data_path_direct.cur_cqe, struct efa_io_rx_cdesc,
		    common);

	return rcqe->src_qp_num;
}

static inline uint32_t efa_data_path_direct_wc_read_slid(struct efa_ibv_cq *ibv_cq)
{
	struct efa_io_rx_cdesc *rcqe;

	rcqe = container_of(ibv_cq->data_path_direct.cur_cqe, struct efa_io_rx_cdesc,
		    common);

	return rcqe->ah;
}

static inline int efa_data_path_direct_wc_read_sgid(struct efa_ibv_cq *ibv_cq,
						    union ibv_gid *sgid)
{
	struct efa_io_rx_cdesc_ex *rcqex;

	rcqex = container_of(ibv_cq->data_path_direct.cur_cqe,
		     struct efa_io_rx_cdesc_ex, base.common);
	if (rcqex->base.ah != 0xFFFF) {
		/* SGID is only available if AH is unknown. */
		return -ENOENT;
	}
	memcpy(sgid->raw, rcqex->u.src_addr, sizeof(sgid->raw));

	return 0;
}

static inline bool efa_data_path_direct_wc_is_unsolicited(struct efa_ibv_cq *ibv_cq)
{
	return EFA_GET(&ibv_cq->data_path_direct.cur_cqe->flags,
		       EFA_IO_CDESC_COMMON_UNSOLICITED);
}

static inline int efa_data_path_direct_get_cq_event(struct efa_ibv_cq *ibv_cq,
						    struct ibv_cq **cq,
						    void **cq_context)
{
	struct ib_uverbs_comp_event_desc ev;

	if (read(ibv_cq->channel->fd, &ev, sizeof ev) != sizeof ev)
		return -1;

	*cq = (struct ibv_cq *) (uintptr_t) ev.cq_handle;
	*cq_context = (*cq)->cq_context;

	ibv_cq->data_path_direct.cmd_sn++;

	return 0;
}

static inline int efa_data_path_direct_req_notify_cq(struct efa_ibv_cq *ibv_cq,
						     int solicited_only)
{
	if (OFI_UNLIKELY(solicited_only))
		return EOPNOTSUPP;

#if HAVE_EFADV_CQ_ATTR_DB
	if (ibv_cq->data_path_direct.db)
		efa_update_cq_doorbell(&ibv_cq->data_path_direct, true);
#endif

	return 0;
}

#endif /* HAVE_EFA_DATA_PATH_DIRECT */
#endif /* _EFA_DATA_PATH_DIRECT_ENTRY_H */