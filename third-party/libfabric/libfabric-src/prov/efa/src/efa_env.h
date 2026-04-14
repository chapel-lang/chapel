/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef _EFA_ENV_H
#define _EFA_ENV_H

#include "efa_prov.h"

enum efa_env_huge_page_setting
{
	EFA_ENV_HUGE_PAGE_UNSPEC, /**< user did not set FI_EFA_USE_HUGE_PAGE, provider will decide whether to use huge page*/
	EFA_ENV_HUGE_PAGE_ENABLED, /**< user explicitly set FI_EFA_USE_HUGE_PAGE to 1/true/on */
	EFA_ENV_HUGE_PAGE_DISABLED, /**< user explicitly set FI_EFA_USE_HUGE_PAGE to 0/false/off */
};

struct efa_env {
	char *iface;
	int tx_min_credits;
	int tx_queue_size;
	int use_zcpy_rx;
	int zcpy_rx_seed;
	int enable_shm_transfer;
	int shm_av_size;
	int recvwin_size;
	int ooo_pool_chunk_size;
	int unexp_pool_chunk_size;
	int readcopy_pool_size;
	int atomrsp_pool_size;
	int cq_size;
	size_t max_memcpy_size;
	size_t mtu_size;
	size_t tx_size;
	size_t rx_size;
	size_t tx_iov_limit;
	size_t rx_iov_limit;
	int rx_copy_unexp;
	int rx_copy_ooo;
	int rnr_backoff_wait_time_cap; /* unit is us */
	int rnr_backoff_initial_wait_time; /* unit is us */
	size_t efa_cq_read_size;
	size_t shm_cq_read_size;
	size_t efa_max_gdrcopy_msg_size;
	size_t efa_read_segment_size;
	size_t efa_write_segment_size;
	/**
	 * The absolute path to a file that contains an EC2 instance id-like string.
	 * If host_id_file is provided, the program will attempt to read the
	 * 16 hex characters starting at the 4th character and convert it to an 8-byte
	 * integer as the host id.
	 *
	 * 	Accept example:
	 *		i-0f7c826e5f3bd8685	->	0xf7c826e5f3bd8685 is a valid hex string
	 *
	 * 	Illegal examples,
	 *		quickfoxbrownfence	->	Not a hex string
	 *		789e1cab			->	String is too short
	 *
	 * Host id parsing is best-effort. If the file does not exist, or the file
	 * is malformatted, the program should proceed with a default host id, e.g. 0.
	 */
	char *host_id_file;
	int use_sm2;
	enum efa_env_huge_page_setting huge_page_setting;
	int use_unsolicited_write_recv;
	/**
	 * The threshold that EFA provider will refill the internal rx pkt pool.
	 * When the number of internal rx pkts to post is lower than this threshold,
	 * the refill will be skipped.
	 */
	size_t internal_rx_refill_threshold;
	int use_data_path_direct;
	/**
	 * The maximum size of the implicit AV used to store AV entries of peers
	 * that were not explicitly inserted into the AV by the application.
	 * Value of 0 means there is no limit on the size.
	 */
	size_t implicit_av_size;
};

extern struct efa_env efa_env;

/**
 * @brief Return true if the environment variable FI_EFA_USE_DEVICE_RDMA is present
 *
 * @return true  - FI_EFA_USE_DEVICE_RDMA is defined in the environment
 * @return false - otherwise.
 * @related efa_rdm_get_use_device_rdma
 */
static inline bool efa_env_has_use_device_rdma() {
	int ret, param_val;
	ret = fi_param_get_bool(&efa_prov, "use_device_rdma", &param_val);
	return (ret != -FI_ENODATA);
}

/**
 * @brief Return true if the NIC is allowed by FI_EFA_IFACE variable
 *
 * @param[in]	name	NIC name string
 * @return	true if the NIC is allowed, otherwise false
 */
static inline bool efa_env_allows_nic(const char *name) {
	char *match = efa_env.iface;
	char *end = efa_env.iface + strlen(efa_env.iface);

	if (name == NULL || strlen(name) < 1)
		return false;

	if (!strncmp("all", efa_env.iface, 3))
		return true;

	while (match < end) {
		match = strstr(match, name);
		if (!match)
			return false;

		if ((match > efa_env.iface && *(match - 1) != ',') ||
		    ((match + strlen(name)) < end && *(match + strlen(name)) != ',')) {
			/* Skip partial match */
			match += strlen(name);
			continue;
		}
		return true;
	}

	return false;
}

void efa_env_initialize();

#endif
