/*
 * Copyright (c) 2013-2018 Intel Corporation. All rights reserved.
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

#include "psmx2.h"

extern int psmx2_trx_ctxt_cnt;
struct psmx2_fid_fabric *psmx2_active_fabric = NULL;

static int psmx2_fabric_close(fid_t fid)
{
	struct psmx2_fid_fabric *fabric;

	fabric = container_of(fid, struct psmx2_fid_fabric,
			      util_fabric.fabric_fid.fid);

	psmx2_fabric_release(fabric);

	FI_INFO(&psmx2_prov, FI_LOG_CORE, "refcnt=%d\n",
		ofi_atomic_get32(&fabric->util_fabric.ref));

	if (ofi_fabric_close(&fabric->util_fabric))
		return 0;

	if (psmx2_env.name_server)
		ofi_ns_stop_server(&fabric->name_server);

	ofi_spin_destroy(&fabric->domain_lock);
	assert(fabric == psmx2_active_fabric);
	psmx2_active_fabric = NULL;
	free(fabric);

	psmx2_atomic_global_fini();
	return 0;
}

static struct fi_ops psmx2_fabric_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = psmx2_fabric_close,
};

static struct fi_ops_fabric psmx2_fabric_ops = {
	.size = sizeof(struct fi_ops_fabric),
	.domain = psmx2_domain_open,
	.passive_ep = fi_no_passive_ep,
	.eq_open = ofi_eq_create,
	.wait_open = psmx2_wait_open,
	.trywait = psmx2_wait_trywait
};

static struct fi_fabric_attr psmx2_fabric_attr = {
	.name = PSMX2_FABRIC_NAME,
	.prov_version = OFI_VERSION_DEF_PROV,
};

int psmx2_fabric(struct fi_fabric_attr *attr,
		 struct fid_fabric **fabric, void *context)
{
	struct psmx2_fid_fabric *fabric_priv;
	int ret;

	FI_INFO(&psmx2_prov, FI_LOG_CORE, "\n");

	if (strcmp(attr->name, PSMX2_FABRIC_NAME))
		return -FI_ENODATA;

	if (psmx2_active_fabric) {
		psmx2_fabric_acquire(psmx2_active_fabric);
		*fabric = &psmx2_active_fabric->util_fabric.fabric_fid;
		return 0;
	}

	fabric_priv = calloc(1, sizeof(*fabric_priv));
	if (!fabric_priv)
		return -FI_ENOMEM;

	ofi_spin_init(&fabric_priv->domain_lock);
	dlist_init(&fabric_priv->domain_list);

	if (psmx2_env.name_server) {
		psm2_uuid_t uuid;

		psmx2_get_uuid(uuid);
		fabric_priv->name_server.port = psmx2_uuid_to_port(uuid);
		fabric_priv->name_server.name_len = sizeof(struct psmx2_ep_name);
		fabric_priv->name_server.service_len = sizeof(int);
		fabric_priv->name_server.service_cmp = psmx2_ns_service_cmp;
		fabric_priv->name_server.is_service_wildcard = psmx2_ns_is_service_wildcard;

		ofi_ns_init(&fabric_priv->name_server);
		ofi_ns_start_server(&fabric_priv->name_server);
	}

	ret = ofi_fabric_init(&psmx2_prov, &psmx2_fabric_attr, attr,
			     &fabric_priv->util_fabric, context);
	if (ret) {
		FI_INFO(&psmx2_prov, FI_LOG_CORE, "ofi_fabric_init returns %d\n", ret);
		if (psmx2_env.name_server)
			ofi_ns_stop_server(&fabric_priv->name_server);
		free(fabric_priv);
		return ret;
	}

	/* fclass & context initialized in ofi_fabric_init */
	fabric_priv->util_fabric.fabric_fid.fid.ops = &psmx2_fabric_fi_ops;
	fabric_priv->util_fabric.fabric_fid.ops = &psmx2_fabric_ops;

	psmx2_atomic_global_init();
	psmx2_query_mpi();

	/* take the reference to count for multiple fabric open calls */
	psmx2_fabric_acquire(fabric_priv);

	*fabric = &fabric_priv->util_fabric.fabric_fid;
	psmx2_active_fabric = fabric_priv;
	psmx2_trx_ctxt_cnt = 0;

	return 0;
}

