/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "config.h"
#include <ofi_util.h>
#include "efa.h"
#include "rdm/efa_rdm_ep.h"
#include "rdm/efa_rdm_ope.h"
#if HAVE_CUDA
#include <cuda.h>
#endif

static int efa_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
			  uint64_t flags, struct fid_mr **mr_fid);
static int efa_mr_reg_impl(struct efa_mr *efa_mr, uint64_t flags, const struct fi_mr_attr *mr_attr);
static int efa_mr_dereg_impl(struct efa_mr *efa_mr);


/*
 * Disable MR cache by default under ASAN. The memhooks monitor is
 * incompatible with ASAN, and userfaultfd may not be available on
 * all systems. Without a working monitor, efa_mr_cache_open fails.
 * Can be overridden at runtime with FI_EFA_MR_CACHE_ENABLE=1.
 */
#ifdef ENABLE_ASAN
int efa_mr_cache_enable	= 0;
#else
int efa_mr_cache_enable	= 1;
#endif
size_t efa_mr_max_cached_count;
size_t efa_mr_max_cached_size;

#define EFA_MR_ATTR_INIT_SYSTEM(iov, count, access, offset, requested_key, context) \
	{ \
		.mr_iov = (iov), \
		.iov_count = (count), \
		.access = (access), \
		.offset = (offset), \
		.requested_key = (requested_key), \
		.context = (context), \
		.iface = FI_HMEM_SYSTEM, \
	}

/*
 * Initial values for internal keygen functions to generate MR keys
 * (efa_mr->mr_fid.key)
 *
 * Typically the rkey returned from ibv_reg_mr() (ibv_mr->rkey) would be used.
 * In cases where ibv_reg_mr() should be avoided, we use proprietary MR key
 * generation instead.
 *
 * Initial values should be > UINT32_MAX to avoid collisions with ibv_mr rkeys,
 * and should be sufficiently spaced apart s.t. they don't collide with each
 * other.
 */
#define NON_P2P_MR_KEYGEN_INIT	BIT_ULL(32)

/* @brief Setup the MR cache.
 *
 * This function enables the MR cache using the util MR cache code.
 *
 * @param cache		The ofi_mr_cache that is to be set up.
 * @param domain	The EFA domain where cache will be used.
 * @return 0 on success, fi_errno on failure.
 */
int efa_mr_cache_open(struct ofi_mr_cache **cache, struct efa_domain *domain)
{
	struct ofi_mem_monitor *memory_monitors[OFI_HMEM_MAX] = {
		[FI_HMEM_SYSTEM] = default_monitor,
		[FI_HMEM_CUDA] = cuda_monitor,
		[FI_HMEM_ROCR] = rocr_monitor,
	};
	int err;

	/* Both Open MPI (and possibly other MPI implementations) and
	 * Libfabric use the same live binary patching to enable memory
	 * monitoring, but the patching technique only allows a single
	 * "winning" patch.  The Libfabric memhooks monitor will not
	 * overwrite a previous patch, but instead return
	 * -FI_EALREADY.  There are three cases of concern, and in all
	 * but one of them, we can avoid changing the default monitor.
	 *
	 * (1) Upper layer does not patch, such as Open MPI 4.0 and
	 * earlier.  In this case, the default monitor will be used,
	 * as the default monitor is either not the memhooks monitor
	 * (because the user specified a different monitor) or the
	 * default monitor is the memhooks monitor, but we were able
	 * to install the patches.  We will use the default monitor in
	 * this case.
	 *
	 * (2) Upper layer does patch, but does not export a memory
	 * monitor, such as Open MPI 4.1.0 and 4.1.1.  In this case,
	 * if the default memory monitor is not memhooks, we will use
	 * the default monitor.  If the default monitor is memhooks,
	 * the patch will fail to apply, and we will change the
	 * requested monitor to UFFD to avoid a broken configuration.
	 * If the user explicitly requested memhooks, we will return
	 * an error, as we can not satisfy that request.
	 *
	 * (3) Upper layer does patch and exports a memory monitor,
	 * such as Open MPI 4.1.2 and later.  In this case, the
	 * default monitor will have been changed from the memhooks
	 * monitor to the imported monitor, so we will use the
	 * imported monitor.
	 *
	 * The only known cases in which we will not use the default
	 * monitor are Open MPI 4.1.0/4.1.1.
	 *
	 * It is possible that this could be better handled at the
	 * mem_monitor level in Libfabric, but so far we have not
	 * reached agreement on how that would work.
	 */
	if (default_monitor == memhooks_monitor) {
		err = memhooks_monitor->start(memhooks_monitor);
		if (err == -FI_EALREADY) {
			if (cache_params.monitor) {
				EFA_WARN(FI_LOG_DOMAIN,
					 "Memhooks monitor requested via FI_MR_CACHE_MONITOR, but memhooks failed to\n"
					 "install.  No working monitor availale.\n");
				return -FI_ENOSYS;
			}
			EFA_INFO(FI_LOG_DOMAIN,
				 "Detected potential memhooks monitor conflict. Switching to UFFD.\n");
			memory_monitors[FI_HMEM_SYSTEM] = uffd_monitor;
		}
	} else if (default_monitor == NULL) {
		/* TODO: Fail if we don't find a system monitor.  This
		 * is a debatable decision, as the VERBS provider
		 * falls back to a no-cache mode in this case.  We
		 * fail the domain creation because the rest of the MR
		 * code hasn't been audited to deal with a NULL
		 * monitor.
		 */
		EFA_WARN(FI_LOG_DOMAIN,
			 "No default SYSTEM monitor available.\n");
		return -FI_ENOSYS;
	}

	*cache = (struct ofi_mr_cache *)calloc(1, sizeof(struct ofi_mr_cache));
	if (!*cache)
		return -FI_ENOMEM;

	if (!efa_mr_max_cached_count)
		efa_mr_max_cached_count = domain->device->ibv_attr.max_mr *
					  EFA_MR_CACHE_LIMIT_MULT;
	if (!efa_mr_max_cached_size)
		efa_mr_max_cached_size = domain->device->ibv_attr.max_mr_size *
					 EFA_MR_CACHE_LIMIT_MULT;
	/*
	 * XXX: we're modifying a global in the util mr cache? do we need an
	 * API here instead?
	 */
	cache_params.max_cnt = efa_mr_max_cached_count;
	cache_params.max_size = efa_mr_max_cached_size;
	(*cache)->entry_data_size = sizeof(struct efa_mr);
	(*cache)->add_region = efa_mr_cache_entry_reg;
	(*cache)->delete_region = efa_mr_cache_entry_dereg;
	err = ofi_mr_cache_init(&domain->util_domain, memory_monitors,
				*cache);
	if (err) {
		EFA_WARN(FI_LOG_DOMAIN, "EFA MR cache init failed: %s\n",
		         fi_strerror(err));
		free(*cache);
		*cache = NULL;
		return -err;
	}

	EFA_INFO(FI_LOG_DOMAIN, "EFA MR cache enabled, max_cnt: %zu max_size: %zu\n",
		 cache_params.max_cnt, cache_params.max_size);
	return 0;
}

static int efa_mr_cache_close(fid_t fid)
{
	struct efa_mr *efa_mr = container_of(fid, struct efa_mr,
					       mr_fid.fid);

	ofi_mr_cache_delete(efa_mr->domain->cache, efa_mr->entry);

	return 0;
}

static struct fi_ops efa_mr_cache_ops = {
	.size = sizeof(struct fi_ops),
	.close = efa_mr_cache_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

/**
 * @brief Validate HMEM attributes and populate efa_mr struct
 *
 * Check if FI_HMEM is enabled for the domain, validate whether the specific
 * device type requested is currently supported by the provider, and update the
 * efa_mr structure based on the attributes requested by the user.
 *
 * @param[in]	efa_mr	efa_mr structure to be updated
 * @param[in]	attr	a copy of fi_mr_attr updated from the user's registration call
 * @param[in]	flags   MR flags
 *
 * @return FI_SUCCESS or negative FI error code
 */
static int efa_mr_hmem_setup(struct efa_mr *efa_mr,
                             const struct fi_mr_attr *attr,
							 uint64_t flags)
{
	int err;
	struct iovec mr_iov = {0};
	efa_mr->peer.flags = flags;

	if (attr->iface == FI_HMEM_SYSTEM) {
		efa_mr->peer.iface = FI_HMEM_SYSTEM;
		return FI_SUCCESS;
	}

	if (efa_mr->domain->util_domain.info_domain_caps & FI_HMEM) {
		if (g_efa_hmem_info[attr->iface].initialized) {
			efa_mr->peer.iface = attr->iface;
		} else {
			EFA_WARN(FI_LOG_MR,
				"%s is not initialized\n",
				fi_tostr(&attr->iface, FI_TYPE_HMEM_IFACE));
			return -FI_ENOSYS;
		}
	} else {
		/*
		 * It's possible that attr->iface is not initialized when
		 * FI_HMEM is off, so this can't be a fatal error. Print a
		 * warning in case this value is not FI_HMEM_SYSTEM for
		 * whatever reason.
		 */
		EFA_WARN_ONCE(FI_LOG_MR,
		             "FI_HMEM support is disabled, assuming FI_HMEM_SYSTEM instead of %s\n",
		             fi_tostr(&attr->iface, FI_TYPE_HMEM_IFACE));
		efa_mr->peer.iface = FI_HMEM_SYSTEM;
	}

	efa_mr->peer.device = 0;
	efa_mr->peer.flags &= ~OFI_HMEM_DATA_DEV_REG_HANDLE;
	efa_mr->peer.hmem_data = NULL;
	if (efa_mr->peer.iface == FI_HMEM_CUDA) {
		efa_mr->needs_sync = true;
		efa_mr->peer.device = attr->device.cuda;

		/* Only attempt GDRCopy registrations for efa rdm path */
		if (efa_mr->domain->info_type == EFA_INFO_RDM && !(flags & FI_MR_DMABUF) && cuda_is_gdrcopy_enabled()) {
			mr_iov = *attr->mr_iov;
			err = ofi_hmem_dev_register(FI_HMEM_CUDA, mr_iov.iov_base, mr_iov.iov_len,
						    (uint64_t *)&efa_mr->peer.hmem_data);
			efa_mr->peer.flags |= OFI_HMEM_DATA_DEV_REG_HANDLE;
			if (err) {
				EFA_WARN(FI_LOG_MR,
				         "Unable to register handle for GPU memory. err: %d buf: %p len: %zu\n",
				         err, mr_iov.iov_base, mr_iov.iov_len);
				/* When gdrcopy pin buf failed, fallback to cudaMemcpy */
				efa_mr->peer.hmem_data = NULL;
				efa_mr->peer.flags &= ~OFI_HMEM_DATA_DEV_REG_HANDLE;
			}
		}
	} else if (attr->iface == FI_HMEM_ROCR) {
		efa_mr->peer.device = attr->device.rocr;
	} else if (attr->iface == FI_HMEM_NEURON) {
		efa_mr->peer.device = attr->device.neuron;
	} else if (attr->iface == FI_HMEM_SYNAPSEAI) {
		efa_mr->peer.device = attr->device.synapseai;
	}

	return FI_SUCCESS;
}


int efa_mr_cache_entry_reg(struct ofi_mr_cache *cache,
			   struct ofi_mr_entry *entry)
{
	int ret = 0;
	/* TODO
	 * Since, access is not passed as a parameter to efa_mr_cache_entry_reg,
	 * for now we will set access to all supported access modes. Once access
	 * information is available this can be removed.
	 * Issue: https://github.com/ofiwg/libfabric/issues/5677
	 */
	uint64_t access = EFA_MR_SUPPORTED_PERMISSIONS;
	struct fi_mr_attr attr = {0};
	struct efa_mr *efa_mr = (struct efa_mr *)entry->data;

	efa_mr->domain = container_of(cache->domain, struct efa_domain,
					util_domain);
	efa_mr->mr_fid.fid.ops = &efa_mr_cache_ops;
	efa_mr->mr_fid.fid.fclass = FI_CLASS_MR;
	efa_mr->mr_fid.fid.context = NULL;

	attr.mr_iov = &entry->info.iov;
	/* ofi_mr_info only stores one iov */
	attr.iov_count = 1;
	attr.access = access;
	attr.offset = 0;
	attr.requested_key = 0;
	attr.context = NULL;
	attr.iface = entry->info.iface;

	if (attr.iface == FI_HMEM_CUDA)
		attr.device.cuda = entry->info.device;
	else if (attr.iface == FI_HMEM_NEURON)
		attr.device.neuron = entry->info.device;
	else if (attr.iface == FI_HMEM_SYNAPSEAI)
		attr.device.synapseai = entry->info.device;

	ret = efa_mr_reg_impl(efa_mr, 0, &attr);
	return ret;
}

void efa_mr_cache_entry_dereg(struct ofi_mr_cache *cache,
			      struct ofi_mr_entry *entry)
{
	struct efa_mr *efa_mr = (struct efa_mr *)entry->data;
	int ret;

	ret = efa_mr_dereg_impl(efa_mr);
	if (ret)
		EFA_WARN(FI_LOG_MR, "Unable to dereg mr: %d\n", ret);
}

static int efa_mr_cache_regattr(struct fid *fid, const struct fi_mr_attr *attr,
				uint64_t flags, struct fid_mr **mr_fid)
{
	struct efa_domain *domain;
	struct efa_mr *efa_mr;
	struct ofi_mr_entry *entry;
	struct ofi_mr_info info = {0};
	int ret;

	if (attr->iov_count > EFA_MR_IOV_LIMIT) {
		EFA_WARN(FI_LOG_MR, "iov count > %d not supported\n",
			 EFA_MR_IOV_LIMIT);
		return -FI_EINVAL;
	}

	if (!ofi_hmem_is_initialized(attr->iface)) {
		EFA_WARN(FI_LOG_MR,
			"Cannot register memory for uninitialized iface (%s)\n",
			fi_tostr(&attr->iface, FI_TYPE_HMEM_IFACE));
		return -FI_ENOSYS;
	}

	domain = container_of(fid, struct efa_domain,
			      util_domain.domain_fid.fid);

	assert(attr->iov_count > 0 && attr->iov_count <= domain->info->domain_attr->mr_iov_limit);
	ofi_mr_info_get_iov_from_mr_attr(&info, attr, flags);
	info.iface = attr->iface;
	info.device = attr->device.reserved;
	ret = ofi_mr_cache_search(domain->cache, &info, &entry);
	if (OFI_UNLIKELY(ret))
		return ret;

	efa_mr = (struct efa_mr *)entry->data;
	efa_mr->entry = entry;

	*mr_fid = &efa_mr->mr_fid;
	return 0;
}

/**
 * @brief Register a memory region and add it to the MR cache
 *
 * This function is only used internally by the EFA provider to register
 * a memory region containing application memory and add it to the MR cache
 * It is only used when FI_MR_LOCAL is not set and the MR cache is enabled
 *
 * @param[in]	fid		domain fid
 * @param[in]	iov		iovec to be registered
 * @param[in]	access		access flags for MR
 * @param[in]	flags		MR flags
 * @param[out]	mr_fid		MR descriptor fids
 *
 * @return FI_SUCCESS or negative FI error code
 */
int efa_mr_cache_regv(struct fid_domain *domain_fid, const struct iovec *iov,
		      size_t count, uint64_t access, uint64_t offset,
		      uint64_t requested_key, uint64_t flags,
		      struct fid_mr **mr, void *context)
{
	struct fi_mr_attr attr = EFA_MR_ATTR_INIT_SYSTEM(iov, count, access, offset, requested_key, context);

	return efa_mr_cache_regattr(&domain_fid->fid, &attr, flags, mr);
}

static int efa_mr_dereg_impl(struct efa_mr *efa_mr)
{
	struct efa_domain *efa_domain;
	int ret = 0;
	int err;
	size_t ibv_mr_size;
	int64_t reg_ct, reg_sz;

	efa_domain = efa_mr->domain;
	if (efa_mr->ibv_mr) {
		ibv_mr_size = efa_mr->ibv_mr->length;
		err = -ibv_dereg_mr(efa_mr->ibv_mr);
		if (err) {
			EFA_WARN(FI_LOG_MR,
				"Unable to deregister memory registration\n");
			ret = err;
		} else {
			reg_ct = ofi_atomic_dec64(&efa_mr->domain->ibv_mr_reg_ct);
			reg_sz = ofi_atomic_sub64(&efa_mr->domain->ibv_mr_reg_sz, ibv_mr_size);
			EFA_INFO(FI_LOG_MR, "Deregistered memory of size %zu for ibv pd %p, total mr reg size %zd, mr reg count %zd\n",
				 ibv_mr_size, efa_mr->domain->ibv_pd, reg_sz, reg_ct);
		}
	}

	efa_mr->ibv_mr = NULL;

	if (efa_mr->inserted_to_mr_map) {
		ofi_genlock_lock(&efa_domain->util_domain.lock);
		err = ofi_mr_map_remove(&efa_domain->util_domain.mr_map,
					efa_mr->mr_fid.key);
		ofi_genlock_unlock(&efa_domain->util_domain.lock);

		if (err) {
			EFA_WARN(FI_LOG_MR,
				"Unable to remove MR entry from util map (%s)\n",
				fi_strerror(-err));
			ret = err;
		}
		efa_mr->inserted_to_mr_map = false;
	}

	if (efa_mr->peer.iface == FI_HMEM_CUDA &&
	    (efa_mr->peer.flags & OFI_HMEM_DATA_DEV_REG_HANDLE)) {
		assert(efa_mr->peer.hmem_data);
		err = ofi_hmem_dev_unregister(FI_HMEM_CUDA, (uint64_t)efa_mr->peer.hmem_data);
		if (err) {
			EFA_WARN(FI_LOG_MR,
				"Unable to de-register cuda handle\n");
			ret = err;
		}

		efa_mr->peer.hmem_data = NULL;
	}

	efa_mr->mr_fid.mem_desc = NULL;
	efa_mr->mr_fid.key = FI_KEY_NOTAVAIL;
	return ret;
}

/**
 * @brief Warn about MR descriptor references in an in-flight operation
 *
 * This helper function iterates through an operation's descriptor array and
 * clears any references to the specified MR that is being closed. It logs a
 * warning when such references are found, as this indicates the MR is being
 * closed while operations are still in flight.
 *
 * @param[in,out] desc      The descriptor array
 * @param[in]     iov_count Number of IOVs in the operation
 * @param[in]     cq_entry  CQ entry with diagnostic info for the warning
 * @param[in]     efa_mr    The MR being closed
 * @param[in]     base_ep   The base ep
 */
static inline void efa_mr_close_warn_inflight_ope(void **desc, size_t iov_count,
					       struct fi_cq_tagged_entry *cq_entry,
					       struct efa_mr *efa_mr,
					       struct efa_base_ep *base_ep)
{
	size_t i;
	char ep_addr_str[OFI_ADDRSTRLEN] = {0};
	size_t ep_addr_strlen = sizeof(ep_addr_str);

	for (i = 0; i < iov_count; i++) {
		if (desc[i] == efa_mr) {
			efa_base_ep_raw_addr_str(base_ep, ep_addr_str, &ep_addr_strlen);
			EFA_WARN(FI_LOG_MR,
				 "MR %p (key: %ld) on EP %s is being closed "
				 "while an operation is still in flight: "
				 "context=%p, flags=%s, len=%zu, "
				 "buf=%p, data=%lx, tag=%lx.\n",
				 efa_mr, efa_mr->mr_fid.key,
				 ep_addr_str,
				 cq_entry->op_context,
				 fi_tostr(&cq_entry->flags, FI_TYPE_CAPS),
				 cq_entry->len, cq_entry->buf,
				 cq_entry->data, cq_entry->tag);
			desc[i] = NULL;
		}
	}
}

/**
 * @brief Check all in-flight operations for references to a closing MR
 *
 * Iterates across all endpoints in the domain and warns about any
 * in-flight operations that still reference the MR being closed.
 *
 * @param[in] efa_mr	The MR being closed
 */
static void efa_mr_close_check_inflight_ope(struct efa_mr *efa_mr)
{
	struct efa_domain *efa_domain = efa_mr->domain;
	struct efa_base_ep *base_ep;
	struct dlist_entry *tmp;

	ofi_genlock_lock(&efa_domain->util_domain.lock);
	dlist_foreach_container(&efa_domain->base_ep_list, struct efa_base_ep,
				base_ep, base_ep_entry) {
		if (efa_domain->info_type == EFA_INFO_RDM) {
			struct efa_rdm_ep *rdm_ep;
			struct efa_rdm_ope *ope;
			rdm_ep = container_of(base_ep, struct efa_rdm_ep, base_ep);
			dlist_foreach_container_safe (
				&rdm_ep->txe_list, struct efa_rdm_ope,
				ope, ep_entry, tmp) {
				efa_mr_close_warn_inflight_ope(ope->desc, ope->iov_count, &ope->cq_entry, efa_mr, base_ep);
			}
			dlist_foreach_container_safe (
				&rdm_ep->rxe_list, struct efa_rdm_ope,
				ope, ep_entry, tmp) {
				efa_mr_close_warn_inflight_ope(ope->desc, ope->iov_count, &ope->cq_entry, efa_mr, base_ep);
			}
		} else if (efa_domain->info_type == EFA_INFO_DIRECT) {
			struct efa_direct_ope *direct_ope;
			dlist_foreach_container_safe (
				&base_ep->efa_direct_ope_list,
				struct efa_direct_ope, direct_ope,
				entry, tmp) {
				efa_mr_close_warn_inflight_ope(direct_ope->desc, direct_ope->iov_count, &direct_ope->cq_entry, efa_mr, base_ep);
			}
		}
	}
	ofi_genlock_unlock(&efa_domain->util_domain.lock);
}

static int efa_mr_close(fid_t fid)
{
	struct efa_mr *efa_mr;
	int ret, err;

	efa_mr = container_of(fid, struct efa_mr, mr_fid.fid);
	if (efa_env.track_mr)
		efa_mr_close_check_inflight_ope(efa_mr);

	if (efa_mr->shm_mr) {
		err = fi_close(&efa_mr->shm_mr->fid);
		if (err) {
			EFA_WARN(FI_LOG_MR,
				"Unable to close shm MR\n");
			ret = err;
		}
		efa_mr->shm_mr = NULL;
	}

	ret = efa_mr_dereg_impl(efa_mr);
	if (ret)
		EFA_WARN(FI_LOG_MR, "Unable to close MR\n");
	free(efa_mr);
	return ret;
}

struct fi_ops efa_mr_ops = {
	.size = sizeof(struct fi_ops),
	.close = efa_mr_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

#if HAVE_EFA_DMABUF_MR

static inline
struct ibv_mr *efa_mr_reg_ibv_dmabuf_mr(struct ibv_pd *pd, uint64_t offset,
					size_t len, uint64_t iova, int fd, int access)
{
	return ibv_reg_dmabuf_mr(pd, offset, len, iova, fd, access);
}

#else

static inline
struct ibv_mr *efa_mr_reg_ibv_dmabuf_mr(struct ibv_pd *pd, uint64_t offset,
					size_t len, uint64_t iova, int fd, int access)
{
	EFA_WARN(FI_LOG_MR,
		"ibv_reg_dmabuf_mr is required for memory"
		" registration with FI_MR_DMABUF flags, but "
		" not available in the current rdma-core library."
		" please build libfabric with rdma-core >= 34.0\n");
	return NULL;
}

#endif

static inline void efa_mark_dmabuf_fail(struct efa_hmem_info *i, enum fi_hmem_iface iface)
{
	if (DMABUF_IS_NOT_SUPPORTED(i)) {
		return;
	} else if (DMABUF_IS_ASSUMED(i)) {
		EFA_INFO(FI_LOG_MR,
			 "assumed dmabuf support for %s was incorrect\n",
			 fi_tostr(&iface, FI_TYPE_HMEM_IFACE));
	} else {
		EFA_WARN(FI_LOG_MR,
			 "Explicit dmabuf support for %s was incorrect\n",
			 fi_tostr(&iface, FI_TYPE_HMEM_IFACE));
	}
	i->dmabuf_supported_by_device = EFA_DMABUF_NOT_SUPPORTED;
}

/**
 * @brief Register a memory buffer with rdma-core api.
 *
 * @param efa_mr the ptr to the efa_mr object
 * @param mr_attr the ptr to the fi_mr_attr object
 * @param access the desired memory protection attributes
 * @param flags flags in fi_mr_reg/fi_mr_regattr
 * @return struct ibv_mr* the ptr to the registered MR
 */
static struct ibv_mr *efa_mr_reg_ibv_mr(struct efa_mr *efa_mr,
					 struct fi_mr_attr *mr_attr,
					 int access, const uint64_t flags)
{
	struct efa_hmem_info *p_info = &g_efa_hmem_info[efa_mr->peer.iface];
	struct ibv_mr *dmabuf_mr;
	uint64_t offset;
	int dmabuf_fd;
	int ret;

	/* Explicit dmabuf registration */
	if (flags & FI_MR_DMABUF) {
		if (!mr_attr->dmabuf) {
			EFA_WARN(FI_LOG_MR, "FI_MR_DMABUF set but mr_attr->dmabuf == NULL\n");
			return NULL;
		}

		EFA_INFO(FI_LOG_MR,
			 "FI_MR_DMABUF: fd=%d offset=%lu len=%zu\n",
			 mr_attr->dmabuf->fd, mr_attr->dmabuf->offset,
			 mr_attr->dmabuf->len);

		dmabuf_mr = efa_mr_reg_ibv_dmabuf_mr(
			efa_mr->domain->ibv_pd,
			mr_attr->dmabuf->offset,
			mr_attr->dmabuf->len,
			(uintptr_t) mr_attr->dmabuf->base_addr + mr_attr->dmabuf->offset,
			mr_attr->dmabuf->fd,
			access);

		if (!dmabuf_mr)
			EFA_WARN(FI_LOG_MR,
				 "Could not register dmabuf fd=%d offset=%lu len=%zu\n",
				 mr_attr->dmabuf->fd, mr_attr->dmabuf->offset,
				 mr_attr->dmabuf->len);

		return dmabuf_mr;
	}

	/* Implicit VA path with dmabuf-first */
	if (DMABUF_IS_SUPPORTED(p_info)) {
		ret = ofi_hmem_get_dmabuf_fd(
			efa_mr->peer.iface,
			mr_attr->mr_iov->iov_base,
			(uint64_t) mr_attr->mr_iov->iov_len,
			&dmabuf_fd, &offset);
		if (ret == FI_SUCCESS) {
			/* get fd succeeded */
			EFA_INFO(FI_LOG_MR,
				 "Registering dmabuf MR: fd=%d offset=%lu len=%zu\n",
				 dmabuf_fd, offset, mr_attr->mr_iov->iov_len);

			dmabuf_mr = efa_mr_reg_ibv_dmabuf_mr(
				efa_mr->domain->ibv_pd, offset,
				mr_attr->mr_iov->iov_len,
				(uint64_t)mr_attr->mr_iov->iov_base,
				dmabuf_fd, access);

			/* Close the dmabuf file descriptor - it's no longer needed
			 * after registration
			 */
			(void) ofi_hmem_put_dmabuf_fd(efa_mr->peer.iface, dmabuf_fd);

			if (!dmabuf_mr) {
				efa_mark_dmabuf_fail(p_info, efa_mr->peer.iface);
				if (p_info->dmabuf_fallback_enabled) {
					return ibv_reg_mr(efa_mr->domain->ibv_pd,
						  	  (void *)mr_attr->mr_iov->iov_base,
						  	  mr_attr->mr_iov->iov_len, access);
				}
				return NULL;
			}

			p_info->dmabuf_supported_by_device = EFA_DMABUF_SUPPORTED;
			return dmabuf_mr;

		} else if (p_info->dmabuf_fallback_enabled) {
			/* get fd failed, fallback */
			EFA_INFO(FI_LOG_MR,
				 "Unable to get dmabuf fd for %s device buffer, "
				 "Fall back to ibv_reg_mr\n",
				 fi_tostr(&efa_mr->peer.iface, FI_TYPE_HMEM_IFACE));
			efa_mark_dmabuf_fail(p_info, efa_mr->peer.iface);
			return ibv_reg_mr(efa_mr->domain->ibv_pd,
					  (void *)mr_attr->mr_iov->iov_base,
					  mr_attr->mr_iov->iov_len, access);
		}
		
		/* get fd failed, no fallback */
		EFA_WARN(FI_LOG_MR,
			 "ofi_hmem_get_dmabuf_fd failed for %s: ret=%d (%s)\n",
			 fi_tostr(&efa_mr->peer.iface, FI_TYPE_HMEM_IFACE), ret, fi_strerror(-ret));
		p_info->dmabuf_supported_by_device = EFA_DMABUF_NOT_SUPPORTED;

		return NULL;
	} else {
		/* dmabuf is not supported */
		return ibv_reg_mr(efa_mr->domain->ibv_pd,
				  (void *)mr_attr->mr_iov->iov_base,
				  mr_attr->mr_iov->iov_len, access);
	}
}

#if HAVE_CUDA
static inline
int efa_mr_is_cuda_memory_freed(struct efa_mr *efa_mr, bool *freed)
{
	int err;
	uint64_t buffer_id;

	err = ofi_cuPointerGetAttribute(&buffer_id, CU_POINTER_ATTRIBUTE_BUFFER_ID,
					(CUdeviceptr)efa_mr->ibv_mr->addr);

	if (err == CUDA_ERROR_INVALID_VALUE) {
		/* According to CUDA document, the return code of ofi_cuPointerGetAttribute() being CUDA_ERROR_INVALID_VALUE
		 * means the efa_mr's pointer is NOT allocated by, mapped by or registered with CUDA.
		 * Because the address was registered, the only possiblity is that the memory has been freed.
		 */
		*freed = true;
		return 0;
	}

	if (!err) {
		/* Buffer ID mismatch means the original buffer was freed, and a new buffer has been
		 * allocated with the same address
		 */
		*freed = (buffer_id != efa_mr->entry->hmem_info.cuda_id);
		return 0;
	}

	EFA_WARN(FI_LOG_DOMAIN, "cuPointerGetAttribute() failed with error code: %d, error message: %s\n",
		 err, ofi_cudaGetErrorString(err));
	return -FI_EINVAL;
}

/**
 * @brief update the mr_map inside util_domain with a new memory registration.
 *
 * The mr_map is in util domain is a map between MR key and MR, and has all
 * the active MR in it. This function add the information of a new MR into
 * the mr_map.
 *
 * @param	efa_mr		the pointer to the efa_mr object, which has pointer to domain
 * @param	mr_attr		the pointer to an fi_mr_attr object.
 * @return	0 on success.
 * 		negative libfabric error code on failure.
 */
static
int efa_mr_update_domain_mr_map(struct efa_mr *efa_mr, struct fi_mr_attr *mr_attr,
				uint64_t flags)
{
	struct fid_mr *existing_mr_fid;
	struct efa_mr *existing_mr;
	bool cuda_memory_freed;
	int err;

	mr_attr->requested_key = efa_mr->mr_fid.key;
	ofi_genlock_lock(&efa_mr->domain->util_domain.lock);
	err = ofi_mr_map_insert(&efa_mr->domain->util_domain.mr_map, mr_attr,
				&efa_mr->mr_fid.key, &efa_mr->mr_fid, flags);
	ofi_genlock_unlock(&efa_mr->domain->util_domain.lock);
	if (!err)
		return 0;

	/* There is a special error that we can recover from, which is:
	 * 1. error code is FI_ENOKEY, which means there is already a MR in mr_map with the same key
	 * 2. that MR is for a CUDA memory region
	 * 3. that CUDA memory region has been freed.
	 *
	 * This situation can happen because the combination of the following 3 things:
	 * 1. cuda memory uses memory cache.
	 * 2. cuda memory cache's monitor is not monitoring call to cudaFree().
	 * 3. kernel released the kernel space of memory registration when cudaFree() is called on a registered cuda memory.
	 *
	 * Therefore, when application call cudaFree() on a registered CUDA memory region, the EFA kernel module
	 * device released the kernel space memory region, made the key available for reuse.
	 * However, libfabric is not aware of the incident, and kept the user space memory registration in its
	 * MR cache and MR map.
	 *
	 * After we implement cuda memhook monitor (which monitors call to cudaFree() and remove dead region from MR cache),
	 * we should NOT encounter this special situation any more. At that time, this function should be removed.
	 */
	if (err != -FI_ENOKEY) {
		/* no way we can recover from this error, return error code */
		EFA_WARN(FI_LOG_MR,
			"Unable to add MR to map. errno: %d errmsg: (%s) key: %ld buff: %p hmem_iface: %s len: %zu\n",
			err,
			fi_strerror(-err),
			efa_mr->mr_fid.key,
			mr_attr->mr_iov->iov_base,
			fi_tostr(&mr_attr->iface, FI_TYPE_HMEM_IFACE),
			mr_attr->mr_iov->iov_len);
		return err;
	}

	existing_mr_fid = ofi_mr_map_get(&efa_mr->domain->util_domain.mr_map, efa_mr->mr_fid.key);
	assert(existing_mr_fid);
	existing_mr = container_of(existing_mr_fid, struct efa_mr, mr_fid);

	if (existing_mr->peer.iface != FI_HMEM_CUDA) {
		/* no way we can recover from this situation, return error code */
		EFA_WARN(FI_LOG_DOMAIN, "key %ld already assigned to buffer: %p hmem_iface: %s length: %ld\n",
			 existing_mr->mr_fid.key,
			 existing_mr->ibv_mr->addr,
			 fi_tostr(&existing_mr->peer.iface, FI_TYPE_HMEM_IFACE),
			 existing_mr->ibv_mr->length);
		return -FI_ENOKEY;
	}

	err = efa_mr_is_cuda_memory_freed(existing_mr, &cuda_memory_freed);
	if (err)
		return err;

	if (!cuda_memory_freed) {
		/* The same key was assigned to two valid cuda memory region,
		 * there is no way we can recover from this situation, return error code */
		EFA_WARN(FI_LOG_DOMAIN, "key %ld has already assigned to another cuda buffer: %p length: %ld\n",
			 existing_mr->mr_fid.key,
			 existing_mr->ibv_mr->addr,
			 existing_mr->ibv_mr->length);
		return -FI_ENOKEY;
	}

	EFA_INFO(FI_LOG_DOMAIN, "key %ld has been assigned to cuda buffer: %p length: %ld, which has since been freed\n",
		 existing_mr->mr_fid.key,
		 existing_mr->ibv_mr->addr,
		 existing_mr->ibv_mr->length);

	/* this can only happen when MR cache is enabled, hence the assertion */
	assert(efa_mr->domain->cache);
	pthread_mutex_lock(&mm_lock);
	ofi_mr_cache_notify(efa_mr->domain->cache, existing_mr->ibv_mr->addr, existing_mr->ibv_mr->length);
	pthread_mutex_unlock(&mm_lock);

	/* due to MR cache's deferred de-registration, ofi_mr_cache_notify() only move the region to dead_region_list
	 * ofi_mr_cache_flush() will actually remove the region from cache.
	 * lru is a list of regions that are still active, so we set flush_lru to false.
	 */
	ofi_mr_cache_flush(efa_mr->domain->cache, false /*flush_lru */);

	/*
	 * When MR cache removes a MR, it will call its delete_region() call back. delete_region() calls efa_mr_dereg_impl(),
	 * which should remove the staled entry from MR map. So insert again here.
	 */
	ofi_genlock_lock(&efa_mr->domain->util_domain.lock);
	err = ofi_mr_map_insert(&efa_mr->domain->util_domain.mr_map, mr_attr,
				&efa_mr->mr_fid.key, &efa_mr->mr_fid, flags);
	ofi_genlock_unlock(&efa_mr->domain->util_domain.lock);
	if (err) {
		EFA_WARN(FI_LOG_MR,
			"Unable to add MR to map, even though we already tried to evict staled memory registration."
			"errno: %d errmsg: (%s) key: %ld buff: %p hmem_iface: %s len: %zu\n",
			err,
			fi_strerror(-err),
			efa_mr->mr_fid.key,
			mr_attr->mr_iov->iov_base,
			fi_tostr(&mr_attr->iface, FI_TYPE_HMEM_IFACE),
			mr_attr->mr_iov->iov_len);
		return err;
	}

	return 0;
}
#else /* HAVE_CUDA */
static
int efa_mr_update_domain_mr_map(struct efa_mr *efa_mr, struct fi_mr_attr *mr_attr,
				uint64_t flags)
{
	int err;

	mr_attr->requested_key = efa_mr->mr_fid.key;
	ofi_genlock_lock(&efa_mr->domain->util_domain.lock);
	err = ofi_mr_map_insert(&efa_mr->domain->util_domain.mr_map, mr_attr,
				&efa_mr->mr_fid.key, &efa_mr->mr_fid, flags);
	ofi_genlock_unlock(&efa_mr->domain->util_domain.lock);
	if (err) {
		EFA_WARN(FI_LOG_MR,
			"Unable to add MR to map. errno: %d errmsg: (%s) key: %ld buff: %p hmem_iface: %s len: %zu\n",
			err,
			fi_strerror(-err),
			efa_mr->mr_fid.key,
			mr_attr->mr_iov->iov_base,
			fi_tostr(&mr_attr->iface, FI_TYPE_HMEM_IFACE),
			mr_attr->mr_iov->iov_len);
		return err;
	}

	return 0;
}
#endif /* HAVE_CUDA */

/*
 * Since ibv_reg_mr() will fail for accelerator buffers when p2p is unavailable
 * (and thus isn't called), generate a proprietary internal key for
 * efa_mr->mr_fid.key. The key must be larger than UINT32_MAX to avoid potential
 * collisions with keys returned by ibv_reg_mr() for standard MR registrations.
 */
static uint64_t efa_mr_non_p2p_keygen(void) {
	static uint64_t NON_P2P_MR_KEYGEN = NON_P2P_MR_KEYGEN_INIT;
	return NON_P2P_MR_KEYGEN++;
}

/*
 * Set ofi_access to FI_SEND | FI_RECV if not already set,
 * Convert ofi_access flags to ibv_access flags.
 */
int efa_mr_ofi_to_ibv_access(uint64_t ofi_access,
			     bool device_support_rdma_read,
			     bool device_support_rdma_write)
{
	int ibv_access = 0;

	/* To support Emulated RMA path, if the access is not supported
	 * by EFA, modify it to FI_SEND | FI_RECV
	 */
	if (!ofi_access || (ofi_access & ~EFA_MR_SUPPORTED_PERMISSIONS))
		ofi_access = FI_SEND | FI_RECV;

	if (ofi_access & FI_RECV)
		ibv_access |= IBV_ACCESS_LOCAL_WRITE;

	if (device_support_rdma_read) {
		if (ofi_access & (FI_REMOTE_WRITE | FI_READ))
			ibv_access |= IBV_ACCESS_LOCAL_WRITE;

		if (ofi_access & (FI_REMOTE_READ | FI_SEND) ||
		    (ofi_access & FI_WRITE && !device_support_rdma_write))
			/* IBV_ACCESS_REMOTE_READ is needed for emulating
			 * fi_send/fi_write with RDMA read */
			ibv_access |= IBV_ACCESS_REMOTE_READ;
	}

	if (device_support_rdma_write && (ofi_access & FI_REMOTE_WRITE))
		/* If IBV_ACCESS_REMOTE_WRITE is set, then
		 * IBV_ACCESS_LOCAL_WRITE must be set too since remote write
		 * should be allowed only if local write is allowed. */
		ibv_access |= IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE;

	return ibv_access;
}

/**
 * @brief Register a memory region and for efa internal usage
 * This function is only used internally by the EFA provider to register
 * a memory region to EFA device when the mr cache is NOT availble
 *
 * @param[in]	fid		domain fid
 * @param[in]	iov		iovec to be registered
 * @param[in]	access		access flags for MR
 * @param[in]	flags		MR flags
 * @param[out]	mr_fid		MR descriptor fids
 *
 * @return FI_SUCCESS or negative FI error code
 */
int efa_mr_internal_regv(struct fid_domain *domain_fid, const struct iovec *iov,
		      size_t count, uint64_t access, uint64_t offset,
		      uint64_t requested_key, uint64_t flags,
		      struct fid_mr **mr, void *context)
{
	struct fi_mr_attr attr = EFA_MR_ATTR_INIT_SYSTEM(iov, count, access, offset, requested_key, context);
	struct efa_mr *efa_mr;
	struct efa_domain *domain;
	int ret;
	*mr = NULL;

	domain = container_of(domain_fid, struct efa_domain,
			      util_domain.domain_fid);
	efa_mr = calloc(1, sizeof(*efa_mr));
	if (!efa_mr) {
		EFA_WARN(FI_LOG_MR, "Unable to initialize MR\n");
		return -FI_ENOMEM;
	}

	efa_mr->domain = domain;
	efa_mr->mr_fid.fid.fclass = FI_CLASS_MR;
	efa_mr->mr_fid.fid.context = attr.context;
	efa_mr->mr_fid.fid.ops = &efa_mr_ops;

	ret = efa_mr_reg_impl(efa_mr, flags, &attr);
	if (ret) {
		EFA_WARN(FI_LOG_MR, "Unable to register MR: %s\n",
			fi_strerror(-ret));
		free(efa_mr);
		return ret;
	}
	*mr = &efa_mr->mr_fid;
	return 0;
}

/*
 * Set the fi_ibv_access modes and do real registration (ibv_mr_reg)
 * Insert the key returned by ibv_mr_reg into efa mr_map and shm mr_map
 */
static int efa_mr_reg_impl(struct efa_mr *efa_mr, uint64_t flags, const struct fi_mr_attr *mr_attr)
{
	int64_t reg_sz, reg_ct;
	int ret = 0;
	bool device_support_rdma_read = false;
	bool device_support_rdma_write = false;

	efa_mr->ibv_mr = NULL;
	efa_mr->shm_mr = NULL;
	efa_mr->inserted_to_mr_map = false;
	efa_mr->mr_fid.mem_desc = NULL;
	efa_mr->mr_fid.key = FI_KEY_NOTAVAIL;
	efa_mr->needs_sync = false;

	ret = efa_mr_hmem_setup(efa_mr, mr_attr, flags);
	if (ret)
		return ret;

	device_support_rdma_read = efa_mr->domain->device->device_caps & EFADV_DEVICE_ATTR_CAPS_RDMA_READ;
#if HAVE_CAPS_RDMA_WRITE
	device_support_rdma_write = efa_mr->domain->device->device_caps & EFADV_DEVICE_ATTR_CAPS_RDMA_WRITE;
#endif
	/* For efa-direct, fail registration if RDMA operations are requested 
	 * but hardware doesn't support them */
	if (efa_mr->domain->info_type == EFA_INFO_DIRECT) {
		if ((mr_attr->access & (FI_READ | FI_REMOTE_READ)) &&
		    !device_support_rdma_read) {
			EFA_WARN(FI_LOG_MR, "FI_READ or FI_REMOTE_READ "
					    "requested but hardware does not "
					    "support RDMA read operations\n");
			return -FI_EOPNOTSUPP;
		}
		if (mr_attr->access & (FI_WRITE | FI_REMOTE_WRITE) &&
		    !device_support_rdma_write) {
			EFA_WARN(FI_LOG_MR, "FI_WRITE or FI_REMOTE_WRITE "
					    "requested but hardware does not "
					    "support RDMA write operations\n");
			return -FI_EOPNOTSUPP;
		}
	}

	if (efa_mr->domain->cache)
		ofi_mr_cache_flush(efa_mr->domain->cache, false);

	/*
	 * For FI_HMEM_CUDA iface when p2p is unavailable, skip ibv_reg_mr() and
	 * generate proprietary mr_fid key.
	 */
	if ((mr_attr->iface == FI_HMEM_CUDA || mr_attr->iface == FI_HMEM_ROCR)
		&& !g_efa_hmem_info[mr_attr->iface].p2p_supported_by_device) {
		efa_mr->mr_fid.key = efa_mr_non_p2p_keygen();
	} else {
		efa_mr->ibv_mr = efa_mr_reg_ibv_mr(
			efa_mr, (struct fi_mr_attr *)mr_attr,
			efa_mr_ofi_to_ibv_access(mr_attr->access,
						 device_support_rdma_read,
						 device_support_rdma_write),
			flags);
		if (!efa_mr->ibv_mr) {
			/* error path that doesn't call ibv_reg doesn't set errno */
			if (!errno)
				errno = ENOTSUP;
			EFA_WARN(FI_LOG_MR,
				 "Unable to register MR of %zu bytes: %s, "
				 "flags %ld, ibv pd: %p, total mr "
				 "reg size %zd, mr reg count %zd\n",
				 (flags & FI_MR_DMABUF) ?
					 mr_attr->dmabuf->len :
					 mr_attr->mr_iov->iov_len,
				 strerror(errno), flags,
				 efa_mr->domain->ibv_pd,
				 ofi_atomic_get64(&efa_mr->domain->ibv_mr_reg_sz),
				 ofi_atomic_get64(&efa_mr->domain->ibv_mr_reg_ct));
			if (efa_mr->peer.iface == FI_HMEM_CUDA &&
			    (efa_mr->peer.flags & OFI_HMEM_DATA_DEV_REG_HANDLE)) {
				assert(efa_mr->peer.hmem_data);
				ofi_hmem_dev_unregister(FI_HMEM_CUDA, (uint64_t)efa_mr->peer.hmem_data);
			}

			return -errno;
		}
		reg_ct = ofi_atomic_inc64(&efa_mr->domain->ibv_mr_reg_ct);
		reg_sz = ofi_atomic_add64(&efa_mr->domain->ibv_mr_reg_sz, efa_mr->ibv_mr->length);
		EFA_INFO(FI_LOG_MR,
			 "Registered memory at %p of size %zu"
			 "flags %ld for ibv pd %p, "
			 "total mr reg size %zd, mr reg count %zd\n",
			 efa_mr->ibv_mr->addr, efa_mr->ibv_mr->length,
			 flags, efa_mr->domain->ibv_pd,
			 reg_sz,
			 reg_ct);
		efa_mr->mr_fid.key = efa_mr->ibv_mr->rkey;
	}
	efa_mr->mr_fid.mem_desc = efa_mr;
	assert(efa_mr->mr_fid.key != FI_KEY_NOTAVAIL);

	ret = efa_mr_update_domain_mr_map(efa_mr, (struct fi_mr_attr *)mr_attr, flags);
	if (ret) {
		efa_mr_dereg_impl(efa_mr);
		return ret;
	}

	efa_mr->inserted_to_mr_map = true;

	return 0;
}

static int efa_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
			  uint64_t flags, struct fid_mr **mr_fid)
{
	struct efa_domain *domain;
	struct efa_mr *efa_mr = NULL;
	uint64_t supported_flags;
	int ret = 0;
	uint32_t api_version;
	struct fi_mr_attr mr_attr = {0};

	/*
	 * Notes supported memory registration flags:
	 *
	 * FI_HMEM_DEVICE_ONLY:
	 * This flag is used by some provider that need to distinguish
	 * whether a device memory can be accessed from device only, or
	 * can be access from host. EFA provider considers all device memory
	 * to be accessed by device only. Therefore, this function claim
	 * support of this flag, but do not save it in efa_mr.
	 *
	 * FI_MR_DMABUF:
	 * This flag indicates that the memory region to registered is
	 * a DMA-buf backed region. When set, the region is specified through
	 * the dmabuf field of the fi_mr_attr structure. This flag is only
	 * usable for domains opened with FI_HMEM capability support.
	 * This flag is introduced since Libfabric 1.20.
	 */
	supported_flags = FI_HMEM_DEVICE_ONLY;

	domain = container_of(fid, struct efa_domain,
			      util_domain.domain_fid.fid);
	api_version = domain->util_domain.fabric->fabric_fid.api_version;

	if (FI_VERSION_GE(api_version, FI_VERSION(1, 20)))
		supported_flags |= FI_MR_DMABUF;

	if (flags & (~supported_flags)) {
		EFA_WARN(FI_LOG_MR, "Unsupported flag type. requested"
			 "[0x%" PRIx64 "] supported[0x%" PRIx64 "]\n",
			 flags, supported_flags);
		return -FI_EBADFLAGS;
	}

	if (fid->fclass != FI_CLASS_DOMAIN) {
		EFA_WARN(FI_LOG_MR, "Unsupported domain. requested"
			 "[0x%" PRIx64 "] supported[0x%" PRIx64 "]\n",
			 fid->fclass, (uint64_t) FI_CLASS_DOMAIN);
		return -FI_EINVAL;
	}

	if (attr->iov_count > EFA_MR_IOV_LIMIT) {
		EFA_WARN(FI_LOG_MR, "iov count > %d not supported\n",
			 EFA_MR_IOV_LIMIT);
		return -FI_EINVAL;
	}

	if (attr->iface >= OFI_HMEM_MAX || !g_efa_hmem_info[attr->iface].initialized) {
		EFA_WARN(FI_LOG_MR,
			 "Cannot register memory for uninitialized iface (%s)\n",
			 fi_tostr(&attr->iface, FI_TYPE_HMEM_IFACE));
		return -FI_ENOSYS;
	}

	efa_mr = calloc(1, sizeof(*efa_mr));
	if (!efa_mr) {
		EFA_WARN(FI_LOG_MR, "Unable to initialize md\n");
		return -FI_ENOMEM;
	}

	efa_mr->domain = domain;
	efa_mr->mr_fid.fid.fclass = FI_CLASS_MR;
	efa_mr->mr_fid.fid.context = attr->context;
	efa_mr->mr_fid.fid.ops = &efa_mr_ops;

	/* Update attr for ABI compatibility */
	ofi_mr_update_attr(
		efa_mr->domain->util_domain.fabric->fabric_fid.api_version,
		efa_mr->domain->util_domain.info_domain_caps,
		attr, &mr_attr, flags);

	ret = efa_mr_reg_impl(efa_mr, flags, &mr_attr);
	if (ret)
		goto err;

	if (efa_mr->domain->shm_domain) {
		uint64_t shm_flags;

		/* Inherit peer.flags with addtional feature bits such as gdrcopy handle switch */
		shm_flags = efa_mr->peer.flags;
		if (mr_attr.iface != FI_HMEM_SYSTEM) {
			/* shm provider need the flag to turn on IPC support */
			shm_flags |= FI_HMEM_DEVICE_ONLY;
		}

		mr_attr.hmem_data = efa_mr->peer.hmem_data;

		ret = fi_mr_regattr(efa_mr->domain->shm_domain, &mr_attr,
				    shm_flags, &efa_mr->shm_mr);

		if (ret) {
			EFA_WARN(FI_LOG_MR,
				 "Unable to register shm MR. errno: %d "
				 "err_msg: (%s) key: %ld buf: %p len: %zu "
				 "flags %ld\n",
				 ret, fi_strerror(-ret), efa_mr->mr_fid.key,
				 mr_attr.mr_iov ? mr_attr.mr_iov->iov_base :
						  NULL,
				 mr_attr.mr_iov ? mr_attr.mr_iov->iov_len : 0,
				 flags);
			efa_mr_dereg_impl(efa_mr);
			free(efa_mr);
			return ret;
		}
	}

	*mr_fid = &efa_mr->mr_fid;
	return 0;
err:
	EFA_WARN(FI_LOG_MR, "Unable to register MR: %s\n",
			fi_strerror(-ret));
	free(efa_mr);
	return ret;
}

static int efa_mr_regv(struct fid *fid, const struct iovec *iov,
		       size_t count, uint64_t access, uint64_t offset, uint64_t requested_key,
		       uint64_t flags, struct fid_mr **mr_fid, void *context)
{
	struct fi_mr_attr attr = EFA_MR_ATTR_INIT_SYSTEM(iov, count, access, offset, requested_key, context);

	return efa_mr_regattr(fid, &attr, flags, mr_fid);
}

static int efa_mr_reg(struct fid *fid, const void *buf, size_t len,
		      uint64_t access, uint64_t offset, uint64_t requested_key,
		      uint64_t flags, struct fid_mr **mr_fid, void *context)
{
	struct iovec iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	return efa_mr_regv(fid, &iov, 1, access, offset, requested_key,
			   flags, mr_fid, context);
}

struct fi_ops_mr efa_domain_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = efa_mr_reg,
	.regv = efa_mr_regv,
	.regattr = efa_mr_regattr,
};
