/*
 * Copyright (c) 2023 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *	   Redistribution and use in source and binary forms, with or
 *	   without modification, are permitted provided that the following
 *	   conditions are met:
 *
 *		- Redistributions of source code must retain the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer.
 *
 *		- Redistributions in binary form must reproduce the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer in the documentation and/or other materials
 *		  provided with the distribution.
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

#include <rdma/fi_errno.h>
#include <rdma/fabric.h>

#include <ofi_prov.h>
#include "xnet.h"

#ifdef HAVE_FABRIC_PROFILE
#include <ofi_profile.h>

static int
xnet_prof_init(struct fid *fid, uint64_t flags, void *context,
	       struct fi_profile_ops *ops, struct xnet_profile **xnet_prof)
{
	int ret = 0;
	struct util_profile *prof;

	*xnet_prof = 
		(struct xnet_profile *)calloc(1, sizeof(struct xnet_profile));
	if (!(*xnet_prof)) {
		FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
			"connot allocate memory.\n");
		return -FI_ENOMEM;
	}

	prof = &((*xnet_prof)->util_prof);
	prof->prov = &xnet_prov;
	ret = ofi_prof_init(prof, fid, flags, context, ops, 0, 0);
	if (ret) {
		goto err;
	}

	ofi_prof_add_common_vars(prof);
	ret = ofi_prof_add_var(prof, FI_VAR_UNEXP_MSG_CNT, NULL,
			       &((*xnet_prof)->unexp_msg_cnt));

	ofi_prof_add_common_events(prof);

	FI_TRACE(&xnet_prov, FI_LOG_EP_CTRL,
		"xnet_profile_init: flags 0x%lx, "
		"common: vars %zu, events %zu\n",
		flags, ofi_common_var_count, ofi_common_event_count);

	FI_TRACE(&xnet_prov, FI_LOG_EP_CTRL,
		"xnet_profile_init: flags 0x%lx, "
		"total: vars %zu, events %zu\n",
		flags, prof->var_count, prof->event_count);

	return 0;

err:
	if (*xnet_prof)
		free(*xnet_prof);

	return ret;

}

static void
xnet_prof_reset(struct fid_profile *prof_fid, uint64_t flags)
{
	struct util_profile *util_prof =
		 container_of(prof_fid, struct util_profile, prof_fid);

	ofi_prof_reset(util_prof, flags);
}

static ssize_t
xnet_prof_query_vars(struct fid_profile *prof_fid,
		     struct fi_profile_desc *varlist, size_t *count)
{
	struct util_profile *util_prof =
		container_of(prof_fid, struct util_profile, prof_fid);

	return ofi_prof_query_vars(util_prof, varlist, count);
}

static ssize_t
xnet_prof_query_events(struct fid_profile *prof_fid,
		       struct fi_profile_desc *eventlist, size_t *count)
{
	struct util_profile *util_prof =
		container_of(prof_fid, struct util_profile, prof_fid);

	return ofi_prof_query_events(util_prof, eventlist, count);
}

static int
xnet_prof_reg_cb(struct fid_profile *prof_fid, uint32_t event,
	    ofi_prof_callback_t cb, void *context)
{
	struct util_profile *util_prof =
		container_of(prof_fid, struct util_profile, prof_fid);

	return ofi_prof_reg_callback(util_prof, event, cb, context);
}

static ssize_t
xnet_prof_read_var(struct fid_profile *prof_fid, uint32_t var_id,
		   void *data, size_t *size)
{
	struct util_profile *util_prof =
		container_of(prof_fid, struct util_profile, prof_fid);

	int idx = ofi_prof_id2_idx(var_id, ofi_common_var_count);

	if ((idx >= util_prof->varlist_size) ||
	    (!OFI_VAR_ENABLED(&util_prof->varlist[idx])))
		return -FI_EINVAL;
	
	if (OFI_VAR_DATATYPE_U64(&(util_prof->varlist[idx])))
		return ofi_prof_read_u64(util_prof, idx, data, size);
	
	if (OFI_PROF_DATA_CACHED(util_prof))
		return ofi_prof_read_cached_data(util_prof, idx, data, size);

	// support only primitive data for now.
	return 0;
}

static void
xnet_prof_start_reads(struct fid_profile *prof_fid, uint64_t flags)
{
	int i;
	uint64_t size_u64 = sizeof(uint64_t);
	struct util_profile *util_prof =
		container_of(prof_fid, struct util_profile, prof_fid);

	// cache primitive data
	OFI_PROF_END_READS(util_prof);
	for (i = 0; i < util_prof->var_count; i++) {
		if (OFI_VAR_DATATYPE_U64(&(util_prof->varlist[i]))) {
			util_prof->data[i].size = 
			       ofi_prof_read_u64(util_prof, i,
						 &(util_prof->data[i].value.u64),
						 &size_u64);
		}
	}
	OFI_PROF_START_READS(util_prof);
}

static void
xnet_prof_end_reads(struct fid_profile *prof_fid, uint64_t flags)
{
	struct util_profile *util_prof =
		container_of(prof_fid, struct util_profile, prof_fid);

	OFI_PROF_END_READS(util_prof);
}

static struct fi_profile_ops xnet_prof_ep_ops =  {
	.size = sizeof(struct fi_profile_ops),
	.reset = xnet_prof_reset,
	.query_vars = xnet_prof_query_vars,
	.query_events = xnet_prof_query_events,
	.read_var = xnet_prof_read_var,
	.reg_callback = xnet_prof_reg_cb,
	.start_reads = xnet_prof_start_reads,
	.end_reads = xnet_prof_end_reads,
};

int xnet_ep_ops_open(struct fid *fid, const char *name,
		     uint64_t flags, void **ops, void *context)
{
	int ret = 0;
	struct xnet_profile *xnet_prof;
	struct xnet_ep *ep;

	if (!strcmp(name, "fi_profile_ops")) {
		if (fid->fclass == FI_CLASS_EP) {
			ret = xnet_prof_init(fid, flags, context, 
					     &xnet_prof_ep_ops, &xnet_prof);
			if (ret)
				return ret;

			ep = container_of(fid, struct xnet_ep,
					  util_ep.ep_fid.fid);
			ep->profile = xnet_prof;
			*ops = &(xnet_prof->util_prof.prof_fid.ops);
			return ret;
		}
	}
	FI_WARN(&xnet_prov, FI_LOG_EP_CTRL, "unsupported ep ops <%s>\n", name);

	return -FI_ENOSYS;
}

int xnet_rdm_ops_open(struct fid *fid, const char *name,
		      uint64_t flags, void **ops, void *context)
{
	int ret = 0;
	struct xnet_profile *xnet_prof;
	struct xnet_rdm *rdm; 

	if (!strncmp(name, "fi_profile_ops", 11)) {
		if (fid->fclass == FI_CLASS_EP) {
			ret = xnet_prof_init(fid, flags, context,
					     &xnet_prof_ep_ops,
					     &xnet_prof);
			if (ret)
				return ret;

			rdm = container_of(fid, struct xnet_rdm,
					   util_ep.ep_fid.fid);
			rdm->profile = xnet_prof;
			if (rdm->srx)
				rdm->srx->profile = xnet_prof;
			*ops = &(xnet_prof->util_prof.prof_fid.ops);
			return ret;
		}
	}
	FI_WARN(&xnet_prov, FI_LOG_EP_CTRL,
		"unsupported rdm ops <%s>\n", name);
	return -FI_ENOSYS;
}

#else

int xnet_ep_ops_open(struct fid *fid, const char *name,
		     uint64_t flags, void **ops, void *context)
{
	OFI_UNUSED(fid);
	OFI_UNUSED(name);
	OFI_UNUSED(flags);
	OFI_UNUSED(ops);
	OFI_UNUSED(context);
	return -FI_ENOSYS;
}

int xnet_rdm_ops_open(struct fid *fid, const char *name,
		      uint64_t flags, void **ops, void *context)
{
	OFI_UNUSED(fid);
	OFI_UNUSED(name);
	OFI_UNUSED(flags);
	OFI_UNUSED(ops);
	OFI_UNUSED(context);
	return -FI_ENOSYS;
}

#endif

