/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include <sys/time.h>
#include "efa.h"
#include "efa_av.h"
#include "rdm/efa_rdm_protocol.h"

int efa_base_ep_bind_av(struct efa_base_ep *base_ep, struct efa_av *av)
{
	/*
	 * Binding multiple endpoints to a single AV is currently not
	 * supported.
	 */
	if (av->base_ep) {
		EFA_WARN(FI_LOG_EP_CTRL,
			 "Address vector already has endpoint bound to it.\n");
		return -FI_ENOSYS;
	}
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
	base_ep->av->base_ep = base_ep;

	return 0;
}

int efa_base_ep_destruct_qp(struct efa_base_ep *base_ep)
{
	struct efa_domain *domain;
	struct efa_qp *qp = base_ep->qp;
	int err;

	if (qp) {
		domain = qp->base_ep->domain;
		domain->qp_table[qp->qp_num & domain->qp_table_sz_m1] = NULL;
		err = -ibv_destroy_qp(qp->ibv_qp);
		if (err)
			EFA_INFO(FI_LOG_CORE, "destroy qp[%u] failed!\n", qp->qp_num);

		free(qp);
		base_ep->qp = NULL;
	}

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
		ibv_destroy_ah(base_ep->self_ah);

	err = efa_base_ep_destruct_qp(base_ep);

	if (base_ep->efa_recv_wr_vec)
		free(base_ep->efa_recv_wr_vec);

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
 * @return int 0 on success, negative integer on failure
 */
int efa_qp_create(struct efa_qp **qp, struct ibv_qp_init_attr_ex *init_attr_ex)
{
	struct efadv_qp_init_attr efa_attr = { 0 };

	*qp = calloc(1, sizeof(struct efa_qp));
	if (!*qp)
		return -FI_ENOMEM;

	if (init_attr_ex->qp_type == IBV_QPT_UD) {
		(*qp)->ibv_qp = ibv_create_qp_ex(init_attr_ex->pd->context,
					      init_attr_ex);
	} else {
		assert(init_attr_ex->qp_type == IBV_QPT_DRIVER);
		efa_attr.driver_qp_type = EFADV_QP_DRIVER_TYPE_SRD;
		(*qp)->ibv_qp = efadv_create_qp_ex(
			init_attr_ex->pd->context, init_attr_ex, &efa_attr,
			sizeof(struct efadv_qp_init_attr));
	}

	if (!(*qp)->ibv_qp) {
		EFA_WARN(FI_LOG_EP_CTRL, "ibv_create_qp failed. errno: %d\n", errno);
		free(*qp);
		*qp = NULL;
		return -errno;
	}

	(*qp)->ibv_qp_ex = ibv_qp_to_qp_ex((*qp)->ibv_qp);
	return FI_SUCCESS;
}

int efa_base_ep_create_qp(struct efa_base_ep *base_ep,
			  struct ibv_qp_init_attr_ex *init_attr_ex)
{
	int ret;

	ret = efa_qp_create(&base_ep->qp, init_attr_ex);
	if (ret)
		return ret;

	base_ep->qp->base_ep = base_ep;
	return 0;
}

static
int efa_base_ep_enable_qp(struct efa_base_ep *base_ep)
{
	struct efa_qp *qp;
	int err;

	qp = base_ep->qp;
	qp->qkey = (base_ep->util_ep.type == FI_EP_DGRAM) ?
			   EFA_DGRAM_CONNID :
			   efa_generate_rdm_connid();
	err = efa_base_ep_modify_qp_rst2rts(base_ep, qp);
	if (err)
		return err;

	base_ep->efa_qp_enabled = true;

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
	struct ibv_ah_attr ah_attr;

	memset(&ah_attr, 0, sizeof(ah_attr));
	ah_attr.port_num = 1;
	ah_attr.is_global = 1;
	memcpy(ah_attr.grh.dgid.raw, base_ep->src_addr.raw, EFA_GID_LEN);
	base_ep->self_ah = ibv_create_ah(ibv_pd, &ah_attr);
	return base_ep->self_ah ? 0 : -FI_EINVAL;
}

int efa_base_ep_enable(struct efa_base_ep *base_ep)
{
	int err;

	err = efa_base_ep_enable_qp(base_ep);
	if (err)
		return err;

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

	base_ep->rnr_retry = efa_env.rnr_retry;

	base_ep->xmit_more_wr_tail = &base_ep->xmit_more_wr_head;
	base_ep->recv_more_wr_tail = &base_ep->recv_more_wr_head;
	base_ep->efa_recv_wr_vec = calloc(sizeof(struct efa_recv_wr), EFA_RDM_EP_MAX_WR_PER_IBV_POST_RECV);
	if (!base_ep->efa_recv_wr_vec) {
		EFA_WARN(FI_LOG_EP_CTRL, "cannot alloc memory for base_ep->efa_recv_wr_vec!\n");
		return -FI_ENOMEM;
	}
	base_ep->efa_qp_enabled = false;
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

	caps = ibv_query_qp_data_in_order(qp->ibv_qp, op,
					  IBV_QUERY_QP_DATA_IN_ORDER_RETURN_CAPS);

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
