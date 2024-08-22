/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "efa.h"
#include "efa_av.h"
#include "efa_rdm_ep.h"
#include "efa_rdm_cq.h"
#include "efa_rdm_srx.h"
#include "efa_rdm_rma.h"
#include "efa_rdm_msg.h"
#include "efa_rdm_atomic.h"
#include "efa_rdm_rxe_map.h"
#include "efa_rdm_pkt_type.h"
#include "efa_rdm_pke_req.h"
#include "efa_cntr.h"

static
void efa_rdm_ep_construct_ibv_qp_init_attr_ex(struct efa_rdm_ep *ep,
					struct ibv_qp_init_attr_ex *attr_ex,
					struct ibv_cq_ex *tx_cq,
					struct ibv_cq_ex *rx_cq)
{
	attr_ex->cap.max_send_wr = ep->base_ep.domain->device->rdm_info->tx_attr->size;
	attr_ex->cap.max_send_sge = ep->base_ep.domain->device->rdm_info->tx_attr->iov_limit;
	attr_ex->cap.max_recv_wr = ep->base_ep.domain->device->rdm_info->rx_attr->size;
	attr_ex->cap.max_recv_sge = ep->base_ep.domain->device->rdm_info->rx_attr->iov_limit;
	attr_ex->cap.max_inline_data = ep->base_ep.domain->device->efa_attr.inline_buf_size;
	attr_ex->qp_type = IBV_QPT_DRIVER;
	attr_ex->comp_mask = IBV_QP_INIT_ATTR_PD | IBV_QP_INIT_ATTR_SEND_OPS_FLAGS;
	attr_ex->send_ops_flags = IBV_QP_EX_WITH_SEND;
	if (efa_device_support_rdma_read())
		attr_ex->send_ops_flags |= IBV_QP_EX_WITH_RDMA_READ;
	if (efa_device_support_rdma_write()) {
		attr_ex->send_ops_flags |= IBV_QP_EX_WITH_RDMA_WRITE;
		attr_ex->send_ops_flags |= IBV_QP_EX_WITH_RDMA_WRITE_WITH_IMM;
	}
	attr_ex->pd = efa_rdm_ep_domain(ep)->ibv_pd;
	attr_ex->qp_context = ep;
	attr_ex->sq_sig_all = 1;

	attr_ex->send_cq = ibv_cq_ex_to_cq(tx_cq);
	attr_ex->recv_cq = ibv_cq_ex_to_cq(rx_cq);
}

static inline
struct efa_rdm_cq *efa_rdm_ep_get_tx_rdm_cq(struct efa_rdm_ep *ep)
{
	return ep->base_ep.util_ep.tx_cq ? container_of(ep->base_ep.util_ep.tx_cq, struct efa_rdm_cq, util_cq) : NULL;
}

static inline
struct efa_rdm_cq *efa_rdm_ep_get_rx_rdm_cq(struct efa_rdm_ep *ep)
{
	return ep->base_ep.util_ep.rx_cq ? container_of(ep->base_ep.util_ep.rx_cq, struct efa_rdm_cq, util_cq) : NULL;
}

/**
 * @brief set the "efa_qp" field in the efa_rdm_ep->efa_base_ep
 * called by efa_rdm_ep_open()
 *
 * @param[in,out] ep The EFA RDM endpoint to set the qp in
 * @return int 0 on success, negative libfabric error code otherwise
 * @todo merge this function with #efa_base_ep_construct
 */
static
int efa_rdm_ep_create_base_ep_ibv_qp(struct efa_rdm_ep *ep)
{
	struct ibv_qp_init_attr_ex attr_ex = { 0 };
	struct efa_rdm_cq *tx_rdm_cq, *rx_rdm_cq;
	struct ibv_cq_ex *tx_ibv_cq, *rx_ibv_cq;

	tx_rdm_cq = efa_rdm_ep_get_tx_rdm_cq(ep);
	rx_rdm_cq = efa_rdm_ep_get_rx_rdm_cq(ep);

	if (!tx_rdm_cq && !rx_rdm_cq) {
		EFA_WARN(FI_LOG_EP_CTRL,
			"Endpoint is not bound to a send or receive completion queue\n");
		return -FI_ENOCQ;
	}

	if (!tx_rdm_cq && ofi_needs_tx(ep->base_ep.info->caps)) {
		EFA_WARN(FI_LOG_EP_CTRL,
			"Endpoint is not bound to a send completion queue when it has transmit capabilities enabled (FI_SEND).\n");
		return -FI_ENOCQ;
	}

	if (!rx_rdm_cq && ofi_needs_rx(ep->base_ep.info->caps)) {
		EFA_WARN(FI_LOG_EP_CTRL,
			"Endpoint is not bound to a receive completion queue when it has receive capabilities enabled. (FI_RECV)\n");
		return -FI_ENOCQ;
	}

	tx_ibv_cq = tx_rdm_cq ? tx_rdm_cq->ibv_cq.ibv_cq_ex : rx_rdm_cq->ibv_cq.ibv_cq_ex;
	rx_ibv_cq = rx_rdm_cq ? rx_rdm_cq->ibv_cq.ibv_cq_ex : tx_rdm_cq->ibv_cq.ibv_cq_ex;

	efa_rdm_ep_construct_ibv_qp_init_attr_ex(ep, &attr_ex, tx_ibv_cq, rx_ibv_cq);

	return efa_base_ep_create_qp(&ep->base_ep, &attr_ex);
}

static
int efa_rdm_pke_pool_mr_reg_handler(struct ofi_bufpool_region *region)
{
	size_t ret;
	struct fid_mr *mr;
	struct efa_domain *domain = region->pool->attr.context;

	ret = fi_mr_reg(&domain->util_domain.domain_fid, region->alloc_region,
			region->pool->alloc_size, FI_SEND | FI_RECV, 0, 0, 0,
			&mr, NULL);

	region->context = mr;
	return ret;
}

static
void efa_rdm_pke_pool_mr_dereg_handler(struct ofi_bufpool_region *region)
{
	ssize_t ret;

	ret = fi_close((struct fid *)region->context);
	if (ret)
		EFA_WARN(FI_LOG_EP_CTRL,
			"Unable to deregister memory in a buf pool: %s\n",
			fi_strerror(-ret));
}

/**
 * @brief creates a packet entry pool.
 *
 * The pool is allowed to grow if
 * max_cnt is 0 and is fixed size otherwise.
 *
 * @param ep efa_rdm_ep
 * @param pkt_pool_type type of pkt pool
 * @param chunk_cnt count of chunks in the pool
 * @param max_cnt maximal count of chunks
 * @param alignment memory alignment
 * @param pkt_pool pkt pool
 * @return int 0 on success, a negative integer on failure
 */
int efa_rdm_ep_create_pke_pool(struct efa_rdm_ep *ep,
			       bool need_mr,
			       size_t chunk_cnt,
			       size_t max_cnt,
			       size_t alignment,
			       struct ofi_bufpool **pke_pool)
{
	/*
	 * use bufpool flags to make sure that no data structures can share
	 * the memory pages used for this buffer pool if the pool's memory
	 * need to be registered with EFA device.
	 *
	 * Using huge page has a small performance advantage, so we use it
	 * unless it is explicitly prihibitted by user.
	 *
	 * When the bufpools's memory need to be registered, we use
	 * either OFI_BUFPOOL_NONSHARED or OFI_BUFPOOL_HUGEPAGES, both
	 * would ensure that the allocated memory for bufpool does not share
	 * page with other memory regions. This is because memory registration
	 * is page based, e.g. it will always register the whole page.
	 *
	 * This is especially important when rdma-core's fork support is turned on,
	 * which will mark the entire pages of registered memory to be MADV_DONTFORK.
	 * As a result, the child process does not have the page in its memory space.
	 */
	uint64_t mr_flags = (efa_env.huge_page_setting == EFA_ENV_HUGE_PAGE_DISABLED)
					? OFI_BUFPOOL_NONSHARED
					: OFI_BUFPOOL_HUGEPAGES;

	struct ofi_bufpool_attr wiredata_attr = {
		.size = sizeof(struct efa_rdm_pke) + ep->mtu_size,
		.alignment = alignment,
		.max_cnt = max_cnt,
		.chunk_cnt = chunk_cnt,
		.alloc_fn = need_mr ? efa_rdm_pke_pool_mr_reg_handler : NULL,
		.free_fn = need_mr ? efa_rdm_pke_pool_mr_dereg_handler : NULL,
		.init_fn = NULL,
		.context = efa_rdm_ep_domain(ep),
		.flags = need_mr ? mr_flags : 0,
	};

	return ofi_bufpool_create_attr(&wiredata_attr, pke_pool);
}

/** @brief initializes the various buffer pools of EFA RDM endpoint.
 *
 * called by efa_rdm_ep_open()
 *
 * @param ep efa_rdm_ep struct to initialize.
 * @return 0 on success, fi_errno on error.
 * @related #efa_rdm_ep
 */
int efa_rdm_ep_create_buffer_pools(struct efa_rdm_ep *ep)
{
	int ret;

	ret = efa_rdm_ep_create_pke_pool(
		ep,
		true, /* need memory registration */
		efa_rdm_ep_get_tx_pool_size(ep),
		efa_rdm_ep_get_tx_pool_size(ep), /* max count==chunk_cnt means pool is not allowed to grow */
		EFA_RDM_BUFPOOL_ALIGNMENT,
		&ep->efa_tx_pkt_pool);
	if (ret)
		goto err_free;

	ret = efa_rdm_ep_create_pke_pool(
		ep,
		true, /* need memory registration */
		efa_rdm_ep_get_rx_pool_size(ep),
		efa_rdm_ep_get_rx_pool_size(ep), /* max count==chunk_cnt means pool is not allowed to grow */
		EFA_RDM_BUFPOOL_ALIGNMENT,
		&ep->efa_rx_pkt_pool);
	if (ret)
		goto err_free;

	if (efa_env.rx_copy_unexp) {
		ret = efa_rdm_ep_create_pke_pool(
			ep,
			false, /* do not need memory registration */
			efa_env.unexp_pool_chunk_size,
			0, /* max count = 0, so pool is allowed to grow */
			EFA_RDM_BUFPOOL_ALIGNMENT,
			&ep->rx_unexp_pkt_pool);
		if (ret)
			goto err_free;
	}

	if (efa_env.rx_copy_ooo) {
		ret = efa_rdm_ep_create_pke_pool(
			ep,
			false, /* do not need memory registration */
			efa_env.ooo_pool_chunk_size,
			0, /* max count = 0, so pool is allowed to grow */
			EFA_RDM_BUFPOOL_ALIGNMENT,
			&ep->rx_ooo_pkt_pool);
		if (ret)
			goto err_free;
	}

	if ((efa_env.rx_copy_unexp || efa_env.rx_copy_ooo) &&
	    (efa_rdm_ep_domain(ep)->util_domain.mr_mode & FI_MR_HMEM)) {
		/* this pool is only needed when application requested FI_HMEM capability */
		ret = efa_rdm_ep_create_pke_pool(
			ep,
			true, /* need memory registration */
			efa_env.readcopy_pool_size,
			efa_env.readcopy_pool_size, /* max_cnt==chunk_cnt means pool is not allowed to grow */
			EFA_RDM_IN_ORDER_ALIGNMENT, /* support in-order aligned send/recv */
			&ep->rx_readcopy_pkt_pool);
		if (ret)
			goto err_free;

		ep->rx_readcopy_pkt_pool_used = 0;
		ep->rx_readcopy_pkt_pool_max_used = 0;
	}

	ret = ofi_bufpool_create(&ep->map_entry_pool,
				 sizeof(struct efa_rdm_rxe_map_entry),
				 EFA_RDM_BUFPOOL_ALIGNMENT,
				 0, /* no limit for max_cnt */
				 ep->rx_size, 0);

	if (ret)
		goto err_free;

	ret = ofi_bufpool_create(&ep->rx_atomrsp_pool, ep->mtu_size,
				 EFA_RDM_BUFPOOL_ALIGNMENT,
				 0, /* no limit for max_cnt */
				 efa_env.atomrsp_pool_size, 0);
	if (ret)
		goto err_free;

	ret = ofi_bufpool_create(&ep->ope_pool,
				 sizeof(struct efa_rdm_ope),
				 EFA_RDM_BUFPOOL_ALIGNMENT,
				 0, /* no limit for max_cnt */
				 ep->tx_size + ep->rx_size, 0);
	if (ret)
		goto err_free;

	efa_rdm_rxe_map_construct(&ep->rxe_map);
	return 0;

err_free:
	if (ep->rx_atomrsp_pool)
		ofi_bufpool_destroy(ep->rx_atomrsp_pool);

	if (ep->map_entry_pool)
		ofi_bufpool_destroy(ep->map_entry_pool);

	if (ep->ope_pool)
		ofi_bufpool_destroy(ep->ope_pool);

	if (ep->rx_readcopy_pkt_pool)
		ofi_bufpool_destroy(ep->rx_readcopy_pkt_pool);

	if (efa_env.rx_copy_ooo && ep->rx_ooo_pkt_pool)
		ofi_bufpool_destroy(ep->rx_ooo_pkt_pool);

	if (efa_env.rx_copy_unexp && ep->rx_unexp_pkt_pool)
		ofi_bufpool_destroy(ep->rx_unexp_pkt_pool);

	if (ep->efa_rx_pkt_pool)
		ofi_bufpool_destroy(ep->efa_rx_pkt_pool);

	if (ep->efa_tx_pkt_pool)
		ofi_bufpool_destroy(ep->efa_tx_pkt_pool);

	return ret;
}

/**
 * @brief Initialize the various linked lists in an EFA RDM endpoint
 * @param[in,out] ep EFA RDM endpoint
 * @related #efa_rdm_ep
 */
void efa_rdm_ep_init_linked_lists(struct efa_rdm_ep *ep)
{
	dlist_init(&ep->rx_posted_buf_list);
	dlist_init(&ep->ope_queued_rnr_list);
	dlist_init(&ep->ope_queued_ctrl_list);
	dlist_init(&ep->ope_queued_read_list);
	dlist_init(&ep->ope_longcts_send_list);
	dlist_init(&ep->read_pending_list);
	dlist_init(&ep->peer_backoff_list);
	dlist_init(&ep->handshake_queued_peer_list);
#if ENABLE_DEBUG
	dlist_init(&ep->ope_recv_list);
	dlist_init(&ep->rx_pkt_list);
	dlist_init(&ep->tx_pkt_list);
#endif
	dlist_init(&ep->rxe_list);
	dlist_init(&ep->txe_list);
}

/**
 * @brief function pointers to libfabric connection management API
 */
struct fi_ops_cm efa_rdm_ep_cm_ops = {
	.size = sizeof(struct fi_ops_cm),
	.setname = fi_no_setname,
	.getname = efa_base_ep_getname,
	.getpeer = fi_no_getpeer,
	.connect = fi_no_connect,
	.listen = fi_no_listen,
	.accept = fi_no_accept,
	.reject = fi_no_reject,
	.shutdown = fi_no_shutdown,
	.join = fi_no_join,
};

static int efa_rdm_ep_setopt(fid_t fid, int level, int optname,
			     const void *optval, size_t optlen);

static int efa_rdm_ep_getopt(fid_t fid, int level, int optname, void *optval,
			     size_t *optlen);

static ssize_t efa_rdm_ep_cancel(fid_t fid_ep, void *context);

/**
 * @brief function pointers to libfabric endpoint's endpoint API
 * These functions applies to an endpoint
 */
static struct fi_ops_ep efa_rdm_ep_ep_ops = {
	.size = sizeof(struct fi_ops_ep),
	.cancel = efa_rdm_ep_cancel,
	.getopt = efa_rdm_ep_getopt,
	.setopt = efa_rdm_ep_setopt,
	.tx_ctx = fi_no_tx_ctx,
	.rx_ctx = fi_no_rx_ctx,
	.rx_size_left = fi_no_rx_size_left,
	.tx_size_left = fi_no_tx_size_left,
};

static int efa_rdm_ep_close(struct fid *fid);

static int efa_rdm_ep_ctrl(struct fid *fid, int command, void *arg);

static int efa_rdm_ep_bind(struct fid *ep_fid, struct fid *bfid, uint64_t flags);

/**
 * @brief function pointers to libfabric endpoint's fabric interface API
 * These functions applies to a libfabric object
 */
static struct fi_ops efa_rdm_ep_base_ops = {
	.size = sizeof(struct fi_ops),
	.close = efa_rdm_ep_close,
	.bind = efa_rdm_ep_bind,
	.control = efa_rdm_ep_ctrl,
	.ops_open = fi_no_ops_open,
};

/**
 * @brief set the "use_zcpy_rx" flag in an EFA RDM endpoint.
 * called by efa_rdm_ep_open()
 *
 * @param[in,out] ep EFA RDM endpoint
 */
static inline
void efa_rdm_ep_set_use_zcpy_rx(struct efa_rdm_ep *ep)
{
	ep->use_zcpy_rx = !(ep->base_ep.util_ep.caps & FI_DIRECTED_RECV) &&
			  !(ep->base_ep.util_ep.caps & FI_TAGGED) &&
			  !(ep->base_ep.util_ep.caps & FI_ATOMIC) &&
			  (ep->max_msg_size <= ep->mtu_size - ep->max_proto_hdr_size) &&
			  !efa_rdm_ep_need_sas(ep) &&
			  ep->user_info->mode & FI_MSG_PREFIX &&
			  efa_env.use_zcpy_rx;

	EFA_INFO(FI_LOG_EP_CTRL, "efa_rdm_ep->use_zcpy_rx = %d\n",
		 ep->use_zcpy_rx);
}

/**
 * @brief implement the fi_endpoint() API for EFA RDM endpoint
 *
 * @param[in,out] domain The domain this endpoint belongs to
 * @param[in] info The info struct used to create this endpoint
 * @param[out] ep The endpoint to be created
 * @param[in] context The context associated with this endpoint
 * @return int 0 on success, negative libfabric error code otherwise
 */
int efa_rdm_ep_open(struct fid_domain *domain, struct fi_info *info,
		    struct fid_ep **ep, void *context)
{
	struct efa_domain *efa_domain = NULL;
	struct efa_rdm_ep *efa_rdm_ep = NULL;
	struct fi_cq_attr cq_attr;
	int ret, retv, i;

	efa_rdm_ep = calloc(1, sizeof(*efa_rdm_ep));
	if (!efa_rdm_ep)
		return -FI_ENOMEM;

	efa_domain = container_of(domain, struct efa_domain,
				  util_domain.domain_fid);
	memset(&cq_attr, 0, sizeof(cq_attr));
	cq_attr.format = FI_CQ_FORMAT_DATA;
	cq_attr.wait_obj = FI_WAIT_NONE;

	ret = efa_base_ep_construct(&efa_rdm_ep->base_ep, domain, info,
				    efa_rdm_ep_progress, context);
	if (ret)
		goto err_free_ep;

	if (efa_domain->shm_domain) {
		ret = fi_endpoint(efa_domain->shm_domain, efa_domain->shm_info,
				  &efa_rdm_ep->shm_ep, efa_rdm_ep);
		if (ret)
			goto err_destroy_base_ep;
	} else {
		efa_rdm_ep->shm_ep = NULL;
	}

	efa_rdm_ep->user_info = fi_dupinfo(info);
	if (!efa_rdm_ep->user_info) {
		ret = -FI_ENOMEM;
		goto err_free_ep;
	}

	efa_rdm_ep->host_id = efa_get_host_id(efa_env.host_id_file);
	if (efa_rdm_ep->host_id) {
		EFA_INFO(FI_LOG_EP_CTRL, "efa_rdm_ep->host_id: i-%017lx\n", efa_rdm_ep->host_id);
	}

	efa_rdm_ep->rx_size = info->rx_attr->size;
	efa_rdm_ep->tx_size = info->tx_attr->size;
	efa_rdm_ep->rx_iov_limit = info->rx_attr->iov_limit;
	efa_rdm_ep->tx_iov_limit = info->tx_attr->iov_limit;
	efa_rdm_ep->inject_size = info->tx_attr->inject_size;
	efa_rdm_ep->efa_max_outstanding_tx_ops = efa_domain->device->rdm_info->tx_attr->size;
	efa_rdm_ep->efa_max_outstanding_rx_ops = efa_domain->device->rdm_info->rx_attr->size;
	efa_rdm_ep->efa_device_iov_limit = efa_domain->device->rdm_info->tx_attr->iov_limit;
	efa_rdm_ep->use_device_rdma = efa_rdm_get_use_device_rdma(info->fabric_attr->api_version);
	efa_rdm_ep->shm_permitted = true;

	cq_attr.size = MAX(efa_rdm_ep->rx_size + efa_rdm_ep->tx_size,
			   efa_env.cq_size);

	assert(info->tx_attr->msg_order == info->rx_attr->msg_order);
	efa_rdm_ep->msg_order = info->rx_attr->msg_order;
	efa_rdm_ep->max_msg_size = info->ep_attr->max_msg_size;
	efa_rdm_ep->msg_prefix_size = info->ep_attr->msg_prefix_size;
	efa_rdm_ep->max_proto_hdr_size = efa_rdm_pkt_type_get_max_hdr_size();
	efa_rdm_ep->mtu_size = efa_domain->device->rdm_info->ep_attr->max_msg_size;

	efa_rdm_ep->max_data_payload_size = efa_rdm_ep->mtu_size - sizeof(struct efa_rdm_ctsdata_hdr) - sizeof(struct efa_rdm_ctsdata_opt_connid_hdr);
	efa_rdm_ep->min_multi_recv_size = efa_rdm_ep->mtu_size - efa_rdm_ep->max_proto_hdr_size;

	if (efa_env.tx_queue_size > 0 &&
	    efa_env.tx_queue_size < efa_rdm_ep->efa_max_outstanding_tx_ops)
		efa_rdm_ep->efa_max_outstanding_tx_ops = efa_env.tx_queue_size;

	efa_rdm_ep_set_use_zcpy_rx(efa_rdm_ep);

	efa_rdm_ep->handle_resource_management = info->domain_attr->resource_mgmt;
	EFA_INFO(FI_LOG_EP_CTRL,
		"efa_rdm_ep->handle_resource_management = %d\n",
		efa_rdm_ep->handle_resource_management);

#if ENABLE_DEBUG
	efa_rdm_ep->efa_total_posted_tx_ops = 0;
	efa_rdm_ep->send_comps = 0;
	efa_rdm_ep->recv_comps = 0;
#endif

	efa_rdm_ep->efa_rx_pkts_posted = 0;
	efa_rdm_ep->efa_rx_pkts_to_post = 0;
	efa_rdm_ep->efa_rx_pkts_held = 0;
	efa_rdm_ep->efa_outstanding_tx_ops = 0;

	if (ret) {
		EFA_WARN(FI_LOG_CQ, "Unable to create extended CQ: %s\n", strerror(errno));
		goto err_close_shm_ep;
	}

	ret = efa_rdm_ep_create_buffer_pools(efa_rdm_ep);
	if (ret)
		goto err_close_shm_ep;

	efa_rdm_ep_init_linked_lists(efa_rdm_ep);

	/* Set hmem_p2p_opt */
	efa_rdm_ep->hmem_p2p_opt = FI_HMEM_P2P_DISABLED;

	/*
	 * TODO this assumes only one non-stantard interface is initialized at a
	 * time. Refactor to handle multiple initialized interfaces to impose
	 * tighter requirements for the default p2p opt
	 */
	EFA_HMEM_IFACE_FOREACH_NON_SYSTEM(i) {
		if (efa_rdm_ep->base_ep.domain->hmem_info[efa_hmem_ifaces[i]].initialized &&
			efa_rdm_ep->base_ep.domain->hmem_info[efa_hmem_ifaces[i]].p2p_supported_by_device) {
			efa_rdm_ep->hmem_p2p_opt = efa_rdm_ep->base_ep.domain->hmem_info[efa_hmem_ifaces[i]].p2p_required_by_impl
				? FI_HMEM_P2P_REQUIRED
				: FI_HMEM_P2P_PREFERRED;
			break;
		}
	}

	efa_rdm_ep->cuda_api_permitted = (FI_VERSION_GE(info->fabric_attr->api_version, FI_VERSION(1, 18)));
	efa_rdm_ep->sendrecv_in_order_aligned_128_bytes = false;
	efa_rdm_ep->write_in_order_aligned_128_bytes = false;

	efa_rdm_ep->pke_vec = calloc(sizeof(struct efa_rdm_pke *), EFA_RDM_EP_MAX_WR_PER_IBV_POST_RECV);
	if (!efa_rdm_ep->pke_vec) {
		EFA_WARN(FI_LOG_EP_CTRL, "cannot alloc memory for efa_rdm_ep->pke_vec!\n");
		ret = -FI_ENOMEM;
		goto err_close_shm_ep;
	}

	*ep = &efa_rdm_ep->base_ep.util_ep.ep_fid;
	(*ep)->msg = &efa_rdm_msg_ops;
	(*ep)->rma = &efa_rdm_rma_ops;
	(*ep)->atomic = &efa_rdm_atomic_ops;
	(*ep)->tagged = &efa_rdm_msg_tagged_ops;
	(*ep)->fid.ops = &efa_rdm_ep_base_ops;
	(*ep)->ops = &efa_rdm_ep_ep_ops;
	(*ep)->cm = &efa_rdm_ep_cm_ops;
	return 0;

err_close_shm_ep:
	if (efa_rdm_ep->shm_ep) {
		retv = fi_close(&efa_rdm_ep->shm_ep->fid);
		if (retv)
			EFA_WARN(FI_LOG_EP_CTRL, "Unable to close shm EP: %s\n",
				fi_strerror(-retv));
	}
err_destroy_base_ep:
	efa_base_ep_destruct(&efa_rdm_ep->base_ep);
err_free_ep:
	if (efa_rdm_ep)
		free(efa_rdm_ep);
	return ret;
}

/**
 * @brief implement the fi_ep_bind API for EFA RDM endpoint
 * Currently supported objects are: AV, CQ, CNTR, EQ
 * @param ep_fid - endpoint fid
 * @param bfid - fid of the object to be binded with the endpoint
 * @param flags - bind flags
 * @return 0 on success, negative libfabric error code otherwise
 */
static int efa_rdm_ep_bind(struct fid *ep_fid, struct fid *bfid, uint64_t flags)
{
	struct efa_rdm_ep *efa_rdm_ep =
		container_of(ep_fid, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	struct efa_rdm_cq *cq;
	struct efa_av *av;
	struct efa_cntr *cntr;
	struct util_eq *eq;
	int ret = 0;

	switch (bfid->fclass) {
	case FI_CLASS_AV:
		av = container_of(bfid, struct efa_av, util_av.av_fid.fid);
		/* Bind util provider endpoint and av */
		ret = ofi_ep_bind_av(&efa_rdm_ep->base_ep.util_ep, &av->util_av);
		if (ret)
			return ret;

		ret = efa_base_ep_bind_av(&efa_rdm_ep->base_ep, av);
		if (ret)
			return ret;

		/* Bind shm provider endpoint & shm av */
		if (efa_rdm_ep->shm_ep) {
			assert(av->shm_rdm_av);
			ret = fi_ep_bind(efa_rdm_ep->shm_ep, &av->shm_rdm_av->fid, flags);
			if (ret)
				return ret;
		}
		break;
	case FI_CLASS_CQ:
		cq = container_of(bfid, struct efa_rdm_cq, util_cq.cq_fid.fid);

		ret = ofi_ep_bind_cq(&efa_rdm_ep->base_ep.util_ep, &cq->util_cq, flags);
		if (ret)
			return ret;

		if (cq->shm_cq) {
			/* Bind ep with shm provider's cq */
			ret = fi_ep_bind(efa_rdm_ep->shm_ep, &cq->shm_cq->fid, flags);
			if (ret)
				return ret;
		}
		break;
	case FI_CLASS_CNTR:
		cntr = container_of(bfid, struct efa_cntr, util_cntr.cntr_fid.fid);

		ret = ofi_ep_bind_cntr(&efa_rdm_ep->base_ep.util_ep, &cntr->util_cntr, flags);
		if (ret)
			return ret;

		if (cntr->shm_cntr) {
			/* Bind shm ep with shm provider's cntr */
			ret = fi_ep_bind(efa_rdm_ep->shm_ep, &cntr->shm_cntr->fid, flags);
			if (ret)
				return ret;
		}
		break;
	case FI_CLASS_EQ:
		eq = container_of(bfid, struct util_eq, eq_fid.fid);

		ret = ofi_ep_bind_eq(&efa_rdm_ep->base_ep.util_ep, eq);
		if (ret)
			return ret;
		break;
	default:
		EFA_WARN(FI_LOG_EP_CTRL, "invalid fid class\n");
		ret = -FI_EINVAL;
		break;
	}
	return ret;
}

/**
 * @brief destroy the various buffer pools of an EFA RDM endpoint
 * @param[in,out] efa_rdm_ep  EFA RDM endpoint
 * @related #efa_rdm_ep
 */
static void efa_rdm_ep_destroy_buffer_pools(struct efa_rdm_ep *efa_rdm_ep)
{
	struct dlist_entry *entry, *tmp;
	struct efa_rdm_ope *rxe;
	struct efa_rdm_ope *txe;
	struct efa_rdm_ope *ope;
#if ENABLE_DEBUG
	struct efa_rdm_pke *pkt_entry;
#endif

	dlist_foreach_safe(&efa_rdm_ep->ope_queued_rnr_list, entry, tmp) {
		txe = container_of(entry, struct efa_rdm_ope,
					queued_rnr_entry);
		EFA_WARN(FI_LOG_EP_CTRL,
			"Closing ep with queued rnr txe: %p\n",
			txe);
		efa_rdm_txe_release(txe);
	}

	dlist_foreach_safe(&efa_rdm_ep->ope_queued_ctrl_list, entry, tmp) {
		ope = container_of(entry, struct efa_rdm_ope,
					queued_ctrl_entry);
		EFA_WARN(FI_LOG_EP_CTRL,
			"Closing ep with queued ctrl ope: %p\n",
			ope);
		if (ope->type == EFA_RDM_TXE) {
			efa_rdm_txe_release(ope);
		} else {
			assert(ope->type == EFA_RDM_RXE);
			efa_rdm_rxe_release(ope);
		}
	}

#if ENABLE_DEBUG
	dlist_foreach_safe(&efa_rdm_ep->rx_posted_buf_list, entry, tmp) {
		pkt_entry = container_of(entry, struct efa_rdm_pke, dbg_entry);
		efa_rdm_pke_release_rx(pkt_entry);
	}

	dlist_foreach_safe(&efa_rdm_ep->rx_pkt_list, entry, tmp) {
		pkt_entry = container_of(entry, struct efa_rdm_pke, dbg_entry);
		EFA_WARN(FI_LOG_EP_CTRL,
			"Closing ep with unreleased RX pkt_entry: %p\n",
			pkt_entry);
		efa_rdm_pke_release_rx(pkt_entry);
	}

	dlist_foreach_safe(&efa_rdm_ep->tx_pkt_list, entry, tmp) {
		pkt_entry = container_of(entry, struct efa_rdm_pke, dbg_entry);
		EFA_WARN(FI_LOG_EP_CTRL,
			"Closing ep with unreleased TX pkt_entry: %p\n",
			pkt_entry);
		efa_rdm_pke_release_tx(pkt_entry);
	}
#endif

	dlist_foreach_safe(&efa_rdm_ep->rxe_list, entry, tmp) {
		rxe = container_of(entry, struct efa_rdm_ope,
					ep_entry);
		EFA_WARN(FI_LOG_EP_CTRL,
			"Closing ep with unreleased rxe\n");
		efa_rdm_rxe_release(rxe);
	}

	dlist_foreach_safe(&efa_rdm_ep->txe_list, entry, tmp) {
		txe = container_of(entry, struct efa_rdm_ope,
					ep_entry);
		EFA_WARN(FI_LOG_EP_CTRL,
			"Closing ep with unreleased txe: %p\n",
			txe);
		efa_rdm_txe_release(txe);
	}

	if (efa_rdm_ep->ope_pool)
		ofi_bufpool_destroy(efa_rdm_ep->ope_pool);

	if (efa_rdm_ep->map_entry_pool)
		ofi_bufpool_destroy(efa_rdm_ep->map_entry_pool);

	if (efa_rdm_ep->rx_readcopy_pkt_pool) {
		EFA_INFO(FI_LOG_EP_CTRL, "current usage of read copy packet pool is %d\n",
			efa_rdm_ep->rx_readcopy_pkt_pool_used);
		EFA_INFO(FI_LOG_EP_CTRL, "maximum usage of read copy packet pool is %d\n",
			efa_rdm_ep->rx_readcopy_pkt_pool_max_used);
		assert(!efa_rdm_ep->rx_readcopy_pkt_pool_used);
		ofi_bufpool_destroy(efa_rdm_ep->rx_readcopy_pkt_pool);
	}

	if (efa_rdm_ep->rx_ooo_pkt_pool)
		ofi_bufpool_destroy(efa_rdm_ep->rx_ooo_pkt_pool);

	if (efa_rdm_ep->rx_unexp_pkt_pool)
		ofi_bufpool_destroy(efa_rdm_ep->rx_unexp_pkt_pool);

	if (efa_rdm_ep->efa_rx_pkt_pool)
		ofi_bufpool_destroy(efa_rdm_ep->efa_rx_pkt_pool);

	if (efa_rdm_ep->efa_tx_pkt_pool)
		ofi_bufpool_destroy(efa_rdm_ep->efa_tx_pkt_pool);
}

/*
 * @brief determine whether an endpoint has unfinished send
 *
 * Unfinished send includes queued ctrl packets, queued
 * RNR packets and inflight TX packets.
 *
 * @param[in]	efa_rdm_ep	endpoint
 * @return	a boolean
 */
static
bool efa_rdm_ep_has_unfinished_send(struct efa_rdm_ep *efa_rdm_ep)
{
	return !dlist_empty(&efa_rdm_ep->ope_queued_rnr_list) ||
	       !dlist_empty(&efa_rdm_ep->ope_queued_ctrl_list) ||
	       (efa_rdm_ep->efa_outstanding_tx_ops > 0);
}

/*
 * @brief wait for send to finish
 *
 * Wait for queued packet to be sent, and inflight send to
 * complete.
 *
 * @param[in]	efa_rdm_ep		endpoint
 * @return 	no return
 */
static inline
void efa_rdm_ep_wait_send(struct efa_rdm_ep *efa_rdm_ep)
{
	struct util_srx_ctx *srx_ctx;
	struct efa_rdm_cq *tx_cq, *rx_cq;
	/* peer srx should be initialized when ep is enabled */
	assert(efa_rdm_ep->peer_srx_ep);
	srx_ctx = efa_rdm_ep_get_peer_srx_ctx(efa_rdm_ep);
	ofi_genlock_lock(srx_ctx->lock);

	tx_cq = efa_rdm_ep_get_tx_rdm_cq(efa_rdm_ep);
	rx_cq = efa_rdm_ep_get_rx_rdm_cq(efa_rdm_ep);

	while (efa_rdm_ep_has_unfinished_send(efa_rdm_ep)) {
		/* poll cq until empty */
		if (tx_cq)
			efa_rdm_cq_poll_ibv_cq(-1, &tx_cq->ibv_cq);
		if (rx_cq)
			efa_rdm_cq_poll_ibv_cq(-1, &rx_cq->ibv_cq);
		efa_rdm_ep_progress_internal(efa_rdm_ep);
	}

	ofi_genlock_unlock(srx_ctx->lock);
}

static inline
void efa_rdm_ep_remove_cntr_ibv_cq_poll_list(struct efa_rdm_ep *ep)
{
	int i;
	struct efa_cntr *efa_cntr;
	struct util_cntr *util_cntr;
	struct efa_rdm_cq *tx_cq, *rx_cq;

	tx_cq = efa_rdm_ep_get_tx_rdm_cq(ep);
	rx_cq = efa_rdm_ep_get_rx_rdm_cq(ep);

	for (i = 0; i< CNTR_CNT; i++) {
		util_cntr = ep->base_ep.util_ep.cntrs[i];
		if (util_cntr) {
			efa_cntr = container_of(util_cntr, struct efa_cntr, util_cntr);
			if (tx_cq && !ofi_atomic_get32(&tx_cq->util_cq.ref))
				efa_ibv_cq_poll_list_remove(&efa_cntr->ibv_cq_poll_list, &efa_cntr->util_cntr.ep_list_lock, &tx_cq->ibv_cq);

			if (rx_cq && !ofi_atomic_get32(&rx_cq->util_cq.ref))
				efa_ibv_cq_poll_list_remove(&efa_cntr->ibv_cq_poll_list, &efa_cntr->util_cntr.ep_list_lock, &rx_cq->ibv_cq);
		}
	}
}

static inline
void efa_rdm_ep_remove_cq_ibv_cq_poll_list(struct efa_rdm_ep *ep)
{
	struct efa_rdm_cq *tx_cq, *rx_cq;

	tx_cq = efa_rdm_ep_get_tx_rdm_cq(ep);
	rx_cq = efa_rdm_ep_get_rx_rdm_cq(ep);

	/* Remove the cross referencing of the CQs.
	 * It must happen after ofi_endpoint_close
	 * so we have cq's reference counters updated.
	 */
	if (tx_cq && !ofi_atomic_get32(&tx_cq->util_cq.ref)) {
		efa_ibv_cq_poll_list_remove(&tx_cq->ibv_cq_poll_list, &tx_cq->util_cq.ep_list_lock, &tx_cq->ibv_cq);
		if (rx_cq)
			efa_ibv_cq_poll_list_remove(&rx_cq->ibv_cq_poll_list, &rx_cq->util_cq.ep_list_lock, &tx_cq->ibv_cq);
	}

	if (rx_cq && !ofi_atomic_get32(&rx_cq->util_cq.ref)) {
		efa_ibv_cq_poll_list_remove(&rx_cq->ibv_cq_poll_list, &rx_cq->util_cq.ep_list_lock, &rx_cq->ibv_cq);
		if (tx_cq)
			efa_ibv_cq_poll_list_remove(&tx_cq->ibv_cq_poll_list, &tx_cq->util_cq.ep_list_lock, &rx_cq->ibv_cq);
	}
}

/**
 * @brief implement the fi_close() API for the EFA RDM endpoint
 * @param[in,out]	fid		Endpoint to close
 */
static int efa_rdm_ep_close(struct fid *fid)
{
	int ret, retv = 0;
	struct efa_rdm_ep *efa_rdm_ep;

	efa_rdm_ep = container_of(fid, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);

	if (efa_rdm_ep->base_ep.efa_qp_enabled)
		efa_rdm_ep_wait_send(efa_rdm_ep);

	/*
	 * util_srx_close will clean all efa_rdm_rxes that are
	 * associated with peer_rx_entries in unexp msg/tag lists.
	 * It also decrements the ref count of rx cq. So it must
	 * be called before we clean up the ibv cq poll list which
	 * relies on the correct ref count of tx/rx cq.
	 */
	if (efa_rdm_ep->peer_srx_ep) {
		util_srx_close(&efa_rdm_ep->peer_srx_ep->fid);
		efa_rdm_ep->peer_srx_ep = NULL;
	}

	/* We need to free the util_ep first to avoid race conditions
	 * with other threads progressing the cq. */
	efa_base_ep_close_util_ep(&efa_rdm_ep->base_ep);

	efa_rdm_ep_remove_cntr_ibv_cq_poll_list(efa_rdm_ep);

	efa_rdm_ep_remove_cq_ibv_cq_poll_list(efa_rdm_ep);

	ret = efa_base_ep_destruct(&efa_rdm_ep->base_ep);
	if (ret) {
		EFA_WARN(FI_LOG_EP_CTRL, "Unable to close base endpoint\n");
		retv = ret;
	}

	if (efa_rdm_ep->shm_ep) {
		ret = fi_close(&efa_rdm_ep->shm_ep->fid);
		if (ret) {
			EFA_WARN(FI_LOG_EP_CTRL, "Unable to close shm EP\n");
			retv = ret;
		}
	}

	efa_rdm_ep_destroy_buffer_pools(efa_rdm_ep);

	if (efa_rdm_ep->pke_vec)
		free(efa_rdm_ep->pke_vec);
	free(efa_rdm_ep);
	return retv;
}

/**
 * @brief set the "extra_info" field of an EFA RDM endpoint
 * "extra_info" is a bitfield that indicates which extra features
 * are supported. Extra features are defined in the EFA protocol.
 * This functions is called by efa_rdm_ep_ctrl() when user call
 * fi_enable().
 * @related #efa_rdm_ep
 * @param[in,out]	ep	Endpoint to set
*/
static
void efa_rdm_ep_set_extra_info(struct efa_rdm_ep *ep)
{
	memset(ep->extra_info, 0, sizeof(ep->extra_info));

	/* RDMA read is an extra feature defined in protocol version 4 (the base version) */
	if (efa_rdm_ep_support_rdma_read(ep))
		ep->extra_info[0] |= EFA_RDM_EXTRA_FEATURE_RDMA_READ;

	/* RDMA write is defined in protocol v4, and introduced in libfabric 1.18.0 */
	if (efa_rdm_ep_support_rdma_write(ep))
		ep->extra_info[0] |= EFA_RDM_EXTRA_FEATURE_RDMA_WRITE;

	ep->extra_info[0] |= EFA_RDM_EXTRA_FEATURE_DELIVERY_COMPLETE;

	if (ep->use_zcpy_rx) {
		/*
		 * zero copy receive requires the packet header length remains
		 * constant, so the application receive buffer is match with
		 * incoming application data.
		 */
		ep->extra_info[0] |= EFA_RDM_EXTRA_REQUEST_CONSTANT_HEADER_LENGTH;
	}

	ep->extra_info[0] |= EFA_RDM_EXTRA_REQUEST_CONNID_HEADER;

	ep->extra_info[0] |= EFA_RDM_EXTRA_FEATURE_RUNT;

	/* READ_NACK feature introduced in libfabric 1.20 */
	ep->extra_info[0] |= EFA_RDM_EXTRA_FEATURE_READ_NACK;
}

/**
 * @brief Close all shm resources bound to the efa ep and domain.
 * This function will do this cleanup as best effort. When there is failure
 * to clean up shm resource, it will still move forward by setting the resource
 * pointer to NULL so it won't be used later.
 *
 * @param efa_rdm_ep pointer to efa_rdm_ep.
 */
static void efa_rdm_ep_close_shm_resources(struct efa_rdm_ep *efa_rdm_ep)
{
	int ret;
	struct efa_domain *efa_domain;
	struct efa_av *efa_av;
	struct efa_rdm_cq *efa_rdm_cq;

	if (efa_rdm_ep->shm_ep) {
		ret = fi_close(&efa_rdm_ep->shm_ep->fid);
		if (ret)
			EFA_WARN(FI_LOG_EP_CTRL, "Unable to close shm ep\n");
		efa_rdm_ep->shm_ep = NULL;
	}

	efa_av = efa_rdm_ep->base_ep.av;
	if (efa_av->shm_rdm_av) {
		ret = fi_close(&efa_av->shm_rdm_av->fid);
		if (ret)
			EFA_WARN(FI_LOG_EP_CTRL, "Unable to close shm av\n");
		efa_av->shm_rdm_av = NULL;
	}

	efa_rdm_cq = container_of(efa_rdm_ep->base_ep.util_ep.tx_cq, struct efa_rdm_cq, util_cq);
	if (efa_rdm_cq->shm_cq) {
		ret = fi_close(&efa_rdm_cq->shm_cq->fid);
		if (ret)
			EFA_WARN(FI_LOG_EP_CTRL, "Unable to close shm cq\n");
		efa_rdm_cq->shm_cq = NULL;
	}

	efa_rdm_cq = container_of(efa_rdm_ep->base_ep.util_ep.rx_cq, struct efa_rdm_cq, util_cq);
	if (efa_rdm_cq->shm_cq) {
		ret = fi_close(&efa_rdm_cq->shm_cq->fid);
		if (ret)
			EFA_WARN(FI_LOG_EP_CTRL, "Unable to close shm cq\n");
		efa_rdm_cq->shm_cq = NULL;
	}

	efa_domain = efa_rdm_ep_domain(efa_rdm_ep);

	if (efa_domain->shm_domain) {
		ret = fi_close(&efa_domain->shm_domain->fid);
		if (ret)
			EFA_WARN(FI_LOG_EP_CTRL, "Unable to close shm domain\n");
		efa_domain->shm_domain = NULL;
	}

	if (efa_domain->fabric->shm_fabric) {
		ret = fi_close(&efa_domain->fabric->shm_fabric->fid);
		if (ret)
			EFA_WARN(FI_LOG_EP_CTRL, "Unable to close shm fabric\n");
		efa_domain->fabric->shm_fabric = NULL;
	}

	if (efa_domain->shm_info) {
		fi_freeinfo(efa_domain->shm_info);
		efa_domain->shm_info = NULL;
	}
}

/**
 * @brief update the shm resources based on the
 * the current ep status. When cuda_api_permitted
 * is set as false via fi_setopt, shm should be
 * shut down. This function must be called inside
 * fi_enable which is called after fi_setopt.
 *
 * @param[in,out]	ep	efa_rdm_ep
 */
static
void efa_rdm_ep_update_shm(struct efa_rdm_ep *ep)
{
	bool use_shm;

	/*
	 * when efa_env.enable_shm_transfer is false
	 * , shm resources won't be created.
	 */
	if (!efa_rdm_ep_domain(ep)->shm_domain)
		return;

	use_shm = true;

	assert(ep->user_info);

	/*
	 * shm provider must make cuda calls to transfer cuda memory.
	 * if cuda call is not allowed, we cannot use shm for transfer.
	 *
	 * Note that the other two hmem interfaces supported by EFA,
	 * AWS Neuron and Habana Synapse, have no SHM provider
	 * support anyways, so disabling SHM will not impact them.
	 */
	if (((ep->user_info->caps & FI_HMEM)
	    && hmem_ops[FI_HMEM_CUDA].initialized
	    && !ep->cuda_api_permitted)
		|| !ep->shm_permitted) {
		use_shm = false;
	}

	if (!use_shm)
		efa_rdm_ep_close_shm_resources(ep);
}

static inline
int efa_rdm_ep_insert_cntr_ibv_cq_poll_list(struct efa_rdm_ep *ep)
{
	int i, ret;
	struct efa_cntr *efa_cntr;
	struct util_cntr *util_cntr;
	struct efa_rdm_cq *tx_cq, *rx_cq;
	tx_cq = efa_rdm_ep_get_tx_rdm_cq(ep);
	rx_cq = efa_rdm_ep_get_rx_rdm_cq(ep);

	for (i = 0; i < CNTR_CNT; i++) {
		util_cntr = ep->base_ep.util_ep.cntrs[i];
		if (util_cntr) {
			efa_cntr = container_of(util_cntr, struct efa_cntr, util_cntr);
			if (tx_cq) {
				ret = efa_ibv_cq_poll_list_insert(&efa_cntr->ibv_cq_poll_list, &efa_cntr->util_cntr.ep_list_lock, &tx_cq->ibv_cq);
				if (ret)
					return ret;
			}
			if (rx_cq) {
				ret = efa_ibv_cq_poll_list_insert(&efa_cntr->ibv_cq_poll_list, &efa_cntr->util_cntr.ep_list_lock, &rx_cq->ibv_cq);
				if (ret)
					return ret;
			}
		}
	}

	return FI_SUCCESS;
}

static inline
int efa_rdm_ep_insert_cq_ibv_cq_poll_list(struct efa_rdm_ep *ep)
{
	int ret;
	struct efa_rdm_cq *tx_cq, *rx_cq;
	/* cross referencing */
	tx_cq = efa_rdm_ep_get_tx_rdm_cq(ep);
	rx_cq = efa_rdm_ep_get_rx_rdm_cq(ep);

	if (tx_cq) {
		ret = efa_ibv_cq_poll_list_insert(&tx_cq->ibv_cq_poll_list, &tx_cq->util_cq.ep_list_lock, &tx_cq->ibv_cq);
		if (ret)
			return ret;

		if (rx_cq) {
			ret = efa_ibv_cq_poll_list_insert(&tx_cq->ibv_cq_poll_list, &tx_cq->util_cq.ep_list_lock, &rx_cq->ibv_cq);
			if (ret)
				return ret;
		}
	}

	if (rx_cq) {
		ret = efa_ibv_cq_poll_list_insert(&rx_cq->ibv_cq_poll_list, &rx_cq->util_cq.ep_list_lock, &rx_cq->ibv_cq);
		if (ret)
			return ret;

		if (tx_cq) {
			ret = efa_ibv_cq_poll_list_insert(&rx_cq->ibv_cq_poll_list, &rx_cq->util_cq.ep_list_lock, &tx_cq->ibv_cq);
			if (ret)
				return ret;
		}
	}

	return FI_SUCCESS;
}

/**
 * @brief implement the fi_enable() API for EFA RDM endpoint
 * @param[in,out]	fid	Endpoint to enable
 * @param[in]		flags	Flags
 * @return 0 on success, negative libfabric error code otherwise
 */
static int efa_rdm_ep_ctrl(struct fid *fid, int command, void *arg)
{
	struct efa_rdm_ep *ep;
	char shm_ep_name[EFA_SHM_NAME_MAX], ep_addr_str[OFI_ADDRSTRLEN];
	size_t shm_ep_name_len, ep_addr_strlen;
	int ret = 0;
	struct fi_peer_srx_context peer_srx_context = {0};
	struct fi_rx_attr peer_srx_attr = {0};
	struct fid_ep *peer_srx_ep = NULL;
	struct util_srx_ctx *srx_ctx;

	switch (command) {
	case FI_ENABLE:
		ep = container_of(fid, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);

		/*
		 * efa uses util SRX no matter shm is enabled, so we need to initialize
		 * it anyway.
		 */
		ret = efa_rdm_peer_srx_construct(ep);
		if (ret)
			return ret;

		ret = efa_rdm_ep_create_base_ep_ibv_qp(ep);
		if (ret)
			return ret;

		/* efa_base_ep_enable destroys qp in the error path */
		ret = efa_base_ep_enable(&ep->base_ep);
		if (ret)
			return ret;

		ret = efa_rdm_ep_insert_cq_ibv_cq_poll_list(ep);
		if (ret)
			return ret;

		ret = efa_rdm_ep_insert_cntr_ibv_cq_poll_list(ep);
		if (ret)
			return ret;

		assert(ep->peer_srx_ep);
		srx_ctx = efa_rdm_ep_get_peer_srx_ctx(ep);
		ofi_genlock_lock(srx_ctx->lock);

		efa_rdm_ep_set_extra_info(ep);

		ep_addr_strlen = sizeof(ep_addr_str);
		efa_rdm_ep_raw_addr_str(ep, ep_addr_str, &ep_addr_strlen);
		EFA_INFO(FI_LOG_EP_CTRL, "libfabric %s efa endpoint created! address: %s\n",
			fi_tostr("1", FI_TYPE_VERSION), ep_addr_str);

		efa_rdm_ep_update_shm(ep);

		/* Enable shm provider endpoint & post recv buff.
		 * Once core ep enabled, 18 bytes efa_addr (16 bytes raw + 2 bytes qpn) is set.
		 * We convert the address to 'gid_qpn' format, and set it as shm ep name, so
		 * that shm ep can create shared memory region with it when enabling.
		 * In this way, each peer is able to open and map to other local peers'
		 * shared memory region.
		 */
		if (ep->shm_ep) {
                        peer_srx_context.srx = util_get_peer_srx(ep->peer_srx_ep);
                        peer_srx_attr.op_flags |= FI_PEER;
                        ret = fi_srx_context(efa_rdm_ep_domain(ep)->shm_domain,
					     &peer_srx_attr, &peer_srx_ep, &peer_srx_context);
                        if (ret)
                                goto err_unlock_and_destroy_qp;
			shm_ep_name_len = EFA_SHM_NAME_MAX;
			ret = efa_shm_ep_name_construct(shm_ep_name, &shm_ep_name_len, &ep->base_ep.src_addr);
			if (ret < 0)
				goto err_unlock_and_destroy_qp;
			fi_setname(&ep->shm_ep->fid, shm_ep_name, shm_ep_name_len);

			/* Bind srx to shm ep */
			ret = fi_ep_bind(ep->shm_ep, &ep->peer_srx_ep->fid, 0);
			if (ret)
				goto err_unlock_and_destroy_qp;

			ret = fi_enable(ep->shm_ep);
			if (ret)
				goto err_unlock_and_destroy_qp;
		}
		ofi_genlock_unlock(srx_ctx->lock);
		break;
	default:
		ret = -FI_ENOSYS;
		break;
	}

	return ret;

err_unlock_and_destroy_qp:
	ofi_genlock_unlock(srx_ctx->lock);
	efa_base_ep_destruct_qp(&ep->base_ep);
	return ret;
}

/**
 * @brief implement the fi_cancel API
 * @param[in]	fid_ep	EFA RDM endpoint to perform the cancel operation
 * @param[in]	context	pointer to the context to be cancelled
 */
static
ssize_t efa_rdm_ep_cancel(fid_t fid_ep, void *context)
{
	struct efa_rdm_ep *ep;

	ep = container_of(fid_ep, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
	return ep->peer_srx_ep->ops->cancel(&ep->peer_srx_ep->fid, context);
}

/**
 * @brief set the FI_OPT_FI_HMEM_P2P option on the endpoint
 *
 * Validate p2p opt passed by the user and set the endpoint option if it is
 * valid. If the option chosen is invalid or not supported, return an error.
 *
 * @param[in]	efa_rdm_ep	EFA RDM endpoint
 * @return 	0 on success, negative errno on error
 */
static int efa_rdm_ep_set_fi_hmem_p2p_opt(struct efa_rdm_ep *efa_rdm_ep, int opt)
{
	int i, err;

	/*
	 * Check the opt's validity against the first initialized non-system FI_HMEM
	 * interface
	 */
	/*
	 * TODO this assumes only one non-stantard interface is initialized at a
	 * time. Refactor to handle multiple initialized interfaces to impose
	 * tighter restrictions on valid p2p options.
	 */
	EFA_HMEM_IFACE_FOREACH_NON_SYSTEM(i) {
		err = efa_domain_hmem_validate_p2p_opt(efa_rdm_ep_domain(efa_rdm_ep), efa_hmem_ifaces[i], opt);
		if (err == -FI_ENODATA)
			continue;

		if (!err)
			efa_rdm_ep->hmem_p2p_opt = opt;
		return err;
	}
	return -FI_EINVAL;
}

/**
 * @brief set cuda_api_permitted flag in efa_rdm_ep
 * called by efa_rdm_ep_setopt
 * @param[in,out]	ep			endpoint
 * @param[in]		cuda_api_permitted	whether cuda api is permitted
 * @return		0 on success,
 *			-FI_EOPNOTSUPP if endpoint relies on CUDA API call to support CUDA memory
 * @related efa_rdm_ep
 */
static int efa_rdm_ep_set_cuda_api_permitted(struct efa_rdm_ep *ep, bool cuda_api_permitted)
{
	if (!hmem_ops[FI_HMEM_CUDA].initialized) {
		EFA_WARN(FI_LOG_EP_CTRL, "FI_OPT_CUDA_API_PERMITTED cannot be set when "
			 "CUDA library or CUDA device is not available\n");
		return -FI_EINVAL;
	}

	if (cuda_api_permitted) {
		ep->cuda_api_permitted = true;
		return FI_SUCCESS;
	}

	/* CUDA memory can be supported by using either peer to peer or CUDA API. If neither is
	 * available, we cannot support CUDA memory
	 */
	if (!efa_rdm_ep_domain(ep)->hmem_info[FI_HMEM_CUDA].p2p_supported_by_device)
		return -FI_EOPNOTSUPP;

	ep->cuda_api_permitted = false;
	return 0;
}

/**
 * @brief act on shared_memory_permitted flag called by efa_rdm_ep_setopt
 * @param[in,out]	ep			endpoint
 * @param[in]		shm_permitted	whether shared memory is permitted
 * @return		0 on success,
 *			-FI_EINVAL if shm is requested but the FI_EFA_ENABLE_SHM_TRANSFER environment variable is set to false
 * @related efa_rdm_ep
 */
static int efa_rdm_ep_set_shared_memory_permitted(struct efa_rdm_ep *ep, bool shm_permitted)
{
	if (!shm_permitted) {
		EFA_WARN(FI_LOG_EP_CTRL,
			 "FI_OPT_SHARED_MEMORY_PERMITTED set to false");
		ep->shm_permitted = false;
		return FI_SUCCESS;
	}

	if (!efa_env.enable_shm_transfer) {
		EFA_WARN(FI_LOG_EP_CTRL,
			 "FI_OPT_SHARED_MEMORY_PERMITTED endpoint option set "
			 "to true but FI_EFA_ENABLE_SHM_TRANSFER environment "
			 "variable is set to false.");
		return -FI_EINVAL;
	}

	ep->shm_permitted = true;
	return 0;
}

/**
 * @brief set use_device_rdma flag in efa_rdm_ep.
 *
 * If the environment variable FI_EFA_USE_DEVICE_RDMA is set, this function will
 * return an error if the value of use_device_rdma is in conflict with the
 * environment setting.
 * called by efa_rdm_ep_setopt
 * @param[in,out]	ep			endpoint
 * @param[in]		use_device_rdma		when true, use device RDMA capabilities.
 * @return		0 on success
 *
 * @related efa_rdm_ep
 */
static int efa_rdm_ep_set_use_device_rdma(struct efa_rdm_ep *ep, bool use_device_rdma)
{
	bool env_value, env_set;

	uint32_t api_version =
		 efa_rdm_ep_domain(ep)->util_domain.fabric->fabric_fid.api_version;

	env_set = efa_env_has_use_device_rdma();
	if (env_set) {
		env_value = efa_rdm_get_use_device_rdma(api_version);
	}

	if FI_VERSION_LT(api_version, FI_VERSION(1, 18)) {
		/* let the application developer know something is wrong */
		EFA_WARN( FI_LOG_EP_CTRL,
			"Applications using libfabric API version <1.18 are not "
			"allowed to call fi_setopt with FI_OPT_EFA_USE_DEVICE_RDMA.  "
			"Please select a newer libfabric API version in "
			"fi_getinfo during startup to use this option.\n");
		return -FI_ENOPROTOOPT;
	}

	if (env_set && use_device_rdma && !env_value) {
		/* conflict: environment off, but application on */
		/* environment wins: turn it off */
		ep->use_device_rdma = env_value;
		EFA_WARN(FI_LOG_EP_CTRL,
		"Application used fi_setopt to request use_device_rdma, "
		"but user has disabled this by setting the environment "
		"variable FI_EFA_USE_DEVICE_RDMA to 1.\n");
		return -FI_EINVAL;
	}
	if (env_set && !use_device_rdma && env_value) {
		/* conflict: environment on, but application off */
		/* environment wins: turn it on */
		ep->use_device_rdma = env_value;
		EFA_WARN(FI_LOG_EP_CTRL,
		"Application used fi_setopt to disable use_device_rdma, "
		"but this conflicts with user's environment "
		"which has FI_EFA_USE_DEVICE_RDMA=1.  Proceeding with "
		"use_device_rdma=true\n");
		return -FI_EINVAL;
	}
	if (use_device_rdma && !efa_device_support_rdma_read()) {
		/* conflict: application on, hardware off. */
		/* hardware always wins ;-) */
		ep->use_device_rdma = false;
		EFA_WARN(FI_LOG_EP_CTRL,
		"Application used setopt to request use_device_rdma, "
		"but EFA device does not support it\n");
		return -FI_EOPNOTSUPP;
	}
	ep->use_device_rdma = use_device_rdma;
	return 0;
}

/**
 * @brief check the in order aligned 128 bytes support for a given ibv_wr_op code
 *
 * @param ep efa_rdm_ep
 * @param op_code ibv wr op code
 * @return int 0 if in order aligned 128 bytes is supported, -FI_EOPNOTSUPP if
 * it is not supported. Other negative integer for other errors.
 */
static
int efa_rdm_ep_check_qp_in_order_aligned_128_bytes(struct efa_rdm_ep *ep,
						   enum ibv_wr_opcode op_code)
{
	struct efa_qp *qp = NULL;
	struct ibv_qp_init_attr_ex attr_ex = {0};
	int ret, retv;
	struct ibv_cq_ex *ibv_cq_ex = NULL;
	enum ibv_cq_ex_type ibv_cq_ex_type;
	struct fi_cq_attr cq_attr = {0};

	ret = efa_cq_ibv_cq_ex_open(&cq_attr, efa_rdm_ep_domain(ep)->device->ibv_ctx, &ibv_cq_ex, &ibv_cq_ex_type);
	if (ret) {
		EFA_WARN(FI_LOG_CQ, "Unable to create extended CQ: %d\n", ret);
		ret = -FI_EINVAL;
		goto out;
	}

	/* Create a dummy qp for query only */
	efa_rdm_ep_construct_ibv_qp_init_attr_ex(ep, &attr_ex, ibv_cq_ex, ibv_cq_ex);

	ret = efa_qp_create(&qp, &attr_ex);
	if (ret)
		goto out;

	if (!efa_qp_support_op_in_order_aligned_128_bytes(qp, op_code))
		ret = -FI_EOPNOTSUPP;

out:
	if (qp) {
		retv = ibv_destroy_qp(qp->ibv_qp);
		if (retv)
			EFA_WARN(FI_LOG_EP_CTRL, "destroy ibv qp failed! err: %s\n",
				fi_strerror(-retv));
		free(qp);
	}

	if (ibv_cq_ex) {
		retv = -ibv_destroy_cq(ibv_cq_ex_to_cq(ibv_cq_ex));
		if (retv)
			EFA_WARN(FI_LOG_EP_CTRL, "Unable to close ibv cq: %s\n",
				fi_strerror(-retv));
	}
	return ret;
}

/**
 * @brief implement the fi_setopt() API for EFA RDM endpoint
 * @param[in]	fid		fid to endpoint
 * @param[in]	level		level of the option
 * @param[in]	optname		name of the option
 * @param[in]	optval		value of the option
 * @param[in]	optlen		length of the option
 * @related efa_rdm_ep
 *
 */
static int efa_rdm_ep_setopt(fid_t fid, int level, int optname,
			 const void *optval, size_t optlen)
{
	struct efa_rdm_ep *efa_rdm_ep;
	int intval, ret;
	struct util_srx_ctx *srx;

	efa_rdm_ep = container_of(fid, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);

	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:
		if (optlen != sizeof(size_t))
			return -FI_EINVAL;

		efa_rdm_ep->min_multi_recv_size = *(size_t *)optval;
		srx = util_get_peer_srx(efa_rdm_ep->peer_srx_ep)->ep_fid.fid.context;
		srx->min_multi_recv_size = *(size_t *)optval;
		break;
	case FI_OPT_EFA_RNR_RETRY:
		if (optlen != sizeof(size_t))
			return -FI_EINVAL;

		/*
		 * Application is required to call to fi_setopt before EP
		 * enabled. If it's calling to fi_setopt after EP enabled,
		 * fail the call.
		 *
		 * efa_ep->qp will be NULL before EP enabled, use it to check
		 * if the call to fi_setopt is before or after EP enabled for
		 * convience, instead of calling to ibv_query_qp
		 */
		if (efa_rdm_ep->base_ep.efa_qp_enabled) {
			EFA_WARN(FI_LOG_EP_CTRL,
				"The option FI_OPT_EFA_RNR_RETRY is required \
				to be set before EP enabled %s\n", __func__);
			return -FI_EINVAL;
		}

		if (!efa_domain_support_rnr_retry_modify(efa_rdm_ep_domain(efa_rdm_ep))) {
			EFA_WARN(FI_LOG_EP_CTRL,
				"RNR capability is not supported %s\n", __func__);
			return -FI_ENOSYS;
		}
		efa_rdm_ep->base_ep.rnr_retry = *(size_t *)optval;
		break;
	case FI_OPT_FI_HMEM_P2P:
		if (optlen != sizeof(int))
			return -FI_EINVAL;

		intval = *(int *)optval;

		ret = efa_rdm_ep_set_fi_hmem_p2p_opt(efa_rdm_ep, intval);
		if (ret)
			return ret;
		break;
	case FI_OPT_CUDA_API_PERMITTED:
		if (optlen != sizeof(bool))
			return -FI_EINVAL;
		ret = efa_rdm_ep_set_cuda_api_permitted(efa_rdm_ep, *(bool *)optval);
		if (ret)
			return ret;
		break;
	case FI_OPT_SHARED_MEMORY_PERMITTED:
		if (optlen != sizeof(bool))
			return -FI_EINVAL;
		ret = efa_rdm_ep_set_shared_memory_permitted(efa_rdm_ep, *(bool *)optval);
		if (ret)
			return ret;
		break;
	case FI_OPT_EFA_USE_DEVICE_RDMA:
		if (optlen != sizeof(bool))
			return -FI_EINVAL;
		ret = efa_rdm_ep_set_use_device_rdma(efa_rdm_ep, *(bool *)optval);
		if (ret)
			return ret;
		break;
	case FI_OPT_EFA_SENDRECV_IN_ORDER_ALIGNED_128_BYTES:
		if (optlen != sizeof(bool))
			return -FI_EINVAL;
		/*
		 * RDMA read is used to copy data from host bounce buffer to the
		 * application buffer on device
		 */
		if (*(bool *)optval) {
			ret = efa_rdm_ep_check_qp_in_order_aligned_128_bytes(efa_rdm_ep, IBV_WR_RDMA_READ);
			if (ret)
				return ret;
		}
		efa_rdm_ep->sendrecv_in_order_aligned_128_bytes = *(bool *)optval;
		break;
	case FI_OPT_EFA_WRITE_IN_ORDER_ALIGNED_128_BYTES:
		if (optlen != sizeof(bool))
			return -FI_EINVAL;
		if (*(bool *)optval) {
			ret = efa_rdm_ep_check_qp_in_order_aligned_128_bytes(efa_rdm_ep, IBV_WR_RDMA_WRITE);
			if (ret)
				return ret;
		}
		efa_rdm_ep->write_in_order_aligned_128_bytes = *(bool *)optval;
		break;
	default:
		EFA_WARN(FI_LOG_EP_CTRL,
			"Unknown endpoint option %s\n", __func__);
		return -FI_ENOPROTOOPT;
	}

	return FI_SUCCESS;
}

/**
 * @brief implement the fi_getopt() API for EFA RDM endpoint
 * @param[in]	fid		fid to endpoint
 * @param[in]	level		level of the option
 * @param[in]	optname		name of the option
 * @param[out]	optval		value of the option
 * @param[out]	optlen		length of the option
 */
static int efa_rdm_ep_getopt(fid_t fid, int level, int optname, void *optval,
			 size_t *optlen)
{
	struct efa_rdm_ep *efa_rdm_ep;

	efa_rdm_ep = container_of(fid, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);

	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:
		if (*optlen < sizeof(size_t))
			return -FI_ETOOSMALL;
		*(size_t *)optval = efa_rdm_ep->min_multi_recv_size;
		*optlen = sizeof(size_t);
		break;
	case FI_OPT_EFA_RNR_RETRY:
		if (*optlen < sizeof(size_t))
			return -FI_ETOOSMALL;
		*(size_t *)optval = efa_rdm_ep->base_ep.rnr_retry;
		*optlen = sizeof(size_t);
		break;
	case FI_OPT_FI_HMEM_P2P:
		if (*optlen < sizeof(int))
			return -FI_ETOOSMALL;
		*(int *)optval = efa_rdm_ep->hmem_p2p_opt;
		*optlen = sizeof(int);
		break;
	case FI_OPT_EFA_EMULATED_READ:
		if (*optlen < sizeof(bool))
			return -FI_ETOOSMALL;
		*(bool *)optval = !efa_rdm_ep_support_rdma_read(efa_rdm_ep);
		*optlen = sizeof(bool);
		break;
	case FI_OPT_EFA_EMULATED_WRITE:
		if (*optlen < sizeof(bool))
			return -FI_ETOOSMALL;
		*(bool *)optval = !efa_rdm_ep_support_rdma_write(efa_rdm_ep);
		*optlen = sizeof(bool);
		break;
	case FI_OPT_EFA_EMULATED_ATOMICS:
		if (*optlen < sizeof(bool))
			return -FI_ETOOSMALL;
		*(bool *)optval = true;
		*optlen = sizeof(bool);
		break;
	case FI_OPT_EFA_USE_DEVICE_RDMA:
		if (*optlen < sizeof(bool))
			return -FI_ETOOSMALL;
		*(bool *)optval = efa_rdm_ep->use_device_rdma;
		*optlen = sizeof(bool);
		break;
	case FI_OPT_EFA_SENDRECV_IN_ORDER_ALIGNED_128_BYTES:
		if (*optlen < sizeof(bool))
			return -FI_ETOOSMALL;
		*(bool *)optval = efa_rdm_ep->sendrecv_in_order_aligned_128_bytes;
		*optlen = sizeof(bool);
		break;
	case FI_OPT_EFA_WRITE_IN_ORDER_ALIGNED_128_BYTES:
		if (*optlen < sizeof(bool))
			return -FI_ETOOSMALL;
		*(bool *)optval = efa_rdm_ep->write_in_order_aligned_128_bytes;
		*optlen = sizeof(bool);
		break;
	default:
		EFA_WARN(FI_LOG_EP_CTRL,
			"Unknown endpoint option %s\n", __func__);
		return -FI_ENOPROTOOPT;
	}

	return FI_SUCCESS;
}
