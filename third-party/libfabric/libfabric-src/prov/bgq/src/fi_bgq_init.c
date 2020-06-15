/*
 * Copyright (C) 2016 by Argonne National Laboratory.
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
#include "rdma/bgq/fi_bgq.h"
#include "ofi_prov.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int fi_bgq_init;
static int fi_bgq_count;

int fi_bgq_check_info(const struct fi_info *info)
{
	int ret;

	/* TODO: check caps, mode */

	if ((info->tx_attr) && ((info->tx_attr->caps | info->caps) != info->caps)) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
				"The tx_attr capabilities (0x%016lx) must be a subset of those requested of the associated endpoint (0x%016lx)",
				info->tx_attr->caps, info->caps);
		goto err;
	}

	if ((info->rx_attr) && ((info->rx_attr->caps | info->caps) != info->caps)) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
				"The rx_attr capabilities (0x%016lx) must be a subset of those requested of the associated endpoint (0x%016lx)",
				info->rx_attr->caps, info->caps);
		goto err;
	}


	switch (info->addr_format) {
	case FI_FORMAT_UNSPEC:
	case FI_ADDR_BGQ:
		break;
	default:
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
				"unavailable [bad info->addr_format (%u)]",
				info->addr_format);
		goto err;
	}

	if (info->tx_attr) {
		ret = fi_bgq_check_tx_attr(info->tx_attr);
		if (ret)
			return ret;
	}

	if (info->rx_attr) {
		ret = fi_bgq_check_rx_attr(info->rx_attr);
		if (ret)
			return ret;
	}

	if (info->ep_attr) {
		ret = fi_bgq_check_ep_attr(info->ep_attr);
		if (ret)
			return ret;
	}

	if (info->domain_attr) {
		ret = fi_bgq_check_domain_attr(info->domain_attr);
		if (ret)
			return ret;
	}
	if (info->fabric_attr) {
		ret = fi_bgq_check_fabric_attr(info->fabric_attr);
		if (ret)
			return ret;
	}

	return 0;

err:
	errno = FI_ENODATA;
	return -errno;
}

static int fi_bgq_fillinfo(struct fi_info *fi, const char *node,
		const char* service, const struct fi_info *hints,
	        uint64_t flags)
{
	int ret;
	uint64_t caps;

	if (!fi)
		goto err;

	if (!hints && !node && !service)
		goto err;

	if (hints->dest_addr) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
				"cannot support dest_addr lookups now");
		errno = FI_ENOSYS;
		return -errno;
	}

	fi->next = NULL;
	fi->caps = FI_BGQ_DEFAULT_CAPS;

	/* set the mode that we require */
	fi->mode = FI_ASYNC_IOV;
	fi->mode |= (FI_CONTEXT);

	/* clear modes that we do not require */
	fi->mode &= (~FI_LOCAL_MR);
	fi->mode &= (~FI_MSG_PREFIX);

	fi->addr_format = FI_ADDR_BGQ;
	fi->src_addrlen = 24; // includes null
	fi->dest_addrlen = 24; // includes null
#ifdef TODO
	if (flags & FI_SOURCE) {
		fi->src_addr = strdup(service);
		if (!fi->src_addr) {
			goto err;
		}
	}
#endif
	fi->dest_addr = NULL;

	/*
	 * man/fi_fabric.3
	 *
	 * On input to fi_getinfo, a user may set this (fi_fabric_attr::fabric)
	 * to an opened fabric instance to restrict output to the given fabric.
	 * On output from fi_getinfo, if no fabric was specified, but the user
	 * has an opened instance of the named fabric, this (fi_fabric_attr::fabric)
	 * will reference the first opened instance. If no instance has been
	 * opened, this field will be NULL.
	 */

	fi->fabric_attr->name = strdup(FI_BGQ_FABRIC_NAME);
	if (!fi->fabric_attr->name) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
				"memory allocation failed");
		goto err;
	}
	fi->fabric_attr->prov_version = FI_BGQ_PROVIDER_VERSION;

	memcpy(fi->tx_attr, fi_bgq_global.default_tx_attr, sizeof(*fi->tx_attr));

	if (hints->tx_attr) {

		/*
		 * man/fi_endpoint.3
		 *
		 *   fi_tx_attr::caps
		 *
		 *   "... If the caps field is 0 on input to fi_getinfo(3), the
		 *   caps value from the fi_info structure will be used."
		 */
		if (hints->tx_attr->caps) {
			fi->tx_attr->caps = hints->tx_attr->caps;
		}

		/* adjust parameters down from what requested if required */
		fi->tx_attr->op_flags = hints->tx_attr->op_flags;
	} else if (hints->caps) {
		fi->tx_attr->caps = hints->caps;
	}

	memcpy(fi->rx_attr, fi_bgq_global.default_rx_attr, sizeof(*fi->rx_attr));
	if (hints->rx_attr) {

		/*
		 * man/fi_endpoint.3
		 *
		 *   fi_rx_attr::caps
		 *
		 *   "... If the caps field is 0 on input to fi_getinfo(3), the
		 *   caps value from the fi_info structure will be used."
		 */
		if (hints->rx_attr->caps) {
			fi->rx_attr->caps = hints->rx_attr->caps;
		}

		/* adjust parameters down from what requested if required */
		fi->rx_attr->op_flags = hints->rx_attr->op_flags;
		if (hints->rx_attr->total_buffered_recv > 0 &&
			hints->rx_attr->total_buffered_recv < fi_bgq_global.default_rx_attr->total_buffered_recv)
				fi->rx_attr->total_buffered_recv = hints->rx_attr->total_buffered_recv;
	} else if (hints->caps) {
		fi->rx_attr->caps = hints->caps;
	}

	caps = fi->caps | fi->tx_attr->caps | fi->rx_attr->caps;

	/*
	 * man/fi_domain.3
	 *
	 * On input to fi_getinfo, a user may set this (fi_domain_attr::domain)
	 * to an opened domain instance to restrict output to the given domain.
	 * On output from fi_getinfo, if no domain was specified, but the user
	 * has an opened instance of the named domain, this (fi_domain_attr::domain)
	 * will reference the first opened instance. If no instance has been
	 * opened, this field will be NULL.
	 */

	ret = fi_bgq_choose_domain(caps, fi->domain_attr, hints->domain_attr);
	if (ret) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
				"cannot find appropriate domain");
		goto err;
	}

	memcpy(fi->ep_attr, fi_bgq_global.default_ep_attr, sizeof(*fi->ep_attr));
	if (hints->ep_attr) {
		/* adjust parameters down from what requested if required */
		fi->ep_attr->type	= hints->ep_attr->type;
		if (hints->ep_attr->max_msg_size > 0 &&
			hints->ep_attr->max_msg_size <= fi_bgq_global.default_ep_attr->max_msg_size)
				fi->ep_attr->max_msg_size = hints->ep_attr->max_msg_size;

		if (0 != hints->ep_attr->tx_ctx_cnt && hints->ep_attr->tx_ctx_cnt <= fi->ep_attr->tx_ctx_cnt)
			fi->ep_attr->tx_ctx_cnt = hints->ep_attr->tx_ctx_cnt;	/* TODO - check */

		if (0 != hints->ep_attr->rx_ctx_cnt && hints->ep_attr->rx_ctx_cnt <= fi->ep_attr->rx_ctx_cnt)
			fi->ep_attr->rx_ctx_cnt = hints->ep_attr->rx_ctx_cnt;	/* TODO - check */
	}



	return 0;
err:
	if (fi->domain_attr->name) free(fi->domain_attr->name);
	if (fi->fabric_attr->name) free(fi->fabric_attr->name);
	if (fi->fabric_attr->prov_name) free(fi->fabric_attr->prov_name);
	errno = FI_ENODATA;
	return -errno;
}

struct fi_bgq_global_data fi_bgq_global;

static int fi_bgq_getinfo(uint32_t version, const char *node,
		const char *service, uint64_t flags,
		const struct fi_info *hints, struct fi_info **info)
{

	if (!((FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_MANUAL) || (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_AUTO))){
		fprintf(stderr,"BGQ Provider must be configured with either auto or manual progresss mode specified\n");
		exit(1);
		assert(0);
	}

	BG_JobCoords_t jobCoords;
	uint32_t jcrc = Kernel_JobCoords(&jobCoords);
	if (jobCoords.isSubBlock) {
		fprintf(stderr,"BGQ Provider cannot be run in a sub-block.\n");
		fflush(stderr);
		exit(1);
	}

	int ret;
	struct fi_info *fi, *prev_fi, *curr;

	if (!fi_bgq_count) {
		errno = FI_ENODATA;
		return -errno;
	}

	if (hints) {
		ret = fi_bgq_check_info(hints);
		if (ret) {
			return ret;
		}
		if (!(fi = fi_allocinfo())) {
			return -FI_ENOMEM;
		}
		if (fi_bgq_fillinfo(fi, node, service,
					hints, flags)) {
			return -errno;
		}
		*info = fi;
	} else {
		if(node || service) {
			errno = FI_ENODATA;
			return -errno;
		} else {
			if (!(fi = fi_dupinfo(fi_bgq_global.info))) {
				return -FI_ENOMEM;
			}
			*info = fi;
		}
	}

	return 0;
}

static void fi_bgq_fini()
{
	always_assert(fi_bgq_init == 1,
		"BGQ provider finalize called before initialize\n");
	fi_freeinfo(fi_bgq_global.info);
}

static struct fi_provider fi_bgq_provider = {
	.name 		= FI_BGQ_PROVIDER_NAME,
	.version 	= FI_VERSION(0, 1),
	.fi_version 	= OFI_VERSION_LATEST,
	.getinfo	= fi_bgq_getinfo,
	.fabric		= fi_bgq_fabric,
	.cleanup	= fi_bgq_fini
};

BGQ_INI
{
	fi_bgq_count = 1;
	fi_bgq_set_default_info(); // TODO: fold into fi_bgq_set_defaults

	if (fi_bgq_alloc_default_domain_attr(&fi_bgq_global.default_domain_attr)) {
		return NULL;
	}

	if (fi_bgq_alloc_default_ep_attr(&fi_bgq_global.default_ep_attr)) {
		return NULL;
	}

	if (fi_bgq_alloc_default_tx_attr(&fi_bgq_global.default_tx_attr)) {
		return NULL;
	}

	if (fi_bgq_alloc_default_rx_attr(&fi_bgq_global.default_rx_attr)) {
		return NULL;
	}

	fi_bgq_global.prov = &fi_bgq_provider;

	fi_bgq_init = 1;

	return (&fi_bgq_provider);
}
