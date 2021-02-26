/*
 * Copyright (c) 2019 Amazon.com, Inc. or its affiliates.
 * All rights reserved.
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

#include <ofi_perf.h>

#include "rxr.h"

#ifdef RXR_PERF_ENABLED
const char *rxr_perf_counters_str[] = {
	RXR_PERF_FOREACH(OFI_STR)
};
#endif

static struct fi_ops_fabric rxr_fabric_ops = {
	.size = sizeof(struct fi_ops_fabric),
	.domain = rxr_domain_open,
	.passive_ep = fi_no_passive_ep,
	.eq_open = ofi_eq_create,
	.wait_open = ofi_wait_fd_open,
	.trywait = ofi_trywait
};

static int rxr_fabric_close(fid_t fid)
{
	int ret;
	struct rxr_fabric *rxr_fabric;

	rxr_fabric = container_of(fid, struct rxr_fabric,
				  util_fabric.fabric_fid.fid);
	ret = fi_close(&rxr_fabric->lower_fabric->fid);
	if (ret)
		return ret;

	if (rxr_env.enable_shm_transfer) {
		ret = fi_close(&rxr_fabric->shm_fabric->fid);
		if (ret)
			return ret;
	}

	ret = ofi_fabric_close(&rxr_fabric->util_fabric);
	if (ret)
		return ret;

#ifdef RXR_PERF_ENABLED
	ofi_perfset_log(&rxr_fabric->perf_set, rxr_perf_counters_str);
	ofi_perfset_close(&rxr_fabric->perf_set);
#endif
	free(rxr_fabric);
	return 0;
}

static struct fi_ops rxr_fabric_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = rxr_fabric_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

int rxr_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fabric,
	       void *context)
{
	struct rxr_fabric *rxr_fabric;
	struct fi_info hints, *rdm_info;
	int ret, retv;

	rxr_fabric = calloc(1, sizeof(*rxr_fabric));
	if (!rxr_fabric)
		return -FI_ENOMEM;

	ret = ofi_fabric_init(&rxr_prov, &rxr_fabric_attr, attr,
			      &rxr_fabric->util_fabric, context);
	if (ret)
		goto err_free_fabric;

	memset(&hints, 0, sizeof(hints));
	hints.fabric_attr = calloc(1, sizeof(*hints.fabric_attr));
	if (!hints.fabric_attr) {
		ret = -FI_ENOMEM;
		goto err_close_util_fabric;
	}
	hints.fabric_attr->name = attr->name;
	hints.fabric_attr->api_version = attr->api_version;
	hints.mode = ~0;

	ret = lower_efa_prov->getinfo(attr->api_version, NULL, NULL, 0, &hints,
				      &rdm_info);
	if (ret) {
		FI_WARN(&rxr_prov, FI_LOG_FABRIC,
			"Unable to get core info!\n");
		ret = -FI_EINVAL;
		goto err_free_hints;
	}

	ret = lower_efa_prov->fabric(rdm_info->fabric_attr,
				     &rxr_fabric->lower_fabric, context);
	if (ret)
		goto err_free_rdm_info;

	/* Open shm provider's fabric domain */
	if (rxr_env.enable_shm_transfer) {
		assert(!strcmp(shm_info->fabric_attr->name, "shm"));
		ret = fi_fabric(shm_info->fabric_attr,
				       &rxr_fabric->shm_fabric, context);
		if (ret)
			goto err_close_rdm_fabric;
	}


#ifdef RXR_PERF_ENABLED
	ret = ofi_perfset_create(&rxr_prov, &rxr_fabric->perf_set,
				 rxr_perf_size, perf_domain, perf_cntr,
				 perf_flags);

	if (ret)
		FI_WARN(&rxr_prov, FI_LOG_FABRIC,
			"Error initializing RxR perfset: %s\n",
			fi_strerror(-ret));
#endif

	*fabric = &rxr_fabric->util_fabric.fabric_fid;
	(*fabric)->fid.ops = &rxr_fabric_fi_ops;
	(*fabric)->ops = &rxr_fabric_ops;

	free(hints.fabric_attr);
	fi_freeinfo(rdm_info);
	return 0;

err_close_rdm_fabric:
	retv = fi_close(&rxr_fabric->lower_fabric->fid);
	if (retv)
		FI_WARN(&rxr_prov, FI_LOG_FABRIC,
			"Unable to close lower rdm fabric: %s\n",
			fi_strerror(-retv));
err_free_rdm_info:
	fi_freeinfo(rdm_info);
err_free_hints:
	free(hints.fabric_attr);
err_close_util_fabric:
	retv = ofi_fabric_close(&rxr_fabric->util_fabric);
	if (retv)
		FI_WARN(&rxr_prov, FI_LOG_FABRIC,
			"Unable to close fabric: %s\n",
			fi_strerror(-retv));
err_free_fabric:
	free(rxr_fabric);
	return ret;
}
