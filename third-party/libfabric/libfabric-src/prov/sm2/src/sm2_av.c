/*
 * Copyright (c) Intel Corporation. All rights reserved.
 * Copyright (c) Amazon.com, Inc. or its affiliates. All rights reserved.
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

#include "sm2.h"

static int sm2_av_close(struct fid *fid)
{
	int ret;
	struct util_av *av;
	struct sm2_av *sm2_av;

	av = container_of(fid, struct util_av, av_fid);
	sm2_av = container_of(av, struct sm2_av, util_av);

	ret = ofi_av_close(av);
	if (ret)
		return ret;

	sm2_mmap_cleanup(&sm2_av->mmap);
	free(av);
	return 0;
}

static fi_addr_t sm2_get_addr(struct fi_peer_rx_entry *rx_entry)
{
	struct sm2_xfer_ctx *xfer_ctx = rx_entry->peer_context;
	struct sm2_ep *ep = xfer_ctx->ep;
	struct sm2_av *sm2_av =
		container_of(ep->util_ep.av, struct sm2_av, util_av);

	return sm2_av->reverse_lookup[xfer_ctx->xfer_entry.hdr.sender_gid];
}

/*
 * Input address: smr name (string)
 * output address: index (fi_addr_t) of the address
 * Returns the number of successful insertions
 */
static int sm2_av_insert(struct fid_av *av_fid, const void *addr, size_t count,
			 fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct util_ep *util_ep;
	struct sm2_ep *sm2_ep;
	struct fid_peer_srx *srx;
	fi_addr_t util_addr;
	struct sm2_av *sm2_av;
	struct util_av *util_av;
	struct dlist_entry *av_entry;
	sm2_gid_t gid;
	int i, ret, succ_count = 0;

	util_av = container_of(av_fid, struct util_av, av_fid);
	sm2_av = container_of(util_av, struct sm2_av, util_av);

	sm2_file_lock(&sm2_av->mmap);

	for (i = 0; i < count; i++, addr = (char *) addr + strlen(addr) + 1) {
		ret = sm2_entry_allocate(addr, &sm2_av->mmap, &gid, false);
		FI_DBG(&sm2_prov, FI_LOG_AV,
		       "fi_av_insert(): finished sm2_entry_allocate() "
		       "resulting AV Found = %d\n",
		       gid);

		if (ret) {
			if (util_av->eq)
				ofi_av_write_event(util_av, i, -ret, context);
			continue;
		}

		ofi_mutex_lock(&util_av->lock);
		ret = ofi_av_insert_addr(util_av, &gid, &util_addr);

		if (ret) {
			if (util_av->eq)
				ofi_av_write_event(util_av, i, -ret, context);
			ofi_mutex_unlock(&util_av->lock);
			continue;
		}

		if (flags & FI_AV_USER_ID)
			sm2_av->reverse_lookup[gid] = fi_addr[i];
		else
			sm2_av->reverse_lookup[gid] = util_addr;

		if (fi_addr)
			fi_addr[i] = util_addr;

		ofi_mutex_unlock(&util_av->lock);
		succ_count++;
	}

	sm2_file_unlock(&sm2_av->mmap);

	dlist_foreach (&util_av->ep_list, av_entry) {
		util_ep = container_of(av_entry, struct util_ep, av_entry);
		sm2_ep = container_of(util_ep, struct sm2_ep, util_ep);
		srx = sm2_get_peer_srx(sm2_ep);
		srx->owner_ops->foreach_unspec_addr(srx, &sm2_get_addr);
	}

	if (flags & FI_EVENT)
		ofi_av_write_event(util_av, succ_count, 0, context);

	return succ_count;
}

static int sm2_av_remove(struct fid_av *av_fid, fi_addr_t *fi_addr,
			 size_t count, uint64_t flags)
{
	struct util_av *util_av;
	struct sm2_av *sm2_av;
	int ret, i;
	sm2_gid_t gid;

	util_av = container_of(av_fid, struct util_av, av_fid);
	sm2_av = container_of(util_av, struct sm2_av, util_av);

	ofi_mutex_lock(&util_av->lock);
	for (i = 0; i < count; i++) {
		gid = *((sm2_gid_t *) ofi_av_get_addr(util_av, fi_addr[i]));
		if (gid > 0 && gid < SM2_MAX_UNIVERSE_SIZE)
			sm2_av->reverse_lookup[gid] = FI_ADDR_NOTAVAIL;

		ret = ofi_av_remove_addr(util_av, fi_addr[i]);
		if (ret) {
			FI_WARN(&sm2_prov, FI_LOG_AV,
				"Unable to remove address from AV\n");
			break;
		}
	}

	ofi_mutex_unlock(&util_av->lock);
	return ret;
}

static int sm2_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr,
			 size_t *addrlen)
{
	struct util_av *util_av;
	struct sm2_av *sm2_av;
	struct sm2_coord_file_header *header;
	struct sm2_ep_allocation_entry *entries;
	sm2_gid_t gid;

	*addrlen = MIN(FI_NAME_MAX, *addrlen);

	util_av = container_of(av, struct util_av, av_fid);
	sm2_av = container_of(util_av, struct sm2_av, util_av);

	ofi_mutex_lock(&util_av->lock);
	gid = *((sm2_gid_t *) ofi_av_get_addr(util_av, fi_addr));
	ofi_mutex_unlock(&util_av->lock);

	if (gid >= SM2_MAX_UNIVERSE_SIZE) {
		FI_WARN(&sm2_prov, FI_LOG_EP_DATA,
			"Looking up fi_addr %" PRIu64
			" which does not exist in map\n",
			fi_addr);
		return -FI_EINVAL;
	}

	header = (void *) sm2_av->mmap.base;
	entries = (void *) (sm2_av->mmap.base + header->ep_allocation_offset);

	strncpy(addr, entries[gid].ep_name, *addrlen);
	*addrlen = strnlen(entries[gid].ep_name, FI_NAME_MAX);

	FI_DBG(&sm2_prov, FI_LOG_AV, "sm2_av_lookup: %s\n", (char *) addr);

	return 0;
}

static const char *sm2_av_straddr(struct fid_av *av, const void *addr,
				  char *buf, size_t *len)
{
	/* the input address is a string format */
	if (buf)
		strncpy(buf, (const char *) addr, *len);

	*len = strlen((const char *) addr) + 1;
	return buf;
}

static struct fi_ops sm2_av_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = sm2_av_close,
	.bind = ofi_av_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_av sm2_av_ops = {
	.size = sizeof(struct fi_ops_av),
	.insert = sm2_av_insert,
	.insertsvc = fi_no_av_insertsvc,
	.insertsym = fi_no_av_insertsym,
	.remove = sm2_av_remove,
	.lookup = sm2_av_lookup,
	.straddr = sm2_av_straddr,
};

int sm2_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
		struct fid_av **av, void *context)
{
	struct util_domain *util_domain;
	struct util_av_attr util_attr;
	struct sm2_av *sm2_av;
	int ret, i;

	if (!attr) {
		FI_INFO(&sm2_prov, FI_LOG_AV, "invalid attr\n");
		return -FI_EINVAL;
	}

	if (attr->name) {
		FI_INFO(&sm2_prov, FI_LOG_AV, "shared AV not supported\n");
		return -FI_ENOSYS;
	}

	if (attr->type == FI_AV_UNSPEC)
		attr->type = FI_AV_TABLE;

	util_domain = container_of(domain, struct util_domain, domain_fid);

	sm2_av = calloc(1, sizeof *sm2_av);
	if (!sm2_av)
		return -FI_ENOMEM;

	util_attr.addrlen = sizeof(sm2_gid_t);
	util_attr.context_len = 0;
	util_attr.flags = 0;
	if (attr->count > SM2_MAX_UNIVERSE_SIZE) {
		FI_INFO(&sm2_prov, FI_LOG_AV, "count %d exceeds max peers\n",
			(int) attr->count);
		free(sm2_av);
		return -FI_ENOSYS;
	}

	ret = ofi_av_init(util_domain, attr, &util_attr, &sm2_av->util_av,
			  context);
	if (ret)
		goto out;

	ret = sm2_file_open_or_create(&sm2_av->mmap);
	if (ret)
		goto out;

	*av = &sm2_av->util_av.av_fid;
	(*av)->fid.ops = &sm2_av_fi_ops;
	(*av)->ops = &sm2_av_ops;

	/* Initialize all addresses to FI_ADDR_NOTAVAIL */
	for (i = 0; i < SM2_MAX_UNIVERSE_SIZE; i++)
		sm2_av->reverse_lookup[i] = FI_ADDR_NOTAVAIL;

	return 0;
out:
	ofi_av_close(&sm2_av->util_av);
	free(sm2_av);
	return ret;
}
