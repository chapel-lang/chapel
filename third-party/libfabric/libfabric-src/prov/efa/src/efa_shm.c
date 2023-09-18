/*
 * Copyright (c) 2022 Amazon.com, Inc. or its affiliates. All rights reserved.
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
#include <assert.h>
#include "efa.h"
#include "efa_shm.h"


/**
 * @brief construct an unique shm endpoint name (smr_name) from EFA raw address
 *
 * Note even though all shm endpoints are on same instance. But because
 * one instance can have multiple EFA device, it is still necessary
 * to include GID on the name.
 *
 * a smr name consist of the following 4 parts:
 *
 *    GID:   ipv6 address from inet_ntop
 *    QPN:   %04x format
 *    QKEY:  %08x format
 *    UID:   %04x format
 *
 * each part is connected via an underscore.
 *
 * The following is an example:
 *
 *    fe80::4a5:28ff:fe98:e500_0001_12918366_03e8
 *
 * @param[in]		ptr		pointer to raw address (struct efa_ep_addr)
 * @param[out]		smr_name	an unique name for shm ep
 * @param[in,out]	smr_name_len    As input, specify size of the "smr_name" buffer.
 *					As output, specify number of bytes written to the buffer.
 *
 * @return	0 on success.
 * 		negative error code on failure.
 */
int efa_shm_ep_name_construct(char *smr_name, size_t *smr_name_len, struct efa_ep_addr *raw_addr)
{
	char gidstr[INET6_ADDRSTRLEN] = { 0 };
	int ret;

	if (!inet_ntop(AF_INET6, raw_addr->raw, gidstr, INET6_ADDRSTRLEN)) {
		FI_WARN(&rxr_prov, FI_LOG_CQ, "Failed to convert GID to string errno: %d\n", errno);
		return -errno;
	}

	ret = snprintf(smr_name, *smr_name_len, "%s_%04x_%08x_%04x",
		       gidstr, raw_addr->qpn, raw_addr->qkey, getuid());
	if (ret < 0)
		return ret;

	if (ret == 0 || ret >= *smr_name_len)
		return -FI_EINVAL;

	/* plus 1 here for the ending '\0' character, which was not
	 * included in ret of snprintf
	 */
	*smr_name_len = ret + 1;
	return FI_SUCCESS;
}

struct fi_info *g_shm_info;

/**
 * @brief initliaze the global variable g_shm_info
 *
 * g_shm_info is used to create shm resources (fabric,domain,endpoint,cq)
 *
 */
void efa_shm_info_initialize(const struct fi_info *app_hints)
{
	int ret;
	struct fi_info *shm_hints;

	shm_hints = fi_allocinfo();
	shm_hints->caps = FI_MSG | FI_TAGGED | FI_RECV | FI_SEND | FI_READ
			   | FI_WRITE | FI_REMOTE_READ | FI_REMOTE_WRITE
			   | FI_MULTI_RECV | FI_RMA;
	shm_hints->domain_attr->av_type = FI_AV_TABLE;
	shm_hints->domain_attr->mr_mode = FI_MR_VIRT_ADDR;
	shm_hints->domain_attr->caps |= FI_LOCAL_COMM;
	shm_hints->tx_attr->msg_order = FI_ORDER_SAS;
	shm_hints->rx_attr->msg_order = FI_ORDER_SAS;
	shm_hints->fabric_attr->name = strdup("shm");
	shm_hints->fabric_attr->prov_name = strdup("shm");
	shm_hints->ep_attr->type = FI_EP_RDM;

	/*
	 * We validate whether FI_HMEM is supported before this function is
	 * called, so it's safe to check for this via the app hints directly.
	 * We should combine this and the earlier FI_HMEM validation when we
	 * clean up the getinfo path. That's not possible at the moment as we
	 * only have one SHM info for the entire provider which isn't right.
	 */
	if (app_hints && (app_hints->caps & FI_HMEM)) {
		shm_hints->caps |= FI_HMEM;
		shm_hints->domain_attr->mr_mode |= FI_MR_HMEM;
	}

	ret = fi_getinfo(FI_VERSION(1, 8), NULL, NULL,
	                 OFI_GETINFO_HIDDEN, shm_hints, &g_shm_info);
	fi_freeinfo(shm_hints);
	if (ret) {
		FI_WARN(&rxr_prov, FI_LOG_CORE, "Disabling EFA shared memory support; failed to get shm provider's info: %s\n",
			fi_strerror(-ret));
		rxr_env.enable_shm_transfer = 0;
		ret = 0;
	} else {
		assert(!strcmp(g_shm_info->fabric_attr->name, "shm"));
	}
}

void efa_shm_info_finalize()
{
	if (g_shm_info) {
		fi_freeinfo(g_shm_info);
		g_shm_info = NULL;
	}
}
