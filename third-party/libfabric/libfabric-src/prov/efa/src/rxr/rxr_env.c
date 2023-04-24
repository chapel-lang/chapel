/*
 * Copyright (c) 2019 Amazon.com, Inc. or its affiliates.
 * All rights reserved.
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

#include "efa.h"
#include "efa_prov_info.h"
#include "efa_user_info.h"
#include "ofi_hmem.h"

struct rxr_env rxr_env = {
	.tx_min_credits = RXR_DEF_MIN_TX_CREDITS,
	.tx_queue_size = 0,
	.enable_shm_transfer = 1,
	.use_device_rdma = 0,
	.use_zcpy_rx = 1,
	.set_cuda_sync_memops = 1,
	.zcpy_rx_seed = 0,
	.shm_av_size = 128,
	.shm_max_medium_size = 4096,
	.recvwin_size = RXR_RECVWIN_SIZE,
	.ooo_pool_chunk_size = 64,
	.unexp_pool_chunk_size = 1024,
	.readcopy_pool_size = 256,
	.atomrsp_pool_size = 1024,
	.cq_size = RXR_DEF_CQ_SIZE,
	.max_memcpy_size = 4096,
	.mtu_size = 0,
	.tx_size = 0,
	.rx_size = 0,
	.tx_iov_limit = 0,
	.rx_iov_limit = 0,
	.rx_copy_unexp = 1,
	.rx_copy_ooo = 1,
	.rnr_backoff_wait_time_cap = RXR_DEFAULT_RNR_BACKOFF_WAIT_TIME_CAP,
	.rnr_backoff_initial_wait_time = 0, /* 0 is random wait time  */
	.efa_cq_read_size = 50,
	.shm_cq_read_size = 50,
	.efa_max_gdrcopy_msg_size = 32768,
	.efa_read_segment_size = 1073741824,
	.rnr_retry = 3, /* Setting this value to EFA_RNR_INFINITE_RETRY makes the firmware retry indefinitey */
};

/* @brief Read and store the FI_EFA_* environment variables.
 */
void rxr_env_param_get(void)
{
	int ret;

	if (getenv("FI_EFA_SHM_MAX_MEDIUM_SIZE")) {
		fprintf(stderr,
			"FI_EFA_SHM_MAX_MEDIUM_SIZE env variable detected! The use of this variable has been deprecated and as such execution cannot proceed.\n");
		abort();
	};

	fi_param_get_int(&rxr_prov, "tx_min_credits", &rxr_env.tx_min_credits);
	if (rxr_env.tx_min_credits <= 0) {
		FI_WARN(&rxr_prov, FI_LOG_EP_DATA,
			"FI_EFA_TX_MIN_CREDITS was set to %d, which is <= 0."
			"This value will cause EFA communication to deadlock."
			"To avoid that, the variable was reset to %d\n",
			rxr_env.tx_min_credits, RXR_DEF_MIN_TX_CREDITS);
		rxr_env.tx_min_credits = RXR_DEF_MIN_TX_CREDITS;
	}

	fi_param_get_int(&rxr_prov, "tx_queue_size", &rxr_env.tx_queue_size);
	fi_param_get_int(&rxr_prov, "enable_shm_transfer", &rxr_env.enable_shm_transfer);
	ret = fi_param_get_int(&rxr_prov, "use_device_rdma", &rxr_env.use_device_rdma);
	if (ret != -FI_ENODATA && rxr_env.use_device_rdma &&
	    !(g_device_list[0].efa_attr.device_caps & EFADV_DEVICE_ATTR_CAPS_RDMA_READ)) {
		fprintf(stderr, "FI_EFA_USE_DEVICE_RDMA=1 was set by users, "
				"but EFA device has no rdma-read capability.\n");
		abort();
	}
	fi_param_get_int(&rxr_prov, "use_zcpy_rx", &rxr_env.use_zcpy_rx);
	fi_param_get_int(&rxr_prov, "set_cuda_sync_memops", &rxr_env.set_cuda_sync_memops);
	fi_param_get_int(&rxr_prov, "zcpy_rx_seed", &rxr_env.zcpy_rx_seed);
	fi_param_get_int(&rxr_prov, "shm_av_size", &rxr_env.shm_av_size);
	fi_param_get_int(&rxr_prov, "recvwin_size", &rxr_env.recvwin_size);
	fi_param_get_int(&rxr_prov, "readcopy_pool_size", &rxr_env.readcopy_pool_size);
	fi_param_get_int(&rxr_prov, "cq_size", &rxr_env.cq_size);
	fi_param_get_size_t(&rxr_prov, "max_memcpy_size",
			    &rxr_env.max_memcpy_size);
	fi_param_get_bool(&rxr_prov, "mr_cache_enable",
			  &efa_mr_cache_enable);
	fi_param_get_size_t(&rxr_prov, "mr_max_cached_count",
			    &efa_mr_max_cached_count);
	fi_param_get_size_t(&rxr_prov, "mr_max_cached_size",
			    &efa_mr_max_cached_size);
	fi_param_get_size_t(&rxr_prov, "mtu_size",
			    &rxr_env.mtu_size);
	fi_param_get_size_t(&rxr_prov, "tx_size", &rxr_env.tx_size);
	fi_param_get_size_t(&rxr_prov, "rx_size", &rxr_env.rx_size);
	fi_param_get_size_t(&rxr_prov, "tx_iov_limit", &rxr_env.tx_iov_limit);
	fi_param_get_size_t(&rxr_prov, "rx_iov_limit", &rxr_env.rx_iov_limit);
	fi_param_get_bool(&rxr_prov, "rx_copy_unexp",
			  &rxr_env.rx_copy_unexp);
	fi_param_get_bool(&rxr_prov, "rx_copy_ooo",
			  &rxr_env.rx_copy_ooo);

	fi_param_get_int(&rxr_prov, "max_timeout", &rxr_env.rnr_backoff_wait_time_cap);
	if (rxr_env.rnr_backoff_wait_time_cap > RXR_MAX_RNR_BACKOFF_WAIT_TIME_CAP)
		rxr_env.rnr_backoff_wait_time_cap = RXR_MAX_RNR_BACKOFF_WAIT_TIME_CAP;

	fi_param_get_int(&rxr_prov, "timeout_interval",
			 &rxr_env.rnr_backoff_initial_wait_time);
	fi_param_get_size_t(&rxr_prov, "efa_cq_read_size",
			 &rxr_env.efa_cq_read_size);
	fi_param_get_size_t(&rxr_prov, "shm_cq_read_size",
			 &rxr_env.shm_cq_read_size);
	fi_param_get_size_t(&rxr_prov, "inter_read_segment_size",
			    &rxr_env.efa_read_segment_size);
	fi_param_get_size_t(&rxr_prov, "inter_max_gdrcopy_message_size",
			    &rxr_env.efa_max_gdrcopy_msg_size);
	efa_fork_support_request_initialize();
}

void rxr_env_define()
{
	char *str = "";

	fi_param_define(&rxr_prov, "tx_min_credits", FI_PARAM_INT,
			"Defines the minimum number of credits a sender requests from a receiver (Default: 32).");
	fi_param_define(&rxr_prov, "tx_queue_size", FI_PARAM_INT,
			"Defines the maximum number of unacknowledged sends with the NIC.");
	fi_param_define(&rxr_prov, "enable_shm_transfer", FI_PARAM_INT,
			"Enable using SHM provider to perform TX operations between processes on the same system. (Default: 1)");
	if (!(g_device_list[0].efa_attr.device_caps & EFADV_DEVICE_ATTR_CAPS_RDMA_READ))
		str = "EFA device on your system does not support RDMA, so this variable cannot be set to 1";
	fi_param_define(&rxr_prov, "use_device_rdma", FI_PARAM_INT,
			"whether to use device's RDMA functionality for one-sided and two-sided transfer. (Default: %d). %s", rxr_env.use_device_rdma, str);
	fi_param_define(&rxr_prov, "use_zcpy_rx", FI_PARAM_INT,
			"Enables the use of application's receive buffers in place of bounce-buffers when feasible. (Default: 1)");
	fi_param_define(&rxr_prov, "set_cuda_sync_memops", FI_PARAM_INT,
			"Set CU_POINTER_ATTRIBUTE_SYNC_MEMOPS for cuda ptr. (Default: 1)");
	fi_param_define(&rxr_prov, "zcpy_rx_seed", FI_PARAM_INT,
			"Defines the number of bounce-buffers the provider will prepost during EP initialization.  (Default: 0)");
	fi_param_define(&rxr_prov, "shm_av_size", FI_PARAM_INT,
			"Defines the maximum number of entries in SHM provider's address vector (Default 128).");
	fi_param_define(&rxr_prov, "recvwin_size", FI_PARAM_INT,
			"Defines the size of sliding receive window. (Default: 16384)");
	fi_param_define(&rxr_prov, "readcopy_pool_size", FI_PARAM_INT,
			"Defines the size of readcopy packet pool size. (Default: 256)");
	fi_param_define(&rxr_prov, "cq_size", FI_PARAM_INT,
			"Define the size of completion queue. (Default: 8192)");
	fi_param_define(&rxr_prov, "mr_cache_enable", FI_PARAM_BOOL,
			"Enables using the mr cache and in-line registration instead of a bounce buffer for iov's larger than max_memcpy_size. Defaults to true. When disabled, only uses a bounce buffer.");
	fi_param_define(&rxr_prov, "mr_max_cached_count", FI_PARAM_SIZE_T,
			"Sets the maximum number of memory registrations that can be cached at any time.");
	fi_param_define(&rxr_prov, "mr_max_cached_size", FI_PARAM_SIZE_T,
			"Sets the maximum amount of memory that cached memory registrations can hold onto at any time.");
	fi_param_define(&rxr_prov, "max_memcpy_size", FI_PARAM_SIZE_T,
			"Threshold size switch between using memory copy into a pre-registered bounce buffer and memory registration on the user buffer. (Default: 4096)");
	fi_param_define(&rxr_prov, "mtu_size", FI_PARAM_SIZE_T,
			"Override the MTU size of the device.");
	fi_param_define(&rxr_prov, "tx_size", FI_PARAM_SIZE_T,
			"Set the maximum number of transmit operations before the provider returns -FI_EAGAIN. For only the RDM endpoint, this parameter will cause transmit operations to be queued when this value is set higher than the default and the transmit queue is full.");
	fi_param_define(&rxr_prov, "rx_size", FI_PARAM_SIZE_T,
			"Set the maximum number of receive operations before the provider returns -FI_EAGAIN.");
	fi_param_define(&rxr_prov, "tx_iov_limit", FI_PARAM_SIZE_T,
			"Maximum transmit iov_limit.");
	fi_param_define(&rxr_prov, "rx_iov_limit", FI_PARAM_SIZE_T,
			"Maximum receive iov_limit.");
	fi_param_define(&rxr_prov, "rx_copy_unexp", FI_PARAM_BOOL,
			"Enables the use of a separate pool of bounce-buffers to copy unexpected messages out of the pre-posted receive buffers. (Default: 1)");
	fi_param_define(&rxr_prov, "rx_copy_ooo", FI_PARAM_BOOL,
			"Enables the use of a separate pool of bounce-buffers to copy out-of-order RTM packets out of the pre-posted receive buffers. (Default: 1)");
	fi_param_define(&rxr_prov, "max_timeout", FI_PARAM_INT,
			"Set the maximum timeout (us) for backoff to a peer after a receiver not ready error. (Default: 1000000)");
	fi_param_define(&rxr_prov, "timeout_interval", FI_PARAM_INT,
			"Set the time interval (us) for the base timeout to use for exponential backoff to a peer after a receiver not ready error. (Default: 0 [random])");
	fi_param_define(&rxr_prov, "efa_cq_read_size", FI_PARAM_SIZE_T,
			"Set the number of EFA completion entries to read for one loop for one iteration of the progress engine. (Default: 50)");
	fi_param_define(&rxr_prov, "shm_cq_read_size", FI_PARAM_SIZE_T,
			"Set the number of SHM completion entries to read for one loop for one iteration of the progress engine. (Default: 50)");
	fi_param_define(&rxr_prov, "inter_max_medium_message_size", FI_PARAM_INT,
			"The maximum message size for inter EFA medium message protocol (Default 65536).");
	fi_param_define(&rxr_prov, "inter_min_read_message_size", FI_PARAM_INT,
			"The minimum message size in bytes for inter EFA read message protocol. If instance support RDMA read, messages whose size is larger than this value will be sent by read message protocol (Default 1048576).");
	fi_param_define(&rxr_prov, "inter_max_gdrcopy_message_size", FI_PARAM_INT,
			"The maximum message size to use gdrcopy. If instance support gdrcopy, messages whose size is smaller than this value will be sent by eager/longcts protocol (Default 32768).");
	fi_param_define(&rxr_prov, "inter_min_read_write_size", FI_PARAM_INT,
			"The mimimum message size for inter EFA write to use read write protocol. If firmware support RDMA read, and FI_EFA_USE_DEVICE_RDMA is 1, write requests whose size is larger than this value will use the read write protocol (Default 65536).");
	fi_param_define(&rxr_prov, "inter_read_segment_size", FI_PARAM_INT,
			"Calls to RDMA read is segmented using this value.");
	fi_param_define(&rxr_prov, "fork_safe", FI_PARAM_BOOL,
			"Enables fork support and disables internal usage of huge pages. Has no effect on kernels which set copy-on-fork for registered pages, generally 5.13 and later. (Default: false)");
	fi_param_define(&rxr_prov, "runt_size", FI_PARAM_INT,
			"The maximum number of bytes that will be eagerly sent by inflight messages uses runting read message protocol (Default 307200).");
}

/**
 * @brief Initialize the variables in rxr_env.
 * This function uses the g_device_list initialized in
 * efa_device_list_initialize(), thus it must be called
 * after efa_device_list_initialize().
 */
void rxr_env_initialize()
{
	uint32_t vendor_part_id = g_device_list[0].ibv_attr.vendor_part_id;

	if (vendor_part_id == 0xefa0 || vendor_part_id == 0xefa1) {
		rxr_env.use_device_rdma = 0;
	} else {
		rxr_env.use_device_rdma = 1;
		if (!(g_device_list[0].efa_attr.device_caps & EFADV_DEVICE_ATTR_CAPS_RDMA_READ)) {
			FI_WARN(&rxr_prov, FI_LOG_FABRIC,
				"EFA device with vendor_part_id 0x%x has no rdma-read capability.\n", vendor_part_id);
			rxr_env.use_device_rdma = 0;
		}
	}
	rxr_env_define();
	rxr_env_param_get();
}
