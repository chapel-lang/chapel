/*
 * Copyright (c) 2017-2019 Intel Corporation, Inc.  All rights reserved.
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
#include "fi_verbs.h"


static int
vrb_mr_regv(struct fid *fid, const struct iovec *iov,
	       size_t count, uint64_t access, uint64_t offset,
	       uint64_t requested_key, uint64_t flags,
	       struct fid_mr **mr, void *context)
{
	struct fid_domain *domain = container_of(fid, struct fid_domain, fid);

	if (OFI_UNLIKELY(count > 1))
		return -FI_EINVAL;

	return count ? fi_mr_reg(domain, (const void *) iov->iov_base,
				 iov->iov_len, access, offset, requested_key,
				 flags, mr, context) :
		       fi_mr_reg(domain, NULL, 0, access, offset, requested_key,
				 flags, mr, context);
}

static int vrb_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
			     uint64_t flags, struct fid_mr **mr)
{
	return vrb_mr_regv(fid, attr->mr_iov, attr->iov_count, attr->access,
			      attr->offset, attr->requested_key, flags, mr,
			      attr->context);
}

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

static inline
int vrb_mr_reg_common(struct vrb_mem_desc *md, int vrb_access,
			 const void *buf, size_t len, void *context)
{
	/* ops should be set in special functions */
	md->mr_fid.fid.fclass = FI_CLASS_MR;
	md->mr_fid.fid.context = context;

	if (md->domain->flags & VRB_USE_ODP)
		vrb_access |= VRB_ACCESS_ON_DEMAND;

	md->mr = ibv_reg_mr(md->domain->pd, (void *) buf, len, vrb_access);
	if (!md->mr) {
		if (len)
			return -errno;
		else
			/* Ignore failure for zero length memory registration */
			assert(errno == FI_EINVAL);
	} else {
		md->mr_fid.mem_desc = (void *)(uintptr_t)md->mr->lkey;
		md->mr_fid.key = md->mr->rkey;
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

	if (ofi_access & FI_WRITE)
		ibv_access |= IBV_ACCESS_LOCAL_WRITE;

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
vrb_mr_reg(struct fid *fid, const void *buf, size_t len,
	      uint64_t access, uint64_t offset, uint64_t requested_key,
	      uint64_t flags, struct fid_mr **mr, void *context)
{
	struct vrb_mem_desc *md;
	int ret;

	if (OFI_UNLIKELY(flags & ~OFI_MR_NOCACHE))
		return -FI_EBADFLAGS;

	md = calloc(1, sizeof(*md));
	if (OFI_UNLIKELY(!md))
		return -FI_ENOMEM;

	md->domain = container_of(fid, struct vrb_domain,
				  util_domain.domain_fid.fid);
	md->mr_fid.fid.ops = &vrb_mr_fi_ops;

	ret = vrb_mr_reg_common(md, vrb_mr_ofi2ibv_access(access, md->domain),
				   buf, len, context);
	if (OFI_UNLIKELY(ret))
		goto err;

	*mr = &md->mr_fid;
	return FI_SUCCESS;
err:
	free(md);
	return ret;
}

static int vrb_mr_cache_close(fid_t fid)
{
	struct vrb_mem_desc *md =
		container_of(fid, struct vrb_mem_desc, mr_fid.fid);
	
	ofi_mr_cache_delete(&md->domain->cache, md->entry);
	return FI_SUCCESS;
}

struct fi_ops_mr vrb_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = vrb_mr_reg,
	.regv = vrb_mr_regv,
	.regattr = vrb_mr_regattr,
};

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
			IBV_ACCESS_REMOTE_READ, entry->info.iov.iov_base,
			entry->info.iov.iov_len, NULL);
}

void vrb_mr_cache_delete_region(struct ofi_mr_cache *cache,
				   struct ofi_mr_entry *entry)
{
	struct vrb_mem_desc *md = (struct vrb_mem_desc *)entry->data;
	if (md->mr)
		(void)ibv_dereg_mr(md->mr);
}

static int
vrb_mr_cache_reg(struct fid *fid, const void *buf, size_t len,
		    uint64_t access, uint64_t offset, uint64_t requested_key,
		    uint64_t flags, struct fid_mr **mr, void *context)
{
	struct vrb_domain *domain;
	struct vrb_mem_desc *md;
	struct ofi_mr_entry *entry;
	struct fi_mr_attr attr;
	struct iovec iov;
	int ret;

	if (flags & ~OFI_MR_NOCACHE)
		return -FI_EBADFLAGS;

	domain = container_of(fid, struct vrb_domain,
			      util_domain.domain_fid.fid);

	attr.access = access;
	attr.context = context;
	attr.iov_count = 1;
	iov.iov_base = (void *) buf;
	iov.iov_len = len;
	attr.mr_iov = &iov;
	attr.offset = offset;
	attr.requested_key = requested_key;
	attr.auth_key_size = 0;

	ret = (flags & OFI_MR_NOCACHE) ?
	      ofi_mr_cache_reg(&domain->cache, &attr, &entry) :
	      ofi_mr_cache_search(&domain->cache, &attr, &entry);
	if (OFI_UNLIKELY(ret))
		return ret;

	md = (struct vrb_mem_desc *) entry->data;
	*mr = &md->mr_fid;
	return FI_SUCCESS;
}

struct fi_ops_mr vrb_mr_cache_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = vrb_mr_cache_reg,
	.regv = vrb_mr_regv,
	.regattr = vrb_mr_regattr,
};
