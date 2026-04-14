/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include <rdma/fi_errno.h>

#include "efa.h"
#include "efa_prov_info.h"
#include "efa_user_info.h"
#include "ofi_hmem.h"

struct efa_env efa_env = {
	.iface = "all",
	.tx_min_credits = 32,
	.tx_queue_size = 0,
	.enable_shm_transfer = 1,
	.use_zcpy_rx = 1,
	.zcpy_rx_seed = 0,
	.shm_av_size = 256,
	.recvwin_size = EFA_RDM_PEER_DEFAULT_REORDER_BUFFER_SIZE,
	.ooo_pool_chunk_size = 64,
	.unexp_pool_chunk_size = 1024,
	.readcopy_pool_size = 256,
	.atomrsp_pool_size = 1024,
	.cq_size = 8192,
	.max_memcpy_size = 4096,
	.tx_size = 0,
	.rx_size = 0,
	.rx_copy_unexp = 1,
	.rx_copy_ooo = 1,
	.rnr_backoff_wait_time_cap = 1000000,
	.rnr_backoff_initial_wait_time = 0, /* 0 is random wait time  */
	.efa_cq_read_size = 50,
	.shm_cq_read_size = 50,
	.efa_max_gdrcopy_msg_size = 32768,
	.efa_read_segment_size = 1073741824,
	.efa_write_segment_size = 1073741824, /* need to confirm this constant. */
	.host_id_file = "/sys/devices/virtual/dmi/id/board_asset_tag", /* Available on EC2 instances and containers */
	.use_sm2 = false,
	.huge_page_setting = EFA_ENV_HUGE_PAGE_UNSPEC,
	.use_unsolicited_write_recv = 1,
	.internal_rx_refill_threshold = 8,
	.use_data_path_direct = true,
	.implicit_av_size = 0,
};

/* @brief Read and store the FI_EFA_* environment variables.
 */
void efa_env_param_get(void)
{
	/*
	 * the maximum value for efa_env.rnr_backoff_wait_time_cap
	 * Because the backoff wait time is multiplied by 2 when
	 * RNR is encountered, its value must be < INT_MAX/2.
	 * Therefore, its cap must be < INT_MAX/2 too.
	 */
	size_t max_rnr_backoff_wait_time_cap = INT_MAX/2 - 1;
	char *abort_deprecated_env_vars[] = {"FI_EFA_MTU_SIZE", "FI_EFA_TX_IOV_LIMIT", "FI_EFA_RX_IOV_LIMIT"};
	char *info_deprecated_env_vars[] = {"FI_EFA_SET_CUDA_SYNC_MEMOPS", "FI_EFA_SHM_MAX_MEDIUM_SIZE"};
	int i;

	for (i = 0; i < sizeof(abort_deprecated_env_vars) / sizeof(abort_deprecated_env_vars[0]); i++) {
		if (getenv(abort_deprecated_env_vars[i])) {
			fprintf(stderr, "%s env variable detected! The use of this variable has been deprecated "
					"and as such execution cannot proceed.\n", abort_deprecated_env_vars[i]);
			abort();
		};
	}

	for (i = 0; i < sizeof(info_deprecated_env_vars) / sizeof(info_deprecated_env_vars[0]); i++) {
		if (getenv(info_deprecated_env_vars[i])) {
			EFA_INFO(FI_LOG_CORE, "%s env variable detected! The use of this variable "
				 "has been deprecated\n", info_deprecated_env_vars[i]);
		};
	}

	fi_param_get_int(&efa_prov, "tx_min_credits", &efa_env.tx_min_credits);
	if (efa_env.tx_min_credits <= 0) {
		fprintf(stderr,
			"FI_EFA_TX_MIN_CREDITS was set to %d, which is <= 0.\n"
			"This value will cause EFA communication to deadlock.\n"
			"Please unset the environment variable or set it to a positive number.\n"
			"Your application will now abort.\n",
			efa_env.tx_min_credits);
		abort();
	}

	fi_param_get_str(&efa_prov, "iface", &efa_env.iface);
	if (strlen(efa_env.iface) < 1) {
		fprintf(stderr,
			"FI_EFA_IFACE is empty. Specify full-qualified names separated by comma, "
			"or \"all\" to use all available devices.\n"
			"Your application will now abort.\n");
		abort();
	}

	fi_param_get_int(&efa_prov, "tx_queue_size", &efa_env.tx_queue_size);
	fi_param_get_int(&efa_prov, "enable_shm_transfer", &efa_env.enable_shm_transfer);
	fi_param_get_int(&efa_prov, "use_zcpy_rx", &efa_env.use_zcpy_rx);
	fi_param_get_int(&efa_prov, "zcpy_rx_seed", &efa_env.zcpy_rx_seed);
	fi_param_get_int(&efa_prov, "shm_av_size", &efa_env.shm_av_size);
	fi_param_get_int(&efa_prov, "recvwin_size", &efa_env.recvwin_size);
	fi_param_get_int(&efa_prov, "readcopy_pool_size", &efa_env.readcopy_pool_size);
	fi_param_get_int(&efa_prov, "cq_size", &efa_env.cq_size);
	fi_param_get_size_t(&efa_prov, "max_memcpy_size",
			    &efa_env.max_memcpy_size);
	fi_param_get_bool(&efa_prov, "mr_cache_enable",
			  &efa_mr_cache_enable);
	fi_param_get_size_t(&efa_prov, "mr_max_cached_count",
			    &efa_mr_max_cached_count);
	fi_param_get_size_t(&efa_prov, "mr_max_cached_size",
			    &efa_mr_max_cached_size);
	fi_param_get_size_t(&efa_prov, "tx_size", &efa_env.tx_size);
	fi_param_get_size_t(&efa_prov, "rx_size", &efa_env.rx_size);
	fi_param_get_size_t(&efa_prov, "internal_rx_refill_threshold", &efa_env.internal_rx_refill_threshold);
	fi_param_get_size_t(&efa_prov, "implicit_av_size", &efa_env.implicit_av_size);
	fi_param_get_bool(&efa_prov, "rx_copy_unexp",
			  &efa_env.rx_copy_unexp);
	fi_param_get_bool(&efa_prov, "rx_copy_ooo",
			  &efa_env.rx_copy_ooo);

	fi_param_get_int(&efa_prov, "max_timeout", &efa_env.rnr_backoff_wait_time_cap);
	if (efa_env.rnr_backoff_wait_time_cap > max_rnr_backoff_wait_time_cap)
		efa_env.rnr_backoff_wait_time_cap = max_rnr_backoff_wait_time_cap;

	fi_param_get_int(&efa_prov, "timeout_interval",
			 &efa_env.rnr_backoff_initial_wait_time);
	fi_param_get_size_t(&efa_prov, "efa_cq_read_size",
			 &efa_env.efa_cq_read_size);
	fi_param_get_size_t(&efa_prov, "shm_cq_read_size",
			 &efa_env.shm_cq_read_size);
	fi_param_get_size_t(&efa_prov, "inter_read_segment_size",
			    &efa_env.efa_read_segment_size);
	fi_param_get_size_t(&efa_prov, "inter_max_gdrcopy_message_size",
			    &efa_env.efa_max_gdrcopy_msg_size);
	fi_param_get_bool(&efa_prov, "use_sm2", &efa_env.use_sm2);
	fi_param_get_bool(&efa_prov, "use_unsolicited_write_recv", &efa_env.use_unsolicited_write_recv);

	int use_huge_page;
	if (fi_param_get_bool(&efa_prov, "use_huge_page", &use_huge_page) ==0) {
		efa_env.huge_page_setting = use_huge_page ? EFA_ENV_HUGE_PAGE_ENABLED : EFA_ENV_HUGE_PAGE_DISABLED;
	}
	fi_param_get_bool(&efa_prov, "use_data_path_direct", &efa_env.use_data_path_direct);

	efa_fork_support_request_initialize();
}

void efa_env_define()
{
	fi_param_define(&efa_prov, "use_device_rdma", FI_PARAM_BOOL,
			"Specifies whether to use device's RDMA functionality"
			" for one-sided and two-sided transfers if supported "
			"by the EFA device on the instance.");
	fi_param_define(&efa_prov, "iface", FI_PARAM_STRING,
			"A comma delimited list of case-sensitive names to restrict eligible EFA NICs (Default: all).");
	fi_param_define(&efa_prov, "tx_min_credits", FI_PARAM_INT,
			"Defines the minimum number of credits a sender requests from a receiver (Default: 32).");
	fi_param_define(&efa_prov, "tx_queue_size", FI_PARAM_INT,
			"Defines the maximum number of unacknowledged sends with the NIC.");
	fi_param_define(&efa_prov, "enable_shm_transfer", FI_PARAM_INT,
			"Enable using SHM provider to perform TX/RX operations between processes on the same system. (Default: 1)");
	fi_param_define(&efa_prov, "use_zcpy_rx", FI_PARAM_INT,
			"Enables the use of application's receive buffers in place of bounce-buffers when feasible. (Default: 1)");
	fi_param_define(&efa_prov, "zcpy_rx_seed", FI_PARAM_INT,
			"Defines the number of bounce-buffers the provider will prepost during EP initialization.  (Default: 0)");
	fi_param_define(&efa_prov, "shm_av_size", FI_PARAM_INT,
			"Defines the maximum number of entries in SHM provider's address vector (Default 128).");
	fi_param_define(&efa_prov, "recvwin_size", FI_PARAM_INT,
			"Defines the size of sliding receive window. (Default: 16384)");
	fi_param_define(&efa_prov, "readcopy_pool_size", FI_PARAM_INT,
			"Defines the size of readcopy packet pool size. (Default: 256)");
	fi_param_define(&efa_prov, "cq_size", FI_PARAM_INT,
			"Define the size of completion queue. (Default: 8192)");
	fi_param_define(&efa_prov, "mr_cache_enable", FI_PARAM_BOOL,
			"Enables using the mr cache and in-line registration instead of a bounce buffer for iov's larger than max_memcpy_size. Defaults to true. When disabled, only uses a bounce buffer.");
	fi_param_define(&efa_prov, "mr_max_cached_count", FI_PARAM_SIZE_T,
			"Sets the maximum number of memory registrations that can be cached at any time.");
	fi_param_define(&efa_prov, "mr_max_cached_size", FI_PARAM_SIZE_T,
			"Sets the maximum amount of memory that cached memory registrations can hold onto at any time.");
	fi_param_define(&efa_prov, "max_memcpy_size", FI_PARAM_SIZE_T,
			"Threshold size switch between using memory copy into a pre-registered bounce buffer and memory registration on the user buffer. (Default: 4096)");
	fi_param_define(&efa_prov, "tx_size", FI_PARAM_SIZE_T,
			"Set the maximum number of transmit operations before the provider returns -FI_EAGAIN. For only the RDM endpoint, this parameter will cause transmit operations to be queued when this value is set higher than the default and the transmit queue is full.");
	fi_param_define(&efa_prov, "rx_size", FI_PARAM_SIZE_T,
			"Set the maximum number of receive operations before the provider returns -FI_EAGAIN.");
	fi_param_define(&efa_prov, "rx_copy_unexp", FI_PARAM_BOOL,
			"Enables the use of a separate pool of bounce-buffers to copy unexpected messages out of the pre-posted receive buffers. (Default: 1)");
	fi_param_define(&efa_prov, "rx_copy_ooo", FI_PARAM_BOOL,
			"Enables the use of a separate pool of bounce-buffers to copy out-of-order RTM packets out of the pre-posted receive buffers. (Default: 1)");
	fi_param_define(&efa_prov, "max_timeout", FI_PARAM_INT,
			"Set the maximum timeout (us) for backoff to a peer after a receiver not ready error. (Default: 1000000)");
	fi_param_define(&efa_prov, "timeout_interval", FI_PARAM_INT,
			"Set the time interval (us) for the base timeout to use for exponential backoff to a peer after a receiver not ready error. (Default: 0 [random])");
	fi_param_define(&efa_prov, "efa_cq_read_size", FI_PARAM_SIZE_T,
			"Set the number of EFA completion entries to read for one loop for one iteration of the progress engine. (Default: 50)");
	fi_param_define(&efa_prov, "shm_cq_read_size", FI_PARAM_SIZE_T,
			"Set the number of SHM completion entries to read for one loop for one iteration of the progress engine. (Default: 50)");
	fi_param_define(&efa_prov, "inter_max_medium_message_size", FI_PARAM_INT,
			"The maximum message size for inter EFA medium message protocol (Default 65536).");
	fi_param_define(&efa_prov, "inter_min_read_message_size", FI_PARAM_INT,
			"The minimum message size in bytes for inter EFA read message protocol. If instance support RDMA read, messages whose size is larger than this value will be sent by read message protocol (Default 1048576).");
	fi_param_define(&efa_prov, "inter_max_gdrcopy_message_size", FI_PARAM_INT,
			"The maximum message size to use gdrcopy. If instance support gdrcopy, messages whose size is smaller than this value will be sent by eager/longcts protocol (Default 32768).");
	fi_param_define(&efa_prov, "inter_min_read_write_size", FI_PARAM_INT,
			"The mimimum message size for inter EFA write to use read write protocol. If firmware support RDMA read, and FI_EFA_USE_DEVICE_RDMA is 1, write requests whose size is larger than this value will use the read write protocol (Default 65536). If the efa device supports RDMA write, device RDMA write will always be used.");
	fi_param_define(&efa_prov, "inter_read_segment_size", FI_PARAM_INT,
			"Calls to RDMA read is segmented using this value.");
	fi_param_define(&efa_prov, "fork_safe", FI_PARAM_BOOL,
			"Enables fork support and disables internal usage of huge pages. Has no effect on kernels which set copy-on-fork for registered pages, generally 5.13 and later. (Default: false)");
	fi_param_define(&efa_prov, "runt_size", FI_PARAM_INT,
			"The maximum number of bytes that will be eagerly sent by inflight messages uses runting read message protocol (Default 307200).");
	fi_param_define(&efa_prov, "use_sm2", FI_PARAM_BOOL,
			"Use the experimental shared memory provider SM2 for intra node communication.");
	fi_param_define(&efa_prov, "use_huge_page", FI_PARAM_BOOL,
			"Whether EFA provider can use huge page memory for internal buffer. "
			"Using huge page memory has a small performance advantage, but can "
			"cause system to run out of huge page memory. By default, EFA provider "
			"will use huge page unless FI_EFA_FORK_SAFE is set to 1/on/true.");
	fi_param_define(&efa_prov, "use_unsolicited_write_recv", FI_PARAM_BOOL,
			"Use device's unsolicited write recv functionality when it's available. (Default: true)");
	fi_param_define(&efa_prov, "internal_rx_refill_threshold", FI_PARAM_SIZE_T,
			"The threshold that EFA provider will refill the internal rx pkt pool. (Default: %zu)", efa_env.internal_rx_refill_threshold);
	fi_param_define(
		&efa_prov, "use_data_path_direct", FI_PARAM_BOOL,
		"Use the direct data path implementation that bypasses rdma-core on data path, including"
		"the CQ polling and TX/RX submissions, when it's available. Setting this variable as 0"
		"will disable this feature (Default: %d)",
		efa_env.use_data_path_direct);
	fi_param_define(&efa_prov, "implicit_av_size", FI_PARAM_SIZE_T,
			"The maximum size of the implicit AV used to store AV "
			"entries of peers that were not explicitly inserted "
			"into the AV by the application. Setting this variable "
			"to a positive value will enforce the the maximum "
			"size. Setting this value to 0 will allow unbounded "
			"growth of the implicit AV. (Default: 0)",
			efa_env.implicit_av_size);
}


/**
 * @brief Initialize the variables in efa_env.
 */
void efa_env_initialize()
{
	efa_env_define();
	efa_env_param_get();
}
