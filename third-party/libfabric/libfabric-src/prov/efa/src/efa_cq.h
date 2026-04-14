/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef _EFA_CQ_H
#define _EFA_CQ_H

#include "efa.h"

#include "efa_data_path_direct_structs.h"
enum ibv_cq_ex_type {
	IBV_CQ,
	EFADV_CQ
};

struct efa_ibv_cq {
	struct ibv_cq_ex *ibv_cq_ex;
	enum ibv_cq_ex_type ibv_cq_ex_type;
	bool data_path_direct_enabled;
	bool poll_active;
	int poll_err;
#if HAVE_EFA_DATA_PATH_DIRECT
	struct efa_data_path_direct_cq data_path_direct;
#endif
	struct ibv_comp_channel	*channel;
	bool unsolicited_write_recv_enabled;
};

struct efa_ibv_cq_poll_list_entry {
	struct dlist_entry	entry;
	struct efa_ibv_cq	*cq;
};

struct efa_cq {
	struct util_cq			util_cq;
	struct efa_ibv_cq		ibv_cq;
	int	(*poll_ibv_cq)(ssize_t cqe_to_progress, struct efa_ibv_cq *ibv_cq);
	struct fd_signal		signal;
	size_t					entry_size;
	ofi_atomic32_t			nevents;
	enum fi_wait_obj		wait_obj;
	enum fi_cq_wait_cond	wait_cond;
	/* Only used by efa-direct cq on util cq bypass path */
	void (*read_entry)(struct efa_ibv_cq *ibv_cq, void *buf, int opcode);
	char *err_buf;
};

extern struct fi_ops_cq efa_cq_ops;
extern struct fi_ops_cq efa_cq_bypass_util_cq_ops;

extern struct fi_ops efa_cq_fi_ops;

/*
 * Control header with completion data. CQ data length is static.
 */
#define EFA_CQ_DATA_SIZE (4)

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
 * @brief Create completion channel for efa_ibv_cq
 *
 * @param[in,out] ibv_cq Pointer to efa_ibv_cq
 * @param[in] ibv_ctx Pointer to ibv_context
 * @return Return 0 on success, error code otherwise
 */
#if HAVE_EFA_CQ_NOTIFICATION
static inline int efa_cq_create_comp_channel(struct efa_ibv_cq *ibv_cq,
					     struct ibv_context *ibv_ctx)
{
	int ret;

	if (ibv_ctx->num_comp_vectors <= 1) {
		ibv_cq->channel = NULL;
		EFA_WARN(FI_LOG_CQ, "EFA device does not support CQ interrupts\n");
		return -FI_ENOSYS;
	}

	ibv_cq->channel = ibv_create_comp_channel(ibv_ctx);
	if (!ibv_cq->channel) {
		ret = -errno;
		EFA_WARN(FI_LOG_CQ, "Failed to create completion channel: %s\n",
				 fi_strerror(ret));
		return ret;
	}

	ret = fi_fd_nonblock(ibv_cq->channel->fd);
	if (ret) {
		EFA_WARN(FI_LOG_CQ, "Failed to set non-blocking mode: %s\n",
				 fi_strerror(-ret));
		ibv_destroy_comp_channel(ibv_cq->channel);
		ibv_cq->channel = NULL;
		return ret;
	}

	return FI_SUCCESS;
}
#else
static inline int efa_cq_create_comp_channel(struct efa_ibv_cq *ibv_cq,
					     struct ibv_context *ibv_ctx)
{
	return -FI_ENOSYS;
}
#endif

/**
 * @brief Create ibv_cq_ex by calling ibv_create_cq_ex
 *
 * @param[in] ibv_cq_init_attr_ex Pointer to ibv_cq_init_attr_ex
 * @param[in] ibv_ctx Pointer to ibv_context
 * @param[in,out] ibv_cq_ex Pointer to newly created ibv_cq_ex
 * @param[in,out] ibv_cq_ex_type enum indicating if efadv_create_cq or ibv_create_cq_ex was used
 * @return Return 0 on success, error code otherwise
 */
static inline int efa_cq_open_ibv_cq_with_ibv_create_cq_ex(
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
 * @param[in] attr Completion queue attributes
 * @param[in] ibv_ctx Pointer to ibv_context
 * @param[in,out] ibv_cq Pointer to efa_ibv_cq to be initialized
 * @param[in] efa_cq_init_attr Pointer to fi_efa_cq_init_attr containing attributes for efadv_create_cq
 * @return Return 0 on success, error code otherwise
 */
#if HAVE_EFADV_CQ_EX
static inline
int efa_cq_open_ibv_cq(struct fi_cq_attr *attr,
			struct ibv_context *ibv_ctx,
			struct efa_ibv_cq *ibv_cq,
			struct fi_efa_cq_init_attr *efa_cq_init_attr)
{
	int ret;

	ibv_cq->channel = NULL;
	if (attr->wait_obj != FI_WAIT_NONE) {
		ret = efa_cq_create_comp_channel(ibv_cq, ibv_ctx);
		if (ret)
			return ret;
	}

	struct ibv_cq_init_attr_ex init_attr_ex = {
		.cqe = attr->size ? attr->size : EFA_DEF_CQ_SIZE,
		.cq_context = NULL,
		.channel = ibv_cq->channel,
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

	ibv_cq->unsolicited_write_recv_enabled = false;
#if HAVE_CAPS_UNSOLICITED_WRITE_RECV
	if (efa_use_unsolicited_write_recv())
		efadv_cq_init_attr.wc_flags |= EFADV_WC_EX_WITH_IS_UNSOLICITED;
#endif

#if HAVE_CAPS_CQ_WITH_EXT_MEM_DMABUF
	if (efa_cq_init_attr->flags & FI_EFA_CQ_INIT_FLAGS_EXT_MEM_DMABUF) {
		efadv_cq_init_attr.flags = EFADV_CQ_INIT_FLAGS_EXT_MEM_DMABUF;
		efadv_cq_init_attr.ext_mem_dmabuf.buffer = efa_cq_init_attr->ext_mem_dmabuf.buffer;
		efadv_cq_init_attr.ext_mem_dmabuf.length = efa_cq_init_attr->ext_mem_dmabuf.length;
		efadv_cq_init_attr.ext_mem_dmabuf.offset = efa_cq_init_attr->ext_mem_dmabuf.offset;
		efadv_cq_init_attr.ext_mem_dmabuf.fd = efa_cq_init_attr->ext_mem_dmabuf.fd;
	}
#endif

	ibv_cq->data_path_direct_enabled = false;
	ibv_cq->ibv_cq_ex = efadv_create_cq(ibv_ctx, &init_attr_ex,
				     &efadv_cq_init_attr,
				     sizeof(efadv_cq_init_attr));

	if (!ibv_cq->ibv_cq_ex) {
#if HAVE_CAPS_CQ_WITH_EXT_MEM_DMABUF
		if (efa_cq_init_attr->flags & FI_EFA_CQ_INIT_FLAGS_EXT_MEM_DMABUF) {
			EFA_WARN(FI_LOG_CQ,
				 "efadv_create_cq failed on external memory. "
				 "errno: %s\n", strerror(errno));
			return (errno == EOPNOTSUPP) ? -FI_EOPNOTSUPP : -FI_EINVAL;
		}
#endif
		/* This could be due to old EFA kernel module versions */
		/* Fallback to ibv_create_cq_ex */
		return efa_cq_open_ibv_cq_with_ibv_create_cq_ex(
			&init_attr_ex, ibv_ctx, &ibv_cq->ibv_cq_ex, &ibv_cq->ibv_cq_ex_type);
	}

#if HAVE_CAPS_UNSOLICITED_WRITE_RECV
	if (efadv_cq_init_attr.wc_flags & EFADV_WC_EX_WITH_IS_UNSOLICITED)
		ibv_cq->unsolicited_write_recv_enabled = true;
#endif

	ibv_cq->ibv_cq_ex_type = EFADV_CQ;
	return 0;
}
#else
static inline
int efa_cq_open_ibv_cq(struct fi_cq_attr *attr,
			struct ibv_context *ibv_ctx,
			struct efa_ibv_cq *ibv_cq,
			struct fi_efa_cq_init_attr *efa_cq_init_attr)
{
	int ret;

	ibv_cq->channel = NULL;
	if (attr->wait_obj != FI_WAIT_NONE) {
		ret = efa_cq_create_comp_channel(ibv_cq, ibv_ctx);
		if (ret)
			return ret;
	}

	struct ibv_cq_init_attr_ex init_attr_ex = {
		.cqe = attr->size ? attr->size : EFA_DEF_CQ_SIZE,
		.cq_context = NULL,
		.channel = ibv_cq->channel,
		.comp_vector = 0,
		/* EFA requires these values for wc_flags and comp_mask.
		 * See `efa_create_cq_ex` in rdma-core.
		 */
		.wc_flags = IBV_WC_STANDARD_FLAGS,
		.comp_mask = 0,
	};

	ibv_cq->data_path_direct_enabled = false;
	ibv_cq->unsolicited_write_recv_enabled = false;
	return efa_cq_open_ibv_cq_with_ibv_create_cq_ex(
		&init_attr_ex, ibv_ctx, &ibv_cq->ibv_cq_ex, &ibv_cq->ibv_cq_ex_type);
}
#endif

int efa_cq_open(struct fid_domain *domain_fid, struct fi_cq_attr *attr,
		struct fid_cq **cq_fid, void *context);

void efa_cq_progress(struct util_cq *cq);

int efa_cq_close(fid_t fid);

const char *efa_cq_strerror(struct fid_cq *cq_fid, int prov_errno,
			    const void *err_data, char *buf, size_t len);

/**
 * @brief Write the error message and return its byte length
 * @param[in]    ep          EFA base endpoint
 * @param[in]    addr        Remote peer fi_addr_t
 * @param[in]    prov_errno  EFA provider * error code(must be positive)
 * @param[out]   err_msg     Pointer to the address of error message written by
 * this function
 * @param[out]   buflen      Pointer to the returned error data size
 * @return       A status code. 0 if the error data was written successfully,
 * otherwise a negative FI error code.
 */
static inline int efa_write_error_msg(struct efa_base_ep *ep, fi_addr_t addr,
				      int prov_errno, char *err_msg,
				      size_t *buflen)
{
	char ep_addr_str[OFI_ADDRSTRLEN] = {0}, peer_addr_str[OFI_ADDRSTRLEN] = {0};
	char peer_host_id_str[EFA_HOST_ID_STRING_LENGTH + 1] = {0};
	char local_host_id_str[EFA_HOST_ID_STRING_LENGTH + 1] = {0};
	const char *base_msg = efa_strerror(prov_errno);
	size_t len = 0;
	uint64_t local_host_id;

	*buflen = 0;

	len = sizeof(ep_addr_str);
	efa_base_ep_raw_addr_str(ep, ep_addr_str, &len);
	len = sizeof(peer_addr_str);
	efa_base_ep_get_peer_raw_addr_str(ep, addr, peer_addr_str, &len);

	local_host_id = efa_get_host_id(efa_env.host_id_file);
	if (!local_host_id ||
	    EFA_HOST_ID_STRING_LENGTH != snprintf(local_host_id_str,
						  EFA_HOST_ID_STRING_LENGTH + 1,
						  "i-%017lx", local_host_id)) {
		strcpy(local_host_id_str, "N/A");
	}

	/* efa-raw cannot get peer host id without a handshake */
	strcpy(peer_host_id_str, "N/A");

	int ret = snprintf(err_msg, EFA_ERROR_MSG_BUFFER_LENGTH,
			   "%s My EFA addr: %s My host id: %s Peer EFA addr: "
			   "%s Peer host id: %s",
			   base_msg, ep_addr_str, local_host_id_str,
			   peer_addr_str, peer_host_id_str);

	if (ret < 0 || ret > EFA_ERROR_MSG_BUFFER_LENGTH - 1) {
		return -FI_EINVAL;
	}

	if (strlen(err_msg) >= EFA_ERROR_MSG_BUFFER_LENGTH) {
		return -FI_ENOBUFS;
	}

	*buflen = EFA_ERROR_MSG_BUFFER_LENGTH;

	return 0;
}

int efa_cq_poll_ibv_cq(ssize_t cqe_to_process, struct efa_ibv_cq *ibv_cq);
int efa_cq_trywait(struct efa_cq *cq);
int efa_cq_signal(struct fid_cq *cq_fid);
int efa_poll_events(struct efa_cq *cq, int timeout);
int efa_cq_control(struct fid *cq, int command, void *arg);

#endif /* end of _EFA_CQ_H*/
