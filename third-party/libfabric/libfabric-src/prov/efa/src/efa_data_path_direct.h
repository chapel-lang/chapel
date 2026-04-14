/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All
 * rights reserved. */

/**
 * @file efa_data_path_direct.h
 * @brief EFA Direct Data Path Operations Header
 *
 * This header file defines the public interface for EFA's direct completion queue
 * operations. Direct CQ operations provide a high-performance path for completion
 * processing by bypassing the standard libfabric completion queue abstraction
 * and directly accessing hardware completion queues.
 *
 * Key Features:
 * - Direct hardware completion queue access for reduced latency
 * - Optimized completion processing for high-performance applications
 * - Integration with EFA's queue pair and completion queue infrastructure
 * - Support for both send and receive completion processing
 *
 * The direct CQ operations are designed to work alongside the standard EFA
 * provider functionality while providing an optimized fast path for
 * performance-critical applications.
 */

#ifndef _EFA_DATA_PATH_DIRECT_H
#define _EFA_DATA_PATH_DIRECT_H

#include "efa.h"
#include "efa_base_ep.h"
#include "efa_cq.h"
#include <infiniband/verbs.h>

/**
 * @brief Initialize direct completion queue operations for a queue pair
 *
 * Sets up the necessary data structures and hardware mappings to enable
 * direct completion queue operations on the specified EFA queue pair.
 * This includes mapping hardware buffers, initializing work queue structures,
 * and setting up doorbell registers.
 *
 * @param efa_qp Pointer to the EFA queue pair to initialize
 * @return 0 on success, negative error code on failure
 *
 * @note This function is called during queue pair creation and requires
 *       that the underlying hardware queue pair has been successfully created
 */
int efa_data_path_direct_qp_initialize(struct efa_qp *efa_qp);

/**
 * @brief Initialize direct completion queue operations for a completion queue
 *
 * Sets up the necessary data structures and hardware mappings to enable
 * direct completion queue operations on the specified EFA completion queue.
 * This includes mapping the hardware completion queue buffer and initializing
 * completion processing state.
 *
 * @param efa_cq Pointer to the EFA completion queue to initialize
 * @return 0 on success, negative error code on failure
 *
 * @note This function is called during completion queue creation and requires
 *       that the underlying hardware completion queue has been successfully created
 */
int efa_data_path_direct_cq_initialize(struct efa_cq *efa_cq);

/**
 * @brief Finalize and cleanup direct completion queue operations for a queue pair
 *
 * Cleans up resources allocated for direct completion queue operations,
 * including freeing work request ID pools and other associated data structures.
 * This function should be called during queue pair destruction.
 *
 * @param efa_qp Pointer to the EFA queue pair to finalize
 *
 * @note This function does not unmap hardware buffers as those are managed
 *       by the underlying rdma-core library
 */
void efa_data_path_direct_qp_finalize(struct efa_qp *efa_qp);

#endif /* _EFA_DATA_PATH_DIRECT_H */