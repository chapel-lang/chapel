/*
 * Copyright (c) 2018-2019 Intel Corporation, Inc.  All rights reserved.
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

#include "mrail.h"

static int mrail_domain_close(fid_t fid)
{
	struct mrail_domain *mrail_domain =
		container_of(fid, struct mrail_domain, util_domain.domain_fid.fid);
	int ret, retv = 0;

	if (mrail_domain->domains) {
		ret = mrail_close_fids((struct fid **)mrail_domain->domains,
				       mrail_domain->num_domains);
		if (ret)
			retv = ret;
		free(mrail_domain->domains);
	}

	ret = ofi_domain_close(&mrail_domain->util_domain);
	if (ret)
		retv = ret;

	free(mrail_domain);
	return retv;
}

static int mrail_domain_map_raw(struct mrail_domain *mrail_domain,
                                struct fi_mr_map_raw *map)
{
	struct mrail_addr_key *mr_map;

	/* Copy the raw key and use a pointer as the new key. */

	mr_map = calloc(1, map->key_size);
	if (!mr_map) {
		return -FI_ENOMEM;
	}

	memcpy(mr_map, map->raw_key, map->key_size);

	*(map->key) = (uint64_t)mr_map;

	return 0;
}

static int mrail_domain_unmap_key(struct mrail_addr_key **mr_map)
{
	assert(mr_map);
	free(*mr_map);
	return 0;
}

static int mrail_domain_control(struct fid *fid, int command, void *arg)
{
	struct mrail_domain *mrail_domain = container_of(fid,
			struct mrail_domain, util_domain.domain_fid.fid);

	switch(command) {
	case FI_MAP_RAW_MR:
		return mrail_domain_map_raw(mrail_domain, arg);
	case FI_UNMAP_KEY:
		return mrail_domain_unmap_key(arg);
	}
	return -FI_EINVAL;
}

static struct fi_ops mrail_domain_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = mrail_domain_close,
	.bind = fi_no_bind,
	.control = mrail_domain_control,
	.ops_open = fi_no_ops_open,
};

static int mrail_mr_close(fid_t fid)
{
	uint32_t i;
	struct mrail_mr *mrail_mr = container_of(fid, struct mrail_mr,
			mr_fid.fid);

	for (i = 0; i < mrail_mr->num_mrs; ++i) {
		fi_close(&mrail_mr->rails[i].mr->fid);
	}
	return 0;
}

static int mrail_mr_raw_attr(struct mrail_mr *mrail_mr,
                             struct fi_mr_raw_attr *attr)
{
	uint32_t num_rails;
	uint32_t i;
	struct mrail_addr_key *rail;
	size_t required_key_size;

	num_rails = mrail_mr->num_mrs;

	required_key_size = num_rails * sizeof(struct mrail_addr_key);

	if (*(attr->key_size) < required_key_size) {
		*(attr->key_size) = required_key_size;
		return -FI_ETOOSMALL;
	}

	/* The raw key is the concatenation of one "struct mrail_addr_key" per
	 * rail. */
	for (i = 0, rail = (struct mrail_addr_key*)attr->raw_key; i < num_rails;
			++i, ++rail) {
		rail->base_addr = mrail_mr->rails[i].base_addr;
		rail->key	= fi_mr_key(mrail_mr->rails[i].mr);
	}

	*(attr->key_size) = required_key_size;
	*(attr->base_addr) = 0;

	return 0;
}

static int mrail_mr_control(struct fid *fid, int command, void *arg)
{
	struct mrail_mr *mrail_mr = container_of(fid, struct mrail_mr,
			mr_fid.fid);

	switch(command) {
	case FI_GET_RAW_MR:
		return mrail_mr_raw_attr(mrail_mr, arg);
	}
	return -FI_EINVAL;
}

static struct fi_ops mrail_mr_ops = {
	.size = sizeof(struct fi_ops),
	.close = mrail_mr_close,
	.bind = fi_no_bind,
	.control = mrail_mr_control,
	.ops_open = fi_no_ops_open,
};

static int mrail_mr_reg(struct fid *domain_fid, const void *buf, size_t len,
			 uint64_t access, uint64_t offset, uint64_t requested_key,
			 uint64_t flags, struct fid_mr **mr, void *context)
{
	struct mrail_domain *mrail_domain = container_of(domain_fid,
			struct mrail_domain, util_domain.domain_fid.fid);
	size_t num_rails = mrail_domain->num_domains;
	struct mrail_mr *mrail_mr;
	struct fi_info *fi;
	uint32_t rail;
	int ret = 0;

	mrail_mr = calloc(1, sizeof(*mrail_mr) +
			num_rails * sizeof(*mrail_mr->rails));
	if (!mrail_mr)
		return -FI_ENOMEM;

	for (rail = 0, fi = mrail_domain->info->next;
			rail < mrail_domain->num_domains;
			++rail, fi = fi->next) {
		ret = fi_mr_reg(mrail_domain->domains[rail], buf, len, access,
				offset, requested_key, flags,
				&mrail_mr->rails[rail].mr, context);
		if (ret) {
			FI_WARN(&mrail_prov, FI_LOG_DOMAIN,
				"Unable to register memory, rail %" PRIu32 "\n",
				rail);
			goto err1;
		}
		mrail_mr->rails[rail].base_addr =
			(fi->domain_attr->mr_mode & FI_MR_VIRT_ADDR) ?
			(uint64_t)buf : 0;
	}

	mrail_mr->mr_fid.fid.fclass = FI_CLASS_MR;
	mrail_mr->mr_fid.fid.context = context;
	mrail_mr->mr_fid.fid.ops = &mrail_mr_ops;
	mrail_mr->mr_fid.mem_desc = mrail_mr;
	mrail_mr->mr_fid.key = FI_KEY_NOTAVAIL;
	mrail_mr->num_mrs = mrail_domain->num_domains;
	*mr = &mrail_mr->mr_fid;

	return 0;
err1:
	for (; rail != 0; --rail)
		fi_close(&mrail_mr->rails[rail].mr->fid);
	free(mrail_mr);
	return ret;
}

static int mrail_mr_regv(struct fid *domain_fid, const struct iovec *iov,
			 size_t count, uint64_t access, uint64_t offset,
			 uint64_t requested_key, uint64_t flags,
			 struct fid_mr **mr, void *context)
{
	struct mrail_domain *mrail_domain = container_of(domain_fid,
			struct mrail_domain, util_domain.domain_fid.fid);
	size_t num_rails = mrail_domain->num_domains;
	struct mrail_mr *mrail_mr;
	struct fi_info *fi;
	uint32_t rail;
	int ret = 0;

	mrail_mr = calloc(1, sizeof(*mrail_mr) +
			num_rails * sizeof(*mrail_mr->rails));
	if (!mrail_mr)
		return -FI_ENOMEM;

	for (rail = 0, fi = mrail_domain->info->next;
			rail < mrail_domain->num_domains;
			++rail, fi = fi->next) {
		ret = fi_mr_regv(mrail_domain->domains[rail], iov, count, access,
				offset, requested_key, flags,
				&mrail_mr->rails[rail].mr, context);
		if (ret) {
			FI_WARN(&mrail_prov, FI_LOG_DOMAIN,
				"Unable to register memory, rail %" PRIu32 "\n",
				rail);
			goto err1;
		}
		mrail_mr->rails[rail].base_addr =
			(fi->domain_attr->mr_mode & FI_MR_VIRT_ADDR) ?
			(uint64_t)iov[0].iov_base : 0;
	}

	mrail_mr->mr_fid.fid.fclass = FI_CLASS_MR;
	mrail_mr->mr_fid.fid.context = context;
	mrail_mr->mr_fid.fid.ops = &mrail_mr_ops;
	mrail_mr->mr_fid.mem_desc = mrail_mr;
	mrail_mr->mr_fid.key = FI_KEY_NOTAVAIL;
	mrail_mr->num_mrs = mrail_domain->num_domains;
	*mr = &mrail_mr->mr_fid;

	return 0;
err1:
	for (; rail != 0; --rail)
		fi_close(&mrail_mr->rails[rail].mr->fid);
	free(mrail_mr);
	return ret;
}

static int mrail_mr_regattr(struct fid *domain_fid, const struct fi_mr_attr *attr,
			    uint64_t flags, struct fid_mr **mr)
{
	struct mrail_domain *mrail_domain = container_of(domain_fid,
			struct mrail_domain, util_domain.domain_fid.fid);
	size_t num_rails = mrail_domain->num_domains;
	struct mrail_mr *mrail_mr;
	struct fi_info *fi;
	uint32_t rail;
	int ret = 0;

	mrail_mr = calloc(1, sizeof(*mrail_mr) +
			num_rails * sizeof(*mrail_mr->rails));
	if (!mrail_mr)
		return -FI_ENOMEM;

	for (rail = 0, fi = mrail_domain->info->next;
			rail < mrail_domain->num_domains;
			++rail, fi = fi->next) {
		ret = fi_mr_regattr(mrail_domain->domains[rail], attr,
				    flags, &mrail_mr->rails[rail].mr);
		if (ret) {
			FI_WARN(&mrail_prov, FI_LOG_DOMAIN,
				"Unable to register memory, rail %" PRIu32 "\n",
				rail);
			goto err1;
		}
		mrail_mr->rails[rail].base_addr =
			(fi->domain_attr->mr_mode & FI_MR_VIRT_ADDR) ?
			(uint64_t)attr->mr_iov[0].iov_base : 0;
	}

	mrail_mr->mr_fid.fid.fclass = FI_CLASS_MR;
	mrail_mr->mr_fid.fid.context = attr->context;
	mrail_mr->mr_fid.fid.ops = &mrail_mr_ops;
	mrail_mr->mr_fid.mem_desc = mrail_mr;
	mrail_mr->mr_fid.key = FI_KEY_NOTAVAIL;
	mrail_mr->num_mrs = mrail_domain->num_domains;
	*mr = &mrail_mr->mr_fid;

	return 0;
err1:
	for (; rail != 0; --rail)
		fi_close(&mrail_mr->rails[rail].mr->fid);
	free(mrail_mr);
	return ret;
}

static struct fi_ops_mr mrail_domain_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = mrail_mr_reg,
	.regv = mrail_mr_regv,
	.regattr = mrail_mr_regattr,
};

int mrail_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
		struct fid_cntr **cntr_fid, void *context)
{
	int ret;
	struct util_cntr *cntr;

	cntr = calloc(1, sizeof(*cntr));
	if (!cntr)
		return -FI_ENOMEM;

	ret = ofi_cntr_init(&mrail_prov, domain, attr, cntr,
			&ofi_cntr_progress, context);
	if (ret)
		goto error;

	*cntr_fid = &cntr->cntr_fid;
	return FI_SUCCESS;

error:
	free(cntr);
	return ret;
}

static struct fi_ops_domain mrail_domain_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = mrail_av_open,
	.cq_open = mrail_cq_open,
	.endpoint = mrail_ep_open,
	.scalable_ep = fi_no_scalable_ep,
	.cntr_open = mrail_cntr_open,
	.poll_open = fi_no_poll_open,
	.stx_ctx = fi_no_stx_context,
	.srx_ctx = fi_no_srx_context,
	.query_atomic = fi_no_query_atomic,
	.query_collective = fi_no_query_collective,
};

int mrail_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		      struct fid_domain **domain, void *context)
{
	struct mrail_fabric *mrail_fabric =
		container_of(fabric, struct mrail_fabric, util_fabric.fabric_fid);
	struct mrail_domain *mrail_domain;
	struct fi_info *fi;
	size_t i;
	int ret;

	assert(!strcmp(mrail_fabric->info->fabric_attr->name, info->fabric_attr->name));

	mrail_domain = calloc(1, sizeof(*mrail_domain));
	if (!mrail_domain)
		return -FI_ENOMEM;

	ret = ofi_domain_init(fabric, info, &mrail_domain->util_domain, context,
			      OFI_LOCK_MUTEX);
	if (ret) {
		free(mrail_domain);
		return ret;
	}

	mrail_domain->info = mrail_fabric->info;
	mrail_domain->num_domains = mrail_fabric->num_fabrics;

	mrail_domain->domains = calloc(mrail_domain->num_domains,
				       sizeof(*mrail_domain->domains));
	if (!mrail_domain->domains) {
		ret = -FI_ENOMEM;
		goto err;
	}

	for (i = 0, fi = mrail_domain->info->next; fi; fi = fi->next, i++) {
		ret = fi_domain(mrail_fabric->fabrics[i], fi,
				&mrail_domain->domains[i], context);
		if (ret)
			goto err;

		mrail_domain->addrlen += fi->src_addrlen;
	}

	*domain = &mrail_domain->util_domain.domain_fid;
	(*domain)->fid.ops = &mrail_domain_fi_ops;
	(*domain)->mr = &mrail_domain_mr_ops;
	(*domain)->ops = &mrail_domain_ops;

	return 0;
err:
	mrail_domain_close(&mrail_domain->util_domain.domain_fid.fid);
	return ret;
}
