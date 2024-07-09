/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright (c) 2014-2016, Cisco Systems, Inc. All rights reserved. */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "config.h"

#include <netdb.h>
#include <inttypes.h>

#include <infiniband/efadv.h>

#include <rdma/fabric.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_errno.h>

#include "ofi_prov.h"
#include <ofi_util.h>

#include "efa.h"
#include "efa_prov_info.h"
#ifdef EFA_PERF_ENABLED
const char *efa_perf_counters_str[] = {
	EFA_PERF_FOREACH(OFI_STR)
};
#endif


static int efa_fabric_close(fid_t fid)
{
	struct efa_fabric *efa_fabric;
	int ret;

	efa_fabric = container_of(fid, struct efa_fabric, util_fabric.fabric_fid.fid);
	ret = ofi_fabric_close(&efa_fabric->util_fabric);
	if (ret) {
		EFA_WARN(FI_LOG_FABRIC,
			"Unable to close fabric: %s\n",
			fi_strerror(-ret));
		return ret;
	}

	if (efa_fabric->shm_fabric) {
		ret = fi_close(&efa_fabric->shm_fabric->fid);
		if (ret) {
			EFA_WARN(FI_LOG_FABRIC,
				"Unable to close fabric: %s\n",
				fi_strerror(-ret));
			return ret;
		}
	}

#ifdef EFA_PERF_ENABLED
	ofi_perfset_log(&efa_fabric->perf_set, efa_perf_counters_str);
	ofi_perfset_close(&efa_fabric->perf_set);
#endif
	free(efa_fabric);

	return 0;
}

static struct fi_ops efa_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = efa_fabric_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_fabric efa_ops_fabric = {
	.size = sizeof(struct fi_ops_fabric),
	.domain = efa_domain_open,
	.passive_ep = fi_no_passive_ep,
	.eq_open = ofi_eq_create,
	.wait_open = ofi_wait_fd_open,
	.trywait = ofi_trywait
};

int efa_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fabric_fid,
	       void *context)
{
	const struct fi_info *info;
	struct efa_fabric *efa_fabric;
	int ret = 0;

	efa_fabric = calloc(1, sizeof(*efa_fabric));
	if (!efa_fabric)
		return -FI_ENOMEM;

	for (info = efa_util_prov.info; info; info = info->next) {
		ret = ofi_fabric_init(&efa_prov, info->fabric_attr, attr,
				      &efa_fabric->util_fabric, context);
		if (ret != -FI_ENODATA)
			break;
	}

	if (ret)
		goto err_free_fabric;

#ifdef EFA_PERF_ENABLED
	ret = ofi_perfset_create(&efa_prov, &efa_fabric->perf_set,
				 efa_perf_size, perf_domain, perf_cntr,
				 perf_flags);

	if (ret)
		EFA_WARN(FI_LOG_FABRIC,
			"Error initializing EFA perfset: %s\n",
			fi_strerror(-ret));
#endif


	*fabric_fid = &efa_fabric->util_fabric.fabric_fid;
	(*fabric_fid)->fid.fclass = FI_CLASS_FABRIC;
	(*fabric_fid)->fid.ops = &efa_fi_ops;
	(*fabric_fid)->ops = &efa_ops_fabric;
	(*fabric_fid)->api_version = attr->api_version;

	return 0;

err_free_fabric:
	free(efa_fabric);

	return ret;
}
