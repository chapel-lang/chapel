/*
* Copyright (c) 2017 Intel Corporation, Inc.  All rights reserved.
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

#include <stdlib.h>
#include <string.h>

#include <ofi_util.h>

#include "sock.h"
#include "sock_util.h"

#define SOCK_LOG_DBG(...) _SOCK_LOG_DBG(FI_LOG_MR, __VA_ARGS__)
#define SOCK_LOG_ERROR(...) _SOCK_LOG_ERROR(FI_LOG_MR, __VA_ARGS__)

static int sock_mr_close(struct fid *fid)
{
	struct sock_domain *dom;
	struct sock_mr *mr;
	int err = 0;

	mr = container_of(fid, struct sock_mr, mr_fid.fid);
	dom = mr->domain;

	ofi_mutex_lock(&dom->lock);
	err = ofi_mr_map_remove(&dom->mr_map, mr->key);
	if (err != 0)
		SOCK_LOG_ERROR("MR Erase error %d \n", err);

	ofi_mutex_unlock(&dom->lock);
	ofi_atomic_dec32(&dom->ref);
	free(mr);
	return 0;
}

static int sock_mr_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	struct sock_cntr *cntr;
	struct sock_cq *cq;
	struct sock_mr *mr;

	mr = container_of(fid, struct sock_mr, mr_fid.fid);
	switch (bfid->fclass) {
	case FI_CLASS_CQ:
		cq = container_of(bfid, struct sock_cq, cq_fid.fid);
		if (mr->domain != cq->domain)
			return -FI_EINVAL;

		if (flags & FI_REMOTE_WRITE)
			mr->cq = cq;
		break;

	case FI_CLASS_CNTR:
		cntr = container_of(bfid, struct sock_cntr, cntr_fid.fid);
		if (mr->domain != cntr->domain)
			return -FI_EINVAL;

		if (flags & FI_REMOTE_WRITE)
			mr->cntr = cntr;
		break;

	default:
		return -FI_EINVAL;
	}
	return 0;
}

static struct fi_ops sock_mr_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = sock_mr_close,
	.bind = sock_mr_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

struct sock_mr *sock_mr_verify_key(struct sock_domain *domain, uint64_t key,
	uintptr_t *buf, size_t len, uint64_t access)
{
	int err = 0;
	struct sock_mr *mr;

	ofi_mutex_lock(&domain->lock);

	err = ofi_mr_map_verify(&domain->mr_map, buf, len, key, access, (void **)&mr);
	if (err != 0) {
		SOCK_LOG_ERROR("MR check failed\n");
		mr = NULL;
	}

	ofi_mutex_unlock(&domain->lock);
	return mr;
}

struct sock_mr *sock_mr_verify_desc(struct sock_domain *domain, void *desc,
	void *buf, size_t len, uint64_t access)
{
	uint64_t key = (uintptr_t)desc;
	return sock_mr_verify_key(domain, key, buf, len, access);
}

static int sock_regattr(struct fid *fid, const struct fi_mr_attr *attr,
	uint64_t flags, struct fid_mr **mr)
{
	struct fi_eq_entry eq_entry;
	struct sock_domain *dom;
	struct fi_mr_attr cur_abi_attr;
	struct sock_mr *_mr;
	uint64_t key;
	struct fid_domain *domain;
	int ret = 0;

	if (fid->fclass != FI_CLASS_DOMAIN || !attr || attr->iov_count <= 0) {
		return -FI_EINVAL;
	}

	domain = container_of(fid, struct fid_domain, fid);
	dom = container_of(domain, struct sock_domain, dom_fid);

	_mr = calloc(1, sizeof(*_mr));
	if (!_mr)
		return -FI_ENOMEM;

	ofi_mr_update_attr(dom->fab->fab_fid.api_version, dom->info.caps,
			   attr, &cur_abi_attr, flags);
	ofi_mutex_lock(&dom->lock);

	_mr->mr_fid.fid.fclass = FI_CLASS_MR;
	_mr->mr_fid.fid.context = attr->context;
	_mr->mr_fid.fid.ops = &sock_mr_fi_ops;

	_mr->domain = dom;
	_mr->flags = flags;

	ret = ofi_mr_map_insert(&dom->mr_map, &cur_abi_attr, &key, _mr, flags);
	if (ret != 0)
		goto err;

	_mr->mr_fid.key = _mr->key = key;
	_mr->mr_fid.mem_desc = (void *) (uintptr_t) key;
	ofi_mutex_unlock(&dom->lock);

	*mr = &_mr->mr_fid;
	ofi_atomic_inc32(&dom->ref);

	if (dom->mr_eq) {
		eq_entry.fid = &domain->fid;
		eq_entry.context = attr->context;
		return sock_eq_report_event(dom->mr_eq, FI_MR_COMPLETE,
			&eq_entry, sizeof(eq_entry), 0);
	}

	return 0;

err:
	ofi_mutex_unlock(&dom->lock);
	free(_mr);
	return ret;
}

static int sock_regv(struct fid *fid, const struct iovec *iov,
	size_t count, uint64_t access,
	uint64_t offset, uint64_t requested_key,
	uint64_t flags, struct fid_mr **mr, void *context)
{
	struct fi_mr_attr attr;

	attr.mr_iov = iov;
	attr.iov_count = count;
	attr.access = access;
	attr.offset = offset;
	attr.requested_key = requested_key;
	attr.context = context;
	attr.auth_key_size = 0;
	attr.auth_key = NULL;
	return sock_regattr(fid, &attr, flags, mr);
}

static int sock_reg(struct fid *fid, const void *buf, size_t len,
	uint64_t access, uint64_t offset, uint64_t requested_key,
	uint64_t flags, struct fid_mr **mr, void *context)
{
	struct iovec iov;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	return sock_regv(fid, &iov, 1, access, offset, requested_key,
		flags, mr, context);
}

struct fi_ops_mr sock_dom_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = sock_reg,
	.regv = sock_regv,
	.regattr = sock_regattr,
};
