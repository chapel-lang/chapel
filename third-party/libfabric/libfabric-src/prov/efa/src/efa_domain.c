/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright (c) 2013-2015 Intel Corporation, Inc.  All rights reserved. */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include <assert.h>
#include <ofi_util.h>

#include "config.h"
#include "efa.h"
#include "efa_av.h"
#include "efa_cntr.h"
#include "rdm/efa_rdm_cq.h"
#include "rdm/efa_rdm_atomic.h"


struct dlist_entry g_efa_domain_list;

static int efa_domain_close(fid_t fid);

static int efa_domain_ops_open(struct fid *fid, const char *ops_name,
				uint64_t flags, void **ops, void *context);

static struct fi_ops efa_ops_domain_fid = {
	.size = sizeof(struct fi_ops),
	.close = efa_domain_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = efa_domain_ops_open,
};

static struct fi_ops_domain efa_domain_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = efa_av_open,
	.cq_open = efa_cq_open,
	.endpoint = efa_ep_open,
	.scalable_ep = fi_no_scalable_ep,
	.cntr_open = efa_cntr_open,
	.poll_open = fi_no_poll_open,
	.stx_ctx = fi_no_stx_context,
	.srx_ctx = fi_no_srx_context,
	.query_atomic = fi_no_query_atomic,
	.query_collective = fi_no_query_collective,
};

static struct fi_ops_domain efa_domain_ops_rdm = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = efa_av_open,
	.cq_open = efa_rdm_cq_open,
	.endpoint = efa_rdm_ep_open,
	.scalable_ep = fi_no_scalable_ep,
	.cntr_open = efa_rdm_cntr_open,
	.poll_open = fi_poll_create,
	.stx_ctx = fi_no_stx_context,
	.srx_ctx = fi_no_srx_context,
	.query_atomic = efa_rdm_atomic_query,
	.query_collective = fi_no_query_collective,
};

/**
 * @brief init the device and ibv_pd field in efa_domain
 *
 * @param efa_domain[in,out]	efa domain to be set.
 * @param domain_name		domain name
 * @param ep_type		endpoint type
 * @return 0 if efa_domain->device and efa_domain->ibv_pd has been set successfully
 *         negative error code if err is encountered
 */
static int efa_domain_init_device_and_pd(struct efa_domain *efa_domain,
                                         const char *domain_name,
                                         enum fi_ep_type ep_type)
{
	int i;
	char *device_name = NULL;
	const char *domain_name_suffix = efa_domain_name_suffix(ep_type);

	if (!domain_name)
		return -FI_EINVAL;

	for (i = 0; i < g_efa_selected_device_cnt; i++) {
		device_name = g_efa_selected_device_list[i].ibv_ctx->device->name;
		if (strstr(domain_name, device_name) == domain_name &&
		    strlen(domain_name) - strlen(device_name) ==
		            strlen(domain_name_suffix) &&
		    strcmp((const char *) (domain_name + strlen(device_name)),
		           domain_name_suffix) == 0) {
			efa_domain->device = &g_efa_selected_device_list[i];
			break;
		}
	}

	if (i == g_efa_selected_device_cnt)
		return -FI_ENODEV;

	efa_domain->ibv_pd = ibv_alloc_pd(efa_domain->device->ibv_ctx);
	if (!efa_domain->ibv_pd) {
		EFA_WARN(FI_LOG_DOMAIN, "Failed to allocated ibv_pd: %d\n", errno);
		return -FI_ENOMEM;
	}

	EFA_INFO(FI_LOG_DOMAIN, "Domain %s selected device %s\n", domain_name, device_name);
	return 0;
}

static int efa_domain_init_qp_table(struct efa_domain *efa_domain)
{
	size_t qp_table_size;

	qp_table_size = roundup_power_of_two(efa_domain->device->ibv_attr.max_qp);
	efa_domain->qp_table_sz_m1 = qp_table_size - 1;
	efa_domain->qp_table = calloc(qp_table_size, sizeof(*efa_domain->qp_table));
	if (!efa_domain->qp_table)
		return -FI_ENOMEM;

	return 0;
}

static int efa_domain_init_rdm(struct efa_domain *efa_domain, struct fi_info *info)
{
	struct fi_info *shm_info = NULL;
	int err;

	assert(EFA_INFO_TYPE_IS_RDM(info));

	efa_shm_info_create(info, &shm_info);
	if (shm_info && !efa_domain->fabric->shm_fabric) {
		err = fi_fabric(shm_info->fabric_attr,
				&efa_domain->fabric->shm_fabric,
				efa_domain->fabric->util_fabric.fabric_fid.fid.context);
		if (err) {
			EFA_WARN(FI_LOG_DOMAIN, 
				 "Failed to create shm_fabric: %s\n",
				 fi_strerror(-err));
			return err;
		}
	}

	if (efa_domain->fabric->shm_fabric) {
		err = fi_domain(efa_domain->fabric->shm_fabric, shm_info,
				&efa_domain->shm_domain, NULL);
		if (err)
			return err;
	}

	efa_domain->mtu_size = efa_domain->device->ibv_port_attr.max_msg_sz;
	efa_domain->addrlen = (info->src_addr) ? info->src_addrlen : info->dest_addrlen;
	efa_domain->rdm_cq_size = MAX(info->rx_attr->size + info->tx_attr->size,
				  efa_env.cq_size);
	efa_domain->num_read_msg_in_flight = 0;

	dlist_init(&efa_domain->ope_queued_list);
	dlist_init(&efa_domain->ope_longcts_send_list);
	dlist_init(&efa_domain->peer_backoff_list);
	dlist_init(&efa_domain->handshake_queued_peer_list);

	if (shm_info)
		fi_freeinfo(shm_info);

	return 0;
}

/* @brief Allocate a domain, open the device, and set it up based on the hints.
 *
 * This function creates a domain and uses the info struct to configure the
 * domain based on what capabilities are set. Fork support is checked here and
 * the MR cache is also set up here.
 *
 * @param fabric_fid fabric that the domain should be tied to
 * @param info info struct that was validated and returned by fi_getinfo
 * @param domain_fid pointer where newly domain fid should be stored
 * @param context void pointer stored with the domain fid
 * @return 0 on success, fi_errno on error
 */
int efa_domain_open(struct fid_fabric *fabric_fid, struct fi_info *info,
		    struct fid_domain **domain_fid, void *context)
{
	struct efa_domain *efa_domain;
	int ret = 0, err;
	bool use_lock;

	efa_domain = calloc(1, sizeof(struct efa_domain));
	if (!efa_domain)
		return -FI_ENOMEM;

	dlist_init(&efa_domain->list_entry);
	efa_domain->fabric = container_of(fabric_fid, struct efa_fabric,
					  util_fabric.fabric_fid);

	err = ofi_domain_init(fabric_fid, info, &efa_domain->util_domain,
			      context, OFI_LOCK_MUTEX);
	if (err) {
		ret = err;
		goto err_free;
	}

	efa_domain->ibv_mr_reg_ct = 0;
	efa_domain->ibv_mr_reg_sz = 0;

	efa_domain->ah_map = NULL;

	use_lock = ofi_thread_level(efa_domain->util_domain.threading) <= ofi_thread_level(FI_THREAD_COMPLETION);
	err = ofi_genlock_init(&efa_domain->srx_lock, use_lock ? OFI_LOCK_MUTEX : OFI_LOCK_NOOP);
	if (err) {
		EFA_WARN(FI_LOG_DOMAIN, "srx lock init failed! err: %d\n", err);
		ret = err;
		goto err_free;
	}

	efa_domain->util_domain.av_type = FI_AV_TABLE;
	efa_domain->util_domain.mr_map.mode |= FI_MR_VIRT_ADDR;
	/*
	 * FI_MR_PROV_KEY means provider will generate a key for MR,
	 * which EFA provider does by using key generated by EFA device.
	 *
	 * util_domain.mr_map.mode is same as info->mode, which has
	 * the bit FI_MR_PROV_KEY on. When the bit is on, util_domain.mr_map
	 * will generate a key for MR, which is not what we want
	 * (we want to use the key generated by device). Therefore unset
	 * the FI_MR_PROV_KEY bit of mr_map.
	 */
	efa_domain->util_domain.mr_map.mode &= ~FI_MR_PROV_KEY;

	if (!info->ep_attr || info->ep_attr->type == FI_EP_UNSPEC) {
		EFA_WARN(FI_LOG_DOMAIN, "ep type not specified when creating domain\n");
		return -FI_EINVAL;
	}

	efa_domain->mr_local = ofi_mr_local(info);
	if ((EFA_INFO_TYPE_IS_DGRAM(info) || EFA_INFO_TYPE_IS_DIRECT(info)) && !efa_domain->mr_local) {
		EFA_WARN(FI_LOG_EP_DATA, "EFA direct and dgram require FI_MR_LOCAL, but application does not support it\n");
		ret = -FI_ENODATA;
		goto err_free;
	}

	err = efa_domain_init_device_and_pd(efa_domain, info->domain_attr->name, info->ep_attr->type);
	if (err) {
		ret = err;
		goto err_free;
	}

	efa_domain->info = fi_dupinfo(EFA_EP_TYPE_IS_RDM(info) ? efa_domain->device->rdm_info : efa_domain->device->dgram_info);
	if (!efa_domain->info) {
		ret = -FI_ENOMEM;
		goto err_free;
	}

	*domain_fid = &efa_domain->util_domain.domain_fid;

	err = efa_domain_init_qp_table(efa_domain);
	if (err) {
		ret = err;
		EFA_WARN(FI_LOG_DOMAIN, "Failed to init qp table. err: %d\n", ret);
		goto err_free;
	}

	/*
	 * Open the MR cache if application did not set FI_MR_LOCAL
	 * and the cache is enabled
	 * 
	 * Explicit memory registrations from external application
	 * should never go in the MR cache
	 */
	if (!efa_domain->mr_local && efa_mr_cache_enable) {
		err = efa_mr_cache_open(&efa_domain->cache, efa_domain);
		if (err) {
			ret = err;
			goto err_free;
		}
		efa_domain->internal_buf_mr_regv = efa_mr_cache_regv;
	} else {
		efa_domain->internal_buf_mr_regv = fi_mr_regv;
	}
	efa_domain->util_domain.domain_fid.mr = &efa_domain_mr_ops;

	if (EFA_INFO_TYPE_IS_RDM(info)) {
		efa_domain->info_type = EFA_INFO_RDM;
	} else if (EFA_INFO_TYPE_IS_DIRECT(info)) {
		efa_domain->info_type = EFA_INFO_DIRECT;
	} else {
		assert(EFA_INFO_TYPE_IS_DGRAM(info));
		efa_domain->info_type = EFA_INFO_DGRAM;
	}

	efa_domain->util_domain.domain_fid.fid.ops = &efa_ops_domain_fid;
	if (efa_domain->info_type == EFA_INFO_RDM) {
		err = efa_domain_init_rdm(efa_domain, info);
		if (err) {
			EFA_WARN(FI_LOG_DOMAIN,
				 "efa_domain_init_rdm failed. err: %d\n",
				 -err);
			goto err_free;
		}
		efa_domain->util_domain.domain_fid.ops = &efa_domain_ops_rdm;
	} else {
		assert(efa_domain->info_type == EFA_INFO_DIRECT || efa_domain->info_type == EFA_INFO_DGRAM);
		efa_domain->util_domain.domain_fid.ops = &efa_domain_ops;
	}

#ifndef _WIN32
	err = efa_fork_support_install_fork_handler();
	if (err) {
		EFA_WARN(FI_LOG_CORE,
			 "Unable to install fork handler: %s\n",
			 strerror(-err));
		return err;
	}
#endif

	ofi_mutex_lock(&g_efa_domain_list_lock);
	dlist_insert_tail(&efa_domain->list_entry, &g_efa_domain_list);
	ofi_mutex_unlock(&g_efa_domain_list_lock);

	return 0;

err_free:
	assert(efa_domain);

	err = efa_domain_close(&efa_domain->util_domain.domain_fid.fid);
	if (err) {
		EFA_WARN(FI_LOG_DOMAIN, "When handling error (%d), domain resource was being released."
			 "During the release process, an addtional error (%d) was encoutered\n",
			 -ret, -err);
	}

	efa_domain = NULL;
	*domain_fid = NULL;
	return ret;
}

static int efa_domain_close(fid_t fid)
{
	struct efa_domain *efa_domain;
	struct efa_ah *ah_entry, *tmp;
	int ret;

	efa_domain = container_of(fid, struct efa_domain,
				  util_domain.domain_fid.fid);

	ofi_mutex_lock(&g_efa_domain_list_lock);
	dlist_remove(&efa_domain->list_entry);
	ofi_mutex_unlock(&g_efa_domain_list_lock);

	if (efa_domain->cache) {
		ofi_mr_cache_cleanup(efa_domain->cache);
		free(efa_domain->cache);
		efa_domain->cache = NULL;
	}

	/* Clean up ah_map if any entries remain */
	ofi_genlock_lock(&efa_domain->util_domain.lock);
	if (efa_domain->ah_map) {
		EFA_WARN(FI_LOG_DOMAIN, "AH map not empty during domain close! Cleaning up ...\n");
		HASH_ITER(hh, efa_domain->ah_map, ah_entry, tmp) {
			ret = ibv_destroy_ah(ah_entry->ibv_ah);
			if (ret)
				EFA_WARN(FI_LOG_DOMAIN, "ibv_destroy_ah failed during cleanup! err=%d\n", ret);
			HASH_DEL(efa_domain->ah_map, ah_entry);
			free(ah_entry);
		}
	}
	ofi_genlock_unlock(&efa_domain->util_domain.lock);

	if (efa_domain->ibv_pd) {
		ret = ibv_dealloc_pd(efa_domain->ibv_pd);
		if (ret)
			EFA_WARN(FI_LOG_DOMAIN, "Failed to dealloc ibv_pd: %d\n", ret);
		efa_domain->ibv_pd = NULL;
	}

	ret = ofi_domain_close(&efa_domain->util_domain);
	if (ret)
		return ret;

	if (efa_domain->shm_domain) {
		ret = fi_close(&efa_domain->shm_domain->fid);
		if (ret)
			return ret;
	}

	if (efa_domain->info)
		fi_freeinfo(efa_domain->info);

	ofi_genlock_destroy(&efa_domain->srx_lock);
	free(efa_domain->qp_table);
	free(efa_domain);
	return 0;
}

/**
 * @brief Query EFA specific Memory Region attributes
 *
 * @param mr ptr to fid_mr
 * @param mr_attr  ptr to fi_efa_mr_attr
 * @return int 0 on success, negative integer on failure
 */
#if HAVE_EFADV_QUERY_MR

static int
efa_domain_query_mr(struct fid_mr *mr_fid, struct fi_efa_mr_attr *mr_attr)
{
	struct efadv_mr_attr attr = {0};
	struct efa_mr *efa_mr;
	int ret;

	memset(mr_attr, 0, sizeof(*mr_attr));

	efa_mr = container_of(mr_fid, struct efa_mr, mr_fid);
	ret = efadv_query_mr(efa_mr->ibv_mr, &attr, sizeof(attr));
	if (ret) {
		EFA_WARN(FI_LOG_DOMAIN, "efadv_query_mr failed. err: %d\n", ret);
		return ret;
	}

	/* Translate the validity masks and bus_id from efadv_mr_attr to fi_efa_mr_attr */
	if (attr.ic_id_validity & EFADV_MR_ATTR_VALIDITY_RECV_IC_ID) {
		mr_attr->recv_ic_id = attr.recv_ic_id;
		mr_attr->ic_id_validity |= FI_EFA_MR_ATTR_RECV_IC_ID;
	}

	if (attr.ic_id_validity & EFADV_MR_ATTR_VALIDITY_RDMA_READ_IC_ID) {
		mr_attr->rdma_read_ic_id = attr.rdma_read_ic_id;
		mr_attr->ic_id_validity |= FI_EFA_MR_ATTR_RDMA_READ_IC_ID;
	}

	if (attr.ic_id_validity & EFADV_MR_ATTR_VALIDITY_RDMA_RECV_IC_ID) {
		mr_attr->rdma_recv_ic_id = attr.rdma_recv_ic_id;
		mr_attr->ic_id_validity |= FI_EFA_MR_ATTR_RDMA_RECV_IC_ID;
	}

	return FI_SUCCESS;
}

#else

static int
efa_domain_query_mr(struct fid_mr *mr, struct fi_efa_mr_attr *mr_attr)
{
	return -FI_ENOSYS;
}

#endif /* HAVE_EFADV_QUERY_MR */

/**
 * @brief Query address information for a given endpoint and address.
 *
 * @param[in]  ep_fid Endpoint fid
 * @param[in]  addr Destination address
 * @param[out] ahn Pointer to store the address handle number
 * @param[out] remote_qpn Pointer to store the remote QP number
 * @param[out] remote_qkey Pointer to store the remote qkey
 * @return 0 on success, negative integer on failure
 */
static int efa_domain_query_addr(struct fid_ep *ep_fid, fi_addr_t addr,
				 uint16_t *ahn, uint16_t *remote_qpn,
				 uint32_t *remote_qkey)
{
	struct efa_base_ep *base_ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);
	struct efa_conn *conn = efa_av_addr_to_conn(base_ep->av, addr);
	if (!conn || !conn->ah || !conn->ep_addr) {
		EFA_WARN(FI_LOG_EP_CTRL, "Failed to find connection for addr %lu\n", addr);
		return -FI_EINVAL;
	}
	*ahn = conn->ah->ahn;
	*remote_qpn = conn->ep_addr->qpn;
	*remote_qkey = conn->ep_addr->qkey;

	return FI_SUCCESS;
}

#if HAVE_EFADV_QUERY_QP_WQS
/**
 * @brief Query EFA specific Queue Pair work queue attributes
 *
 * @param ep_fid  pointer to endpoint fid
 * @param sq_attr pointer to send queue attributes
 * @param rq_attr pointer to receive queue attributes
 * @return 0 on success, negative integer on failure
 */
static int efa_domain_query_qp_wqs(struct fid_ep *ep_fid,
				   struct fi_efa_wq_attr *sq_attr,
				   struct fi_efa_wq_attr *rq_attr)
{
	struct efa_base_ep *base_ep;
	struct efadv_wq_attr qp_sq_attr = {0};
	struct efadv_wq_attr qp_rq_attr = {0};
	int ret;

	memset(sq_attr, 0, sizeof(*sq_attr));
	memset(rq_attr, 0, sizeof(*rq_attr));

	base_ep = container_of(ep_fid, struct efa_base_ep, util_ep.ep_fid);
	ret = efadv_query_qp_wqs(base_ep->qp->ibv_qp, &qp_sq_attr, &qp_rq_attr, sizeof(qp_sq_attr));
	if (ret) {
		EFA_WARN(FI_LOG_DOMAIN, "efadv_query_qp_wqs failed. err: %d\n", ret);
		return (ret == EOPNOTSUPP) ? -FI_EOPNOTSUPP : -FI_EINVAL;
	}
	if (OFI_UNLIKELY(qp_sq_attr.comp_mask)) {
		EFA_WARN(FI_LOG_DOMAIN,
			 "efadv_query_qp_wqs returned invalid sq comp_mask value: %lu\n",
			 qp_sq_attr.comp_mask);
		return -FI_EINVAL;
	}
	if (OFI_UNLIKELY(qp_rq_attr.comp_mask)) {
		EFA_WARN(FI_LOG_DOMAIN,
			 "efadv_query_qp_wqs returned invalid rq comp_mask value: %lu\n",
			 qp_rq_attr.comp_mask);
		return -FI_EINVAL;
	}

	sq_attr->buffer = qp_sq_attr.buffer;
	sq_attr->entry_size = qp_sq_attr.entry_size;
	sq_attr->num_entries = qp_sq_attr.num_entries;
	sq_attr->doorbell = qp_sq_attr.doorbell;
	sq_attr->max_batch = qp_sq_attr.max_batch;

	rq_attr->buffer = qp_rq_attr.buffer;
	rq_attr->entry_size = qp_rq_attr.entry_size;
	rq_attr->num_entries = qp_rq_attr.num_entries;
	rq_attr->doorbell = qp_rq_attr.doorbell;
	rq_attr->max_batch = qp_rq_attr.max_batch;

	return FI_SUCCESS;
}
#else
static int efa_domain_query_qp_wqs(struct fid_ep *ep_fid,
				   struct fi_efa_wq_attr *sq_attr,
				   struct fi_efa_wq_attr *rq_attr)
{
	return -FI_ENOSYS;
}
#endif /* HAVE_EFADV_QUERY_QP_WQS */


#if HAVE_EFADV_QUERY_CQ
/**
 * @brief Query EFA specific Completion Queue attributes
 *
 * @param cq_fid pointer to completion queue fid
 * @param cq_attr pointer to fi_efa_cq_attr
 * @return 0 on success, negative integer on failure
 */
static int efa_domain_query_cq(struct fid_cq *cq_fid, struct fi_efa_cq_attr *cq_attr)
{
	struct efa_cq *efa_cq = container_of(cq_fid, struct efa_cq, util_cq.cq_fid);
	struct efadv_cq_attr attr = {0};
	int ret;

	memset(cq_attr, 0, sizeof(*cq_attr));

	ret = efadv_query_cq(ibv_cq_ex_to_cq(efa_cq->ibv_cq.ibv_cq_ex), &attr, sizeof(attr));
	if (ret) {
		EFA_WARN(FI_LOG_DOMAIN, "efadv_query_cq failed. err: %d\n", ret);
		return (ret == EOPNOTSUPP) ? -FI_EOPNOTSUPP : -FI_EINVAL;
	}
	if (OFI_UNLIKELY(attr.comp_mask)) {
		EFA_WARN(FI_LOG_DOMAIN,
			 "efadv_query_cq returned invalid comp_mask value: "
			 "%lu\n", attr.comp_mask);
		return -FI_EINVAL;
	}

	cq_attr->buffer = attr.buffer;
	cq_attr->entry_size = attr.entry_size;
	cq_attr->num_entries = attr.num_entries;

	return FI_SUCCESS;
}
#else
static int efa_domain_query_cq(struct fid_cq *cq_fid, struct fi_efa_cq_attr *cq_attr)
{
	return -FI_ENOSYS;
}
#endif /* HAVE_EFADV_QUERY_CQ */


#if HAVE_CAPS_CQ_WITH_EXT_MEM_DMABUF && HAVE_EFADV_CQ_EX
static struct fi_ops_cq efa_cq_ext_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = fi_no_cq_read,
	.readfrom = fi_no_cq_readfrom,
	.readerr = fi_no_cq_readerr,
	.sread = fi_no_cq_sread,
	.sreadfrom = fi_no_cq_sreadfrom,
	.signal = fi_no_cq_signal,
	.strerror = efa_cq_strerror,
};

static void efa_cq_ext_progress_no_op(struct util_cq *cq)
{
	return;
}

// Avoid flushing cq when it is created with external memory
static int efa_cq_ext_no_poll_ibv_cq(ssize_t cqe_to_process, struct efa_ibv_cq *ibv_cq)
{
	return ENOENT;
}

/**
 * @brief Create a completion queue with external memory provided via dmabuf.
 *
 * @param domain_fid Open resource domain
 * @param attr Completion queue attributes
 * @param efa_cq_init_attr Structure containing attributes for creating cq on external memory
 * @param cq_fid pointer to the created completion queue fid
 * @param context User specified context associated with the completion queue.
 * @return 0 on success, negative integer on failure
 */
static int efa_domain_cq_open_ext(struct fid_domain *domain_fid,
				  struct fi_cq_attr *attr,
				  struct fi_efa_cq_init_attr *efa_cq_init_attr,
				  struct fid_cq **cq_fid, void *context)
{
	struct efa_cq *cq;
	struct efa_domain *efa_domain;
	int err, retv;

	/* GPU cannot do a blocking wait on CQ entries because 
	 * system FDs are only accessible to CPU. */
	if (attr->wait_obj != FI_WAIT_NONE)
		return -FI_ENOSYS;

	if (!(efa_cq_init_attr->flags & FI_EFA_CQ_INIT_FLAGS_EXT_MEM_DMABUF)) {
		EFA_WARN(FI_LOG_DOMAIN, "FI_EFA_CQ_INIT_FLAGS_EXT_MEM_DMABUF flag is not set\n");
		return -FI_EINVAL;
	}

	if (!efa_cq_init_attr->ext_mem_dmabuf.length) {
		EFA_WARN(FI_LOG_DOMAIN, "struct ext_mem_dmabuf is invalid\n");
		return -FI_EINVAL;
	}

	if (!efa_device_support_cq_with_ext_mem_dmabuf()) {
		EFA_WARN(FI_LOG_DOMAIN, "External memory CQ requested but not supported by device\n");
		return -FI_EOPNOTSUPP;
	}

	cq = calloc(1, sizeof(*cq));
	if (!cq)
		return -FI_ENOMEM;

	/* 
	 * CQ polling is safe when CPU virtual address is provided in buffer.
	 * Otherwise, the memory is on GPU and the use of CQ poll interfaces should be avoided.
	 */
	cq->poll_ibv_cq = efa_cq_init_attr->ext_mem_dmabuf.buffer ? efa_cq_poll_ibv_cq: efa_cq_ext_no_poll_ibv_cq;
	err = ofi_cq_init(&efa_prov, domain_fid, attr, &cq->util_cq,
			  efa_cq_init_attr->ext_mem_dmabuf.buffer ?
				  &efa_cq_progress :
				  &efa_cq_ext_progress_no_op,
			  context);
	if (err) {
		EFA_WARN(FI_LOG_CQ, "Unable to create UTIL_CQ\n");
		goto err_free_cq;
	}

	efa_domain = container_of(cq->util_cq.domain, struct efa_domain,
				  util_domain);
	err = efa_cq_open_ibv_cq(attr, efa_domain->device->ibv_ctx,
				    &cq->ibv_cq,
				    efa_cq_init_attr);
	if (err) {
		EFA_WARN(FI_LOG_CQ, "Unable to create extended CQ with external memory: %s\n", fi_strerror(err));
		goto err_free_util_cq;
	}

	ofi_atomic_initialize32(&cq->nevents, 0);

	*cq_fid = &cq->util_cq.cq_fid;
	(*cq_fid)->fid.fclass = FI_CLASS_CQ;
	(*cq_fid)->fid.context = context;
	(*cq_fid)->fid.ops = &efa_cq_fi_ops;
	(*cq_fid)->ops = efa_cq_init_attr->ext_mem_dmabuf.buffer ? &efa_cq_ops : &efa_cq_ext_ops;

	return 0;

err_free_util_cq:
	retv = ofi_cq_cleanup(&cq->util_cq);
	if (retv)
		EFA_WARN(FI_LOG_CQ, "Unable to close util cq: %s\n",
			 fi_strerror(-retv));
err_free_cq:
	free(cq);
	return err;
}
#else
static int efa_domain_cq_open_ext(struct fid_domain *domain_fid,
				  struct fi_cq_attr *attr,
				  struct fi_efa_cq_init_attr *efa_cq_init_attr,
				  struct fid_cq **cq_fid, void *context)
{
	return -FI_ENOSYS;
}
#endif

static uint64_t efa_domain_get_mr_lkey(struct fid_mr *mr)
{
	struct efa_mr *efa_mr;

	efa_mr = container_of(mr, struct efa_mr, mr_fid);
	if (!efa_mr->ibv_mr) {
		EFA_WARN(FI_LOG_DOMAIN, "MR not registered\n");
		return FI_KEY_NOTAVAIL;
	}

	return efa_mr->ibv_mr->lkey;
}

static struct fi_efa_ops_domain efa_ops_domain = {
	.query_mr = efa_domain_query_mr,
};

static struct fi_efa_ops_gda efa_ops_gda = {
	.query_addr = efa_domain_query_addr,
	.query_qp_wqs = efa_domain_query_qp_wqs,
	.query_cq = efa_domain_query_cq,
	.cq_open_ext = efa_domain_cq_open_ext,
	.get_mr_lkey = efa_domain_get_mr_lkey,
};

static int
efa_domain_ops_open(struct fid *fid, const char *ops_name, uint64_t flags,
		     void **ops, void *context)
{
	int ret = FI_SUCCESS;
	struct efa_domain *efa_domain;

	if (strcmp(ops_name, FI_EFA_DOMAIN_OPS) == 0) {
		*ops = &efa_ops_domain;
		return ret;
	}
	if (strcmp(ops_name, FI_EFA_GDA_OPS) == 0) {
		efa_domain = container_of(fid, struct efa_domain, util_domain.domain_fid.fid);
		if (efa_domain->info_type != EFA_INFO_DIRECT) {
			EFA_WARN(FI_LOG_DOMAIN, "Only efa direct supports FI_EFA_GDA_OPS\n");
			return -FI_EOPNOTSUPP;
		}

		*ops = &efa_ops_gda;
		return ret;
	}

	EFA_WARN(FI_LOG_DOMAIN, "Unknown ops name: %s\n", ops_name);
	ret = -FI_EINVAL;

	return ret;
}

void efa_domain_progress_rdm_peers_and_queues(struct efa_domain *domain)
{
	struct efa_rdm_peer *peer;
	struct dlist_entry *tmp;
	struct efa_rdm_ope *ope;
	int ret;

	assert(domain->info->ep_attr->type == FI_EP_RDM);

	/* Update timers for peers that are in backoff list*/
	dlist_foreach_container_safe(&domain->peer_backoff_list, struct efa_rdm_peer,
				     peer, rnr_backoff_entry, tmp) {
		if (ofi_gettime_us() >= peer->rnr_backoff_begin_ts +
					peer->rnr_backoff_wait_time) {
			peer->flags &= ~EFA_RDM_PEER_IN_BACKOFF;
			dlist_remove(&peer->rnr_backoff_entry);
		}
	}

	/*
	 * Resend handshake packet for any peers where the first
	 * handshake send failed.
	 */
	dlist_foreach_container_safe(&domain->handshake_queued_peer_list,
				     struct efa_rdm_peer, peer,
				     handshake_queued_entry, tmp) {
		if (peer->flags & EFA_RDM_PEER_IN_BACKOFF)
			continue;

		ret = efa_rdm_ep_post_handshake(peer->ep, peer);
		if (ret == -FI_EAGAIN)
			continue;

		if (OFI_UNLIKELY(ret)) {
			EFA_WARN(FI_LOG_EP_CTRL,
				 "Failed to post HANDSHAKE to peer fi_addr: "
				 "%ld implicit fi_addr: %ld. %s\n",
				 peer->conn->fi_addr,
				 peer->conn->implicit_fi_addr,
				 fi_strerror(-ret));
			efa_base_ep_write_eq_error(&peer->ep->base_ep, -ret, FI_EFA_ERR_PEER_HANDSHAKE);
			continue;
		}

		dlist_remove(&peer->handshake_queued_entry);
		peer->flags &= ~EFA_RDM_PEER_HANDSHAKE_QUEUED;
		peer->flags |= EFA_RDM_PEER_HANDSHAKE_SENT;
	}

	/*
	 * Repost pkts for all queued op entries
	 */
	dlist_foreach_container_safe(&domain->ope_queued_list,
				     struct efa_rdm_ope,
				     ope, queued_entry, tmp) {

		peer = ope->peer;
		if (peer && (peer->flags & EFA_RDM_PEER_IN_BACKOFF))
			continue;

		if (efa_rdm_ope_process_queued_ope(ope, EFA_RDM_OPE_QUEUED_BEFORE_HANDSHAKE))
			continue;
		if (efa_rdm_ope_process_queued_ope(ope, EFA_RDM_OPE_QUEUED_RNR))
			continue;
		if (efa_rdm_ope_process_queued_ope(ope, EFA_RDM_OPE_QUEUED_CTRL))
			continue;
		if (efa_rdm_ope_process_queued_ope(ope, EFA_RDM_OPE_QUEUED_READ))
			continue;
	}
	/*
	 * Send data packets until window or data queue is exhausted.
	 */
	dlist_foreach_container(&domain->ope_longcts_send_list, struct efa_rdm_ope,
				ope, entry) {
		peer = ope->peer;
		assert(peer);
		if (peer->flags & EFA_RDM_PEER_IN_BACKOFF)
			continue;

		/*
		 * Do not send DATA packet until we received HANDSHAKE packet from the peer,
		 * this is because endpoint does not know whether peer need connid in header
		 * until it get the HANDSHAKE packet.
		 *
		 * We only do this for DATA packet because other types of packets always
		 * has connid in there packet header. If peer does not make use of the connid,
		 * the connid can be safely ignored.
		 *
		 * DATA packet is different because for DATA packet connid is an optional
		 * header inserted between the mandatory header and the application data.
		 * Therefore if peer does not use/understand connid, it will take connid
		 * as application data thus cause data corruption.
		 *
		 * This will not cause deadlock because peer will send a HANDSHAKE packet
		 * back upon receiving 1st packet from the endpoint, and in all 3 sub0protocols
		 * (long-CTS message, emulated long-CTS write and emulated long-CTS read)
		 * where DATA packet is used, endpoint will send other types of packet to
		 * peer before sending DATA packets. The workflow of the 3 sub-protocol
		 * can be found in protocol v4 document chapter 3.
		 */
		if (!(peer->flags & EFA_RDM_PEER_HANDSHAKE_RECEIVED))
			continue;

		if (ope->window > 0) {
			ret = efa_rdm_ope_post_send(ope, EFA_RDM_CTSDATA_PKT);
			if (OFI_UNLIKELY(ret)) {
				if (ret == -FI_EAGAIN)
					continue;

				efa_rdm_txe_handle_error(ope, -ret, FI_EFA_ERR_PKT_POST);
				continue;
			}
		}
	}
}
