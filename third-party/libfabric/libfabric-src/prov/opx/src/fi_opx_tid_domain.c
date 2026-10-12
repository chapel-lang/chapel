/*
 * Copyright (C) 2022-2024 by Cornelis Networks.
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
#include "rdma/opx/fi_opx_tid_domain.h"

#include <ofi_enosys.h>

#include "rdma/opx/fi_opx.h"

#include "fi_opx_tid_cache.h"

/* The TID fabric/domain are very limited objects just used to
 * support util cache/memory monitor.
 * The usual fid/fi_xxx support is bare bones, most calls use
 * the object pointers
 */

int opx_close_tid_fabric(struct opx_tid_fabric *opx_tid_fabric)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_FABRIC, "close fabric\n");

	free((void *) opx_tid_fabric->util_fabric.name);
	opx_tid_fabric->util_fabric.name = NULL;
	free(opx_tid_fabric);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_FABRIC, "tid fabric closed\n");
	return 0;
}

static int fi_opx_tid_no_close_fabric(fid_t fid)
{
	assert(0);
	return -FI_ENOSYS;
}

static struct fi_ops fi_opx_fi_ops = {.size	= sizeof(struct fi_ops),
				      .close	= fi_opx_tid_no_close_fabric,
				      .bind	= fi_no_bind,
				      .control	= fi_no_control,
				      .ops_open = fi_no_ops_open,
				      .tostr	= fi_no_tostr,
				      .ops_set	= fi_no_ops_set};

static int opx_tid_no_domain(struct fid_fabric *fabric, struct fi_info *info, struct fid_domain **dom, void *context)
{
	assert(0);
	return -FI_ENOSYS;
}

static struct fi_ops_fabric fi_opx_ops_fabric = {.size	     = sizeof(struct fi_ops_fabric),
						 .domain     = opx_tid_no_domain,
						 .passive_ep = fi_no_passive_ep,
						 .eq_open    = fi_no_eq_open,
						 .wait_open  = fi_no_wait_open,
						 .trywait    = fi_no_trywait};

int opx_open_tid_fabric(struct opx_tid_fabric **opx_tid_fabric)
{
	struct opx_tid_fabric *tid_fabric;
	tid_fabric = calloc(1, sizeof(*tid_fabric));
	if (tid_fabric == NULL) {
		FI_WARN(fi_opx_global.prov, FI_LOG_FABRIC, "Couldn't create tid fabric FI_ENOMEM\n");
		return -FI_ENOMEM;
	}

	tid_fabric->util_fabric.fabric_fid.fid.fclass  = FI_CLASS_FABRIC;
	tid_fabric->util_fabric.fabric_fid.fid.context = NULL;
	tid_fabric->util_fabric.fabric_fid.fid.ops     = &fi_opx_fi_ops;
	tid_fabric->util_fabric.fabric_fid.ops	       = &fi_opx_ops_fabric;
	tid_fabric->util_fabric.fabric_fid.api_version = fi_opx_global.prov->fi_version;
	tid_fabric->util_fabric.prov		       = fi_opx_global.prov;
	ofi_atomic_initialize32(&tid_fabric->util_fabric.ref, 0);
	dlist_init(&tid_fabric->util_fabric.domain_list);
	ofi_mutex_init(&tid_fabric->util_fabric.lock);
	if (fi_opx_global.default_domain_attr && fi_opx_global.default_domain_attr->name) {
		tid_fabric->util_fabric.name = strdup(fi_opx_global.default_domain_attr->name);
	} else {
		tid_fabric->util_fabric.name = strdup(FI_OPX_DOMAIN_NAME);
	}
	if (!tid_fabric->util_fabric.name) {
		free(tid_fabric);
		tid_fabric = NULL;
		return -FI_ENOMEM;
	}

	ofi_fabric_insert(&tid_fabric->util_fabric);
	*opx_tid_fabric = tid_fabric;
	return 0;
}

/* The locked variable is used to handle abornmal exit cases where the
 * memory monitor lock is held. In that case, we can't flush/cleanup, but
 * we still want to do our best to free storage. In non-error cases,
 * locked is always 0. */
int opx_close_tid_domain(struct opx_tid_domain *tid_domain, int locked)
{
	if (tid_domain->tid_cache) {
		if (!locked) {
			opx_tid_cache_cleanup(tid_domain->tid_cache);
		}
		free(tid_domain->tid_cache);
		tid_domain->tid_cache = NULL;
	}

	dlist_remove(&tid_domain->list_entry);
	int ret = ofi_domain_close(&tid_domain->util_domain);
	if (ret != 0) {
		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN, "Error closing domain: %d\n", ret);
	}

	free(tid_domain);

	return 0;
}

int opx_open_tid_domain(struct opx_tid_fabric *tid_fabric, struct fi_info *info, struct opx_tid_domain **opx_tid_domain)
{
	int		       ret;
	struct opx_tid_domain *tid_domain = calloc(1, sizeof(struct opx_tid_domain));
	if (!tid_domain) {
		return -1;
	}

	ret = ofi_domain_init(&tid_fabric->util_fabric.fabric_fid, info, &tid_domain->util_domain, NULL, OFI_LOCK_NOOP);
	if (ret) {
		free(tid_domain);
		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN, "init util domain failed %d\n", ret);
		return ret;
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_DOMAIN, "cache %p, domain %p\n", tid_domain->tid_cache, tid_domain);
	ret = opx_tid_cache_setup(&tid_domain->tid_cache, tid_domain);

	/* Track TID domains so cache can be cleared on exit */
	dlist_insert_tail(&tid_domain->list_entry, &(fi_opx_global.tid_domain_list));

	if (ret) {
		free(tid_domain);
		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN, "init util domain failed %d\n", ret);
		return ret;
	}

	tid_domain->util_domain.domain_fid.fid.fclass  = FI_CLASS_DOMAIN;
	tid_domain->util_domain.domain_fid.fid.context = NULL;

	*opx_tid_domain = tid_domain;
	return ret;
}
