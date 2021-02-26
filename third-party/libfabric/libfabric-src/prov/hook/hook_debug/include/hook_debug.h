/*
 * Copyright (c) 2019 Intel Corporation. All rights reserved.
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

#ifndef _HOOK_DEBUG_H_
#define _HOOK_DEBUG_H_

#include "ofi_hook.h"
#include "ofi.h"

#define HOOK_DEBUG_EAGAIN_LOG 10000000
#define HOOK_DEBUG_EQ_EVENT_MAX (FI_JOIN_COMPLETE + 1)

extern struct hook_prov_ctx hook_debug_ctx;

struct hook_debug_config {
	uint64_t trace_exit : 1;
	uint64_t trace_cq_entry : 1;
	uint64_t track_sends : 1;
	uint64_t track_recvs : 1;
};

struct hook_debug_eq {
	struct hook_eq hook_eq;
	ofi_atomic64_t event_cntr[HOOK_DEBUG_EQ_EVENT_MAX];
	size_t eagain_count;
};

struct hook_debug_cq {
	struct hook_cq hook_cq;
	enum fi_cq_format format;
	size_t entry_size;
	size_t eagain_count;
};

struct hook_debug_txrx_entry {
	uint64_t 		magic;
	struct hook_debug_ep 	*ep;
	uint64_t 		op_flags;
	void 			*context;
};

struct hook_debug_ep {
	struct hook_ep hook_ep;
	uint64_t tx_op_flags;
	uint64_t rx_op_flags;
	struct ofi_bufpool *tx_pool;
	struct ofi_bufpool *rx_pool;
	size_t tx_outs;
	size_t rx_outs;
	size_t tx_eagain_count;
	size_t rx_eagain_count;
};

#endif /* _HOOK_DEBUG_H_ */
