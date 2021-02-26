/*
 * Copyright (c) 2013-2015 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2017-2020 Amazon.com, Inc. or its affiliates. All rights reserved.
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

#include "config.h"

#include <ofi_util.h>
#include "efa.h"
#include "rxr_cntr.h"

static int efa_domain_close(fid_t fid)
{
	struct efa_domain *domain;
	int ret;

	domain = container_of(fid, struct efa_domain,
			      util_domain.domain_fid.fid);

	if (efa_mr_cache_enable)
		ofi_mr_cache_cleanup(&domain->cache);

	if (domain->ibv_pd) {
		ret = -ibv_dealloc_pd(domain->ibv_pd);
		if (ret) {
			EFA_INFO_ERRNO(FI_LOG_DOMAIN, "ibv_dealloc_pd", ret);
			return ret;
		}
		domain->ibv_pd = NULL;
	}

	ret = ofi_domain_close(&domain->util_domain);
	if (ret)
		return ret;

	fi_freeinfo(domain->info);
	free(domain->qp_table);
	free(domain);
	return 0;
}

static int efa_open_device_by_name(struct efa_domain *domain, const char *name)
{
	struct efa_context **ctx_list;
	int i, ret = -FI_ENODEV;
	int name_len;
	int num_ctx;

	if (!name)
		return -FI_EINVAL;

	ctx_list = efa_device_get_context_list(&num_ctx);
	if (!ctx_list)
		return -errno;

	if (domain->type == EFA_DOMAIN_RDM)
		name_len = strlen(name) - strlen(efa_rdm_domain.suffix);
	else
		name_len = strlen(name) - strlen(efa_dgrm_domain.suffix);

	for (i = 0; i < num_ctx; i++) {
		ret = strncmp(name, ctx_list[i]->ibv_ctx->device->name, name_len);
		if (!ret) {
			domain->ctx = ctx_list[i];
			break;
		}
	}

	efa_device_free_context_list(ctx_list);
	return ret;
}

static struct fi_ops efa_fid_ops = {
	.size = sizeof(struct fi_ops),
	.close = efa_domain_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_domain efa_domain_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = efa_av_open,
	.cq_open = efa_cq_open,
	.endpoint = efa_ep_open,
	.scalable_ep = fi_no_scalable_ep,
	.cntr_open = efa_cntr_open,
	.poll_open = fi_no_poll_open,
	.stx_ctx = fi_no_stx_context,
	.srx_ctx = fi_no_srx_context,
	.query_atomic = fi_no_query_atomic,
	.query_collective = fi_no_query_collective,
};

int efa_domain_open(struct fid_fabric *fabric_fid, struct fi_info *info,
		    struct fid_domain **domain_fid, void *context)
{
	struct efa_domain *domain;
	struct efa_fabric *fabric;
	const struct fi_info *fi;
	size_t qp_table_size;
	int ret;

	fi = efa_get_efa_info(info->domain_attr->name);
	if (!fi)
		return -FI_EINVAL;

	fabric = container_of(fabric_fid, struct efa_fabric,
			      util_fabric.fabric_fid);
	ret = ofi_check_domain_attr(&efa_prov, fabric_fid->api_version,
				    fi->domain_attr, info);
	if (ret)
		return ret;

	domain = calloc(1, sizeof(*domain));
	if (!domain)
		return -FI_ENOMEM;

	qp_table_size = roundup_power_of_two(info->domain_attr->ep_cnt);
	domain->qp_table_sz_m1 = qp_table_size - 1;
	domain->qp_table = calloc(qp_table_size, sizeof(*domain->qp_table));
	if (!domain->qp_table) {
		ret = -FI_ENOMEM;
		goto err_free_domain;
	}

	ret = ofi_domain_init(fabric_fid, info, &domain->util_domain,
			      context);
	if (ret)
		goto err_free_qp_table;

	domain->info = fi_dupinfo(info);
	if (!domain->info) {
		ret = -FI_ENOMEM;
		goto err_close_domain;
	}

	if (EFA_EP_TYPE_IS_RDM(info))
		domain->type = EFA_DOMAIN_RDM;
	else
		domain->type = EFA_DOMAIN_DGRAM;

	ret = efa_open_device_by_name(domain, info->domain_attr->name);
	if (ret)
		goto err_free_info;

	domain->ibv_pd = ibv_alloc_pd(domain->ctx->ibv_ctx);
	if (!domain->ibv_pd) {
		ret = -errno;
		goto err_free_info;
	}

	domain->util_domain.domain_fid.fid.ops = &efa_fid_ops;
	domain->util_domain.domain_fid.ops = &efa_domain_ops;
	/* RMA mr_modes are being removed, since EFA layer
	 * does not have RMA capabilities. Hence, adding FI_MR_VIRT_ADDR
	 * until RMA capabilities are added to EFA layer
	 */
	domain->util_domain.mr_map.mode |= FI_MR_VIRT_ADDR;
	/*
	 * ofi_domain_init() would have stored the EFA mr_modes in the mr_map,
	 * but we need the rbtree insertions and lookups to use EFA provider's
	 * specific key, so unset the FI_MR_PROV_KEY bit for mr_map.
	 */
	domain->util_domain.mr_map.mode &= ~FI_MR_PROV_KEY;
	domain->fab = fabric;

	*domain_fid = &domain->util_domain.domain_fid;

	if (efa_mr_cache_enable) {
		if (!efa_mr_max_cached_count)
			efa_mr_max_cached_count = info->domain_attr->mr_cnt *
			                          EFA_MR_CACHE_LIMIT_MULT;
		if (!efa_mr_max_cached_size)
			efa_mr_max_cached_size = domain->ctx->max_mr_size *
			                         EFA_MR_CACHE_LIMIT_MULT;
		cache_params.max_cnt = efa_mr_max_cached_count;
		cache_params.max_size = efa_mr_max_cached_size;
		domain->cache.entry_data_size = sizeof(struct efa_mr);
		domain->cache.add_region = efa_mr_cache_entry_reg;
		domain->cache.delete_region = efa_mr_cache_entry_dereg;
		ret = ofi_mr_cache_init(&domain->util_domain, uffd_monitor,
					&domain->cache);
		if (!ret) {
			domain->util_domain.domain_fid.mr = &efa_domain_mr_cache_ops;
			EFA_INFO(FI_LOG_DOMAIN, "EFA MR cache enabled, max_cnt: %zu max_size: %zu\n",
			         cache_params.max_cnt, cache_params.max_size);
			return 0;
		}
	}

	domain->util_domain.domain_fid.mr = &efa_domain_mr_ops;
	efa_mr_cache_enable = 0;

	return 0;
err_free_info:
	fi_freeinfo(domain->info);
err_close_domain:
	ofi_domain_close(&domain->util_domain);
err_free_qp_table:
	free(domain->qp_table);
err_free_domain:
	free(domain);
	return ret;
}
