/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All
 * rights reserved. */

/**
 * @file efa_data_path_direct_structs.h
 * @brief EFA Direct Data Path Data Structures
 *
 * This header file defines the core data structures used by EFA's direct
 * data path operations. These structures provide direct access to
 * hardware queue pairs and completion queues, enabling high-performance
 * completion processing by bypassing standard libfabric abstractions.
 *
 * Key Components:
 * - Work queue structures that mirror rdma-core's internal representations
 * - Completion queue structures for direct hardware access
 * - Queue pair structures combining send and receive queues
 *
 * The structures in this file are designed to be binary-compatible with
 * rdma-core's internal structures while providing the additional metadata
 * needed for direct completion queue operations.
 *
 * @note This file is only compiled when HAVE_EFA_DATA_PATH_DIRECT is defined,
 *       indicating that the system supports direct completion queue operations
 */

#ifndef _EFA_DATA_PATH_DIRECT_STRUCTS_H
#define _EFA_DATA_PATH_DIRECT_STRUCTS_H

#include "config.h"

#if HAVE_EFA_DATA_PATH_DIRECT

#include "efa_io_defs.h"
#include "efa_io_regs_defs.h"

/**
 * The contents of this file only make sense if we can query rdma-core for QP
 * and CQ information. These structures provide direct access to hardware
 * resources and must be kept in sync with rdma-core's internal representations.
 */


/**
 * @struct efa_data_path_direct_wq
 * @brief Direct work queue structure mirroring rdma-core's efa_wq
 *
 * This structure provides direct access to work queue state and is designed
 * to be binary-compatible with rdma-core's internal efa_wq structure.
 * It manages work request IDs, queue indices, and synchronization for
 * both send and receive operations.
 */
struct efa_data_path_direct_wq {
	/* Mirror of `struct efa_wq` in rdma-core/providers/efa/efa.h */

	/** Array of work request IDs indexed by queue position */
	uint64_t *wrid;

	/**
	 * Pool of free indexes in the wrid array, used to select the wrid entry
	 * to be used to hold the next work request's context. At initialization,
	 * entry N holds value N. As out-of-order completions arrive, the value
	 * stored in a given entry might not equal the entry's index.
	 */
	uint32_t *wrid_idx_pool;

	uint32_t wqe_cnt;       /**< Total number of work queue entries */
	uint32_t wqe_size;      /**< Size of each work queue entry in bytes */
	uint32_t wqe_posted;    /**< Number of work requests posted */
	uint32_t wqe_completed; /**< Number of work requests completed */
	uint16_t pc;            /**< Producer counter for device queue indexing */
	uint16_t desc_mask;     /**< Mask for wrapping queue indices */

	/**
	 * Index of the next entry to use in wrid_idx_pool. This tracks the
	 * next available slot for assigning work request IDs.
	 */
	uint16_t wrid_idx_pool_next;

	int phase;                    /**< Current phase bit for queue wrapping */
	struct ofi_genlock *wqlock;   /**< Lock for thread-safe queue operations */
	uint32_t *db;                 /**< Hardware doorbell register pointer */
	uint32_t max_batch;           /**< max wqe cnt that can be posted in a batch */
};

/**
 * @struct efa_data_path_direct_cq
 * @brief Direct completion queue structure
 *
 * Combines libfabric's public completion queue attributes with rdma-core's
 * private completion queue state. Provides direct access to hardware
 * completion queue buffer and maintains state for efficient completion
 * processing.
 */
struct efa_data_path_direct_cq {
	/* Combines fi_efa_cq_attr (public) with rdma-core's private efa_device_cq */

	uint8_t *buffer;        /**< Hardware completion queue buffer */
	uint32_t entry_size;    /**< Size of each completion queue entry */
	uint32_t num_entries;   /**< Total number of completion queue entries */

	/** Current completion queue entry being processed */
	struct efa_io_cdesc_common *cur_cqe;
	struct efa_qp *cur_qp;                    /**< Current queue pair being processed */
	struct efa_data_path_direct_wq *cur_wq;   /**< Current work queue being processed */
	int phase;                                /**< Current phase bit for queue wrapping */
	int qmask;                                /**< Mask for queue index wrapping */
	uint16_t consumed_cnt;                    /**< Number of completions consumed */

	uint32_t *db; /**< Doorbell */
	uint16_t cc; /**< Consumer Counter */
	uint8_t cmd_sn;
};

/**
 * @struct efa_data_path_direct_rq
 * @brief Direct receive queue structure
 *
 * Mirrors rdma-core's efa_rq structure and provides direct access to
 * the receive queue buffer and work queue state.
 */
struct efa_data_path_direct_rq {
	/* Mirror of efa_rq in rdma-core/providers/efa/efa.h */
	struct efa_data_path_direct_wq wq;  /**< Work queue management structure */
	uint8_t *buf;                       /**< Hardware receive queue buffer */
};

/**
 * @struct efa_data_path_direct_sq
 * @brief Direct send queue structure
 *
 * Mirrors rdma-core's efa_sq structure and provides direct access to
 * the send queue buffer and work queue state. Includes optimizations
 * for batching work queue entries before ringing the doorbell.
 */
struct efa_data_path_direct_sq {
	/* Mirror of efa_sq in rdma-core/providers/efa/efa.h */
	struct efa_data_path_direct_wq wq;  /**< Work queue management structure */
	uint8_t *desc;                      /**< Hardware send queue buffer ("buf" for SQ) */

	/**
	 * Number of work request entries we have accepted without ringing
	 * the doorbell. Each WQE is copied to hardware as soon as it's
	 * built, but doorbell is deferred for batching efficiency.
	 */
	uint32_t num_wqe_pending;

	/** Current work queue entry being constructed */
	struct efa_io_tx_wqe curr_tx_wqe;
};

/**
 * @struct efa_data_path_direct_qp
 * @brief Direct queue pair structure
 *
 * Combines send and receive queues with error tracking and performance
 * to an EFA queue pair's hardware resources.
 * to an EFA queue pair's hardware resources.
 */
struct efa_data_path_direct_qp {
	struct efa_data_path_direct_sq sq;        /**< Send queue structure */
	struct efa_data_path_direct_rq rq;        /**< Receive queue structure */
	int wr_session_err;                       /**< Error state for current WR session */

};


#endif /* HAVE_EFA_DATA_PATH_DIRECT */
#endif /* _EFA_DATA_PATH_DIRECT_STRUCTS_H */