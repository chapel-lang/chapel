/*
 * Copyright (c) 2014 Intel Corporation. All rights reserved.
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

#ifndef FI_TRIGGER_H
#define FI_TRIGGER_H

#include <rdma/fabric.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_tagged.h>
#include <rdma/fi_atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

enum fi_trigger_event {
	FI_TRIGGER_THRESHOLD,
	FI_TRIGGER_XPU,
};

enum fi_op_type {
	FI_OP_RECV,
	FI_OP_SEND,
	FI_OP_TRECV,
	FI_OP_TSEND,
	FI_OP_READ,
	FI_OP_WRITE,
	FI_OP_ATOMIC,
	FI_OP_FETCH_ATOMIC,
	FI_OP_COMPARE_ATOMIC,
	FI_OP_CNTR_SET,
	FI_OP_CNTR_ADD
};

struct fi_trigger_threshold {
	struct fid_cntr		*cntr;
	size_t			threshold;
};

struct fi_trigger_var {
	enum fi_datatype	datatype;
	int			count;
	void			*addr;
	union {
		uint8_t		val8;
		uint16_t	val16;
		uint32_t	val32;
		uint64_t	val64;
		uint8_t		*data;
	} value;
};

struct fi_trigger_xpu {
	int			count;
	enum fi_hmem_iface	iface;
	union {
		uint64_t	reserved;
		int		cuda;
		int		ze;
	} device;
	struct fi_trigger_var	*var;
};

struct fi_op_msg {
	struct fid_ep		*ep;
	struct fi_msg		msg;
	uint64_t		flags;
};

struct fi_op_tagged {
	struct fid_ep		*ep;
	struct fi_msg_tagged	msg;
	uint64_t		flags;
};

struct fi_op_rma {
	struct fid_ep		*ep;
	struct fi_msg_rma	msg;
	uint64_t		flags;
};

struct fi_op_atomic {
	struct fid_ep		*ep;
	struct fi_msg_atomic	msg;
	uint64_t		flags;
};

struct fi_op_fetch_atomic {
	struct fid_ep		*ep;
	struct fi_msg_atomic	msg;
	struct fi_msg_fetch	fetch;
	uint64_t		flags;
};

struct fi_op_compare_atomic {
	struct fid_ep		*ep;
	struct fi_msg_atomic	msg;
	struct fi_msg_fetch	fetch;
	struct fi_msg_compare	compare;
	uint64_t		flags;
};

struct fi_op_cntr {
	struct fid_cntr		*cntr;
	uint64_t		value;
};

#ifdef FABRIC_DIRECT
#include <rdma/fi_direct_trigger.h>
#endif

#ifndef FABRIC_DIRECT_TRIGGER

/* Size must match struct fi_context */
struct fi_triggered_context {
	enum fi_trigger_event			event_type;
	union {
		struct fi_trigger_threshold	threshold;
		struct fi_trigger_xpu		xpu;
		void				*internal[3];
	} trigger;
};

/* Size must match struct fi_context2 */
struct fi_triggered_context2 {
	enum fi_trigger_event			event_type;
	union {
		struct fi_trigger_threshold	threshold;
		struct fi_trigger_xpu		xpu;
		void				*internal[7];
	} trigger;
};

struct fi_deferred_work {
	struct fi_context2			context;

	uint64_t				threshold;
	struct fid_cntr				*triggering_cntr;
	struct fid_cntr				*completion_cntr;

	enum fi_op_type				op_type;

	union {
		struct fi_op_msg		*msg;
		struct fi_op_tagged		*tagged;
		struct fi_op_rma		*rma;
		struct fi_op_atomic		*atomic;
		struct fi_op_fetch_atomic	*fetch_atomic;
		struct fi_op_compare_atomic	*compare_atomic;
		struct fi_op_cntr		*cntr;
	} op;
};

#endif


#ifdef __cplusplus
}
#endif

#endif /* FI_TRIGGER_H */
