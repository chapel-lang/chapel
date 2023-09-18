/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021-2023 Cornelis Networks.
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
#include <ofi.h>
#include <ofi_mem.h>

#include "rdma/opx/fi_opx.h"
#include "rdma/opx/fi_opx_internal.h"
#include "rdma/opx/fi_opx_hfi1.h"
#include "rdma/opx/fi_opx_domain.h"
#include "ofi_prov.h"
#include "opa_service.h"

#include "rdma/opx/fi_opx_addr.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

union fi_opx_addr opx_default_addr = {
	.hfi1_rx = 0,
	.hfi1_unit = 0xff,
	.reliability_rx = 0,
	.uid = { .lid = 0xffff, .endpoint_id = 0xffff },
	.rx_index = 0,
};

static int fi_opx_init;
static int fi_opx_count;

int fi_opx_check_info(const struct fi_info *info)
{
	int ret;
	/* TODO: check caps, mode */

	if ((info->tx_attr) && ((info->tx_attr->caps | info->caps) != info->caps)) {
FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "info->tx_attr->caps = 0x%016lx, info->caps = 0x%016lx\n", info->tx_attr->caps, info->caps);
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
				"The tx_attr capabilities (0x%016lx) must be a subset of those requested of the associated endpoint (0x%016lx)",
				info->tx_attr->caps, info->caps);
		goto err;
	}

	if ((info->rx_attr) && ((info->rx_attr->caps | info->caps) != info->caps)) {
FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "info->rx_attr->caps = 0x%016lx, info->caps = 0x%016lx, (info->rx_attr->caps | info->caps) = 0x%016lx, ((info->rx_attr->caps | info->caps) ^ info->caps) = 0x%016lx\n", info->rx_attr->caps, info->caps, (info->rx_attr->caps | info->caps), ((info->rx_attr->caps | info->caps) ^ info->caps));
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
				"The rx_attr capabilities (0x%016lx) must be a subset of those requested of the associated endpoint (0x%016lx)",
				info->rx_attr->caps, info->caps);
		goto err;
	}

	switch (info->addr_format) {
	case FI_ADDR_OPX:
	case FI_FORMAT_UNSPEC:
		break;
	default:
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
				"unavailable [bad info->addr_format (%u)]",
				info->addr_format);
		goto err;
	}

	if (info->tx_attr) {
		ret = fi_opx_check_tx_attr(info->tx_attr);
		if (ret)
			return ret;
	}

	if (info->rx_attr) {
		ret = fi_opx_check_rx_attr(info->rx_attr);
		if (ret)
			return ret;
	}

	if (info->ep_attr) {
		ret = fi_opx_check_ep_attr(info->ep_attr);
		if (ret)
			return ret;
	}

	if (info->domain_attr) {
		ret = fi_opx_check_domain_attr(info->domain_attr);
		if (ret)
			return ret;
	}

	if (info->fabric_attr) {
		ret = fi_opx_check_fabric_attr(info->fabric_attr);
		if (ret)
			return ret;
	}

	return 0;

err:

	errno = FI_ENODATA;
	return -errno;
}

static int fi_opx_fillinfo(struct fi_info *fi, const char *node,
		const char* service, const struct fi_info *hints,
	        uint64_t flags, enum fi_progress progress)
{
	int ret;
	uint64_t caps;
	union fi_opx_addr *addr;
	uint32_t fmt;
	size_t len;	

	if (!fi)
		goto err;

	if (!hints && !node && !service)
		goto err;

	if (hints && (((hints->mode & FI_CONTEXT) != 0) && ((hints->mode & FI_CONTEXT2) == 0))) {
		FI_WARN(fi_opx_global.prov, FI_LOG_FABRIC,
			"FI_CONTEXT mode is not supported. Use FI_CONTEXT2 mode instead.\n");
		errno = FI_ENODATA;
		return -errno;
	}

	fi->next = NULL;
	fi->caps = FI_OPX_DEFAULT_CAPS;

	/* set the mode that we require */
	fi->mode = FI_ASYNC_IOV;
	fi->mode |= (FI_CONTEXT2);

	fi->addr_format = FI_ADDR_OPX;
	fi->src_addrlen = 0;
	fi->dest_addrlen = 0;
	fi->src_addr = NULL;
	fi->dest_addr = NULL;

	// Process the node field. Service is treated identically to node.
	if (node) {
		if (!ofi_str_toaddr(node, &fmt, (void **)&addr, &len) &&
	    	fmt == FI_ADDR_OPX) {
			if (flags & FI_SOURCE) {
				fi->src_addr = addr;
				fi->src_addrlen = sizeof(union fi_opx_addr);
				FI_INFO(fi_opx_global.prov, FI_LOG_FABRIC,
					"'%s' is taken as src_addr.\n", node);
			} else {
				fi->dest_addr = addr;
				fi->dest_addrlen = sizeof(union fi_opx_addr);
				FI_INFO(fi_opx_global.prov, FI_LOG_FABRIC,
					"'%s' is taken as dest_addr.\n", node);
			}
			node = NULL;
		} else {
			FI_WARN(fi_opx_global.prov, FI_LOG_FABRIC,
				"'%s' is not a valid OPX address.\n", node);
			goto err;
		}
	}

	if (hints) {
		if (hints->dest_addr) {
			FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
				"cannot support dest_addr lookups now\n");
			goto err;
		}

		if (hints->src_addr) {
			fi->src_addr = mem_dup(hints->src_addr, hints->src_addrlen);
			if (!fi->src_addr) {
				FI_WARN(fi_opx_global.prov, FI_LOG_FABRIC, "Failed to alloc memory.\n");
				goto err;
			}
			fi->src_addrlen = sizeof(union fi_opx_addr);
		}

	}

	if (!fi->src_addr) {
		fi->src_addr = mem_dup(&opx_default_addr, sizeof(opx_default_addr));
		if (!fi->src_addr) {
			FI_WARN(fi_opx_global.prov, FI_LOG_FABRIC, "Failed to alloc memory.\n");
			goto err;
		}
		fi->src_addrlen = sizeof(union fi_opx_addr);
	}

	if ((hints != NULL) && (hints->dest_addr != NULL) && (((node == NULL) && (service == NULL)) || (flags & FI_SOURCE))) {

		/*
		 * man/fi_getinfo.3
		 *
		 * dest_addr - destination address
		 * If specified, indicates the destination address. This field
		 * will be ignored in hints unless the node and service
		 * parameters are NULL or FI_SOURCE flag is set. If FI_SOURCE
		 * is not specified, on output a provider shall return an
		 * address the corresponds to the indicated node and/or service
		 * fields, relative to the fabric and domain. Note that any
		 * returned address is only usable locally.
		 */

		if ((flags & FI_SOURCE) == 0) {
			if ((hints->addr_format != FI_FORMAT_UNSPEC) &&
				(hints->addr_format != FI_ADDR_OPX)) {

				FI_WARN(fi_opx_global.prov, FI_LOG_FABRIC,
					"invalid addr_format hint (%d)\n", hints->addr_format);
				errno = FI_EINVAL;
				goto err;
			}
			fi->dest_addr = mem_dup(hints->dest_addr, hints->dest_addrlen);
			if (!fi->dest_addr) {
				FI_WARN(fi_opx_global.prov, FI_LOG_FABRIC, "Failed to alloc memory.\n");
				goto err;
			}
			fi->dest_addrlen = sizeof(union fi_opx_addr);
		}
	}

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

	fi->fabric_attr->name = strdup(FI_OPX_FABRIC_NAME);
	if (!fi->fabric_attr->name) {
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
				"memory allocation failed");
		goto err;
	}

	fi->fabric_attr->prov_version = FI_OPX_PROVIDER_VERSION;

	if (fi_opx_global.default_tx_attr == NULL) {
		if (fi_opx_alloc_default_tx_attr(&fi_opx_global.default_tx_attr)) {
			FI_DBG(fi_opx_global.prov, FI_LOG_DOMAIN, "alloc function could not allocate block of memory\n");
			errno = FI_ENOMEM;
			goto err;
		}
	}

	memcpy(fi->tx_attr, fi_opx_global.default_tx_attr, sizeof(*fi->tx_attr));
	if (hints && hints->tx_attr) {

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
	} else if (hints && hints->caps) {
		fi->tx_attr->caps = hints->caps;
	}

	if (fi_opx_global.default_rx_attr == NULL) {
		if (fi_opx_alloc_default_rx_attr(&fi_opx_global.default_rx_attr)) {
			FI_DBG(fi_opx_global.prov, FI_LOG_DOMAIN, "alloc function could not allocate block of memory\n");
			errno = FI_ENOMEM;
			goto err;
		}
	}
	memcpy(fi->rx_attr, fi_opx_global.default_rx_attr, sizeof(*fi->rx_attr));
	if (hints && hints->rx_attr) {

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
			hints->rx_attr->total_buffered_recv < fi_opx_global.default_rx_attr->total_buffered_recv)
				fi->rx_attr->total_buffered_recv = hints->rx_attr->total_buffered_recv;
	} else if (hints && hints->caps) {
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

	if (fi_opx_global.default_domain_attr == NULL) {
		if (fi_opx_alloc_default_domain_attr(&fi_opx_global.default_domain_attr)) {
			FI_DBG(fi_opx_global.prov, FI_LOG_DOMAIN, "alloc function could not allocate block of memory\n");
			errno = FI_ENOMEM;
			goto err;
		}
	}

	ret = fi_opx_choose_domain(caps, fi->domain_attr,
		(hints)?(hints->domain_attr):NULL, progress);
	if (ret) {
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
				"cannot find appropriate domain\n");
		goto err;
	}

	if (fi_opx_global.default_ep_attr == NULL) {
		if (fi_opx_alloc_default_ep_attr(&fi_opx_global.default_ep_attr)) {
			FI_DBG(fi_opx_global.prov, FI_LOG_DOMAIN, "alloc function could not allocate block of memory\n");
			errno = FI_ENOMEM;
			goto err;
		}
	}

	memcpy(fi->ep_attr, fi_opx_global.default_ep_attr, sizeof(*fi->ep_attr));
	if (hints && hints->ep_attr) {
		/* adjust parameters down from what requested if required */
		fi->ep_attr->type	= hints->ep_attr->type;
		if (hints->ep_attr->max_msg_size > 0 &&
			hints->ep_attr->max_msg_size <= fi_opx_global.default_ep_attr->max_msg_size)
				fi->ep_attr->max_msg_size = hints->ep_attr->max_msg_size;

		if (0 != hints->ep_attr->tx_ctx_cnt && hints->ep_attr->tx_ctx_cnt <= fi->ep_attr->tx_ctx_cnt)
			fi->ep_attr->tx_ctx_cnt = hints->ep_attr->tx_ctx_cnt;	/* TODO - check */

		if (0 != hints->ep_attr->rx_ctx_cnt && hints->ep_attr->rx_ctx_cnt <= fi->ep_attr->rx_ctx_cnt)
			fi->ep_attr->rx_ctx_cnt = hints->ep_attr->rx_ctx_cnt;	/* TODO - check */
	}

	return 0;

err:
	if(fi){
		free(fi->domain_attr->name); fi->domain_attr->name = NULL;
		free(fi->fabric_attr->name); fi->fabric_attr->name = NULL;
		free(fi->fabric_attr->prov_name); fi->fabric_attr->prov_name = NULL;
		free(fi->src_addr); fi->src_addr = NULL; fi->src_addrlen=0;
		free(fi->dest_addr); fi->dest_addr = NULL; fi->dest_addrlen=0;
	}

	if (fi_opx_global.default_ep_attr != NULL) {
		free(fi_opx_global.default_ep_attr);
		fi_opx_global.default_ep_attr = NULL;
	}

	if (fi_opx_global.default_tx_attr != NULL) {
		free(fi_opx_global.default_tx_attr);
		fi_opx_global.default_tx_attr = NULL;
	}

	if (fi_opx_global.default_rx_attr != NULL) {
		free(fi_opx_global.default_rx_attr);
		fi_opx_global.default_rx_attr = NULL;
	}

	if (fi_opx_global.default_domain_attr != NULL) {
		if (fi_opx_global.default_domain_attr->name != NULL) {
			free(fi_opx_global.default_domain_attr->name);
			fi_opx_global.default_domain_attr->name = NULL;
		}
		free(fi_opx_global.default_domain_attr);
		fi_opx_global.default_domain_attr = NULL;
	}

	return -errno;
}

struct fi_opx_global_data fi_opx_global;

static int fi_opx_getinfo_hfi(int hfi, uint32_t version, const char *node,
		const char *service, uint64_t flags,
		const struct fi_info *hints,
		struct fi_info **info, struct fi_info **info_tail)
{
	int ret, ret_auto;
	struct fi_info *fi = NULL;
	struct fi_info *fi_auto = NULL;

	*info = NULL;
	*info_tail = NULL;

	if (hints) {
		ret = fi_opx_check_info(hints);
		if (ret) {
			return ret;
		}
		if (!(fi = fi_allocinfo()) || !(fi_auto = fi_allocinfo())) {
			ret = -FI_ENOMEM;
			goto err;
		}
		ret = fi_opx_fillinfo(fi, node, service,
					hints, flags, FI_PROGRESS_MANUAL);
		ret_auto = fi_opx_fillinfo(fi_auto, node, service,
					hints, flags, FI_PROGRESS_AUTO);
		if (hints->domain_attr->data_progress != FI_PROGRESS_UNSPEC) {
			fi_opx_global.progress = hints->domain_attr->data_progress;
			if (hints->domain_attr->data_progress == FI_PROGRESS_AUTO) {
				FI_INFO(fi_opx_global.prov, FI_LOG_FABRIC, "Locking is forced in FI_PROGRESS_AUTO\n");
			}
		}
		if (ret || ret_auto) {
			ret = ret?ret:ret_auto;
			goto err;
		}

	} else if (node || service) {
		if (!(fi = fi_allocinfo()) || !(fi_auto = fi_allocinfo())) {
			ret = -FI_ENOMEM;
			goto err;
		}

		ret = fi_opx_fillinfo(fi, node, service,
					hints, flags, FI_PROGRESS_MANUAL);
		ret_auto = fi_opx_fillinfo(fi_auto, node, service,
					hints, flags, FI_PROGRESS_AUTO);
		if (ret || ret_auto) {
			ret = ret?ret:ret_auto;
			goto err;
		}

	} else {
		if (!(fi = fi_dupinfo(fi_opx_global.info)) || !(fi_opx_global.info->next != NULL && (fi_auto = fi_dupinfo(fi_opx_global.info->next)))) {
			ret = -FI_ENOMEM;
			goto err;
		}
	}

	/* Set the appropriate domain name associated with the HFI */
	char domain_name[128];

	sprintf(domain_name, "%s%d", FI_OPX_DOMAIN_NAME_PREFIX, hfi);
	free(fi->domain_attr->name);
	fi->domain_attr->name = strdup(domain_name);

	if (fi_auto) {
		free(fi_auto->domain_attr->name);
		fi_auto->domain_attr->name = strdup(fi->domain_attr->name);
	}

	fi->next = fi_auto;

	*info = fi;
	*info_tail = (fi->next) ? fi->next : fi;

	return 0;

err:
	if (fi) fi_freeinfo(fi);
	if (fi_auto) fi_freeinfo(fi_auto);
	return ret;

}

static int fi_opx_getinfo(uint32_t version, const char *node,
		const char *service, uint64_t flags,
		const struct fi_info *hints, struct fi_info **info)
{
	int ret, i;
	struct fi_info *cur, *cur_tail;
	struct fi_info *tail = NULL;

	*info = NULL;
	fi_opx_count = opx_hfi_get_hfi1_count();
	FI_LOG(fi_opx_global.prov, FI_LOG_TRACE, FI_LOG_FABRIC,
			"Detected %d hfi1(s) in the system\n", fi_opx_count);	

	if (!fi_opx_count) {
		return -FI_ENODATA;
	}

	for (i = 0; i < fi_opx_count; i++) {
		ret = fi_opx_getinfo_hfi(i, version, node, service, flags, hints, &cur, &cur_tail);
		if (ret) {
			continue;
		}
		if (!cur) {
			continue;
		}

		FI_LOG(fi_opx_global.prov, FI_LOG_TRACE, FI_LOG_FABRIC,
				"Successfully got getinfo for HFI %d\n", i);	

		if (!*info) {
			*info = cur;
		} else {
			tail->next = cur;
		}

		tail = cur_tail;
	}

	return 0;
}

static void fi_opx_fini()
{
	always_assert(fi_opx_init == 1,
		"OPX provider finalize called before initialize\n");
	fi_freeinfo(fi_opx_global.info);

	if (fi_opx_global.daos_hfi_rank_hashmap) {
		struct fi_opx_daos_hfi_rank *cur_hfi_rank = NULL;
		struct fi_opx_daos_hfi_rank *tmp_hfi_rank = NULL;

		HASH_ITER(hh, fi_opx_global.daos_hfi_rank_hashmap, cur_hfi_rank, tmp_hfi_rank) {
			if (cur_hfi_rank) {
				HASH_DEL(fi_opx_global.daos_hfi_rank_hashmap, cur_hfi_rank);
				free(cur_hfi_rank);
				cur_hfi_rank = NULL;
			}
		}
	}
}

struct fi_provider fi_opx_provider = {
	.name 		= FI_OPX_PROVIDER_NAME,
	.version	= FI_OPX_PROVIDER_VERSION,
	.fi_version 	= OFI_VERSION_LATEST,
	.getinfo	= fi_opx_getinfo,
	.fabric		= fi_opx_fabric,
	.cleanup	= fi_opx_fini
};

#pragma GCC diagnostic ignored "=Wunused-function"
/*
 * Use this dummy function to do any compile-time validation of data
 * structure sizes needed to ensure performance.
 */
static void do_static_assert_tests()
{
	// Verify that pio_state is exactly one cache-line long. */
	OPX_COMPILE_TIME_ASSERT((sizeof(union fi_opx_hfi1_pio_state) == 8),
		"fi_opx_hfi1_pio_state size error.");
	// Verify that pointers are exactly one cache-line long. */
	OPX_COMPILE_TIME_ASSERT((sizeof(union fi_opx_hfi1_pio_state*) == 8),
		"fi_opx_hfi1_pio_state pointer size error.");

	union fi_opx_hfi1_packet_payload *payload = NULL;
	OPX_COMPILE_TIME_ASSERT(sizeof(*payload) == sizeof(payload->tid_cts),
							"Expected TID rendezvous CTS payload size error");

	OPX_COMPILE_TIME_ASSERT(sizeof(*payload) == sizeof(payload->rendezvous.contiguous),
							"Contiguous rendezvous payload size error");

	OPX_COMPILE_TIME_ASSERT(sizeof(*payload) == sizeof(payload->rendezvous.noncontiguous),
							"Non-contiguous rendezvous payload size error");
}
#pragma GCC diagnostic pop

OPX_INI
{
	fi_opx_count = 1;
	fi_opx_global.progress = FI_PROGRESS_MANUAL;
	fi_opx_set_default_info(); // TODO: fold into fi_opx_set_defaults

	/* Refrain from allocating memory dynamically in this INI function. 
	   That sort of behavior will results in memory leaks for the fi_info
	   executable. */

	fi_opx_global.default_domain_attr = NULL;
	fi_opx_global.default_ep_attr = NULL;
	fi_opx_global.default_tx_attr = NULL;
	fi_opx_global.default_rx_attr = NULL;

	fi_opx_global.prov = &fi_opx_provider;
	fi_opx_global.daos_hfi_rank_hashmap = NULL;

	fi_opx_init = 1;

	fi_param_define(&fi_opx_provider, "uuid", FI_PARAM_STRING, "Globally unique ID for preventing OPX jobs from conflicting either in shared memory or over the OPX fabric. Defaults to \"%s\"",
		OPX_DEFAULT_JOB_KEY_STR);
	fi_param_define(&fi_opx_provider, "force_cpuaffinity", FI_PARAM_BOOL, "Causes the thread to bind itself to the cpu core it is running on. Defaults to \"No\"");
	fi_param_define(&fi_opx_provider, "reliability_service_usec_max", FI_PARAM_INT, "The number of microseconds between pings for un-acknowledged packets. Defaults to 500 usec.");
	fi_param_define(&fi_opx_provider, "reliability_service_pre_ack_rate", FI_PARAM_INT, "The number of packets to receive from a particular sender before preemptively acknowledging them without waiting for a ping. Valid values are powers of 2 in the range of 0-32,768, where 0 indicates no preemptive acking. Defaults to 64.");
	fi_param_define(&fi_opx_provider, "selinux", FI_PARAM_BOOL, "Set to true if you're running a security-enhanced Linux. This enables updating the Jkey used based on system settings. Defaults to \"No\"");
	fi_param_define(&fi_opx_provider, "hfi_select", FI_PARAM_STRING, "Overrides the normal algorithm used to choose which HFI a process will use. See the documentation for more information.");
	fi_param_define(&fi_opx_provider, "delivery_completion_threshold", FI_PARAM_INT, "The minimum message length in bytes to force delivery completion.  Value must be between %d and %d. Defaults to %d.", OPX_MIN_DCOMP_THRESHOLD, OPX_MAX_DCOMP_THRESHOLD, OPX_DEFAULT_DCOMP_THRESHOLD);
 	fi_param_define(&fi_opx_provider, "sdma_disable", FI_PARAM_INT, "Disables SDMA offload hardware. Default is 0");
 	fi_param_define(&fi_opx_provider, "reliability_service_nack_threshold", FI_PARAM_INT, "The number of NACKs needed to be seen before a replay is initiated. Valid values are 1-32767. Default is 1");
	fi_param_define(&fi_opx_provider, "expected_receive_enable", FI_PARAM_BOOL, "Enables expected receive rendezvous using Token ID (TID). Defaults to \"No\"");
	fi_param_define(&fi_opx_provider, "tid_reuse_enable", FI_PARAM_BOOL, "Enables the reuse cache for Token ID (TID) and pinned rendezvous receive buffers. Defaults to \"No\"");
	fi_param_define(&fi_opx_provider, "prog_affinity", FI_PARAM_STRING,
                        "When set, specify the set of CPU cores to set the progress "
                        "thread affinity to. The format is "
                        "<start>:<end>:<stride>"
                        "where each triplet <start>:<end>:<stride> defines a block "
                        "Both <start> and <end> is a core_id.");
	fi_param_define(&fi_opx_provider, "auto_progress_interval_usec", FI_PARAM_INT, "Number of usec that the progress thread waits between polling, the value of 0 is default where the interval is 1 if progress affinity is set, or 1000 otherwise.");
	// fi_param_define(&fi_opx_provider, "varname", FI_PARAM_*, "help");
	return (&fi_opx_provider);
}
