/*
 * Copyright (c) Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *	   Redistribution and use in source and binary forms, with or
 *	   without modification, are permitted provided that the following
 *	   conditions are met:
 *
 *		- Redistributions of source code must retain the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer.
 *
 *		- Redistributions in binary form must reproduce the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer in the documentation and/or other materials
 *		  provided with the distribution.
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

#include "xnet.h"

int xnet_av_open(struct fid_domain *domain_fid, struct fi_av_attr *attr,
		 struct fid_av **fid_av, void *context)
{
	return rxm_util_av_open(domain_fid, attr, fid_av, context,
				sizeof(struct xnet_conn), NULL, NULL);
}

static int xnet_mplex_av_remove(struct fid_av *av_fid, fi_addr_t *fi_addr,
			 size_t count, uint64_t flags)
{
	int ret;
	struct fid_list_entry *item;
	struct fid_av *subav_fid;
	struct xnet_mplex_av *av = container_of(av_fid, struct xnet_mplex_av,
						util_av.av_fid);

	ofi_genlock_lock(&av->lock);
	dlist_foreach_container(&av->subav_list, struct fid_list_entry,	item, entry) {
		subav_fid = container_of(item->fid, struct fid_av, fid);
		ret = fi_av_remove(subav_fid, fi_addr, count, flags);
		if (ret)
			goto out;
	}
	ret = ofi_ip_av_remove(&av->util_av.av_fid, fi_addr, count, flags);
out:
	ofi_genlock_unlock(&av->lock);
	return ret;
}

static int xnet_mplex_av_insert(struct fid_av *av_fid, const void *addr, size_t count,
			 fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	int ret;
	struct fid_list_entry *item;
	struct fid_av *subav_fid;
	fi_addr_t *sub_fi_addr;
	struct xnet_mplex_av *av = container_of(av_fid, struct xnet_mplex_av,
						util_av.av_fid);
	sub_fi_addr = calloc(count, sizeof(fi_addr_t));
	if (!sub_fi_addr)
		return -FI_ENOMEM;
	ofi_genlock_lock(&av->lock);
	ret = ofi_ip_av_insert(&av->util_av.av_fid, addr, count, fi_addr, flags, context);
	if (ret < count)
		goto out;
	dlist_foreach_container(&av->subav_list, struct fid_list_entry, item, entry) {
		subav_fid = container_of(item->fid, struct fid_av, fid);
		ret = fi_av_insert(subav_fid, addr, count, sub_fi_addr, flags, context);
		if (ret < count)
			break;
		assert(!fi_addr || memcmp(fi_addr, sub_fi_addr,
					  sizeof(fi_addr_t) * count) == 0);
	}
out:
	ofi_genlock_unlock(&av->lock);
	free(sub_fi_addr);
	return ret;
}

static int xnet_mplex_av_insertsym(struct fid_av *av_fid, const char *node,
				   size_t nodecnt, const char *service,
				   size_t svccnt, fi_addr_t *fi_addr,
				   uint64_t flags, void *context)
{
	int ret;
	struct fid_list_entry *item;
	struct fid_av *subav_fid;
	fi_addr_t *sub_fi_addr;
	struct xnet_mplex_av *av = container_of(av_fid, struct xnet_mplex_av,
						util_av.av_fid.fid);
	sub_fi_addr = calloc(nodecnt * svccnt, sizeof(fi_addr_t));
	if (!sub_fi_addr)
		return -FI_ENOMEM;
	ofi_genlock_lock(&av->lock);
	ret = ofi_ip_av_insertsym(&av->util_av.av_fid, node, nodecnt,
				  service, svccnt, fi_addr, flags, context);
	if (ret < nodecnt * svccnt)
		goto out;
	dlist_foreach_container(&av->subav_list, struct fid_list_entry,	item, entry) {
		subav_fid = container_of(item->fid, struct fid_av, fid);
		ret = fi_av_insertsym(subav_fid, node, nodecnt, service, svccnt,
				      sub_fi_addr, flags, context);
		if (ret <= nodecnt * svccnt)
			break;
		assert(!fi_addr || memcmp(fi_addr, sub_fi_addr,
					  sizeof(fi_addr_t) * nodecnt * svccnt) == 0);
	}
out:
	ofi_genlock_unlock(&av->lock);
	free(sub_fi_addr);
	return ret;
}

static int xnet_mplex_av_insertsvc(struct fid_av *av_fid, const char *node,
				   const char *service, fi_addr_t *fi_addr,
				   uint64_t flags, void *context)
{
	int ret;
	struct fid_list_entry *item;
	struct fid_av *subav_fid;
	fi_addr_t sub_fi_addr;
	struct xnet_mplex_av *av = container_of(av_fid, struct xnet_mplex_av,
						util_av.av_fid.fid);
	ofi_genlock_lock(&av->lock);
	ret = ofi_ip_av_insertsvc(&av->util_av.av_fid, node, service,
				  fi_addr, flags, context);
	if (ret <= 0)
		goto out;
	dlist_foreach_container(&av->subav_list, struct fid_list_entry,	item, entry) {
		subav_fid = container_of(item->fid, struct fid_av, fid);
		ret = fi_av_insertsvc(subav_fid, node, service, &sub_fi_addr, flags,
				      context);
		if (ret <= 0)
			break;
		assert(!fi_addr || *fi_addr == sub_fi_addr);
	}
out:
	ofi_genlock_unlock(&av->lock);
	return ret;
}

static int xnet_mplex_av_lookup(struct fid_av *av_fid, fi_addr_t fi_addr,
				void *addr, size_t *addrlen)
{
	struct xnet_mplex_av *av = container_of(av_fid, struct xnet_mplex_av,
						util_av.av_fid.fid);
	return ofi_ip_av_lookup(&av->util_av.av_fid, fi_addr, addr, addrlen);
}

static int xnet_mplex_av_close(struct fid *av_fid)
{
	struct xnet_mplex_av *av;
	struct fid_list_entry *item;
	int ret = 0;

	av = container_of(av_fid, struct xnet_mplex_av, util_av.av_fid.fid);
	while (!dlist_empty(&av->subav_list)) {
		dlist_pop_front(&av->subav_list, struct fid_list_entry,	item, entry);
		(void)fi_close(item->fid);
		free(item);
	}
	ret = ofi_av_close(&av->util_av);
	ofi_genlock_destroy(&av->lock);
	free(av);
	return ret;
}

static struct fi_ops xnet_mplex_av_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = xnet_mplex_av_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_av xnet_mplex_av_ops = {
	.size = sizeof(struct fi_ops_av),
	.insert = xnet_mplex_av_insert,
	.insertsvc = xnet_mplex_av_insertsvc,
	.insertsym = xnet_mplex_av_insertsym,
	.remove = xnet_mplex_av_remove,
	.lookup = xnet_mplex_av_lookup,
	.straddr = ofi_ip_av_straddr,
	.av_set = fi_no_av_set,
};

int xnet_mplex_av_open(struct fid_domain *domain_fid, struct fi_av_attr *attr,
		       struct fid_av **fid_av, void *context)
{
	struct xnet_mplex_av *av;
	struct util_domain *domain;
	struct util_av_attr util_attr = {0};
	int ret;

	av = calloc(1, sizeof(*av));
	if (!av)
		return -FI_ENOMEM;

	ret = ofi_genlock_init(&av->lock, OFI_LOCK_MUTEX);
	if (ret)
		goto free;

	domain = container_of(domain_fid, struct util_domain, domain_fid);

	util_attr.context_len = sizeof(struct util_peer_addr *);
	util_attr.addrlen = ofi_sizeof_addr_format(domain->addr_format);
	if (attr->type == FI_AV_UNSPEC)
		attr->type = FI_AV_TABLE;

	ret = ofi_av_init(domain, attr, &util_attr, &av->util_av, context);
	if (ret)
		goto free_lock;
	dlist_init(&av->subav_list);
	av->util_av.av_fid.fid.ops = &xnet_mplex_av_fi_ops;
	av->util_av.av_fid.ops = &xnet_mplex_av_ops;
	*fid_av = &av->util_av.av_fid;
	return FI_SUCCESS;

free_lock:
	ofi_genlock_destroy(&av->lock);
free:
	free(av);
	return ret;
}
