/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "efa.h"

enum ibv_cq_ex_type {
	IBV_CQ,
	EFADV_CQ
};

struct efa_ibv_cq {
	struct ibv_cq_ex *ibv_cq_ex;
	enum ibv_cq_ex_type ibv_cq_ex_type;
};

struct efa_ibv_cq_poll_list_entry {
	struct dlist_entry	entry;
	struct efa_ibv_cq	*cq;
};

void efa_rdm_cq_poll_ibv_cq(ssize_t cqe_to_process, struct efa_ibv_cq *ibv_cq);

static inline
int efa_ibv_cq_poll_list_match(struct dlist_entry *entry, const void *cq)
{
	struct efa_ibv_cq_poll_list_entry *item;
	item = container_of(entry, struct efa_ibv_cq_poll_list_entry, entry);
	return (item->cq == cq);
}


static inline
int efa_ibv_cq_poll_list_insert(struct dlist_entry *poll_list, struct ofi_genlock *lock, struct efa_ibv_cq *cq)
{
	int ret = 0;
	struct dlist_entry *entry;
	struct efa_ibv_cq_poll_list_entry *item;

	ofi_genlock_lock(lock);
	entry = dlist_find_first_match(poll_list, efa_ibv_cq_poll_list_match, cq);
	if (entry) {
		ret = -FI_EALREADY;
		goto out;
	}

	item = calloc(1, sizeof(*item));
	if (!item) {
		ret = -FI_ENOMEM;
		goto out;
	}

	item->cq = cq;
	dlist_insert_tail(&item->entry, poll_list);

out:
	ofi_genlock_unlock(lock);
	return (!ret || (ret == -FI_EALREADY)) ? 0 : ret;
}

static inline
void efa_ibv_cq_poll_list_remove(struct dlist_entry *poll_list, struct ofi_genlock *lock,
		      struct efa_ibv_cq *cq)
{
	struct efa_ibv_cq_poll_list_entry *item;
	struct dlist_entry *entry;

	ofi_genlock_lock(lock);
	entry = dlist_remove_first_match(poll_list, efa_ibv_cq_poll_list_match, cq);
	ofi_genlock_unlock(lock);

	if (entry) {
		item = container_of(entry, struct efa_ibv_cq_poll_list_entry, entry);
		free(item);
	}
}

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
		return -FI_EINVAL;
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
