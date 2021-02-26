/*
 * Copyright (c) 2013-2017 Intel Corporation. All rights reserved.
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

#include "psmx.h"

struct psmx_fid_mr *psmx_mr_get(struct psmx_fid_domain *domain, uint64_t key)
{
	RbtIterator it;
	struct psmx_fid_mr *mr = NULL;

	fastlock_acquire(&domain->mr_lock);
	it = rbtFind(domain->mr_map, (void *)key);
	if (!it)
		goto exit;

	rbtKeyValue(domain->mr_map, it, (void **)&key, (void **)&mr);
exit:
	fastlock_release(&domain->mr_lock);
	return mr;
}

static inline void psmx_mr_release_key(struct psmx_fid_domain *domain, uint64_t key)
{
	RbtIterator it;

	fastlock_acquire(&domain->mr_lock);
	it = rbtFind(domain->mr_map, (void *)key);
	if (it)
		rbtErase(domain->mr_map, it);
	fastlock_release(&domain->mr_lock);
}

static int psmx_mr_reserve_key(struct psmx_fid_domain *domain,
			       uint64_t requested_key,
			       uint64_t *assigned_key,
			       void *mr)
{
	uint64_t key;
	int i;
	int try_count;
	int err = -FI_ENOKEY;

	fastlock_acquire(&domain->mr_lock);

	if (domain->mr_mode == FI_MR_BASIC) {
		key = domain->mr_reserved_key;
		try_count = 10000; /* large enough */
	} else {
		key = requested_key;
		try_count = 1;
	}

	for (i=0; i<try_count; i++, key++) {
		if (!rbtFind(domain->mr_map, (void *)key)) {
			if (!rbtInsert(domain->mr_map, (void *)key, mr)) {
				if (domain->mr_mode == FI_MR_BASIC)
					domain->mr_reserved_key = key + 1;
				*assigned_key = key;
				err = 0;
			}
			break;
		}
	}

	fastlock_release(&domain->mr_lock);

	return err;
}

int psmx_mr_validate(struct psmx_fid_mr *mr, uint64_t addr, size_t len, uint64_t access)
{
	int i;

	addr += mr->offset;

	if (!addr)
		return -FI_EINVAL;

	if ((access & mr->access) != access)
		return -FI_EACCES;

	for (i = 0; i < mr->iov_count; i++) {
		if ((uint64_t)mr->iov[i].iov_base <= addr &&
		    (uint64_t)mr->iov[i].iov_base + mr->iov[i].iov_len >= addr + len)
			return 0;
	}

	return -FI_EACCES;
}

static int psmx_mr_close(fid_t fid)
{
	struct psmx_fid_mr *mr;

	mr = container_of(fid, struct psmx_fid_mr, mr.fid);
	psmx_mr_release_key(mr->domain, mr->mr.key);
	psmx_domain_release(mr->domain);
	free(mr);

	return 0;
}

static int psmx_mr_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct psmx_fid_mr *mr;
	struct psmx_fid_ep *ep;
	struct psmx_fid_cntr *cntr;

	mr = container_of(fid, struct psmx_fid_mr, mr.fid);

	if (!bfid)
		return -FI_EINVAL;
	switch (bfid->fclass) {
	case FI_CLASS_EP:
		ep = container_of(bfid, struct psmx_fid_ep, ep.fid);
		if (mr->domain != ep->domain)
			return -FI_EINVAL;
		break;

	case FI_CLASS_CNTR:
		cntr = container_of(bfid, struct psmx_fid_cntr, cntr.fid);
		if (mr->cntr && mr->cntr != cntr)
			return -FI_EBUSY;
		if (mr->domain != cntr->domain)
			return -FI_EINVAL;
		if (flags) {
			if (flags != FI_REMOTE_WRITE)
				return -FI_EINVAL;
			mr->cntr = cntr;
		}
		break;

	default:
		return -FI_ENOSYS;
	}

	return 0;
}

static struct fi_ops psmx_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = psmx_mr_close,
	.bind = psmx_mr_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static void psmx_mr_normalize_iov(struct iovec *iov, size_t *count)
{
	struct iovec tmp_iov;
	int i, j, n, new_len;
	uintptr_t iov_end_i, iov_end_j;

	n = *count;

	if (!n)
		return;

	/* sort segments by base address */
	for (i = 0; i < n - 1; i++) {
		for (j = i + 1; j < n; j++) {
			if (iov[i].iov_base > iov[j].iov_base) {
				tmp_iov = iov[i];
				iov[i] = iov[j];
				iov[j] = tmp_iov;
			}
		}
	}

	/* merge overlapping segments */
	for (i = 0; i < n - 1; i++) {
		if (iov[i].iov_len == 0)
			continue;

		for (j = i + 1; j < n; j++) {
			if (iov[j].iov_len == 0)
				continue;

			iov_end_i = (uintptr_t)iov[i].iov_base + iov[i].iov_len;
			iov_end_j = (uintptr_t)iov[j].iov_base + iov[j].iov_len;
			if (iov_end_i >= (uintptr_t)iov[j].iov_base) {
				new_len = iov_end_j - (uintptr_t)iov[i].iov_base;
				if (new_len > iov[i].iov_len)
					iov[i].iov_len = new_len;
				iov[j].iov_len = 0;
			} else {
				break;
			}
		}
	}

	/* remove empty segments */
	for (i = 0, j = 1; i < n; i++, j++) {
		if (iov[i].iov_len)
			continue;

		while (j < n && iov[j].iov_len == 0)
			j++;

		if (j >= n)
			break;

		iov[i] = iov[j];
		iov[j].iov_len = 0;
	}

	*count = i;
}

static int psmx_mr_reg(struct fid *fid, const void *buf, size_t len,
			uint64_t access, uint64_t offset, uint64_t requested_key,
			uint64_t flags, struct fid_mr **mr, void *context)
{
	struct fid_domain *domain;
	struct psmx_fid_domain *domain_priv;
	struct psmx_fid_mr *mr_priv;
	uint64_t key;
	int err;

	if (fid->fclass != FI_CLASS_DOMAIN) {
		return -FI_EINVAL;
	}
	domain = container_of(fid, struct fid_domain, fid);
	domain_priv = container_of(domain, struct psmx_fid_domain,
				   util_domain.domain_fid);

	mr_priv = (struct psmx_fid_mr *) calloc(1, sizeof(*mr_priv) + sizeof(struct iovec));
	if (!mr_priv)
		return -FI_ENOMEM;

	err = psmx_mr_reserve_key(domain_priv, requested_key, &key, mr_priv);
	if (err) {
		free(mr_priv);
		return err;
	}

	psmx_domain_acquire(domain_priv);

	mr_priv->mr.fid.fclass = FI_CLASS_MR;
	mr_priv->mr.fid.context = context;
	mr_priv->mr.fid.ops = &psmx_fi_ops;
	mr_priv->mr.mem_desc = mr_priv;
	mr_priv->mr.key = key;
	mr_priv->domain = domain_priv;
	mr_priv->access = access;
	mr_priv->flags = flags;
	mr_priv->iov_count = 1;
	mr_priv->iov[0].iov_base = (void *)buf;
	mr_priv->iov[0].iov_len = len;
	mr_priv->offset = (domain_priv->mr_mode == FI_MR_BASIC) ? 0 :
				((uint64_t)mr_priv->iov[0].iov_base - offset);

	*mr = &mr_priv->mr;
	return 0;
}

static int psmx_mr_regv(struct fid *fid,
			const struct iovec *iov, size_t count,
			uint64_t access, uint64_t offset, uint64_t requested_key,
			uint64_t flags, struct fid_mr **mr, void *context)
{
	struct fid_domain *domain;
	struct psmx_fid_domain *domain_priv;
	struct psmx_fid_mr *mr_priv;
	int i, err;
	uint64_t key;

	if (fid->fclass != FI_CLASS_DOMAIN) {
		return -FI_EINVAL;
	}
	domain = container_of(fid, struct fid_domain, fid);
	domain_priv = container_of(domain, struct psmx_fid_domain,
				   util_domain.domain_fid);

	if (count == 0 || iov == NULL)
		return -FI_EINVAL;

	mr_priv = (struct psmx_fid_mr *)
			calloc(1, sizeof(*mr_priv) +
				  sizeof(struct iovec) * count);
	if (!mr_priv)
		return -FI_ENOMEM;

	err = psmx_mr_reserve_key(domain_priv, requested_key, &key, mr_priv);
	if (err) {
		free(mr_priv);
		return err;
	}

	psmx_domain_acquire(domain_priv);

	mr_priv->mr.fid.fclass = FI_CLASS_MR;
	mr_priv->mr.fid.context = context;
	mr_priv->mr.fid.ops = &psmx_fi_ops;
	mr_priv->mr.mem_desc = mr_priv;
	mr_priv->mr.key = key;
	mr_priv->domain = domain_priv;
	mr_priv->access = access;
	mr_priv->flags = flags;
	mr_priv->iov_count = count;
	for (i=0; i<count; i++)
		mr_priv->iov[i] = iov[i];
	psmx_mr_normalize_iov(mr_priv->iov, &mr_priv->iov_count);
	mr_priv->offset = (domain_priv->mr_mode == FI_MR_BASIC) ? 0 :
				((uint64_t)mr_priv->iov[0].iov_base - offset);

	*mr = &mr_priv->mr;
	return 0;
}

static int psmx_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
			uint64_t flags, struct fid_mr **mr)
{
	struct fid_domain *domain;
	struct psmx_fid_domain *domain_priv;
	struct psmx_fid_mr *mr_priv;
	int i, err;
	uint64_t key;

	if (fid->fclass != FI_CLASS_DOMAIN) {
		return -FI_EINVAL;
	}
	domain = container_of(fid, struct fid_domain, fid);
	domain_priv = container_of(domain, struct psmx_fid_domain,
				   util_domain.domain_fid);

	if (!attr)
		return -FI_EINVAL;

	if (attr->iov_count == 0 || attr->mr_iov == NULL)
		return -FI_EINVAL;

	mr_priv = (struct psmx_fid_mr *)
			calloc(1, sizeof(*mr_priv) +
				  sizeof(struct iovec) * attr->iov_count);
	if (!mr_priv)
		return -FI_ENOMEM;

	err = psmx_mr_reserve_key(domain_priv, attr->requested_key, &key, mr_priv);
	if (err) {
		free(mr_priv);
		return err;
	}

	psmx_domain_acquire(domain_priv);

	mr_priv->mr.fid.fclass = FI_CLASS_MR;
	mr_priv->mr.fid.context = attr->context;
	mr_priv->mr.fid.ops = &psmx_fi_ops;
	mr_priv->mr.mem_desc = mr_priv;
	mr_priv->mr.key = key;
	mr_priv->domain = domain_priv;
	mr_priv->access = attr->access;
	mr_priv->flags = flags;
	mr_priv->iov_count = attr->iov_count;
	for (i=0; i<attr->iov_count; i++)
		mr_priv->iov[i] = attr->mr_iov[i];
	psmx_mr_normalize_iov(mr_priv->iov, &mr_priv->iov_count);
	mr_priv->offset = (domain_priv->mr_mode == FI_MR_SCALABLE) ? 0 :
				((uint64_t)mr_priv->iov[0].iov_base - attr->offset);

	*mr = &mr_priv->mr;
	return 0;
}

struct fi_ops_mr psmx_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = psmx_mr_reg,
	.regv = psmx_mr_regv,
	.regattr = psmx_mr_regattr,
};

