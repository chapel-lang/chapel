/*
 * Copyright (c) 2018-2020 Amazon.com, Inc. or its affiliates. All rights reserved.
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

#ifndef EFA_H
#define EFA_H

#include "config.h"

#include <asm/types.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <uthash.h>

#include <rdma/fabric.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_errno.h>

#include <infiniband/verbs.h>
#include <infiniband/efadv.h>

#include "ofi.h"
#include "ofi_enosys.h"
#include "ofi_list.h"
#include "ofi_util.h"
#include "ofi_file.h"

#include "rxr.h"
#define EFA_PROV_NAME "efa"

#define EFA_WARN(subsys, ...) FI_WARN(&efa_prov, subsys, __VA_ARGS__)
#define EFA_TRACE(subsys, ...) FI_TRACE(&efa_prov, subsys, __VA_ARGS__)
#define EFA_INFO(subsys, ...) FI_INFO(&efa_prov, subsys, __VA_ARGS__)
#define EFA_INFO_ERRNO(subsys, fn, errno) \
	EFA_INFO(subsys, fn ": %s(%d)\n", strerror(errno), errno)
#define EFA_WARN_ERRNO(subsys, fn, errno) \
	EFA_WARN(subsys, fn ": %s(%d)\n", strerror(errno), errno)
#define EFA_DBG(subsys, ...) FI_DBG(&efa_prov, subsys, __VA_ARGS__)

#define EFA_ABI_VER_MAX_LEN 8

#define EFA_WCE_CNT 1024

#define EFA_EP_TYPE_IS_RDM(_info) \
	(_info && _info->ep_attr && (_info->ep_attr->type == FI_EP_RDM))

#define EFA_MEM_ALIGNMENT (64)

#define EFA_DEF_CQ_SIZE 1024
#define EFA_MR_IOV_LIMIT 1
#define EFA_MR_SUPPORTED_PERMISSIONS (FI_SEND | FI_RECV | FI_REMOTE_READ)

/*
 * Multiplier to give some room in the device memory registration limits
 * to allow processes added to a running job to bootstrap.
 */
#define EFA_MR_CACHE_LIMIT_MULT (.9)

#define EFA_MIN_AV_SIZE (16384)

/*
 * Specific flags and attributes for shm provider
 */
#define EFA_SHM_MAX_AV_COUNT       (256)

extern int efa_mr_cache_enable;
extern size_t efa_mr_max_cached_count;
extern size_t efa_mr_max_cached_size;

extern struct fi_provider efa_prov;
extern struct util_prov efa_util_prov;

struct efa_fabric {
	struct util_fabric	util_fabric;
};

struct efa_ep_addr {
	uint8_t			raw[16];
	uint16_t		qpn;
	uint16_t		pad;
	uint32_t		qkey;
	struct efa_ep_addr	*next;
};

#define EFA_EP_ADDR_LEN sizeof(struct efa_ep_addr)

struct efa_ah {
	struct ibv_ah	*ibv_ah;
	uint16_t	ahn;
};

struct efa_conn {
	struct efa_ah		ah;
	struct efa_ep_addr	ep_addr;
};

/*
 * Common fields for the beginning of the efa_domain and rxr_domain structures.
 * This structure must be kept in sync with rxr_domain and efa_domain. This
 * will be removed when the rxr and efa domain structures are combined.
 */
struct efa_domain_base {
	struct util_domain	util_domain;
	enum efa_domain_type	type;
};

struct efa_domain {
	struct util_domain	util_domain;
	enum efa_domain_type	type;
	struct fid_domain	*shm_domain;
	struct efa_context	*ctx;
	struct ibv_pd		*ibv_pd;
	struct fi_info		*info;
	struct efa_fabric	*fab;
	struct ofi_mr_cache	cache;
	struct efa_qp		**qp_table;
	size_t			qp_table_sz_m1;
};

extern struct fi_ops_mr efa_domain_mr_ops;
extern struct fi_ops_mr efa_domain_mr_cache_ops;
int efa_mr_cache_entry_reg(struct ofi_mr_cache *cache,
			   struct ofi_mr_entry *entry);
void efa_mr_cache_entry_dereg(struct ofi_mr_cache *cache,
			      struct ofi_mr_entry *entry);

int efa_mr_reg_shm(struct fid_domain *domain_fid, struct iovec *iov,
		   uint64_t access, struct fid_mr **mr_fid);

struct efa_wc {
	struct ibv_wc		ibv_wc;
	/* Source address */
	uint16_t		efa_ah;
};

struct efa_wce {
	struct slist_entry	entry;
	struct efa_wc		wc;
};

typedef void (*efa_cq_read_entry)(struct efa_wc *wc, int index, void *buf);

struct efa_cq {
	struct util_cq		util_cq;
	struct efa_domain	*domain;
	size_t			entry_size;
	efa_cq_read_entry	read_entry;
	struct slist		wcq;
	fastlock_t		lock;
	struct ofi_bufpool	*wce_pool;

	struct ibv_cq		*ibv_cq;
};

struct efa_context {
	struct ibv_context	*ibv_ctx;
	uint64_t		max_mr_size;
	uint16_t		inline_buf_size;
	uint16_t		max_wr_rdma_sge;
	uint32_t		max_rdma_size;
	uint32_t		device_caps;
};

struct efa_qp {
	struct ibv_qp	*ibv_qp;
	struct ibv_qp_ex *ibv_qp_ex;
	struct efa_ep	*ep;
	uint32_t	qp_num;
	uint32_t	qkey;
};

/*
 * Descriptor returned for FI_HMEM peer memory registrations
 */
struct efa_mr_peer {
	enum fi_hmem_iface      iface;
	union {
		uint64_t        reserved;
		int             cuda;
	} device;
};

struct efa_mr {
	struct fid_mr		mr_fid;
	struct ibv_mr		*ibv_mr;
	struct efa_domain	*domain;
	/* Used only in MR cache */
	struct ofi_mr_entry	*entry;
	/* Used only in rdm */
	struct fid_mr		*shm_mr;
	struct efa_mr_peer	peer;
};

struct efa_ep {
	struct util_ep		util_ep;
	struct efa_domain	*domain;
	struct efa_qp		*qp;
	struct efa_cq		*rcq;
	struct efa_cq		*scq;
	struct efa_av		*av;
	struct fi_info		*info;
	void			*src_addr;
	struct ibv_send_wr	xmit_more_wr_head;
	struct ibv_send_wr	*xmit_more_wr_tail;
	struct ibv_recv_wr	recv_more_wr_head;
	struct ibv_recv_wr	*recv_more_wr_tail;
	struct ofi_bufpool	*send_wr_pool;
	struct ofi_bufpool	*recv_wr_pool;
};

struct efa_send_wr {
	struct ibv_send_wr wr;
	struct ibv_sge sge[0];
};

struct efa_recv_wr {
	struct ibv_recv_wr wr;
	struct ibv_sge sge[0];
};

typedef struct efa_conn *
	(*efa_addr_to_conn_func)
	(struct efa_av *av, fi_addr_t addr);

struct efa_av {
	struct fid_av		*shm_rdm_av;
	fi_addr_t		shm_rdm_addr_map[EFA_SHM_MAX_AV_COUNT];
	struct efa_domain       *domain;
	struct efa_ep           *ep;
	size_t			used;
	size_t			next;
	size_t			shm_used;
	enum fi_av_type		type;
	efa_addr_to_conn_func	addr_to_conn;
	struct efa_reverse_av	*reverse_av;
	struct efa_av_entry     *av_map;
	struct util_av		util_av;
	enum fi_ep_type         ep_type;
	/* Used only for FI_AV_TABLE */
	struct efa_conn         **conn_table;
};

struct efa_av_entry {
	uint8_t			ep_addr[EFA_EP_ADDR_LEN];
	fi_addr_t		rdm_addr;
	fi_addr_t		shm_rdm_addr;
	bool			local_mapping;
	UT_hash_handle		hh;
};

struct efa_ah_qpn {
	uint16_t ahn;
	uint16_t qpn;
};

struct efa_reverse_av {
	struct efa_ah_qpn key;
	fi_addr_t fi_addr;
	UT_hash_handle hh;
};

struct efa_ep_domain {
	char		*suffix;
	enum fi_ep_type	type;
	uint64_t	caps;
};

struct efa_device_attr {
	struct ibv_device_attr	ibv_attr;
	uint32_t		max_sq_wr;
	uint32_t		max_rq_wr;
	uint16_t		max_sq_sge;
	uint16_t		max_rq_sge;
};


static inline struct efa_av *rxr_ep_av(struct rxr_ep *ep)
{
	return container_of(ep->util_ep.av, struct efa_av, util_av);
}

#define align_down_to_power_of_2(x)		\
	({					\
		__typeof__(x) n = (x);		\
		while (n & (n - 1))		\
			n = n & (n - 1);	\
		n;				\
	})

extern const struct efa_ep_domain efa_rdm_domain;
extern const struct efa_ep_domain efa_dgrm_domain;

extern struct fi_ops_cm efa_ep_cm_ops;
extern struct fi_ops_msg efa_ep_msg_ops;
extern struct fi_ops_rma efa_ep_rma_ops;

int efa_device_init(void);
void efa_device_free(void);

struct efa_context **efa_device_get_context_list(int *num_ctx);
void efa_device_free_context_list(struct efa_context **list);

const struct fi_info *efa_get_efa_info(const char *domain_name);
int efa_domain_open(struct fid_fabric *fabric_fid, struct fi_info *info,
		    struct fid_domain **domain_fid, void *context);
int efa_ep_open(struct fid_domain *domain_fid, struct fi_info *info,
		struct fid_ep **ep_fid, void *context);
int efa_av_open(struct fid_domain *domain_fid, struct fi_av_attr *attr,
		struct fid_av **av_fid, void *context);
int efa_cq_open(struct fid_domain *domain_fid, struct fi_cq_attr *attr,
		struct fid_cq **cq_fid, void *context);

/* AV sub-functions */
int efa_av_insert_addr(struct efa_av *av, struct efa_ep_addr *addr,
		       fi_addr_t *fi_addr, uint64_t flags, void *context);

/* Caller must hold cq->inner_lock. */
void efa_cq_inc_ref_cnt(struct efa_cq *cq, uint8_t sub_cq_idx);
/* Caller must hold cq->inner_lock. */
void efa_cq_dec_ref_cnt(struct efa_cq *cq, uint8_t sub_cq_idx);

fi_addr_t efa_ahn_qpn_to_addr(struct efa_av *av, uint16_t ahn, uint16_t qpn);

struct fi_provider *init_lower_efa_prov();

ssize_t efa_cq_readfrom(struct fid_cq *cq_fid, void *buf, size_t count, fi_addr_t *src_addr);

ssize_t efa_cq_readerr(struct fid_cq *cq_fid, struct fi_cq_err_entry *entry, uint64_t flags);

static inline
bool efa_ep_support_rdma_read(struct fid_ep *ep_fid)
{
	struct efa_ep *efa_ep;

	efa_ep = container_of(ep_fid, struct efa_ep, util_ep.ep_fid);
	return efa_ep->domain->ctx->device_caps & EFADV_DEVICE_ATTR_CAPS_RDMA_READ;
}

static inline
bool efa_peer_support_rdma_read(struct rxr_peer *peer)
{
	/* RDMA READ is an extra feature defined in version 4 (the base version).
	 * Because it is an extra feature, an EP will assume the peer does not support
	 * it before a handshake packet was received.
	 */
	return (peer->flags & RXR_PEER_HANDSHAKE_RECEIVED) &&
	       (peer->features[0] & RXR_REQ_FEATURE_RDMA_READ);
}

static inline
bool efa_both_support_rdma_read(struct rxr_ep *ep, struct rxr_peer *peer)
{
	if (!rxr_env.use_device_rdma)
		return 0;

	return efa_ep_support_rdma_read(ep->rdm_ep) &&
	       (peer->is_self || efa_peer_support_rdma_read(peer));
}

static inline
size_t efa_max_rdma_size(struct fid_ep *ep_fid)
{
	struct efa_ep *efa_ep;

	efa_ep = container_of(ep_fid, struct efa_ep, util_ep.ep_fid);
	return efa_ep->domain->ctx->max_rdma_size;
}

#endif /* EFA_H */
