/*
 * Copyright (C) 2016 by Argonne National Laboratory.
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
#include "rdma/bgq/fi_bgq.h"
#include <ofi_enosys.h>

static int fi_bgq_close_mr(fid_t fid)
{
	struct fi_bgq_domain *bgq_domain;
	struct fi_bgq_mr *bgq_mr = (struct fi_bgq_mr *) fid;

	bgq_domain = bgq_mr->domain;

	if (FI_BGQ_FABRIC_DIRECT_MR == FI_MR_SCALABLE) {
	int ret;
	fi_bgq_domain_bat_clear(bgq_domain, bgq_mr->mr_fid.key);

	ret = fi_bgq_ref_dec(&bgq_domain->ref_cnt, "domain");
	if (ret) return ret;
	}
	free(bgq_mr);
	return 0;
}

static int fi_bgq_bind_mr(struct fid *fid,
		struct fid *bfid, uint64_t flags)
{
	int ret;
	struct fi_bgq_mr *bgq_mr =
		(struct fi_bgq_mr *) fid;
	struct fi_bgq_cntr *bgq_cntr;

	ret = fi_bgq_fid_check(fid, FI_CLASS_MR, "memory region");
	if (ret)
		return ret;

	switch (bfid->fclass) {
	case FI_CLASS_CNTR:
		bgq_cntr = (struct fi_bgq_cntr *) bfid;
		bgq_mr->cntr = bgq_cntr;
		bgq_mr->cntr_bflags = flags;
		break;
	default:
		errno = FI_ENOSYS;
		return -errno;
	}
	return 0;
}

static struct fi_ops fi_bgq_fi_ops = {
	.size		= sizeof(struct fi_ops),
	.close		= fi_bgq_close_mr,
	.bind		= fi_bgq_bind_mr,
	.control	= fi_no_control,
	.ops_open	= fi_no_ops_open
};

static int fi_bgq_mr_reg(struct fid *fid, const void *buf,
		size_t len, uint64_t access, uint64_t offset,
		uint64_t requested_key, uint64_t flags,
		struct fid_mr **mr, void *context)
{
	int ret;

	struct fi_bgq_mr *bgq_mr;
	struct fi_bgq_domain *bgq_domain;

	if (!fid || !mr) {
		errno = FI_EINVAL;
		return -errno;
	}

	ret = fi_bgq_fid_check(fid, FI_CLASS_DOMAIN, "domain");
	if (ret) return ret;

	if (flags != 0) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_MR,
				"Flags for fi_mr_reg must be 0\n");
		errno = FI_EINVAL;
		return -errno;
	}

	bgq_domain = (struct fi_bgq_domain *) container_of(fid, struct fid_domain, fid);

	if (FI_BGQ_FABRIC_DIRECT_MR == FI_MR_SCALABLE) {
		if (requested_key >= bgq_domain->num_mr_keys) {
			/* requested key is too large */
			errno = FI_EKEYREJECTED;
			return -errno;
		}
	}
	bgq_mr = calloc(1, sizeof(*bgq_mr));
	if (!bgq_mr) {
		errno = FI_ENOMEM;
		return -errno;
	}

	bgq_mr->mr_fid.fid.fclass	= FI_CLASS_MR;
	bgq_mr->mr_fid.fid.context	= context;
	bgq_mr->mr_fid.fid.ops		= &fi_bgq_fi_ops;
	if (FI_BGQ_FABRIC_DIRECT_MR == FI_MR_SCALABLE) {
		bgq_mr->mr_fid.key		= requested_key;
	}
	else if (FI_BGQ_FABRIC_DIRECT_MR == FI_MR_BASIC) {

		uint64_t paddr = 0;

		fi_bgq_cnk_vaddr2paddr(buf,1,&paddr);
		bgq_mr->mr_fid.key		= ((uint64_t)buf - paddr);
#ifdef FI_BGQ_TRACE
        fprintf(stderr,"fi_bgq_mr_reg - FI_MR_BASIC virtual addr is 0x%016lx physical addr is 0x%016lx key is %lu  \n",(uint64_t)buf,paddr,(uint64_t)((uint64_t)buf - paddr));
fflush(stderr);

#endif

	}
	bgq_mr->buf 	= buf;
	bgq_mr->len	= len;
	bgq_mr->offset	= offset;
	bgq_mr->access	= FI_SEND | FI_RECV | FI_READ | FI_WRITE | FI_REMOTE_READ | FI_REMOTE_WRITE;
	bgq_mr->flags	= flags;
	bgq_mr->domain  = bgq_domain;

	if (FI_BGQ_FABRIC_DIRECT_MR == FI_MR_SCALABLE) {
		fi_bgq_domain_bat_write(bgq_domain, requested_key, buf, len);

		fi_bgq_ref_inc(&bgq_domain->ref_cnt, "domain");
	}

	*mr = &bgq_mr->mr_fid;

	return 0;
}

int fi_bgq_bind_ep_mr(struct fi_bgq_ep *bgq_ep,
		struct fi_bgq_mr *bgq_mr, uint64_t flags)
{
	return 0;
}

static struct fi_ops_mr fi_bgq_mr_ops = {
	.size		= sizeof(struct fi_ops_mr),
	.reg 		= fi_bgq_mr_reg,
	.regv 		= fi_no_mr_regv,
	.regattr	= fi_no_mr_regattr
};

int fi_bgq_init_mr_ops(struct fi_bgq_domain *bgq_domain, struct fi_info *info)
{
	if (!bgq_domain || !info) {
		goto err;
	}

	if (info->domain_attr->mr_mode == FI_MR_UNSPEC) goto err; 

	bgq_domain->domain_fid.mr	   = &fi_bgq_mr_ops;

	bgq_domain->mr_mode = info->domain_attr->mr_mode;

	if (FI_BGQ_FABRIC_DIRECT_MR == FI_MR_SCALABLE) {
	bgq_domain->num_mr_keys = (1<<(8*info->domain_attr->mr_key_size));
	bgq_domain->bat = (struct fi_bgq_bat_entry *) calloc(bgq_domain->num_mr_keys, sizeof(struct fi_bgq_bat_entry));

	}
	return 0;
err:
	errno = FI_EINVAL;
	return -errno;
}

int fi_bgq_finalize_mr_ops(struct fi_bgq_domain *bgq_domain)
{
	if (FI_BGQ_FABRIC_DIRECT_MR == FI_MR_SCALABLE) {
	free((void*)bgq_domain->bat);
	bgq_domain->bat = (void*)NULL;
	bgq_domain->num_mr_keys = 0;
	}
	return 0;
}
