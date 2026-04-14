/*
 * Copyright (c) 2018-2024 GigaIO, Inc. All Rights Reserved.
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
#include "lpp.h"
#include "lpp_completion.h"

#define VALID_COMP_FLAGS (FI_SEND | FI_RECV | FI_RMA | FI_ATOMIC | FI_MSG     \
			 | FI_TAGGED | FI_MULTICAST | FI_READ | FI_WRITE       \
			 | FI_REMOTE_READ | FI_REMOTE_WRITE | FI_REMOTE_CQ_DATA \
			 | FI_MULTI_RECV | FI_OS_BYPASS)

void lpp_completion_transmit(struct lpp_ep *lpp_epp, struct lpp_cntr *cntrp,
			     uint64_t flags, uint64_t data, void *context, int status)
{
	struct lpp_cq *cqp = lpp_epp->cq_transmit;
	struct klpp_cq_tagged_entry entry = { 0 };
	struct klpp_cq_err_entry err_entry = { 0 };
	uint64_t comp_flags = flags & VALID_COMP_FLAGS;

	if ((flags & FI_COMPLETION) && status == 0) {
		entry.generic.op_context = context;
		entry.generic.flags = comp_flags;
		entry.generic.data = data;
		lpp_cq_enqueue_entry(cqp, &entry);
	} else if (status != 0) {
		err_entry.op_context = context;
		err_entry.flags = comp_flags;
		err_entry.data = data;
		err_entry.err = lpp_translate_errno(status);
		err_entry.prov_errno = status;
		lpp_cq_enqueue_err_entry(cqp, &err_entry);
	}

	if (cntrp != NULL) {
		if (status == 0)
			lpp_fi_cntr_add(&cntrp->cntr_fid, 1);
		else
			lpp_fi_cntr_adderr(&cntrp->cntr_fid, 1);
	}
}

void lpp_completion_multirecv(struct lpp_ep *lpp_epp, void *context)
{
	struct lpp_cq *cqp = lpp_epp->cq_recv;
	struct klpp_cq_tagged_entry entry = { 0 };

	entry.generic.op_context = context;
	entry.generic.flags = FI_MULTI_RECV;
	lpp_cq_enqueue_entry(cqp, &entry);
}

void lpp_completion_recv(struct lpp_ep *lpp_epp, uint64_t flags, uint64_t data,
			 void *context, uint64_t tag, void *buf, size_t len,
			 size_t olen, fi_addr_t src_addr, int status)
{
	struct lpp_cq *cqp = lpp_epp->cq_recv;
	struct lpp_cntr *cntrp = lpp_epp->cntr_recv;
	struct klpp_cq_tagged_entry entry = { 0 };
	struct klpp_cq_err_entry err_entry = { 0 };
	uint64_t comp_flags = flags & VALID_COMP_FLAGS;

	if (status)
		FI_WARN(&lpp_prov, FI_LOG_CQ,
			"RECV error code: %d (buf: %p, len: %ld)\n", status, buf, len);

	if (status == 0 && olen != 0) {
		status = EMSGSIZE;
		err_entry.err = FI_ETRUNC;
		err_entry.prov_errno = status;
	} else if (status != 0) {
		err_entry.prov_errno = status;
		err_entry.err = lpp_translate_errno(status);
	}

	if (flags & FI_COMPLETION && status == 0) {
		entry.generic.op_context = context;
		entry.generic.flags = comp_flags;
		entry.generic.len = len;
		entry.generic.buf = buf;
		entry.generic.data = data;
		entry.generic.tag = tag;
		entry.lpp.src_addr.raw = src_addr;
		lpp_cq_enqueue_entry(cqp, &entry);
	} else if (status != 0) {
		err_entry.op_context = context;
		err_entry.flags = comp_flags;
		err_entry.len = len;
		err_entry.buf = buf;
		err_entry.data = data;
		err_entry.tag = tag;
		err_entry.olen = olen;
		lpp_cq_enqueue_err_entry(cqp, &err_entry);
	}

	if (cntrp != NULL) {
		if (status == 0)
			lpp_fi_cntr_add(&cntrp->cntr_fid, 1);
		else
			lpp_fi_cntr_adderr(&cntrp->cntr_fid, 1);
	}
}
