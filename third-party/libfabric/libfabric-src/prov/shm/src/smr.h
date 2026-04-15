/*
 * Copyright (c) Intel Corporation, Inc.  All rights reserved.
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

#ifndef _SMR_H_
#define _SMR_H_

#include "smr_util.h"
#include "ofi_shm_p2p.h"
#include "ofi_util.h"

struct smr_ep {
	struct util_ep		util_ep;
	size_t			tx_size;
	size_t			rx_size;
	const char		*name;
	uint64_t		msg_id;
	struct smr_region	*volatile region;
	struct fid_peer_srx	*srx;
	struct smr_map		*map;
	struct ofi_bufpool	*cmd_ctx_pool;
	struct ofi_bufpool	*unexp_buf_pool;
	struct ofi_bufpool	*pend_pool;

	struct slist		overflow_list;
	struct dlist_entry	sar_list;
	struct dlist_entry	async_cpy_list;
	struct dlist_entry	unexp_cmd_list;
	size_t			min_multi_recv_size;

	int			ep_idx;
	bool			user_setname;
	enum ofi_shm_p2p_type	p2p_type;
	void			*dsa_context;
	void 			(*smr_progress_async)(struct smr_ep *ep);
};


struct smr_map {
	int64_t			cur_id;
	int 			num_peers;
	uint16_t		flags;
	struct ofi_rbmap	rbmap;
	struct smr_peer		peers[SMR_MAX_PEERS];
};

struct smr_av {
	struct util_av		util_av;
	struct smr_map		smr_map;
	size_t			used;
};

static inline struct smr_region *smr_peer_region(struct smr_ep *ep, int i)
{
	return ep->map->peers[i].region;
}

void smr_map_add(struct smr_map *map, const char *name, int64_t *id);
int smr_map_to_region(struct smr_map *map, int64_t id);
void smr_unmap_region(struct smr_map *map, int64_t id, bool found);
void smr_map_to_endpoint(struct smr_ep *ep, int64_t id);

static inline uintptr_t smr_local_to_peer(struct smr_ep *ep,
					  struct smr_region *peer_smr,
					  int64_t id, int64_t peer_id,
					  uintptr_t local_ptr)
{
	uint64_t offset = local_ptr - (uintptr_t) ep->region;

	return smr_peer_data(peer_smr)[peer_id].local_region + offset;
}

static inline uintptr_t smr_peer_to_peer(struct smr_ep *ep,
					 struct smr_region *peer_smr,
					 int64_t id, uintptr_t local_ptr)
{
	uint64_t offset = local_ptr - (uintptr_t) peer_smr;

	return (uintptr_t) peer_smr->base_addr + offset;
}

static inline uintptr_t smr_peer_to_owner(struct smr_ep *ep,
					  struct smr_region *peer_smr,
					  int64_t id, uintptr_t local_ptr)
{
	uint64_t offset = local_ptr - (uintptr_t) peer_smr;

	return (uintptr_t) peer_smr->base_addr + offset;
}

static inline void smr_return_cmd(struct smr_ep *ep, struct smr_cmd *cmd)
{
	struct smr_region *peer_smr = smr_peer_region(ep, cmd->hdr.rx_id);
	uintptr_t peer_ptr;
	int64_t pos;
	struct smr_return_entry *queue_entry;
	int ret;

	ret = smr_return_queue_next(smr_return_queue(peer_smr), &queue_entry,
				    &pos);
	if (ret == -FI_ENOENT) {
		/* return queue runs in parallel to command stack
		 * ie we will never run out of space
		 */
		assert(0);
		return;
	}

	peer_ptr = smr_peer_to_owner(ep, peer_smr, cmd->hdr.rx_id,
				     (uintptr_t) cmd);
	assert(peer_ptr >= (uintptr_t) peer_smr->base_addr &&
	       peer_ptr < (uintptr_t) peer_smr->base_addr +
	       peer_smr->total_size);
	queue_entry->ptr = peer_ptr;

	smr_return_queue_commit(queue_entry, pos);
}

extern struct fi_provider smr_prov;
extern struct fi_info smr_info;
extern struct util_prov smr_util_prov;
extern int smr_global_ep_idx; //protected by the ep_list_lock

int smr_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fabric,
	       void *context);

static inline int64_t smr_addr_lookup(struct util_av *av, fi_addr_t fiaddr)
{
	return *((int64_t *) ofi_av_get_addr(av, fiaddr));
}

int smr_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		    struct fid_domain **doqm, void *context);

int smr_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
		struct fid_eq **eq, void *context);

int smr_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
		struct fid_av **av, void *context);

int smr_query_atomic(struct fid_domain *domain, enum fi_datatype datatype,
		     enum fi_op op, struct fi_atomic_attr *attr,
		     uint64_t flags);

enum {
	SMR_TX_ENTRY,
	SMR_RX_ENTRY,
};

struct smr_pend_entry {
	struct dlist_entry		entry;
	struct fi_peer_rx_entry		*rx_entry;
	struct ofi_mr_entry		*ipc_entry;
	ofi_hmem_async_event_t		async_event;
	uint8_t				type;
	struct smr_cmd			*cmd;
	struct iovec			iov[SMR_IOV_LIMIT];
	size_t				iov_count;
	struct ofi_mr			*mr[SMR_IOV_LIMIT];
	size_t				bytes_done;
	void				*comp_ctx;
	uint64_t			comp_flags;
	int				sar_dir;
	ssize_t				(*sar_copy_fn)(
						struct smr_ep *ep,
						struct smr_pend_entry *pend);
};

struct smr_cmd_ctx {
	struct dlist_entry	entry;
	struct fi_peer_rx_entry *rx_entry;
	struct smr_ep		*ep;
	struct smr_pend_entry	*pend;
	struct smr_cmd		*cmd;
	struct smr_cmd		cmd_cpy;
	char			msg[SMR_MSG_DATA_LEN];
	struct slist		buf_list;
};

struct smr_domain {
	struct util_domain	util_domain;
	bool			fast_rma;
	struct ofi_mr_cache	*ipc_cache;
	struct fid_ep		rx_ep;
	struct fid_peer_srx	*srx;
};

#define SMR_PREFIX	"fi_shm://"
#define SMR_PREFIX_NS	"fi_ns://"

#define SMR_RMA_ORDER (OFI_ORDER_RAR_SET | OFI_ORDER_RAW_SET | FI_ORDER_RAS | \
		       OFI_ORDER_WAR_SET | OFI_ORDER_WAW_SET | FI_ORDER_WAS | \
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

struct smr_unexp_buf {
	struct slist_entry entry;
	char buf[SMR_SAR_SIZE];
};

#define smr_ep_rx_flags(smr_ep) ((smr_ep)->util_ep.rx_op_flags)
#define smr_ep_tx_flags(smr_ep) ((smr_ep)->util_ep.tx_op_flags)

static inline int smr_mmap_name(char *shm_name, const char *ep_name,
				uint64_t msg_id)
{
	return snprintf(shm_name, SMR_NAME_MAX - 1, "%s_%" PRIu64,
			ep_name, (uint64_t)(msg_id));
}

int smr_endpoint(struct fid_domain *domain, struct fi_info *info,
		 struct fid_ep **ep, void *context);

int smr_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		struct fid_cq **cq_fid, void *context);
int smr_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
		  struct fid_cntr **cntr_fid, void *context);

int64_t smr_verify_peer(struct smr_ep *ep, fi_addr_t fi_addr);

void smr_format_tx_pend(struct smr_pend_entry *pend, struct smr_cmd *cmd,
			void *context, struct ofi_mr **mr,
			const struct iovec *iov, uint32_t iov_count,
			uint64_t op_flags);
void smr_generic_format(struct smr_cmd *cmd, int64_t tx_id, int64_t rx_id,
			uint32_t op, uint64_t tag, uint64_t data,
			uint64_t op_flags);
size_t smr_copy_to_sar(struct smr_ep *ep, struct smr_region *smr,
		       struct smr_pend_entry *pend);
size_t smr_copy_from_sar(struct smr_ep *ep, struct smr_region *smr,
		         struct smr_pend_entry *pend);
int smr_select_proto(void **desc, size_t iov_count, bool cma_avail,
		     bool ipc_valid, uint32_t op, uint64_t total_len,
		     uint64_t op_flags);
typedef ssize_t (*smr_send_func)(
		struct smr_ep *ep, struct smr_region *peer_smr,
		int64_t tx_id, int64_t rx_id, uint32_t op, uint64_t tag,
		uint64_t data, uint64_t op_flags, struct ofi_mr **desc,
		const struct iovec *iov, size_t iov_count, size_t total_len,
		void *context, struct smr_cmd *cmd);
extern smr_send_func smr_send_ops[smr_proto_max];

int smr_write_err_comp(struct util_cq *cq, void *context,
		       uint64_t flags, uint64_t tag, int err);
int smr_complete_tx(struct smr_ep *ep, void *context, uint32_t op,
		    uint64_t flags);
int smr_complete_rx(struct smr_ep *ep, void *context, uint32_t op,
		    uint64_t flags, size_t len, void *buf, int64_t id,
		    uint64_t tag, uint64_t data);

static inline uint64_t smr_rx_cq_flags(uint64_t rx_flags, uint16_t op_flags)
{
	if (op_flags & SMR_REMOTE_CQ_DATA)
		rx_flags |= FI_REMOTE_CQ_DATA;
	return rx_flags;
}

void smr_ep_progress(struct util_ep *util_ep);

/* Returns whether any VMA interface is available */
static inline bool smr_vma_enabled(struct smr_ep *ep,
				   struct smr_region *peer_smr)
{
	return ep->region == peer_smr ? ep->region->self_vma_caps :
					ep->region->peer_vma_caps;
}

static inline void smr_set_ipc_valid(struct smr_ep *ep, uint64_t id)
{
	if (ofi_hmem_is_initialized(FI_HMEM_ZE) &&
	    ep->map->peers[id].pid_fd == -1)
		smr_peer_data(ep->region)[id].ipc_valid = 0;
        else
		smr_peer_data(ep->region)[id].ipc_valid = 1;
}

static inline bool smr_ipc_valid(struct smr_ep *ep, struct smr_region *peer_smr,
				 int64_t id, int64_t peer_id)
{
	return (smr_peer_data(ep->region)[id].ipc_valid &&
		smr_peer_data(peer_smr)[peer_id].ipc_valid);
}

static inline struct smr_freestack *smr_pend_sar_pool(
			struct smr_ep *ep, struct smr_pend_entry *pend)
{
	if (pend->type == SMR_TX_ENTRY)
		return smr_sar_pool(ep->region);
	return smr_sar_pool(smr_peer_region(ep, pend->cmd->hdr.rx_id));
}
void smr_free_sar_bufs(struct smr_ep *ep, struct smr_cmd *cmd,
		       struct smr_pend_entry *pending);

void smr_try_send_cmd(struct smr_ep *ep, struct smr_cmd *cmd);

int smr_unexp_start(struct fi_peer_rx_entry *rx_entry);

void smr_progress_async(struct smr_ep *ep);
static inline void smr_progress_async_noop(struct smr_ep *ep)
{
	// noop
}
ssize_t smr_copy_sar(struct smr_ep *ep, struct smr_pend_entry *pend);

#endif /* _SMR_H_ */