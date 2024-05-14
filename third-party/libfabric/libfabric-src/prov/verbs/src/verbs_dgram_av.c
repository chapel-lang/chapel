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

#include "verbs_ofi.h"

static inline int vrb_dgram_av_is_addr_valid(struct vrb_dgram_av *av,
						const void *addr)
{
	const struct ofi_ib_ud_ep_name *check_name = addr;
	return (check_name->lid > 0);
}

static inline int
vrb_dgram_verify_av_flags(struct util_av *av, uint64_t flags)
{
	if ((av->flags & FI_EVENT) && !av->eq) {
		VRB_WARN(FI_LOG_AV, "No EQ bound to AV\n");
		return -FI_ENOEQ;
	}

	if (flags & ~(FI_MORE)) {
		VRB_WARN(FI_LOG_AV, "Unsupported flags\n");
		return -FI_ENOEQ;
	}

	return FI_SUCCESS;
}

static int
vrb_dgram_av_insert_addr(struct vrb_dgram_av *av, const void *addr,
			    fi_addr_t *fi_addr, void *context)
{
	int ret;
	struct vrb_dgram_av_entry *av_entry;
	struct vrb_domain *domain =
		container_of(av->util_av.domain, struct vrb_domain, util_domain);

	struct ibv_ah_attr ah_attr = {
		.is_global = 0,
		.dlid = ((struct ofi_ib_ud_ep_name *)addr)->lid,
		.sl = ((struct ofi_ib_ud_ep_name *)addr)->sl,
		.src_path_bits = 0,
		.port_num = 1,
	};

	if (((struct ofi_ib_ud_ep_name *)addr)->gid.global.interface_id) {
		ah_attr.is_global = 1;
		ah_attr.grh.hop_limit = 64;
		ah_attr.grh.dgid = ((struct ofi_ib_ud_ep_name *)addr)->gid;
		ah_attr.grh.sgid_index = vrb_gl_data.gid_idx;
	} else if (OFI_UNLIKELY(!vrb_dgram_av_is_addr_valid(av, addr))) {
		ret = -FI_EADDRNOTAVAIL;
		VRB_WARN(FI_LOG_AV, "Invalid address\n");
		goto fn1;
	}

	av_entry = calloc(1, sizeof(*av_entry));
	if (OFI_UNLIKELY(!av_entry)) {
		ret = -FI_ENOMEM;
		VRB_WARN(FI_LOG_AV, "Unable to allocate memory for AV entry\n");
		goto fn1;
	}

	av_entry->ah = ibv_create_ah(domain->pd, &ah_attr);
	if (OFI_UNLIKELY(!av_entry->ah)) {
		ret = -errno;
		VRB_WARN(FI_LOG_AV,
			   "Unable to create Address Handle, errno - %d\n", errno);
		goto fn2;
	}
	av_entry->addr = *(struct ofi_ib_ud_ep_name *)addr;
	dlist_insert_tail(&av_entry->list_entry, &av->av_entry_list);

	if (fi_addr)
		*fi_addr = (fi_addr_t)(uintptr_t)av_entry;
	return 0;
fn2:
	free(av_entry);
fn1:
	if (fi_addr)
		*fi_addr = FI_ADDR_NOTAVAIL;
	return ret;
}

static int vrb_dgram_av_insert(struct fid_av *av_fid, const void *addr,
				  size_t count, fi_addr_t *fi_addr,
				  uint64_t flags, void *context)
{
	int ret, success_cnt = 0;
	size_t i;
	struct vrb_dgram_av *av =
		 container_of(av_fid, struct vrb_dgram_av, util_av.av_fid);

	ret = vrb_dgram_verify_av_flags(&av->util_av, flags);
	if (ret)
		return ret;

	VRB_DBG(FI_LOG_AV, "Inserting %"PRIu64" addresses\n", count);
	for (i = 0; i < count; i++) {
		ret = vrb_dgram_av_insert_addr(
				av, (struct ofi_ib_ud_ep_name *)addr + i,
				fi_addr ? &fi_addr[i] : NULL, context);
		if (!ret)
			success_cnt++;
	}

	VRB_DBG(FI_LOG_AV,
		  "%d addresses were inserted successfully\n", success_cnt);
	return success_cnt;
}

static inline void
vrb_dgram_av_remove_addr(struct vrb_dgram_av_entry *av_entry)
{
	int ret = ibv_destroy_ah(av_entry->ah);
	if (ret)
		VRB_WARN(FI_LOG_AV,
			   "AH Destroying failed with status - %d\n",
			   ret);
	dlist_remove(&av_entry->list_entry);
	free(av_entry);
}

static int vrb_dgram_av_remove(struct fid_av *av_fid, fi_addr_t *fi_addr,
				  size_t count, uint64_t flags)
{
	int i, ret;
	struct vrb_dgram_av *av =
		container_of(av_fid, struct vrb_dgram_av, util_av.av_fid);

	ret = vrb_dgram_verify_av_flags(&av->util_av, flags);
	if (ret)
		return ret;

	for (i = count - 1; i >= 0; i--) {
		struct vrb_dgram_av_entry *av_entry =
			(struct vrb_dgram_av_entry *) (uintptr_t) fi_addr[i];
		vrb_dgram_av_remove_addr(av_entry);
	}
	return FI_SUCCESS;
}

static inline
int vrb_dgram_av_lookup(struct fid_av *av_fid, fi_addr_t fi_addr,
			   void *addr, size_t *addrlen)
{
	struct vrb_dgram_av_entry *av_entry;

	av_entry = vrb_dgram_av_lookup_av_entry(fi_addr);
	if (!av_entry)
		return -FI_ENOENT;

	memcpy(addr, &av_entry->addr, MIN(*addrlen, sizeof(av_entry->addr)));
	*addrlen = sizeof(av_entry->addr);
	return FI_SUCCESS;
}

static inline const char *
vrb_dgram_av_straddr(struct fid_av *av, const void *addr, char *buf, size_t *len)
{
	return ofi_straddr(buf, len, FI_ADDR_IB_UD, addr);
}

static int vrb_dgram_av_close(struct fid *av_fid)
{
	struct vrb_dgram_av_entry *av_entry;
	struct vrb_dgram_av *av =
		container_of(av_fid, struct vrb_dgram_av, util_av.av_fid.fid);
	int ret = ofi_av_close_lightweight(&av->util_av);
	if (ret)
		return ret;

	while (!dlist_empty(&av->av_entry_list)) {
		av_entry = container_of(av->av_entry_list.next,
					struct vrb_dgram_av_entry,
					list_entry);
		vrb_dgram_av_remove_addr(av_entry);
	}

	free(av);
	return FI_SUCCESS;
}

static struct fi_ops vrb_dgram_fi_ops = {
	.size		= sizeof(vrb_dgram_fi_ops),
	.close		= vrb_dgram_av_close,
	.bind		= ofi_av_bind,
	.control	= fi_no_control,
	.ops_open	= fi_no_ops_open,
};

static struct fi_ops_av vrb_dgram_av_ops = {
	.size		= sizeof(vrb_dgram_av_ops),
	.insert		= vrb_dgram_av_insert,
	.insertsvc	= fi_no_av_insertsvc,
	.insertsym	= fi_no_av_insertsym,
	.remove		= vrb_dgram_av_remove,
	.lookup		= vrb_dgram_av_lookup,
	.straddr	= vrb_dgram_av_straddr,
};

int vrb_dgram_av_open(struct fid_domain *domain_fid, struct fi_av_attr *attr,
			 struct fid_av **av_fid, void *context)
{
	struct vrb_domain *domain =
		container_of(domain_fid, struct vrb_domain,
			     util_domain.domain_fid);
	struct vrb_dgram_av *av;
	int ret;

	av = calloc(1, sizeof(*av));
	if (!av)
		return -FI_ENOMEM;

	if (attr->type == FI_AV_UNSPEC)
		attr->type = FI_AV_MAP;

	ret = ofi_av_init_lightweight(&domain->util_domain, attr,
				      &av->util_av, context);
	if (ret)
		goto err_av_init;
	dlist_init(&av->av_entry_list);

	av->util_av.av_fid.fid.ops = &vrb_dgram_fi_ops;
	av->util_av.av_fid.ops = &vrb_dgram_av_ops;
	*av_fid = &av->util_av.av_fid;

	return FI_SUCCESS;
err_av_init:
	free(av);
	return ret;
}
