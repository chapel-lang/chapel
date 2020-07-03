/*
 * Copyright (c) 2013-2017 Intel Corporation. All rights reserved.
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

#include "psmx.h"

struct psmx_fid_fabric *psmx_active_fabric = NULL;

static int psmx_fabric_close(fid_t fid)
{
	struct psmx_fid_fabric *fabric;

	fabric = container_of(fid, struct psmx_fid_fabric,
			      util_fabric.fabric_fid.fid);

	FI_INFO(&psmx_prov, FI_LOG_CORE, "refcnt=%d\n",
		ofi_atomic_get32(&fabric->util_fabric.ref));

	if (psmx_env.name_server)
		ofi_ns_stop_server(&fabric->name_server);

	psmx_fabric_release(fabric);

	if (ofi_fabric_close(&fabric->util_fabric))
		return 0;

	if (fabric->active_domain) {
		FI_WARN(&psmx_prov, FI_LOG_CORE, "forced closing of active_domain\n");
		fi_close(&fabric->active_domain->util_domain.domain_fid.fid);
	}
	assert(fabric == psmx_active_fabric);
	psmx_active_fabric = NULL;
	free(fabric);

	return 0;
}

static struct fi_ops psmx_fabric_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = psmx_fabric_close,
};

static struct fi_ops_fabric psmx_fabric_ops = {
	.size = sizeof(struct fi_ops_fabric),
	.domain = psmx_domain_open,
	.passive_ep = fi_no_passive_ep,
	.eq_open = ofi_eq_create,
	.wait_open = psmx_wait_open,
	.trywait = psmx_wait_trywait,
};

static struct fi_fabric_attr psmx_fabric_attr = {
	.name = PSMX_FABRIC_NAME,
	.prov_version = OFI_VERSION_DEF_PROV,
};

int psmx_fabric(struct fi_fabric_attr *attr,
		struct fid_fabric **fabric, void *context)
{
	struct psmx_fid_fabric *fabric_priv;
	int ret;

	FI_INFO(&psmx_prov, FI_LOG_CORE, "\n");

	if (strcmp(attr->name, PSMX_FABRIC_NAME))
		return -FI_ENODATA;

	if (psmx_active_fabric) {
		psmx_fabric_acquire(psmx_active_fabric);
		*fabric = &psmx_active_fabric->util_fabric.fabric_fid;
		return 0;
	}

	fabric_priv = calloc(1, sizeof(*fabric_priv));
	if (!fabric_priv)
		return -FI_ENOMEM;

	psmx_get_uuid(fabric_priv->uuid);
	if (psmx_env.name_server) {
		fabric_priv->name_server.port = psmx_uuid_to_port(fabric_priv->uuid);
		fabric_priv->name_server.name_len = sizeof(psm_epid_t);
		fabric_priv->name_server.service_len = sizeof(int);
		fabric_priv->name_server.service_cmp = psmx_ns_service_cmp;
		fabric_priv->name_server.is_service_wildcard = psmx_ns_is_service_wildcard;

		ofi_ns_init(&fabric_priv->name_server);
		ofi_ns_start_server(&fabric_priv->name_server);
	}

	ret = ofi_fabric_init(&psmx_prov, &psmx_fabric_attr, attr,
			      &fabric_priv->util_fabric, context);
	if (ret) {
		FI_INFO(&psmx_prov, FI_LOG_CORE, "ofi_fabric_init returns %d\n", ret);
		if (psmx_env.name_server)
			ofi_ns_stop_server(&fabric_priv->name_server);
		free(fabric_priv);
		return ret;
	}

	/* fclass & context initialzied in ofi_fabric_init */
	fabric_priv->util_fabric.fabric_fid.fid.ops = &psmx_fabric_fi_ops;
	fabric_priv->util_fabric.fabric_fid.ops = &psmx_fabric_ops;

	psmx_query_mpi();

	/* take the reference to count for multiple fabric open calls */
	psmx_fabric_acquire(fabric_priv);

	*fabric = &fabric_priv->util_fabric.fabric_fid;
	psmx_active_fabric = fabric_priv;

	return 0;
}

