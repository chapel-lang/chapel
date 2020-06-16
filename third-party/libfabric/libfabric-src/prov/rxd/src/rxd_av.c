/*
 * Copyright (c) 2015-2018 Intel Corporation. All rights reserved.
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

#include "rxd.h"
#include <inttypes.h>


static int rxd_tree_compare(struct ofi_rbmap *map, void *key, void *data)
{
	struct rxd_av *av;
	uint8_t addr[RXD_NAME_LENGTH];
	size_t len = RXD_NAME_LENGTH;
	int ret;

	memset(addr, 0, len);
	av = container_of(map, struct rxd_av, rbmap);
	ret = fi_av_lookup(av->dg_av, av->rxd_addr_table[(fi_addr_t) data].dg_addr,
			   addr, &len);
	if (ret)
		return -1;

	return memcmp(key, addr, len);
}

/*
 * The RXD code is agnostic wrt the datagram address format, but we need
 * to know the size of the address in order to iterate over them.  Because
 * the datagram AV may be configured for asynchronous operation, open a
 * temporary one to insert/lookup the address to get the size.  I agree it's
 * goofy.
 */
static int rxd_av_set_addrlen(struct rxd_av *av, const void *addr)
{
	struct rxd_domain *domain;
	struct fid_av *tmp_av;
	struct fi_av_attr attr;
	uint8_t tmp_addr[RXD_NAME_LENGTH];
	fi_addr_t fiaddr;
	size_t len;
	int ret;

	FI_INFO(&rxd_prov, FI_LOG_AV, "determine dgram address len\n");
	memset(&attr, 0, sizeof attr);
	attr.count = 1;

	domain = container_of(av->util_av.domain, struct rxd_domain, util_domain);
	ret = fi_av_open(domain->dg_domain, &attr, &tmp_av, NULL);
	if (ret) {
		FI_WARN(&rxd_prov, FI_LOG_AV, "failed to open av: %d (%s)\n",
			-ret, fi_strerror(-ret));
		return ret;
	}

	ret = fi_av_insert(tmp_av, addr, 1, &fiaddr, 0, NULL);
	if (ret != 1) {
		FI_WARN(&rxd_prov, FI_LOG_AV, "addr insert failed: %d (%s)\n",
			-ret, fi_strerror(-ret));
		ret = -FI_EINVAL;
		goto close;
	}

	len = sizeof tmp_addr;
	ret = fi_av_lookup(tmp_av, fiaddr, tmp_addr, &len);
	if (ret) {
		FI_WARN(&rxd_prov, FI_LOG_AV, "addr lookup failed: %d (%s)\n",
			-ret, fi_strerror(-ret));
		goto close;
	}

	FI_INFO(&rxd_prov, FI_LOG_AV, "set dgram address len: %zu\n", len);
	av->dg_addrlen = len;
close:
	fi_close(&tmp_av->fid);
	return ret;
}

static fi_addr_t rxd_av_dg_addr(struct rxd_av *av, fi_addr_t fi_addr)
{
	fi_addr_t rxd_addr = av->fi_addr_table[fi_addr];

	return rxd_addr == FI_ADDR_UNSPEC ? rxd_addr :
		av->rxd_addr_table[rxd_addr].dg_addr;
}

static fi_addr_t rxd_set_rxd_addr(struct rxd_av *av, fi_addr_t dg_addr)
{
	int tries = 0;

	while (av->rxd_addr_table[av->rxd_addr_idx].dg_addr != FI_ADDR_UNSPEC &&
	       tries < av->util_av.count) {
		if (++av->rxd_addr_idx == av->util_av.count)
			av->rxd_addr_idx = 0;
		tries++;
	}
	assert(av->rxd_addr_idx < av->util_av.count && tries < av->util_av.count);
	av->rxd_addr_table[av->rxd_addr_idx].dg_addr = dg_addr;

	return av->rxd_addr_idx;
}

static fi_addr_t rxd_set_fi_addr(struct rxd_av *av, fi_addr_t rxd_addr)
{
	int tries = 0;

	while (av->fi_addr_table[av->fi_addr_idx] != FI_ADDR_UNSPEC &&
	       tries < av->util_av.count) {
		if (++av->fi_addr_idx == av->util_av.count)
			av->fi_addr_idx = 0;
		tries++;
	}
	assert(av->fi_addr_idx < av->util_av.count && tries < av->util_av.count);
	av->fi_addr_table[av->fi_addr_idx] = rxd_addr;
	av->rxd_addr_table[rxd_addr].fi_addr = av->fi_addr_idx;

	return av->fi_addr_idx;
}

int rxd_av_insert_dg_addr(struct rxd_av *av, const void *addr,
			  fi_addr_t *rxd_addr, uint64_t flags,
			  void *context)
{
	fi_addr_t dg_addr;
	int ret;

	ret = fi_av_insert(av->dg_av, addr, 1, &dg_addr,
			     flags, context);
	if (ret != 1)
		return -FI_EINVAL;

	*rxd_addr = rxd_set_rxd_addr(av, dg_addr);

	ret = ofi_rbmap_insert(&av->rbmap, (void *) addr, (void *) (*rxd_addr),
			       NULL);
	if (ret) {
		assert(ret != -FI_EALREADY);
		fi_av_remove(av->dg_av, &dg_addr, 1, flags);
	}

	return ret;
}

static int rxd_av_insert(struct fid_av *av_fid, const void *addr, size_t count,
			fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct rxd_av *av;
	int i = 0, ret = 0, success_cnt = 0;
	fi_addr_t rxd_addr, util_addr;
	struct ofi_rbnode *node;

	av = container_of(av_fid, struct rxd_av, util_av.av_fid);
	fastlock_acquire(&av->util_av.lock);
	if (!av->dg_addrlen) {
		ret = rxd_av_set_addrlen(av, addr);
		if (ret)
			goto out;
	}

	for (; i < count; i++, addr = (uint8_t *) addr + av->dg_addrlen) {
		node = ofi_rbmap_find(&av->rbmap, (void *) addr);
		if (node) {
			rxd_addr = (fi_addr_t) node->data;
		} else {
			ret = rxd_av_insert_dg_addr(av, addr, &rxd_addr,
						    flags, context);
			if (ret)
				break;
		}

		util_addr = av->rxd_addr_table[rxd_addr].fi_addr == FI_ADDR_UNSPEC ?
			    rxd_set_fi_addr(av, rxd_addr) :
			    av->rxd_addr_table[rxd_addr].fi_addr;
		if (fi_addr)
			fi_addr[i] = util_addr;

		success_cnt++;
	}

	if (ret) {
		FI_WARN(&rxd_prov, FI_LOG_AV,
			"failed to insert address %d: %d (%s)\n",
			i, -ret, fi_strerror(-ret));
		if (av->util_av.eq)
			ofi_av_write_event(&av->util_av, i, -ret, context);
		if (fi_addr)
			fi_addr[i] = FI_ADDR_NOTAVAIL;
		i++;
	}
out:
	av->dg_av_used += success_cnt;
	fastlock_release(&av->util_av.lock);

	for (; i < count; i++) {
		if (av->util_av.eq)
			ofi_av_write_event(&av->util_av, i, FI_ECANCELED, context);
		if (fi_addr)
			fi_addr[i] = FI_ADDR_NOTAVAIL;
	}

	if (av->util_av.eq) {
		ofi_av_write_event(&av->util_av, success_cnt, 0, context);
		return 0;
	}

	return success_cnt;
}

static int rxd_av_insertsvc(struct fid_av *av, const char *node,
			   const char *service, fi_addr_t *fi_addr,
			   uint64_t flags, void *context)
{
	return -FI_ENOSYS;
}

static int rxd_av_insertsym(struct fid_av *av_fid, const char *node, size_t nodecnt,
			   const char *service, size_t svccnt, fi_addr_t *fi_addr,
			   uint64_t flags, void *context)
{
	return -FI_ENOSYS;
}

static int rxd_av_remove(struct fid_av *av_fid, fi_addr_t *fi_addr, size_t count,
			uint64_t flags)
{
	int ret = 0;
	size_t i, addrlen;
	fi_addr_t rxd_addr;
	struct rxd_av *av;
	uint8_t addr[RXD_NAME_LENGTH];

	av = container_of(av_fid, struct rxd_av, util_av.av_fid);
	fastlock_acquire(&av->util_av.lock);
	for (i = 0; i < count; i++) {
		rxd_addr = av->fi_addr_table[fi_addr[i]];

		addrlen = RXD_NAME_LENGTH;
		ret = fi_av_lookup(av->dg_av, av->rxd_addr_table[rxd_addr].dg_addr,
				   addr, &addrlen);
		if (ret)
			goto err;
		
		ret = ofi_rbmap_find_delete(&av->rbmap, (void *) addr);
		if (ret)
			goto err;

		ret = fi_av_remove(av->dg_av, &av->rxd_addr_table[rxd_addr].dg_addr,
				   1, flags);
		if (ret)
			goto err;

		av->fi_addr_table[fi_addr[i]] = FI_ADDR_UNSPEC;
		av->rxd_addr_table[rxd_addr].fi_addr = FI_ADDR_UNSPEC;
		av->rxd_addr_table[rxd_addr].dg_addr = FI_ADDR_UNSPEC;
		av->dg_av_used--;
	}

err:
	if (ret)
		FI_WARN(&rxd_prov, FI_LOG_AV, "Unable to remove address from AV\n");

	fastlock_release(&av->util_av.lock);
	return ret;
}

static const char *rxd_av_straddr(struct fid_av *av, const void *addr,
				  char *buf, size_t *len)
{
	struct rxd_av *rxd_av;
	rxd_av = container_of(av, struct rxd_av, util_av.av_fid);
	return rxd_av->dg_av->ops->straddr(rxd_av->dg_av, addr, buf, len);
}

static int rxd_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr,
			 size_t *addrlen)
{
	struct rxd_av *rxd_av;
	fi_addr_t dg_fiaddr;

	rxd_av = container_of(av, struct rxd_av, util_av.av_fid);
	dg_fiaddr = rxd_av_dg_addr(rxd_av, fi_addr);
	if (dg_fiaddr == FI_ADDR_UNSPEC)
		return -FI_ENODATA;

	return fi_av_lookup(rxd_av->dg_av, dg_fiaddr, addr, addrlen);
}

static struct fi_ops_av rxd_av_ops = {
	.size = sizeof(struct fi_ops_av),
	.insert = rxd_av_insert,
	.insertsvc = rxd_av_insertsvc,
	.insertsym = rxd_av_insertsym,
	.remove = rxd_av_remove,
	.lookup = rxd_av_lookup,
	.straddr = rxd_av_straddr,
};

static int rxd_av_close(struct fid *fid)
{
	struct rxd_av *av;
	int ret;

	av = container_of(fid, struct rxd_av, util_av.av_fid);
	ret = fi_close(&av->dg_av->fid);
	if (ret)
		return ret;

	ofi_rbmap_cleanup(&av->rbmap);
	ret = ofi_av_close(&av->util_av);
	if (ret)
		return ret;

	free(av->fi_addr_table);
	free(av->rxd_addr_table);
	free(av);
	return 0;
}

static int rxd_av_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	return ofi_av_bind(fid, bfid, flags);
}

static struct fi_ops rxd_av_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = rxd_av_close,
	.bind = rxd_av_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

int rxd_av_create(struct fid_domain *domain_fid, struct fi_av_attr *attr,
		   struct fid_av **av_fid, void *context)
{
	int ret, i;
	struct rxd_av *av;
	struct rxd_domain *domain;
	struct util_av_attr util_attr;
	struct fi_av_attr av_attr;

	if (!attr)
		return -FI_EINVAL;

	if (attr->name)
		return -FI_ENOSYS;

	//TODO implement dynamic AV sizing
	attr->count = roundup_power_of_two(attr->count ?
					   attr->count : rxd_env.max_peers);
	domain = container_of(domain_fid, struct rxd_domain, util_domain.domain_fid);
	av = calloc(1, sizeof(*av));
	if (!av)
		return -FI_ENOMEM;
	av->fi_addr_table = calloc(1, attr->count * sizeof(fi_addr_t));
	av->rxd_addr_table = calloc(1, rxd_env.max_peers * sizeof(struct rxd_addr));
	if (!av->fi_addr_table || !av->rxd_addr_table) {
		ret = -FI_ENOMEM;
		goto err1;
	}


	util_attr.addrlen = sizeof(fi_addr_t);
	util_attr.flags = 0;
	attr->type = domain->util_domain.av_type != FI_AV_UNSPEC ?
		     domain->util_domain.av_type : FI_AV_TABLE;

	ret = ofi_av_init(&domain->util_domain, attr, &util_attr,
			 &av->util_av, context);
	if (ret)
		goto err1;

	ofi_rbmap_init(&av->rbmap, rxd_tree_compare);
	for (i = 0; i < attr->count; av->fi_addr_table[i++] = FI_ADDR_UNSPEC)
		;
	for (i = 0; i < rxd_env.max_peers; i++) {
		av->rxd_addr_table[i].fi_addr = FI_ADDR_UNSPEC;
		av->rxd_addr_table[i].dg_addr = FI_ADDR_UNSPEC;
	}

	av_attr = *attr;
	av_attr.count = 0;
	av_attr.flags = 0;
	ret = fi_av_open(domain->dg_domain, &av_attr, &av->dg_av, context);
	if (ret)
		goto err2;

	av->util_av.av_fid.fid.ops = &rxd_av_fi_ops;
	av->util_av.av_fid.ops = &rxd_av_ops;
	*av_fid = &av->util_av.av_fid;
	return 0;

err2:
	ofi_av_close(&av->util_av);
err1:
	free(av->fi_addr_table);
	free(av->rxd_addr_table);
	free(av);
	return ret;
}
