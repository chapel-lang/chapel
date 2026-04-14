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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "lpp.h"

static const struct fi_ops lpp_fi_domain_ops = {
	.size	       = sizeof(struct fi_ops),
	.close	       = lpp_fi_domain_close,
	.bind	       = fi_no_bind,
	.control       = fi_no_control,
	.ops_open      = fi_no_ops_open,
};

static const struct fi_ops_domain lpp_domain_ops = {
	.size		= sizeof(struct fi_ops_domain),
	.av_open	= lpp_fi_av_open,
	.cq_open	= lpp_fi_cq_open,
	.endpoint	= lpp_fi_endpoint,
	.scalable_ep	= fi_no_scalable_ep,
	.cntr_open	= lpp_fi_cntr_open,
	.poll_open	= fi_no_poll_open,
	.stx_ctx	= lpp_fi_stx_context,
	.srx_ctx	= fi_no_srx_context,
	.query_atomic	= lpp_fi_query_atomic,
};

#define LPP_MIN_CQ_DATA_SIZE 4

static const struct fi_domain_attr fi_domain_default_attrs = {
	.domain			= NULL,
	.name			= NULL,
#if HAVE_LPP_THREAD_SAFE
	.threading		= FI_THREAD_SAFE,
#else
	.threading		= FI_THREAD_DOMAIN,
#endif
	.control_progress	= FI_PROGRESS_MANUAL,
	.data_progress		= FI_PROGRESS_MANUAL,
	.resource_mgmt		= FI_RM_ENABLED,
	.av_type		= FI_AV_MAP,
	.mr_mode		= OFI_MR_BASIC | OFI_MR_BASIC_MAP,
	.mr_key_size		= sizeof(uint64_t),
	.cq_data_size		= LPP_MIN_CQ_DATA_SIZE,
	.cq_cnt			= LPP_CQ_MAX_COUNT,
	.ep_cnt			= LPP_EP_MAX_COUNT,
	.tx_ctx_cnt		= 1,
	.rx_ctx_cnt		= 1,
	.max_ep_tx_ctx		= 1,
	.max_ep_rx_ctx		= 1,
	.max_ep_stx_ctx		= 0,
	.max_ep_srx_ctx		= 0,
	.cntr_cnt		= LPP_CNTR_MAX_COUNT,
	.mr_iov_limit		= 1,
	.caps			= LPP_DOMAIN_CAPS,
	.mode			= 0,
	.auth_key		= NULL,
	.auth_key_size		= 8,
	.max_err_data		= 0,
	.mr_cnt			= LPP_MR_CNT,
	.tclass			= FI_TC_UNSPEC,
};

// Internal interface.
static int lpp_domain_allocate_cntrs(struct lpp_domain *lpp_domainp, int num_counters)
{
	int			i = 0;
	void			*cntr_array = NULL;
	size_t			num_bytes_needed = 0;
	_Atomic uint64_t	*cntr_block = NULL;
	struct lpp_cntr		*lpp_cntrp = NULL;

	lpp_domainp->cntr_total = num_counters;
	num_bytes_needed = lpp_domainp->cntr_total * 2 * sizeof(uint64_t);

	if (cntr_array = calloc(lpp_domainp->cntr_total, sizeof(struct lpp_cntr)), cntr_array == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "failed to alloc cntr array\n");
		return -FI_ENOMEM;
	}

	if (cntr_block = klpp_mmap(lpp_domainp, num_bytes_needed, PROT_READ | PROT_WRITE), cntr_block == NULL) {
		free(cntr_array);
		FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "failed to mmap cntr block\n");
		return -FI_ENOMEM;
	}

	lpp_domainp->cntr_block = cntr_block;
	lpp_domainp->cntr_array = cntr_array;

	for (i = 0, lpp_cntrp = cntr_array; i < num_counters; i++, lpp_cntrp++) {
		lpp_cntrp->counter = cntr_block++;
		lpp_cntrp->errcounter = cntr_block++;
	}

	return 0;
}

static void lpp_domain_free_cntrs(struct lpp_domain *lpp_domainp)
{
	klpp_munmap(lpp_domainp->cntr_block, lpp_domainp->cntr_total * 2 * sizeof(uint64_t));
	free(lpp_domainp->cntr_array);
}

static void lpp_domain_unmap_rmr(struct lpp_domain *lpp_domainp)
{
	size_t meta_bytes;
	size_t io_bytes;
	struct klpp_rmr_region_info *rmr_region_info = &lpp_domainp->devinfo.rmr_region_info;

	meta_bytes = klpp_rmr_meta_bytes(rmr_region_info, lpp_domainp->devinfo.host_count);
	io_bytes = klpp_rmr_io_region_bytes(rmr_region_info, lpp_domainp->devinfo.host_count);

	if (lpp_domainp->rmr_meta_arr != NULL) {
		klpp_munmap(lpp_domainp->rmr_meta_arr, meta_bytes);
		lpp_domainp->rmr_meta_arr = NULL;
	}
	if (lpp_domainp->rmr_rw != NULL) {
		klpp_munmap(lpp_domainp->rmr_rw, io_bytes);
		lpp_domainp->rmr_rw = NULL;
	}
	if (lpp_domainp->rmr_ro != NULL) {
		klpp_munmap(lpp_domainp->rmr_ro, io_bytes);
		lpp_domainp->rmr_ro = NULL;
	}
	if (lpp_domainp->rmr_srq != NULL) {
		klpp_munmap(lpp_domainp->rmr_srq, io_bytes);
		lpp_domainp->rmr_srq = NULL;
	}
}

static int lpp_domain_map_rmr(struct lpp_domain *lpp_domainp)
{
	int status;
	size_t meta_bytes;
	size_t io_bytes;
	struct klpp_rmr_region_info *rmr_region_info = &lpp_domainp->devinfo.rmr_region_info;

	lpp_domainp->rmr_meta_arr = NULL;
	lpp_domainp->rmr_rw = NULL;
	lpp_domainp->rmr_ro = NULL;
	lpp_domainp->rmr_srq = NULL;

	meta_bytes = klpp_rmr_meta_bytes(rmr_region_info, lpp_domainp->devinfo.host_count);
	io_bytes = klpp_rmr_io_region_bytes(rmr_region_info, lpp_domainp->devinfo.host_count);

	if (!lpp_disable_osbypass) {
		if ((lpp_domainp->rmr_meta_arr = klpp_mmap(lpp_domainp, meta_bytes, PROT_READ | PROT_WRITE)) == NULL) {
			FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "failed to mmap rmr_meta_arr\n");
			status = -FI_ENOMEM;
			goto err;
		}
		if ((lpp_domainp->rmr_rw = klpp_mmap(lpp_domainp, io_bytes, PROT_READ | PROT_WRITE)) == NULL) {
			FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "failed to mmap rmr_rw\n");
			status = -FI_ENOMEM;
			goto err;
		}
		if ((lpp_domainp->rmr_ro = klpp_mmap(lpp_domainp, io_bytes, PROT_READ)) == NULL) {
			FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "failed to mmap rmr_ro\n");
			status = -FI_ENOMEM;
			goto err;
		}
	}

	if (lpp_write_only) {
		if ((lpp_domainp->rmr_srq = klpp_mmap(lpp_domainp, io_bytes, PROT_READ | PROT_WRITE)) == NULL) {
			FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "failed to mmap rmr_srq\n");
			status = -FI_ENOMEM;
			goto err;
		}
	}

	if ((status = klpp_rmr_label(lpp_domainp)) != 0) {
		FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "failed to label mmapped RMR regions\n");
		goto err;
	}

	return 0;
err:
	lpp_domain_unmap_rmr(lpp_domainp);
	return status;
}

static void lpp_domain_fid(struct fid_domain *domain_fid, void *context)
{

	if (domain_fid != NULL) {
		domain_fid->fid.fclass = FI_CLASS_DOMAIN;
		domain_fid->fid.context = context;
		domain_fid->fid.ops = (struct fi_ops *)&lpp_fi_domain_ops;

		domain_fid->ops = (struct fi_ops_domain *)&lpp_domain_ops;
		domain_fid->mr = (struct fi_ops_mr *)&lpp_mr_ops;
	}
}


static void lpp_domain_attrs_pop_unspec(struct fi_domain_attr *dom_hints,
					struct fi_domain_attr  *attr) {
#define POP_DOMAIN_UNSPEC(_attr_unspec, _attr)                                 \
	if (_attr_unspec == attr->_attr) {                                     \
		if(dom_hints && _attr_unspec != dom_hints->_attr)              \
			attr->_attr = dom_hints->_attr;                        \
		else                                                           \
			attr->_attr = fi_domain_default_attrs._attr;           \
	}

	POP_DOMAIN_UNSPEC(FI_THREAD_UNSPEC, threading);
	POP_DOMAIN_UNSPEC(FI_PROGRESS_UNSPEC, control_progress);
	POP_DOMAIN_UNSPEC(FI_PROGRESS_UNSPEC, data_progress);
	POP_DOMAIN_UNSPEC(FI_RM_UNSPEC, resource_mgmt);
	POP_DOMAIN_UNSPEC(FI_AV_UNSPEC, av_type);
	POP_DOMAIN_UNSPEC(FI_TC_UNSPEC, tclass);

#undef POP_DOMAIN_UNSPEC
}


void lpp_domain_setup_attrs(const struct klppioc_lf *klpp_devinfo,
				const struct fi_info *hints,
				struct fi_domain_attr *tmp_domain) {
	struct fi_domain_attr *hints_domain = (hints)? hints->domain_attr: NULL;

	if(NULL == tmp_domain->name && klpp_devinfo)
		tmp_domain->name = strdup(klpp_devinfo->domain);

	lpp_domain_attrs_pop_unspec(hints_domain, tmp_domain);

#define SETUP_DOMAIN_ATTR(_name)	\
		tmp_domain->_name = fi_domain_default_attrs._name

	SETUP_DOMAIN_ATTR(mr_mode);
	SETUP_DOMAIN_ATTR(mr_key_size);
	SETUP_DOMAIN_ATTR(cq_data_size);
	SETUP_DOMAIN_ATTR(cq_cnt);
	SETUP_DOMAIN_ATTR(ep_cnt);
	SETUP_DOMAIN_ATTR(tx_ctx_cnt);
	SETUP_DOMAIN_ATTR(rx_ctx_cnt);
	SETUP_DOMAIN_ATTR(max_ep_tx_ctx);
	SETUP_DOMAIN_ATTR(max_ep_rx_ctx);
	SETUP_DOMAIN_ATTR(max_ep_stx_ctx);
	SETUP_DOMAIN_ATTR(max_ep_srx_ctx);
	SETUP_DOMAIN_ATTR(cntr_cnt);
	SETUP_DOMAIN_ATTR(mr_iov_limit);
	SETUP_DOMAIN_ATTR(caps);
	SETUP_DOMAIN_ATTR(mode);
	SETUP_DOMAIN_ATTR(auth_key);
	SETUP_DOMAIN_ATTR(auth_key_size);
	SETUP_DOMAIN_ATTR(max_err_data);
	SETUP_DOMAIN_ATTR(mr_cnt);
#undef SETUP_DOMAIN_ATTR
}

int lpp_domain_verify_attrs(const struct klppioc_lf *klpp_devinfo,
		struct fi_domain_attr *attr, uint32_t version)
{
	// Check the attributes.
	if ((attr->name != NULL) && (strcmp(attr->name, klpp_devinfo->domain) != 0)) {
		FI_INFO(&lpp_prov, FI_LOG_DOMAIN, "domain name '%s' doesn't match our name '%s'\n",
		    attr->name, klpp_devinfo->domain);
		return -FI_ENODATA;
	}

#if HAVE_LPP_THREAD_SAFE
	if ((attr->threading != FI_THREAD_UNSPEC) &&
	      (attr->threading != FI_THREAD_SAFE) &&
	    (attr->threading != FI_THREAD_DOMAIN)) {
#else
	if ((attr->threading != FI_THREAD_UNSPEC) &&
	    (attr->threading != FI_THREAD_DOMAIN)) {
#endif
		FI_INFO(&lpp_prov, FI_LOG_DOMAIN,
			"unsupported threading model %u\n", attr->threading);
		return -FI_ENODATA;
	}

	if ((attr->control_progress != FI_PROGRESS_UNSPEC) && (attr->control_progress != FI_PROGRESS_MANUAL)) {
		FI_INFO(&lpp_prov, FI_LOG_DOMAIN, "control progress model doesn't match\n");
		return -FI_ENODATA;
	}

	if ((attr->data_progress != FI_PROGRESS_UNSPEC) && (attr->data_progress != FI_PROGRESS_MANUAL)) {
		FI_INFO(&lpp_prov, FI_LOG_DOMAIN, "data progress model doesn't match\n");
		return -FI_ENODATA;
	}

	if ((attr->resource_mgmt != FI_RM_UNSPEC) && (attr->resource_mgmt != FI_RM_ENABLED)) {
		FI_INFO(&lpp_prov, FI_LOG_DOMAIN, "resource mgmt model doesn't match\n");
		return -FI_ENODATA;
	}

	if ((attr->av_type != FI_AV_UNSPEC) && (attr->av_type != FI_AV_MAP)) {
		FI_INFO(&lpp_prov, FI_LOG_DOMAIN, "AV type not supported\n");
		return -FI_ENODATA;
	}

#define CHECK_ATTR(name, actual, op, expected, fmt)									\
	if (actual op expected) {											\
		FI_INFO(&lpp_prov, FI_LOG_DOMAIN, #name " val " fmt " doesn't match expected " fmt " API version %d\n",	\
			actual, expected, version); \
		return -FI_ENODATA;											\
	}

	CHECK_ATTR(mr_key_size, attr->mr_key_size, >, fi_domain_default_attrs.mr_key_size, "%ld");
	CHECK_ATTR(cq_data_size, attr->cq_data_size, >, fi_domain_default_attrs.cq_data_size, "%ld");
	CHECK_ATTR(cq_cnt, attr->cq_cnt, >, fi_domain_default_attrs.cq_cnt, "%ld");
	CHECK_ATTR(ep_cnt, attr->ep_cnt, >, fi_domain_default_attrs.ep_cnt, "%ld");
	CHECK_ATTR(max_ep_tx_ctx, attr->max_ep_tx_ctx, >, fi_domain_default_attrs.max_ep_tx_ctx, "%ld");
	CHECK_ATTR(max_ep_rx_ctx, attr->max_ep_rx_ctx, >, fi_domain_default_attrs.max_ep_rx_ctx, "%ld");
	CHECK_ATTR(cntr_cnt, attr->cntr_cnt, >, fi_domain_default_attrs.cntr_cnt, "%ld");
	CHECK_ATTR(mr_iov_limit, attr->mr_iov_limit, >, fi_domain_default_attrs.mr_iov_limit, "%ld");
	CHECK_ATTR(max_err_data, attr->max_err_data, >, fi_domain_default_attrs.max_err_data, "%ld");
	CHECK_ATTR(mr_cnt, attr->mr_cnt, >, fi_domain_default_attrs.mr_cnt, "%ld");

#undef CHECK_ATTR

	return 0;
}

int lpp_mr_cache_open(struct util_domain *udomain, struct ofi_mr_cache **cache)
{
	int ret;
	struct ofi_mem_monitor *memory_monitors[OFI_HMEM_MAX] = {
		[FI_HMEM_SYSTEM] = default_monitor,
		[FI_HMEM_CUDA] = cuda_monitor,
		[FI_HMEM_ROCR] = rocr_monitor,
	};


	/* Might need error check on memory_monitors for conflicts with OMPI,
	   see efa/opx providers */
	*cache = (struct ofi_mr_cache *)calloc(1, sizeof(struct ofi_mr_cache));
	if (!*cache)
		return -FI_ENOMEM;

	/* This is the spot to configure cache_params (see util_mr_cache.c) */
	(*cache)->entry_data_size = sizeof(struct lpp_mr);
	(*cache)->add_region = lpp_mr_cache_entry_reg;
	(*cache)->delete_region = lpp_mr_cache_entry_dereg;

	ret = ofi_mr_cache_init(udomain, memory_monitors, *cache);

	if (ret) {
		FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "mr cache creation failed %s",
							fi_strerror(ret));
		free(*cache);
		*cache = NULL;
		return ret;
	}

	return 0;
}

// User visible interface.
int lpp_fi_domain(struct fid_fabric *fabric_fid, struct fi_info *info,
			    struct fid_domain **domain, void *context)
{
	int			dev_index = 0;
	int			fd = -1;
	int			status = 0;
	char			c;
	struct lpp_fabric	*lpp_fabricp = NULL;
	struct lpp_domain	*lpp_domainp = NULL;
	struct fi_domain_attr	domain_attrs = fi_domain_default_attrs;

	lpp_fabricp = container_of(fabric_fid, struct lpp_fabric, fabric_fid);

	// Get the interface attributes.
	if ((info == NULL) || (info->domain_attr == NULL) || (info->domain_attr->name == NULL)) {
		FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "NULL fi_info\n");
		return -FI_ENODATA;
	}

	if (sscanf(info->domain_attr->name, "lpp%d%c", &dev_index, &c) != 1) {
		FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "invalid domain name '%s'\n", info->domain_attr->name);
		return -FI_ENODATA;
	}

	// Allocate space for the domain struct.
	if (lpp_domainp = calloc(1, sizeof(struct lpp_domain)), lpp_domainp == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "failed to alloc domain\n");
		return -FI_ENODATA;
	}

	fd = klpp_open(dev_index);
	if (fd < 0) {
		status = -FI_ENODATA;
		goto err;
	}
	lpp_domainp->fd = fd;

	if (klpp_getdevice(lpp_domainp->fd, &lpp_domainp->devinfo) != 0) {
		FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "failed to get KLPP device\n");
		return -FI_ENODATA;
	}

	// Verify the domain attributes.
	if ((info != NULL) && (info->domain_attr != NULL)) {
		if (lpp_domain_verify_attrs(&lpp_domainp->devinfo,
					info->domain_attr, OFI_VERSION_DEF_PROV) != 0) {
			return -FI_ENODATA;
		}
		lpp_domain_attrs_pop_unspec(NULL, info->domain_attr);

		domain_attrs = *info->domain_attr;
	}

	// Increment the fabric reference count.
	ofi_atomic_inc32(&lpp_fabricp->ref_count);

	// Fill in the domain struct.
	lpp_domainp->util_domain.prov = &lpp_prov;
	lpp_domainp->util_domain.av_type = FI_AV_MAP;
	lpp_domainp->util_domain.data_progress = FI_PROGRESS_MANUAL;
	lpp_domain_fid(&lpp_domainp->util_domain.domain_fid, context);
	lpp_domainp->fabric = lpp_fabricp;
	lpp_domainp->index = dev_index;

	lpp_domainp->attr = domain_attrs;
	ofi_atomic_initialize32(&lpp_domainp->util_domain.ref, 0);
	ofi_mutex_init(&lpp_domainp->ep_lock);
	ofi_mutex_init(&lpp_domainp->cntr_lock);

	lpp_hmem_init(lpp_domainp);

	if (domain_attrs.threading == FI_THREAD_SAFE) {
		lpp_domainp->lock_acquire = ofi_mutex_lock_op;
		lpp_domainp->lock_release = ofi_mutex_unlock_op;
	} else {
		lpp_domainp->lock_acquire = ofi_mutex_lock_noop;
		lpp_domainp->lock_release = ofi_mutex_unlock_noop;
	}

	// Allocate the counters.
	if (lpp_domain_allocate_cntrs(lpp_domainp, domain_attrs.cntr_cnt) < 0) {
		FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "failed to allocate counter region\n");
		status = -FI_ENODATA;
		goto err;
	}

	// Allocate the RMR userspace mapping regions.
	if (lpp_domain_map_rmr(lpp_domainp) < 0) {
		FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "failed to allocate RMR region\n");
		status = -FI_ENODATA;
		goto err;
	}

	status = lpp_mr_cache_open(&lpp_domainp->util_domain,
					&lpp_domainp->mr_cache);

	if (status) {
		FI_WARN(&lpp_prov, FI_LOG_DOMAIN, "mr cache creation failed %s",
							fi_strerror(status));
		goto err;
	}

	// Return good status.
	*domain = &lpp_domainp->util_domain.domain_fid;
	return 0;

err:
	if (fd >= 0) {
		klpp_close(fd);
	}
	if (lpp_domainp != NULL) {
		if (lpp_domainp->cntr_block != NULL) {
			lpp_domain_free_cntrs(lpp_domainp);
		}
		if (lpp_domainp->rmr_meta_arr != NULL) {
			lpp_domain_unmap_rmr(lpp_domainp);
		}
		free(lpp_domainp);
	}

	return status;
}

int lpp_fi_domain_close(struct fid *fid)
{
	struct util_domain *util_domain;
	struct lpp_domain *lpp_domainp;

	util_domain = container_of(fid, struct util_domain, domain_fid);
	lpp_domainp = container_of(util_domain, struct lpp_domain, util_domain);

	if (!lpp_domain_cleanup) {
		if (ofi_atomic_get32(&lpp_domainp->util_domain.ref) > 0) {
			FI_WARN(&lpp_prov, FI_LOG_DOMAIN,
				"failed to close domain, ref_count:%u\n",
				ofi_atomic_get32(&lpp_domainp->util_domain.ref));
			return -FI_EBUSY;
		}
	}

	ofi_mr_cache_cleanup(lpp_domainp->mr_cache);

	lpp_domain_free_cntrs(lpp_domainp);
	lpp_domain_unmap_rmr(lpp_domainp);
	klpp_close(lpp_domainp->fd);
	ofi_atomic_dec32(&lpp_domainp->fabric->ref_count);
	free(lpp_domainp);

	return 0;
}
