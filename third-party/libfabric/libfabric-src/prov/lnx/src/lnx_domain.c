/*
 * Copyright (c) 2022 ORNL. All rights reserved.
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#include <rdma/fi_errno.h>
#include "ofi_util.h"
#include "ofi.h"
#include "ofi_str.h"
#include "ofi_prov.h"
#include "ofi_perf.h"
#include "ofi_hmem.h"
#include "rdma/fi_ext.h"
#include "lnx.h"

static struct fi_ops_domain lnx_domain_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = lnx_av_open,
	.cq_open = lnx_cq_open,
	.endpoint = lnx_endpoint,
	.scalable_ep = fi_no_scalable_ep,
	.cntr_open = fi_no_cntr_open,
	.poll_open = fi_no_poll_open,
	.stx_ctx = fi_no_stx_context,
	.srx_ctx = fi_no_srx_context,
	.query_atomic = fi_no_query_atomic,
	.query_collective = fi_no_query_collective,
};

static int lnx_domain_close(struct fid *fid)
{
	int i, rc, frc = 0;
	struct lnx_domain *domain;
	struct lnx_core_domain *cd;

	domain = container_of(fid, struct lnx_domain, ld_domain.domain_fid.fid);


	/* close all the open core domains */
	for (i = 0; i < domain->ld_num_doms; i++) {
		cd = &domain->ld_core_domains[i];

		rc = fi_close(&cd->cd_domain->fid);
		if (rc)
			frc = rc;
	}

	ofi_bufpool_destroy(domain->ld_mem_reg_bp);

	rc = ofi_domain_close(&domain->ld_domain);
	if (rc)
		frc = rc;

	free(domain->ld_core_domains);
	free(domain);

	return frc;
}

static struct fi_ops lnx_domain_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = lnx_domain_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_mr lnx_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = fi_no_mr_reg,
	.regv = fi_no_mr_regv,
	.regattr = lnx_mr_regattr,
};

static int lnx_open_core_domains(struct lnx_fabric *lnx_fab,
				void *context, struct lnx_domain *lnx_domain)
{
	int rc, i, num_shm_dom = 0, inter_dom_start;
	char *prov_name;
	bool local;
	struct fi_info *itr;
	struct lnx_core_fabric *cf;
	struct lnx_core_domain *cd;

	for (i = 0; i < lnx_fab->lf_num_fabs; i++) {
		cf = &lnx_fab->lf_core_fabrics[i];
		local = false;
		prov_name = cf->cf_info->fabric_attr->name;
		if (!strcmp(prov_name, "shm"))
			local = true;
		for (itr = cf->cf_info; itr; itr = itr->next) {
			if (local)
				num_shm_dom++;
			lnx_domain->ld_num_doms++;
		}
	}

	if (lnx_domain->ld_num_doms >= LNX_MAX_LOCAL_EPS) {
		FI_WARN(&lnx_prov, FI_LOG_FABRIC,
			"Too many domains to link. Maximum allowed: %d\n",
			LNX_MAX_LOCAL_EPS);
		return -FI_E2BIG;
	}

	inter_dom_start = num_shm_dom;

	lnx_domain->ld_core_domains = calloc(sizeof(*lnx_domain->ld_core_domains),
					     lnx_domain->ld_num_doms);
	if (!lnx_domain->ld_core_domains)
		return -FI_ENOMEM;

	for (i = 0; i < lnx_fab->lf_num_fabs; i++) {
		cf = &lnx_fab->lf_core_fabrics[i];

		local = false;
		prov_name = cf->cf_info->fabric_attr->name;
		/* special case for CXI provider. We need to turn off tag
		 * matching HW offload if we're going to support shared
		 * receive queues.
		 */
		if (strstr(prov_name, "cxi"))
			setenv("FI_CXI_RX_MATCH_MODE", "software", 1);
		else if (!strcmp(prov_name, "shm"))
			local = true;

		for (itr = cf->cf_info; itr; itr = itr->next) {
			/* keep the shm domain at the head of the list.
			 * This will cause all the other shm constructs to
			 * be the head of their respective lists, ex: av.
			 * The purpose is to optimize the shm path for
			 * local peers.
			 */
			if (local) {
				num_shm_dom--;
				cd = &lnx_domain->ld_core_domains[num_shm_dom];
			} else {
				cd = &lnx_domain->ld_core_domains[inter_dom_start];
				inter_dom_start++;
			}

			cd->cd_info = itr;

			rc = fi_domain(cf->cf_fabric, cd->cd_info,
				       &cd->cd_domain, context);
			if (rc) {
				free(cd);
				return rc;
			}
			cd->cd_fabric = cf;
		}
	}

	return 0;
}

int lnx_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		struct fid_domain **domain, void *context)
{
	int rc = 0;
	struct lnx_domain *lnx_domain;
	struct util_domain *dom;
	struct ofi_bufpool_attr bp_attrs = {0};
	struct lnx_fabric *lnx_fab = container_of(fabric, struct lnx_fabric,
					lf_util_fabric.fabric_fid);

	rc = -FI_ENOMEM;
	lnx_domain = calloc(sizeof(*lnx_domain), 1);
	if (!lnx_domain)
		goto out;

	bp_attrs.size = sizeof(struct lnx_mr);
	bp_attrs.alignment = 8;
	bp_attrs.max_cnt = UINT32_MAX;
	bp_attrs.chunk_cnt = 256;
	bp_attrs.flags = OFI_BUFPOOL_NO_TRACK;
	rc = ofi_bufpool_create_attr(&bp_attrs, &lnx_domain->ld_mem_reg_bp);
	if (rc)
		goto out;

	rc = lnx_setup_fabrics(info->domain_attr->name, lnx_fab, context);
	if (rc)
		goto fail;

	lnx_domain->ld_iov_limit = info->tx_attr->iov_limit;
	dom = &lnx_domain->ld_domain;
	rc = ofi_domain_init(fabric, info, dom, context,
			     OFI_LOCK_SPINLOCK);
	if (rc)
		goto fail;

	rc = lnx_open_core_domains(lnx_fab, context, lnx_domain);
	if (rc) {
		FI_INFO(&lnx_prov, FI_LOG_CORE, "Failed to initialize domain for %s\n",
			info->domain_attr->name);
		goto close_domain;
	}

	dom->domain_fid.fid.ops = &lnx_domain_fi_ops;
	dom->domain_fid.ops = &lnx_domain_ops;
	dom->domain_fid.mr = &lnx_mr_ops;

	*domain = &dom->domain_fid;

	return 0;

close_domain:
	lnx_domain_close(&(dom->domain_fid.fid));
	return rc;

fail:
	free(lnx_domain);
out:
	return rc;
}

