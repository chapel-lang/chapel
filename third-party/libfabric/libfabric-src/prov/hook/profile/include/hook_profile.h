/*
 * Copyright (c) 2018-2023 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL); Version 2, available from the file
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

#ifndef _HOOK_PROFILE_H_
#define _HOOK_PROFILE_H_

#include "ofi_hook.h"
#include "ofi.h"

#define PROF_IGNORE_SIZE  0

#define PROFILE_APIS(DECL)  \
	DECL(prof_recv),  \
	DECL(prof_recvv), \
	DECL(prof_recvmsg), \
	DECL(prof_trecv), \
	DECL(prof_trecvv), \
	DECL(prof_trecvmsg), \
	DECL(prof_send), \
	DECL(prof_sendv), \
	DECL(prof_sendmsg), \
	DECL(prof_inject),  \
	DECL(prof_senddata),  \
	DECL(prof_injectdata),  \
	DECL(prof_tsend),  \
	DECL(prof_tsendv),  \
	DECL(prof_tsendmsg),  \
	DECL(prof_tinject),	 \
	DECL(prof_tsenddata),  \
	DECL(prof_tinjectdata),  \
	DECL(prof_read),  \
	DECL(prof_readv),  \
	DECL(prof_readmsg),  \
	DECL(prof_write),  \
	DECL(prof_writev),  \
	DECL(prof_writemsg),  \
	DECL(prof_inject_write),  \
	DECL(prof_writedata),  \
	DECL(prof_inject_writedata),  \
	DECL(prof_mr_reg),  \
	DECL(prof_mr_regv),  \
	DECL(prof_mr_regattr),  \
	DECL(prof_cq_read),  \
	DECL(prof_cq_readfrom),	 \
	DECL(prof_cq_readerr),  \
	DECL(prof_cq_sread),  \
	DECL(prof_cq_sreadfrom),  \
	DECL(prof_cq_ctx),  \
	DECL(prof_cq_msg_tx),  \
	DECL(prof_cq_msg_rx),  \
	DECL(prof_cq_data_tx),	\
	DECL(prof_cq_data_rx),	\
	DECL(prof_cq_tagged_tx),  \
	DECL(prof_cq_tagged_rx),  \
	DECL(prof_api_size)

enum profile_api_counters {
	PROFILE_APIS(OFI_ENUM_VAL)
};

#define PROF_RX_API_START  prof_recv
#define PROF_RX_API_END prof_trecvmsg
#define PROF_TX_API_START  prof_send
#define PROF_TX_API_END prof_tinjectdata
#define PROF_RD_API_START  prof_read
#define PROF_RD_API_END prof_readmsg
#define PROF_WR_API_START  prof_write
#define PROF_WR_API_END prof_inject_writedata
#define PROF_CQ_API_START  prof_cq_msg_tx
#define PROF_CQ_API_END prof_cq_tagged_rx
#define PROF_MR_API_START  prof_mr_reg
#define PROF_MR_API_END prof_mr_regattr

enum prof_size_bucket {
	PROF_SIZE_0_64 = 0,
	PROF_SIZE_64_512,
	PROF_SIZE_512_1K,
	PROF_SIZE_1K_4K,
	PROF_SIZE_4K_64K,
	PROF_SIZE_64K_256K,
	PROF_SIZE_256K_1M,
	PROF_SIZE_1M_4M,
	PROF_SIZE_4M_UP,
	PROF_SIZE_MAX
};

struct profile_data {
	uint64_t count[PROF_SIZE_MAX];
	uint64_t sum[PROF_SIZE_MAX];
};

struct profile_context {
	const struct fi_provider *hprov;
	struct profile_data data[prof_api_size];
};

struct profile_fabric {
	struct hook_fabric fabric_hook;
	struct profile_context prof_ctx;
};

void prof_report(const struct fi_provider *hprov,  struct profile_data *data);

#endif /* _HOOK_PROFILE_H_ */
