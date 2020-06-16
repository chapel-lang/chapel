/*
 * Copyright (c) 2018 Intel Corporation. All rights reserved.
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

#include <stdlib.h>
#include "hook_prov.h"


ssize_t hook_cq_read(struct fid_cq *cq, void *buf, size_t count)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);

	return fi_cq_read(mycq->hcq, buf, count);
}

ssize_t
hook_cq_readerr(struct fid_cq *cq, struct fi_cq_err_entry *buf, uint64_t flags)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);

	return fi_cq_readerr(mycq->hcq, buf, flags);
}

ssize_t
hook_cq_readfrom(struct fid_cq *cq, void *buf, size_t count, fi_addr_t *src_addr)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);

	return fi_cq_readfrom(mycq->hcq, buf, count, src_addr);
}

ssize_t
hook_cq_sread(struct fid_cq *cq, void *buf, size_t count,
	      const void *cond, int timeout)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);

	return fi_cq_sread(mycq->hcq, buf, count, cond, timeout);
}

ssize_t
hook_cq_sreadfrom(struct fid_cq *cq, void *buf, size_t count,
		  fi_addr_t *src_addr, const void *cond, int timeout)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);

	return fi_cq_sreadfrom(mycq->hcq, buf, count, src_addr, cond, timeout);
}

int hook_cq_signal(struct fid_cq *cq)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);

	return fi_cq_signal(mycq->hcq);
}

const char *
hook_cq_strerror(struct fid_cq *cq, int prov_errno,
		 const void *err_data, char *buf, size_t len)
{
	struct hook_cq *mycq = container_of(cq, struct hook_cq, cq);

	return fi_cq_strerror(mycq->hcq, prov_errno, err_data, buf,len);
}

struct fi_ops_cq hook_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = hook_cq_read,
	.readfrom = hook_cq_readfrom,
	.readerr = hook_cq_readerr,
	.sread = hook_cq_sread,
	.sreadfrom = hook_cq_sreadfrom,
	.signal = hook_cq_signal,
	.strerror = hook_cq_strerror,
};

int hook_cq_init(struct fid_domain *domain, struct fi_cq_attr *attr,
		 struct fid_cq **cq, void *context, struct hook_cq *mycq)
{
	struct hook_domain *dom = container_of(domain, struct hook_domain, domain);
	struct fi_cq_attr hattr;
	int ret;

	mycq->domain = dom;
	mycq->cq.fid.fclass = FI_CLASS_CQ;
	mycq->cq.fid.context = context;
	mycq->cq.fid.ops = &hook_fid_ops;
	mycq->cq.ops = &hook_cq_ops;

	hattr = *attr;
	if (attr->wait_obj == FI_WAIT_SET)
		hattr.wait_set = hook_to_hwait(attr->wait_set);

	ret = fi_cq_open(dom->hdomain, &hattr, &mycq->hcq, &mycq->cq.fid);
	if (ret)
		return ret;

	*cq = &mycq->cq;
	return 0;
}

int hook_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		 struct fid_cq **cq, void *context)
{
	struct hook_domain *dom = container_of(domain, struct hook_domain, domain);
	struct hook_cq *mycq;
	int ret;

	mycq = calloc(1, sizeof *mycq);
	if (!mycq)
		return -FI_ENOMEM;

	ret = hook_cq_init(domain, attr, cq, context, mycq);
	if (ret)
		goto err1;

	ret = hook_ini_fid(dom->fabric->prov_ctx, &mycq->cq.fid);
	if (ret)
		goto err2;

	return 0;
err2:
	fi_close(&mycq->hcq->fid);
err1:
	free(mycq);
	return ret;
}
