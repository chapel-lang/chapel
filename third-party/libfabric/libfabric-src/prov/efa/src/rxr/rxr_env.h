/*
 * Copyright (c) 2022 Amazon.com, Inc. or its affiliates.
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

#ifndef _RXR_ENV_H
#define _RXR_ENV_H

struct rxr_env {
	int tx_min_credits;
	int tx_queue_size;
	int use_device_rdma;
	int use_zcpy_rx;
	int set_cuda_sync_memops;
	int zcpy_rx_seed;
	int enable_shm_transfer;
	int shm_av_size;
	int shm_max_medium_size;
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
	/* If first attempt to send a packet failed,
	 * this value controls how many times firmware
	 * retries the send before it report an RNR error
	 * (via rdma-core error cq entry).
	 *
	 * The valid number is from
	 *      0 (no retry)
	 * to
	 *      EFA_RNR_INFINITY_RETRY (retry infinitely)
	 */
	int rnr_retry;
};

extern struct rxr_env rxr_env;

void rxr_env_initialize();

#endif
