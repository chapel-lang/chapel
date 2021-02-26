/*
 * Copyright (c) 2014-2019, Cisco Systems, Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include <arpa/inet.h>

#include <rdma/fabric.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_errno.h>
#include "ofi.h"
#include "ofi_enosys.h"
#include "ofi_util.h"

#include "usnic_direct.h"
#include "usdf.h"
#include "usdf_timer.h"
#include "usdf_poll.h"
#include "usdf_cm.h"

static int
usdf_domain_bind(struct fid *fid, struct fid *bfid, uint64_t flags)
{
        struct usdf_domain *udp;

	USDF_TRACE_SYS(DOMAIN, "\n");

	if (flags & FI_REG_MR) {
		USDF_WARN_SYS(DOMAIN,
			"FI_REG_MR for EQs is not supported by the usnic provider");
		return -FI_EOPNOTSUPP;
	}

        udp = dom_fidtou(fid);

        switch (bfid->fclass) {
        case FI_CLASS_EQ:
                if (udp->dom_eq != NULL) {
                        return -FI_EINVAL;
                }
                udp->dom_eq = eq_fidtou(bfid);
                ofi_atomic_inc32(&udp->dom_eq->eq_refcnt);
                break;
        default:
                return -FI_EINVAL;
        }

        return 0;
}

static int
usdf_domain_close(fid_t fid)
{
	struct usdf_domain *udp;
	int ret;

	USDF_TRACE_SYS(DOMAIN, "\n");

	udp = container_of(fid, struct usdf_domain, dom_fid.fid);
	if (ofi_atomic_get32(&udp->dom_refcnt) > 0) {
		return -FI_EBUSY;
	}

	if (udp->dom_dev != NULL) {
		ret = usd_close(udp->dom_dev);
		if (ret != 0) {
			return ret;
		}
	}

	if (udp->dom_eq != NULL) {
		ofi_atomic_dec32(&udp->dom_eq->eq_refcnt);
	}
	ofi_atomic_dec32(&udp->dom_fabric->fab_refcnt);
	LIST_REMOVE(udp, dom_link);
	fi_freeinfo(udp->dom_info);
	free(udp);

	return 0;
}

static struct fi_ops usdf_fid_ops = {
	.size = sizeof(struct fi_ops),
	.close = usdf_domain_close,
	.bind = usdf_domain_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_mr usdf_domain_mr_ops = {
	.size = sizeof(struct fi_ops_mr),
	.reg = usdf_reg_mr,
	.regv = usdf_regv_mr,
	.regattr = usdf_regattr,
};

static struct fi_ops_domain usdf_domain_ops = {
	.size = sizeof(struct fi_ops_domain),
	.av_open = usdf_av_open,
	.cq_open = usdf_cq_open,
	.endpoint = usdf_endpoint_open,
	.scalable_ep = fi_no_scalable_ep,
	.cntr_open = fi_no_cntr_open,
	.poll_open = usdf_poll_open,
	.stx_ctx = fi_no_stx_context,
	.srx_ctx = fi_no_srx_context,
	.query_atomic = usdf_query_atomic,
	.query_collective = fi_no_query_collective,
};

int
usdf_domain_open(struct fid_fabric *fabric, struct fi_info *info,
	   struct fid_domain **domain, void *context)
{
	struct usdf_fabric *fp;
	struct usdf_domain *udp;
	struct sockaddr_in *sin;
	size_t addrlen;
	int ret;
#if ENABLE_DEBUG
	char requested[INET_ADDRSTRLEN], actual[INET_ADDRSTRLEN];
#endif

	USDF_TRACE_SYS(DOMAIN, "\n");
	sin = NULL;

	fp = fab_fidtou(fabric);

	if (info->domain_attr != NULL) {
		/* No versioning information available here. */
		if (!usdf_domain_checkname(0, fp->fab_dev_attrs,
					   info->domain_attr->name)) {
			USDF_WARN_SYS(DOMAIN, "domain name mismatch\n");
			return -FI_ENODATA;
		}

		if (ofi_check_mr_mode(
			&usdf_ops, fabric->api_version,
			FI_MR_BASIC | FI_MR_ALLOCATED | FI_MR_LOCAL, info)) {
			/* the caller ignored our fi_getinfo results */
			USDF_WARN_SYS(DOMAIN, "MR mode (%d) not supported\n",
				      info->domain_attr->mr_mode);
			return -FI_ENODATA;
		}
	}

	udp = calloc(1, sizeof *udp);
	if (udp == NULL) {
		USDF_DBG("unable to alloc mem for domain\n");
		ret = -FI_ENOMEM;
		goto fail;
	}

	USDF_DBG("uda_devname=%s\n", fp->fab_dev_attrs->uda_devname);

	/*
	 * Make sure address format is good and matches this fabric
	 */
	switch (info->addr_format) {
	case FI_SOCKADDR:
		addrlen = sizeof(struct sockaddr);
		sin = info->src_addr;
		break;
	case FI_SOCKADDR_IN:
		addrlen = sizeof(struct sockaddr_in);
		sin = info->src_addr;
		break;
	case FI_ADDR_STR:
		sin = usdf_format_to_sin(info, info->src_addr);
		if (NULL == sin) {
			ret = -FI_ENOMEM;
			goto fail;
		}
		goto skip_size_check;
	default:
		ret = -FI_EINVAL;
		goto fail;
	}

	if (info->src_addrlen != addrlen) {
		ret =  -FI_EINVAL;
		goto fail;
	}

skip_size_check:
	if (sin->sin_family != AF_INET ||
	    sin->sin_addr.s_addr != fp->fab_dev_attrs->uda_ipaddr_be) {
		USDF_DBG_SYS(DOMAIN, "requested src_addr (%s) != fabric addr (%s)\n",
			inet_ntop(AF_INET, &sin->sin_addr.s_addr,
				requested, sizeof(requested)),
			inet_ntop(AF_INET, &fp->fab_dev_attrs->uda_ipaddr_be,
				actual, sizeof(actual)));

		ret = -FI_EINVAL;
		usdf_free_sin_if_needed(info, sin);
		goto fail;
	}
	usdf_free_sin_if_needed(info, sin);

	ret = usd_open(fp->fab_dev_attrs->uda_devname, &udp->dom_dev);
	if (ret != 0) {
		goto fail;
	}

	udp->dom_fid.fid.fclass = FI_CLASS_DOMAIN;
	udp->dom_fid.fid.context = context;
	udp->dom_fid.fid.ops = &usdf_fid_ops;
	udp->dom_fid.ops = &usdf_domain_ops;
	udp->dom_fid.mr = &usdf_domain_mr_ops;

	ret = pthread_spin_init(&udp->dom_progress_lock,
			PTHREAD_PROCESS_PRIVATE);
	if (ret != 0) {
		ret = -ret;
		goto fail;
	}
	TAILQ_INIT(&udp->dom_tx_ready);
	TAILQ_INIT(&udp->dom_hcq_list);

	udp->dom_info = fi_dupinfo(info);
	if (udp->dom_info == NULL) {
		ret = -FI_ENOMEM;
		goto fail;
	}
	if (udp->dom_info->dest_addr != NULL) {
		free(udp->dom_info->dest_addr);
		udp->dom_info->dest_addr = NULL;
	}

	udp->dom_fabric = fp;
	LIST_INSERT_HEAD(&fp->fab_domain_list, udp, dom_link);
	ofi_atomic_initialize32(&udp->dom_refcnt, 0);
	ofi_atomic_inc32(&fp->fab_refcnt);

	*domain = &udp->dom_fid;
	return 0;

fail:
	if (udp != NULL) {
		if (udp->dom_info != NULL) {
			fi_freeinfo(udp->dom_info);
		}
		if (udp->dom_dev != NULL) {
			usd_close(udp->dom_dev);
		}
		free(udp);
	}
	return ret;
}

/* In pre-1.4, the domain name was NULL.
 *
 * There used to be elaborate schemes to try to preserve this pre-1.4
 * behavior.  In Nov 2019 discussions, however, it was determined that
 * we could rationalize classifying this as buggy behavior.
 * Specifically: we should just now always return a domain name --
 * even if the requested version is <1.4.
 *
 * This greatly simplifies the logic here, and also greatly simplifies
 * layering with the rxd provider.
 */
int usdf_domain_getname(uint32_t version, struct usd_device_attrs *dap,
			char **name)
{
	int ret = FI_SUCCESS;
	char *buf = NULL;

	buf = strdup(dap->uda_devname);
	if (NULL == buf) {
		ret = -errno;
		USDF_DBG("strdup failed while creating domain name\n");
	} else {
		*name = buf;
	}

	return ret;
}

/* Check to see if the name supplied in a hint matches the name of our
 * current domain.
 *
 * In pre-1.4, the domain name was NULL.
 *
 * There used to be elaborate schemes to try to preserve this pre-1.4
 * behavior.  In Nov 2019 discussions, however, it was determined that
 * we could rationalize classifying this as buggy behavior.
 * Specifically: we should just now always return a domain name --
 * even if the requested version is <1.4.
 *
 * This greatly simplifies the logic here, and also greatly simplifies
 * layering with the rxd provider.
 *
 * Hence, if a hint was provided, check the domain name (that we now
 * always have) against the hint.
 */
bool usdf_domain_checkname(uint32_t version, struct usd_device_attrs *dap,
			   const char *hint)
{
	char *reference = NULL;
	bool valid;
	int ret;

        /* If no hint was provided, then by definition, we agree with
	 * the hint. */
	if (NULL == hint) {
		return true;
	}

	USDF_DBG("checking domain name: domain name='%s'\n", hint);

	ret = usdf_domain_getname(version, dap, &reference);
	if (ret < 0) {
		return false;
	}

	valid = (strcmp(reference, hint) == 0);
	if (!valid) {
		USDF_DBG("given hint %s does not match %s -- invalid\n",
			hint, reference);
	}

	free(reference);
	return valid;
}

/* Query domain's atomic capability.
 * We dont support atomic operations, just return EOPNOTSUPP.
 */
int usdf_query_atomic(struct fid_domain *domain, enum fi_datatype datatype,
		      enum fi_op op, struct fi_atomic_attr *attr, uint64_t flags)
{
	return -FI_EOPNOTSUPP;
}

/* Catch the version changes for domain_attr. */
int usdf_catch_dom_attr(uint32_t version, const struct fi_info *hints,
			struct fi_domain_attr *dom_attr)
{
	/* version 1.5 introduced new bits. If the user asked for older
	 * version, we can't return these new bits.
	 */
	if (FI_VERSION_LT(version, FI_VERSION(1, 5))) {
		/* We checked mr_mode compatibility before calling
		 * this function. This means it is safe to return
		 * 1.4 default mr_mode.
		 */
		dom_attr->mr_mode = FI_MR_BASIC;

		/* FI_REMOTE_COMM is introduced in 1.5. So don't return it. */
		dom_attr->caps &= ~FI_REMOTE_COMM;

		/* If FI_REMOTE_COMM is given for version < 1.5, fail. */
		if (hints && hints->domain_attr) {
			if (hints->domain_attr->caps == FI_REMOTE_COMM)
				return -FI_EBADFLAGS;
		}
        } else {
            dom_attr->mr_mode &= ~(FI_MR_BASIC | FI_MR_SCALABLE);
	}

	return FI_SUCCESS;
}

/* Catch the version changes for tx_attr. */
int usdf_catch_tx_attr(uint32_t version, const struct fi_tx_attr *tx_attr)
{
	/* In version < 1.5, FI_LOCAL_MR is required. */
	if (FI_VERSION_LT(version, FI_VERSION(1, 5))) {
		if ((tx_attr->mode & FI_LOCAL_MR) == 0)
			return -FI_ENODATA;
	}

	return FI_SUCCESS;
}

/* Catch the version changes for rx_attr. */
int usdf_catch_rx_attr(uint32_t version, const struct fi_rx_attr *rx_attr)
{
	/* In version < 1.5, FI_LOCAL_MR is required. */
	if (FI_VERSION_LT(version, FI_VERSION(1, 5))) {
		if ((rx_attr->mode & FI_LOCAL_MR) == 0)
			return -FI_ENODATA;
	}

	return FI_SUCCESS;
}
