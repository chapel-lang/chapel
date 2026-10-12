/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All
 * rights reserved. */

/**
 * @file efa_data_path_direct.c
 * @brief EFA Direct Data Path Implementation
 *
 * This file implements the core functionality for EFA's direct data path
 * operations. It provides the initialization, configuration, and
 * cleanup functions for direct hardware access to completion queues and
 * queue pairs.
 *
 * Key Features:
 * - Direct hardware queue pair initialization and configuration
 * - Completion queue setup for direct hardware access
 * - Integration with rdma-core's efadv query interfaces
 * - Memory management for work request tracking structures
 *
 * The implementation leverages rdma-core's efadv (EFA device-specific)
 * interfaces to query hardware queue attributes and map hardware buffers
 * for direct access. This bypasses the standard libfabric completion
 * processing path to provide optimized performance for latency-sensitive
 * applications.
 *
 * Functions in this file are called during:
 * - Queue pair creation (efa_data_path_direct_qp_initialize)
 * - Completion queue creation (efa_data_path_direct_cq_initialize)
 * - Resource cleanup (efa_data_path_direct_qp_finalize)
 */

#include <infiniband/efadv.h>

#include "efa.h"
#include "efa_av.h"
#include "efa_data_path_direct.h"
#include "efa_io_defs.h"
#include "efa_data_path_direct_structs.h"

#if HAVE_EFA_DATA_PATH_DIRECT

#include "efa_data_path_direct_entry.h"

/**
 * @brief Initialize direct data path operations for a queue pair
 *
 * Sets up the necessary data structures and hardware mappings to enable
 * direct data path operations on the specified EFA queue pair.
 * This function is called during efa_base_ep_create_qp and mirrors the
 * functionality of efa_setup_qp in rdma-core.
 *
 * The initialization process includes:
 * - Querying hardware queue attributes from rdma-core
 * - Mapping hardware send and receive queue buffers
 * - Setting up work request ID tracking structures
 * - Configuring doorbell register access
 *
 * @param efa_qp Pointer to the EFA queue pair to initialize
 * @return 0 on success, negative error code on failure
 *
 * @note This function requires that the underlying IBV queue pair has
 *       been successfully created before being called
 */
int efa_data_path_direct_qp_initialize(struct efa_qp *efa_qp)
{
	/**
	 * Called during efa_base_ep_create_qp.
	 * See also rdma-core/providers/efa/verbs.c: efa_setup_qp
	 */
	struct efa_data_path_direct_qp *direct_qp = &efa_qp->data_path_direct_qp;
	struct efa_base_ep *base_ep = efa_qp->base_ep;

	struct efadv_wq_attr sq_attr; /* Send queue attributes from hardware */
	struct efadv_wq_attr rq_attr; /* Receive queue attributes from hardware */
	int ret = 0;

	/* Initialize the direct queue pair structure */
	memset(&efa_qp->data_path_direct_qp, 0, sizeof(efa_qp->data_path_direct_qp));


	/* Query hardware work queue attributes from rdma-core */
	ret = efadv_query_qp_wqs(efa_qp->ibv_qp, &sq_attr, &rq_attr,
			 sizeof(rq_attr));
	if (ret != FI_SUCCESS)
		return ret;

	/* Configure receive queue with hardware attributes */
	direct_qp->rq.buf = rq_attr.buffer;           /* Hardware RQ buffer */
	direct_qp->rq.wq.db = rq_attr.doorbell;       /* Hardware doorbell */
	direct_qp->rq.wq.wqe_size = rq_attr.entry_size; /* Entry size */
	direct_qp->rq.wq.max_batch = rq_attr.max_batch;
	/* Initialize receive work queue management structures */
	efa_data_path_direct_wq_initialize(&direct_qp->rq.wq, rq_attr.num_entries,
			   &base_ep->util_ep.lock);

	/* Configure send queue with hardware attributes */
	direct_qp->sq.desc = sq_attr.buffer;          /* Hardware SQ buffer */
	direct_qp->sq.wq.phase = 0;                   /* Initialize phase bit */
	direct_qp->sq.wq.db = sq_attr.doorbell;       /* Hardware doorbell */
	direct_qp->sq.num_wqe_pending = 0;            /* No pending WQEs initially */

	direct_qp->sq.wq.wqe_size = sq_attr.entry_size; /* Entry size */
	direct_qp->sq.wq.max_batch = sq_attr.max_batch;
	/* Initialize send work queue management structures */
	efa_data_path_direct_wq_initialize(&direct_qp->sq.wq, sq_attr.num_entries,
			   &base_ep->util_ep.lock);

	/* Mirror efa_qp_init_indices functionality - indices already initialized */

	/* Mark direct data path operations as enabled for this queue pair */
	efa_qp->data_path_direct_enabled = true;
	return ret;
}

/**
 * @brief Finalize and cleanup direct data path operations for a queue pair
 *
 * Cleans up resources allocated for direct data path operations,
 * including freeing work request ID pools and other associated data structures.
 * This function is called during efa_qp_destruct to ensure proper resource
 * cleanup and prevent memory leaks.
 *
 * The cleanup process includes:
 * - Freeing work request ID arrays and index pools
 * - Cleaning up send and receive queue management structures
 * - Releasing any allocated memory for direct operations
 *
 * @param efa_qp Pointer to the EFA queue pair to finalize
 *
 * @note This function does not unmap hardware buffers as those are managed
 *       by the underlying rdma-core library and will be cleaned up when
 *       the IBV queue pair is destroyed
 */
void efa_data_path_direct_qp_finalize(struct efa_qp *efa_qp)
{
	struct efa_data_path_direct_qp *direct_qp = &efa_qp->data_path_direct_qp;

	/* Clean up send and receive work queue structures */
	efa_data_path_direct_wq_finalize(&direct_qp->sq.wq);
	efa_data_path_direct_wq_finalize(&direct_qp->rq.wq);
}

/**
 * @brief Initialize direct data path operations for a completion queue
 *
 * Sets up the necessary data structures and hardware mappings to enable
 * direct data path operations on the specified EFA completion queue.
 * This function queries hardware completion queue attributes and maps the
 * hardware completion queue buffer for direct access.
 *
 * The initialization process includes:
 * - Checking if direct CQ operations are enabled and supported
 * - Querying hardware completion queue attributes from rdma-core
 * - Mapping the hardware completion queue buffer
 * - Initializing completion processing state and phase tracking
 *
 * @param efa_cq Pointer to the EFA completion queue to initialize
 * @return FI_SUCCESS on success, error code on failure
 *
 * @note This function requires that the underlying IBV completion queue
 *       has been successfully created before being called
 */
int efa_data_path_direct_cq_initialize(struct efa_ibv_cq *ibv_cq)
{
	struct efadv_cq_attr attr = {0};  /* Hardware CQ attributes */
	struct efa_data_path_direct_cq *data_path_direct = &ibv_cq->data_path_direct;
	int ret;

	/* Initialize the direct completion queue structure */
	memset(data_path_direct, 0, sizeof(*data_path_direct));


	/**
	 * Direct CQ operations are disabled if:
	 * 1. Environment variable disables direct CQ operations
	 * 2. Hardware is still using sub-completion queues (older firmware)
	 *
	 * In these cases, fall back to standard completion processing.
	 */
	if (!efa_env.use_data_path_direct || efa_device_use_sub_cq()) {
		/* Direct CQ not enabled - use standard completion processing */
		return FI_SUCCESS;
	}

	/* Query hardware completion queue attributes from rdma-core */
	ret = efadv_query_cq(ibv_cq_ex_to_cq(ibv_cq->ibv_cq_ex), &attr,
		     sizeof(attr));
	if (ret != FI_SUCCESS) {
		return ret;
	}

	/* Mark direct CQ operations as enabled */
	ibv_cq->data_path_direct_enabled = true;

	/* Configure completion queue with hardware attributes */
	data_path_direct->buffer = attr.buffer;           /* Hardware CQ buffer */
	data_path_direct->entry_size = attr.entry_size;   /* Size of each CQ entry */
	data_path_direct->num_entries = attr.num_entries; /* Total number of entries */
#if HAVE_EFADV_CQ_ATTR_DB
	data_path_direct->db = attr.doorbell;
#else
	data_path_direct->db = NULL;
#endif

	/* Initialize completion processing state */
	data_path_direct->phase = 1;                      /* Start with phase 1 */
	data_path_direct->consumed_cnt = 0;               /* No completions consumed yet */
	data_path_direct->qmask = data_path_direct->num_entries - 1; /* Mask for index wrapping */

	return FI_SUCCESS;
}

#endif /* HAVE_EFA_DATA_PATH_DIRECT */