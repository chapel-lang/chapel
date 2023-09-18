/*
 * Copyright (c) 2018-2022 Amazon.com, Inc. or its affiliates. All rights reserved.
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

#include "efa_mr.h"
#include "efa_shm.h"
#include "efa_hmem.h"
#include "efa_device.h"
#include "efa_domain.h"
#include "efa_errno.h"
#include "efa_user_info.h"
#include "efa_fork_support.h"
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

#define EFA_EP_TYPE_IS_DGRAM(_info) \
	(_info && _info->ep_attr && (_info->ep_attr->type == FI_EP_DGRAM))

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

extern struct fi_provider efa_prov;
extern struct util_prov efa_util_prov;

struct efa_fabric {
	struct util_fabric	util_fabric;
	struct fid_fabric *shm_fabric;
#ifdef EFA_PERF_ENABLED
	struct ofi_perfset perf_set;
#endif
};


struct efa_ah {
	uint8_t		gid[EFA_GID_LEN]; /* efa device GID */
	struct ibv_ah	*ibv_ah; /* created by ibv_create_ah() using GID */
	uint16_t	ahn; /* adress handle number */
	int		refcnt; /* reference counter. Multiple efa_conn can share an efa_ah */
	UT_hash_handle	hh; /* hash map handle, link all efa_ah with efa_ep->ah_map */
};

static inline
int efa_str_to_ep_addr(const char *node, const char *service, struct efa_ep_addr *addr)
{
	int ret;

	if (!node)
		return -FI_EINVAL;

	memset(addr, 0, sizeof(*addr));

	ret = inet_pton(AF_INET6, node, addr->raw);
	if (ret != 1)
		return -FI_EINVAL;
	if (service)
		addr->qpn = atoi(service);

	return 0;
}


struct efa_conn {
	struct efa_ah		*ah;
	struct efa_ep_addr	*ep_addr;
	/* for FI_AV_TABLE, fi_addr is same as util_av_fi_addr,
	 * for FI_AV_MAP, fi_addr is pointer to efa_conn; */
	fi_addr_t		fi_addr;
	fi_addr_t		util_av_fi_addr;
	struct rdm_peer		rdm_peer;
};

struct efa_wc {
	struct ibv_wc		ibv_wc;
	/* Source address */
	uint16_t		efa_ah;
};

struct efa_wce {
	struct slist_entry	entry;
	struct efa_wc		wc;
};

typedef void (*efa_cq_read_entry)(struct ibv_cq_ex *ibv_cqx, int index, void *buf);

struct efa_cq {
	struct util_cq		util_cq;
	struct efa_domain	*domain;
	size_t			entry_size;
	efa_cq_read_entry	read_entry;
	ofi_spin_t		lock;
	struct ofi_bufpool	*wce_pool;
	uint32_t	flags; /* User defined capability mask */

	struct ibv_cq_ex	*ibv_cq_ex;
};

struct efa_qp {
	struct ibv_qp	*ibv_qp;
	struct ibv_qp_ex *ibv_qp_ex;
	struct efa_ep	*ep;
	uint32_t	qp_num;
	uint32_t	qkey;
};

struct efa_ep {
	struct util_ep		util_ep;
	struct efa_domain	*domain;
	struct efa_qp		*qp;
	struct efa_cq		*rcq;
	struct efa_cq		*scq;
	struct efa_av		*av;
	struct fi_info		*info;
	size_t			rnr_retry;
	void			*src_addr;
	struct ibv_send_wr	xmit_more_wr_head;
	struct ibv_send_wr	*xmit_more_wr_tail;
	struct ibv_recv_wr	recv_more_wr_head;
	struct ibv_recv_wr	*recv_more_wr_tail;
	struct ofi_bufpool	*send_wr_pool;
	struct ofi_bufpool	*recv_wr_pool;
	struct ibv_ah		*self_ah;
	int			hmem_p2p_opt; /* what to do for hmem transfers */
	bool			util_ep_initialized;
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
	/* cur_reverse_av is a map from (ahn + qpn) to current (latest) efa_conn.
	 * prv_reverse_av is a map from (ahn + qpn + connid) to all previous efa_conns.
	 * cur_reverse_av is faster to search because its key size is smaller
	 */
	struct efa_cur_reverse_av *cur_reverse_av;
	struct efa_prv_reverse_av *prv_reverse_av;
	struct efa_ah		*ah_map;
	struct util_av		util_av;
	enum fi_ep_type         ep_type;
};

struct efa_av_entry {
	uint8_t			ep_addr[EFA_EP_ADDR_LEN];
	struct efa_conn		conn;
};

struct efa_cur_reverse_av_key {
	uint16_t ahn;
	uint16_t qpn;
};

struct efa_cur_reverse_av {
	struct efa_cur_reverse_av_key key;
	struct efa_conn *conn;
	UT_hash_handle hh;
};

struct efa_prv_reverse_av_key {
	uint16_t ahn;
	uint16_t qpn;
	uint32_t connid;
};

struct efa_prv_reverse_av {
	struct efa_prv_reverse_av_key key;
	struct efa_conn *conn;
	UT_hash_handle hh;
};

#define EFA_DGRAM_CONNID (0x0)

static inline struct efa_av *rxr_ep_av(struct rxr_ep *ep)
{
	return container_of(ep->util_ep.av, struct efa_av, util_av);
}

extern struct fi_ops_cm efa_ep_cm_ops;
extern struct fi_ops_msg efa_ep_msg_ops;
extern struct fi_ops_rma efa_ep_rma_ops;

ssize_t efa_rma_post_read(struct efa_ep *ep, const struct fi_msg_rma *msg,
			  uint64_t flags, bool self_comm);

const struct fi_info *efa_get_efa_info(const char *domain_name);
int efa_domain_open(struct fid_fabric *fabric_fid, struct fi_info *info,
		    struct fid_domain **domain_fid, void *context);
int efa_ep_open(struct fid_domain *domain_fid, struct fi_info *info,
		struct fid_ep **ep_fid, void *context);
int efa_av_open(struct fid_domain *domain_fid, struct fi_av_attr *attr,
		struct fid_av **av_fid, void *context);
int efa_cq_open(struct fid_domain *domain_fid, struct fi_cq_attr *attr,
		struct fid_cq **cq_fid, void *context);
int efa_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fabric_fid,
	       void *context);

/* AV sub-functions */
int efa_av_insert_one(struct efa_av *av, struct efa_ep_addr *addr,
		      fi_addr_t *fi_addr, uint64_t flags, void *context);

struct efa_conn *efa_av_addr_to_conn(struct efa_av *av, fi_addr_t fi_addr);

/* Caller must hold cq->inner_lock. */
void efa_cq_inc_ref_cnt(struct efa_cq *cq, uint8_t sub_cq_idx);
/* Caller must hold cq->inner_lock. */
void efa_cq_dec_ref_cnt(struct efa_cq *cq, uint8_t sub_cq_idx);

fi_addr_t efa_av_reverse_lookup_rdm(struct efa_av *av, uint16_t ahn, uint16_t qpn, struct rxr_pkt_entry *pkt_entry);

fi_addr_t efa_av_reverse_lookup_dgram(struct efa_av *av, uint16_t ahn, uint16_t qpn);

int efa_prov_initialize(void);

void efa_prov_finalize(void);

ssize_t efa_post_flush(struct efa_ep *ep, struct ibv_send_wr **bad_wr);

ssize_t efa_cq_readfrom(struct fid_cq *cq_fid, void *buf, size_t count, fi_addr_t *src_addr);

ssize_t efa_cq_readerr(struct fid_cq *cq_fid, struct fi_cq_err_entry *entry, uint64_t flags);

static inline
bool efa_ep_support_rdma_read(struct fid_ep *ep_fid)
{
	struct efa_ep *efa_ep;

	if (!rxr_env.use_device_rdma)
		return 0;

	efa_ep = container_of(ep_fid, struct efa_ep, util_ep.ep_fid);
	return efa_ep->domain->device->device_caps & EFADV_DEVICE_ATTR_CAPS_RDMA_READ;
}

static inline
bool efa_ep_support_rnr_retry_modify(struct fid_ep *ep_fid)
{
#ifdef HAVE_CAPS_RNR_RETRY
	struct efa_ep *efa_ep;

	efa_ep = container_of(ep_fid, struct efa_ep, util_ep.ep_fid);
	return efa_ep->domain->device->device_caps & EFADV_DEVICE_ATTR_CAPS_RNR_RETRY;
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
	       (peer->extra_info[0] & RXR_EXTRA_FEATURE_RDMA_READ);
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
	       (peer->extra_info[0] & RXR_EXTRA_FEATURE_DELIVERY_COMPLETE);
}

static inline
bool efa_both_support_rdma_read(struct rxr_ep *ep, struct rdm_peer *peer)
{
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
 * 2. If the peer requested to keep the header length constant through
 * out the communiciton, endpoint will include the raw address in the
 * header even afer received handshake from a header to conform to the
 * request. Usually, peer has this request because they are in zero
 * copy receive mode, which requires the packet header size to remain
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

	return peer->extra_info[0] & RXR_EXTRA_REQUEST_CONSTANT_HEADER_LENGTH;
}


/**
 * @brief determines whether a peer needs the endpoint to include
 * connection ID (connid) in packet header.
 *
 * Connection ID is a 4 bytes random integer identifies an endpoint.
 * Including connection ID in a packet's header allows peer to
 * identify sender of the packet. It is necessary because device
 * only report GID+QPN of a received packet, while QPN may be reused
 * accross device endpoint teardown and initialization.
 *
 * EFA uses qkey as connection ID.
 *
 * @params[in]	peer	pointer to rdm_peer
 * @return	a boolean indicating whether the peer needs connection ID
 */
static inline
bool rxr_peer_need_connid(struct rdm_peer *peer)
{
	return (peer->flags & RXR_PEER_HANDSHAKE_RECEIVED) &&
	       (peer->extra_info[0] & RXR_EXTRA_REQUEST_CONNID_HEADER);
}

static inline
size_t efa_max_rdma_size(struct fid_ep *ep_fid)
{
	struct efa_ep *efa_ep;

	efa_ep = container_of(ep_fid, struct efa_ep, util_ep.ep_fid);
	return efa_ep->domain->device->max_rdma_size;
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

/*
 * @brief: check whether we should use p2p for this transaction
 *
 * @param[in]	ep	efa_ep
 * @param[in]	efa_mr	memory registration struct
 *
 * @return: 0 if p2p should not be used, 1 if it should, and negative FI code
 * if the transfer should fail.
 */
static inline int efa_ep_use_p2p(struct efa_ep *ep, struct efa_mr *efa_mr)
{
	if (!efa_mr)
		return 0;

	/*
	 * always send from host buffers if we have a descriptor
	 */
	if (efa_mr->peer.iface == FI_HMEM_SYSTEM)
		return 1;

	if (ep->domain->hmem_info[efa_mr->peer.iface].p2p_supported)
		return (ep->hmem_p2p_opt != FI_HMEM_P2P_DISABLED);

	if (ep->hmem_p2p_opt == FI_HMEM_P2P_REQUIRED) {
		EFA_WARN(FI_LOG_EP_CTRL,
			 "Peer to peer support is currently required, but not available.");
		return -FI_ENOSYS;
	}

	return 0;
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

/* Performance counter declarations */
#ifdef EFA_PERF_ENABLED
#define EFA_PERF_FOREACH(DECL)	\
	DECL(perf_efa_tx),	\
	DECL(perf_efa_recv),	\
	DECL(efa_perf_size)	\

enum efa_perf_counters {
	EFA_PERF_FOREACH(OFI_ENUM_VAL)
};

extern const char *efa_perf_counters_str[];

static inline void efa_perfset_start(struct rxr_ep *ep, size_t index)
{
	struct efa_domain *domain = rxr_ep_domain(ep);
	struct efa_fabric *fabric = container_of(domain->util_domain.fabric,
						 struct efa_fabric,
						 util_fabric);
	ofi_perfset_start(&fabric->perf_set, index);
}

static inline void efa_perfset_end(struct rxr_ep *ep, size_t index)
{
	struct efa_domain *domain = rxr_ep_domain(ep);
	struct efa_fabric *fabric = container_of(domain->util_domain.fabric,
						 struct efa_fabric,
						 util_fabric);
	ofi_perfset_end(&fabric->perf_set, index);
}
#else
#define efa_perfset_start(ep, index) do {} while (0)
#define efa_perfset_end(ep, index) do {} while (0)
#endif

#endif /* EFA_H */
