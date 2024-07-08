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

#include "psmx3.h"

extern int psmx3_trx_ctxt_cnt;
struct psmx3_fid_fabric *psmx3_active_fabric = NULL;

static int psmx3_fabric_close(fid_t fid)
{
	struct psmx3_fid_fabric *fabric;

	fabric = container_of(fid, struct psmx3_fid_fabric,
			      util_fabric.fabric_fid.fid);

	psmx3_fabric_release(fabric);

	PSMX3_INFO(&psmx3_prov, FI_LOG_CORE, "refcnt=%d\n",
		ofi_atomic_get32(&fabric->util_fabric.ref));

	if (ofi_fabric_close(&fabric->util_fabric))
		return 0;

	if (psmx3_env.name_server)
		ofi_ns_stop_server(&fabric->name_server);

	ofi_spin_destroy(&fabric->domain_lock);
	assert(fabric == psmx3_active_fabric);
	psmx3_active_fabric = NULL;
	free(fabric);

	psmx3_atomic_global_fini();
	return 0;
}

static struct fi_ops psmx3_fabric_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = psmx3_fabric_close,
};

static struct fi_ops_fabric psmx3_fabric_ops = {
	.size = sizeof(struct fi_ops_fabric),
	.domain = psmx3_domain_open,
	.passive_ep = fi_no_passive_ep,
	.eq_open = ofi_eq_create,
	.wait_open = psmx3_wait_open,
	.trywait = psmx3_wait_trywait
};

static struct fi_fabric_attr psmx3_fabric_attr = {
	.name = NULL,
};

int psmx3_fabric(struct fi_fabric_attr *attr,
		 struct fid_fabric **fabric, void *context)
{
	struct psmx3_fid_fabric *fabric_priv;
	int ret;

	PSMX3_INFO(&psmx3_prov, FI_LOG_CORE, "\n");

	if (psmx3_active_fabric) {
		psmx3_fabric_acquire(psmx3_active_fabric);
		*fabric = &psmx3_active_fabric->util_fabric.fabric_fid;
		return 0;
	}

	fabric_priv = calloc(1, sizeof(*fabric_priv));
	if (!fabric_priv)
		return -FI_ENOMEM;

	ofi_spin_init(&fabric_priv->domain_lock);
	dlist_init(&fabric_priv->domain_list);

	psmx3_get_uuid(fabric_priv->uuid);
	if (psmx3_env.name_server) {
		fabric_priv->name_server.port = psmx3_uuid_to_port(fabric_priv->uuid);
		fabric_priv->name_server.name_len = sizeof(struct psmx3_ep_name);
		fabric_priv->name_server.service_len = sizeof(int);
		fabric_priv->name_server.service_cmp = psmx3_ns_service_cmp;
		fabric_priv->name_server.is_service_wildcard = psmx3_ns_is_service_wildcard;

		ofi_ns_init(&fabric_priv->name_server);
		ofi_ns_start_server(&fabric_priv->name_server);
	}

	psmx3_fabric_attr.prov_version = get_psm3_provider_version();
	ret = ofi_fabric_init(&psmx3_prov, &psmx3_fabric_attr, attr,
			     &fabric_priv->util_fabric, context);
	if (ret) {
		PSMX3_INFO(&psmx3_prov, FI_LOG_CORE, "ofi_fabric_init returns %d\n", ret);
		if (psmx3_env.name_server)
			ofi_ns_stop_server(&fabric_priv->name_server);
		free(fabric_priv);
		return ret;
	}

	/* fclass & context initialized in ofi_fabric_init */
	fabric_priv->util_fabric.fabric_fid.fid.ops = &psmx3_fabric_fi_ops;
	fabric_priv->util_fabric.fabric_fid.ops = &psmx3_fabric_ops;

	psmx3_atomic_global_init();

	/* take the reference to count for multiple fabric open calls */
	psmx3_fabric_acquire(fabric_priv);

	*fabric = &fabric_priv->util_fabric.fabric_fid;
	psmx3_active_fabric = fabric_priv;
	psmx3_trx_ctxt_cnt = 0;

	return 0;
}

