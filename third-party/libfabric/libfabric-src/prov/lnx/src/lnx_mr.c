/*
 * Copyright (c) 2025 ORNL. All rights reserved.
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#include <rdma/fi_errno.h>
#include "ofi_util.h"
#include "ofi.h"
#include "ofi_str.h"
#include "ofi_prov.h"
#include "ofi_perf.h"
#include "ofi_hmem.h"
#include "rdma/fi_ext.h"
#include "lnx.h"

/*
 * 1. On MR registration we store the information passed in the fi_mr_attr
 * 2. If we get a bind, we flag it and store the flags and fid pointer
 * 3. If we get a control command we flag it and store the command
 * 4. When a data operation we're passed the descriptor, using that we can
 * find out the information we stored, and since we already know the
 * target core provider we can do memory registration at that point
 */

int lnx_mr_regattr_core(struct lnx_core_domain *cd, void *desc, void **core_desc)
{
	int rc;
	struct lnx_mr *lm;

	lm = (struct lnx_mr *)desc;
	if (lm->lm_core_mr)
		goto out;

	rc = fi_mr_regattr(cd->cd_domain, &lm->lm_attr, lm->lm_mr.flags,
			   &lm->lm_core_mr);
	if (rc)
		return rc;

out:
	*core_desc = lm->lm_core_mr->mem_desc;
	return FI_SUCCESS;
}

static int lnx_mr_close(struct fid *fid)
{
	int rc, frc = FI_SUCCESS;
	struct lnx_mr *lm;

	lm = container_of(fid, struct lnx_mr, lm_mr.mr_fid.fid);

	if (lm->lm_core_mr) {
		rc = fi_close(&lm->lm_core_mr->fid);
		if (rc)
			frc = rc;
	}

	ofi_atomic_dec32(&lm->lm_mr.domain->ref);

	ofi_buf_free(lm);

	return frc;
}

static int lnx_mr_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	return -FI_ENOSYS;
}

static int lnx_mr_control(struct fid *fid, int command, void *arg)
{
	return -FI_ENOSYS;
}

static struct fi_ops lnx_mr_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = lnx_mr_close,
	.bind = lnx_mr_bind,
	.control = lnx_mr_control,
	.ops_open = fi_no_ops_open
};

int lnx_mr_regattr(struct fid *fid, const struct fi_mr_attr *attr,
		   uint64_t flags, struct fid_mr **mr_fid)
{
	struct lnx_domain *domain;
	struct ofi_mr *mr;
	struct lnx_mr *lm = NULL;

	if (fid->fclass != FI_CLASS_DOMAIN || !attr ||
	    attr->iov_count <= 0 || attr->iov_count > LNX_IOV_LIMIT)
		return -FI_EINVAL;

	domain = container_of(fid, struct lnx_domain, ld_domain.domain_fid.fid);

	lm = ofi_buf_alloc(domain->ld_mem_reg_bp);
	if (!lm)
		return -FI_ENOMEM;

	memset(lm, 0, sizeof(*lm));

	lm->lm_attr = *attr;
	memcpy(lm->lm_iov, attr->mr_iov, sizeof(struct iovec) * attr->iov_count);
	lm->lm_attr.mr_iov = lm->lm_iov;
	mr = &lm->lm_mr;
	mr->mr_fid.fid.fclass = FI_CLASS_MR;
	mr->mr_fid.fid.ops = &lnx_mr_fi_ops;
	mr->mr_fid.mem_desc = lm;
	mr->domain = &domain->ld_domain;
	mr->flags = flags | FI_HMEM_DEVICE_ONLY;

	*mr_fid = &mr->mr_fid;
	ofi_atomic_inc32(&domain->ld_domain.ref);

	return FI_SUCCESS;
}


