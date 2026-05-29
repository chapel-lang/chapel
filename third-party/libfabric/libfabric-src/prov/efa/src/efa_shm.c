/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

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
		EFA_WARN(FI_LOG_CQ, "Failed to convert GID to string errno: %d\n", errno);
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

/**
 * @brief Create a shm info object based on application info
 *
 * @param[in] app_info the application info
 * @param[out] shm_info the shm info
 */
void efa_shm_info_create(const struct fi_info *app_info, struct fi_info **shm_info)
{
	int ret;
	struct fi_info *shm_hints;

	if (!efa_env.enable_shm_transfer) {
		*shm_info = NULL;
		EFA_INFO(FI_LOG_CORE, "EFA will not use SHM for intranode communication because FI_EFA_ENABLE_SHM_TRANSFER=0\n");
		return;
	}

	/* App provided hints supercede environmental variables.
	 *
	 * Using the shm provider comes with some overheads, so avoid
	 * initializing the provider if the app provides a hint that it does not
	 * require node-local communication. We can still loopback over the EFA
	 * device in cases where the app violates the hint and continues
	 * communicating with node-local peers.
	 *
	 */
	if ((app_info->caps & FI_REMOTE_COMM)
	    /* but not local communication */
	    && !(app_info->caps & FI_LOCAL_COMM)) {
		*shm_info = NULL;
		EFA_INFO(FI_LOG_CORE, "EFA will not use SHM for intranode communication because application does not require local communication\n");
		return;
	}

	char *shm_provider;
	if (efa_env.use_sm2) {
		shm_provider = "sm2";
	} else {
		shm_provider = "shm";
	}

	shm_hints = fi_allocinfo();
	shm_hints->caps = app_info->caps;
	shm_hints->caps &= ~FI_REMOTE_COMM;

	/*
	 * If application requests FI_HMEM and efa supports it,
	 * make this request to shm as well.
	 */
	shm_hints->domain_attr->mr_mode = FI_MR_VIRT_ADDR;
	if (app_info->caps & FI_HMEM) {
		shm_hints->domain_attr->mr_mode |= FI_MR_HMEM;
	}

	shm_hints->domain_attr->threading = app_info->domain_attr->threading;
	shm_hints->domain_attr->av_type = FI_AV_TABLE;
	shm_hints->domain_attr->caps |= FI_LOCAL_COMM | FI_PEER | FI_AV_USER_ID;
	shm_hints->tx_attr->msg_order = FI_ORDER_SAS;
	shm_hints->rx_attr->msg_order = FI_ORDER_SAS;
	/*
	 * use the same op_flags requested by applications for shm
	 */
	shm_hints->tx_attr->op_flags  = app_info->tx_attr->op_flags;
	shm_hints->rx_attr->op_flags  = app_info->rx_attr->op_flags;
	shm_hints->fabric_attr->name = strdup(shm_provider);
	shm_hints->fabric_attr->prov_name = strdup(shm_provider);
	shm_hints->ep_attr->type = FI_EP_RDM;

	ret = fi_getinfo(FI_VERSION(1, 19), NULL, NULL,
	                 OFI_GETINFO_HIDDEN, shm_hints, shm_info);
	fi_freeinfo(shm_hints);
	if (ret) {
		EFA_WARN(FI_LOG_CORE, "Disabling EFA shared memory support; failed to get shm provider's info: %s\n",
			fi_strerror(-ret));
		*shm_info = NULL;
	} else {
		assert(!strcmp((*shm_info)->fabric_attr->name, shm_provider));
	}
}
