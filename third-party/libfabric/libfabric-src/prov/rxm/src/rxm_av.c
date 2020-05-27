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

#include <ofi_coll.h>

#include "rxm.h"

static int rxm_av_remove(struct fid_av *av_fid, fi_addr_t *fi_addr,
			 size_t count, uint64_t flags)
{
	struct util_av *av = container_of(av_fid, struct util_av, av_fid);
	struct rxm_ep *rxm_ep;
	int i, ret = 0;

	fastlock_acquire(&av->ep_list_lock);
	/* This should be before ofi_ip_av_remove as we need to know
	 * fi_addr -> addr mapping when moving handle to peer list. */
	dlist_foreach_container(&av->ep_list, struct rxm_ep,
				rxm_ep, util_ep.av_entry) {
		ofi_ep_lock_acquire(&rxm_ep->util_ep);
		for (i = 0; i < count; i++) {
			ret = rxm_cmap_remove(rxm_ep->cmap, *fi_addr + i);
			if (ret)
				FI_WARN(&rxm_prov, FI_LOG_AV,
					"cmap remove failed for fi_addr: %"
					PRIu64 "\n", *fi_addr + i);
		}
		ofi_ep_lock_release(&rxm_ep->util_ep);
	}
	fastlock_release(&av->ep_list_lock);

	return ofi_ip_av_remove(av_fid, fi_addr, count, flags);
}

static int
rxm_av_insert_cmap(struct fid_av *av_fid, const void *addr, size_t count,
		   fi_addr_t *fi_addr, uint64_t flags)
{
	struct util_av *av = container_of(av_fid, struct util_av, av_fid);
	struct rxm_ep *rxm_ep;
	fi_addr_t fi_addr_tmp;
	size_t i;
	int ret = 0;
	const void *cur_addr;

	fastlock_acquire(&av->ep_list_lock);
	dlist_foreach_container(&av->ep_list, struct rxm_ep,
				rxm_ep, util_ep.av_entry) {
		ofi_ep_lock_acquire(&rxm_ep->util_ep);
		for (i = 0; i < count; i++) {
			if (!rxm_ep->cmap)
				break;

			cur_addr = (const void *) ((char *) addr + i * av->addrlen);
			fi_addr_tmp = (fi_addr ? fi_addr[i] :
				       ofi_av_lookup_fi_addr_unsafe(av, cur_addr));
			if (fi_addr_tmp == FI_ADDR_NOTAVAIL)
				continue;

			ret = rxm_cmap_update(rxm_ep->cmap, cur_addr, fi_addr_tmp);
			if (OFI_UNLIKELY(ret)) {
				FI_WARN(&rxm_prov, FI_LOG_AV,
					"cmap update failed for fi_addr: %"
					PRIu64 "\n", fi_addr_tmp);
				break;
			}
		}
		ofi_ep_lock_release(&rxm_ep->util_ep);
	}
	fastlock_release(&av->ep_list_lock);
	return ret;
}

static int rxm_av_insert(struct fid_av *av_fid, const void *addr, size_t count,
			 fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct util_av *av = container_of(av_fid, struct util_av, av_fid);
	int ret, retv;

	ret = ofi_ip_av_insert(av_fid, addr, count, fi_addr, flags, context);
	if (ret < 0)
		return ret;

	if (!av->eq && !ret)
		return ret;

	retv = rxm_av_insert_cmap(av_fid, addr, count, fi_addr, flags);
	if (retv) {
		ret = rxm_av_remove(av_fid, fi_addr, count, flags);
		if (ret)
			FI_WARN(&rxm_prov, FI_LOG_AV, "Failed to remove addr "
				"from AV during error handling\n");
		return retv;
	}
	return ret;
}

static int rxm_av_insertsym(struct fid_av *av_fid, const char *node,
			    size_t nodecnt, const char *service, size_t svccnt,
			    fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct util_av *av = container_of(av_fid, struct util_av, av_fid);
	void *addr;
	size_t addrlen, count = nodecnt * svccnt;
	int ret, retv;

	ret = ofi_verify_av_insert(av, flags);
	if (ret)
		return ret;

	ret = ofi_ip_av_sym_getaddr(av, node, nodecnt, service,
				    svccnt, &addr, &addrlen);
	if (ret <= 0)
		return ret;

	assert(ret == count);

	ret = ofi_ip_av_insertv(av, addr, addrlen, count, fi_addr, context);
	if (ret < 0)
		goto out;

	if (!av->eq && !ret)
		goto out;

	retv = rxm_av_insert_cmap(av_fid, addr, count, fi_addr, flags);
	if (retv) {
		ret = rxm_av_remove(av_fid, fi_addr, count, flags);
		if (ret)
			FI_WARN(&rxm_prov, FI_LOG_AV, "Failed to remove addr "
				"from AV during error handling\n");
		ret = retv;
	}
out:
	free(addr);
	return ret;

}

int rxm_av_insertsvc(struct fid_av *av, const char *node, const char *service,
		     fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	return rxm_av_insertsym(av, node, 1, service, 1, fi_addr, flags, context);
}

static const char *
rxm_av_straddr(struct fid_av *av_fid, const void *addr, char *buf, size_t *len)
{
	return ofi_ip_av_straddr(av_fid, addr, buf, len);
}

int rxm_av_lookup(struct fid_av *av_fid, fi_addr_t fi_addr,
		  void *addr, size_t *addrlen)
{
	return ofi_ip_av_lookup(av_fid, fi_addr, addr, addrlen);
}


static struct fi_ops_av rxm_av_ops = {
	.size = sizeof(struct fi_ops_av),
	.insert = rxm_av_insert,
	.insertsvc = rxm_av_insertsvc,
	.insertsym = rxm_av_insertsym,
	.remove = rxm_av_remove,
	.lookup = rxm_av_lookup,
	.straddr = rxm_av_straddr,
	.av_set = ofi_av_set
};

int rxm_av_open(struct fid_domain *domain_fid, struct fi_av_attr *attr,
		struct fid_av **av, void *context)
{
	int ret;

	ret = ofi_ip_av_create(domain_fid, attr, av, context);
	if (ret)
		return ret;

	(*av)->ops = &rxm_av_ops;
	return 0;
}

