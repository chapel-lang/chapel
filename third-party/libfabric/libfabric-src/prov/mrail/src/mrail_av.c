/*
 * Copyright (c) 2018-2020 Intel Corporation, Inc.  All rights reserved.
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
 *	- Redistributions of source code must retain the above
 *	  copyright notice, this list of conditions and the following
 *	  disclaimer.
 *
 *	- Redistributions in binary form must reproduce the above
 *	  copyright notice, this list of conditions and the following
 *	  disclaimer in the documentation and/or other materials
 *	  provided with the distribution.
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

#include "mrail.h"

static int mrail_av_close(struct fid *fid)
{
	struct mrail_av *mrail_av = container_of(fid, struct mrail_av,
						 util_av.av_fid);
	int ret, retv = 0;

	ret = mrail_close_fids((struct fid **)mrail_av->avs, mrail_av->num_avs);
	if (ret)
		retv = ret;
	free(mrail_av->avs);
	free(mrail_av->rail_addrlen);

	ret = ofi_av_close(&mrail_av->util_av);
	if (ret)
		retv = ret;

	free(mrail_av);
	return retv;
}

static int mrail_av_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	return ofi_av_bind(fid, bfid, flags);
}

static const char *mrail_av_straddr(struct fid_av *av, const void *addr,
				  char *buf, size_t *len)
{
	return NULL;
}

static int mrail_av_insertsvc(struct fid_av *av, const char *node,
			   const char *service, fi_addr_t *fi_addr,
			   uint64_t flags, void *context)
{
	return -FI_ENOSYS;
}

static int mrail_av_insertsym(struct fid_av *av_fid, const char *node, size_t nodecnt,
			   const char *service, size_t svccnt, fi_addr_t *fi_addr,
			   uint64_t flags, void *context)
{
	return -FI_ENOSYS;
}

static int mrail_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr,
			 size_t *addrlen)
{
	return -FI_ENOSYS;

}

static int mrail_av_remove(struct fid_av *av_fid, fi_addr_t *fi_addr, size_t count,
			uint64_t flags)
{
	return -FI_ENOSYS;
}

static int mrail_av_insert(struct fid_av *av_fid, const void *addr, size_t count,
			fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct mrail_domain *mrail_domain;
	struct mrail_av *mrail_av;
	struct mrail_peer_info *peer_info;
	size_t i, j, offset, num_inserted = 0;
	fi_addr_t index, index_rail0 = FI_ADDR_NOTAVAIL;
	int ret;

	mrail_av = container_of(av_fid, struct mrail_av, util_av.av_fid);
	mrail_domain = container_of(mrail_av->util_av.domain, struct mrail_domain,
				    util_domain);

	/* TODO if it's more optimal to insert multiple addresses at once
	 * convert ADDR1: ADDR1_RAIL1:ADDR1_RAIL2
	 *         ADDR2: ADDR2_RAIL1:ADDR2_RAIL2
	 * 	   to
	 *         ADDR1: ADDR1_RAIL1:ADDR2_RAIL1
	 *         ADDR2: ADDR1_RAIL2:ADDR2_RAIL2
	*/

	peer_info = calloc(1, mrail_av->util_av.addrlen);
	if (!peer_info)
		return -FI_ENOMEM;
	slist_init(&peer_info->ooo_recv_queue);

	for (i = 0; i < count; i++) {
		offset = i * mrail_domain->addrlen;
		for (j = 0; j < mrail_av->num_avs; j++) {
			ofi_straddr_dbg(&mrail_prov, FI_LOG_EP_CTRL,
					"addr", addr);
			ret = fi_av_insert(mrail_av->avs[j],
					   (char *)addr + offset, 1,
					   &index, flags, NULL);
			if (ret != 1) {
				free(peer_info);
				return ret;
			}
			offset += mrail_av->rail_addrlen[j];
			if (j == 0)
				index_rail0 = index;
			assert(index == index_rail0);
		}

		peer_info->addr = index_rail0;
		ret = ofi_av_insert_addr(&mrail_av->util_av, peer_info,
					 &index);
		if (ret) {
			FI_WARN(&mrail_prov, FI_LOG_AV, \
				"Unable to get rail fi_addr\n");
		} else {
			assert(index == index_rail0);
			num_inserted++;
		}

		if (fi_addr)
			fi_addr[i] = index;
	}

	free(peer_info);
	return num_inserted;
}

static struct fi_ops_av mrail_av_ops = {
	.size = sizeof(struct fi_ops_av),
	.insert = mrail_av_insert,
	.insertsvc = mrail_av_insertsvc,
	.insertsym = mrail_av_insertsym,
	.remove = mrail_av_remove,
	.lookup = mrail_av_lookup,
	.straddr = mrail_av_straddr,
};

static struct fi_ops mrail_av_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = mrail_av_close,
	.bind = mrail_av_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

int mrail_av_open(struct fid_domain *domain_fid, struct fi_av_attr *attr,
		   struct fid_av **av_fid, void *context)
{
	struct mrail_av *mrail_av;
	struct mrail_domain *mrail_domain;
	struct fi_av_attr rail_attr;
	struct util_av_attr util_attr;
	struct fi_info *fi;
	size_t i;
	int ret;

	mrail_domain = container_of(domain_fid, struct mrail_domain,
				    util_domain.domain_fid);
	mrail_av = calloc(1, sizeof(*mrail_av));
	if (!mrail_av)
		return -FI_ENOMEM;

	mrail_av->num_avs = mrail_domain->num_domains;

	util_attr.addrlen = sizeof(struct mrail_peer_info);
	/* We just need a table to store the mapping */
	util_attr.flags = 0;
	util_attr.context_len = 0;

	if (attr->type == FI_AV_UNSPEC)
		attr->type = FI_AV_TABLE;

	ret = ofi_av_init(&mrail_domain->util_domain, attr, &util_attr,
			 &mrail_av->util_av, context);
	if (ret) {
		free(mrail_av);
		return ret;
	}

	mrail_av->avs = calloc(mrail_av->num_avs, sizeof(*mrail_av->avs));
	if (!mrail_av->avs) {
		ret = -FI_ENOMEM;
		goto err;
	}

	mrail_av->rail_addrlen = calloc(mrail_av->num_avs,
					sizeof(*mrail_av->rail_addrlen));
	if (!mrail_av->rail_addrlen) {
		ret = -FI_ENOMEM;
		goto err;
	}

	rail_attr = *attr;
	rail_attr.type = FI_AV_TABLE;
	for (i = 0, fi = mrail_domain->info->next; i < mrail_av->num_avs;
	     i++, fi = fi->next) {
		ret = fi_av_open(mrail_domain->domains[i], &rail_attr,
				 &mrail_av->avs[i], context);
		if (ret)
			goto err;
		mrail_av->rail_addrlen[i] = fi->src_addrlen;
	}

	mrail_av->util_av.av_fid.fid.ops = &mrail_av_fi_ops;
	mrail_av->util_av.av_fid.ops = &mrail_av_ops;
	*av_fid = &mrail_av->util_av.av_fid;

	return 0;
err:
	mrail_av_close(&mrail_av->util_av.av_fid.fid);
	return ret;
}
