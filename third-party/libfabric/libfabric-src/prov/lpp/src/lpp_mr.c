/*
 * Copyright (c) 2018-2024 GigaIO, Inc. All Rights Reserved.
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

#include "lpp.h"

static int lpp_fi_mr_reg(struct fid *fid, const void *buf, size_t len,
			 uint64_t access, uint64_t offset,
			 uint64_t requested_key, uint64_t flags,
			 struct fid_mr **mr, void *context);

static int lpp_fi_mr_regv(struct fid *fid, const struct iovec * iov,
			  size_t count, uint64_t access, uint64_t offset,
			  uint64_t requested_key, uint64_t flags,
			  struct fid_mr **mr, void *context);

static int lpp_fi_mr_regattr(struct fid *fid,const struct fi_mr_attr *attr,
			     uint64_t flags, struct fid_mr **mr);

static int lpp_fi_mr_close(struct fid *fid);

static const struct fi_ops lpp_fi_mr_ops = {
	.size     = sizeof(struct fi_ops),
	.close    = lpp_fi_mr_close,
	.bind     = fi_no_bind,
	.control  = fi_no_control,
	.ops_open = fi_no_ops_open,
};

const struct fi_ops_mr lpp_mr_ops = {
	.size     = sizeof(struct fi_ops_mr),
	.reg      = lpp_fi_mr_reg,
	.regv     = lpp_fi_mr_regv,
	.regattr  = lpp_fi_mr_regattr,
};

static int lpp_fi_mr_reg(struct fid *fid, const void *buf, size_t len,
			 uint64_t access, uint64_t offset,
			 uint64_t requested_key, uint64_t flags,
			 struct fid_mr **mr, void *context)
{
	struct util_domain *util_domain;
	struct lpp_domain *lpp_domainp;
	struct lpp_mr *lpp_mrp = NULL;
	int rc = 0;
	enum fi_hmem_iface iface;

	util_domain = container_of(fid, struct util_domain, domain_fid);
	lpp_domainp = container_of(util_domain, struct lpp_domain, util_domain);

	if (!mr) {
		FI_WARN(&lpp_prov, FI_LOG_MR, "Bad mr ptr\n");
		return -FI_EINVAL;
	}

	if (fid->fclass != FI_CLASS_DOMAIN) {
		FI_WARN(&lpp_prov, FI_LOG_MR, "Bad domain\n");
		return -FI_EINVAL;
	}

	if (offset != 0ull) {
		FI_WARN(&lpp_prov, FI_LOG_MR, "offset not 0\n");
		return -FI_EINVAL;
	}

	if (flags != 0ull) {
		FI_WARN(&lpp_prov, FI_LOG_MR, "flags not 0\n");
		return -FI_EINVAL;
	}

	rc = lpp_hmem_ptr(lpp_domainp, buf, &iface);
	if (rc) {
		FI_WARN(&lpp_prov, FI_LOG_MR, "Failed to determine hmem iface\n");
		return rc;
	}

	if (iface) {
		FI_WARN(&lpp_prov, FI_LOG_MR, "Registering GPU buffers is not supported");
		return -FI_EINVAL;
	}

	rc = lpp_mr_reg_internal(lpp_domainp, buf, len, access, requested_key,
				 iface, context, &lpp_mrp);
	*mr = (rc == 0) ? &lpp_mrp->mr_fid : NULL;
	return rc;
}

static int lpp_fi_mr_regv(struct fid *fid, const struct iovec *iov,
			  size_t count, uint64_t access, uint64_t offset,
			  uint64_t requested_key, uint64_t flags,
			  struct fid_mr **mr, void *context)
{
	if (count != 1) {
		FI_WARN(&lpp_prov, FI_LOG_MR, "Only single VIO supported currently\n");
		return -EINVAL;
	}

	return lpp_fi_mr_reg(fid, iov->iov_base, iov->iov_len, access,
			     offset, requested_key, flags, mr, context);
}

static int lpp_fi_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
			     uint64_t flags, struct fid_mr **mr)
{
	if (attr->iface != FI_HMEM_SYSTEM &&
	    attr->iface != FI_HMEM_CUDA &&
	    attr->iface != FI_HMEM_ROCR) {
		FI_WARN(&lpp_prov, FI_LOG_MR,
			"Only SYSTEM, CUDA, or ROCR HMEM supported currently\n");
		return -ENOSYS;
	}

	return lpp_fi_mr_regv(fid, attr->mr_iov, attr->iov_count, attr->access,
			      attr->offset, attr->requested_key, flags, mr,
			      attr->context);
}

static int lpp_fi_mr_close(struct fid *fid)
{
	struct lpp_mr *lpp_mrp = container_of(fid, struct lpp_mr, mr_fid.fid);

	return lpp_mr_close_internal(lpp_mrp);
}

static void lpp_mr_fid(struct fid_mr *mr_fidp, void *context)
{
	if (mr_fidp != NULL) {
		mr_fidp->fid.fclass = FI_CLASS_MR;
		mr_fidp->fid.context = context;
		mr_fidp->fid.ops = (struct fi_ops *)&lpp_fi_mr_ops;

		// TODO - are there better defaults?
		mr_fidp->mem_desc = NULL;
		mr_fidp->key = 0x0ull;
	}
}

struct lpp_mr *lpp_mr_cache_find(struct lpp_domain *lpp_domainp, void *addr,
			     uint64_t len, uint64_t key)
{
	struct lpp_mr *mrp;
	struct ofi_mr_entry *entry;
	struct iovec mr_iov = {
		.iov_base = addr,
		.iov_len = len,
	};
	struct fi_mr_attr mr_attr = {
		.iov_count = 1,
		.mr_iov = &mr_iov,
	};

	entry = ofi_mr_cache_find(lpp_domainp->mr_cache, &mr_attr, 0);

	if (!entry)
		return NULL;

	mrp = (struct lpp_mr *) entry->data;

	if (mrp->mr_fid.key != key) {
		ofi_mr_cache_delete(lpp_domainp->mr_cache, entry);
		return NULL;
	}

	return mrp;
}

void lpp_mr_cache_drop(struct lpp_domain *lpp_domainp, struct lpp_mr *mrp)
{
	struct ofi_mr_entry *entry;

	entry = container_of(mrp, struct ofi_mr_entry, data);
	ofi_mr_cache_delete(lpp_domainp->mr_cache, entry);
}

int __lpp_mr_reg_internal(struct lpp_mr *lpp_mrp, struct iovec *iovp,
			  uint64_t access, enum fi_hmem_iface iface)
{
	int ret;
	int ioctl_flags = 0;

	lpp_mrp->attr.mr_iov = iovp;
	lpp_mrp->attr.iov_count = 1;
	lpp_mrp->attr.access = access;
	lpp_mrp->attr.offset = 0;
	lpp_mrp->attr.requested_key = 0;
	lpp_mrp->attr.context = NULL;
	lpp_mrp->attr.auth_key_size = sizeof(uint64_t);
	lpp_mrp->attr.auth_key = 0;
	ofi_atomic_initialize32(&lpp_mrp->ref_count, 0);

	if (iface == FI_HMEM_SYSTEM) {
		ioctl_flags |= KLPPIOCF_MR_EXPORT;
	} else if (iface == FI_HMEM_CUDA) {
		ioctl_flags |= KLPPIOCF_MR_GPU_CUDA;
	} else if (iface == FI_HMEM_ROCR) {
		ioctl_flags |= KLPPIOCF_MR_GPU_ROCM;
	} else {
		FI_WARN(&lpp_prov, FI_LOG_MR, "Unsupported hmem iface %d\n",
				iface);
		return -FI_EINVAL;
	}

	ret = klpp_mr_register(lpp_mrp, ioctl_flags);
	if (ret) {
		FI_WARN(&lpp_prov, FI_LOG_MR, "klpp_mr_register IOCTL failed\n");
		return ret;
	}

	return 0;

}

int lpp_mr_reg_internal(struct lpp_domain *lpp_domainp, const void *buf, size_t len,
			uint64_t access, uint64_t requested_key,
			enum fi_hmem_iface iface, void *context,
			struct lpp_mr **mr_ret)
{
	int ret;
	struct lpp_mr *lpp_mrp;
	struct ofi_mr_entry *entry;

	struct ofi_mr_info info = {
		.iface = iface,
		.device = -1,
		/* .flags = access, fi_mr_cache doesn't properly support access params yet
		 * https://github.com/ofiwg/libfabric/issues/5677 */
		.flags = FI_SEND | FI_RECV | FI_READ | FI_WRITE | FI_REMOTE_READ | FI_REMOTE_WRITE,
		.mapped_addr = NULL,
	};
	info.iov.iov_base = (void *) buf;
	info.iov.iov_len = len;

	ret = ofi_mr_cache_search(lpp_domainp->mr_cache, &info, &entry);

	if (ret)
		return ret;

	lpp_mrp = (struct lpp_mr *)entry->data;
	lpp_mrp->mr_fid.fid.context = context;
	lpp_mrp->attr.context = context;

	*mr_ret = lpp_mrp;

	return 0;
}

int lpp_mr_close_internal(struct lpp_mr *lpp_mrp)
{
	struct ofi_mr_entry *entry;

	entry = container_of(lpp_mrp, struct ofi_mr_entry, data);

	ofi_mr_cache_delete(lpp_mrp->domain->mr_cache, entry);
	return 0;
}

int __lpp_mr_close_internal(struct lpp_mr *lpp_mrp)
{
	int ret;

	if (ofi_atomic_get32(&lpp_mrp->ref_count) > 0)
		return -FI_EBUSY;

	ret = klpp_mr_close(lpp_mrp);
	if (ret != 0)
		FI_WARN(&lpp_prov, FI_LOG_EP_CTRL, "failed to close MR: %d\n", ret);

	return 0;
}

int lpp_mr_cache_entry_reg(struct ofi_mr_cache *cache, struct ofi_mr_entry *entry)
{
	int ret;
	struct iovec *mr_iovp = &entry->info.iov;
	struct lpp_mr *lpp_mrp = (struct lpp_mr *)entry->data;
	struct lpp_domain *lpp_domainp;
	uint64_t access = entry->info.flags;

	lpp_domainp = container_of(cache->domain, struct lpp_domain, util_domain);

	lpp_mr_fid(&lpp_mrp->mr_fid, NULL);
	lpp_mrp->mr_fid.mem_desc = lpp_mrp;

	lpp_mrp->id = -1;
	lpp_mrp->domain = lpp_domainp;
	lpp_mrp->start = (void *)mr_iovp->iov_base;
	lpp_mrp->end = (char *)mr_iovp->iov_base + mr_iovp->iov_len;

	ret = __lpp_mr_reg_internal(lpp_mrp, mr_iovp, access, entry->info.iface);

	if (ret)
		return ret;

	return 0;
}
void lpp_mr_cache_entry_dereg(struct ofi_mr_cache *cache,
			      struct ofi_mr_entry *entry)
{
	int ret;
	struct lpp_mr *lpp_mrp = (struct lpp_mr *) entry->data;

	ret = __lpp_mr_close_internal(lpp_mrp);
	if (ret)
		FI_WARN(&lpp_prov, FI_LOG_MR, "mr_close_internal failed (%d)", ret);
}
