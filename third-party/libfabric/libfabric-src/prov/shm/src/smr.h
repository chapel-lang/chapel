/*
 * Copyright (c) 2015-2021 Intel Corporation, Inc.  All rights reserved.
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

#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>
#include <sys/statvfs.h>
#include <pthread.h>
#include <stdint.h>
#include <stddef.h>

#include <rdma/fabric.h>
#include <rdma/fi_atomic.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_eq.h>
#include <rdma/fi_errno.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_tagged.h>
#include <rdma/fi_trigger.h>
#include <rdma/providers/fi_prov.h>
#include <rdma/providers/fi_peer.h>

#include <ofi.h>
#include <ofi_enosys.h>
#include <ofi_shm_p2p.h>
#include <ofi_rbuf.h>
#include <ofi_list.h>
#include <ofi_signal.h>
#include <ofi_epoll.h>
#include <ofi_util.h>
#include <ofi_atomic.h>
#include <ofi_iov.h>
#include <ofi_mr.h>
#include <ofi_lock.h>

#include "smr_util.h"

#ifndef _SMR_H_
#define _SMR_H_

struct smr_env {
	size_t sar_threshold;
	int disable_cma;
	int use_dsa_sar;
	size_t max_gdrcopy_size;
	int use_xpmem;
};

extern struct smr_env smr_env;
extern struct fi_provider smr_prov;
extern struct fi_info smr_info;
extern struct util_prov smr_util_prov;
extern int smr_global_ep_idx; //protected by the ep_list_lock

int smr_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fabric,
		void *context);

struct smr_av {
	struct util_av		util_av;
	struct smr_map		*smr_map;
	size_t			used;
};

static inline int64_t smr_addr_lookup(struct util_av *av, fi_addr_t fiaddr)
{
	return *((int64_t *) ofi_av_get_addr(av, fiaddr));
}

int smr_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		struct fid_domain **dom, void *context);

int smr_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
		struct fid_eq **eq, void *context);

int smr_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
		struct fid_av **av, void *context);

int smr_query_atomic(struct fid_domain *domain, enum fi_datatype datatype,
		enum fi_op op, struct fi_atomic_attr *attr, uint64_t flags);

#define SMR_IOV_LIMIT		4

struct smr_tx_entry {
	struct smr_cmd	cmd;
	int64_t		peer_id;
	void		*context;
	struct iovec	iov[SMR_IOV_LIMIT];
	uint32_t	iov_count;
	uint64_t	op_flags;
	size_t		bytes_done;
	void		*map_ptr;
	struct smr_ep_name *map_name;
	struct ofi_mr	*mr[SMR_IOV_LIMIT];
	int			fd;
};

struct smr_pend_entry {
	struct dlist_entry	entry;
	struct smr_cmd		cmd;
	struct fi_peer_rx_entry	*rx_entry;
	struct smr_cmd_ctx	*cmd_ctx;
	size_t			bytes_done;
	struct iovec		iov[SMR_IOV_LIMIT];
	size_t			iov_count;
	struct ofi_mr		*mr[SMR_IOV_LIMIT];
	struct ofi_mr_entry	*ipc_entry;
	ofi_hmem_async_event_t	async_event;
};

struct smr_cmd_ctx {
	struct dlist_entry entry;
	struct smr_ep *ep;
	struct smr_cmd cmd;
	struct smr_pend_entry *sar_entry;
	struct slist buf_list;
};

OFI_DECLARE_FREESTACK(struct smr_tx_entry, smr_tx_fs);

struct smr_fabric {
	struct util_fabric	util_fabric;
};

struct smr_domain {
	struct util_domain	util_domain;
	int			fast_rma;
	/* cache for use with hmem ipc */
	struct ofi_mr_cache	*ipc_cache;
	struct fid_peer_srx	*srx;
};

#define SMR_PREFIX	"fi_shm://"
#define SMR_PREFIX_NS	"fi_ns://"

#define SMR_ZE_SOCK_PATH	"/dev/shm/ze_"

#define SMR_RMA_ORDER (OFI_ORDER_RAR_SET | OFI_ORDER_RAW_SET | FI_ORDER_RAS |	\
		       OFI_ORDER_WAR_SET | OFI_ORDER_WAW_SET | FI_ORDER_WAS |	\
		       FI_ORDER_SAR | FI_ORDER_SAW)
#define smr_fast_rma_enabled(mode, order) ((mode & FI_MR_VIRT_ADDR) && \
			!(order & SMR_RMA_ORDER))

static inline uint64_t smr_get_offset(void *base, void *addr)
{
	return (uintptr_t) ((char *) addr - (char *) base);
}

static inline void *smr_get_ptr(void *base, uint64_t offset)
{
	return (char *) base + (uintptr_t) offset;
}

struct smr_sock_name {
	char name[SMR_SOCK_NAME_MAX];
	struct dlist_entry entry;
};

enum smr_cmap_state {
	SMR_CMAP_INIT = 0,
	SMR_CMAP_SUCCESS,
	SMR_CMAP_FAILED,
};

struct smr_cmap_entry {
	enum smr_cmap_state	state;
	int			*device_fds;
};

struct smr_sock_info {
	char			name[SMR_SOCK_NAME_MAX];
	int			listen_sock;
	ofi_epoll_t		epollfd;
	struct fd_signal	signal;
	pthread_t		listener_thread;
	int			*my_fds;
	int			nfds;
	struct smr_cmap_entry	peers[SMR_MAX_PEERS];
};

struct smr_unexp_buf {
	struct slist_entry entry;
	char buf[SMR_SAR_SIZE];
};

struct smr_ep {
	struct util_ep		util_ep;
	size_t			tx_size;
	size_t			rx_size;
	const char		*name;
	uint64_t		msg_id;
	struct smr_region	*volatile region;
	struct fid_ep		*srx;
	struct ofi_bufpool	*cmd_ctx_pool;
	struct ofi_bufpool	*unexp_buf_pool;
	struct ofi_bufpool	*pend_buf_pool;

	struct smr_tx_fs	*tx_fs;
	struct dlist_entry	sar_list;
	struct dlist_entry	ipc_cpy_pend_list;

	int			ep_idx;
	enum ofi_shm_p2p_type	p2p_type;
	struct smr_sock_info	*sock_info;
	void			*dsa_context;
	void 			(*smr_progress_ipc_list)(struct smr_ep *ep);
};

static inline struct fid_peer_srx *smr_get_peer_srx(struct smr_ep *ep)
{
	return container_of(ep->srx, struct fid_peer_srx, ep_fid);
}

#define smr_ep_rx_flags(smr_ep) ((smr_ep)->util_ep.rx_op_flags)
#define smr_ep_tx_flags(smr_ep) ((smr_ep)->util_ep.tx_op_flags)

static inline int smr_mmap_name(char *shm_name, const char *ep_name,
				uint64_t msg_id)
{
	return snprintf(shm_name, SMR_NAME_MAX - 1, "%s_%ld",
			ep_name, msg_id);
}

int smr_srx_context(struct fid_domain *domain, struct fi_rx_attr *attr,
		struct fid_ep **rx_ep, void *context);

int smr_endpoint(struct fid_domain *domain, struct fi_info *info,
		  struct fid_ep **ep, void *context);
void smr_ep_exchange_fds(struct smr_ep *ep, int64_t id);

int smr_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		struct fid_cq **cq_fid, void *context);
int smr_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
		  struct fid_cntr **cntr_fid, void *context);

int64_t smr_verify_peer(struct smr_ep *ep, fi_addr_t fi_addr);

void smr_format_pend_resp(struct smr_tx_entry *pend, struct smr_cmd *cmd,
			  void *context, struct ofi_mr **mr,
			  const struct iovec *iov, uint32_t iov_count,
			  uint64_t op_flags, int64_t id, struct smr_resp *resp);
void smr_generic_format(struct smr_cmd *cmd, int64_t peer_id, uint32_t op,
			uint64_t tag, uint64_t data, uint64_t op_flags);
size_t smr_copy_to_sar(struct smr_freestack *sar_pool, struct smr_resp *resp,
		       struct smr_cmd *cmd, struct ofi_mr **mr,
		       const struct iovec *iov, size_t count,
		       size_t *bytes_done);
size_t smr_copy_from_sar(struct smr_freestack *sar_pool, struct smr_resp *resp,
			 struct smr_cmd *cmd, struct ofi_mr **mr,
			 const struct iovec *iov, size_t count,
			 size_t *bytes_done);
int smr_select_proto(void **desc, size_t iov_count, bool cma_avail,
		     uint32_t op, uint64_t total_len, uint64_t op_flags);
typedef ssize_t (*smr_proto_func)(struct smr_ep *ep, struct smr_region *peer_smr,
		int64_t id, int64_t peer_id, uint32_t op, uint64_t tag,
		uint64_t data, uint64_t op_flags, struct ofi_mr **desc,
		const struct iovec *iov, size_t iov_count, size_t total_len,
		void *context, struct smr_cmd *cmd);
extern smr_proto_func smr_proto_ops[smr_src_max];

int smr_write_err_comp(struct util_cq *cq, void *context,
		       uint64_t flags, uint64_t tag, uint64_t err);
int smr_complete_tx(struct smr_ep *ep, void *context, uint32_t op,
		    uint64_t flags);
int smr_complete_rx(struct smr_ep *ep, void *context, uint32_t op,
		    uint64_t flags, size_t len, void *buf, int64_t id,
		    uint64_t tag, uint64_t data);

static inline uint64_t smr_rx_cq_flags(uint32_t op, uint64_t rx_flags,
				       uint16_t op_flags)
{
	rx_flags |= ofi_rx_cq_flags(op);
	if (op_flags & SMR_REMOTE_CQ_DATA)
		rx_flags |= FI_REMOTE_CQ_DATA;
	return rx_flags;
}

void smr_ep_progress(struct util_ep *util_ep);

static inline bool smr_vma_enabled(struct smr_ep *ep,
				   struct smr_region *peer_smr)
{
	if (ep->region == peer_smr)
		return (ep->region->cma_cap_self == SMR_VMA_CAP_ON ||
			ep->region->xpmem_cap_self == SMR_VMA_CAP_ON);
	else
		return (ep->region->cma_cap_peer == SMR_VMA_CAP_ON ||
			peer_smr->xpmem_cap_self == SMR_VMA_CAP_ON);
}

static inline bool smr_ze_ipc_enabled(struct smr_region *smr,
				      struct smr_region *peer_smr)
{
	return (smr->flags & SMR_FLAG_IPC_SOCK) &&
	       (peer_smr->flags & SMR_FLAG_IPC_SOCK);
}

static inline struct smr_inject_buf *
smr_get_txbuf(struct smr_region *smr)
{
	struct smr_inject_buf *txbuf;

	pthread_spin_lock(&smr->lock);
	if (!smr_freestack_isempty(smr_inject_pool(smr)))
		txbuf = smr_freestack_pop(smr_inject_pool(smr));
	else
		txbuf = NULL;
	pthread_spin_unlock(&smr->lock);
	return txbuf;
}

static inline void
smr_release_txbuf(struct smr_region *smr,
		  struct smr_inject_buf *tx_buf)
{
	pthread_spin_lock(&smr->lock);
	smr_freestack_push(smr_inject_pool(smr), tx_buf);
	pthread_spin_unlock(&smr->lock);
}

int smr_unexp_start(struct fi_peer_rx_entry *rx_entry);

void smr_progress_ipc_list(struct smr_ep *ep);
static inline void smr_progress_ipc_list_noop(struct smr_ep *ep)
{
	// noop
}

#endif
