/**
 * @file efa_data_path_direct_internal.h
 * @brief EFA Direct Data Path Internal Functions
 *
 * This header file contains the internal implementation functions for EFA's
 * direct data path operations. These functions provide low-level
 * access to hardware resources and implement the core logic for direct
 * completion queue processing.
 *
 * Key Components:
 * - Hardware doorbell operations for queue management
 * - Work request ID management and indexing
 * - Completion queue entry processing and validation
 * - Work queue initialization and cleanup
 * - Direct memory access utilities for hardware buffers
 *
 * All functions in this file are marked as EFA_ALWAYS_INLINE for optimal
 * performance in the critical path. These functions are designed to
 * minimize overhead and provide direct hardware access.
 *
 * @note This file is only compiled when HAVE_EFA_DATA_PATH_DIRECT is defined
 */

#ifndef _EFA_DATA_PATH_DIRECT_INTERNAL_H
#define _EFA_DATA_PATH_DIRECT_INTERNAL_H

#include "config.h"

#if HAVE_EFA_DATA_PATH_DIRECT

#include "efa_base_ep.h"
#include "efa_cq.h"
#include "efa_data_path_direct_structs.h"
#include "efa_errno.h"
#include "efa_mmio.h"

/* Compiler optimization hints for performance-critical functions */
#define EFA_ALWAYS_INLINE __attribute__((always_inline)) static inline

/**
 * @brief Ring the receive queue doorbell to notify hardware of new entries
 *
 * Writes the producer counter to the hardware doorbell register to notify
 * the EFA device that new receive descriptors are available for processing.
 * Includes a memory barrier to ensure all descriptor writes are visible
 * to the device before the doorbell is rung.
 *
 * @param rq Pointer to the direct receive queue structure
 * @param pc Producer counter value to write to doorbell
 */
EFA_ALWAYS_INLINE void
efa_data_path_direct_rq_ring_doorbell(struct efa_data_path_direct_rq *rq,
				      uint16_t pc)
{
	udma_to_device_barrier(); /* Ensure all writes are visible to device */
	mmio_write32(rq->wq.db, pc);
}

/**
 * @brief Ring the send queue doorbell to notify hardware of new entries
 *
 * Writes the producer counter to the hardware doorbell register to notify
 * the EFA device that new send descriptors are available for processing.
 *
 * @param sq Pointer to the direct send queue structure
 * @param pc Producer counter value to write to doorbell
 */
EFA_ALWAYS_INLINE void efa_sq_ring_doorbell(struct efa_data_path_direct_sq *sq,
					 uint16_t pc)
{
	mmio_write32(sq->wq.db, pc);
}

/**
 * @brief Allocate the next work request ID index from the pool
 *
 * Retrieves the next available index from the work request ID pool and
 * associates it with the provided work request ID. This enables tracking
 * of work requests for completion processing, especially important for
 * out-of-order completions.
 *
 * @param wq Pointer to the work queue structure
 * @param wr_id Work request ID to associate with the allocated index
 * @return The allocated index that can be used in hardware descriptors
 *
 * @note This function assumes the queue has available space (validated
 * elsewhere)
 */
EFA_ALWAYS_INLINE uint32_t
efa_wq_get_next_wrid_idx(struct efa_data_path_direct_wq *wq, uint64_t wr_id)
{
	uint32_t wrid_idx;

	/* Get the next wrid index to be used from the free index pool */
	wrid_idx = wq->wrid_idx_pool[wq->wrid_idx_pool_next];
	wq->wrid[wrid_idx] = wr_id;

	/* Advance to next pool entry - will never overflow as validate
	 * succeeded */
	wq->wrid_idx_pool_next++;
	assert(wq->wrid_idx_pool_next <= wq->wqe_cnt);

	return wrid_idx;
}

/**
 * @brief Convert EFA hardware completion status to IBV work completion status
 *
 * Translates EFA-specific completion status codes to standard InfiniBand
 * verbs work completion status codes. This mapping ensures compatibility
 * with applications expecting standard IBV status codes.
 *
 * @param status EFA hardware completion status code
 * @return Corresponding IBV work completion status code
 *
 * @note EFA errno values are identical to efa_io_comp_status values
 */
EFA_ALWAYS_INLINE enum ibv_wc_status to_ibv_status(enum efa_errno status)
{
	/* Note: enum efa_errno status codes are precisely enum
	 * efa_io_comp_status */
	switch (status) {
	case EFA_IO_COMP_STATUS_OK:
		return IBV_WC_SUCCESS;
	case EFA_IO_COMP_STATUS_FLUSHED:
		return IBV_WC_WR_FLUSH_ERR;
	case EFA_IO_COMP_STATUS_LOCAL_ERROR_QP_INTERNAL_ERROR:
	case EFA_IO_COMP_STATUS_LOCAL_ERROR_UNSUPPORTED_OP:
	case EFA_IO_COMP_STATUS_LOCAL_ERROR_INVALID_AH:
		return IBV_WC_LOC_QP_OP_ERR;
	case EFA_IO_COMP_STATUS_LOCAL_ERROR_INVALID_LKEY:
		return IBV_WC_LOC_PROT_ERR;
	case EFA_IO_COMP_STATUS_LOCAL_ERROR_BAD_LENGTH:
		return IBV_WC_LOC_LEN_ERR;
	case EFA_IO_COMP_STATUS_REMOTE_ERROR_ABORT:
		return IBV_WC_REM_ABORT_ERR;
	case EFA_IO_COMP_STATUS_REMOTE_ERROR_RNR:
		return IBV_WC_RNR_RETRY_EXC_ERR;
	case EFA_IO_COMP_STATUS_REMOTE_ERROR_BAD_DEST_QPN:
		return IBV_WC_REM_INV_RD_REQ_ERR;
	case EFA_IO_COMP_STATUS_REMOTE_ERROR_BAD_STATUS:
		return IBV_WC_BAD_RESP_ERR;
	case EFA_IO_COMP_STATUS_REMOTE_ERROR_BAD_LENGTH:
		return IBV_WC_REM_INV_REQ_ERR;
	case EFA_IO_COMP_STATUS_LOCAL_ERROR_UNRESP_REMOTE:
	case EFA_IO_COMP_STATUS_LOCAL_ERROR_UNREACH_REMOTE:
		return IBV_WC_RESP_TIMEOUT_ERR;
	case EFA_IO_COMP_STATUS_REMOTE_ERROR_BAD_ADDRESS:
		return IBV_WC_REM_ACCESS_ERR;
	case EFA_IO_COMP_STATUS_REMOTE_ERROR_UNKNOWN_PEER:
		return IBV_WC_REM_OP_ERR;
	default:
		return IBV_WC_GENERAL_ERR;
	}
}

/**
 * @brief Check if a completion queue entry is ready for processing
 *
 * Determines if a completion queue entry has been written by hardware
 * by comparing its phase bit with the expected phase. The phase bit
 * alternates each time the completion queue wraps around.
 *
 * @param cqe_common Pointer to the completion queue entry
 * @param phase Expected phase bit value
 * @return 1 if the entry is ready, 0 otherwise
 */
EFA_ALWAYS_INLINE int efa_cqe_is_pending(struct efa_io_cdesc_common *cqe_common,
				     int phase)
{
	return EFA_GET(&cqe_common->flags, EFA_IO_CDESC_COMMON_PHASE) == phase;
}

/**
 * @brief Get a completion queue entry at a specific index
 *
 * Calculates the address of a completion queue entry within the hardware
 * completion queue buffer based on the entry index and entry size.
 *
 * @param cqd Pointer to the direct completion queue structure
 * @param entry Index of the completion queue entry to retrieve
 * @return Pointer to the completion queue entry
 */
EFA_ALWAYS_INLINE struct efa_io_cdesc_common *
efa_device_cq_get_cqe(struct efa_data_path_direct_cq *cqd, int entry)
{
	return (struct efa_io_cdesc_common *)(cqd->buffer +
					      (entry * cqd->entry_size));
}

/**
 * @brief Get the current completion queue index
 *
 * Calculates the current index within the completion queue buffer by
 * masking the consumed count with the queue mask. This handles queue
 * wraparound automatically.
 *
 * @param data_path_direct Pointer to the direct completion queue structure
 * @return Current index within the completion queue
 */
EFA_ALWAYS_INLINE uint32_t
efa_data_path_direct_get_current_index(struct efa_data_path_direct_cq *data_path_direct)
{
	return data_path_direct->consumed_cnt & data_path_direct->qmask;
}

/**
 * @brief Get the next available completion queue entry
 *
 * Retrieves the next completion queue entry that is ready for processing.
 * Handles phase bit validation, queue wraparound, and ensures proper
 * memory ordering when reading completion entries from hardware.
 *
 * @param data_path_direct Pointer to the direct completion queue structure
 * @return Pointer to the next completion entry, or NULL if none available
 *
 * @note This function mirrors cq_next_device_cqe_get from rdma-core
 */
EFA_ALWAYS_INLINE struct efa_io_cdesc_common *
efa_data_path_direct_next_device_cqe_get(struct efa_data_path_direct_cq *data_path_direct)
{
	/* Mirror of cq_next_device_cqe_get from rdma-core */
	struct efa_io_cdesc_common *cqe;
	uint32_t current_index;

	current_index =
		efa_data_path_direct_get_current_index(data_path_direct);
	cqe = efa_device_cq_get_cqe(data_path_direct, current_index);
	if (efa_cqe_is_pending(cqe, data_path_direct->phase)) {
		/**
		 * Do not read the rest of the completion entry before the
		 * phase bit has been validated. This ensures we don't read
		 * stale data from a previous queue wrap.
		 */
		udma_from_device_barrier();
		data_path_direct->consumed_cnt++;
		/* Check for queue wraparound and flip phase if needed */
		if (!efa_data_path_direct_get_current_index(data_path_direct))
			data_path_direct->phase = 1 - data_path_direct->phase;
		return cqe;
	}

	return NULL; /* No completion available */
}

/**
 * @brief Process an extended completion queue entry
 *
 * Extracts completion information from a hardware completion queue entry
 * and populates the extended IBV completion queue structure. Handles both
 * send and receive completions, including unsolicited receives.
 *
 * @param ibv_cq Pointer to the IBV completion queue structure
 * @param qp Pointer to the EFA queue pair that generated the completion
 */
EFA_ALWAYS_INLINE void
efa_data_path_direct_process_ex_cqe(struct efa_ibv_cq *ibv_cq,
				    struct efa_qp *qp)
{
	struct ibv_cq_ex *ibvcqx = ibv_cq->ibv_cq_ex;
	struct efa_io_cdesc_common *cqe = ibv_cq->data_path_direct.cur_cqe;
	uint32_t wrid_idx;

	wrid_idx = cqe->req_id;

	/* Handle send queue completions */
	if (EFA_GET(&cqe->flags, EFA_IO_CDESC_COMMON_Q_TYPE) ==
	    EFA_IO_SEND_QUEUE) {
		ibv_cq->data_path_direct.cur_wq =
			&qp->data_path_direct_qp.sq.wq;
		ibvcqx->wr_id = ibv_cq->data_path_direct.cur_wq->wrid[wrid_idx];
		ibvcqx->status = to_ibv_status(cqe->status);
	} else {
		/* Handle receive queue completions */
		/* Unsolicited receives don't have associated work requests */
		ibv_cq->data_path_direct.cur_wq =
			EFA_GET(&cqe->flags, EFA_IO_CDESC_COMMON_UNSOLICITED) ?
				NULL :
				&qp->data_path_direct_qp.rq.wq;
		ibvcqx->wr_id =
			EFA_GET(&cqe->flags, EFA_IO_CDESC_COMMON_UNSOLICITED) ?
				0 :
				ibv_cq->data_path_direct.cur_wq->wrid[wrid_idx];
		ibvcqx->status = to_ibv_status(cqe->status);
	}
}

/**
 * @brief Return a work request ID index to the free pool
 *
 * Returns a previously allocated work request ID index back to the free
 * pool for reuse. This is called when a work request completes and its
 * index is no longer needed. Thread-safe operation using the work queue lock.
 *
 * @param wq Pointer to the work queue structure
 * @param wrid_idx Work request ID index to return to the pool
 */
EFA_ALWAYS_INLINE void efa_wq_put_wrid_idx(struct efa_data_path_direct_wq *wq,
				       uint32_t wrid_idx)
{
	ofi_genlock_lock(wq->wqlock);
	wq->wrid_idx_pool_next--; /* Move back in the pool */
	wq->wrid_idx_pool[wq->wrid_idx_pool_next] = wrid_idx; /* Return index */
	wq->wqe_completed++; /* Update completion counter */
	ofi_genlock_unlock(wq->wqlock);
}

/**
 * @brief Initialize a direct work queue structure
 *
 * Allocates and initializes the data structures needed for direct work queue
 * operations, including work request ID arrays and index pools. Sets up the
 * queue for efficient work request tracking and completion processing.
 *
 * @param wq Pointer to the work queue structure to initialize
 * @param wqe_cnt Number of work queue entries (must be power of 2)
 * @param wqlock Pointer to the lock for thread-safe operations
 * @return 0 on success, ENOMEM on allocation failure
 */
EFA_ALWAYS_INLINE int
efa_data_path_direct_wq_initialize(struct efa_data_path_direct_wq *wq,
				   uint32_t wqe_cnt,
				   struct ofi_genlock *wqlock)
{
	int i;

	wq->wqe_cnt = wqe_cnt;
	wq->desc_mask = wqe_cnt - 1; /* Assumes wqe_cnt is power of 2 */
	wq->pc = 0; /* Initialize producer counter */

	/* Allocate work request ID array */
	wq->wrid = malloc(wq->wqe_cnt * sizeof(*wq->wrid));
	if (!wq->wrid)
		return ENOMEM;

	/* Allocate work request ID index pool */
	wq->wrid_idx_pool = malloc(wqe_cnt * sizeof(uint32_t));
	if (!wq->wrid_idx_pool) {
		free(wq->wrid);
		return ENOMEM;
	}

	/* Initialize the work request ID free index pool */
	for (i = 0; i < wqe_cnt; i++)
		wq->wrid_idx_pool[i] = i;

	wq->wqlock = wqlock;
	return 0;
}

/**
 * @brief Finalize and cleanup a direct work queue structure
 *
 * Frees all memory allocated for work queue operations, including work
 * request ID arrays and index pools. Should be called during queue pair
 * destruction to prevent memory leaks.
 *
 * @param wq Pointer to the work queue structure to finalize
 */
EFA_ALWAYS_INLINE void
efa_data_path_direct_wq_finalize(struct efa_data_path_direct_wq *wq)
{
	if (wq->wrid) {
		free(wq->wrid);
		wq->wrid = NULL;
	}

	if (wq->wrid_idx_pool) {
		free(wq->wrid_idx_pool);
		wq->wrid_idx_pool = NULL;
	}
}

/**
 * @brief Calculate total bytes in a scatter-gather list
 *
 * Sums the lengths of all scatter-gather elements to determine the total
 * number of bytes represented by the scatter-gather list.
 *
 * @param sg_list Array of scatter-gather elements
 * @param num_sge Number of elements in the scatter-gather list
 * @return Total number of bytes across all elements
 */
EFA_ALWAYS_INLINE size_t efa_sge_total_bytes(const struct ibv_sge *sg_list,
					  int num_sge)
{
	size_t bytes = 0;
	size_t i;

	for (i = 0; i < num_sge; i++)
		bytes += sg_list[i].length;

	return bytes;
}

/**
 * @brief Set up a transmit buffer descriptor
 *
 * Initializes a hardware transmit buffer descriptor with the provided
 * address, memory key, and length. Splits the 64-bit address into
 * separate high and low 32-bit fields as required by the hardware.
 *
 * @param tx_buf Pointer to the transmit buffer descriptor
 * @param addr 64-bit buffer address
 * @param lkey Local memory key for the buffer
 * @param length Length of the buffer in bytes
 */
EFA_ALWAYS_INLINE void efa_set_tx_buf(struct efa_io_tx_buf_desc *tx_buf,
				   uint64_t addr, uint32_t lkey,
				   uint32_t length)
{
	tx_buf->length = length;
	EFA_SET(&tx_buf->lkey, EFA_IO_TX_BUF_DESC_LKEY, lkey);
	tx_buf->buf_addr_lo = addr & 0xffffffff;
	tx_buf->buf_addr_hi = addr >> 32;
}

/**
 * @brief Convert scatter-gather list to hardware transmit buffer descriptors
 *
 * Converts an array of IBV scatter-gather elements into hardware-specific
 * transmit buffer descriptors. Each scatter-gather element is mapped to
 * a corresponding hardware descriptor.
 *
 * @param tx_bufs Array of hardware transmit buffer descriptors to populate
 * @param sg_list Array of IBV scatter-gather elements
 * @param num_sge Number of scatter-gather elements to convert
 */
EFA_ALWAYS_INLINE void efa_post_send_sgl(struct efa_io_tx_buf_desc *tx_bufs,
				      const struct ibv_sge *sg_list,
				      int num_sge)
{
	const struct ibv_sge *sge;
	size_t i;

	for (i = 0; i < num_sge; i++) {
		sge = &sg_list[i];
		efa_set_tx_buf(&tx_bufs[i], sge->addr, sge->lkey, sge->length);
	}
}

/**
 * @brief Validate that a send operation can be posted
 *
 * Checks if the send queue has available space for a new work request.
 * Prevents queue overflow by comparing posted vs completed work requests.
 *
 * @param qp Pointer to the EFA queue pair
 * @return 0 if send can be posted, ENOMEM if queue is full
 */
EFA_ALWAYS_INLINE int efa_post_send_validate(struct efa_qp *qp)
{
	/* Check if send queue is full */
	if (OFI_UNLIKELY(qp->data_path_direct_qp.sq.wq.wqe_posted -
				 qp->data_path_direct_qp.sq.wq.wqe_completed ==
			 qp->data_path_direct_qp.sq.wq.wqe_cnt)) {
		EFA_DBG(FI_LOG_EP_DATA,
			"SQ[%u] is full wqe_posted[%u] wqe_completed[%u] "
			"wqe_cnt[%u]\n",
			qp->qp_num, qp->data_path_direct_qp.sq.wq.wqe_posted,
			qp->data_path_direct_qp.sq.wq.wqe_completed,
			qp->data_path_direct_qp.sq.wq.wqe_cnt);
		return ENOMEM;
	}

	return 0; /* Queue has space available */
}

/**
 * @brief Validate that a receive operation can be posted
 *
 * Checks if the receive queue has available space for a new work request.
 * Prevents queue overflow by comparing posted vs completed work requests.
 *
 * @param qp Pointer to the EFA queue pair
 * @param wr Pointer to the receive work request (currently unused)
 * @return 0 if receive can be posted, ENOMEM if queue is full
 */
EFA_ALWAYS_INLINE int efa_post_recv_validate(struct efa_qp *qp,
					  struct ibv_recv_wr *wr)
{
	/* Check if receive queue is full */
	if (OFI_UNLIKELY(qp->data_path_direct_qp.rq.wq.wqe_posted -
				 qp->data_path_direct_qp.rq.wq.wqe_completed ==
			 qp->data_path_direct_qp.rq.wq.wqe_cnt)) {
		EFA_WARN(FI_LOG_EP_DATA,
			 "RQ[%u] is full wqe_posted[%u] wqe_completed[%u] "
			 "wqe_cnt[%u]\n",
			 qp->ibv_qp->qp_num,
			 qp->data_path_direct_qp.rq.wq.wqe_posted,
			 qp->data_path_direct_qp.rq.wq.wqe_completed,
			 qp->data_path_direct_qp.rq.wq.wqe_cnt);
		return ENOMEM;
	}

	return 0; /* Queue has space available */
}

/**
 * @brief Set common control flags for transmit work queue entries
 *
 * Initializes the standard control flags that are common to all transmit
 * operations, including operation type, phase bit, and completion request.
 *
 * @param desc Pointer to the transmit metadata descriptor
 * @param sq Pointer to the send queue structure
 * @param op_type Type of send operation (SEND, RDMA_READ, RDMA_WRITE, etc.)
 */
EFA_ALWAYS_INLINE void efa_set_common_ctrl_flags(struct efa_io_tx_meta_desc *desc,
					      struct efa_data_path_direct_sq *sq,
					      enum efa_io_send_op_type op_type)
{
	EFA_SET(&desc->ctrl1, EFA_IO_TX_META_DESC_META_DESC, 1);
	EFA_SET(&desc->ctrl1, EFA_IO_TX_META_DESC_OP_TYPE, op_type);
	EFA_SET(&desc->ctrl2, EFA_IO_TX_META_DESC_PHASE, sq->wq.phase);
	EFA_SET(&desc->ctrl2, EFA_IO_TX_META_DESC_FIRST, 1);
	EFA_SET(&desc->ctrl2, EFA_IO_TX_META_DESC_LAST, 1);
	EFA_SET(&desc->ctrl2, EFA_IO_TX_META_DESC_COMP_REQ, 1);
}

/**
 * @brief Advance the send queue posting index
 *
 * Updates the send queue indices after posting a work request. Increments
 * the producer counter and handles phase bit changes when the queue wraps.
 *
 * @param sq Pointer to the send queue structure
 */
EFA_ALWAYS_INLINE void efa_sq_advance_post_idx(struct efa_data_path_direct_sq *sq)
{
	struct efa_data_path_direct_wq *wq = &sq->wq;

	wq->wqe_posted++; /* Increment posted work request count */
	wq->pc++; /* Advance producer counter */

	/* Check for queue wraparound and advance phase if needed */
	if (!(wq->pc & wq->desc_mask))
		wq->phase++;
}

EFA_ALWAYS_INLINE void efa_send_wr_set_imm_data(struct efa_io_tx_wqe *tx_wqe,
					     __be32 imm_data)
{
	struct efa_io_tx_meta_desc *meta_desc;

	meta_desc = &tx_wqe->meta;
	meta_desc->immediate_data = be32toh(imm_data);
	EFA_SET(&meta_desc->ctrl1, EFA_IO_TX_META_DESC_HAS_IMM, 1);
}

EFA_ALWAYS_INLINE void efa_send_wr_set_rdma_addr(struct efa_io_tx_wqe *tx_wqe,
					      uint32_t rkey,
					      uint64_t remote_addr)
{
	struct efa_io_remote_mem_addr *remote_mem;

	remote_mem = &tx_wqe->data.rdma_req.remote_mem;
	remote_mem->rkey = rkey;
	remote_mem->buf_addr_lo = remote_addr & 0xFFFFFFFF;
	remote_mem->buf_addr_hi = remote_addr >> 32;
}

EFA_ALWAYS_INLINE void
efa_data_path_direct_send_wr_post_working(struct efa_data_path_direct_sq *sq,
					  bool force_doorbell)
{
	uint32_t sq_desc_idx;

	sq_desc_idx = (sq->wq.pc - 1) & sq->wq.desc_mask;
	mmio_memcpy_x64((struct efa_io_tx_wqe *)sq->desc + sq_desc_idx,
			&sq->curr_tx_wqe, sizeof(struct efa_io_tx_wqe));
	/* this routine only rings the doorbell if it must. */
	if (force_doorbell) {
		mmio_flush_writes();
		efa_sq_ring_doorbell(sq, sq->wq.pc);
		mmio_wc_start();
		sq->num_wqe_pending = 0;
	}
}

EFA_ALWAYS_INLINE struct efa_io_tx_wqe *
efa_data_path_direct_send_wr_common(struct efa_qp *qp,
				     enum efa_io_send_op_type op_type)
{
	struct ibv_qp_ex *ibvqpx = qp->ibv_qp_ex;
	struct efa_data_path_direct_qp *dpd_qp = &qp->data_path_direct_qp;
	struct efa_data_path_direct_sq *sq = &qp->data_path_direct_qp.sq;
	struct efa_io_tx_meta_desc *meta_desc;
	int err;

	if (OFI_UNLIKELY(dpd_qp->wr_session_err))
		return NULL;

	err = efa_post_send_validate(qp);
	if (OFI_UNLIKELY(err)) {
		dpd_qp->wr_session_err = err;
		return NULL;
	}

	/* MODIFIED: if any are pending, copy out the previous one first: */
	if (sq->num_wqe_pending) {
		/* when reaching the sq max_batch, ring the db */
		efa_data_path_direct_send_wr_post_working(sq, sq->num_wqe_pending == sq->wq.max_batch);
	}

	memset(&sq->curr_tx_wqe, 0, sizeof(sq->curr_tx_wqe));

	meta_desc = &sq->curr_tx_wqe.meta;
	efa_set_common_ctrl_flags(meta_desc, sq, op_type);
	meta_desc->req_id = efa_wq_get_next_wrid_idx(&sq->wq, ibvqpx->wr_id);

	/* advance index and change phase */
	efa_sq_advance_post_idx(sq);
	sq->num_wqe_pending++;
	return &sq->curr_tx_wqe;
}

#endif /* HAVE_EFA_DATA_PATH_DIRECT */
#endif /* _EFA_DATA_PATH_DIRECT_INTERNAL_H */