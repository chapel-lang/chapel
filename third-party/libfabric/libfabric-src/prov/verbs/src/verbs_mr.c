/*
 * Copyright (c) 2017-2019 Intel Corporation, Inc.  All rights reserved.
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include <ofi_util.h>
#include "verbs_ofi.h"

static int vrb_mr_close(fid_t fid)
{
	struct vrb_mem_desc *mr;
	int ret;

	mr = container_of(fid, struct vrb_mem_desc, mr_fid.fid);
	if (!mr->mr)
		return 0;

	ret = -ibv_dereg_mr(mr->mr);
	if (!ret)
		free(mr);
	return ret;
}

static struct fi_ops vrb_mr_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = vrb_mr_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

#if VERBS_HAVE_DMABUF_MR
static struct ibv_mr *vrb_reg_hmem_dmabuf(enum fi_hmem_iface iface,
					  struct ibv_pd *pd, const void *buf,
					  size_t len, int vrb_access)
{
	int err;
	int fd;
	uint64_t offset;
	struct ibv_mr *mr;
	int saved_errno = 0;
	enum { TRY, ALWAYS, NEVER };
	static int failover_policy[] = {
		[FI_HMEM_SYSTEM] = ALWAYS,
		[FI_HMEM_CUDA] = TRY,
		[FI_HMEM_ROCR] = TRY,
		[FI_HMEM_ZE] = TRY,
		[FI_HMEM_NEURON] = NEVER,
		[FI_HMEM_SYNAPSEAI] = NEVER,
	};

	if (failover_policy[iface] == ALWAYS)
		goto failover;

	err = ofi_hmem_get_dmabuf_fd(iface, buf, len, &fd, &offset);
	if (err)
		goto failover;

	mr = ibv_reg_dmabuf_mr(pd, offset, len, (uint64_t)buf/* iova */,
			       fd, vrb_access);
	if (!mr && failover_policy[iface] == TRY &&
	    vrb_gl_data.peer_mem_support) {
		saved_errno = errno;
		goto failover;
	}

	failover_policy[iface] = NEVER;
	return mr;

failover:
	mr = ibv_reg_mr(pd, (void *)buf, len, vrb_access);
	if (!mr) {
		if (saved_errno) {
			FI_INFO(&vrb_prov, FI_LOG_MR,
				"Failover failed: ibv_reg_mr(%p, %zd) error %d, iface %d\n",
				buf, len, errno, iface);
			errno = saved_errno;
		}
		return NULL;
	}

	if (failover_policy[iface] == TRY) {
		failover_policy[iface] = ALWAYS;
		FI_INFO(&vrb_prov, FI_LOG_MR,
			"Failover on: ibv_reg_dmabuf_mr() ==> ibv_reg_mr(), iface %d\n", iface);
	}
	return mr;
}
#endif

static int
vrb_mr_reg_common(struct vrb_mem_desc *md, int vrb_access, const void *base_addr,
		  const void *buf, size_t len, void *context, enum fi_hmem_iface iface,
		  uint64_t device, uint64_t flags)
{
	if (!ofi_hmem_is_initialized(iface)) {
		FI_WARN(&vrb_prov, FI_LOG_MR,
			"Cannot register memory for uninitialized iface\n");
		return -FI_ENOSYS;
	}
	/* ops should be set in special functions */
	md->mr_fid.fid.fclass = FI_CLASS_MR;
	md->mr_fid.fid.context = context;
	md->info.iface = iface;
	md->info.device = device;
	md->info.iov.iov_base = (void *) buf;
	md->info.iov.iov_len = len;

	if (md->domain->ext_flags & VRB_USE_ODP && iface == FI_HMEM_SYSTEM)
		vrb_access |= VRB_ACCESS_ON_DEMAND;

#if VERBS_HAVE_DMABUF_MR
	if (flags & FI_MR_DMABUF)
		md->mr = ibv_reg_dmabuf_mr(md->domain->pd, (uintptr_t) buf,
					   len, (uintptr_t) base_addr + (uintptr_t) buf,
					   (int) device, vrb_access);
	else if (vrb_gl_data.dmabuf_support &&
		 (iface == FI_HMEM_ZE ||
		  iface == FI_HMEM_SYNAPSEAI ||
		  iface == FI_HMEM_ROCR))
		md->mr = vrb_reg_hmem_dmabuf(iface, md->domain->pd, buf, len,
						vrb_access);
	else
#endif
		md->mr = ibv_reg_mr(md->domain->pd, (void *) buf, len,
				    vrb_access);
	if (!md->mr) {
		if (len)
			return -errno;
		else
			/* Ignore failure for zero length memory registration */
			assert(errno == FI_EINVAL);
	} else {
		md->mr_fid.mem_desc = md;
		md->mr_fid.key = md->mr->rkey;
		md->lkey = md->mr->lkey;
	}

	if (md->domain->eq_flags & FI_REG_MR) {
		struct fi_eq_entry entry = {
			.fid = &md->mr_fid.fid,
			.context = context,
		};
		if (md->domain->eq)
			vrb_eq_write_event(md->domain->eq, FI_MR_COMPLETE,
					   &entry, sizeof(entry));
		else if (md->domain->util_domain.eq)
			 /* This branch is taken for the verbs/DGRAM */
			fi_eq_write(&md->domain->util_domain.eq->eq_fid,
				    FI_MR_COMPLETE, &entry, sizeof(entry), 0);
	}
	return FI_SUCCESS;
}

static inline int
vrb_mr_ofi2ibv_access(uint64_t ofi_access, struct vrb_domain *domain)
{
	int ibv_access = 0;

	/* Local read access to an MR is enabled by default in verbs */
	if (ofi_access & FI_RECV)
		ibv_access |= IBV_ACCESS_LOCAL_WRITE;

	/* iWARP spec requires Remote Write access for an MR that is used
	 * as a data sink for a Remote Read */
	if (ofi_access & FI_READ) {
		ibv_access |= IBV_ACCESS_LOCAL_WRITE;
		if (domain->verbs->device->transport_type == IBV_TRANSPORT_IWARP)
			ibv_access |= IBV_ACCESS_REMOTE_WRITE;
	}

	if (ofi_access & FI_REMOTE_READ)
		ibv_access |= IBV_ACCESS_REMOTE_READ;

	/* Verbs requires Local Write access too for Remote Write access */
	if (ofi_access & FI_REMOTE_WRITE)
		ibv_access |= IBV_ACCESS_LOCAL_WRITE |
			      IBV_ACCESS_REMOTE_WRITE |
			      IBV_ACCESS_REMOTE_ATOMIC;

	return ibv_access;
}

static int
vrb_mr_nocache_reg(struct vrb_domain *domain, const void *buf, size_t len,
		   uint64_t access, void *base_addr, uint64_t offset, uint64_t requested_key,
		   uint64_t flags, struct fid_mr **mr, void *context,
		   enum fi_hmem_iface iface, uint64_t device)
{
	struct vrb_mem_desc *md;
	int vrb_access, ret;

	md = calloc(1, sizeof(*md));
	if (OFI_UNLIKELY(!md))
		return -FI_ENOMEM;

	md->domain = domain;
	md->mr_fid.fid.ops = &vrb_mr_fi_ops;

	vrb_access = vrb_mr_ofi2ibv_access(access, md->domain);
	if (flags & FI_MR_DMABUF)
		ret = vrb_mr_reg_common(md, vrb_access, base_addr, (void *) (uintptr_t) offset,
					len, context, iface, device, flags);
	else
		ret = vrb_mr_reg_common(md, vrb_access, NULL, buf, len, context,
					iface, device, flags);
	if (ret)
		goto err;

	*mr = &md->mr_fid;
	return FI_SUCCESS;
err:
	free(md);
	return ret;
}

#if VERBS_HAVE_DMABUF_MR

static int
vrb_reg_dmabuf(struct vrb_domain *domain, const struct fi_mr_attr *attr,
	       uint64_t flags, struct fid_mr **mr)
{
	int ret;

	if (!vrb_gl_data.dmabuf_support)
		return -FI_ENOSYS;

	/* Skip trying to cache the MR.  We don't have a mechanism
	 * to monitor or verify if the region is invalidated.
	 */
	ret = vrb_mr_nocache_reg(domain, NULL, attr->dmabuf->len, attr->access,
				 attr->dmabuf->base_addr, attr->dmabuf->offset,
				 attr->requested_key, flags, mr, attr->context,
				 attr->iface, (uint64_t) attr->dmabuf->fd);

	return ret;
}

#else /* VERBS_HAVE_DMABUF_MR */

#define vrb_reg_dmabuf(domain, attr, flags, mr) -FI_ENOSYS

#endif

static int vrb_mr_cache_close(fid_t fid)
{
	struct vrb_mem_desc *md =
		container_of(fid, struct vrb_mem_desc, mr_fid.fid);
	ofi_mr_cache_delete(&md->domain->cache, md->entry);
	return FI_SUCCESS;
}

static struct fi_ops vrb_mr_cache_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = vrb_mr_cache_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

int vrb_mr_cache_add_region(struct ofi_mr_cache *cache,
			    struct ofi_mr_entry *entry)
{
	struct vrb_mem_desc *md = (struct vrb_mem_desc *) entry->data;

	md->domain = container_of(cache->domain, struct vrb_domain, util_domain);
	md->mr_fid.fid.ops = &vrb_mr_cache_fi_ops;
	md->entry = entry;

	return vrb_mr_reg_common(md, IBV_ACCESS_LOCAL_WRITE |
			IBV_ACCESS_REMOTE_WRITE | IBV_ACCESS_REMOTE_ATOMIC |
			IBV_ACCESS_REMOTE_READ, NULL, entry->info.iov.iov_base,
			entry->info.iov.iov_len, NULL, entry->info.iface,
			entry->info.device, 0);
}

void vrb_mr_cache_delete_region(struct ofi_mr_cache *cache,
				   struct ofi_mr_entry *entry)
{
	struct vrb_mem_desc *md = (struct vrb_mem_desc *)entry->data;
	if (md->mr)
		(void)ibv_dereg_mr(md->mr);
}

static int
vrb_mr_cache_reg(struct vrb_domain *domain, const void *buf, size_t len,
		 uint64_t access, void *base_addr, uint64_t offset, uint64_t requested_key,
		 uint64_t flags, struct fid_mr **mr, void *context,
		 enum fi_hmem_iface iface, uint64_t device)
{
	struct vrb_mem_desc *md;
	struct ofi_mr_entry *entry;
	struct fi_mr_attr attr;
	struct ofi_mr_info info = {0};
	struct iovec iov;
	int ret;

	attr.access = access;
	attr.context = context;
	attr.iov_count = 1;
	iov.iov_base = (void *) buf;
	iov.iov_len = len;
	attr.mr_iov = &iov;
	attr.offset = offset;
	attr.requested_key = requested_key;
	attr.auth_key_size = 0;
	attr.iface = iface;
	attr.device.reserved = device;
	assert(attr.iov_count == 1);
	info.iov = iov;
	info.iface = iface;
	info.device = device;

	ret = (flags & OFI_MR_NOCACHE) ?
	      ofi_mr_cache_reg(&domain->cache, &attr, &entry, flags) :
	      ofi_mr_cache_search(&domain->cache, &info, &entry);
	if (OFI_UNLIKELY(ret))
		return ret;

	md = (struct vrb_mem_desc *) entry->data;
	*mr = &md->mr_fid;
	return FI_SUCCESS;
}

static int
vrb_mr_reg_iface(struct fid *fid, const void *buf, size_t len, uint64_t access,
		 uint64_t offset, uint64_t requested_key, uint64_t flags,
		 struct fid_mr **mr, void *context, enum fi_hmem_iface iface,
		 uint64_t device)
{
	struct vrb_domain *domain;

	if (flags & FI_MR_DMABUF)
		return -FI_EINVAL;

	domain = container_of(fid, struct vrb_domain,
			      util_domain.domain_fid.fid);

	if (domain->cache.monitors[iface])
		return vrb_mr_cache_reg(domain, buf, len, access, NULL, offset,
					requested_key, flags, mr, context,
					iface, device);
	else
		return vrb_mr_nocache_reg(domain, buf, len, access, NULL, offset,
					  requested_key, flags, mr, context,
					  iface, device);
}

static int
vrb_mr_regv_iface(struct fid *fid, const struct iovec *iov, size_t count,
		  uint64_t access, uint64_t offset, uint64_t requested_key,
		  uint64_t flags, struct fid_mr **mr, void *context,
		  enum fi_hmem_iface iface, uint64_t device)
{
	const void *addr = count ? iov->iov_base: NULL;
	size_t len = count ? iov->iov_len : 0;

	if (OFI_UNLIKELY(count > 1))
		return -FI_EINVAL;

	return vrb_mr_reg_iface(fid, addr, len, access, offset, requested_key,
				flags, mr, context, iface, device);
}

static int
vrb_mr_reg(struct fid *fid, const void *buf, size_t len, uint64_t access,
	   uint64_t offset, uint64_t requested_key, uint64_t flags,
	   struct fid_mr **mr, void *context)
{
	return vrb_mr_reg_iface(fid, buf, len, access, offset, requested_key,
				flags, mr, context, FI_HMEM_SYSTEM, 0);
}

static int
vrb_mr_regv(struct fid *fid, const struct iovec *iov, size_t count,
	    uint64_t access, uint64_t offset, uint64_t requested_key,
	    uint64_t flags, struct fid_mr **mr, void *context)
{
	return vrb_mr_regv_iface(fid, iov, count, access, offset, requested_key,
				 flags, mr, context, FI_HMEM_SYSTEM, 0);
}

static int vrb_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
			  uint64_t flags, struct fid_mr **mr)
{
	struct vrb_domain *domain;
	struct fi_mr_attr cur_abi_attr;

	domain = container_of(fid, struct vrb_domain,
			      util_domain.domain_fid.fid);

	ofi_mr_update_attr(domain->util_domain.fabric->fabric_fid.api_version,
			   domain->util_domain.info_domain_caps, attr,
			   &cur_abi_attr, flags);

	if (flags & FI_MR_DMABUF)
		return vrb_reg_dmabuf(domain, &cur_abi_attr, flags, mr);

	if ((flags & FI_HMEM_HOST_ALLOC) && (cur_abi_attr.iface == FI_HMEM_ZE))
		cur_abi_attr.device.ze = -1;

	return vrb_mr_regv_iface(fid, cur_abi_attr.mr_iov,
				 cur_abi_attr.iov_count, cur_abi_attr.access,
				 cur_abi_attr.offset,
				 cur_abi_attr.requested_key, flags, mr,
				 cur_abi_attr.context, cur_abi_attr.iface,
				 cur_abi_attr.device.reserved);
}

struct fi_ops_mr vrb_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = vrb_mr_reg,
	.regv = vrb_mr_regv,
	.regattr = vrb_mr_regattr,
};
