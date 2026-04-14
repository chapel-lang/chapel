/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include <sys/time.h>
#include "efa.h"
#include "efa_av.h"
#include "efa_cq.h"
#include "efa_cntr.h"
#include "rdm/efa_rdm_protocol.h"
#include "efa_data_path_direct.h"

int efa_base_ep_bind_av(struct efa_base_ep *base_ep, struct efa_av *av)
{
	if (base_ep->domain != av->domain) {
		EFA_WARN(FI_LOG_EP_CTRL,
			 "Address vector doesn't belong to same domain as EP.\n");
		return -FI_EINVAL;
	}
	if (base_ep->av) {
		EFA_WARN(FI_LOG_EP_CTRL,
			 "Address vector already bound to EP.\n");
		return -FI_EINVAL;
	}

	base_ep->av = av;
	return 0;
}

static inline void efa_base_ep_lock_cq(struct efa_base_ep *base_ep)
{
	struct efa_cq *tx_cq, *rx_cq;

	tx_cq = efa_base_ep_get_tx_cq(base_ep);
	rx_cq = efa_base_ep_get_rx_cq(base_ep);

	if (rx_cq)
		ofi_genlock_lock(&rx_cq->util_cq.ep_list_lock);
	if (tx_cq && tx_cq != rx_cq)
		ofi_genlock_lock(&tx_cq->util_cq.ep_list_lock);
}

static inline void efa_base_ep_unlock_cq(struct efa_base_ep *base_ep)
{
	struct efa_cq *tx_cq, *rx_cq;

	tx_cq = efa_base_ep_get_tx_cq(base_ep);
	rx_cq = efa_base_ep_get_rx_cq(base_ep);

	if (tx_cq && tx_cq != rx_cq)
		ofi_genlock_unlock(&tx_cq->util_cq.ep_list_lock);
	if (rx_cq)
		ofi_genlock_unlock(&rx_cq->util_cq.ep_list_lock);
}

int efa_base_ep_destruct_qp(struct efa_base_ep *base_ep)
{
	struct efa_domain *domain;
	struct efa_qp *qp = base_ep->qp;
	struct efa_qp *user_recv_qp = base_ep->user_recv_qp;
	uint32_t qp_num;
	struct efa_cq *tx_cq, *rx_cq;
	int err;

	/*
	 * Acquire the lock to prevent race conditions when CQ polling accesses the qp_table
	 * and the qp resource
	 */
	efa_base_ep_lock_cq(base_ep);
	if (qp) {
		domain = qp->base_ep->domain;
		qp_num = qp->qp_num;
		efa_qp_destruct(qp);
		domain->qp_table[qp_num & domain->qp_table_sz_m1] = NULL;
		base_ep->qp = NULL;
	}

	if (user_recv_qp) {
		domain = user_recv_qp->base_ep->domain;
		qp_num = user_recv_qp->qp_num;
		efa_qp_destruct(user_recv_qp);
		domain->qp_table[qp_num & domain->qp_table_sz_m1] = NULL;
		base_ep->user_recv_qp = NULL;
	}

	/* Flush the cq to poll out all stale cqes for the destroyed qp */
	tx_cq = efa_base_ep_get_tx_cq(base_ep);
	rx_cq = efa_base_ep_get_rx_cq(base_ep);

	if (tx_cq) {
		err = 0;
		while (err != ENOENT) {
			err = tx_cq->poll_ibv_cq(-1, &tx_cq->ibv_cq);
		}
	}

	if (rx_cq && rx_cq != tx_cq) {
		err = 0;
		while (err != ENOENT) {
			err = rx_cq->poll_ibv_cq(-1, &rx_cq->ibv_cq);
		}
	}

	efa_base_ep_unlock_cq(base_ep);

	return 0;
}

void efa_base_ep_close_util_ep(struct efa_base_ep *base_ep)
{
	int err;

	if (base_ep->util_ep_initialized) {
		err = ofi_endpoint_close(&base_ep->util_ep);
		if (err)
			EFA_WARN(FI_LOG_EP_CTRL, "Unable to close util EP\n");
		base_ep->util_ep_initialized = false;
	}
}

int efa_base_ep_destruct(struct efa_base_ep *base_ep)
{
	int err;

	/* We need to free the util_ep first to avoid race conditions
	 * with other threads progressing the cq. */
	efa_base_ep_close_util_ep(base_ep);

	fi_freeinfo(base_ep->info);

	if (base_ep->self_ah)
		efa_ah_release(base_ep->domain, base_ep->self_ah);

	err = efa_base_ep_destruct_qp(base_ep);

	if (base_ep->efa_recv_wr_vec)
		free(base_ep->efa_recv_wr_vec);
	
	if (base_ep->user_recv_wr_vec)
		free(base_ep->user_recv_wr_vec);

	return err;
}

static int efa_generate_rdm_connid(void)
{
	struct timeval tv;
	uint32_t val;
	int err;

	err = gettimeofday(&tv, NULL);
	if (err) {
		EFA_WARN(FI_LOG_EP_CTRL, "Cannot gettimeofday, err=%d.\n", err);
		return 0;
	}

	/* tv_usec is in range [0,1,000,000), shift it by 12 to [0,4,096,000,000 */
	val = (tv.tv_usec << 12) + tv.tv_sec;

	val = ofi_xorshift_random(val);

	/* 0x80000000 and up is privileged Q Key range. */
	val &= 0x7fffffff;

	return val;
}

static int efa_base_ep_modify_qp_state(struct efa_base_ep *base_ep,
				       struct efa_qp *qp,
				       enum ibv_qp_state qp_state,
				       int attr_mask)
{
	struct ibv_qp_attr attr = { 0 };

	attr.qp_state = qp_state;

	if (attr_mask & IBV_QP_PORT)
		attr.port_num = 1;

	if (attr_mask & IBV_QP_QKEY)
		attr.qkey = qp->qkey;

	if (attr_mask & IBV_QP_RNR_RETRY)
		attr.rnr_retry = base_ep->rnr_retry;

	return -ibv_modify_qp(qp->ibv_qp, &attr, attr_mask);
}

static int efa_base_ep_modify_qp_rst2rts(struct efa_base_ep *base_ep,
					 struct efa_qp *qp)
{
	int err;

	err = efa_base_ep_modify_qp_state(base_ep, qp, IBV_QPS_INIT,
					  IBV_QP_STATE | IBV_QP_PKEY_INDEX |
						  IBV_QP_PORT | IBV_QP_QKEY);
	if (err)
		return err;

	err = efa_base_ep_modify_qp_state(base_ep, qp, IBV_QPS_RTR,
					  IBV_QP_STATE);
	if (err)
		return err;

	if (base_ep->util_ep.type != FI_EP_DGRAM &&
	    efa_domain_support_rnr_retry_modify(base_ep->domain))
		return efa_base_ep_modify_qp_state(
			base_ep, qp, IBV_QPS_RTS,
			IBV_QP_STATE | IBV_QP_SQ_PSN | IBV_QP_RNR_RETRY);

	return efa_base_ep_modify_qp_state(base_ep, qp, IBV_QPS_RTS,
					   IBV_QP_STATE | IBV_QP_SQ_PSN);
}

/**
 * @brief Create a efa_qp
 *
 * @param qp double pointer of struct efa_qp
 * @param init_attr_ex ibv_qp_init_attr_ex
 * @param tclass traffic class (QoS) of the qp
 * @param use_unsolicited_write_recv whether to use unsolicited write recv in the qp
 * @return int 0 on success, negative integer on failure
 */
int efa_qp_create(struct efa_qp **qp, struct ibv_qp_init_attr_ex *init_attr_ex,
		   uint32_t tclass, bool use_unsolicited_write_recv)
{
	struct efadv_qp_init_attr efa_attr = { 0 };

	*qp = calloc(1, sizeof(struct efa_qp));
	if (!*qp)
		return -FI_ENOMEM;

	init_attr_ex->comp_mask = IBV_QP_INIT_ATTR_PD | IBV_QP_INIT_ATTR_SEND_OPS_FLAGS;
	init_attr_ex->send_ops_flags |= IBV_QP_EX_WITH_SEND | IBV_QP_EX_WITH_SEND_WITH_IMM;

	if (init_attr_ex->qp_type == IBV_QPT_UD) {
		(*qp)->ibv_qp = ibv_create_qp_ex(init_attr_ex->pd->context,
					      init_attr_ex);
	} else {
		assert(init_attr_ex->qp_type == IBV_QPT_DRIVER);
		if (efa_device_support_rdma_read())
			init_attr_ex->send_ops_flags |= IBV_QP_EX_WITH_RDMA_READ;
		if (efa_device_support_rdma_write()) {
			init_attr_ex->send_ops_flags |= IBV_QP_EX_WITH_RDMA_WRITE;
			init_attr_ex->send_ops_flags |= IBV_QP_EX_WITH_RDMA_WRITE_WITH_IMM;
		}
		(*qp)->unsolicited_write_recv_enabled = use_unsolicited_write_recv;
#if HAVE_CAPS_UNSOLICITED_WRITE_RECV
		if (use_unsolicited_write_recv)
			efa_attr.flags |= EFADV_QP_FLAGS_UNSOLICITED_WRITE_RECV;
#endif
		efa_attr.driver_qp_type = EFADV_QP_DRIVER_TYPE_SRD;
#if HAVE_EFADV_SL
		efa_attr.sl = EFA_QP_DEFAULT_SERVICE_LEVEL;
		if (tclass == FI_TC_LOW_LATENCY)
			efa_attr.sl = EFA_QP_LOW_LATENCY_SERVICE_LEVEL;
#endif
		(*qp)->ibv_qp = efadv_create_qp_ex(
			init_attr_ex->pd->context, init_attr_ex, &efa_attr,
			sizeof(struct efadv_qp_init_attr));
	}

#if HAVE_EFADV_SL
	if (!(*qp)->ibv_qp && tclass == FI_TC_LOW_LATENCY) {
		EFA_INFO(FI_LOG_EP_CTRL, "ibv_create_qp failed with sl %u, errno: %d. Retrying with default sl.\n", efa_attr.sl, errno);
		efa_attr.sl = EFA_QP_DEFAULT_SERVICE_LEVEL;
		(*qp)->ibv_qp = efadv_create_qp_ex(
			init_attr_ex->pd->context, init_attr_ex, &efa_attr,
			sizeof(struct efadv_qp_init_attr));
	}
#endif

	if (!(*qp)->ibv_qp) {
		EFA_WARN(FI_LOG_EP_CTRL, "ibv_create_qp failed. errno: %d\n", errno);
		free(*qp);
		*qp = NULL;
		return -errno;
	}

	(*qp)->ibv_qp_ex = ibv_qp_to_qp_ex((*qp)->ibv_qp);
	/* Initialize it explicitly for safety */
	(*qp)->data_path_direct_enabled = false;
	return FI_SUCCESS;
}

/**
 * @brief Construct the ibv qp init attr for given ep and cq
 *
 * @param ep a ptr to the efa_base_ep
 * @param attr_ex the constructed qp attr
 * @param tx_cq tx cq
 * @param rx_cq rx cq
 */
static inline
void efa_base_ep_construct_ibv_qp_init_attr_ex(struct efa_base_ep *ep,
						struct ibv_qp_init_attr_ex *attr_ex,
						struct ibv_cq_ex *tx_cq,
						struct ibv_cq_ex *rx_cq)
{
	struct fi_info *info;

	if (ep->info->ep_attr->type == FI_EP_RDM) {
		attr_ex->qp_type = IBV_QPT_DRIVER;
		info = ep->domain->device->rdm_info;
	} else {
		assert(ep->info->ep_attr->type == FI_EP_DGRAM);
		attr_ex->qp_type = IBV_QPT_UD;
		info = ep->domain->device->dgram_info;
	}
	attr_ex->cap.max_send_wr = info->tx_attr->size;
	attr_ex->cap.max_send_sge = info->tx_attr->iov_limit;
	attr_ex->cap.max_recv_wr = info->rx_attr->size;
	attr_ex->cap.max_recv_sge = info->rx_attr->iov_limit;
	attr_ex->cap.max_inline_data = ep->domain->device->efa_attr.inline_buf_size;
	attr_ex->pd = ep->domain->ibv_pd;
	attr_ex->qp_context = ep;
	attr_ex->sq_sig_all = 1;

	attr_ex->send_cq = ibv_cq_ex_to_cq(tx_cq);
	attr_ex->recv_cq = ibv_cq_ex_to_cq(rx_cq);
}

/**
 * @brief Create the IBV QP that backs the base ep
 *
 * @param base_ep efa_base_ep
 * @param create_user_recv_qp whether to create the user_recv_qp. This boolean
 * is only true for the zero copy recv mode in the efa-rdm endpoint
 *
 * @return int 0 on success, negative integer on failure
 */
static int efa_base_ep_create_qp(struct efa_base_ep *base_ep,
				  struct efa_ibv_cq *tx_cq,
				  struct efa_ibv_cq *rx_cq,
				  bool create_user_recv_qp)
{
	int ret;
	struct ibv_qp_init_attr_ex attr_ex = { 0 };

	efa_base_ep_construct_ibv_qp_init_attr_ex(base_ep, &attr_ex, tx_cq->ibv_cq_ex, rx_cq->ibv_cq_ex);

	assert(tx_cq->unsolicited_write_recv_enabled == rx_cq->unsolicited_write_recv_enabled);

	ret = efa_qp_create(&base_ep->qp, &attr_ex, base_ep->info->tx_attr->tclass,
			    tx_cq->unsolicited_write_recv_enabled);
	if (ret)
		return ret;

	base_ep->qp->base_ep = base_ep;

#if HAVE_EFA_DATA_PATH_DIRECT
	/* Only enable direct QP when direct CQ is enabled */
	assert(tx_cq->data_path_direct_enabled == rx_cq->data_path_direct_enabled);
	if (tx_cq->data_path_direct_enabled) {
		ret = efa_data_path_direct_qp_initialize(base_ep->qp);
		if (ret) {
			efa_base_ep_destruct_qp(base_ep);
			return ret;
		}
	}
#endif

	if (create_user_recv_qp) {
		ret = efa_qp_create(&base_ep->user_recv_qp, &attr_ex, base_ep->info->tx_attr->tclass, tx_cq->unsolicited_write_recv_enabled);
		if (ret) {
			efa_base_ep_destruct_qp(base_ep);
			return ret;
		}
		base_ep->user_recv_qp->base_ep = base_ep;
	}

	return 0;
}

static
int efa_base_ep_enable_qp(struct efa_base_ep *base_ep, struct efa_qp *qp)
{
	int err;

	qp->qkey = (base_ep->util_ep.type == FI_EP_DGRAM) ?
			   EFA_DGRAM_CONNID :
			   efa_generate_rdm_connid();
	err = efa_base_ep_modify_qp_rst2rts(base_ep, qp);
	if (err)
		return err;

	qp->qp_num = qp->ibv_qp->qp_num;

	base_ep->domain->qp_table[qp->qp_num & base_ep->domain->qp_table_sz_m1] = qp;
	
	EFA_INFO(FI_LOG_EP_CTRL, "QP enabled! qp_n: %d qkey: %d\n", qp->qp_num, qp->qkey);

	return err;
}

/* efa_base_ep_create_self_ah() create an address handler for
 * an EP's own address. The address handler is used by
 * an EP to read from itself. It is used to
 * copy data from host memory to GPU memory.
 */
static inline
int efa_base_ep_create_self_ah(struct efa_base_ep *base_ep, struct ibv_pd *ibv_pd)
{

	base_ep->self_ah = efa_ah_alloc(base_ep->domain, base_ep->src_addr.raw);

	return base_ep->self_ah ? 0 : -FI_EINVAL;
}

void efa_qp_destruct(struct efa_qp *qp)
{
	int err;

	err = -ibv_destroy_qp(qp->ibv_qp);
	if (err)
		EFA_INFO(FI_LOG_CORE, "destroy qp[%u] failed, err: %s\n", qp->qp_num, fi_strerror(-err));
#if HAVE_EFA_DATA_PATH_DIRECT
	if (qp->data_path_direct_enabled)
		efa_data_path_direct_qp_finalize(qp);
#endif
	free(qp);
}

int efa_base_ep_enable(struct efa_base_ep *base_ep)
{
	int err;

	err = efa_base_ep_enable_qp(base_ep, base_ep->qp);
	if (err) {
		efa_base_ep_destruct_qp(base_ep);
		return err;
	}

	base_ep->efa_qp_enabled = true;

	if (base_ep->user_recv_qp) {
		err = efa_base_ep_enable_qp(base_ep, base_ep->user_recv_qp);
		if (err) {
			efa_base_ep_destruct_qp(base_ep);
			return err;
		}
	}

	memcpy(base_ep->src_addr.raw, base_ep->domain->device->ibv_gid.raw, EFA_GID_LEN);
	base_ep->src_addr.qpn = base_ep->qp->qp_num;
	base_ep->src_addr.pad = 0;
	base_ep->src_addr.qkey = base_ep->qp->qkey;

	err = efa_base_ep_create_self_ah(base_ep, base_ep->domain->ibv_pd);
	if (err) {
		EFA_WARN(FI_LOG_EP_CTRL,
			 "Endpoint cannot create ah for its own address\n");
		efa_base_ep_destruct_qp(base_ep);
	}

	return err;
}

int efa_base_ep_construct(struct efa_base_ep *base_ep,
			  struct fid_domain *domain_fid,
			  struct fi_info *info,
			  ofi_ep_progress_func progress,
			  void *context)
{
	int err;

	err = ofi_endpoint_init(domain_fid, &efa_util_prov, info, &base_ep->util_ep,
				context, progress);
	if (err)
		return err;

	base_ep->util_ep_initialized = true;

	base_ep->domain = container_of(domain_fid, struct efa_domain, util_domain.domain_fid);

	base_ep->info = fi_dupinfo(info);
	if (!base_ep->info) {
		EFA_WARN(FI_LOG_EP_CTRL, "fi_dupinfo() failed for base_ep->info!\n");
		ofi_endpoint_close(&base_ep->util_ep);
		base_ep->util_ep_initialized = false;
		return -FI_ENOMEM;
	}

	/* This is SRD qp's default behavior */
	base_ep->rnr_retry = EFA_RNR_INFINITE_RETRY;

	base_ep->efa_recv_wr_vec = calloc(sizeof(struct efa_recv_wr), EFA_RDM_EP_MAX_WR_PER_IBV_POST_RECV);
	if (!base_ep->efa_recv_wr_vec) {
		EFA_WARN(FI_LOG_EP_CTRL, "cannot alloc memory for base_ep->efa_recv_wr_vec!\n");
		return -FI_ENOMEM;
	}
	base_ep->user_recv_wr_vec = calloc(sizeof(struct efa_recv_wr), EFA_RDM_EP_MAX_WR_PER_IBV_POST_RECV);
	if (!base_ep->user_recv_wr_vec) {
		EFA_WARN(FI_LOG_EP_CTRL, "cannot alloc memory for base_ep->user_recv_wr_vec!\n");
		return -FI_ENOMEM;
	}
	base_ep->recv_wr_index = 0;
	base_ep->efa_qp_enabled = false;
	base_ep->qp = NULL;
	base_ep->user_recv_qp = NULL;

	/* Use device's native limit as the default value of base ep*/
	base_ep->max_msg_size = (size_t) base_ep->domain->device->ibv_port_attr.max_msg_sz;
	base_ep->max_rma_size = (size_t) base_ep->domain->device->max_rdma_size;
	base_ep->inject_msg_size = (size_t) base_ep->domain->device->efa_attr.inline_buf_size;
	/* TODO: update inject_rma_size to inline size after firmware
	 * supports inline rdma write */
	base_ep->inject_rma_size = 0;
	return 0;
}

int efa_base_ep_getname(fid_t fid, void *addr, size_t *addrlen)
{
	struct efa_base_ep *base_ep;
	char str[INET6_ADDRSTRLEN] = { 0 };

	base_ep = container_of(fid, struct efa_base_ep, util_ep.ep_fid.fid);

	inet_ntop(AF_INET6, base_ep->src_addr.raw, str, INET6_ADDRSTRLEN);

	EFA_INFO(FI_LOG_EP_CTRL, "EP addr: GID[%s] QP[%d] QKEY[%d] (length %zu)\n",
		 str, base_ep->src_addr.qpn, base_ep->src_addr.qkey, *addrlen);

	if (*addrlen < EFA_EP_ADDR_LEN) {
		/*
		 * According to libfabric doc,
		 *
		 *  .  https://ofiwg.github.io/libfabric/v1.1.1/man/fi_cm.3.html
		 *
		 * addrlen is set to the size of the buffer needed to store the address,
		 * which may be larger than the input value.
		 *
		 * Some applications rely on this behavior. They call fi_getname()
		 * twice:
		 *
		 * On the first time, they pass NULL as addr, and 0 as
		 * addrlen, just to get the actual buffer size.
		 *
		 * They then allocate a buffer with the size returned,
		 * and call fi_getname() the 2nd time with the buffer.
		 */
		*addrlen = EFA_EP_ADDR_LEN;
		return -FI_ETOOSMALL;
	}

	*addrlen = EFA_EP_ADDR_LEN;
	memcpy(addr, &base_ep->src_addr, EFA_EP_ADDR_LEN);
	return 0;
}

/*
 * Determine if an endpoint supports an in-order operation for a 128-bytes piece
 * of data.
 *
 * @param[in]	base_ep	efa base endpoint
 * @param[in]	op	RDMA opcode
 * @return 	true if the base ep endpoint support the 128-bytes aligned
 * 		in-order operation.
 */
#if HAVE_EFA_DATA_IN_ORDER_ALIGNED_128_BYTES
bool efa_qp_support_op_in_order_aligned_128_bytes(struct efa_qp *qp, enum ibv_wr_opcode op)
{
	int caps;
	uint32_t flags = IBV_QUERY_QP_DATA_IN_ORDER_RETURN_CAPS;

#if HAVE_IBV_QUERY_QP_DATA_IN_ORDER_DEVICE_ONLY
	flags |= IBV_QUERY_QP_DATA_IN_ORDER_DEVICE_ONLY;
#endif

	caps = ibv_query_qp_data_in_order(qp->ibv_qp, op, flags);

	return !!(caps & IBV_QUERY_QP_DATA_IN_ORDER_ALIGNED_128_BYTES);
}
#else
bool efa_qp_support_op_in_order_aligned_128_bytes(struct efa_qp *qp, enum ibv_wr_opcode op)
{
	return false;
}
#endif

/**
 * @brief write error to event queue (EQ)
 * EQ error is written when
 *    a) write to CQ failed
 *    b) the error was not associated to an user action
 * Therefore it is really the last resort to report an error.
 * If the base_ep is not binded to an EQ, or write to EQ failed,
 * this function will print the error message to console, then abort()
 *
 * @param[in,out] ep	base endpoint
 * @param[in] err	OFI error code
 * @param[in] prov_errno	provider error code
 */
void efa_base_ep_write_eq_error(struct efa_base_ep *ep, ssize_t err, ssize_t prov_errno)
{
	struct fi_eq_err_entry err_entry;
	int ret = -FI_ENOEQ;

	EFA_WARN(FI_LOG_EQ, "Writing error to EQ: err: %s (%zd) prov_errno: %s (%zd)\n",
	         fi_strerror(err), err, efa_strerror(prov_errno), prov_errno);
	efa_show_help(prov_errno);
	if (ep->util_ep.eq) {
		memset(&err_entry, 0, sizeof(err_entry));
		err_entry.err = err;
		err_entry.prov_errno = prov_errno;
		ret = fi_eq_write(&ep->util_ep.eq->eq_fid, FI_NOTIFY,
				  &err_entry, sizeof(err_entry),
				  UTIL_FLAG_ERROR);

		if (ret == sizeof(err_entry))
			return;
	}

	EFA_WARN(FI_LOG_EQ, "Unable to write to EQ\n");
	fprintf(stderr,
		"Libfabric EFA provider has encountered an internal error:\n\n"
		"Libfabric error: (%zd) %s\n"
		"EFA internal error: (%zd) %s\n\n"
		"Your application will now abort().\n",
		err, fi_strerror(err),
		prov_errno, efa_strerror(prov_errno));
	abort();
}

const char *efa_base_ep_raw_addr_str(struct efa_base_ep *base_ep, char *buf, size_t *buflen)
{
	return ofi_straddr(buf, buflen, FI_ADDR_EFA, &base_ep->src_addr);
}

/**
 * @brief return peer's raw address in #efa_ep_addr
 *
 * @param[in] ep		end point
 * @param[in] addr 		libfabric address
 * @returns
 * If peer exists, return peer's raw addrress as pointer to #efa_ep_addr;
 * Otherwise, return NULL
 */
struct efa_ep_addr *efa_base_ep_get_peer_raw_addr(struct efa_base_ep *base_ep, fi_addr_t addr)
{
	struct efa_av *efa_av;
	struct efa_conn *efa_conn;

	efa_av = base_ep->av;
	efa_conn = efa_av_addr_to_conn(efa_av, addr);
	return efa_conn ? efa_conn->ep_addr : NULL;
}

/**
 * @brief return peer's raw address in a readable string
 *
 * @param[in] base_ep   end point
 * @param[in] addr 		libfabric address
 * @param[out] buf		a buffer to be used to store string
 * @param[in,out] buflen	length of `buf` as input. length of the string as output.
 * @return a string with peer's raw address
 */
const char *efa_base_ep_get_peer_raw_addr_str(struct efa_base_ep *base_ep, fi_addr_t addr, char *buf, size_t *buflen)
{
	return ofi_straddr(buf, buflen, FI_ADDR_EFA, efa_base_ep_get_peer_raw_addr(base_ep, addr));
}

struct efa_cq *efa_base_ep_get_tx_cq(struct efa_base_ep *ep)
{
	return ep->util_ep.tx_cq ? container_of(ep->util_ep.tx_cq, struct efa_cq, util_cq) : NULL;
}

struct efa_cq *efa_base_ep_get_rx_cq(struct efa_base_ep *ep)
{
	return ep->util_ep.rx_cq ? container_of(ep->util_ep.rx_cq, struct efa_cq, util_cq) : NULL;
}

/**
 * @brief check the in order aligned 128 bytes support for a given ibv_wr_op code
 *
 * @param ep efa_base_ep
 * @param op_code ibv wr op code
 * @return int 0 if in order aligned 128 bytes is supported, -FI_EOPNOTSUPP if
 * it is not supported. Other negative integer for other errors.
 */
int efa_base_ep_check_qp_in_order_aligned_128_bytes(struct efa_base_ep *ep,
						     enum ibv_wr_opcode op_code)
{
	struct efa_qp *qp = NULL;
	struct ibv_qp_init_attr_ex attr_ex = {0};
	int ret, retv;
	struct efa_ibv_cq ibv_cq = {0};
	struct fi_cq_attr cq_attr = {0};
	struct fi_efa_cq_init_attr efa_cq_init_attr = {0};

	ret = efa_cq_open_ibv_cq(&cq_attr, ep->domain->device->ibv_ctx, &ibv_cq,
				 &efa_cq_init_attr);
	if (ret) {
		EFA_WARN(FI_LOG_CQ, "Unable to create extended CQ: %d\n", ret);
		ret = -FI_EINVAL;
		goto out;
	}

	/* Create a dummy qp for query only */
	efa_base_ep_construct_ibv_qp_init_attr_ex(ep, &attr_ex, ibv_cq.ibv_cq_ex, ibv_cq.ibv_cq_ex);

	ret = efa_qp_create(&qp, &attr_ex, FI_TC_UNSPEC, ibv_cq.unsolicited_write_recv_enabled);
	if (ret)
		goto out;

	if (!efa_qp_support_op_in_order_aligned_128_bytes(qp, op_code))
		ret = -FI_EOPNOTSUPP;

out:
	if (qp)
		efa_qp_destruct(qp);

	if (ibv_cq.ibv_cq_ex) {
		retv = -ibv_destroy_cq(ibv_cq_ex_to_cq(ibv_cq.ibv_cq_ex));
		if (retv)
			EFA_WARN(FI_LOG_EP_CTRL, "Unable to close ibv cq: %s\n",
				fi_strerror(-retv));
	}
	return ret;
}



/**
 * @brief Set CQ ops for counter binding
 *
 * @param ep efa_base_ep
 * @param cq efa_cq to configure
 */
static inline void efa_base_ep_set_cq_ops_for_cntr(struct efa_base_ep *ep, struct efa_cq *cq)
{
	if (!EFA_INFO_TYPE_IS_RDM(ep->info))
		cq->util_cq.cq_fid.ops = &efa_cq_ops;
}

/**
 * @brief Insert tx/rx cq into the cntrs the ep is bind to
 *
 * @param ep efa_base_ep
 * @return int 0 on success, negative integer on failure
 */
int efa_base_ep_insert_cntr_ibv_cq_poll_list(struct efa_base_ep *ep)
{
	int i, ret;
	struct efa_cntr *efa_cntr;
	struct util_cntr *util_cntr;
	struct efa_cq *tx_cq, *rx_cq;

	tx_cq = efa_base_ep_get_tx_cq(ep);
	rx_cq = efa_base_ep_get_rx_cq(ep);

	for (i = 0; i < CNTR_CNT; i++) {
		util_cntr = ep->util_ep.cntrs[i];
		if (util_cntr) {
			efa_cntr = container_of(util_cntr, struct efa_cntr, util_cntr);
			/* Switch CQ to bypass OFF mode when cntr is bound */
			if (tx_cq) {
				ret = efa_ibv_cq_poll_list_insert(&efa_cntr->ibv_cq_poll_list, &efa_cntr->util_cntr.ep_list_lock, &tx_cq->ibv_cq);
				if (ret)
					return ret;
				efa_base_ep_set_cq_ops_for_cntr(ep, tx_cq);
			}
			if (rx_cq) {
				ret = efa_ibv_cq_poll_list_insert(&efa_cntr->ibv_cq_poll_list, &efa_cntr->util_cntr.ep_list_lock, &rx_cq->ibv_cq);
				if (ret)
					return ret;
				efa_base_ep_set_cq_ops_for_cntr(ep, rx_cq);
			}
			ofi_genlock_lock(&efa_cntr->util_cntr.ep_list_lock);
			efa_cntr->need_to_scan_ep_list = true;
			ofi_genlock_unlock(&efa_cntr->util_cntr.ep_list_lock);
		}
	}

	return FI_SUCCESS;
}

/**
 * @brief Remove tx/rx cq from the cntr that ep is bind to
 *
 * @param ep efa_base_ep
 */
void efa_base_ep_remove_cntr_ibv_cq_poll_list(struct efa_base_ep *ep)
{
	int i;
	struct efa_cntr *efa_cntr;
	struct util_cntr *util_cntr;
	struct efa_cq *tx_cq, *rx_cq;

	tx_cq = efa_base_ep_get_tx_cq(ep);
	rx_cq = efa_base_ep_get_rx_cq(ep);

	for (i = 0; i< CNTR_CNT; i++) {
		util_cntr = ep->util_ep.cntrs[i];
		if (util_cntr) {
			efa_cntr = container_of(util_cntr, struct efa_cntr, util_cntr);
			if (tx_cq && !ofi_atomic_get32(&tx_cq->util_cq.ref))
				efa_ibv_cq_poll_list_remove(&efa_cntr->ibv_cq_poll_list, &efa_cntr->util_cntr.ep_list_lock, &tx_cq->ibv_cq);

			if (rx_cq && !ofi_atomic_get32(&rx_cq->util_cq.ref))
				efa_ibv_cq_poll_list_remove(&efa_cntr->ibv_cq_poll_list, &efa_cntr->util_cntr.ep_list_lock, &rx_cq->ibv_cq);
		}
	}
}

/**
 * @brief Create and enable the IBV QP that backs the EP
 *
 * @param ep efa_base_ep
 * @param create_user_recv_qp whether to create the user_recv_qp. This boolean
 * is only true for the zero copy recv mode in the efa-rdm endpoint
 *
 * @return int 0 on success, negative integer on failure
 */
int efa_base_ep_create_and_enable_qp(struct efa_base_ep *ep, bool create_user_recv_qp)
{
	struct efa_cq *scq, *rcq, *txcq, *rxcq;
	int err;

	txcq = efa_base_ep_get_tx_cq(ep);
	rxcq = efa_base_ep_get_rx_cq(ep);

	if (!txcq && !rxcq) {
		EFA_WARN(FI_LOG_EP_CTRL,
			"Endpoint is not bound to a send or receive completion queue\n");
		return -FI_ENOCQ;
	}

	if (!txcq && ofi_needs_tx(ep->info->caps)) {
		EFA_WARN(FI_LOG_EP_CTRL,
			"Endpoint is not bound to a send completion queue when it has transmit capabilities enabled (FI_SEND).\n");
		return -FI_ENOCQ;
	}

	if (!rxcq && ofi_needs_rx(ep->info->caps)) {
		EFA_WARN(FI_LOG_EP_CTRL,
			"Endpoint is not bound to a receive completion queue when it has receive capabilities enabled. (FI_RECV)\n");
		return -FI_ENOCQ;
	}

	scq = txcq ? txcq : rxcq;
	rcq = rxcq ? rxcq : txcq;

	/* Acquire the lock to prevent race conditions when CQ polling accesses the qp_table */
	efa_base_ep_lock_cq(ep);
	err = efa_base_ep_create_qp(ep, &scq->ibv_cq, &rcq->ibv_cq, create_user_recv_qp);
	if (err)
		goto out;

	err = efa_base_ep_enable(ep);

out:
	efa_base_ep_unlock_cq(ep);
	return err;
}

#if ENABLE_DEBUG
void efa_ep_addr_print(char *prefix, struct efa_ep_addr *addr) {
	char raw_gid_str[INET6_ADDRSTRLEN];

	memset(raw_gid_str, 0, sizeof(raw_gid_str));
	if (!inet_ntop(AF_INET6, addr->raw, raw_gid_str, INET6_ADDRSTRLEN)) {
		EFA_DBG(FI_LOG_AV, "couldn't convert raw addr to string");
	}
	EFA_DBG(FI_LOG_AV, "%s raw: %s qpn: %d, qkey: %d\n", prefix, raw_gid_str, addr->qpn, addr->qkey);
}
#endif
