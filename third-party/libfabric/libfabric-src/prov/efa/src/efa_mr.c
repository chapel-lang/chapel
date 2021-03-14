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

static int efa_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
			  uint64_t flags, struct fid_mr **mr_fid);
static int efa_mr_reg_impl(struct efa_mr *efa_mr, uint64_t flags, void *attr);
static int efa_mr_dereg_impl(struct efa_mr *efa_mr);

static int efa_mr_cache_close(fid_t fid)
{
	struct efa_mr *efa_mr = container_of(fid, struct efa_mr,
					       mr_fid.fid);

	ofi_mr_cache_delete(&efa_mr->domain->cache, efa_mr->entry);

	return 0;
}

static struct fi_ops efa_mr_cache_ops = {
	.size = sizeof(struct fi_ops),
	.close = efa_mr_cache_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

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
	struct fi_mr_attr attr;
	struct efa_mr *efa_mr = (struct efa_mr *)entry->data;

	efa_mr->domain = container_of(cache->domain, struct efa_domain,
					util_domain);
	efa_mr->mr_fid.fid.ops = &efa_mr_cache_ops;
	efa_mr->mr_fid.fid.fclass = FI_CLASS_MR;
	efa_mr->mr_fid.fid.context = NULL;

	attr.mr_iov = &entry->info.iov;
	attr.iov_count = 1;
	attr.access = access;
	attr.offset = 0;
	attr.requested_key = 0;
	attr.context = NULL;
	attr.iface = FI_HMEM_SYSTEM;

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
	int ret;

	if (flags & OFI_MR_NOCACHE) {
		ret = efa_mr_regattr(fid, attr, flags, mr_fid);
		return ret;
	}

	if (attr->iov_count > EFA_MR_IOV_LIMIT) {
		EFA_WARN(FI_LOG_MR, "iov count > %d not supported\n",
			 EFA_MR_IOV_LIMIT);
		return -FI_EINVAL;
	}

	domain = container_of(fid, struct efa_domain,
			      util_domain.domain_fid.fid);

	ret = ofi_mr_cache_search(&domain->cache, attr, &entry);
	if (OFI_UNLIKELY(ret))
		return ret;

	efa_mr = (struct efa_mr *)entry->data;
	efa_mr->entry = entry;

	efa_mr->peer.iface = attr->iface;
	if (attr->iface == FI_HMEM_CUDA)
		efa_mr->peer.device.cuda = attr->device.cuda;

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
	err = ofi_mr_map_remove(&efa_domain->util_domain.mr_map,
				efa_mr->mr_fid.key);
	if (err) {
		EFA_WARN(FI_LOG_MR,
			"Unable to remove MR entry from util map (%s)\n",
			fi_strerror(-ret));
		ret = err;
	}
	if (rxr_env.enable_shm_transfer && efa_mr->shm_mr) {
		err = fi_close(&efa_mr->shm_mr->fid);
		if (err) {
			EFA_WARN(FI_LOG_MR,
				"Unable to close shm MR\n");
			ret = err;
		}
	}
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

/*
 * Set core_access to FI_SEND | FI_RECV if not already set,
 * set the fi_ibv_access modes and do real registration (ibv_mr_reg)
 * Insert the key returned by ibv_mr_reg into efa mr_map and shm mr_map
 */
static int efa_mr_reg_impl(struct efa_mr *efa_mr, uint64_t flags, void *attr)
{
	uint64_t core_access;
	struct fi_mr_attr *mr_attr = (struct fi_mr_attr *)attr;
	int fi_ibv_access = 0;
	int ret = 0;

	/* To support Emulated RMA path, if the access is not supported
	 * by EFA, modify it to FI_SEND | FI_RECV
	 */
	core_access = mr_attr->access;
	if (!core_access || (core_access & ~EFA_MR_SUPPORTED_PERMISSIONS))
		core_access = FI_SEND | FI_RECV;

	/* Local read access to an MR is enabled by default in verbs */
	if (core_access & FI_RECV)
		fi_ibv_access |= IBV_ACCESS_LOCAL_WRITE;

	if (efa_mr->domain->ctx->device_caps & EFADV_DEVICE_ATTR_CAPS_RDMA_READ)
		fi_ibv_access |= IBV_ACCESS_REMOTE_READ;

	efa_mr->ibv_mr = ibv_reg_mr(efa_mr->domain->ibv_pd, 
				    (void *)mr_attr->mr_iov->iov_base,
				    mr_attr->mr_iov->iov_len, fi_ibv_access);
	if (!efa_mr->ibv_mr) {
		EFA_WARN(FI_LOG_MR, "Unable to register MR: %s\n",
				fi_strerror(-errno));
		return -errno;
	}

	efa_mr->mr_fid.mem_desc = efa_mr;
	efa_mr->mr_fid.key = efa_mr->ibv_mr->rkey;
	efa_mr->peer.iface = mr_attr->iface;
	if (mr_attr->iface == FI_HMEM_CUDA)
		efa_mr->peer.device.cuda = mr_attr->device.cuda;
	assert(efa_mr->mr_fid.key != FI_KEY_NOTAVAIL);

	mr_attr->requested_key = efa_mr->mr_fid.key;

	ret = ofi_mr_map_insert(&efa_mr->domain->util_domain.mr_map, attr,
				&efa_mr->mr_fid.key, &efa_mr->mr_fid);
	if (ret) {
		EFA_WARN(FI_LOG_MR,
			"Unable to add MR to map buf (%s): %p len: %zu\n",
			fi_strerror(-ret), mr_attr->mr_iov->iov_base,
			mr_attr->mr_iov->iov_len);
		return ret;
	}
	if (efa_mr->domain->shm_domain && rxr_env.enable_shm_transfer) {
		ret = fi_mr_regattr(efa_mr->domain->shm_domain, attr,
				    flags, &efa_mr->shm_mr);
		if (ret) {
			EFA_WARN(FI_LOG_MR,
				"Unable to register shm MR buf (%s): %p len: %zu\n",
				fi_strerror(-ret), mr_attr->mr_iov->iov_base,
				mr_attr->mr_iov->iov_len);
			fi_close(&efa_mr->mr_fid.fid);
			ofi_mr_map_remove(&efa_mr->domain->util_domain.mr_map,
						efa_mr->mr_fid.key);
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
	int ret = 0;

	if (flags && flags != OFI_MR_NOCACHE) {
		EFA_WARN(FI_LOG_MR, "Unsupported flag type. requested"
			 "[0x%" PRIx64 "] supported[0x%" PRIx64 "]\n",
			 flags, (uint64_t) OFI_MR_NOCACHE);
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
