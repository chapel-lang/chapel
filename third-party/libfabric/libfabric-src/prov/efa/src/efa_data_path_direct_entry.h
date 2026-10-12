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
#include "efa_tp.h"

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

		req_id = efa_wq_get_dev_req_id(wq, wr->wr_id);
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
#if HAVE_LTTNG
		efa_data_path_direct_tracepoint_post_recv(qp, wr);
#endif
		wr = wr->next;
	}

ring_db:
	efa_data_path_direct_rq_ring_doorbell(&qp->data_path_direct_qp.rq, wq->pc);
	return err;
}





/**
 * @brief Check if a completion has a valid QP generation
 *
 * Compares the generation bits in the CQE's req_id against the current
 * QP's work queue generation. Returns true if the completion's generation
 * matches the current QP.
 *
 * @param cqe Pointer to the completion queue entry
 * @param qp Pointer to the current EFA queue pair
 * @return true if the completion's generation matches the QP
 */
static inline bool
efa_data_path_direct_is_valid_wrid_qp_gen(struct efa_io_cdesc_common *cqe,
					  struct efa_qp *qp)
{
	struct efa_data_path_direct_wq *cur_wq;

	if (EFA_GET(&cqe->flags, EFA_IO_CDESC_COMMON_Q_TYPE) == EFA_IO_SEND_QUEUE) {
		cur_wq = &qp->data_path_direct_qp.sq.wq;
	} else {
		if (EFA_GET(&cqe->flags, EFA_IO_CDESC_COMMON_UNSOLICITED))
			return true;
		cur_wq = &qp->data_path_direct_qp.rq.wq;
	}

	return (cqe->req_id & cur_wq->gen_mask) == cur_wq->shifted_gen;
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
		efa_domain->device->qp_table[qpn & efa_domain->device->qp_table_sz_m1];

	if (!data_path_direct->cur_qp || qpn != data_path_direct->cur_qp->qp_num ||
	    !efa_data_path_direct_is_valid_wrid_qp_gen(data_path_direct->cur_cqe,
						       data_path_direct->cur_qp)) {
		data_path_direct->cur_wq = NULL;
		EFA_INFO(FI_LOG_CQ, "Dropping CQE for QP[%u]: stale or invalid\n", qpn);
		return EINVAL;
	}

	efa_data_path_direct_process_ex_cqe(ibv_cq, data_path_direct->cur_qp);
	data_path_direct->cc++;
	return 0;
}

static inline uint32_t efa_data_path_direct_wc_read_qp_num(struct efa_ibv_cq *ibv_cq)
{
	assert(ibv_cq->data_path_direct.cur_cqe);
	return ibv_cq->data_path_direct.cur_cqe->qp_num;
}

static inline enum ibv_wc_opcode
efa_data_path_direct_wc_read_opcode(struct efa_ibv_cq *ibv_cq)
{
	enum efa_io_send_op_type op_type;
	struct efa_io_cdesc_common *cqe;

	cqe = ibv_cq->data_path_direct.cur_cqe;
	assert(cqe);
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
	assert(cqe);

	if (ibv_cq->data_path_direct.cur_wq)
		efa_wq_put_dev_req_id(ibv_cq->data_path_direct.cur_wq, cqe->req_id);
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
			efa_wq_put_dev_req_id(ibv_cq->data_path_direct.cur_wq,
				    cqe->req_id);
#if HAVE_EFADV_CQ_ATTR_DB
		if (ibv_cq->data_path_direct.db)
			efa_update_cq_doorbell(&ibv_cq->data_path_direct, false);
#endif
	}
}

static inline uint32_t efa_data_path_direct_wc_read_vendor_err(struct efa_ibv_cq *ibv_cq)
{
	assert(ibv_cq->data_path_direct.cur_cqe);
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

/**
 * @brief Consolidated send operation - builds WQE as stack variable and posts directly
 * @param qp EFA queue pair
 * @param sge_list Pre-prepared SGE list (used when use_inline=false)
 * @param inline_data_list Pre-prepared inline data list (used when use_inline=true)
 * @param iov_count Number of SGE entries or inline data buffers
 * @param use_inline True to use inline data, false to use SGE list
 * @param wr_id Work request ID (pre-prepared by caller)
 * @param data Immediate data (used when FI_REMOTE_CQ_DATA flag is set)
 * @param flags Operation flags
 * @param ah Address handle
 * @param qpn Remote queue pair number
 * @param qkey Remote queue key
 */
static inline int efa_data_path_direct_post_send(
		struct efa_qp *qp,
		const struct ibv_sge *sge_list,
		const struct ibv_data_buf *inline_data_list,
		size_t iov_count,
		bool use_inline,
		uintptr_t wr_id,
		uint64_t data,
		uint64_t flags,
		struct efa_ah *ah,
		uint32_t qpn,
		uint32_t qkey)
{
	struct efa_data_path_direct_sq *sq = &qp->data_path_direct_qp.sq;
	struct efa_io_tx_wqe local_wqe = {0}; /* Stack variable - can be in registers */
	struct efa_io_tx_meta_desc *meta_desc = &local_wqe.meta;
	int err = 0;

	/* Validate queue space */
	err = efa_post_send_validate(qp);
	if (OFI_UNLIKELY(err)) {
		/* ring db for earlier wqes if there is any */
		if (sq->num_wqe_pending)
			efa_data_path_direct_send_wr_ring_db(sq);
		return err;
	}

	/* This means we are starting from fresh after a db ring so we need a barrier */
	if (!sq->num_wqe_pending)
		mmio_wc_start();

	/* when reaching the sq max_batch, ring the db */
	if (sq->num_wqe_pending == sq->wq.max_batch) {
		efa_data_path_direct_send_wr_ring_db(sq);
		/* we will prepare more WQE after db ring, so we need a barrier */
		mmio_wc_start();
	}

	/* Build metadata in local stack variable */
	efa_data_path_direct_set_ud_addr(meta_desc, ah, qpn, qkey);
	meta_desc->req_id = efa_wq_get_dev_req_id(&sq->wq, wr_id);

	/* Set common control flags */
	efa_set_common_ctrl_flags(meta_desc, sq, EFA_IO_SEND);
	if (flags & FI_REMOTE_CQ_DATA) {
		efa_send_wr_set_imm_data(meta_desc, data);
	}

	/* Handle inline data or SGE list */
	if (use_inline) {
		/* Inline data path - caller has prepared inline_data_list */
		efa_data_path_direct_set_inline_data(&local_wqe, iov_count, inline_data_list);
	} else {
		/* SGE list path - caller has prepared sge_list */
		efa_data_path_direct_set_sgl(local_wqe.data.sgl, meta_desc, sge_list, iov_count);
	}

	efa_data_path_direct_send_wr_post(qp, sq, &local_wqe);

	/* Update queue state */
	efa_sq_advance_post_idx(sq);
	sq->num_wqe_pending++;

	if (!(flags & FI_MORE))
		efa_data_path_direct_send_wr_ring_db(sq);

	return 0;
}

/**
 * @brief Consolidated RDMA read operation - builds WQE as stack variable and posts directly
 * @param qp EFA queue pair
 * @param sge_list Pre-prepared SGE list for local buffers
 * @param sge_count Number of SGE entries
 * @param remote_key Remote memory key
 * @param remote_addr Remote memory address
 * @param wr_id Work request ID (pre-prepared by caller)
 * @param flags Operation flags
 * @param ah Address handle
 * @param qpn Remote queue pair number
 * @param qkey Remote queue key
 */
static inline int efa_data_path_direct_post_read(
		struct efa_qp *qp,
		const struct ibv_sge *sge_list,
		size_t sge_count,
		uint32_t remote_key,
		uint64_t remote_addr,
		uintptr_t wr_id,
		uint64_t flags,
		struct efa_ah *ah,
		uint32_t qpn,
		uint32_t qkey)
{
	struct efa_data_path_direct_sq *sq = &qp->data_path_direct_qp.sq;
	struct efa_io_tx_wqe local_wqe = {0}; /* Stack variable - can be in registers */
	struct efa_io_tx_meta_desc *meta_desc = &local_wqe.meta;
	struct efa_io_remote_mem_addr *remote_mem = &local_wqe.data.rdma_req.remote_mem;
	int err;

	/* Validate queue space */
	err = efa_post_send_validate(qp);
	if (OFI_UNLIKELY(err)) {
		/* ring db for earlier wqes if there is any */
		if (sq->num_wqe_pending)
			efa_data_path_direct_send_wr_ring_db(sq);
		return err;
	}

	/* Validate SGE count for RDMA operations */
	if (OFI_UNLIKELY(sge_count > EFA_IO_TX_DESC_NUM_RDMA_BUFS)) {
		EFA_WARN(FI_LOG_EP_DATA, "EFA device doesn't support > %d iov for rdma operations\n", EFA_IO_TX_DESC_NUM_RDMA_BUFS);
		/* ring db for earlier wqes if there is any */
		if (sq->num_wqe_pending)
			efa_data_path_direct_send_wr_ring_db(sq);
		return EINVAL;
	}

	/* This means we are starting from fresh after a db ring so we need a barrier */
	if (!sq->num_wqe_pending)
		mmio_wc_start();

	/* when reaching the sq max_batch, ring the db */
	if (sq->num_wqe_pending == sq->wq.max_batch) {
		efa_data_path_direct_send_wr_ring_db(sq);
		/* we will prepare more WQE after db ring, so we need a barrier */
		mmio_wc_start();
	}

	/* Build metadata in local stack variable */
	efa_data_path_direct_set_ud_addr(meta_desc, ah, qpn, qkey);
	meta_desc->req_id = efa_wq_get_dev_req_id(&sq->wq, wr_id);

	/* Set common control flags for RDMA READ */
	efa_set_common_ctrl_flags(meta_desc, sq, EFA_IO_RDMA_READ);

	/* Set remote memory information */
	efa_send_wr_set_rdma_addr(remote_mem, remote_key, remote_addr);
	remote_mem->length = efa_sge_total_bytes(sge_list, sge_count);

	/* Set local SGE list - caller has prepared sge_list */
	efa_data_path_direct_set_sgl(local_wqe.data.rdma_req.local_mem, meta_desc, sge_list, sge_count);

	efa_data_path_direct_send_wr_post(qp, sq, &local_wqe);

	/* Update queue state */
	efa_sq_advance_post_idx(sq);
	sq->num_wqe_pending++;

	/* Ring doorbell if required */
	if (!(flags & FI_MORE))
		efa_data_path_direct_send_wr_ring_db(sq);

	return 0;
}

/**
 * @brief Consolidated RDMA write operation - builds WQE as stack variable and posts directly
 * @param qp EFA queue pair
 * @param sge_list Pre-prepared SGE list for local buffers
 * @param sge_count Number of SGE entries
 * @param remote_key Remote memory key
 * @param remote_addr Remote memory address
 * @param wr_id Work request ID (pre-prepared by caller)
 * @param data Immediate data (used when FI_REMOTE_CQ_DATA flag is set)
 * @param flags Operation flags
 * @param ah Address handle
 * @param qpn Remote queue pair number
 * @param qkey Remote queue key
 */
static inline int
efa_data_path_direct_post_write(
		struct efa_qp *qp,
		const struct ibv_sge *sge_list,
		size_t sge_count,
		uint32_t remote_key,
		uint64_t remote_addr,
		uintptr_t wr_id,
		uint64_t data,
		uint64_t flags,
		struct efa_ah *ah,
		uint32_t qpn,
		uint32_t qkey)
{
	struct efa_data_path_direct_sq *sq = &qp->data_path_direct_qp.sq;
	struct efa_io_tx_wqe local_wqe = {0}; /* Stack variable - can be in registers */
	struct efa_io_tx_meta_desc *meta_desc = &local_wqe.meta;
	struct efa_io_remote_mem_addr *remote_mem = &local_wqe.data.rdma_req.remote_mem;
	int err;

	/* Validate SGE count for RDMA operations */
	if (OFI_UNLIKELY(sge_count > EFA_IO_TX_DESC_NUM_RDMA_BUFS)) {
		EFA_WARN(FI_LOG_EP_DATA, "EFA device doesn't support > %d iov for rdma operations\n", EFA_IO_TX_DESC_NUM_RDMA_BUFS);
		/* ring db for earlier wqes if there is any */
		if (sq->num_wqe_pending) {
			efa_data_path_direct_send_wr_ring_db(sq);
		}
		return EINVAL;
	}

	/* Validate queue space */
	err = efa_post_send_validate(qp);
	if (OFI_UNLIKELY(err)) {
		/* ring db for earlier wqes if there is any */
		if (sq->num_wqe_pending)
			efa_data_path_direct_send_wr_ring_db(sq);
		return err;
	}

	/* Validate SGE count for RDMA operations */
	if (OFI_UNLIKELY(sge_count > EFA_IO_TX_DESC_NUM_RDMA_BUFS)) {
		EFA_WARN(FI_LOG_EP_DATA, "EFA device doesn't support > %d iov for rdma operations\n", EFA_IO_TX_DESC_NUM_RDMA_BUFS);
		/* ring db for earlier wqes if there is any */
		if (sq->num_wqe_pending)
			efa_data_path_direct_send_wr_ring_db(sq);
		return EINVAL;
	}

	/* This means we are starting from fresh after a db ring so we need a barrier */
	if (!sq->num_wqe_pending)
		mmio_wc_start();

	/* when reaching the sq max_batch, ring the db */
	if (sq->num_wqe_pending == sq->wq.max_batch) {
		efa_data_path_direct_send_wr_ring_db(sq);
		/* we will prepare more WQE after db ring, so we need a barrier */
		mmio_wc_start();
	}

	/* Build metadata in local stack variable */
	efa_data_path_direct_set_ud_addr(meta_desc, ah, qpn, qkey);
	meta_desc->req_id = efa_wq_get_dev_req_id(&sq->wq, wr_id);

	/* Set common control flags for RDMA WRITE */
	efa_set_common_ctrl_flags(meta_desc, sq, EFA_IO_RDMA_WRITE);
	if (flags & FI_REMOTE_CQ_DATA) {
		efa_send_wr_set_imm_data(meta_desc, data);
	}

	/* Set remote memory information */
	efa_send_wr_set_rdma_addr(remote_mem, remote_key, remote_addr);
	remote_mem->length = efa_sge_total_bytes(sge_list, sge_count);

	/* Set local SGE list - caller has prepared sge_list */
	efa_data_path_direct_set_sgl(local_wqe.data.rdma_req.local_mem, meta_desc, sge_list, sge_count);

	efa_data_path_direct_send_wr_post(qp, sq, &local_wqe);

	/* Update queue state */
	efa_sq_advance_post_idx(sq);
	sq->num_wqe_pending++;

	/* Ring doorbell if required */
	if (!(flags & FI_MORE))
		efa_data_path_direct_send_wr_ring_db(sq);

	return 0;
}

#endif /* HAVE_EFA_DATA_PATH_DIRECT */
#endif /* _EFA_DATA_PATH_DIRECT_ENTRY_H */
