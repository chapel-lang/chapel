/*
 * Copyright (c) 2025 Philipp A. Friese, Technical University of Munich
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

#ifndef _HOOK_MONITOR_H_
#define _HOOK_MONITOR_H_

#include "ofi_hook.h"
#include "ofi.h"

#define MON_IGNORE_SIZE  0
#define MON_TICK_MAX_DEFAULT 1024
#define MON_LINGER_DEFAULT 0
#define MON_BASEPATH_DEFAULT "/dev/shm/ofi"
#define MON_FILE_MODE_DEFAULT 0600
#define MON_DIR_MODE_DEFAULT 01700

// Note: keep in-sync with util/mon_sampler.c
#define MONITOR_APIS(DECL)  \
	DECL(mon_recv),  \
	DECL(mon_recvv), \
	DECL(mon_recvmsg), \
	DECL(mon_trecv), \
	DECL(mon_trecvv), \
	DECL(mon_trecvmsg), \
	DECL(mon_send), \
	DECL(mon_sendv), \
	DECL(mon_sendmsg), \
	DECL(mon_inject),  \
	DECL(mon_senddata),  \
	DECL(mon_injectdata),  \
	DECL(mon_tsend),  \
	DECL(mon_tsendv),  \
	DECL(mon_tsendmsg),  \
	DECL(mon_tinject),	 \
	DECL(mon_tsenddata),  \
	DECL(mon_tinjectdata),  \
	DECL(mon_read),  \
	DECL(mon_readv),  \
	DECL(mon_readmsg),  \
	DECL(mon_write),  \
	DECL(mon_writev),  \
	DECL(mon_writemsg),  \
	DECL(mon_inject_write),  \
	DECL(mon_writedata),  \
	DECL(mon_inject_writedata),  \
	DECL(mon_mr_reg),  \
	DECL(mon_mr_regv),  \
	DECL(mon_mr_regattr),  \
	DECL(mon_cq_read),  \
	DECL(mon_cq_readfrom),	 \
	DECL(mon_cq_readerr),  \
	DECL(mon_cq_sread),  \
	DECL(mon_cq_sreadfrom),  \
	DECL(mon_cq_ctx),  \
	DECL(mon_cq_msg_tx),  \
	DECL(mon_cq_msg_rx),  \
	DECL(mon_cq_data_tx),	\
	DECL(mon_cq_data_rx),	\
	DECL(mon_cq_tagged_tx),  \
	DECL(mon_cq_tagged_rx),    \
	DECL(mon_api_size)

enum mon_api_counters {
	MONITOR_APIS(OFI_ENUM_VAL)
};

enum mon_size_bucket {
	MON_SIZE_0_64 = 0,
	MON_SIZE_64_512,
	MON_SIZE_512_1K,
	MON_SIZE_1K_4K,
	MON_SIZE_4K_64K,
	MON_SIZE_64K_256K,
	MON_SIZE_256K_1M,
	MON_SIZE_1M_4M,
	MON_SIZE_4M_UP,
	MON_SIZE_MAX
};

struct monitor_data {
	uint64_t count[MON_SIZE_MAX];
	uint64_t sum[MON_SIZE_MAX];
};

struct monitor_mapped_data {
	struct monitor_data data[mon_api_size];

	/* Synchronisation Flag
	 * bit 0    : data flush request
	 * bit 1    : termination finished
	 * remainder: reserved
	 */
	_Atomic uint8_t flags;
};

struct monitor_context {
	const struct fi_provider *hprov;

	// internal counter data
	struct monitor_data data[mon_api_size];

	// current number of hooked API calls
	unsigned int tick;

	// external counter data, shared via communication file
	struct monitor_mapped_data* share;

	// name of communication file
	char shm_name[PATH_MAX];
};

struct monitor_fabric {
	struct hook_fabric fabric_hook;
	struct monitor_context mon_ctx;
};

struct monitor_environment {
	int linger;
	unsigned int tick_max;
	int file_mode;
	int dir_mode;
	char basepath[PATH_MAX];
};

#endif /* _HOOK_MONITOR_H_ */
