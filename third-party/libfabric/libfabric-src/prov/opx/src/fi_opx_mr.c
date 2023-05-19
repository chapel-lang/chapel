/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021-2023 Cornelis Networks.
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
#include <ofi.h>

#include "rdma/opx/fi_opx_domain.h"
#include "rdma/opx/fi_opx.h"
#include "rdma/opx/fi_opx_internal.h"

#include <ofi_enosys.h>

static int fi_opx_close_mr(fid_t fid)
{
	struct fi_opx_domain *opx_domain;
	struct fi_opx_mr *opx_mr = (struct fi_opx_mr *) fid;

	opx_domain = opx_mr->domain;

	HASH_DEL(opx_domain->mr_hashmap, opx_mr);

	if (opx_domain->mr_mode == FI_MR_SCALABLE) {
		int ret;

		ret = fi_opx_ref_dec(&opx_domain->ref_cnt, "domain");
		if (ret) return ret;
	}
	free(opx_mr);
	opx_mr = NULL;
	//opx_mr (the object passed in as fid) is now unusable
	return 0;
}

static int fi_opx_bind_mr(struct fid *fid,
		struct fid *bfid, uint64_t flags)
{
	int ret;
	struct fi_opx_mr *opx_mr =
		(struct fi_opx_mr *) fid;
	struct fi_opx_cntr *opx_cntr;

	ret = fi_opx_fid_check(fid, FI_CLASS_MR, "memory region");
	if (ret)
		return ret;

	switch (bfid->fclass) {
	case FI_CLASS_CNTR:
		opx_cntr = (struct fi_opx_cntr *) bfid;
		opx_mr->cntr = opx_cntr;
		opx_mr->cntr_bflags = flags;
		break;
	default:
		errno = FI_ENOSYS;
		return -errno;
	}
	return 0;
}

static struct fi_ops fi_opx_fi_ops = {
	.size		= sizeof(struct fi_ops),
	.close		= fi_opx_close_mr,
	.bind		= fi_opx_bind_mr,
	.control	= fi_no_control,
	.ops_open	= fi_no_ops_open
};

static int fi_opx_mr_reg(struct fid *fid, const void *buf,
		size_t len, uint64_t access, uint64_t offset,
		uint64_t requested_key, uint64_t flags,
		struct fid_mr **mr, void *context)
{
	int ret;

	struct fi_opx_mr *opx_mr;
	struct fi_opx_domain *opx_domain;

	if (!fid || !mr) {
		errno = FI_EINVAL;
		return -errno;
	}

	ret = fi_opx_fid_check(fid, FI_CLASS_DOMAIN, "domain");
	if (ret) return ret;

	FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_MR,
			"buf=%p, len=%lu, access=%lu, offset=%lu, requested_key=%lu, flags=%lu, context=%p\n", 
			buf, len, access, offset, requested_key, flags, context);

	opx_domain = (struct fi_opx_domain *) container_of(fid, struct fid_domain, fid);

	if (opx_domain->mr_mode == FI_MR_SCALABLE) {
		if (requested_key >= opx_domain->num_mr_keys) {
			/* requested key is too large */
			errno = FI_EKEYREJECTED;
			return -errno;
		}
	}

	if ( (access & (FI_SEND | FI_RECV | FI_READ | FI_WRITE | FI_REMOTE_READ | FI_REMOTE_WRITE)) != access ) {
		FI_WARN(fi_opx_global.prov, FI_LOG_MR,
		"Unsupported access mask specified, client requested %lu\n", access);
		errno = FI_EINVAL;
		return -errno;
	}

	if ( (flags & (FI_MR_LOCAL | FI_MR_RAW | FI_MR_VIRT_ADDR | FI_MR_ALLOCATED |
		FI_MR_PROV_KEY | FI_MR_MMU_NOTIFY | FI_MR_RMA_EVENT | FI_MR_ENDPOINT | FI_MR_HMEM )) != flags ) {

		FI_WARN(fi_opx_global.prov, FI_LOG_MR,
		"Unsupported flags specified, client requested %lu\n", flags);
		errno = FI_EINVAL;
		return -errno;
	}

	opx_mr = calloc(1, sizeof(*opx_mr));
	if (!opx_mr) {
		errno = FI_ENOMEM;
		return -errno;
	}

	opx_mr->mr_fid.mem_desc 	= opx_mr;
	opx_mr->mr_fid.fid.fclass	= FI_CLASS_MR;
	opx_mr->mr_fid.fid.context	= context;
	opx_mr->mr_fid.fid.ops		= &fi_opx_fi_ops;
	if (opx_domain->mr_mode == FI_MR_SCALABLE) {
		opx_mr->mr_fid.key		= requested_key;
	}

	opx_mr->buf 	= buf;
	opx_mr->len 	= len;
	opx_mr->offset	= offset;
	opx_mr->access 	= access;
	opx_mr->flags	= flags;
	opx_mr->domain  = opx_domain;

	if (opx_domain->mr_mode == FI_MR_SCALABLE) {
		fi_opx_ref_inc(&opx_domain->ref_cnt, "domain");
	}
	HASH_ADD(hh, opx_domain->mr_hashmap,
			 mr_fid.key, sizeof(opx_mr->mr_fid.key),
			 opx_mr);

	*mr = &opx_mr->mr_fid;

	return 0;
}

static int fi_opx_mr_regv(struct fid *fid,
                        const struct iovec *iov, size_t count,
                        uint64_t access, uint64_t offset,
                        uint64_t requested_key, uint64_t flags,
                        struct fid_mr **mr, void *context)
{
	if(iov) {
		if(count == 1) {
			const void *buf = iov[0].iov_base;
			size_t len = iov[0].iov_len;
			return fi_opx_mr_reg(fid, buf, len, access, offset, requested_key, flags, mr, context);
		}
		FI_WARN(fi_opx_global.prov, FI_LOG_MR, "Unsupported iov count %lu\n", count);
	}
	errno = FI_EINVAL;
	return -errno;
}

static int fi_opx_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
							uint64_t flags, struct fid_mr **mr)
{
	if (!attr) {
		errno = FI_EINVAL;
		return -errno;
	}
    return fi_opx_mr_regv(fid, attr->mr_iov, attr->iov_count, attr->access,
						attr->offset, attr->requested_key, flags, mr, attr->context);
}

int fi_opx_bind_ep_mr(struct fid_ep *ep,
		struct fid_mr *mr, uint64_t flags)
{
	return 0;
}

static struct fi_ops_mr fi_opx_mr_ops = {
	.size		= sizeof(struct fi_ops_mr),
	.reg 		= fi_opx_mr_reg,
	.regv 		= fi_opx_mr_regv,
	.regattr	= fi_opx_mr_regattr
};

int fi_opx_init_mr_ops(struct fid_domain *domain, struct fi_info *info)
{
	if (!domain || !info) {
		goto err;
	}

	struct fi_opx_domain *opx_domain =
		container_of(domain, struct fi_opx_domain, domain_fid);

	if (info->domain_attr->mr_mode == FI_MR_UNSPEC) goto err; 

	opx_domain->domain_fid.mr	   = &fi_opx_mr_ops;

	opx_domain->mr_mode = info->domain_attr->mr_mode;

	if (opx_domain->mr_mode == FI_MR_SCALABLE) {
            opx_domain->num_mr_keys = UINT64_MAX;
	}
	return 0;
err:
	errno = FI_EINVAL;
	return -errno;
}

int fi_opx_finalize_mr_ops(struct fid_domain *domain)
{
	struct fi_opx_domain *opx_domain =
		container_of(domain, struct fi_opx_domain, domain_fid);

	if (opx_domain->mr_mode == FI_MR_SCALABLE) {
		opx_domain->num_mr_keys = 0;
	}
	return 0;
}
