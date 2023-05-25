/*
 * Copyright (c) 2022 Amazon.com, Inc. or its affiliates. All rights reserved.
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

#include "efa.h"

/**
 * @brief Create ibv_cq_ex by calling ibv_create_cq_ex
 *
 * @param[in] ibv_cq_init_attr_ex Pointer to ibv_cq_init_attr_ex
 * @param[in] ibv_ctx Pointer to ibv_context
 * @param[in,out] ibv_cq_ex Pointer to newly created ibv_cq_ex
 * @param[in,out] ibv_cq_ex_type enum indicating if efadv_create_cq or ibv_create_cq_ex was used
 * @return Return 0 on success, error code otherwise
 */
static inline int efa_cq_ibv_cq_ex_open_with_ibv_create_cq_ex(
	struct ibv_cq_init_attr_ex *ibv_cq_init_attr_ex,
	struct ibv_context *ibv_ctx, struct ibv_cq_ex **ibv_cq_ex,
	enum ibv_cq_ex_type *ibv_cq_ex_type)
{
	*ibv_cq_ex = ibv_create_cq_ex(ibv_ctx, ibv_cq_init_attr_ex);

	if (!*ibv_cq_ex) {
		EFA_WARN(FI_LOG_CQ, "Unable to create extended CQ: %s\n", strerror(errno));
		return -FI_ENOCQ;
	}

	*ibv_cq_ex_type = IBV_CQ;
	return 0;
}

/**
 * @brief Create ibv_cq_ex by calling efadv_create_cq or ibv_create_cq_ex
 *
 * @param[in] ibv_cq_init_attr_ex Pointer to ibv_cq_init_attr_ex
 * @param[in] efadv_cq_init_attr_ex Pointer to efadv_cq_init_attr_ex
 * @param[in] ibv_ctx Pointer to ibv_context
 * @param[in,out] ibv_cq_ex Pointer to newly created ibv_cq_ex
 * @param[in,out] ibv_cq_ex_type enum indicating if efadv_create_cq or ibv_create_cq_ex was used
 * @return Return 0 on success, error code otherwise
 */
#if HAVE_EFADV_CQ_EX
static inline int efa_cq_ibv_cq_ex_open(struct fi_cq_attr *attr,
					struct ibv_context *ibv_ctx,
					struct ibv_cq_ex **ibv_cq_ex,
					enum ibv_cq_ex_type *ibv_cq_ex_type)
{
	struct ibv_cq_init_attr_ex init_attr_ex = {
		.cqe = attr->size ? attr->size : EFA_DEF_CQ_SIZE,
		.cq_context = NULL,
		.channel = NULL,
		.comp_vector = 0,
		/* EFA requires these values for wc_flags and comp_mask.
		 * See `efa_create_cq_ex` in rdma-core.
		 */
		.wc_flags = IBV_WC_STANDARD_FLAGS,
		.comp_mask = 0,
	};

	struct efadv_cq_init_attr efadv_cq_init_attr = {
		.comp_mask = 0,
		.wc_flags = EFADV_WC_EX_WITH_SGID,
	};

	*ibv_cq_ex = efadv_create_cq(ibv_ctx, &init_attr_ex,
				     &efadv_cq_init_attr,
				     sizeof(efadv_cq_init_attr));

	if (!*ibv_cq_ex) {
		/* This could be due to old EFA kernel module versions */
		/* Fallback to ibv_create_cq_ex */
		return efa_cq_ibv_cq_ex_open_with_ibv_create_cq_ex(
			&init_attr_ex, ibv_ctx, ibv_cq_ex, ibv_cq_ex_type);
	}

	*ibv_cq_ex_type = EFADV_CQ;
	return 0;
}
#else
static inline int efa_cq_ibv_cq_ex_open(struct fi_cq_attr *attr,
					struct ibv_context *ibv_ctx,
					struct ibv_cq_ex **ibv_cq_ex,
					enum ibv_cq_ex_type *ibv_cq_ex_type)
{
	struct ibv_cq_init_attr_ex init_attr_ex = {
		.cqe = attr->size ? attr->size : EFA_DEF_CQ_SIZE,
		.cq_context = NULL,
		.channel = NULL,
		.comp_vector = 0,
		/* EFA requires these values for wc_flags and comp_mask.
		 * See `efa_create_cq_ex` in rdma-core.
		 */
		.wc_flags = IBV_WC_STANDARD_FLAGS,
		.comp_mask = 0,
	};

	return efa_cq_ibv_cq_ex_open_with_ibv_create_cq_ex(
		&init_attr_ex, ibv_ctx, ibv_cq_ex, ibv_cq_ex_type);
}
#endif
