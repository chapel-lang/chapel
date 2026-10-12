/*
 * Copyright (c) 2018-2024 GigaIO, Inc. All Rights Reserved.
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

#include <ofi_prov.h>

#include "lpp.h"

static const struct fi_ops_fabric lpp_fabric_ops = {
	.size       = sizeof(struct fi_ops_fabric),
	.domain     = lpp_fi_domain,
	.passive_ep = fi_no_passive_ep,
	.eq_open    = lpp_fi_eq_open,
	.wait_open  = fi_no_wait_open,
	.trywait    = fi_no_trywait
};

static const struct fi_ops lpp_fi_fabric_ops = {
	.size       = sizeof(struct fi_ops),
	.close      = lpp_fi_fabric_close,
	.bind       = fi_no_bind,
	.control    = fi_no_control,
	.ops_open   = fi_no_ops_open,
};

static void lpp_fabric_fid(struct fid_fabric *fabric_fidp, void *context)
{
	if (fabric_fidp != NULL) {
		fabric_fidp->fid.fclass = FI_CLASS_FABRIC;
		fabric_fidp->fid.context = context;
		fabric_fidp->fid.ops = (struct fi_ops *)&lpp_fi_fabric_ops;
		fabric_fidp->ops = (struct fi_ops_fabric *)&lpp_fabric_ops;
	}
}

struct fi_fabric_attr lpp_fabric_default_attrs(const struct klppioc_lf *klpp_devinfo)
{
	struct fi_fabric_attr	attrs = { 0 };

	attrs.name = strdup(klpp_devinfo->fabric);
	attrs.prov_name = NULL;
	attrs.prov_version = OFI_VERSION_DEF_PROV;
	attrs.api_version = OFI_VERSION_DEF_PROV;

	return attrs;
}

void lpp_fabric_setup_attrs(const struct klppioc_lf *klpp_devinfo,
			    const struct fi_info *hints,
			    struct fi_fabric_attr *tmp_fabric) {
	tmp_fabric->name = strdup(klpp_devinfo->fabric);
	tmp_fabric->prov_name = NULL;
	tmp_fabric->prov_version = OFI_VERSION_DEF_PROV;
	tmp_fabric->api_version = OFI_VERSION_DEF_PROV;
}

int lpp_fabric_verify_attrs(const struct klppioc_lf *klpp_devinfo, const struct fi_fabric_attr *attrs)
{
	uint32_t version = OFI_VERSION_DEF_PROV;

	// Check the fabric hints.
	if (attrs != NULL) {
		if ((attrs->name != NULL) && (strcmp(attrs->name, klpp_devinfo->fabric) != 0)) {
			FI_INFO(&lpp_prov, FI_LOG_FABRIC, "fabric name '%s' does not match expected '%s'\n",
			    attrs->name, klpp_devinfo->fabric);
			return -FI_ENODATA;
		}

		if ((attrs->prov_name != NULL) && (strcmp(attrs->prov_name, "lpp") != 0)) {
			FI_INFO(&lpp_prov, FI_LOG_FABRIC, "prov name '%s' does not match expected '%s'\n",
			    attrs->prov_name, "lpp");
			return -FI_ENODATA;
		}

		if ((attrs->prov_version != 0) && (attrs->prov_version != version)) {
			FI_INFO(&lpp_prov, FI_LOG_FABRIC, "prov_version %d does not match expected %d\n",
			    attrs->prov_version, version);
			return -FI_ENODATA;
		}

		if ((attrs->api_version != 0) && (attrs->api_version != version)) {
			FI_INFO(&lpp_prov, FI_LOG_FABRIC, "api_version %d does not match expected %d\n",
			    attrs->api_version, version);
			return -FI_ENODATA;
		}
	}

	return 0;
}

int lpp_fi_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fabric, void *context)
{
	struct lpp_fabric *lpp_fabricp = NULL;

	// MPICH only sets the 'prov_name' field.  So we don't need to check the attributes.
	if (lpp_fabricp = calloc(1, sizeof(struct lpp_fabric)), lpp_fabricp == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_FABRIC, "failed to calloc fabric object\n");
		return -FI_ENOMEM;
	}

	lpp_fabric_fid(&lpp_fabricp->fabric_fid, context);

	ofi_atomic_initialize32(&lpp_fabricp->ref_count, 0);

	*fabric = (struct fid_fabric *)lpp_fabricp;

	return 0;
}

int lpp_fi_fabric_close(struct fid *fid)
{
	struct lpp_fabric *lpp_fabricp = NULL;

	lpp_fabricp = container_of(fid, struct lpp_fabric, fabric_fid);
	if (ofi_atomic_get32(&lpp_fabricp->ref_count)) {
		FI_WARN(&lpp_prov, FI_LOG_FABRIC, "can't close fabric, ref_count != 0\n");
		return -FI_EBUSY;
	}
	free(lpp_fabricp);
	return 0;
}
