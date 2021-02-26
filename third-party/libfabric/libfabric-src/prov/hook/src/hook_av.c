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
#include "ofi_hook.h"


static int
hook_av_insert(struct fid_av *av, const void *addr, size_t count,
	       fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct hook_av *myav = container_of(av, struct hook_av, av);

	return fi_av_insert(myav->hav, addr, count, fi_addr, flags, context);
}

static int
hook_av_insertsvc(struct fid_av *av, const char *node,
		  const char *service, fi_addr_t *fi_addr, uint64_t flags,
		  void *context)
{
	struct hook_av *myav = container_of(av, struct hook_av, av);

	return fi_av_insertsvc(myav->hav, node, service, fi_addr,
			       flags, context);
}

static int
hook_av_insertsym(struct fid_av *av, const char *node, size_t nodecnt,
		  const char *service, size_t svccnt, fi_addr_t *fi_addr,
		  uint64_t flags, void *context)
{
	struct hook_av *myav = container_of(av, struct hook_av, av);

	return fi_av_insertsym(myav->hav, node, nodecnt, service, svccnt,
			       fi_addr, flags, context);
}

static int
hook_av_remove(struct fid_av *av, fi_addr_t *fi_addr, size_t count,
	       uint64_t flags)
{
	struct hook_av *myav = container_of(av, struct hook_av, av);

	return fi_av_remove(myav->hav, fi_addr, count, flags);
}

static int
hook_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr,
	       size_t *addrlen)
{
	struct hook_av *myav = container_of(av, struct hook_av, av);

	return fi_av_lookup(myav->hav, fi_addr, addr, addrlen);
}

static const char *
hook_av_straddr(struct fid_av *av, const void *addr, char *buf, size_t *len)
{
	struct hook_av *myav = container_of(av, struct hook_av, av);

	return fi_av_straddr(myav->hav, addr, buf, len);
}

static struct fi_ops_av hook_av_ops = {
	.size = sizeof(struct fi_ops_av),
	.insert = hook_av_insert,
	.insertsvc = hook_av_insertsvc,
	.insertsym = hook_av_insertsym,
	.remove = hook_av_remove,
	.lookup = hook_av_lookup,
	.straddr = hook_av_straddr,
};

int hook_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
		 struct fid_av **av, void *context)
{
	struct hook_domain *dom = container_of(domain, struct hook_domain, domain);
	struct hook_av *myav;
	int ret;

	myav = calloc(1, sizeof *myav);
	if (!myav)
		return -FI_ENOMEM;

	myav->domain = dom;
	myav->av.fid.fclass = FI_CLASS_AV;
	myav->av.fid.context = context;
	myav->av.fid.ops = &hook_fid_ops;
	myav->av.ops = &hook_av_ops;

	ret = fi_av_open(dom->hdomain, attr, &myav->hav, &myav->av.fid);
	if (ret)
		free(myav);
	else
		*av = &myav->av;

	return ret;
}
