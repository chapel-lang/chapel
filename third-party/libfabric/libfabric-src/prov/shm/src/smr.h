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
#include <rdma/fi_ext.h>

#include <ofi.h>
#include <ofi_enosys.h>
#include <ofi_shm.h>
#include <ofi_rbuf.h>
#include <ofi_list.h>
#include <ofi_signal.h>
#include <ofi_epoll.h>
#include <ofi_util.h>
#include <ofi_atomic.h>
#include <ofi_iov.h>
#include <ofi_mr.h>
#include <ofi_lock.h>

#ifndef _SMR_H_
#define _SMR_H_

struct smr_env {
	size_t sar_threshold;
	int disable_cma;
	int use_dsa_sar;
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

struct smr_rx_entry {
	struct fi_peer_rx_entry	peer_entry;
	struct iovec		iov[SMR_IOV_LIMIT];
	void			*desc[SMR_IOV_LIMIT];
	int64_t			peer_id;
	uint64_t		ignore;
	int			multi_recv_ref;
	uint64_t		err;
	enum fi_hmem_iface	iface;
	uint64_t		device;
};

struct smr_tx_entry {
	struct smr_cmd	cmd;
	int64_t		peer_id;
	void		*context;
	struct iovec	iov[SMR_IOV_LIMIT];
	uint32_t	iov_count;
	uint64_t	op_flags;
	size_t		bytes_done;
	int		next;
	void		*map_ptr;
	struct smr_ep_name *map_name;
	enum fi_hmem_iface	iface;
	uint64_t		device;
	int			fd;
};

struct smr_sar_entry {
	struct dlist_entry	entry;
	struct smr_cmd		cmd;
	struct fi_peer_rx_entry	*rx_entry;
	size_t			bytes_done;
	int			next;
	struct iovec		iov[SMR_IOV_LIMIT];
	size_t			iov_count;
	enum fi_hmem_iface	iface;
	uint64_t		device;
};

struct smr_cq {
	struct util_cq util_cq;
	struct fid_peer_cq *peer_cq;
};

typedef int (*smr_rx_comp_func)(struct smr_cq *cq, void *context,
		uint64_t flags, size_t len, void *buf,
		fi_addr_t fi_addr, uint64_t tag, uint64_t data);

struct smr_match_attr {
	fi_addr_t	id;
	uint64_t	tag;
	uint64_t	ignore;
};

static inline int smr_match_id(fi_addr_t addr, fi_addr_t match_addr)
{
	return (addr == FI_ADDR_UNSPEC) || (match_addr == FI_ADDR_UNSPEC) ||
		(addr == match_addr);
}

static inline int smr_match_tag(uint64_t tag, uint64_t ignore, uint64_t match_tag)
{
	return ((tag | ignore) == (match_tag | ignore));
}

static inline enum fi_hmem_iface smr_get_mr_hmem_iface(struct util_domain *domain,
				void **desc, uint64_t *device)
{
	if (!(domain->mr_mode & FI_MR_HMEM) || !desc || !*desc) {
		*device = 0;
		return FI_HMEM_SYSTEM;
	}

	*device = ((struct ofi_mr *) *desc)->device;
	return ((struct ofi_mr *) *desc)->iface;
}

static inline uint64_t smr_get_mr_flags(void **desc)
{
	assert(desc && *desc);
	return ((struct ofi_mr *) *desc)->flags;
}

struct smr_cmd_ctx {
	struct dlist_entry entry;
	struct smr_ep *ep;
	struct smr_cmd cmd;
};

OFI_DECLARE_FREESTACK(struct smr_rx_entry, smr_recv_fs);
OFI_DECLARE_FREESTACK(struct smr_cmd_ctx, smr_cmd_ctx_fs);
OFI_DECLARE_FREESTACK(struct smr_tx_entry, smr_pend_fs);
OFI_DECLARE_FREESTACK(struct smr_sar_entry, smr_sar_fs);

struct smr_queue {
	struct dlist_entry list;
	dlist_func_t *match_func;
};

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
	int			device_fds[ZE_MAX_DEVICES];
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

struct smr_srx_ctx {
	struct fid_peer_srx	peer_srx;
	struct smr_queue	recv_queue;
	struct smr_queue	trecv_queue;
	bool			dir_recv;
	size_t			min_multi_recv_size;
	uint64_t		rx_op_flags;
	uint64_t		rx_msg_flags;

	struct smr_cq		*cq;
	struct smr_queue	unexp_msg_queue;
	struct smr_queue	unexp_tagged_queue;
	struct smr_recv_fs	*recv_fs;
	ofi_spin_t		lock;
};

struct smr_rx_entry *smr_alloc_rx_entry(struct smr_srx_ctx *srx);

struct smr_ep {
	struct util_ep		util_ep;
	smr_rx_comp_func	rx_comp;
	size_t			tx_size;
	size_t			rx_size;
	const char		*name;
	uint64_t		msg_id;
	struct smr_region	*volatile region;
	//if double locking is needed, shm region lock must
	//be acquired before any shm EP locks
	ofi_spin_t		tx_lock;

	struct fid_ep		*srx;
	struct smr_cmd_ctx_fs	*cmd_ctx_fs;
	struct smr_pend_fs	*pend_fs;
	struct smr_sar_fs	*sar_fs;

	struct dlist_entry	sar_list;

	int			ep_idx;
	struct smr_sock_info	*sock_info;
	void			*dsa_context;
};

static inline struct smr_srx_ctx *smr_get_smr_srx(struct smr_ep *ep)
{
	return (struct smr_srx_ctx *) ep->srx->fid.context;
}

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
			  void *context, enum fi_hmem_iface iface, uint64_t device,
			  const struct iovec *iov, uint32_t iov_count,
			  uint64_t op_flags, int64_t id, struct smr_resp *resp);
void smr_generic_format(struct smr_cmd *cmd, int64_t peer_id, uint32_t op,
			uint64_t tag, uint64_t data, uint64_t op_flags);
size_t smr_copy_to_sar(struct smr_freestack *sar_pool, struct smr_resp *resp,
		       struct smr_cmd *cmd, enum fi_hmem_iface, uint64_t device,
		       const struct iovec *iov, size_t count,
		       size_t *bytes_done, int *next);
size_t smr_copy_from_sar(struct smr_freestack *sar_pool, struct smr_resp *resp,
			 struct smr_cmd *cmd, enum fi_hmem_iface iface,
			 uint64_t device, const struct iovec *iov, size_t count,
			 size_t *bytes_done, int *next);

int smr_select_proto(bool use_ipc, bool cma_avail, enum fi_hmem_iface iface,
		     uint32_t op, uint64_t total_len, uint64_t op_flags);
typedef ssize_t (*smr_proto_func)(struct smr_ep *ep, struct smr_region *peer_smr,
		int64_t id, int64_t peer_id, uint32_t op, uint64_t tag,
		uint64_t data, uint64_t op_flags, enum fi_hmem_iface iface,
		uint64_t device, const struct iovec *iov, size_t iov_count,
		size_t total_len, void *context);
extern smr_proto_func smr_proto_ops[smr_src_max];

int smr_write_err_comp(struct util_cq *cq, void *context,
		       uint64_t flags, uint64_t tag, uint64_t err);
int smr_complete_tx(struct smr_ep *ep, void *context, uint32_t op,
		    uint64_t flags);
int smr_complete_rx(struct smr_ep *ep, void *context, uint32_t op,
		    uint64_t flags, size_t len, void *buf, int64_t id,
		    uint64_t tag, uint64_t data);
int smr_rx_comp(struct smr_cq *cq, void *context, uint64_t flags, size_t len,
		void *buf, fi_addr_t fi_addr, uint64_t tag, uint64_t data);
int smr_rx_src_comp(struct smr_cq *cq, void *context, uint64_t flags,
		    size_t len, void *buf, fi_addr_t fi_addr, uint64_t tag,
		    uint64_t data);

static inline uint64_t smr_rx_cq_flags(uint32_t op, uint64_t rx_flags,
				       uint16_t op_flags)
{
	rx_flags |= ofi_rx_cq_flags(op);
	if (op_flags & SMR_REMOTE_CQ_DATA)
		rx_flags |= FI_REMOTE_CQ_DATA;
	return rx_flags;
}


bool smr_adjust_multi_recv(struct smr_srx_ctx *srx,
			   struct fi_peer_rx_entry *rx_entry, size_t len);
void smr_init_rx_entry(struct smr_rx_entry *entry, const struct iovec *iov,
		       void **desc, size_t count, fi_addr_t addr,
		       void *context, uint64_t tag, uint64_t flags);
struct smr_rx_entry *smr_get_recv_entry(struct smr_srx_ctx *srx,
		const struct iovec *iov, void **desc, size_t count, fi_addr_t addr,
		void *context, uint64_t tag, uint64_t ignore, uint64_t flags);

void smr_ep_progress(struct util_ep *util_ep);

static inline bool smr_cma_enabled(struct smr_ep *ep,
				   struct smr_region *peer_smr)
{
	if (ep->region == peer_smr)
		return ep->region->cma_cap_self == SMR_CMA_CAP_ON;
	else
		return ep->region->cma_cap_peer == SMR_CMA_CAP_ON;
}

static inline bool smr_ze_ipc_enabled(struct smr_region *smr,
				      struct smr_region *peer_smr)
{
	return (smr->flags & SMR_FLAG_IPC_SOCK) &&
	       (peer_smr->flags & SMR_FLAG_IPC_SOCK);
}

static inline int smr_cma_loop(pid_t pid, struct iovec *local,
			unsigned long local_cnt, struct iovec *remote,
			unsigned long remote_cnt, unsigned long flags,
			size_t total, bool write)
{
	ssize_t ret;

	while (1) {
		if (write)
			ret = ofi_process_vm_writev(pid, local, local_cnt, remote,
						    remote_cnt, flags);
		else
			ret = ofi_process_vm_readv(pid, local, local_cnt, remote,
						   remote_cnt, flags);
		if (ret < 0) {
			FI_WARN(&smr_prov, FI_LOG_EP_CTRL,
				"CMA error %d\n", errno);
			return -FI_EIO;
		}

		total -= ret;
		if (!total)
			return FI_SUCCESS;

		ofi_consume_iov(local, &local_cnt, (size_t) ret);
		ofi_consume_iov(remote, &remote_cnt, (size_t) ret);
	}
}

int smr_unexp_start(struct fi_peer_rx_entry *rx_entry);
#endif
