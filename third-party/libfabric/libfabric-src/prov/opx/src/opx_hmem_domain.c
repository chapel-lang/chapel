/*
 * Copyright (C) 2024-2025 by Cornelis Networks.
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

#ifdef OPX_HMEM

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <ctype.h>

#include "rdma/fabric.h"

#include "rdma/opx/fi_opx_endpoint.h"
#include "rdma/opx/opx_hmem_domain.h"

#include <ofi_enosys.h>

#include "rdma/opx/fi_opx.h"

#include "opx_hmem_cache.h"

int opx_hmem_close_fabric(struct opx_hmem_fabric *opx_hmem_fabric)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_FABRIC, "close hmem fabric\n");

	free((void *) opx_hmem_fabric->util_fabric.name);
	opx_hmem_fabric->util_fabric.name = NULL;
	free(opx_hmem_fabric);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_FABRIC, "hmem fabric closed\n");
	return 0;
}

static int opx_hmem_no_close_fabric(fid_t fid)
{
	assert(0);
	return -FI_ENOSYS;
}

static struct fi_ops opx_hmem_domain_fi_ops = {.size	 = sizeof(struct fi_ops),
					       .close	 = opx_hmem_no_close_fabric,
					       .bind	 = fi_no_bind,
					       .control	 = fi_no_control,
					       .ops_open = fi_no_ops_open,
					       .tostr	 = fi_no_tostr,
					       .ops_set	 = fi_no_ops_set};

static int opx_hmem_no_domain(struct fid_fabric *fabric, struct fi_info *info, struct fid_domain **dom, void *context)
{
	assert(0);
	return -FI_ENOSYS;
}

static struct fi_ops_fabric opx_hmem_fabric_ops = {.size       = sizeof(struct fi_ops_fabric),
						   .domain     = opx_hmem_no_domain,
						   .passive_ep = fi_no_passive_ep,
						   .eq_open    = fi_no_eq_open,
						   .wait_open  = fi_no_wait_open,
						   .trywait    = fi_no_trywait};

int opx_hmem_open_fabric(struct opx_hmem_fabric **hmem_fabric)
{
	struct opx_hmem_fabric *new_hmem_fabric;
	new_hmem_fabric = calloc(1, sizeof(*new_hmem_fabric));
	if (new_hmem_fabric == NULL) {
		FI_WARN(fi_opx_global.prov, FI_LOG_FABRIC, "Couldn't create hmem fabric FI_ENOMEM\n");
		return -FI_ENOMEM;
	}

	new_hmem_fabric->util_fabric.fabric_fid.fid.fclass  = FI_CLASS_FABRIC;
	new_hmem_fabric->util_fabric.fabric_fid.fid.context = NULL;
	new_hmem_fabric->util_fabric.fabric_fid.fid.ops	    = &opx_hmem_domain_fi_ops;
	new_hmem_fabric->util_fabric.fabric_fid.ops	    = &opx_hmem_fabric_ops;
	new_hmem_fabric->util_fabric.fabric_fid.api_version = fi_opx_global.prov->fi_version;
	new_hmem_fabric->util_fabric.prov		    = fi_opx_global.prov;
	ofi_atomic_initialize32(&new_hmem_fabric->util_fabric.ref, 0);
	dlist_init(&new_hmem_fabric->util_fabric.domain_list);
	ofi_mutex_init(&new_hmem_fabric->util_fabric.lock);
	if (fi_opx_global.default_domain_attr && fi_opx_global.default_domain_attr->name) {
		new_hmem_fabric->util_fabric.name = strdup(fi_opx_global.default_domain_attr->name);
	} else {
		new_hmem_fabric->util_fabric.name = strdup(FI_OPX_DOMAIN_NAME);
	}
	if (!new_hmem_fabric->util_fabric.name) {
		free(new_hmem_fabric);
		return -FI_ENOMEM;
	}

	ofi_fabric_insert(&new_hmem_fabric->util_fabric);
	*hmem_fabric = new_hmem_fabric;
	return 0;
}

/* The locked variable is used to handle abornmal exit cases where the
 * memory monitor lock is held. In that case, we can't flush/cleanup, but
 * we still want to do our best to free storage. In non-error cases,
 * locked is always 0.
 */
int opx_hmem_close_domain(struct opx_hmem_domain *hmem_domain, int locked)
{
	if (hmem_domain->hmem_cache) {
		if (!locked) {
			ofi_mr_cache_cleanup(hmem_domain->hmem_cache);
		}
		free(hmem_domain->hmem_cache);
		hmem_domain->hmem_cache = NULL;
	}

	dlist_remove(&hmem_domain->list_entry);

	if (hmem_domain->ipc_cache) {
		ofi_ipc_cache_destroy(hmem_domain->ipc_cache);
		hmem_domain->ipc_cache = NULL;
	}

	opx_hmem_stream_destroy(hmem_domain->hmem_stream.type, hmem_domain->hmem_stream.stream);

	if (hmem_domain->hmem_stream.event_pool) {
		ofi_bufpool_destroy(hmem_domain->hmem_stream.event_pool);
		hmem_domain->hmem_stream.event_pool = NULL;
	}

	ofi_domain_close(&hmem_domain->util_domain);
	free(hmem_domain);

	return 0;
}

int opx_hmem_open_domain(struct opx_hmem_fabric *hmem_fabric, struct fi_info *info,
			 struct opx_hmem_domain **hmem_domain)
{
	int			ret;
	struct opx_hmem_domain *new_hmem_domain = calloc(1, sizeof(struct opx_hmem_domain));
	if (!new_hmem_domain) {
		return -FI_ENOMEM;
	}

	ret = ofi_domain_init(&hmem_fabric->util_fabric.fabric_fid, info, &new_hmem_domain->util_domain, NULL,
			      OFI_LOCK_NOOP);

	FI_INFO(fi_opx_global.prov, FI_LOG_DOMAIN, "Initalizing the util hmem domain is complete \n");
	if (ret) {
		free(new_hmem_domain);
		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN, "init util domain failed %d (%s)\n", ret, strerror(ret));
		return ret;
	}
	FI_INFO(fi_opx_global.prov, FI_LOG_DOMAIN, "cache %p, domain %p\n", new_hmem_domain->hmem_cache,
		new_hmem_domain);
	ret = opx_hmem_cache_setup(&new_hmem_domain->hmem_cache, new_hmem_domain);

	/* Track HMEM domains so cache can be cleared on exit */
	dlist_insert_tail(&new_hmem_domain->list_entry, &(fi_opx_global.hmem_domain_list));

	if (ret) {
		opx_hmem_close_domain(new_hmem_domain, 0);
		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN, "init util domain failed %d (%s)\n", ret, strerror(ret));
		return ret;
	}

	if ((hmem_ops[FI_HMEM_CUDA].initialized && ofi_hmem_is_ipc_enabled(FI_HMEM_CUDA)) ||
	    (hmem_ops[FI_HMEM_ROCR].initialized && ofi_hmem_is_ipc_enabled(FI_HMEM_ROCR))) {
		ret = ofi_ipc_cache_open(&new_hmem_domain->ipc_cache, &new_hmem_domain->util_domain);
		if (ret) {
			opx_hmem_close_domain(new_hmem_domain, 0);
			FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN, "Error opening IPC Cache ret=%d (%s)\n", ret,
				strerror(ret));
			return ret;
		}
	}

	ret = ofi_bufpool_create(&new_hmem_domain->hmem_stream.event_pool, sizeof(union opx_hmem_event), 0, UINT_MAX,
				 16, OFI_BUFPOOL_NO_ZERO);
	if (ret) {
		opx_hmem_close_domain(new_hmem_domain, 0);
		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN, "Error creating HMEM event pool ret=%d (%s)\n", ret,
			strerror(ret));
		return ret;
	}

	new_hmem_domain->util_domain.domain_fid.fid.fclass  = FI_CLASS_DOMAIN;
	new_hmem_domain->util_domain.domain_fid.fid.context = NULL;

	new_hmem_domain->devreg_copy_from_threshold = OPX_HMEM_DEV_REG_SEND_THRESHOLD_DEFAULT;
	new_hmem_domain->devreg_copy_to_threshold   = OPX_HMEM_DEV_REG_RECV_THRESHOLD_DEFAULT;

	*hmem_domain = new_hmem_domain;
	return FI_SUCCESS;
}
#endif /* OPX_HMEM */
