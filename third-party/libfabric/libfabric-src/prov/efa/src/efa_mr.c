/*
 * Copyright (c) 2017-2020 Amazon.com, Inc. or its affiliates. All rights reserved.
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

#include "config.h"
#include <ofi_util.h>
#include "efa.h"
#if HAVE_CUDA
#include <cuda.h>
#endif

static int efa_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
			  uint64_t flags, struct fid_mr **mr_fid);
static int efa_mr_reg_impl(struct efa_mr *efa_mr, uint64_t flags, void *attr);
static int efa_mr_dereg_impl(struct efa_mr *efa_mr);


#define EFA_DEF_MR_CACHE_ENABLE 1
int efa_mr_cache_enable	= EFA_DEF_MR_CACHE_ENABLE;
size_t efa_mr_max_cached_count;
size_t efa_mr_max_cached_size;

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

/*
 * @brief Validate HMEM attributes and populate efa_mr struct
 *
 * Check if FI_HMEM is enabled for the domain, validate whether the specific
 * device type requested is currently supported by the provider, and update the
 * efa_mr structure based on the attributes requested by the user.
 *
 * @params[in]	efa_mr	efa_mr structure to be updated
 * @params[in]	attr	fi_mr_attr from the user's registration call
 *
 * @return FI_SUCCESS or negative FI error code
 */
static int efa_mr_hmem_setup(struct efa_mr *efa_mr,
                             const struct fi_mr_attr *attr)
{
	int err;

	if (attr->iface == FI_HMEM_SYSTEM) {
		efa_mr->peer.iface = FI_HMEM_SYSTEM;
		return FI_SUCCESS;
	}

	if (efa_mr->domain->util_domain.info_domain_caps & FI_HMEM) {
		/*
		 * Skipping the domain type check above is okay here since
		 * util_domain is at the beginning of both efa_domain and
		 * rxr_domain.
		 */
		if (efa_mr->domain->hmem_info[attr->iface].initialized) {
			efa_mr->peer.iface = attr->iface;
		} else {
			EFA_WARN(FI_LOG_MR,
				 "FI_HMEM is not initialized for device type %d\n",
				 attr->iface);
			return -FI_ENOSYS;
		}
	} else {
		/*
		 * It's possible that attr->iface is not initialized when
		 * FI_HMEM is off, so this can't be a fatal error. Print a
		 * warning in case this value is not FI_HMEM_SYSTEM for
		 * whatever reason.
		 */
		FI_WARN_ONCE(&efa_prov, FI_LOG_MR,
		             "FI_HMEM support is disabled, assuming FI_HMEM_SYSTEM not type: %d.\n",
		             attr->iface);
		efa_mr->peer.iface = FI_HMEM_SYSTEM;
	}

	/* efa_mr->peer.device is an union. Setting reserved to 0 cleared everything in it (cuda, neuron, synapseai etc) */
	efa_mr->peer.device.reserved = 0;
	if (efa_mr->peer.iface == FI_HMEM_CUDA) {
		if (rxr_env.set_cuda_sync_memops) {
			err = cuda_set_sync_memops(attr->mr_iov->iov_base);
			if (err) {
				EFA_WARN(FI_LOG_MR, "unable to set memops for cuda ptr\n");
				return err;
			}
		}
		err = cuda_dev_register((struct fi_mr_attr *)attr, &efa_mr->peer.device.cuda);
		if (err) {
			EFA_WARN(FI_LOG_MR,
				 "Unable to register handle for GPU memory. err: %d buf: %p len: %zu\n",
				 err, attr->mr_iov->iov_base, attr->mr_iov->iov_len);
			return err;
		}
	} else if (attr->iface == FI_HMEM_NEURON) {
		efa_mr->peer.device.neuron = attr->device.neuron;
	} else if (attr->iface == FI_HMEM_SYNAPSEAI) {
		efa_mr->peer.device.synapseai = attr->device.synapseai;
	}

	return FI_SUCCESS;
}


int efa_mr_cache_entry_reg(struct ofi_mr_cache *cache,
			   struct ofi_mr_entry *entry)
{
	int ret = 0;
	/* TODO
	 * Since, access is not passed as a parameter to efa_mr_cache_entry_reg,
	 * for now we will set access to all supported access modes i.e.
	 * FI_SEND | FI_RECV | FI_REMOTE_READ | FI_REMOTE_WRITE. Once access
	 * information is available this can be removed.
	 * Issue: https://github.com/ofiwg/libfabric/issues/5677
	 */
	uint64_t access = FI_SEND | FI_RECV | FI_REMOTE_READ | FI_REMOTE_WRITE;
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

	ret = efa_mr_reg_impl(efa_mr, 0, (void *)&attr);
	return ret;
}

void efa_mr_cache_entry_dereg(struct ofi_mr_cache *cache,
			      struct ofi_mr_entry *entry)
{
	struct efa_mr *efa_mr = (struct efa_mr *)entry->data;
	int ret;

	if (!efa_mr->ibv_mr)
		return;
	ret = efa_mr_dereg_impl(efa_mr);
	if (ret)
		EFA_WARN(FI_LOG_MR, "Unable to dereg mr: %d\n", ret);
}

/*
 * efa_mr_reg_shm() is called by rxr_read_init_iov to used to generate
 * shm only memory registrations. Such memory registrations were used
 * when read message protocol was applied to SHM EP. In which case,
 * we need to register the send iov as FI_REMOTE_READ.
 *
 * Note when we open the SHM domain we did not specify FI_MR_PROV_KEY
 * therefore the SHM domain require us to proivde a key when calling
 * fi_mr_reg on it. (rxr_set_shm_hints())
 *
 * The reason we did not specify FI_MR_PROV_KEY when opening SHM
 * domain is because we want ibv_mr and shm_mr to use the same
 * key. For that, we first call ibv_reg_mr() to register memory
 * and get a key, and use that key to register shm. (efa_m_reg_impl())
 *
 * However, for SHM's read message protocol, we do not want to call
 * ibv_reg_mr() because it is expensive, so we use a static variable
 * SHM_MR_KEYGEN to generate key.
 *
 * It is initialized as 0x100000000, and each call to efa_mr_reg_shm()
 * will use shm_mr_keygen as current key and increase it by 1.
 *
 * Note SHM_MR_KEYGEN starts from 0x100000000 because the key
 * returned from ibv_reg_mr() is a 32 bits integer, thus is always
 * smaller than 0x100000000. By starting from 0x100000000, we avoid
 * key collision.
 */
int efa_mr_reg_shm(struct fid_domain *domain_fid, struct iovec *iov,
		   uint64_t access, struct fid_mr **mr_fid)
{
	static uint64_t SHM_MR_KEYGEN = 0x100000000;
	uint64_t requested_key;
	struct efa_domain *efa_domain;

	efa_domain = container_of(domain_fid, struct efa_domain, util_domain.domain_fid.fid);
	assert(efa_domain->shm_domain);

	requested_key = SHM_MR_KEYGEN++;
	return fi_mr_regv(efa_domain->shm_domain, iov, 1, access, 0, requested_key, 0, mr_fid, NULL);
}

static int efa_mr_cache_regattr(struct fid *fid, const struct fi_mr_attr *attr,
				uint64_t flags, struct fid_mr **mr_fid)
{
	struct efa_domain *domain;
	struct efa_mr *efa_mr;
	struct ofi_mr_entry *entry;
	struct ofi_mr_info info;
	int ret;

	if (attr->iface == FI_HMEM_NEURON || attr->iface == FI_HMEM_SYNAPSEAI)
		flags |= OFI_MR_NOCACHE;

	if (flags & OFI_MR_NOCACHE) {
		ret = efa_mr_regattr(fid, attr, flags, mr_fid);
		return ret;
	}

	if (attr->iov_count > EFA_MR_IOV_LIMIT) {
		EFA_WARN(FI_LOG_MR, "iov count > %d not supported\n",
			 EFA_MR_IOV_LIMIT);
		return -FI_EINVAL;
	}

	if (!ofi_hmem_is_initialized(attr->iface)) {
		EFA_WARN(FI_LOG_MR,
			 "Cannot register memory for uninitialized iface\n");
		return -FI_ENOSYS;
	}

	domain = container_of(fid, struct efa_domain,
			      util_domain.domain_fid.fid);

	assert(attr->iov_count == 1);
	info.iov = *attr->mr_iov;
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

static int efa_mr_cache_regv(struct fid *fid, const struct iovec *iov,
			     size_t count, uint64_t access, uint64_t offset,
			     uint64_t requested_key, uint64_t flags,
			     struct fid_mr **mr_fid, void *context)
{
	struct fi_mr_attr attr;

	attr.mr_iov = iov;
	attr.iov_count = count;
	attr.access = access;
	attr.offset = offset;
	attr.requested_key = requested_key;
	attr.context = context;
	attr.iface = FI_HMEM_SYSTEM;

	return efa_mr_cache_regattr(fid, &attr, flags, mr_fid);
}

static int efa_mr_cache_reg(struct fid *fid, const void *buf, size_t len,
			    uint64_t access, uint64_t offset,
			    uint64_t requested_key, uint64_t flags,
			    struct fid_mr **mr_fid, void *context)
{
	struct iovec iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	return efa_mr_cache_regv(fid, &iov, 1, access, offset, requested_key,
				 flags, mr_fid, context);
}

struct fi_ops_mr efa_domain_mr_cache_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = efa_mr_cache_reg,
	.regv = efa_mr_cache_regv,
	.regattr = efa_mr_cache_regattr,
};

static int efa_mr_dereg_impl(struct efa_mr *efa_mr)
{
	struct efa_domain *efa_domain;
	int ret = 0;
	int err;

	efa_domain = efa_mr->domain;
	err = -ibv_dereg_mr(efa_mr->ibv_mr);
	if (err) {
		EFA_WARN(FI_LOG_MR,
			"Unable to deregister memory registration\n");
		ret = err;
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

	if (efa_mr->shm_mr) {
		err = fi_close(&efa_mr->shm_mr->fid);
		if (err) {
			EFA_WARN(FI_LOG_MR,
				"Unable to close shm MR\n");
			ret = err;
		}

		efa_mr->shm_mr = NULL;
	}

	if (efa_mr->peer.iface == FI_HMEM_CUDA) {
		err = cuda_dev_unregister(efa_mr->peer.device.cuda);
		if (err) {
			EFA_WARN(FI_LOG_MR,
				"Unable to de-register cuda handle\n");
			ret = err;
		}

		efa_mr->peer.device.cuda = 0;
	}

	efa_mr->mr_fid.mem_desc = NULL;
	efa_mr->mr_fid.key = FI_KEY_NOTAVAIL;
	return ret;
}

static int efa_mr_close(fid_t fid)
{
	struct efa_mr *efa_mr;
	int ret;

	efa_mr = container_of(fid, struct efa_mr, mr_fid.fid);
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

#if HAVE_SYNAPSEAI
/**
 * @brief Register a memory buffer with rdma-core api.
 *
 * @param efa_mr the ptr to the efa_mr object
 * @param mr_attr the ptr to the fi_mr_attr object
 * @param access the desired memory protection attributes
 * @return struct ibv_mr* the ptr to the registered MR
 */
static struct ibv_mr *efa_mr_reg_ibv_mr(struct efa_mr *efa_mr, struct fi_mr_attr *mr_attr, int access)
{
	int dmabuf_fd;
	int ret;
	if (efa_mr_is_synapseai(efa_mr)) {
		ret = synapseai_get_dmabuf_fd((uint64_t) mr_attr->mr_iov->iov_base,
						(uint64_t) mr_attr->mr_iov->iov_len,
						&dmabuf_fd);
		if (ret != FI_SUCCESS) {
			EFA_WARN(FI_LOG_MR, "Unable to get dmabuf fd for Gaudi device buffer \n");
			return NULL;
		}
		return ibv_reg_dmabuf_mr(efa_mr->domain->ibv_pd, 0,
					mr_attr->mr_iov->iov_len,
					(uint64_t)mr_attr->mr_iov->iov_base,
					dmabuf_fd, access);
	} else {
		return ibv_reg_mr(efa_mr->domain->ibv_pd,
				(void *)mr_attr->mr_iov->iov_base,
				mr_attr->mr_iov->iov_len, access);
	}
}
#else
/**
 * @brief Register a memory buffer with rdma-core api.
 *
 * @param efa_mr the ptr to the efa_mr object
 * @param mr_attr the ptr to the fi_mr_attr object
 * @param access the desired memory protection attributes
 * @return struct ibv_mr* the ptr to the registered MR
 */
static struct ibv_mr *efa_mr_reg_ibv_mr(struct efa_mr *efa_mr, struct fi_mr_attr *mr_attr, int access)
{
	return ibv_reg_mr(efa_mr->domain->ibv_pd,
			(void *)mr_attr->mr_iov->iov_base,
			mr_attr->mr_iov->iov_len, access);
}
#endif /* HAVE_SYNAPSEAI */

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
int efa_mr_update_domain_mr_map(struct efa_mr *efa_mr, struct fi_mr_attr *mr_attr)
{
	struct fid_mr *existing_mr_fid;
	struct efa_mr *existing_mr;
	bool cuda_memory_freed;
	int err;

	mr_attr->requested_key = efa_mr->mr_fid.key;
	ofi_genlock_lock(&efa_mr->domain->util_domain.lock);
	err = ofi_mr_map_insert(&efa_mr->domain->util_domain.mr_map, mr_attr,
				&efa_mr->mr_fid.key, &efa_mr->mr_fid);
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
			"Unable to add MR to map. errno: %d errmsg: (%s) key: %ld buff: %p hmem_iface: %d len: %zu\n",
			err,
			fi_strerror(-err),
			efa_mr->mr_fid.key,
			mr_attr->mr_iov->iov_base,
			mr_attr->iface,
			mr_attr->mr_iov->iov_len);
		return err;
	}

	existing_mr_fid = ofi_mr_map_get(&efa_mr->domain->util_domain.mr_map, efa_mr->mr_fid.key);
	assert(existing_mr_fid);
	existing_mr = container_of(existing_mr_fid, struct efa_mr, mr_fid);

	if (existing_mr->peer.iface != FI_HMEM_CUDA) {
		/* no way we can recover from this situation, return error code */
		EFA_WARN(FI_LOG_DOMAIN, "key %ld already assigned to buffer: %p hmem_iface: %d length: %ld\n",
			 existing_mr->mr_fid.key,
			 existing_mr->ibv_mr->addr,
			 existing_mr->peer.iface,
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
				&efa_mr->mr_fid.key, &efa_mr->mr_fid);
	ofi_genlock_unlock(&efa_mr->domain->util_domain.lock);
	if (err) {
		EFA_WARN(FI_LOG_MR,
			"Unable to add MR to map, even though we already tried to evict staled memory registration."
			"errno: %d errmsg: (%s) key: %ld buff: %p hmem_iface: %d len: %zu\n",
			err,
			fi_strerror(-err),
			efa_mr->mr_fid.key,
			mr_attr->mr_iov->iov_base,
			mr_attr->iface,
			mr_attr->mr_iov->iov_len);
		return err;
	}

	return 0;
}
#else /* HAVE_CUDA */
static
int efa_mr_update_domain_mr_map(struct efa_mr *efa_mr, struct fi_mr_attr *mr_attr)
{
	int err;

	mr_attr->requested_key = efa_mr->mr_fid.key;
	ofi_genlock_lock(&efa_mr->domain->util_domain.lock);
	err = ofi_mr_map_insert(&efa_mr->domain->util_domain.mr_map, mr_attr,
				&efa_mr->mr_fid.key, &efa_mr->mr_fid);
	ofi_genlock_unlock(&efa_mr->domain->util_domain.lock);
	if (err) {
		EFA_WARN(FI_LOG_MR,
			"Unable to add MR to map. errno: %d errmsg: (%s) key: %ld buff: %p hmem_iface: %d len: %zu\n",
			err,
			fi_strerror(-err),
			efa_mr->mr_fid.key,
			mr_attr->mr_iov->iov_base,
			mr_attr->iface,
			mr_attr->mr_iov->iov_len);
		return err;
	}

	return 0;
}
#endif /* HAVE_CUDA */

/*
 * Set core_access to FI_SEND | FI_RECV if not already set,
 * set the fi_ibv_access modes and do real registration (ibv_mr_reg)
 * Insert the key returned by ibv_mr_reg into efa mr_map and shm mr_map
 */
static int efa_mr_reg_impl(struct efa_mr *efa_mr, uint64_t flags, void *attr)
{
	uint64_t core_access, original_access;
	struct fi_mr_attr *mr_attr = (struct fi_mr_attr *)attr;
	int fi_ibv_access = 0;
	uint64_t shm_flags;
	int ret = 0;

	efa_mr->ibv_mr = NULL;
	efa_mr->shm_mr = NULL;
	efa_mr->inserted_to_mr_map = false;
	efa_mr->mr_fid.mem_desc = NULL;
	efa_mr->mr_fid.key = FI_KEY_NOTAVAIL;

	ret = efa_mr_hmem_setup(efa_mr, mr_attr);
	if (ret)
		return ret;

	/* To support Emulated RMA path, if the access is not supported
	 * by EFA, modify it to FI_SEND | FI_RECV
	 */
	core_access = mr_attr->access;
	if (!core_access || (core_access & ~EFA_MR_SUPPORTED_PERMISSIONS))
		core_access = FI_SEND | FI_RECV;

	/* Local read access to an MR is enabled by default in verbs */
	if (core_access & FI_RECV)
		fi_ibv_access |= IBV_ACCESS_LOCAL_WRITE;

	if (efa_mr->domain->device->device_caps & EFADV_DEVICE_ATTR_CAPS_RDMA_READ)
		fi_ibv_access |= IBV_ACCESS_REMOTE_READ;

	if (efa_mr->domain->cache)
		ofi_mr_cache_flush(efa_mr->domain->cache, false);

	efa_mr->ibv_mr = efa_mr_reg_ibv_mr(efa_mr, mr_attr, fi_ibv_access);
	if (!efa_mr->ibv_mr) {
		EFA_WARN(FI_LOG_MR, "Unable to register MR: %s\n",
				fi_strerror(-errno));
		if (efa_mr->peer.iface == FI_HMEM_CUDA)
			cuda_dev_unregister(efa_mr->peer.device.cuda);

		return -errno;
	}

	efa_mr->mr_fid.mem_desc = efa_mr;
	efa_mr->mr_fid.key = efa_mr->ibv_mr->rkey;
	assert(efa_mr->mr_fid.key != FI_KEY_NOTAVAIL);

	ret = efa_mr_update_domain_mr_map(efa_mr, mr_attr);
	if (ret) {
		efa_mr_dereg_impl(efa_mr);
		return ret;
	}

	efa_mr->inserted_to_mr_map = true;

	if (efa_mr->domain->shm_domain) {
		/* We need to add FI_REMOTE_READ to allow for Read implemented
		* message protocols.
		*/
		original_access = mr_attr->access;
		mr_attr->access |= FI_REMOTE_READ;
		shm_flags = flags;
		if (mr_attr->iface != FI_HMEM_SYSTEM) {
			/* shm provider need the flag to turn on IPC support */
			shm_flags |= FI_HMEM_DEVICE_ONLY;
		}

		ret = fi_mr_regattr(efa_mr->domain->shm_domain, attr,
				    shm_flags, &efa_mr->shm_mr);
		mr_attr->access = original_access;
		if (ret) {
			EFA_WARN(FI_LOG_MR,
				"Unable to register shm MR. errno: %d err_msg: (%s) key: %ld buf: %p len: %zu\n",
				ret,
				fi_strerror(-ret),
				efa_mr->mr_fid.key,
				mr_attr->mr_iov->iov_base,
				mr_attr->mr_iov->iov_len);
			efa_mr_dereg_impl(efa_mr);
			return ret;
		}
	}

	return 0;
}

static int efa_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
			  uint64_t flags, struct fid_mr **mr_fid)
{
	struct fid_domain *domain_fid;
	struct efa_mr *efa_mr = NULL;
	uint64_t supported_flags;
	int ret = 0;

	/*
	 * Notes supported memory registration flags:
	 *
	 * OFI_MR_NOCACHE:
	 * when MR cache is enabled, application's call to fi_mr_regattr
	 * was directed to efa_mr_cache_regattr(). If OFI_MR_NOCACHE
	 * was specified, efa_mr_cache_regattr() will call this
	 * function directly (bypassing MR cache), therefore
	 * this function does not do anything special for this flag
	 * other than allow it.
	 *
	 * FI_HMEM_DEVICE_ONLY:
	 * This flag is used by some provider that need to distinguish
	 * whether a device memory can be accessed from device only, or
	 * can be access from host. EFA provider considers all device memory
	 * to be accessed by device only. Therefore, this function claim
	 * support of this flag, but do not save it in efa_mr.
	 */
	supported_flags = OFI_MR_NOCACHE | FI_HMEM_DEVICE_ONLY;
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

	if (!ofi_hmem_is_initialized(attr->iface)) {
		EFA_WARN(FI_LOG_MR,
			 "Cannot register memory for uninitialized iface\n");
		return -FI_ENOSYS;
	}

	domain_fid = container_of(fid, struct fid_domain, fid);

	efa_mr = calloc(1, sizeof(*efa_mr));
	if (!efa_mr) {
		EFA_WARN(FI_LOG_MR, "Unable to initialize md");
		return -FI_ENOMEM;
	}

	efa_mr->domain = container_of(domain_fid, struct efa_domain,
				util_domain.domain_fid);
	efa_mr->mr_fid.fid.fclass = FI_CLASS_MR;
	efa_mr->mr_fid.fid.context = attr->context;
	efa_mr->mr_fid.fid.ops = &efa_mr_ops;

	ret = efa_mr_reg_impl(efa_mr, flags, (void *)attr);
	if (ret)
		goto err;

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
	struct fi_mr_attr attr;

	attr.mr_iov = iov;
	attr.iov_count = count;
	attr.access = access;
	attr.offset = offset;
	attr.requested_key = requested_key;
	attr.context = context;
	attr.iface = FI_HMEM_SYSTEM;

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
