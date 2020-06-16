/*
 * Copyright (c) 2016 Intel Corporation, Inc.  All rights reserved.
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
#include <string.h>

#include <ofi_enosys.h>
#include <ofi_util.h>


int ofi_domain_bind_eq(struct util_domain *domain, struct util_eq *eq)
{
	if (domain->eq) {
		FI_WARN(domain->prov, FI_LOG_DOMAIN,
			"duplicate EQ binding\n");
		return -FI_EINVAL;
	}

	domain->eq = eq;
	ofi_atomic_inc32(&eq->ref);
	return 0;
}

int ofi_domain_close(struct util_domain *domain)
{
	if (ofi_atomic_get32(&domain->ref))
		return -FI_EBUSY;

	if (domain->mr_map.rbtree)
		ofi_mr_map_close(&domain->mr_map);

	fastlock_acquire(&domain->fabric->lock);
	dlist_remove(&domain->list_entry);
	fastlock_release(&domain->fabric->lock);

	free(domain->name);
	fastlock_destroy(&domain->lock);
	ofi_atomic_dec32(&domain->fabric->ref);
	return 0;
}

static struct fi_ops_mr util_domain_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = fi_no_mr_reg,
	.regv = fi_no_mr_regv,
	.regattr = fi_no_mr_regattr,
};

static int util_domain_init(struct util_domain *domain,
			    const struct fi_info *info)
{
	ofi_atomic_initialize32(&domain->ref, 0);
	fastlock_init(&domain->lock);
	domain->info_domain_caps = info->caps | info->domain_attr->caps;
	domain->info_domain_mode = info->mode | info->domain_attr->mode;
	domain->mr_mode = info->domain_attr->mr_mode;
	domain->addr_format = info->addr_format;
	domain->av_type = info->domain_attr->av_type;
	domain->name = strdup(info->domain_attr->name);
	domain->threading = info->domain_attr->threading;
	domain->data_progress = info->domain_attr->data_progress;
	return domain->name ? 0 : -FI_ENOMEM;
}

int ofi_domain_init(struct fid_fabric *fabric_fid, const struct fi_info *info,
		   struct util_domain *domain, void *context)
{
	struct util_fabric *fabric;
	int ret;

	fabric = container_of(fabric_fid, struct util_fabric, fabric_fid);
	domain->fabric = fabric;
	domain->prov = fabric->prov;
	ret = util_domain_init(domain, info);
	if (ret)
		return ret;

	domain->domain_fid.fid.fclass = FI_CLASS_DOMAIN;
	domain->domain_fid.fid.context = context;
	/*
	 * domain ops set by provider
	 */
	domain->domain_fid.mr = &util_domain_mr_ops;

	ret = ofi_mr_map_init(domain->prov, info->domain_attr->mr_mode,
			      &domain->mr_map);
	if (ret) {
		(void) ofi_domain_close(domain);
		return ret;
	}

	fastlock_acquire(&fabric->lock);
	dlist_insert_tail(&domain->list_entry, &fabric->domain_list);
	fastlock_release(&fabric->lock);

	ofi_atomic_inc32(&fabric->ref);
	return 0;
}
