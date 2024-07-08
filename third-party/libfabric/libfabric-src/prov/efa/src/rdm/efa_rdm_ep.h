/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef _EFA_RDM_EP_H
#define _EFA_RDM_EP_H

#include <ctype.h>
#include "efa.h"
#include "efa_tp.h"
#include "efa_base_ep.h"
#include "efa_rdm_rxe_map.h"

#define EFA_RDM_ERROR_MSG_BUFFER_LENGTH 1024

/** @brief Information of a queued copy.
 *
 * This struct is used when receiving buffer is on device.
 * Under such circumstance, batching a series copies to
 * do them at the same time can avoid memory barriers between
 * copies, and improve performance.
 */
struct efa_rdm_ep_queued_copy {
	struct efa_rdm_pke *pkt_entry;
	char *data;
	size_t data_size;
	size_t data_offset;
};

#define EFA_RDM_MAX_QUEUED_COPY (8)
/** @brief max number of concurrent send reuqests allowed by EFA device
 *
 * The value was from EFA device's attribute (device->efa_attr.max_sq_wr)
 */
#define EFA_RDM_EP_MAX_WR_PER_IBV_POST_SEND (4096)
#define EFA_RDM_EP_MAX_WR_PER_IBV_POST_RECV (8192)

struct efa_rdm_ep {
	struct efa_base_ep base_ep;

	/**
	 * Default to 0
	 */
	uint64_t host_id;

	/* per-version extra feature/request flag */
	uint64_t extra_info[EFA_RDM_MAX_NUM_EXINFO];

	/* shm provider fid */
	struct fid_ep *shm_ep;

	/*
	 * EFA RDM endpoint rx/tx queue sizes. These may be different from the core
	 * provider's rx/tx size and will either limit the number of possible
	 * receives/sends or allow queueing.
	 */
	size_t rx_size;
	size_t tx_size;
	size_t mtu_size;
	size_t rx_iov_limit;
	size_t tx_iov_limit;
	size_t inject_size;

	/* Endpoint's capability to support zero-copy rx */
	bool use_zcpy_rx;

	/* Application requested resource management support */
	int handle_resource_management;

	/* rx/tx queue size of core provider */
	size_t efa_max_outstanding_rx_ops;
	size_t efa_max_outstanding_tx_ops;
	size_t efa_rnr_queued_pkt_cnt;
	size_t max_data_payload_size;

	/* Resource management flag */
	uint64_t rm_full;

	/* application's ordering requirements */
	uint64_t msg_order;

	/* Application's maximum msg size hint */
	size_t max_msg_size;

	/* Applicaiton's message prefix size. */
	size_t msg_prefix_size;

	/* EFA RDM protocol's max header size */
	size_t max_proto_hdr_size;

	/* tx iov limit of EFA device */
	size_t efa_device_iov_limit;

	/* threshold to release multi_recv buffer */
	size_t min_multi_recv_size;

	/* buffer pool for send & recv */
	struct ofi_bufpool *efa_tx_pkt_pool;
	struct ofi_bufpool *efa_rx_pkt_pool;

	/* staging area for unexpected and out-of-order packets */
	struct ofi_bufpool *rx_unexp_pkt_pool;
	struct ofi_bufpool *rx_ooo_pkt_pool;

	/* staging area for read copy */
	struct ofi_bufpool *rx_readcopy_pkt_pool;
	int rx_readcopy_pkt_pool_used;
	int rx_readcopy_pkt_pool_max_used;

	/* datastructure to maintain send/recv states */
	struct ofi_bufpool *ope_pool;
	/* data structure to maintain pkt rx map */
	struct ofi_bufpool *map_entry_pool;
	/** a map between sender address + msg_id to RX entry */
	struct efa_rdm_rxe_map rxe_map;
	/*
	 * buffer pool for atomic response data, used by
	 * emulated fetch and compare atomic.
	 */
	struct ofi_bufpool *rx_atomrsp_pool;
	/* list of pre-posted recv buffers */
	struct dlist_entry rx_posted_buf_list;
	/* op entries with queued rnr packets */
	struct dlist_entry ope_queued_rnr_list;
	/* op entries with queued ctrl packets */
	struct dlist_entry ope_queued_ctrl_list;
	/* op entries with queued read requests */
	struct dlist_entry ope_queued_read_list;
	/* tx/rx_entries used by long CTS msg/write/read protocol
         * which have data to be sent */
	struct dlist_entry ope_longcts_send_list;
	/* read entries with data to be read */
	struct dlist_entry read_pending_list;
	/* list of #efa_rdm_peer that are in backoff due to RNR */
	struct dlist_entry peer_backoff_list;
	/* list of #efa_rdm_peer that will retry posting handshake pkt */
	struct dlist_entry handshake_queued_peer_list;

#if ENABLE_DEBUG
	/* tx/rx_entries waiting to receive data in
         * long CTS msg/read/write protocols */
	struct dlist_entry ope_recv_list;
	/* counter tracking ope_recv_list */
	size_t pending_recv_counter;

	/* rx packets being processed or waiting to be processed */
	struct dlist_entry rx_pkt_list;

	/* tx packets waiting for send completion */
	struct dlist_entry tx_pkt_list;

	size_t efa_total_posted_tx_ops;
	size_t send_comps;
	size_t recv_comps;
#endif
	/* track allocated rx_entries and tx_entries for endpoint cleanup */
	struct dlist_entry rxe_list;
	struct dlist_entry txe_list;

	/*
	 * number of posted RX packets for EFA device
	 */
	size_t efa_rx_pkts_posted;
	/*
	 * Number of RX packets to be posted by progress engine for EFA device.
	 * It exists because posting RX packets by bulk is more efficient.
	 */
	size_t efa_rx_pkts_to_post;

	/*
	 * Number of RX packets that are held (not released) by progress engine
	 * due to queued hmem copy or local read.
	 */
	size_t efa_rx_pkts_held;

	/* number of outstanding tx ops on efa device */
	size_t efa_outstanding_tx_ops;

	struct efa_rdm_ep_queued_copy queued_copy_vec[EFA_RDM_MAX_QUEUED_COPY];
	int queued_copy_num;
	int blocking_copy_rxe_num; /* number of RX entries that are using gdrcopy/cudaMemcpy */

	int	hmem_p2p_opt; /* what to do for hmem transfers */
	struct fid_ep *peer_srx_ep; /* support sharing receive context with peer providers */
	bool cuda_api_permitted; /**< whether end point is permitted to call CUDA API */
	bool shm_permitted; /* Whether the endpoint is allowed to use shared memory for intra-node communication */

	/* use_device_rdma:
	   Can be set via fi_setopt in API >= 1.18.
	   User can override default and setopt using environment variable FI_EFA_USE_DEVICE_RDMA (any API version)
	   When environment sets to true abort() if no hardware support.
	   When setopt sets to true, return error if no hardware support.
	   When setopt attempts to conflict with environment variable, return error.
	   Defaults to true when hardware allows.
	*/
	bool use_device_rdma;

	struct fi_info *user_info; /**< fi_info passed by user when calling fi_endpoint */
	bool sendrecv_in_order_aligned_128_bytes; /**< whether to support in order send/recv of each aligned 128 bytes memory region */
	bool write_in_order_aligned_128_bytes; /**< whether to support in order write of each aligned 128 bytes memory region */
	char err_msg[EFA_RDM_ERROR_MSG_BUFFER_LENGTH]; /* A large enough buffer to store CQ/EQ error data used by e.g. fi_cq_readerr */
	struct efa_rdm_pke **pke_vec;
};

int efa_rdm_ep_flush_queued_blocking_copy_to_hmem(struct efa_rdm_ep *ep);

#define efa_rdm_rx_flags(efa_rdm_ep) ((efa_rdm_ep)->base_ep.util_ep.rx_op_flags)
#define efa_rdm_tx_flags(efa_rdm_ep) ((efa_rdm_ep)->base_ep.util_ep.tx_op_flags)

struct efa_ep_addr *efa_rdm_ep_raw_addr(struct efa_rdm_ep *ep);

const char *efa_rdm_ep_raw_addr_str(struct efa_rdm_ep *ep, char *buf, size_t *buflen);

struct efa_ep_addr *efa_rdm_ep_get_peer_raw_addr(struct efa_rdm_ep *ep, fi_addr_t addr);

const char *efa_rdm_ep_get_peer_raw_addr_str(struct efa_rdm_ep *ep, fi_addr_t addr, char *buf, size_t *buflen);

struct efa_rdm_peer *efa_rdm_ep_get_peer(struct efa_rdm_ep *ep, fi_addr_t addr);

int32_t efa_rdm_ep_get_peer_ahn(struct efa_rdm_ep *ep, fi_addr_t addr);

struct efa_rdm_ope *efa_rdm_ep_alloc_txe(struct efa_rdm_ep *efa_rdm_ep,
					 struct efa_rdm_peer *peer,
					 const struct fi_msg *msg,
					 uint32_t op,
					 uint64_t tag,
					 uint64_t flags);

struct efa_rdm_ope *efa_rdm_ep_alloc_rxe(struct efa_rdm_ep *ep,
					   fi_addr_t addr, uint32_t op);

void efa_rdm_ep_record_tx_op_submitted(struct efa_rdm_ep *ep, struct efa_rdm_pke *pkt_entry);

void efa_rdm_ep_record_tx_op_completed(struct efa_rdm_ep *ep, struct efa_rdm_pke *pkt_entry);

static inline size_t efa_rdm_ep_get_rx_pool_size(struct efa_rdm_ep *ep)
{
	return MIN(ep->efa_max_outstanding_rx_ops, ep->rx_size);
}

static inline size_t efa_rdm_ep_get_tx_pool_size(struct efa_rdm_ep *ep)
{
	return MIN(ep->efa_max_outstanding_tx_ops, ep->tx_size);
}

static inline int efa_rdm_ep_need_sas(struct efa_rdm_ep *ep)
{
	return ep->msg_order & FI_ORDER_SAS;
}



/* Initialization functions */
int efa_rdm_ep_open(struct fid_domain *domain, struct fi_info *info,
		    struct fid_ep **ep, void *context);

/* EP sub-functions */
void efa_rdm_ep_progress(struct util_ep *util_ep);
void efa_rdm_ep_progress_internal(struct efa_rdm_ep *efa_rdm_ep);

int efa_rdm_ep_post_user_recv_buf(struct efa_rdm_ep *ep, struct efa_rdm_ope *rxe,
			      uint64_t flags);

struct efa_rdm_peer;

void efa_rdm_ep_queue_rnr_pkt(struct efa_rdm_ep *ep,
			      struct dlist_entry *list,
			      struct efa_rdm_pke *pkt_entry);

ssize_t efa_rdm_ep_post_queued_pkts(struct efa_rdm_ep *ep,
				    struct dlist_entry *pkts);

size_t efa_rdm_ep_get_memory_alignment(struct efa_rdm_ep *ep, enum fi_hmem_iface iface);

static inline
struct efa_domain *efa_rdm_ep_domain(struct efa_rdm_ep *ep)
{
	return container_of(ep->base_ep.util_ep.domain, struct efa_domain, util_domain);
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
 * The setting was saved as efa_rdm_ep->handle_resource_management.
 *
 * @param[in]	ep	endpoint
 */
static inline
bool efa_rdm_ep_should_write_rnr_completion(struct efa_rdm_ep *ep)
{
	return (efa_env.rnr_retry < EFA_RNR_INFINITE_RETRY) &&
		(ep->handle_resource_management == FI_RM_DISABLED);
}

/*
 * @brief: check whether we should use p2p for this transaction
 *
 * @param[in]	ep	efa_rdm_ep
 * @param[in]	efa_mr	memory registration struct
 *
 * @return: 0 if p2p should not be used, 1 if it should, and negative FI code
 * if the transfer should fail.
 */
static inline
int efa_rdm_ep_use_p2p(struct efa_rdm_ep *efa_rdm_ep, struct efa_mr *efa_mr)
{
	if (!efa_mr)
		return 0;

	/*
	 * always send from host buffers if we have a descriptor
	 */
	if (efa_mr->peer.iface == FI_HMEM_SYSTEM)
		return 1;

	if (efa_rdm_ep_domain(efa_rdm_ep)->hmem_info[efa_mr->peer.iface].p2p_supported_by_device)
		return (efa_rdm_ep->hmem_p2p_opt != FI_HMEM_P2P_DISABLED);

	if (efa_rdm_ep->hmem_p2p_opt == FI_HMEM_P2P_REQUIRED) {
		EFA_WARN(FI_LOG_EP_CTRL,
			 "Peer to peer support is currently required, but not available.\n");
		return -FI_ENOSYS;
	}

	return 0;
}

/*
 * @brief: check whether RDMA read is allowed and supported.
 *
 * @param[in]	endpoint	struct efa_rdm_ep*
 *
 * @return: true if rdma read is supported. false otherwise.
 */
static inline
bool efa_rdm_ep_support_rdma_read(struct efa_rdm_ep *ep)
{
	if (!ep->use_device_rdma)
		return false;
	return efa_rdm_ep_domain(ep)->device->device_caps & EFADV_DEVICE_ATTR_CAPS_RDMA_READ;
}

/*
 * @brief: check whether the endpoint supports rdma write
 *
 * @param[in]	endpoint	struct efa_rdm_ep*
 *
 * @return: true if rdma write is supported. false otherwise.
 */
static inline
bool efa_rdm_ep_support_rdma_write(struct efa_rdm_ep *ep)
{
	if (!ep->use_device_rdma)
		return false;

#if HAVE_CAPS_RDMA_WRITE
	return efa_rdm_ep_domain(ep)->device->device_caps & EFADV_DEVICE_ATTR_CAPS_RDMA_WRITE;
#else
	return false;
#endif
}

/**
 * @brief check whether endpoint was configured with FI_RMA capability
 * @return -FI_EOPNOTSUPP if FI_RMA wasn't requested, 0 if it was.
 */
static inline int efa_rdm_ep_cap_check_rma(struct efa_rdm_ep *ep) {
	if ((ep->user_info->caps & FI_RMA) == FI_RMA)
		return 0;
	EFA_WARN_ONCE(FI_LOG_EP_DATA, "Operation requires FI_RMA capability, which was not requested.");
	return -FI_EOPNOTSUPP;
}

/**
 * @brief check whether endpoint was configured with FI_ATOMIC capability
 * @return -FI_EOPNOTSUPP if FI_ATOMIC wasn't requested, 0 if it was.
 */
static inline int efa_rdm_ep_cap_check_atomic(struct efa_rdm_ep *ep) {
	if ((ep->user_info->caps & FI_ATOMIC) == FI_ATOMIC)
		return 0;
	EFA_WARN_ONCE(FI_LOG_EP_DATA, "Operation requires FI_ATOMIC capability, which was not requested.");
	return -FI_EOPNOTSUPP;
}

/**
 * @brief Get the peer_srx from ep
 *
 * @param ep efa rdm endpoint
 * @return struct fid_peer_srx* a ptr to the peer srx
 */
static inline struct fid_peer_srx *efa_rdm_ep_get_peer_srx(struct efa_rdm_ep *ep)
{
	return container_of(ep->peer_srx_ep, struct fid_peer_srx, ep_fid);
}

static inline struct util_srx_ctx *efa_rdm_ep_get_peer_srx_ctx(struct efa_rdm_ep *ep)
{
	return (struct util_srx_ctx *) ep->peer_srx_ep->fid.context;
}

ssize_t efa_rdm_ep_trigger_handshake(struct efa_rdm_ep *ep, struct efa_rdm_peer *peer);

ssize_t efa_rdm_ep_post_handshake(struct efa_rdm_ep *ep, struct efa_rdm_peer *peer);

void efa_rdm_ep_post_handshake_or_queue(struct efa_rdm_ep *ep,
				     struct efa_rdm_peer *peer);

static inline int efa_rdm_attempt_to_sync_memops(struct efa_rdm_ep *ep, void *buf, void *desc)
{
	int err = 0;
	struct efa_mr *efa_mr = (struct efa_mr *) desc;

	if (OFI_UNLIKELY(ep->cuda_api_permitted && efa_mr && efa_mr->needs_sync)) {
		err = cuda_set_sync_memops(buf);
		if (err) {
			EFA_WARN(FI_LOG_MR, "Unable to set memops for cuda ptr %p\n", buf);
			return err;
		}
		efa_mr->needs_sync = false;
	}

	return err;
}

static inline int efa_rdm_attempt_to_sync_memops_iov(struct efa_rdm_ep *ep, struct iovec *iov, void **desc, int num_desc)
{
	int err = 0, i;

	if (!desc)
		return err;

	for (i = 0; i < num_desc; i++) {
		err = efa_rdm_attempt_to_sync_memops(ep, iov[i].iov_base, (struct efa_mr *) desc[i]);
		if (err)
			return err;
	}

	return err;
}

static inline int efa_rdm_attempt_to_sync_memops_ioc(struct efa_rdm_ep *ep, struct fi_ioc *ioc, void **desc, int num_desc)
{
	int err = 0, i;

	if (!desc)
		return err;

	for (i = 0; i < num_desc; i++) {
		err = efa_rdm_attempt_to_sync_memops(ep, ioc[i].addr, (struct efa_mr *) desc[i]);
		if (err)
			return err;
	}

	return err;
}

#endif
