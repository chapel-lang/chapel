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
#include <assert.h>
#include <ofi_util.h>

#include "config.h"
#include "efa.h"
#include "rxr.h"
#include "rxr_cntr.h"
#include "rxr_atomic.h"

struct dlist_entry g_efa_domain_list;

static int efa_domain_close(fid_t fid);

static struct fi_ops efa_ops_domain_fid = {
	.size = sizeof(struct fi_ops),
	.close = efa_domain_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_domain efa_ops_domain_dgram = {
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

static struct fi_ops_domain efa_ops_domain_rdm = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = efa_av_open,
	.cq_open = rxr_cq_open,
	.endpoint = rxr_endpoint,
	.scalable_ep = fi_no_scalable_ep,
	.cntr_open = efa_cntr_open,
	.poll_open = fi_poll_create,
	.stx_ctx = fi_no_stx_context,
	.srx_ctx = fi_no_srx_context,
	.query_atomic = rxr_query_atomic,
	.query_collective = fi_no_query_collective,
};

/**
 * @brief init the device and ibv_pd field in efa_domain
 *
 * @param efa_domain[in,out]	efa domain to be set.
 * @param domain_name		domain name
 * @return 0 if efa_domain->device and efa_domain->ibv_pd has been set successfully
 *         negative error code if err is encountered
 */
static int efa_domain_init_device_and_pd(struct efa_domain *efa_domain, const char *domain_name)
{
	int i;

	if (!domain_name)
		return -FI_EINVAL;

	for (i = 0; i < g_device_cnt; i++) {
		if (strstr(domain_name, g_device_list[i].ibv_ctx->device->name) == domain_name) {
			efa_domain->device = &g_device_list[i];
			break;
		}
	}

	if (i == g_device_cnt)
		return -FI_ENODEV;

	efa_domain->ibv_pd = efa_domain->device->ibv_pd;
	return 0;
}

static int efa_domain_init_qp_table(struct efa_domain *efa_domain)
{
	size_t qp_table_size;

	qp_table_size = roundup_power_of_two(efa_domain->device->ibv_attr.max_qp);
	efa_domain->qp_table_sz_m1 = qp_table_size - 1;
	efa_domain->qp_table = calloc(qp_table_size, sizeof(*efa_domain->qp_table));
	if (!efa_domain->qp_table)
		return -FI_ENOMEM;

	return 0;
}

static int efa_domain_init_rdm(struct efa_domain *efa_domain, struct fi_info *info)
{
	int err;

	if (efa_domain->fabric->shm_fabric) {
		assert(!strcmp(g_shm_info->fabric_attr->name, "shm"));
		err = fi_domain(efa_domain->fabric->shm_fabric, g_shm_info,
				&efa_domain->shm_domain, NULL);
		if (err)
			return err;
	}

	efa_domain->rdm_mode = info->mode;
	efa_domain->mtu_size = efa_domain->device->ibv_port_attr.max_msg_sz;
	efa_domain->addrlen = (info->src_addr) ? info->src_addrlen : info->dest_addrlen;
	efa_domain->rdm_cq_size = MAX(info->rx_attr->size + info->tx_attr->size,
				  rxr_env.cq_size);
	return 0;
}

/* @brief Allocate a domain, open the device, and set it up based on the hints.
 *
 * This function creates a domain and uses the info struct to configure the
 * domain based on what capabilities are set. Fork support is checked here and
 * the MR cache is also set up here.
 *
 * @param fabric_fid fabric that the domain should be tied to
 * @param info info struct that was validated and returned by fi_getinfo
 * @param domain_fid pointer where newly domain fid should be stored
 * @param context void pointer stored with the domain fid
 * @return 0 on success, fi_errno on error
 */
int efa_domain_open(struct fid_fabric *fabric_fid, struct fi_info *info,
		    struct fid_domain **domain_fid, void *context)
{
	struct efa_domain *efa_domain;
	int ret = 0, err;

	efa_domain = calloc(1, sizeof(struct efa_domain));
	if (!efa_domain)
		return -FI_ENOMEM;

	dlist_init(&efa_domain->list_entry);
	efa_domain->fabric = container_of(fabric_fid, struct efa_fabric,
					  util_fabric.fabric_fid);

	err = ofi_domain_init(fabric_fid, info, &efa_domain->util_domain,
			      context, OFI_LOCK_MUTEX);
	if (err) {
		ret = err;
		goto err_free;
	}
	efa_domain->util_domain.av_type = FI_AV_TABLE;
	efa_domain->util_domain.mr_map.mode |= FI_MR_VIRT_ADDR;
	/*
	 * FI_MR_PROV_KEY means provider will generate a key for MR,
	 * which EFA provider does by using key generated by EFA device.
	 *
	 * util_domain.mr_map.mode is same as info->mode, which has
	 * the bit FI_MR_PROV_KEY on. When the bit is on, util_domain.mr_map
	 * will generate a key for MR, which is not what we want
	 * (we want to use the key generated by device). Therefore unset
	 * the FI_MR_PROV_KEY bit of mr_map.
	 */
	efa_domain->util_domain.mr_map.mode &= ~FI_MR_PROV_KEY;

	if (!info->ep_attr || info->ep_attr->type == FI_EP_UNSPEC) {
		EFA_WARN(FI_LOG_DOMAIN, "ep type not specified when creating domain");
		return -FI_EINVAL;
	}


	efa_domain->mr_local = ofi_mr_local(info);
	if (EFA_EP_TYPE_IS_DGRAM(info) && !efa_domain->mr_local) {
		EFA_WARN(FI_LOG_EP_DATA, "dgram require FI_MR_LOCAL, but application does not support it\n");
		ret = -FI_ENODATA;
		goto err_free;
	}

	err = efa_domain_init_device_and_pd(efa_domain, info->domain_attr->name);
	if (err) {
		ret = err;
		goto err_free;
	}

	efa_domain->info = fi_dupinfo(EFA_EP_TYPE_IS_RDM(info) ? efa_domain->device->rdm_info : efa_domain->device->dgram_info);
	if (!efa_domain->info) {
		ret = -FI_ENOMEM;
		goto err_free;
	}

	*domain_fid = &efa_domain->util_domain.domain_fid;

	err = efa_domain_init_qp_table(efa_domain);
	if (err) {
		ret = err;
		EFA_WARN(FI_LOG_DOMAIN, "Failed to init qp table. err: %d", ret);
		goto err_free;
	}

	/*
	 * FI_MR_LOCAL means application will handle memory registration by itself.
	 * Therefore when FI_MR_LOCAL is on, MR cache is not necessary.
	 */
	if (!efa_domain->mr_local && efa_mr_cache_enable) {
		err = efa_mr_cache_open(&efa_domain->cache, efa_domain);
		if (err) {
			ret = err;
			goto err_free;
		}

		efa_domain->util_domain.domain_fid.mr = &efa_domain_mr_cache_ops;
	} else {
		efa_domain->util_domain.domain_fid.mr = &efa_domain_mr_ops;
	}

	efa_domain->util_domain.domain_fid.fid.ops = &efa_ops_domain_fid;
	if (EFA_EP_TYPE_IS_RDM(info)) {
		err = efa_domain_init_rdm(efa_domain, info);
		if (err) {
			EFA_WARN(FI_LOG_DOMAIN,
				 "efa_domain_init_rdm failed. err: %d\n",
				 -err);
			goto err_free;
		}
		efa_domain->util_domain.domain_fid.ops = &efa_ops_domain_rdm;
	} else {
		assert(EFA_EP_TYPE_IS_DGRAM(info));
		efa_domain->util_domain.domain_fid.ops = &efa_ops_domain_dgram;
	}

	err = efa_fork_support_enable_if_requested(*domain_fid);
	if (err) {
		ret = err;
		EFA_WARN(FI_LOG_DOMAIN, "Failed to initialize fork support. err: %d", ret);
		goto err_free;
	}

	err = efa_hmem_info_update_all(efa_domain);
	if (err) {
		ret = err;
		EFA_WARN(FI_LOG_DOMAIN, "Failed to check hmem support status. err: %d", ret);
		goto err_free;
	}

	dlist_insert_tail(&efa_domain->list_entry, &g_efa_domain_list);
	return 0;

err_free:
	assert(efa_domain);

	err = efa_domain_close(&efa_domain->util_domain.domain_fid.fid);
	if (err) {
		EFA_WARN(FI_LOG_DOMAIN, "When handling error (%d), domain resource was being released."
			 "During the release process, an addtional error (%d) was encoutered\n",
			 -ret, -err);
	}

	efa_domain = NULL;
	*domain_fid = NULL;
	return ret;
}

static int efa_domain_close(fid_t fid)
{
	struct efa_domain *efa_domain;
	int ret;

	efa_domain = container_of(fid, struct efa_domain,
				  util_domain.domain_fid.fid);

	dlist_remove(&efa_domain->list_entry);

	if (efa_domain->cache) {
		ofi_mr_cache_cleanup(efa_domain->cache);
		free(efa_domain->cache);
		efa_domain->cache = NULL;
	}

	if (efa_domain->ibv_pd) {
		efa_domain->ibv_pd = NULL;
	}

	ret = ofi_domain_close(&efa_domain->util_domain);
	if (ret)
		return ret;

	if (efa_domain->shm_domain) {
		ret = fi_close(&efa_domain->shm_domain->fid);
		if (ret)
			return ret;
	}

	if (efa_domain->info)
		fi_freeinfo(efa_domain->info);
	free(efa_domain->qp_table);
	free(efa_domain);
	return 0;
}

