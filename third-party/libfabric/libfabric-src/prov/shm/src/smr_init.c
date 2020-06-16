/*
 * Copyright (c) 2015-2017 Intel Corporation. All rights reserved.
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

#include <rdma/fi_errno.h>

#include <ofi_prov.h>
#include "smr.h"
#include "smr_signal.h"

extern struct sigaction *old_action;
struct smr_env smr_env = {
	.sar_threshold = SIZE_MAX,
};

static void smr_init_env(void)
{
	fi_param_get_size_t(&smr_prov, "sar_threshold", &smr_env.sar_threshold);
	fi_param_get_size_t(&smr_prov, "tx_size", &smr_info.tx_attr->size);
	fi_param_get_size_t(&smr_prov, "rx_size", &smr_info.rx_attr->size);
}

static void smr_resolve_addr(const char *node, const char *service,
			     char **addr, size_t *addrlen)
{
	char temp_name[NAME_MAX];

	if (service) {
		if (node)
			snprintf(temp_name, NAME_MAX - 1, "%s%s:%s",
				 SMR_PREFIX_NS, node, service);
		else
			snprintf(temp_name, NAME_MAX - 1, "%s%s",
				 SMR_PREFIX_NS, service);
	} else {
		if (node)
			snprintf(temp_name, NAME_MAX - 1, "%s%s",
				 SMR_PREFIX, node);
		else
			snprintf(temp_name, NAME_MAX - 1, "%s%d",
				 SMR_PREFIX, getpid());
	}

	*addr = strdup(temp_name);
	*addrlen = strlen(*addr) + 1;
	(*addr)[*addrlen - 1]  = '\0';
}

/*
 * The smr_shm_space_check is to check if there's enough shm space we
 * need under /dev/shm.
 * Here we use #core instead of SMR_MAX_PEERS, as it is the most likely
 * value and has less possibility of failing fi_getinfo calls that are
 * currently passing, and breaking currently working app
 */
static int smr_shm_space_check(size_t tx_count, size_t rx_count)
{
	struct statvfs stat;
	char shm_fs[] = "/dev/shm";
	uint64_t available_size, shm_size_needed;
	int num_of_core, err;

	num_of_core = ofi_sysconf(_SC_NPROCESSORS_ONLN);
	if (num_of_core < 0) {
		FI_WARN(&smr_prov, FI_LOG_CORE,
			"Get number of processor failed (%s)\n",
			strerror(errno));
		return -errno;
	}
	shm_size_needed = num_of_core *
			  smr_calculate_size_offsets(tx_count, rx_count,
						     NULL, NULL, NULL,
						     NULL, NULL, NULL);
	err = statvfs(shm_fs, &stat);
	if (err) {
		FI_WARN(&smr_prov, FI_LOG_CORE,
			"Get filesystem %s statistics failed (%s)\n",
			shm_fs, strerror(errno));
	} else {
		available_size = stat.f_bsize * stat.f_bavail;
		if (available_size < shm_size_needed) {
			FI_WARN(&smr_prov, FI_LOG_CORE,
				"Not enough available space in %s.\n", shm_fs);
			return -FI_ENOSPC;
		}
	}
	return 0;
}

static int smr_getinfo(uint32_t version, const char *node, const char *service,
		       uint64_t flags, const struct fi_info *hints,
		       struct fi_info **info)
{
	struct fi_info *cur;
	uint64_t mr_mode, msg_order;
	int fast_rma;
	int ret;

	mr_mode = hints && hints->domain_attr ? hints->domain_attr->mr_mode :
						FI_MR_VIRT_ADDR;
	msg_order = hints && hints->tx_attr ? hints->tx_attr->msg_order : 0;
	fast_rma = smr_fast_rma_enabled(mr_mode, msg_order);

	ret = util_getinfo(&smr_util_prov, version, node, service, flags,
			   hints, info);
	if (ret)
		return ret;

	ret = smr_shm_space_check((*info)->tx_attr->size, (*info)->rx_attr->size);
	if (ret) {
		fi_freeinfo(*info);
		return ret;
	}

	for (cur = *info; cur; cur = cur->next) {
		if (!(flags & FI_SOURCE) && !cur->dest_addr)
			smr_resolve_addr(node, service, (char **) &cur->dest_addr,
					 &cur->dest_addrlen);

		if (!cur->src_addr) {
			if (flags & FI_SOURCE)
				smr_resolve_addr(node, service, (char **) &cur->src_addr,
						 &cur->src_addrlen);
			else
				smr_resolve_addr(NULL, NULL, (char **) &cur->src_addr,
						 &cur->src_addrlen);
		}
		if (fast_rma) {
			cur->domain_attr->mr_mode = FI_MR_VIRT_ADDR;
			cur->tx_attr->msg_order = FI_ORDER_SAS;
			cur->ep_attr->max_order_raw_size = 0;
			cur->ep_attr->max_order_waw_size = 0;
			cur->ep_attr->max_order_war_size = 0;
		}
	}
	return 0;
}

static void smr_fini(void)
{
	smr_cleanup();
	free(old_action);
}

struct fi_provider smr_prov = {
	.name = "shm",
	.version = OFI_VERSION_DEF_PROV,
	.fi_version = OFI_VERSION_LATEST,
	.getinfo = smr_getinfo,
	.fabric = smr_fabric,
	.cleanup = smr_fini
};

struct util_prov smr_util_prov = {
	.prov = &smr_prov,
	.info = &smr_info,
	.flags = 0
};

SHM_INI
{
	fi_param_define(&smr_prov, "sar_threshold", FI_PARAM_SIZE_T,
			"Max size to use for alternate SAR protocol if CMA \
			 is not available before switching to mmap protocol \
			 Default: SIZE_MAX (18446744073709551615)");
	fi_param_define(&smr_prov, "tx_size", FI_PARAM_SIZE_T,
			"Max number of outstanding tx operations \
			 Default: 1024");
	fi_param_define(&smr_prov, "rx_size", FI_PARAM_SIZE_T,
			"Max number of outstanding rx operations \
			 Default: 1024");

	smr_init_env();

	old_action = calloc(SIGRTMIN, sizeof(*old_action));
	if (!old_action)
		return NULL;
	/* Signal handlers to cleanup tmpfs files on an unclean shutdown */
	assert(SIGBUS < SIGRTMIN && SIGSEGV < SIGRTMIN
	       && SIGTERM < SIGRTMIN && SIGINT < SIGRTMIN);
	smr_reg_sig_hander(SIGBUS);
	smr_reg_sig_hander(SIGSEGV);
	smr_reg_sig_hander(SIGTERM);
	smr_reg_sig_hander(SIGINT);

	return &smr_prov;
}
