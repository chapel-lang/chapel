/*
 * Copyright (c) 2022 Intel Corporation, Inc.  All rights reserved.
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

#include "coll.h"

static int coll_av_close(struct fid *av_fid)
{
	struct coll_av *av;
	int ret;

	av = container_of(av_fid, struct coll_av, util_av.av_fid.fid);
	ret = ofi_av_close(&av->util_av);
	if (ret)
		return ret;

	free(av);
	return 0;
}

static struct fi_ops coll_av_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = coll_av_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_av coll_av_ops = {
	.size = sizeof(struct fi_ops_av),
	.insert = fi_no_av_insert,
	.insertsvc = fi_no_av_insertsvc,
	.insertsym = fi_no_av_insertsym,
	.remove = fi_no_av_remove,
	.lookup = fi_no_av_lookup,
	.straddr = fi_no_av_straddr,
	.av_set = coll_av_set,
};

int coll_av_open(struct fid_domain *domain_fid, struct fi_av_attr *attr,
		 struct fid_av **fid_av, void *context)
{
	struct coll_av *av;
	struct fi_peer_av_context *peer_context = context;
	struct util_domain *domain;
	struct util_av_attr util_attr;
	int ret;

	if (!attr || !(attr->flags & FI_PEER)) {
		FI_WARN(&coll_prov, FI_LOG_CORE, "FI_PEER flag required\n");
		return FI_EINVAL;
	}

	if (!peer_context || peer_context->size < sizeof(*peer_context)) {
		FI_WARN(&coll_prov, FI_LOG_CORE, "invalid peer AV context\n");
		return FI_EINVAL;
	}

	av = calloc(1, sizeof(*av));
	if (!av)
		return -FI_ENOMEM;

	domain = container_of(domain_fid, struct util_domain, domain_fid);

	util_attr.context_len = sizeof(struct util_peer_addr *);
	util_attr.flags = 0;
	util_attr.addrlen = ofi_sizeof_addr_format(domain->addr_format);
	if (attr->type == FI_AV_UNSPEC)
		attr->type = FI_AV_TABLE;

        ret = ofi_av_init(domain, attr, &util_attr, &av->util_av, context);
        if (ret)
                goto err;

	av->peer_av = peer_context->av;
        av->util_av.av_fid.fid.ops = &coll_av_fi_ops;
        av->util_av.av_fid.ops = &coll_av_ops;
	(*fid_av) = &av->util_av.av_fid;
	return 0;

err:
	free(av);
	return ret;
}
