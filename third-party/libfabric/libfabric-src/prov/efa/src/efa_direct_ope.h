/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_DIRECT_OPE_H
#define EFA_DIRECT_OPE_H

#include "efa_base_ep.h"

/**
 * @brief Maximum number of IOVs supported by EFA device
 */
#define EFA_DIRECT_IOV_LIMIT 3

/**
 * @brief Structure to track outstanding EFA direct operations
 *
 * Currently only used for debugging when FI_EFA_TRACK_MR is enabled.
 * The wr_id is set to the direct_ope pointer directly.
 */
struct efa_direct_ope {
	struct dlist_entry entry;		/**< linked list entry */
	struct efa_context *context;		/**< pointer to efa_context */
	struct fi_cq_tagged_entry cq_entry;	/**< CQ entry fields for diagnostics */
	size_t iov_count;			/**< number of IOVs in the operation */
	void *desc[EFA_DIRECT_IOV_LIMIT];	/**< memory descriptors */
};

/**
 * @brief Create the EFA direct operation entry pool
 *
 * Creates a buffer pool for tracking outstanding EFA direct operations
 * and initializes the outstanding operation lists.
 * The pool is only created if FI_EFA_TRACK_MR is enabled.
 *
 * @param[in,out] base_ep	base endpoint
 * @return 0 on success, negative error code on failure
 */
int efa_direct_ope_pool_create(struct efa_base_ep *base_ep);

/**
 * @brief Destroy the EFA direct operation entry pool
 *
 * Releases any outstanding operations and destroys the pool.
 *
 * @param[in,out] base_ep	base endpoint
 */
void efa_direct_ope_pool_destroy(struct efa_base_ep *base_ep);

/**
 * @brief Allocate and record an operation entry
 *
 * Acquires efa_domain->util_domain.lock to protect the efa_direct_ope_list.
 * This is the same lock used by efa_mr_close when iterating across all EPs.
 *
 * @param[in,out] base_ep	base endpoint
 * @param[in] context		pointer to already-filled efa_context (from efa_fill_context)
 * @param[in] msg		message, or NULL if msg_rma is provided
 * @param[in] msg_rma		RMA message, or NULL if msg is provided
 * @return pointer to allocated entry, or NULL if tracking disabled or allocation fails
 */
struct efa_direct_ope *efa_direct_ope_alloc(struct efa_base_ep *base_ep,
						    struct efa_context *context,
						    const struct fi_msg *msg,
						    const struct fi_msg_rma *msg_rma);

/**
 * @brief Release an operation entry
 *
 * Acquires efa_domain->util_domain.lock to protect the efa_direct_ope_list.
 * This is the same lock used by efa_mr_close when iterating across all EPs.
 *
 * @param[in,out] base_ep	base endpoint
 * @param[in] direct_ope	operation entry to release
 */
void efa_direct_ope_release(struct efa_base_ep *base_ep,
				     struct efa_direct_ope *direct_ope);

#endif /* EFA_DIRECT_OPE_H */
