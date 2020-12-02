/*
 * Copyright (c) 2018 Intel Corporation. All rights reserved.
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

#ifndef _HOOK_PERF_H_
#define _HOOK_PERF_H_

#include "ofi_hook.h"
#include "ofi.h"
#include "ofi_perf.h"


struct perf_fabric {
	struct hook_fabric fabric_hook;
	struct ofi_perfset perf_set;
};

int hook_perf_destroy(struct fid *fabric);


#define HOOK_FOREACH(DECL)		\
	DECL(perf_recv),		\
	DECL(perf_recvv),		\
	DECL(perf_recvmsg),		\
	DECL(perf_send),		\
	DECL(perf_sendv),		\
	DECL(perf_sendmsg),		\
	DECL(perf_inject),		\
	DECL(perf_senddata),		\
	DECL(perf_injectdata),		\
	DECL(perf_read),		\
	DECL(perf_readv),		\
	DECL(perf_readmsg),		\
	DECL(perf_write),		\
	DECL(perf_writev),		\
	DECL(perf_writemsg),		\
	DECL(perf_inject_write),	\
	DECL(perf_writedata),		\
	DECL(perf_inject_writedata),	\
	DECL(perf_trecv),		\
	DECL(perf_trecvv),		\
	DECL(perf_trecvmsg),		\
	DECL(perf_tsend),		\
	DECL(perf_tsendv),		\
	DECL(perf_tsendmsg),		\
	DECL(perf_tinject),		\
	DECL(perf_tsenddata),		\
	DECL(perf_tinjectdata),		\
	DECL(perf_cq_read),		\
	DECL(perf_cq_readfrom),		\
	DECL(perf_cq_readerr),		\
	DECL(perf_cq_sread),		\
	DECL(perf_cq_sreadfrom),	\
	DECL(perf_cq_signal),		\
	DECL(perf_cntr_read),		\
	DECL(perf_cntr_readerr),	\
	DECL(perf_cntr_add),		\
	DECL(perf_cntr_set),		\
	DECL(perf_cntr_wait),		\
	DECL(perf_cntr_adderr),		\
	DECL(perf_cntr_seterr),		\
	DECL(perf_size)

enum perf_counters {
	HOOK_FOREACH(OFI_ENUM_VAL)
};

extern const char *perf_counters_str[];

#endif /* _HOOK_PERF_H_ */
