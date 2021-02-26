/*
 * Copyright (c) 2014 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC.
 *                         All rights reserved.
 * Copyright (c) 2015-2017 Cray Inc. All rights reserved.
 * Copyrigth (c) 2019      Triad National Security, LLC. All rights
 *                         reserved.
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

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <ofi_util.h>
#include <rdma/fabric.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_errno.h>
#include "ofi_prov.h"

#include "gnix.h"
#include "gnix_nic.h"
#include "gnix_cm.h"
#include "gnix_cm_nic.h"
#include "gnix_util.h"
#include "gnix_nameserver.h"
#include "gnix_wait.h"
#include "gnix_xpmem.h"
#include "gnix_mbox_allocator.h"

/* check if only one bit of a set is enabled, when one is required */
#define IS_EXCLUSIVE(x) \
	((x) && !((x) & ((x)-1)))

/* optional basic bits */
#define GNIX_MR_BASIC_OPT \
	(FI_MR_LOCAL)

/* optional scalable bits */
#define GNIX_MR_SCALABLE_OPT \
	(FI_MR_LOCAL)

/* required basic bits */
#define GNIX_MR_BASIC_REQ \
	(FI_MR_VIRT_ADDR | FI_MR_ALLOCATED | FI_MR_PROV_KEY)

/* required scalable bits */
#define GNIX_MR_SCALABLE_REQ \
	(FI_MR_MMU_NOTIFY)

#define GNIX_MR_BASIC_BITS \
	(GNIX_MR_BASIC_OPT | GNIX_MR_BASIC_REQ)

#define GNIX_MR_SCALABLE_BITS \
	(GNIX_MR_SCALABLE_OPT | GNIX_MR_SCALABLE_REQ)

const char gnix_fab_name[] = "gni";
const char gnix_dom_name[] = "/sys/class/gni/kgni0";
const char gnix_prov_name[] = "gni";

uint32_t gnix_cdm_modes =
	(GNI_CDM_MODE_FAST_DATAGRAM_POLL | GNI_CDM_MODE_FMA_SHARED |
	GNI_CDM_MODE_FMA_SMALL_WINDOW | GNI_CDM_MODE_FORK_PARTCOPY |
	GNI_CDM_MODE_ERR_NO_KILL);

/* default number of directed datagrams per domain */
static int gnix_def_gni_n_dgrams = 128;
/* default number of wildcard datagrams per domain */
static int gnix_def_gni_n_wc_dgrams = 4;
static uint64_t gnix_def_gni_datagram_timeouts = -1;

static struct fi_ops gnix_fab_fi_ops;
static struct fi_gni_ops_fab gnix_ops_fab;
static struct fi_gni_auth_key_ops_fab gnix_fab_ak_ops;

static int __gnix_auth_key_initialize(
		uint8_t *auth_key,
		size_t auth_key_size,
		struct gnix_auth_key_attr *attr);
static int __gnix_auth_key_set_val(
		uint8_t *auth_key,
		size_t auth_key_size,
		gnix_auth_key_opt_t opt,
		void *val);
static int __gnix_auth_key_get_val(
		uint8_t *auth_key,
		size_t auth_key_size,
		gnix_auth_key_opt_t opt,
		void *val);

#define GNIX_DEFAULT_USER_REGISTRATION_LIMIT 192
#define GNIX_DEFAULT_PROV_REGISTRATION_LIMIT 64
#define GNIX_DEFAULT_SHARED_MEMORY_TIMEOUT 30

int gnix_default_user_registration_limit = GNIX_DEFAULT_USER_REGISTRATION_LIMIT;
int gnix_default_prov_registration_limit = GNIX_DEFAULT_PROV_REGISTRATION_LIMIT;
uint32_t gnix_wait_shared_memory_timeout = GNIX_DEFAULT_SHARED_MEMORY_TIMEOUT;

/* assume that the user will open additional fabrics later and that
   ptag information will need to be retained for the lifetime of the
   process. If the user sets this value, we can assume that they
   intend to be done with libfabric when the last fabric instance
   closes so that we can free the ptag information. */
int gnix_dealloc_aki_on_fabric_close = 0;

const struct fi_fabric_attr gnix_fabric_attr = {
	.fabric = NULL,
	.name = NULL,
	.prov_name = NULL,
	.prov_version = FI_VERSION(GNI_MAJOR_VERSION, GNI_MINOR_VERSION),
};

DIRECT_FN int gnix_fabric_trywait(struct fid_fabric *fabric, struct fid **fids, int count)
{
	return -FI_ENOSYS;
}

static struct fi_ops_fabric gnix_fab_ops = {
	.size = sizeof(struct fi_ops_fabric),
	.domain = gnix_domain_open,
	.passive_ep = gnix_pep_open,
	.eq_open = gnix_eq_open,
	.wait_open = gnix_wait_open,
	.trywait = gnix_fabric_trywait
};

static void __fabric_destruct(void *obj)
{
	struct gnix_fid_fabric *fab = (struct gnix_fid_fabric *) obj;

	_gnix_app_cleanup();

	free(fab);
}

static int gnix_fab_ops_open(struct fid *fid, const char *ops_name,
				uint64_t flags, void **ops, void *context)
{
	if (strcmp(ops_name, FI_GNI_FAB_OPS_1) == 0)
		*ops = &gnix_ops_fab;
	else if (strcmp(ops_name, FI_GNI_FAB_OPS_2) == 0)
		*ops = &gnix_fab_ak_ops;
	else
		return -FI_EINVAL;

	return 0;
}

static int gnix_fabric_close(fid_t fid)
{
	struct gnix_fid_fabric *fab;
	int references_held;

	fab = container_of(fid, struct gnix_fid_fabric, fab_fid);

	references_held = _gnix_ref_put(fab);
	if (references_held)
		GNIX_INFO(FI_LOG_FABRIC, "failed to fully close fabric due "
				"to lingering references. references=%i fabric=%p\n",
				references_held, fab);

	return FI_SUCCESS;
}

/*
 * define methods needed for the GNI fabric provider
 */
static int gnix_fabric_open(struct fi_fabric_attr *attr,
			    struct fid_fabric **fabric,
			    void *context)
{
	struct gnix_fid_fabric *fab;

	if (strcmp(attr->name, gnix_fab_name)) {
		return -FI_ENODATA;
	}

	fab = calloc(1, sizeof(*fab));
	if (!fab) {
		return -FI_ENOMEM;
	}

	/*
	 * set defaults related to use of GNI datagrams
	 */
	fab->n_bnd_dgrams = gnix_def_gni_n_dgrams;
	fab->n_wc_dgrams = gnix_def_gni_n_wc_dgrams;
	fab->datagram_timeout = gnix_def_gni_datagram_timeouts;

	fab->fab_fid.fid.fclass = FI_CLASS_FABRIC;
	fab->fab_fid.fid.context = context;
	fab->fab_fid.fid.ops = &gnix_fab_fi_ops;
	fab->fab_fid.ops = &gnix_fab_ops;
	_gnix_ref_init(&fab->ref_cnt, 1, __fabric_destruct);
	dlist_init(&fab->domain_list);

	*fabric = &fab->fab_fid;

	return FI_SUCCESS;
}

static struct fi_info *_gnix_allocinfo(void)
{
	struct fi_info *gnix_info;

	gnix_info = fi_allocinfo();
	if (gnix_info == NULL) {
		return NULL;
	}

	gnix_info->caps = GNIX_EP_CAPS_FULL;
	gnix_info->tx_attr->op_flags = 0;
	gnix_info->rx_attr->op_flags = 0;
	gnix_info->ep_attr->type = FI_EP_RDM;
	gnix_info->ep_attr->protocol = FI_PROTO_GNI;
	gnix_info->ep_attr->max_msg_size = GNIX_MAX_MSG_SIZE;
	gnix_info->ep_attr->mem_tag_format = FI_TAG_GENERIC;
	gnix_info->ep_attr->tx_ctx_cnt = 1;
	gnix_info->ep_attr->rx_ctx_cnt = 1;

	gnix_info->domain_attr->threading = FI_THREAD_SAFE;
	gnix_info->domain_attr->control_progress = FI_PROGRESS_AUTO;
	gnix_info->domain_attr->data_progress = FI_PROGRESS_AUTO;
	gnix_info->domain_attr->av_type = FI_AV_UNSPEC;
	/*
	 * the cm_nic currently sucks up one of the gnix_nic's so
	 * we have to subtract one from the gnix_max_nics_per_ptag.
	 */
	gnix_info->domain_attr->tx_ctx_cnt = (gnix_max_nics_per_ptag == 1) ?
						1 : gnix_max_nics_per_ptag - 1;
	gnix_info->domain_attr->rx_ctx_cnt = gnix_max_nics_per_ptag;
	gnix_info->domain_attr->cntr_cnt = _gnix_get_cq_limit() / 2;
	gnix_info->domain_attr->cq_cnt = _gnix_get_cq_limit() / 2;
	gnix_info->domain_attr->ep_cnt = SIZE_MAX;

	gnix_info->domain_attr->name = strdup(gnix_dom_name);
	gnix_info->domain_attr->cq_data_size = sizeof(uint64_t);
	gnix_info->domain_attr->mr_mode = FI_MR_BASIC;
	gnix_info->domain_attr->resource_mgmt = FI_RM_ENABLED;
	gnix_info->domain_attr->mr_key_size = sizeof(uint64_t);
	gnix_info->domain_attr->max_ep_tx_ctx = GNIX_SEP_MAX_CNT;
	gnix_info->domain_attr->max_ep_rx_ctx = GNIX_SEP_MAX_CNT;
	gnix_info->domain_attr->mr_iov_limit = 1;
	gnix_info->domain_attr->caps = GNIX_DOM_CAPS;
	gnix_info->domain_attr->mode = 0;
	gnix_info->domain_attr->mr_cnt = 65535;

	gnix_info->next = NULL;
	gnix_info->addr_format = FI_ADDR_GNI;
	gnix_info->src_addrlen = sizeof(struct gnix_ep_name);
	gnix_info->dest_addrlen = sizeof(struct gnix_ep_name);
	gnix_info->src_addr = NULL;
	gnix_info->dest_addr = NULL;

	gnix_info->tx_attr->msg_order = FI_ORDER_SAS;
	gnix_info->tx_attr->comp_order = FI_ORDER_NONE;
	gnix_info->tx_attr->size = GNIX_TX_SIZE_DEFAULT;
	gnix_info->tx_attr->iov_limit = GNIX_MAX_MSG_IOV_LIMIT;
	gnix_info->tx_attr->inject_size = GNIX_INJECT_SIZE;
	gnix_info->tx_attr->rma_iov_limit = GNIX_MAX_RMA_IOV_LIMIT;
	gnix_info->rx_attr->msg_order = FI_ORDER_SAS;
	gnix_info->rx_attr->comp_order = FI_ORDER_NONE;
	gnix_info->rx_attr->size = GNIX_RX_SIZE_DEFAULT;
	gnix_info->rx_attr->iov_limit = GNIX_MAX_MSG_IOV_LIMIT;

	return gnix_info;
}

static int __gnix_getinfo_resolve_node(const char *node, const char *service,
				       uint64_t flags, const struct fi_info *hints,
				       struct fi_info *info)
{
	int ret;
	struct gnix_ep_name *dest_addr = NULL;
	struct gnix_ep_name *src_addr = NULL;
	bool is_fi_addr_str = false;

	/* TODO: Add version check when we decide on how to do it */
	if (hints && hints->addr_format == FI_ADDR_STR) {
		is_fi_addr_str = true;
	}

	if (OFI_UNLIKELY(is_fi_addr_str && node && service)) {
		GNIX_WARN(FI_LOG_FABRIC, "service parameter must be NULL when "
			"node parameter is not and using FI_ADDR_STR.\n");
		return -FI_EINVAL;
	}

	if (flags & FI_SOURCE) {
		/* -resolve node/port to make info->src_addr
		 * -ignore hints->src_addr
		 * -copy hints->dest_addr to output info */
		src_addr = malloc(sizeof(*src_addr));
		if (!src_addr) {
			ret = -FI_ENOMEM;
			goto err;
		}

		if (is_fi_addr_str) {
			ret = _gnix_ep_name_from_str(node, src_addr);
		} else {
			ret = _gnix_resolve_name(node, service, flags,
						 src_addr);
		}

		if (ret != FI_SUCCESS) {
			ret = -FI_ENODATA;
			goto err;
		}

		if (hints && hints->dest_addr) {
			dest_addr = malloc(sizeof(*dest_addr));
			if (!dest_addr) {
				ret = -FI_ENOMEM;
				goto err;
			}

			memcpy(dest_addr, hints->dest_addr,
			       hints->dest_addrlen);
		}
	} else {
		/* -try to resolve node/port to make info->dest_addr
		 * -fallback to copying hints->dest_addr to output info
		 * -try to copy hints->src_addr to output info
		 * -falback to finding src_addr for output info */
		if (node || service) {
			dest_addr = malloc(sizeof(*dest_addr));
			if (!dest_addr) {
				ret = -FI_ENOMEM;
				goto err;
			}

			if (is_fi_addr_str) {
				ret = _gnix_ep_name_from_str(node, dest_addr);
			} else {
				ret = _gnix_resolve_name(node, service, flags,
							 dest_addr);
			}

			if (ret != FI_SUCCESS) {
				ret = -FI_ENODATA;
				goto err;
			}
		} else {
			if (hints && hints->dest_addr) {
				dest_addr = malloc(sizeof(*dest_addr));
				if (!dest_addr) {
					ret = -FI_ENOMEM;
					goto err;
				}

				memcpy(dest_addr, hints->dest_addr,
				       hints->dest_addrlen);
			}
		}

		if (hints && hints->src_addr) {
			src_addr = malloc(sizeof(*src_addr));
			if (!src_addr) {
				ret = -FI_ENOMEM;
				goto err;
			}

			memcpy(src_addr, hints->src_addr, hints->src_addrlen);
		} else {
			src_addr = malloc(sizeof(*src_addr));
			if (!src_addr) {
				ret = -FI_ENOMEM;
				goto err;
			}

			ret = _gnix_src_addr(src_addr);
			if (ret != FI_SUCCESS)
				goto err;
		}
	}

	GNIX_INFO(FI_LOG_FABRIC, "%snode: %s service: %s\n",
		  flags & FI_SOURCE ? "(FI_SOURCE) " : "", node, service);

	if (src_addr)
		GNIX_INFO(FI_LOG_FABRIC, "src_pe: 0x%x src_port: 0x%lx\n",
			  src_addr->gnix_addr.device_addr,
			  src_addr->gnix_addr.cdm_id);
	if (dest_addr)
		GNIX_INFO(FI_LOG_FABRIC, "dest_pe: 0x%x dest_port: 0x%lx\n",
			  dest_addr->gnix_addr.device_addr,
			  dest_addr->gnix_addr.cdm_id);

	if (src_addr) {
		info->src_addr = src_addr;
		info->src_addrlen = sizeof(*src_addr);
	}

	if (dest_addr) {
		info->dest_addr = dest_addr;
		info->dest_addrlen = sizeof(*dest_addr);
	}

	return FI_SUCCESS;

err:
	free(src_addr);
	free(dest_addr);

	return ret;
}

static int _gnix_ep_getinfo(enum fi_ep_type ep_type, uint32_t version,
			    const char *node, const char *service,
			    uint64_t flags, const struct fi_info *hints,
			    struct fi_info **info)
{
	uint64_t mode = GNIX_FAB_MODES;
	struct fi_info *gnix_info = NULL;
	int ret = -FI_ENODATA;
	int mr_mode;

	GNIX_TRACE(FI_LOG_FABRIC, "\n");

	if ((hints && hints->ep_attr) &&
	    (hints->ep_attr->type != FI_EP_UNSPEC &&
	     hints->ep_attr->type != ep_type)) {
		return -FI_ENODATA;
	}

	gnix_info = _gnix_allocinfo();
	if (!gnix_info)
		return -FI_ENOMEM;

	gnix_info->ep_attr->type = ep_type;

	if (hints) {
		/* TODO: Add version check when we decide on how to do it */
		if (hints->addr_format == FI_ADDR_STR) {
			gnix_info->addr_format = FI_ADDR_STR;
		}

		if (hints->ep_attr) {
			/* Only support FI_PROTO_GNI protocol. */
			switch (hints->ep_attr->protocol) {
			case FI_PROTO_UNSPEC:
			case FI_PROTO_GNI:
				break;
			default:
				goto err;
			}

			if ((hints->ep_attr->tx_ctx_cnt > GNIX_SEP_MAX_CNT) &&
				(hints->ep_attr->tx_ctx_cnt !=
						FI_SHARED_CONTEXT)) {
				goto err;
			}

			if (hints->ep_attr->rx_ctx_cnt > GNIX_SEP_MAX_CNT)
				goto err;

			if (hints->ep_attr->tx_ctx_cnt)
				gnix_info->ep_attr->tx_ctx_cnt =
					hints->ep_attr->tx_ctx_cnt;
			if (hints->ep_attr->rx_ctx_cnt)
				gnix_info->ep_attr->rx_ctx_cnt =
					hints->ep_attr->rx_ctx_cnt;

			if (hints->ep_attr->max_msg_size > GNIX_MAX_MSG_SIZE)
				goto err;
		}

		GNIX_DEBUG(FI_LOG_FABRIC, "Passed EP attributes check\n");

		/*
		 * check the mode field
		 */
		if (hints->mode) {
			if ((hints->mode & GNIX_FAB_MODES) != GNIX_FAB_MODES) {
				goto err;
			}
			mode = hints->mode & ~GNIX_FAB_MODES_CLEAR;
			if (FI_VERSION_LT(version, FI_VERSION(1, 5))) {
				mode = hints->mode & ~FI_NOTIFY_FLAGS_ONLY;
			}
		}

		GNIX_DEBUG(FI_LOG_FABRIC, "Passed mode check\n");

		if (hints->caps) {
			/* The provider must support all requested
			 * capabilities. */
			if ((hints->caps & GNIX_EP_CAPS_FULL) != hints->caps)
				goto err;
		}

		GNIX_DEBUG(FI_LOG_FABRIC, "Passed caps check gnix_info->caps = 0x%016lx\n",
			   gnix_info->caps);

		if (hints->tx_attr) {
			if ((hints->tx_attr->op_flags & GNIX_EP_OP_FLAGS) !=
				hints->tx_attr->op_flags) {
				goto err;
			}
			if (hints->tx_attr->inject_size > GNIX_INJECT_SIZE) {
				goto err;
			}

			gnix_info->tx_attr->op_flags =
				hints->tx_attr->op_flags & GNIX_EP_OP_FLAGS;
		}

		GNIX_DEBUG(FI_LOG_FABRIC, "Passed TX attributes check\n");

		if (hints->rx_attr) {
			if ((hints->rx_attr->op_flags & GNIX_EP_OP_FLAGS) !=
					hints->rx_attr->op_flags) {
				goto err;
			}

			gnix_info->rx_attr->op_flags =
				hints->rx_attr->op_flags & GNIX_EP_OP_FLAGS;
		}

		if (hints->fabric_attr && hints->fabric_attr->name &&
		    strncmp(hints->fabric_attr->name, gnix_fab_name,
			    strlen(gnix_fab_name))) {
			goto err;
		}

		GNIX_DEBUG(FI_LOG_FABRIC, "Passed fabric name check\n");

		if (hints->domain_attr) {
			mr_mode = hints->domain_attr->mr_mode;

			if (hints->domain_attr->name &&
			    strncmp(hints->domain_attr->name, gnix_dom_name,
				    strlen(gnix_dom_name))) {
				goto err;
			}

			if (hints->domain_attr->control_progress !=
				FI_PROGRESS_UNSPEC)
				gnix_info->domain_attr->control_progress =
					hints->domain_attr->control_progress;

			if (hints->domain_attr->data_progress !=
				FI_PROGRESS_UNSPEC)
				gnix_info->domain_attr->data_progress =
					hints->domain_attr->data_progress;

			/* If basic registration isn't being requested,
			   require FI_MR_MMU_NOTIFY */
			if (!(hints->domain_attr->mr_mode &
					(FI_MR_BASIC | FI_MR_ALLOCATED)))
				gnix_info->domain_attr->mr_mode |= FI_MR_MMU_NOTIFY;

			if (ofi_check_mr_mode(&gnix_prov, version,
					gnix_info->domain_attr->mr_mode,
					hints) != FI_SUCCESS) {
				GNIX_INFO(FI_LOG_DOMAIN,
					"failed ofi_check_mr_mode, "
					"ret=%d\n", ret);
				goto err;
			}

			if (FI_VERSION_LT(version, FI_VERSION(1, 5))) {
				switch (mr_mode) {
				case FI_MR_UNSPEC:
				case FI_MR_BASIC:
					mr_mode = FI_MR_BASIC;
					break;
				default:
					GNIX_DEBUG(FI_LOG_FABRIC,
						"unsupported mr_mode selected, "
						"ret=%d\n", ret);
					goto err;
				}
			} else {
				/* define the mode we return to the user
				 * prefer basic until scalable
				 * has more testing time */
				if (mr_mode & FI_MR_BASIC)
					mr_mode = OFI_MR_BASIC_MAP;
				else if ((mr_mode & GNIX_MR_BASIC_REQ) ==
							GNIX_MR_BASIC_REQ)
					mr_mode &= GNIX_MR_BASIC_BITS;
				else
					mr_mode &= GNIX_MR_SCALABLE_BITS;
			}

			gnix_info->domain_attr->mr_mode = mr_mode;

			switch (hints->domain_attr->threading) {
			case FI_THREAD_COMPLETION:
				gnix_info->domain_attr->threading =
					hints->domain_attr->threading;
				break;
			default:
				break;
			}

			if (hints->domain_attr->caps) {
				if (hints->domain_attr->caps & ~GNIX_DOM_CAPS) {
					GNIX_WARN(FI_LOG_FABRIC,
						  "Invalid domain caps\n");
					goto err;
				}

				gnix_info->domain_attr->caps =
					hints->domain_attr->caps;
			}

		}
	}

	ret = __gnix_getinfo_resolve_node(node, service, flags, hints,
					  gnix_info);
	if (ret != FI_SUCCESS)
		goto err;

	ofi_alter_info(gnix_info, hints, version);

	GNIX_DEBUG(FI_LOG_FABRIC, "Passed the domain attributes check\n");

	/* The provider may silently enable secondary
	 * capabilities that do not introduce any overhead. */
	if (hints && hints->caps)
		gnix_info->caps = hints->caps | GNIX_EP_SEC_CAPS;
	else
		gnix_info->caps = GNIX_EP_CAPS_FULL | GNIX_EP_SEC_CAPS;

	gnix_info->mode = mode;
	gnix_info->fabric_attr->name = strdup(gnix_fab_name);
	gnix_info->tx_attr->caps = gnix_info->caps;
	gnix_info->tx_attr->mode = gnix_info->mode;
	gnix_info->rx_attr->caps = gnix_info->caps;
	gnix_info->rx_attr->mode = gnix_info->mode;

	*info = gnix_info;

	GNIX_DEBUG(FI_LOG_FABRIC, "Returning EP type: %s\n",
		   fi_tostr(&ep_type, FI_TYPE_EP_TYPE));
	return FI_SUCCESS;
err:
	fi_freeinfo(gnix_info);
	return ret;
}

static int gnix_getinfo(uint32_t version, const char *node, const char *service,
			uint64_t flags, const struct fi_info *hints,
			struct fi_info **info)
{
	int ret = 0;
	struct fi_info *info_ptr;

	/* Note that info entries are added to the head of 'info', that is,
	 * they are preferred in the reverse order shown here. */

	*info = NULL;

	ret = _gnix_ep_getinfo(FI_EP_MSG, version, node, service, flags,
			       hints, &info_ptr);
	if (ret == FI_SUCCESS) {
		info_ptr->next = *info;
		*info = info_ptr;
	}

	ret = _gnix_ep_getinfo(FI_EP_DGRAM, version, node, service, flags,
			       hints, &info_ptr);
	if (ret == FI_SUCCESS) {
		info_ptr->next = *info;
		*info = info_ptr;
	}

	ret = _gnix_ep_getinfo(FI_EP_RDM, version, node, service, flags,
			       hints, &info_ptr);
	if (ret == FI_SUCCESS) {
		info_ptr->next = *info;
		*info = info_ptr;
	}

	return *info ? FI_SUCCESS : -FI_ENODATA;
}

static void gnix_fini(void)
{
}

struct fi_provider gnix_prov = {
	.name = gnix_prov_name,
	.version = FI_VERSION(GNI_MAJOR_VERSION, GNI_MINOR_VERSION),
	.fi_version = OFI_VERSION_LATEST,
	.getinfo = gnix_getinfo,
	.fabric = gnix_fabric_open,
	.cleanup = gnix_fini
};

GNI_INI
{
	struct fi_provider *provider = NULL;
	gni_return_t status;
	gni_version_info_t lib_version;
	int num_devices;
	int ret;

	/*
	 * if no GNI devices available, don't register as provider
	 */
	status = GNI_GetNumLocalDevices(&num_devices);
	if ((status != GNI_RC_SUCCESS) || (num_devices == 0)) {
		return NULL;
	}

	/*
	 * ensure all globals are properly initialized
	 */
	_gnix_init();

	/* sanity check that the 1 aries/node holds */
	assert(num_devices == 1);

	/*
	 * don't register if available ugni is older than one libfabric was
	 * built against
	 */
	status = GNI_GetVersionInformation(&lib_version);
	if ((GNI_GET_MAJOR(lib_version.ugni_version) > GNI_MAJOR_REV) ||
	    ((GNI_GET_MAJOR(lib_version.ugni_version) == GNI_MAJOR_REV) &&
	     GNI_GET_MINOR(lib_version.ugni_version) >= GNI_MINOR_REV)) {
		provider = &gnix_prov;
	}

	/* Initialize global MR notifier. */
	ret = _gnix_smrn_init();
	if (ret != FI_SUCCESS)
		GNIX_FATAL(FI_LOG_FABRIC,
			"failed to initialize global mr notifier\n");

	/* Initialize global NIC data. */
	_gnix_nic_init();

	if (getenv("GNIX_DISABLE_XPMEM") != NULL)
		gnix_xpmem_disabled = true;
	if (getenv("GNIX_MBOX_FALLBACK_DISABLE") != NULL)
		gnix_mbox_alloc_allow_fallback = false;

	return (provider);
}

static int
__gnix_fab_ops_get_val(struct fid *fid, fab_ops_val_t t, void *val)
{
	GNIX_TRACE(FI_LOG_FABRIC, "\n");

	assert(val);

	if (fid->fclass != FI_CLASS_FABRIC) {
		GNIX_WARN(FI_LOG_FABRIC, "Invalid fabric\n");
		return -FI_EINVAL;
	}

	switch (t) {
	case GNI_WAIT_THREAD_SLEEP:
		*(uint32_t *)val = gnix_wait_thread_sleep_time;
		break;
	case GNI_DEFAULT_USER_REGISTRATION_LIMIT:
		*(uint32_t *)val = gnix_default_user_registration_limit;
		break;
	case GNI_DEFAULT_PROV_REGISTRATION_LIMIT:
		*(uint32_t *)val = gnix_default_prov_registration_limit;
		break;
	case GNI_WAIT_SHARED_MEMORY_TIMEOUT:
		*(uint32_t *)val = gnix_wait_shared_memory_timeout;
		break;
	default:
		GNIX_WARN(FI_LOG_FABRIC, ("Invalid fab_ops_val\n"));
	}

	return FI_SUCCESS;
}

static int
__gnix_fab_ops_set_val(struct fid *fid, fab_ops_val_t t, void *val)
{
	int v;

	assert(val);

	if (fid->fclass != FI_CLASS_FABRIC) {
		GNIX_WARN(FI_LOG_FABRIC, "Invalid fabric\n");
		return -FI_EINVAL;
	}

	switch (t) {
	case GNI_WAIT_THREAD_SLEEP:
		v = *(uint32_t *) val;
		gnix_wait_thread_sleep_time = v;
		break;
	case GNI_DEFAULT_USER_REGISTRATION_LIMIT:
		v = *(uint32_t *) val;
		if (v > GNIX_MAX_SCALABLE_REGISTRATIONS) {
			GNIX_ERR(FI_LOG_FABRIC,
				"User specified an invalid user registration "
				"limit, requested=%d maximum=%d\n",
				v, GNIX_MAX_SCALABLE_REGISTRATIONS);
			return -FI_EINVAL;
		}
		gnix_default_user_registration_limit = v;
		break;
	case GNI_DEFAULT_PROV_REGISTRATION_LIMIT:
		v = *(uint32_t *) val;
		if (v > GNIX_MAX_SCALABLE_REGISTRATIONS) {
			GNIX_ERR(FI_LOG_FABRIC,
				"User specified an invalid prov registration "
				"limit, requested=%d maximum=%d\n",
				v, GNIX_MAX_SCALABLE_REGISTRATIONS);
			return -FI_EINVAL;
		}
		gnix_default_prov_registration_limit = v;
		break;
	case GNI_WAIT_SHARED_MEMORY_TIMEOUT:
		v = *(uint32_t *) val;
		gnix_wait_shared_memory_timeout = v;
		break;
	default:
		GNIX_WARN(FI_LOG_FABRIC, ("Invalid fab_ops_val\n"));
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

static int __gnix_auth_key_initialize(
	uint8_t *auth_key,
	size_t auth_key_size,
	struct gnix_auth_key_attr *attr)
{
	struct gnix_auth_key *info = NULL;
	int ret = FI_SUCCESS;

	info = _gnix_auth_key_lookup(auth_key, auth_key_size);

	if (info) {
		GNIX_WARN(FI_LOG_FABRIC, "authorization key is already "
			"initialized, auth_key=%d auth_key_size=%d\n",
			auth_key, auth_key_size);
		return -FI_ENOSPC; /* already initialized*/
	}

	info = _gnix_auth_key_alloc();
	if (!info)
		return -FI_ENOMEM;

	if (attr)
		info->attr = *attr;
	else {
		info->attr.user_key_limit =
			gnix_default_user_registration_limit;
		info->attr.prov_key_limit =
			gnix_default_prov_registration_limit;
	}

	ret = _gnix_auth_key_insert(auth_key, auth_key_size, info);
	if (ret) {
		GNIX_INFO(FI_LOG_FABRIC, "failed to insert authorization key"
			", key=%p len=%d ret=%d\n",
			auth_key, auth_key_size, ret);
		_gnix_auth_key_free(info);
		info = NULL;
	}

	return ret;
}

static int __gnix_auth_key_set_val(
	uint8_t *auth_key,
	size_t auth_key_size,
	gnix_auth_key_opt_t opt,
	void *val)
{
	struct gnix_auth_key *info;
	int v;
	int ret = FI_SUCCESS;

	if (!val)
		return -FI_EINVAL;

	info = _gnix_auth_key_lookup(auth_key, auth_key_size);

	if (!info) {
		ret = __gnix_auth_key_initialize(auth_key, auth_key_size, NULL);
		assert(ret == FI_SUCCESS);

		info = _gnix_auth_key_lookup(auth_key, auth_key_size);
		assert(info);
	}

	/* if the limits have already been set, and the user is
	 * trying to modify it, kick it back */
	if (opt == GNIX_USER_KEY_LIMIT || opt == GNIX_PROV_KEY_LIMIT) {
		fastlock_acquire(&info->lock);
		if (info->enabled) {
			fastlock_release(&info->lock);
			GNIX_INFO(FI_LOG_FABRIC, "authorization key already "
				"enabled and cannot be modified\n");
			return -FI_EAGAIN;
		}
	}

	switch (opt) {
	case GNIX_USER_KEY_LIMIT:
		v = *(int *) val;
		if (v >= GNIX_MAX_SCALABLE_REGISTRATIONS) {
			GNIX_ERR(FI_LOG_FABRIC,
				"User is requesting more registrations than is present on node\n");
			ret = -FI_EINVAL;
		} else
			info->attr.user_key_limit = v;
		fastlock_release(&info->lock);
		break;
	case GNIX_PROV_KEY_LIMIT:
		v = *(int *) val;
		if (v >= GNIX_MAX_SCALABLE_REGISTRATIONS) {
			GNIX_ERR(FI_LOG_FABRIC,
				"User is requesting more registrations than is present on node\n");
			ret = -FI_EINVAL;
		}
		info->attr.prov_key_limit = v;
		fastlock_release(&info->lock);
		break;
	case GNIX_TOTAL_KEYS_NEEDED:
		GNIX_WARN(FI_LOG_FABRIC,
			"GNIX_TOTAL_KEYS_NEEDED is not a definable value.\n");
		return -FI_EOPNOTSUPP;
	case GNIX_USER_KEY_MAX_PER_RANK:
		GNIX_WARN(FI_LOG_FABRIC,
			"GNIX_USER_KEY_MAX_PER_RANK is not a definable "
			"value.\n");
		return -FI_EOPNOTSUPP;
	default:
		GNIX_WARN(FI_LOG_FABRIC, ("Invalid fab_ops_val\n"));
		return -FI_EINVAL;
	}

	return ret;
}

static int __gnix_auth_key_get_val(
	uint8_t *auth_key,
	size_t auth_key_size,
	gnix_auth_key_opt_t opt,
	void *val)
{
	struct gnix_auth_key *info;
	uint32_t pes_on_node;
	int ret;

	if (!val)
		return -FI_EINVAL;

	info = _gnix_auth_key_lookup(auth_key, auth_key_size);

	switch (opt) {
	case GNIX_USER_KEY_LIMIT:
		*(int *)val = (info) ?
			info->attr.user_key_limit :
			gnix_default_user_registration_limit;
		break;
	case GNIX_PROV_KEY_LIMIT:
		*(int *)val = (info) ?
			info->attr.prov_key_limit :
			gnix_default_prov_registration_limit;
		break;
	case GNIX_TOTAL_KEYS_NEEDED:
		*(uint32_t *)val = ((info) ?
			(info->attr.user_key_limit +
			info->attr.prov_key_limit) :
			(gnix_default_user_registration_limit +
			 gnix_default_prov_registration_limit));
		break;
	case GNIX_USER_KEY_MAX_PER_RANK:
		ret = _gnix_pes_on_node(&pes_on_node);
		if (ret) {
			GNIX_WARN(FI_LOG_FABRIC,
				"failed to get pes on node count\n");
			return -FI_EINVAL;
		}

		*(int *)val = ((info) ?
			info->attr.user_key_limit :
			gnix_default_user_registration_limit) /
			pes_on_node;
		break;
	default:
		GNIX_WARN(FI_LOG_FABRIC, ("Invalid fab_ops_val\n"));
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

/*******************************************************************************
 * FI_OPS_* data structures.
 ******************************************************************************/

static struct fi_gni_ops_fab gnix_ops_fab = {
	.set_val = __gnix_fab_ops_set_val,
	.get_val = __gnix_fab_ops_get_val
};

static struct fi_ops gnix_fab_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = gnix_fabric_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = gnix_fab_ops_open,
};

static struct fi_gni_auth_key_ops_fab gnix_fab_ak_ops = {
	.set_val = __gnix_auth_key_set_val,
	.get_val = __gnix_auth_key_get_val,
};
