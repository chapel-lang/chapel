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

#define EFA_DEF_POOL_ALIGNMENT (8)
#define EFA_MEM_ALIGNMENT (64)

#define EFA_DEF_CQ_SIZE 1024
#define EFA_MR_IOV_LIMIT 1
#define EFA_MR_SUPPORTED_PERMISSIONS (FI_SEND | FI_RECV | FI_REMOTE_READ)

/*
 * Setting ibv_qp_attr.rnr_retry to this number when modifying qp
 * to cause firmware to retry indefininetly.
 */
#define EFA_RNR_INFINITE_RETRY 7

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
/* maximum name length for shm endpoint */
#define EFA_SHM_NAME_MAX	   (256)

extern int efa_mr_cache_enable;
extern size_t efa_mr_max_cached_count;
extern size_t efa_mr_max_cached_size;

extern struct fi_provider efa_prov;
extern struct util_prov efa_util_prov;

struct efa_fabric {
	struct util_fabric	util_fabric;
};

#define EFA_GID_LEN	16

struct efa_ep_addr {
	uint8_t			raw[EFA_GID_LEN];
	uint16_t		qpn;
	uint16_t		pad;
	uint32_t		qkey;
	struct efa_ep_addr	*next;
};

#define EFA_EP_ADDR_LEN sizeof(struct efa_ep_addr)

struct efa_ah {
	uint8_t		gid[EFA_GID_LEN]; /* efa device GID */
	struct ibv_ah	*ibv_ah; /* created by ibv_create_ah() using GID */
	uint16_t	ahn; /* adress handle number */
	int		refcnt; /* reference counter. Multiple efa_conn can share an efa_ah */
	UT_hash_handle	hh; /* hash map handle, link all efa_ah with efa_ep->ah_map */
};

struct efa_conn {
	struct efa_ah		*ah;
	struct efa_ep_addr	*ep_addr;
	/* for FI_AV_TABLE, fi_addr is same as util_av_fi_addr,
	 * for FI_AV_MAP, fi_addr is pointer to efa_conn; */
	fi_addr_t		fi_addr;
	fi_addr_t		util_av_fi_addr;
	struct rdm_peer		rdm_peer;
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
	struct ofi_mr_cache	*cache;
	struct efa_qp		**qp_table;
	size_t			qp_table_sz_m1;
};

/**
 * @brief get a pointer to struct efa_domain from a domain_fid
 *
 * @param[in]	domain_fid	a fid to a domain
 * @return	return the pointer to struct efa_domain
 */
static inline
struct efa_domain *efa_domain_from_fid(struct fid_domain *domain_fid)
{
	struct util_domain *util_domain;
	struct efa_domain_base *efa_domain_base;
	struct rxr_domain *rxr_domain;
	struct efa_domain *efa_domain;

	util_domain = container_of(domain_fid, struct util_domain,
				   domain_fid);
	efa_domain_base = container_of(util_domain, struct efa_domain_base,
				       util_domain.domain_fid);

	/*
	 * An rxr_domain fid was passed to the user if this is an RDM
	 * endpoint, otherwise it is an efa_domain fid.  This will be
	 * removed once the rxr and efa domain structures are combined.
	 */
	if (efa_domain_base->type == EFA_DOMAIN_RDM) {
		rxr_domain = (struct rxr_domain *)efa_domain_base;
		efa_domain = container_of(rxr_domain->rdm_domain, struct efa_domain,
					  util_domain.domain_fid);
	} else {
		assert(efa_domain_base->type == EFA_DOMAIN_DGRAM);
		efa_domain = (struct efa_domain *)efa_domain_base;
	}

	return efa_domain;
}

/**
 * @brief get efa domain type from domain fid
 *
 * @param[in]	domain_fid	a fid to a domain
 * @return	efa domain type, either EFA_DOMAIN_DGRAM or EFA_DOMAIN_RDM
 */
static inline
enum efa_domain_type efa_domain_get_type(struct fid_domain *domain_fid)
{
	struct util_domain *util_domain;
	struct efa_domain_base *efa_domain_base;

	util_domain = container_of(domain_fid, struct util_domain,
				   domain_fid);
	efa_domain_base = container_of(util_domain, struct efa_domain_base,
				       util_domain.domain_fid);

	return efa_domain_base->type;
}

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
	int			dev_idx;
	uint64_t		max_mr_size;
	uint16_t		inline_buf_size;
	uint16_t		max_wr_rdma_sge;
	uint32_t		max_rdma_size;
	uint32_t		device_caps;
};

struct efa_pd {
	struct ibv_pd	   *ibv_pd;
	int		   use_cnt;
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
	struct ibv_ah		*self_ah;
};

struct efa_send_wr {
	struct ibv_send_wr wr;
	struct ibv_sge sge[];
};

struct efa_recv_wr {
	struct ibv_recv_wr wr;
	struct ibv_sge sge[];
};

struct efa_av {
	struct fid_av		*shm_rdm_av;
	fi_addr_t		shm_rdm_addr_map[EFA_SHM_MAX_AV_COUNT];
	struct efa_domain       *domain;
	struct efa_ep           *ep;
	size_t			used;
	size_t			shm_used;
	enum fi_av_type		type;
	struct efa_reverse_av	*reverse_av;
	struct efa_ah		*ah_map;
	struct util_av		util_av;
	enum fi_ep_type         ep_type;
};

struct efa_av_entry {
	uint8_t			ep_addr[EFA_EP_ADDR_LEN];
	struct efa_conn		conn;
};

struct efa_ah_qpn {
	uint16_t ahn;
	uint16_t qpn;
};

struct efa_reverse_av {
	struct efa_ah_qpn key;
	struct efa_conn *conn;
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

ssize_t efa_rma_post_read(struct efa_ep *ep, const struct fi_msg_rma *msg,
			  uint64_t flags, bool self_comm);

extern fastlock_t pd_list_lock;
// This list has the same indicies as ctx_list.
extern struct efa_pd *pd_list;

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
int efa_av_insert_one(struct efa_av *av, struct efa_ep_addr *addr,
		      fi_addr_t *fi_addr, uint64_t flags, void *context);

struct efa_conn *efa_av_addr_to_conn(struct efa_av *av, fi_addr_t fi_addr);

/* Caller must hold cq->inner_lock. */
void efa_cq_inc_ref_cnt(struct efa_cq *cq, uint8_t sub_cq_idx);
/* Caller must hold cq->inner_lock. */
void efa_cq_dec_ref_cnt(struct efa_cq *cq, uint8_t sub_cq_idx);

fi_addr_t efa_ahn_qpn_to_addr(struct efa_av *av, uint16_t ahn, uint16_t qpn);

struct rdm_peer *efa_ahn_qpn_to_peer(struct efa_av *av, uint16_t ahn, uint16_t qpn);

struct fi_provider *init_lower_efa_prov();

ssize_t efa_post_flush(struct efa_ep *ep, struct ibv_send_wr **bad_wr);

ssize_t efa_cq_readfrom(struct fid_cq *cq_fid, void *buf, size_t count, fi_addr_t *src_addr);

ssize_t efa_cq_readerr(struct fid_cq *cq_fid, struct fi_cq_err_entry *entry, uint64_t flags);

/*
 * ON will avoid using huge pages for bounce buffers, so that the libibverbs
 * fork support can be used safely.
 */
enum efa_fork_support_status {
	EFA_FORK_SUPPORT_OFF = 0,
	EFA_FORK_SUPPORT_ON,
	EFA_FORK_SUPPORT_UNNEEDED,
};
extern enum efa_fork_support_status efa_fork_status;

bool efa_device_support_rdma_read(void);

static inline
bool efa_ep_support_rdma_read(struct fid_ep *ep_fid)
{
	struct efa_ep *efa_ep;

	efa_ep = container_of(ep_fid, struct efa_ep, util_ep.ep_fid);
	return efa_ep->domain->ctx->device_caps & EFADV_DEVICE_ATTR_CAPS_RDMA_READ;
}

static inline
bool efa_ep_support_rnr_retry_modify(struct fid_ep *ep_fid)
{
#ifdef HAVE_CAPS_RNR_RETRY
	struct efa_ep *efa_ep;

	efa_ep = container_of(ep_fid, struct efa_ep, util_ep.ep_fid);
	return efa_ep->domain->ctx->device_caps & EFADV_DEVICE_ATTR_CAPS_RNR_RETRY;
#else
	return false;
#endif
}

/**
 * @brief return whether this endpoint should write error cq entry for RNR.
 *
 * For an endpoint to write RNR completion, two conditions must be met:
 *
 * First, the end point must be able to receive RNR completion from rdma-core,
 * which means rnr_etry must be less then EFA_RNR_INFINITE_RETRY.
 *
 * Second, the app need to request this feature when opening endpoint
 * (by setting info->domain_attr->resource_mgmt to FI_RM_DISABLED).
 * The setting was saved as rxr_ep->handle_resource_management.
 *
 * @param[in]	ep	endpoint
 */
static inline
bool rxr_ep_should_write_rnr_completion(struct rxr_ep *ep)
{
	return (rxr_env.rnr_retry < EFA_RNR_INFINITE_RETRY) &&
		(ep->handle_resource_management == FI_RM_DISABLED);
}

static inline
bool efa_peer_support_rdma_read(struct rdm_peer *peer)
{
	/* RDMA READ is an extra feature defined in version 4 (the base version).
	 * Because it is an extra feature, an EP will assume the peer does not support
	 * it before a handshake packet was received.
	 */
	return (peer->flags & RXR_PEER_HANDSHAKE_RECEIVED) &&
	       (peer->features[0] & RXR_REQ_FEATURE_RDMA_READ);
}

static inline
bool rxr_peer_support_delivery_complete(struct rdm_peer *peer)
{
	/* FI_DELIVERY_COMPLETE is an extra feature defined
	 * in version 4 (the base version).
	 * Because it is an extra feature,
	 * an EP will assume the peer does not support
	 * it before a handshake packet was received.
	 */
	return (peer->flags & RXR_PEER_HANDSHAKE_RECEIVED) &&
	       (peer->features[0] & RXR_REQ_FEATURE_DELIVERY_COMPLETE);
}

static inline
bool efa_both_support_rdma_read(struct rxr_ep *ep, struct rdm_peer *peer)
{
	if (!rxr_env.use_device_rdma)
		return 0;

	return efa_ep_support_rdma_read(ep->rdm_ep) &&
	       (peer->is_self || efa_peer_support_rdma_read(peer));
}

/**
 * @brief determines whether a peer needs the endpoint to include
 * raw address int the req packet header.
 *
 * There are two cases a peer need the raw address in REQ packet header:
 *
 * 1. the initial packets to a peer should include the raw address,
 * because the peer might not have ep's address in its address vector
 * causing the peer to be unable to send packet back. Normally, after
 * an endpoint received a hanshake packet from a peer, it can stop
 * including raw address in packet header.
 *
 * 2. If the peer is in zero copy receive mode, endpoint will include the
 * raw address in the header even afer received handshake from a header. This
 * is because zero copy receive requires the packet header size to remain
 * the same.
 *
 * @params[in]	peer	pointer to rdm_peer
 * @return	a boolean indicating whether the peer needs the raw address header
 */
static inline
bool rxr_peer_need_raw_addr_hdr(struct rdm_peer *peer)
{
	if (OFI_UNLIKELY(!(peer->flags & RXR_PEER_HANDSHAKE_RECEIVED)))
		return true;

	return peer->features[0] & RXR_REQ_FEATURE_ZERO_COPY_RECEIVE;
}

static inline
size_t efa_max_rdma_size(struct fid_ep *ep_fid)
{
	struct efa_ep *efa_ep;

	efa_ep = container_of(ep_fid, struct efa_ep, util_ep.ep_fid);
	return efa_ep->domain->ctx->max_rdma_size;
}

static inline
struct rdm_peer *rxr_ep_get_peer(struct rxr_ep *ep, fi_addr_t addr)
{
	struct util_av_entry *util_av_entry;
	struct efa_av_entry *av_entry;

	if (OFI_UNLIKELY(addr == FI_ADDR_NOTAVAIL))
		return NULL;

	util_av_entry = ofi_bufpool_get_ibuf(ep->util_ep.av->av_entry_pool,
	                                     addr);
	av_entry = (struct efa_av_entry *)util_av_entry->data;
	return av_entry->conn.ep_addr ? &av_entry->conn.rdm_peer : NULL;
}

static inline bool efa_ep_is_cuda_mr(struct efa_mr *efa_mr)
{
	return efa_mr ? (efa_mr->peer.iface == FI_HMEM_CUDA): false;
}

/*
 * efa_is_cache_available() is a check to see whether a memory registration
 * cache is available to be used by this domain.
 *
 * Return value:
 *    return true if a memory registration cache exists in this domain.
 *    return false if a memory registration cache does not exist in this domain.
 */
static inline bool efa_is_cache_available(struct efa_domain *efa_domain)
{
	return efa_domain->cache;
}

#define RXR_REQ_OPT_HDR_ALIGNMENT 8
#define RXR_REQ_OPT_RAW_ADDR_HDR_SIZE (((sizeof(struct rxr_req_opt_raw_addr_hdr) + EFA_EP_ADDR_LEN - 1)/RXR_REQ_OPT_HDR_ALIGNMENT + 1) * RXR_REQ_OPT_HDR_ALIGNMENT)

/*
 * Per libfabric standard, the prefix must be a multiple of 8, hence the static assert
 */
#define RXR_MSG_PREFIX_SIZE (sizeof(struct rxr_pkt_entry) + sizeof(struct rxr_eager_msgrtm_hdr) + RXR_REQ_OPT_RAW_ADDR_HDR_SIZE)

#if defined(static_assert) && defined(__x86_64__)
static_assert(RXR_MSG_PREFIX_SIZE % 8 == 0, "message prefix size alignment check");
#endif

#endif /* EFA_H */
